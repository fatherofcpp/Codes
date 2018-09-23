

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "h3cIPClib.h"


 IMOS_MW_INITIATE			IMOS_MW_Initiate=NULL; 

 IMOS_MW_LOGIN			IMOS_MW_Login =NULL; 
 IMOS_MW_STARTMEDIASTREAM			IMOS_MW_StartMediaStream=NULL;
 IMOS_MW_SETDISPLAYPARAM			IMOS_MW_SetDisplayParam=NULL;
 IMOS_MW_STARTPLAY			IMOS_MW_StartPlay=NULL; 
 IMOS_MW_SETDECODEDVIDEODATACALLBACK			IMOS_MW_SetDecodedVideoDataCallback=NULL;

 IMOS_MW_STOPPLAY			IMOS_MW_StopPlay=NULL; 
 IMOS_MW_STOPMEDIASTREAM			IMOS_MW_StopMediaStream=NULL; 
 IMOS_MW_LOGOUT			IMOS_MW_Logout=NULL; 
 IMOS_MW_CLEANUP			IMOS_MW_Cleanup=NULL; 

int loadH3cIPCLib()
{
	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\ipc\\dll\\");

	hProcessModule	=  LoadLibraryA("imos_mw_sdk.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload imos_mw_sdk.dll failed\n");
		return -1;
	}
	//2
	IMOS_MW_Initiate=(IMOS_MW_INITIATE)GetProcAddress(hProcessModule,"IMOS_MW_Initiate");
	if(IMOS_MW_Initiate == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_Initiate failed\n");
		return -1;
	}
	IMOS_MW_Login=(IMOS_MW_LOGIN)GetProcAddress(hProcessModule,"IMOS_MW_Login");
	if(IMOS_MW_Login == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_Login failed\n");
		return -1;
	}

	IMOS_MW_StartMediaStream=(IMOS_MW_STARTMEDIASTREAM)GetProcAddress(hProcessModule,"IMOS_MW_StartMediaStream");
	if(IMOS_MW_StartMediaStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_StartMediaStream failed\n");
		return -1;
	}
	IMOS_MW_SetDisplayParam=(IMOS_MW_SETDISPLAYPARAM)GetProcAddress(hProcessModule,"IMOS_MW_SetDisplayParam");
	if(IMOS_MW_SetDisplayParam == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_SetDisplayParam failed\n");
		return -1;
	}
	IMOS_MW_StartPlay=(IMOS_MW_STARTPLAY)GetProcAddress(hProcessModule,"IMOS_MW_StartPlay");
	if(IMOS_MW_StartPlay == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_StartPlay failed\n");
		return -1;
	}
	IMOS_MW_SetDecodedVideoDataCallback=(IMOS_MW_SETDECODEDVIDEODATACALLBACK)GetProcAddress(hProcessModule,"IMOS_MW_SetDecodedVideoDataCallback");
	if(IMOS_MW_SetDecodedVideoDataCallback == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_SetDecodedVideoDataCallback failed\n");
		return -1;
	}

	IMOS_MW_StopPlay=(IMOS_MW_STOPPLAY)GetProcAddress(hProcessModule,"IMOS_MW_StopPlay");
	if(IMOS_MW_StopPlay == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_StopPlay failed\n");
		return -1;
	}
	IMOS_MW_StopMediaStream=(IMOS_MW_STOPMEDIASTREAM)GetProcAddress(hProcessModule,"IMOS_MW_StopMediaStream");
	if(IMOS_MW_StopMediaStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_StopMediaStream failed\n");
		return -1;
	}
	IMOS_MW_Logout=(IMOS_MW_LOGOUT)GetProcAddress(hProcessModule,"IMOS_MW_Logout");
	if(IMOS_MW_Logout == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_Logout failed\n");
		return -1;
	}
	IMOS_MW_Cleanup=(IMOS_MW_CLEANUP)GetProcAddress(hProcessModule,"IMOS_MW_Cleanup");
	if(IMOS_MW_Cleanup == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress IMOS_MW_Cleanup failed\n");
		return -1;
	}
	return 0;

}

