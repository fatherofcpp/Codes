

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "rtspLib.h"

DSS_LIVE555_PULLINIT		dss_live555_pullinit	=NULL;
DSS_LIVE555_PULLSTART		dss_live555_pullstart	=NULL;
DSS_LIVE555_PULLEXIT		dss_live555_pullexit	=NULL;
DSS_LIVE555_PULLSTOP		dss_live555_pullstop	=NULL;

DSS_LIVE555_PUSHINIT		dss_live555_pushinit	=NULL;
DSS_LIVE555_PUSHSTART		dss_live555_pushstart	=NULL;
DSS_LIVE555_PUSHSTOP		dss_live555_pushstop	=NULL;
DSS_LIVE555_PUSHEXIT		dss_live555_pushexit	=NULL;

int loadRTSPLib(const char * dir)
{
	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\rtsp_live555\\");
	hProcessModule	=  LoadLibraryA("lib_rtsp.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload lib_rtsp.dll failed\n");
		return -1;
	}

	dss_live555_pullinit=(DSS_LIVE555_PULLINIT)GetProcAddress(hProcessModule,"dss_live555_pullinit");
	if(dss_live555_pullinit == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pullinit failed[%d]\n",GetLastError());
		return -1;
	}

	dss_live555_pullstart=(DSS_LIVE555_PULLSTART)GetProcAddress(hProcessModule,"dss_live555_pullstart");
	if(dss_live555_pullstart == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pullstart failed[%d]\n",GetLastError());
		return -1;
	}

	dss_live555_pullexit=(DSS_LIVE555_PULLEXIT)GetProcAddress(hProcessModule,"dss_live555_pullexit");
	if(dss_live555_pullexit == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pullexit failed[%d]\n",GetLastError());
		return -1;
	}

	dss_live555_pullstop=(DSS_LIVE555_PULLSTOP)GetProcAddress(hProcessModule,"dss_live555_pullstop");
	if(dss_live555_pullstop == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pullstop failed[%d]\n",GetLastError());
		return -1;
	}

	dss_live555_pushinit=(DSS_LIVE555_PUSHINIT)GetProcAddress(hProcessModule,"dss_live555_pushinit");
	if(dss_live555_pushinit == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pushinit failed[%d]\n",GetLastError());
		return -1;
	}

	dss_live555_pushstart=(DSS_LIVE555_PUSHSTART)GetProcAddress(hProcessModule,"dss_live555_pushstart");
	if(dss_live555_pushstart == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pushstart failed[%d]\n",GetLastError());
		return -1;
	}

	dss_live555_pushstop=(DSS_LIVE555_PUSHSTOP)GetProcAddress(hProcessModule,"dss_live555_pushstop");
	if(dss_live555_pushstop == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pushstop failed[%d]\n",GetLastError());
		return -1;
	}

	dss_live555_pushexit=(DSS_LIVE555_PUSHEXIT)GetProcAddress(hProcessModule,"dss_live555_pushexit");
	if(dss_live555_pushexit == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress dss_live555_pushexit failed[%d]\n",GetLastError());
		return -1;
	}

	return 0;
}

