

#include "stdafx.h"

#include "HuaDingLib.h"

#if 1

 VMS_INIT VMS_Init =NULL;//
 VMS_DESTORY VMS_Destory =NULL;//
 VMS_REGCALLBACK VMS_RegCallBack =NULL;//
 VMS_LOGIN VMS_Login =NULL;//
 VMS_LOGOUT VMS_Logout =NULL;//
 VMS_ERRCHSTRING VMS_ErrCHString =NULL;//
 VMS_RSSTARTREPLY VMS_RSStartReply =NULL;//


 VMS_STARTMONITOR VMS_StartMonitor =NULL;
 VMS_STOPMONITOR VMS_StopMonitor =NULL;

 VMS_RSSTOPREPLY VMS_RSStopReply =NULL;

int loadHuaDingLib()
{
	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\vms\\lib\\");
	hProcessModule	=  LoadLibraryA("NVR_SDK.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload NVR_SDK.dll failed\n");
		return -1;
	}
	
	VMS_Init=(VMS_INIT)GetProcAddress(hProcessModule,"_VMS_Init@0");
	if(VMS_Init == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_Init failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_Destory=(VMS_DESTORY)GetProcAddress(hProcessModule,"_VMS_Destory@0");
	if(VMS_Destory == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_Destory failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_RegCallBack=(VMS_REGCALLBACK)GetProcAddress(hProcessModule,"_VMS_RegCallBack@8");
	if(VMS_Init == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_RegCallBack failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_Login=(VMS_LOGIN)GetProcAddress(hProcessModule,"_VMS_Login@20");
	if(VMS_Login == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_Login failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_Logout=(VMS_LOGOUT)GetProcAddress(hProcessModule,"_VMS_Logout@0");
	if(VMS_Logout == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_Logout failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_ErrCHString=(VMS_ERRCHSTRING)GetProcAddress(hProcessModule,"_VMS_ErrCHString@12");
	if(VMS_ErrCHString == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_ErrCHString failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_RSStartReply=(VMS_RSSTARTREPLY)GetProcAddress(hProcessModule,"_VMS_RSStartReply@8");
	if(VMS_RSStartReply == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_RSStartReply failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_StartMonitor=(VMS_STARTMONITOR)GetProcAddress(hProcessModule,"_VMS_StartMonitor@32");
	if(VMS_StartMonitor == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_StartMonitor failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_StopMonitor=(VMS_STOPMONITOR)GetProcAddress(hProcessModule,"_VMS_StopMonitor@20");
	if(VMS_StopMonitor == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_StopMonitor failed[%d]\n",GetLastError());
		return -1;
	}

	VMS_RSStopReply=(VMS_RSSTOPREPLY)GetProcAddress(hProcessModule,"_VMS_RSStopReply@4");
	if(VMS_RSStopReply == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress VMS_RSStopReply failed[%d]\n",GetLastError());
		return -1;
	}
	
	return 0;

}

#endif