/********************************************************************
	created:	2014/03/27
	filename: 	vnmpLib.cpp
	file base:	VideoNetSDK
	file ext:	cpp
	author:		zhh
	
	purpose:	实现动态加载长天智远动态库，减少在程序启动后对所需库的依赖。
				目前实现动态加载的主要来自VideoNetSDK.dll及VideoNetPlay.dll
				两个文件
*********************************************************************/

#include "stdafx.h"
//#include "WMV_TO_H264.h"
//#include "WMV_TO_H264Dlg.h"
#include "vnmpLib.h"

VIDEO_DEVICE_INIT					video_device_init				=NULL;
VIDEO_DEVICE_DESTROY				video_device_destroy			=NULL;
VIDEO_DEVICE_LOGIN					video_device_login				=NULL;
VIDEO_DEVICE_LOGOUT					video_device_logout				=NULL;
VIDEO_DEVICE_CAPTURING_START		video_device_capturing_start	=NULL;
VIDEO_DEVICE_CAPTURING_STOP			video_device_capturing_stop		=NULL;

VIDEO_DEVICE_CAMERACONTROL			video_device_cameracontrol		=NULL;

VIDEO_PLAY_SETSTREAMOPENMODE		video_play_SetStreamOpenMode	=NULL;
VIDEO_PLAY_SETDISPLAYBUF			video_play_SetDisplayBuf		=NULL;
VIDEO_PLAY_SETBUFFERTYPE			video_play_SetBufferType		=NULL;
VIDEO_PLAY_SETYUVCALLBACK			video_play_SetYUVCallBack		=NULL;
VIDEO_PLAY_PLAY						video_play_Play					=NULL;
VIDEO_PLAY_INPUTVIDEODATA			video_play_InputVideoData		=NULL;
VIDEO_PLAY_STOP						video_play_Stop					=NULL;

int loadVNMPLib()
{
	int rt = 0;

	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\dll\\");
//	SetDllDirectory("VNMP\\dll\\");
	hProcessModule	=  LoadLibraryA("VNMPNETSDK.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR：load VNMPNETSDK.dll failed\n");
		return -1;
	}
	//2

	video_device_init=(VIDEO_DEVICE_INIT)GetProcAddress(hProcessModule,"video_device_init");
	if(video_device_init == NULL)
	{
		int code = GetLastError();
		m_Log.Add("ERR：GetProcAddress video_device_init failed\n");
		return -1;
	}

	video_device_destroy=(VIDEO_DEVICE_DESTROY)GetProcAddress(hProcessModule,"video_device_destroy");
	if(video_device_destroy == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_device_destroy failed\n");
		return -1;
	}
	video_device_login=(VIDEO_DEVICE_LOGIN)GetProcAddress(hProcessModule,"video_device_login");
	if(video_device_login == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_device_login failed\n");
		return -1;
	}
	video_device_logout=(VIDEO_DEVICE_LOGOUT)GetProcAddress(hProcessModule,"video_device_logout");
	if(video_device_logout == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_device_logout failed\n");
		return -1;
	}
	video_device_capturing_start=(VIDEO_DEVICE_CAPTURING_START)GetProcAddress(hProcessModule,"video_device_capturing_start");
	if(video_device_capturing_start == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_device_capturing_start failed\n");
		return -1;
	}
	video_device_capturing_stop=(VIDEO_DEVICE_CAPTURING_STOP)GetProcAddress(hProcessModule,"video_device_capturing_stop");
	if(video_device_capturing_stop == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_device_capturing_stop failed\n");
		return -1;
	}

	video_device_cameracontrol=(VIDEO_DEVICE_CAMERACONTROL)GetProcAddress(hProcessModule,"video_device_cameracontrol");
	if(video_device_cameracontrol == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_device_cameracontrol failed\n");
		return -1;
	}

	//SetDllDirectory(g_CurrentDir);
	hProcessModule	=  LoadLibraryA("VideoNetPlay.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR：load VideoNetPlay.dll failed\n");
		return -1;
	}

	video_play_SetStreamOpenMode=(VIDEO_PLAY_SETSTREAMOPENMODE)GetProcAddress(hProcessModule,"video_play_SetStreamOpenMode");
	if(video_play_SetStreamOpenMode == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_play_SetStreamOpenMode failed\n");
		return -1;
	}

	video_play_SetDisplayBuf=(VIDEO_PLAY_SETDISPLAYBUF)GetProcAddress(hProcessModule,"video_play_SetDisplayBuf");
	if(video_play_SetDisplayBuf == NULL)
	{
		m_Log.Add("ERR：GetProcAddress NP_PLAY_Init failed\n");
		return -1;
	}

	video_play_SetBufferType=(VIDEO_PLAY_SETBUFFERTYPE)GetProcAddress(hProcessModule,"video_play_SetBufferType");
	if(video_play_SetBufferType == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_play_SetBufferType failed\n");
		return -1;
	}

	video_play_SetYUVCallBack=(VIDEO_PLAY_SETYUVCALLBACK)GetProcAddress(hProcessModule,"video_play_SetYUVCallBack");
	if(video_play_SetYUVCallBack == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_play_SetYUVCallBack failed\n");
		return -1;
	}
	video_play_Play=(VIDEO_PLAY_PLAY)GetProcAddress(hProcessModule,"video_play_Play");
	if(video_play_Play == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_play_Play failed\n");
		return -1;
	}
	video_play_InputVideoData=(VIDEO_PLAY_INPUTVIDEODATA)GetProcAddress(hProcessModule,"video_play_InputVideoData");
	if(video_play_InputVideoData == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_play_InputVideoData failed\n");
		return -1;
	}
	video_play_Stop=(VIDEO_PLAY_STOP)GetProcAddress(hProcessModule,"video_play_Stop");
	if(video_play_Stop == NULL)
	{
		m_Log.Add("ERR：GetProcAddress video_play_Stop failed\n");
		return -1;
	}

	return 0;

}

