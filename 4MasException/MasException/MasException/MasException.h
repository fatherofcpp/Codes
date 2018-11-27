#pragma once

#ifdef MASEXCEPTION_EXPORTS
#define MASEXCEPTION_EXPORTS_API(x)			extern "C" __declspec(dllexport) x
#else
#define MASEXCEPTION_EXPORTS_API(x)			extern "C" __declspec(dllimport) x
#endif

//紧急处理函数
typedef void (CALLBACK *PUrgencyHandling)(PVOID);
typedef INT_PTR (*PMessageBox)(const wchar_t* pstrText, const wchar_t* pstrCaption, UINT, BOOL, BOOL*);

/*
*功能：		捕获异常
*功能描述：捕获异常
*参数说明：
*			pUrgencyHandling:	紧急处理函数
*			pMasMessageBox:		弹出信息函数
*			pParam:				描述变量
*			pErrorText:			错误信息
*			pErrorCaption:		错误标题
*			uMessageBoxType:	信息窗口类型（一般错误信息框）
*返回值：	无
*/

MASEXCEPTION_EXPORTS_API(void) HE_SetUnhandledException(PUrgencyHandling pUrgencyHandling, PMessageBox pMasMessageBox, PVOID pParam, const wchar_t* pErrorText, const wchar_t* pErrorCaption, UINT uMessageBoxType);
