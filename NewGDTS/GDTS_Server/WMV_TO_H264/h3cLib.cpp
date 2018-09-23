

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "h3cLib.h"



IMOS_INITIATE			IMOS_Initiate			= NULL; 
IMOS_CLEANUP			IMOS_CleanUp			= NULL;
IMOS_ENCRYPT			IMOS_Encrypt			= NULL;
IMOS_LOGINEX			IMOS_LoginEx			= NULL; 
IMOS_LOGOUTEX			IMOS_LogoutEx			= NULL;
IMOS_AUTOKEEPALIVE		IMOS_AutoKeepAlive		= NULL;
IMOS_STARTPLAYER		IMOS_StartPlayer		= NULL;
IMOS_GETCHANNELCODE		IMOS_GetChannelCode		= NULL;
IMOS_FREECHANNELCODE	IMOS_FreeChannelCode	= NULL;
IMOS_STARTMONITOR		IMOS_StartMonitor		= NULL;
IMOS_STOPMONITOR		IMOS_StopMonitor		= NULL;
IMOS_SETPARSEVIDEODATACB IMOS_SetParseVideoDataCB		= NULL;
IMOS_SETDECODEVIDEODATACB IMOS_SetDecodeVideoDataCB		= NULL;

int loadH3cLib()
{
	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\imos_sdk_win\\redist\\");
	hProcessModule	=  LoadLibraryA("imos_sdk.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload imos_sdk.dll failed\n");
		return -1;
	}
	//2

	IMOS_Initiate=(IMOS_INITIATE)GetProcAddress(hProcessModule,"IMOS_Initiate");
	if(IMOS_Initiate == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_Initiate failed\n");
		return -1;
	}

	IMOS_CleanUp=(IMOS_CLEANUP)GetProcAddress(hProcessModule,"IMOS_CleanUp");
	if(IMOS_CleanUp == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_CleanUp failed\n");
		return -1;
	}
	IMOS_Encrypt=(IMOS_ENCRYPT)GetProcAddress(hProcessModule,"IMOS_Encrypt");
	if(IMOS_Encrypt == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_Encrypt failed\n");
		return -1;
	}
	IMOS_LoginEx=(IMOS_LOGINEX)GetProcAddress(hProcessModule,"IMOS_LoginEx");
	if(IMOS_LoginEx == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_LoginEx failed\n");
		return -1;
	}
	IMOS_LogoutEx=(IMOS_LOGOUTEX)GetProcAddress(hProcessModule,"IMOS_LogoutEx");
	if(IMOS_LogoutEx == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_LogoutEx failed\n");
		return -1;
	}
	IMOS_AutoKeepAlive=(IMOS_AUTOKEEPALIVE)GetProcAddress(hProcessModule,"IMOS_AutoKeepAlive");
	if(IMOS_AutoKeepAlive == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_AutoKeepAlive failed\n");
		return -1;
	}
	IMOS_StartPlayer=(IMOS_STARTPLAYER)GetProcAddress(hProcessModule,"IMOS_StartPlayer");
	if(IMOS_StartPlayer == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_StartPlayer failed\n");
		return -1;
	}
	IMOS_StartMonitor=(IMOS_STARTMONITOR)GetProcAddress(hProcessModule,"IMOS_StartMonitor");
	if(IMOS_StartMonitor == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_StartMonitor failed\n");
		return -1;
	}
	IMOS_StopMonitor=(IMOS_STOPMONITOR)GetProcAddress(hProcessModule,"IMOS_StopMonitor");
	if(IMOS_StopMonitor == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_StopMonitor failed\n");
		return -1;
	}


	hProcessModule	=  LoadLibraryA("xp_frame.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload xp_frame.dll failed\n");
		return -1;
	}

	IMOS_GetChannelCode=(IMOS_GETCHANNELCODE)GetProcAddress(hProcessModule,"IMOS_GetChannelCode");
	if(IMOS_GetChannelCode == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_GetChannelCode failed\n");
		return -1;
	}
	IMOS_FreeChannelCode=(IMOS_FREECHANNELCODE)GetProcAddress(hProcessModule,"IMOS_FreeChannelCode");
	if(IMOS_FreeChannelCode == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_FreeChannelCode failed\n");
		return -1;
	}
	IMOS_SetParseVideoDataCB=(IMOS_SETPARSEVIDEODATACB)GetProcAddress(hProcessModule,"IMOS_SetParseVideoDataCB");
	if(IMOS_SetParseVideoDataCB == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_SetParseVideoDataCB failed\n");
		return -1;
	}
	IMOS_SetDecodeVideoDataCB=(IMOS_SETDECODEVIDEODATACB)GetProcAddress(hProcessModule,"IMOS_SetDecodeVideoDataCB");
	if(IMOS_SetDecodeVideoDataCB == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_SetDecodeVideoDataCB failed\n");
		return -1;
	}
	return 0;

}

