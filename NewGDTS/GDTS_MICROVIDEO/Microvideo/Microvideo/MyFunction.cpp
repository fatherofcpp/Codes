#include "stdafx.h"
#include "MyFunction.h"

HMODULE   GetCurrentModule() 
{
	MEMORY_BASIC_INFORMATION   mbi; 
	static   int   dummy; 
	VirtualQuery(   &dummy,   &mbi,   sizeof(mbi)   ); 

	return   reinterpret_cast <HMODULE> (mbi.AllocationBase); 
}

void MyInitLog(char*TmpNmae)
{
	CString thisDir="";
	CString logName;
	CString tmpName;

	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	g_CurrentDir = thisDir;

	tmpName.Format("\\%s_%d.log",TmpNmae,g_devId);
	logName = thisDir+tmpName;

	HANDLE test=GetModuleHandle(NULL);
	CString haha="";
	haha.Format("³ÌÐò×°ÔØµØÖ·£º0x%08x",test);

	m_Log.InitLog(logName);//("c:\\log.log");
	m_Log.Add(haha);
}

