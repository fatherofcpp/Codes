// MasException.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MasException.h"

typedef struct tagHandleExceptionInfo
{
	tagHandleExceptionInfo()
		:pUrgencyHandling(NULL)
		,pMasMessageBox(NULL)
		,pParam(NULL)
		,strErrorInfo(L"")
		,strErrorCaption(L"")
		,uMessageBoxType(0)
	{}
	PUrgencyHandling	pUrgencyHandling;
	PMessageBox		pMasMessageBox;
	PVOID				pParam;
	std::wstring		strErrorInfo;
	std::wstring		strErrorCaption;
	UINT				uMessageBoxType;
}HANDLEEXCEPTIONINFO, *PHANDLEEXCEPTIONINFO;

HANDLEEXCEPTIONINFO& GetHandleExceptionInfo()
{
	static HANDLEEXCEPTIONINFO sHandleExceptionInfo;
	return sHandleExceptionInfo;
}

//EXCEPTION_EXECUTE_HANDLER equ 1 表示我已经处理了异常,可以优雅地结束了  
//EXCEPTION_CONTINUE_SEARCH equ 0 表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束  
//EXCEPTION_CONTINUE_EXECUTION equ -1 表示错误已经被修复,请从异常发生处继续执行
LONG WINAPI HandleException(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	std::wstring strErrorInfo(L"");

	WCHAR buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	_swprintf(buffer, L"%08X ", ExceptionInfo->ExceptionRecord->ExceptionAddress);
	_swprintf(buffer + wcslen(buffer), L"eax:%08X ebx:%08X ecx:%08X edx:%08X", 
										ExceptionInfo->ContextRecord->Eax,   
										ExceptionInfo->ContextRecord->Ebx,
										ExceptionInfo->ContextRecord->Ecx,   
										ExceptionInfo->ContextRecord->Edx);

	unsigned int uIndex = GetHandleExceptionInfo().strErrorInfo.find(L"%s");

	strErrorInfo = GetHandleExceptionInfo().strErrorInfo.substr(0, uIndex) + buffer + GetHandleExceptionInfo().strErrorInfo.substr(uIndex + 2);

	//在异常退出之前紧急处理数据
	if (GetHandleExceptionInfo().pUrgencyHandling != NULL)(*GetHandleExceptionInfo().pUrgencyHandling)(GetHandleExceptionInfo().pParam);

	HANDLE lhDumpFile = CreateFile(_T("DumpFile.dmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
	if (MAS_IS_VALID_HANDLE(lhDumpFile))
	{
		MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
		loExceptionInfo.ExceptionPointers = ExceptionInfo;
		loExceptionInfo.ThreadId = GetCurrentThreadId();
		loExceptionInfo.ClientPointers = TRUE;
		::MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
		CloseHandle(lhDumpFile);
	}

	if (GetHandleExceptionInfo().pMasMessageBox != NULL)
	{
		(*GetHandleExceptionInfo().pMasMessageBox)(strErrorInfo.c_str(), GetHandleExceptionInfo().strErrorCaption.c_str(), GetHandleExceptionInfo().uMessageBoxType, FALSE, NULL);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

#ifndef _M_IX86

       #error "The following code only works for x86!"
#endif

void DisableSetUnhandledExceptionFilter()
{
	if (void *addr = (void*)GetProcAddress(LoadLibrary(_T("kernel32.dll")), "SetUnhandledExceptionFilter"))
    {
		unsigned char code[16];
		int size = 0;
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD dwOldFlag, dwTempFlag;
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}

MASEXCEPTION_EXPORTS_API(void) HE_SetUnhandledException(PUrgencyHandling pUrgencyHandling, PMessageBox pMasMessageBox, PVOID pParam, const wchar_t* pErrorText, const wchar_t* pErrorCaption, UINT uMessageBoxType)
{
	GetHandleExceptionInfo().pUrgencyHandling	= pUrgencyHandling;
	GetHandleExceptionInfo().pMasMessageBox		= pMasMessageBox;
	GetHandleExceptionInfo().pParam				= pParam;
	GetHandleExceptionInfo().strErrorInfo		= (pErrorText != NULL?pErrorText:L"");
	GetHandleExceptionInfo().strErrorCaption	= (pErrorCaption != NULL?pErrorCaption:L"");
	GetHandleExceptionInfo().uMessageBoxType	= uMessageBoxType;

	::SetUnhandledExceptionFilter(&HandleException); 
	DisableSetUnhandledExceptionFilter();
}



