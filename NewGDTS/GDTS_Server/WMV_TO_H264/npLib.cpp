/********************************************************************
	created:	2014/03/27
	filename: 	npLib.cpp
	file ext:	cpp
	author:		zhh
	
	purpose:	实现动态加载东方网力动态库，减少在程序启动后对所需库的依赖。
				目前实现动态加载的主要来自np_netsdk.dll及np_playsdk.dll
				两个文件
*********************************************************************/

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "npLib.h"


NP_NET_INIT				NP_NET_Init				=NULL;
NP_NET_CLEANUP			NP_NET_Cleanup			=NULL;
NP_NET_GetSDK_VERSION	NP_NET_GetSDK_Version	=NULL;
NP_NET_LOGIN			NP_NET_Login			=NULL;
NP_PLAY_CREATEPLAYER	NP_PLAY_CreatePlayer	=NULL;
NP_PLAY_PLAYCONTROL		NP_PLAY_PlayControl		=NULL;
NP_NET_STARTSTREAM		NP_NET_StartStream		=NULL;
NP_NET_STOPSTREAM		NP_NET_StopStream		=NULL;
NP_PLAY_RELEASEPLAYER	NP_PLAY_ReleasePlayer	=NULL;
NP_NET_LOGOUT			NP_NET_Logout			=NULL;
NP_PLAY_CLEANUP			NP_PLAY_Cleanup			=NULL;
NP_PLAY_INIT			NP_PLAY_Init			=NULL;
NP_PLAY_INPUTDATA		NP_PLAY_InputData		=NULL;
NP_NET_PTZCONTROL		NP_NET_PtzControl		=NULL;
NP_NET_GETPTZSTATUS		NP_NET_GetPtzStatus		=NULL;

NP_PLAY_GETSDK_VERSION	NP_PLAY_GetSDK_Version	=NULL;

int loadNpLib()
{
	int rt = 0;

	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\np_sdk\\lib\\");
	hProcessModule	=  LoadLibraryA("np_netsdk.dll");
	//hProcessModule	=  LoadLibraryA("E:\\项目\\常州\\h264_rtmp_wowza_ver1.9_COMMON\\Debug\\np_sdk\\lib\\np_netsdk.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR：load np_netsdk.dll failed\n");
		return -1;
	}
	//2

	NP_NET_Init=(NP_NET_INIT)GetProcAddress(hProcessModule,"_NP_NET_Init@0");
	if(NP_NET_Init == NULL)
	{
		int code = GetLastError();
		m_Log.Add("ERR：GetProcAddress NP_NET_Init failed\n");
		return -1;
	}

	NP_NET_Cleanup=(NP_NET_CLEANUP)GetProcAddress(hProcessModule,"_NP_NET_Cleanup@0");
	if(NP_NET_Cleanup == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_Cleanup failed\n");
		return -1;
	}
	NP_NET_GetSDK_Version=(NP_NET_GetSDK_VERSION)GetProcAddress(hProcessModule,"_NP_NET_GetSDK_Version@12");
	if(NP_NET_GetSDK_Version == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_GetSDK_Version failed\n");
		return -1;
	}
	NP_NET_Login=(NP_NET_LOGIN)GetProcAddress(hProcessModule,"_NP_NET_Login@20");
	if(NP_NET_Login == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_Login failed\n");
		return -1;
	}
	NP_NET_StartStream=(NP_NET_STARTSTREAM)GetProcAddress(hProcessModule,"_NP_NET_StartStream@24");
	if(NP_NET_StartStream == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_StartStream failed\n");
		return -1;
	}
	NP_NET_StopStream=(NP_NET_STOPSTREAM)GetProcAddress(hProcessModule,"_NP_NET_StopStream@4");
	if(NP_NET_StopStream == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_StopStream failed\n");
		return -1;
	}
	NP_NET_Logout=(NP_NET_LOGOUT)GetProcAddress(hProcessModule,"_NP_NET_Logout@4");
	if(NP_NET_Logout == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_Logout failed\n");
		return -1;
	}
	NP_NET_PtzControl=(NP_NET_PTZCONTROL)GetProcAddress(hProcessModule,"_NP_NET_PtzControl@16");
	if(NP_NET_PtzControl == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_PtzControl failed\n");
		return -1;
	}
	NP_NET_GetPtzStatus=(NP_NET_GETPTZSTATUS)GetProcAddress(hProcessModule,"_NP_NET_GetPtzStatus@20");
	if(NP_NET_GetPtzStatus == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_NET_GetPtzStatus failed\n");
		return -1;
	}

	SetDllDirectory(g_CurrentDir+"\\np_sdk\\lib\\");
	hProcessModule	=  LoadLibraryA("np_playsdk.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR：load np_playsdk.dll failed\n");
		return -1;
	}

	NP_PLAY_GetSDK_Version=(NP_PLAY_GETSDK_VERSION)GetProcAddress(hProcessModule,"_NP_PLAY_GetSDK_Version@12");
	if(NP_PLAY_GetSDK_Version == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_GetSDK_Version failed\n");
		return -1;
	}

	NP_PLAY_Init=(NP_PLAY_INIT)GetProcAddress(hProcessModule,"_NP_PLAY_Init@0");
	if(NP_PLAY_Init == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_Init failed\n");
		return -1;
	}

	NP_PLAY_InputData=(NP_PLAY_INPUTDATA)GetProcAddress(hProcessModule,"_NP_PLAY_InputData@12");
	if(NP_PLAY_InputData == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_Inputdata failed\n");
		return -1;
	}

	NP_PLAY_CreatePlayer=(NP_PLAY_CREATEPLAYER)GetProcAddress(hProcessModule,"_NP_PLAY_CreatePlayer@20");
	if(NP_PLAY_CreatePlayer == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_CreatePlayer failed\n");
		return -1;
	}
	NP_PLAY_PlayControl=(NP_PLAY_PLAYCONTROL)GetProcAddress(hProcessModule,"_NP_PLAY_PlayControl@16");
	if(NP_PLAY_PlayControl == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_PlayControl failed\n");
		return -1;
	}
	NP_PLAY_ReleasePlayer=(NP_PLAY_RELEASEPLAYER)GetProcAddress(hProcessModule,"_NP_PLAY_ReleasePlayer@4");
	if(NP_PLAY_ReleasePlayer == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_ReleasePlayer failed\n");
		return -1;
	}
	NP_PLAY_Cleanup=(NP_PLAY_CLEANUP)GetProcAddress(hProcessModule,"_NP_PLAY_Cleanup@0");
	if(NP_PLAY_Cleanup == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_Cleanup failed\n");
		return -1;
	}

	return 0;

}

