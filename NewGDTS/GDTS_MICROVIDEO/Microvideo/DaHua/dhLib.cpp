

#include "stdafx.h"
#include "gd_errorType.h"
//#include "WMV_TO_H264Dlg.h"
#include "dhLib.h"

CLIENT_INIT						CLIENT_Init=NULL;
CLIENT_SETSUBCONNCALLBACK		CLIENT_SetSubconnCallBack=NULL;
CLIENT_SETAUTORECONNECT			CLIENT_SetAutoReconnect = NULL;
CLIENT_LOGIN					CLIENT_Login = NULL;
CLIENT_REALPLAYEX				CLIENT_RealPlayEx = NULL;
CLIENT_SETREALDATACALLBACKEX	CLIENT_SetRealDataCallBackEx = NULL;
CLIENT_STOPREALPLAYEX			CLIENT_StopRealPlayEx = NULL;
CLIENT_LOGOUT					CLIENT_Logout = NULL;
CLIENT_CLEANUP					CLIENT_Cleanup = NULL;
CLIENT_GETLASTERROR				CLIENT_GetLastError = NULL;
CLIENT_STOPREALPLAY				CLIENT_StopRealPlay = NULL;

PLAY_OPENSTREAM					PLAY_OpenStream = NULL;
PLAY_PLAY						PLAY_Play = NULL;
PLAY_STOP						PLAY_Stop = NULL;
PLAY_CLOSESTREAM				PLAY_CloseStream = NULL;
PLAY_GETLASTERROR				PLAY_GetLastError = NULL;
PLAY_INPUTDATA					PLAY_InputData = NULL;
PLAY_SETSTREAMOPENMODE			PLAY_SetStreamOpenMode = NULL;
PLAY_SETDECCALLBACKEX			PLAY_SetDecCallBackEx = NULL;


int loadDHLib(const char * dir)
{
	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\");
	hProcessModule	=  LoadLibraryA("dhnetsdk.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload dhnetsdk.dll failed\n");
		return -1;
	}

	CLIENT_Init=(CLIENT_INIT)GetProcAddress(hProcessModule,"CLIENT_Init");
	if(CLIENT_Init == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_Init failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_SetSubconnCallBack=(CLIENT_SETSUBCONNCALLBACK)GetProcAddress(hProcessModule,"CLIENT_SetSubconnCallBack");
	if(CLIENT_SetSubconnCallBack == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_SetSubconnCallBack failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_SetAutoReconnect=(CLIENT_SETAUTORECONNECT)GetProcAddress(hProcessModule,"CLIENT_SetAutoReconnect");
	if(CLIENT_SetAutoReconnect == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_SetAutoReconnect failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_Login=(CLIENT_LOGIN)GetProcAddress(hProcessModule,"CLIENT_Login");
	if(CLIENT_Login == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_Login failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_RealPlayEx=(CLIENT_REALPLAYEX)GetProcAddress(hProcessModule,"CLIENT_RealPlayEx");
	if(CLIENT_RealPlayEx == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_RealPlayEx failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_SetRealDataCallBackEx=(CLIENT_SETREALDATACALLBACKEX)GetProcAddress(hProcessModule,"CLIENT_SetRealDataCallBackEx");
	if(CLIENT_SetRealDataCallBackEx == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_SetRealDataCallBackEx failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_StopRealPlayEx=(CLIENT_STOPREALPLAYEX)GetProcAddress(hProcessModule,"CLIENT_StopRealPlayEx");
	if(CLIENT_StopRealPlayEx == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_StopRealPlayEx failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_Logout=(CLIENT_LOGOUT)GetProcAddress(hProcessModule,"CLIENT_Logout");
	if(CLIENT_Logout == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_Init failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_Cleanup=(CLIENT_CLEANUP)GetProcAddress(hProcessModule,"CLIENT_Cleanup");
	if(CLIENT_Cleanup == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_Cleanup failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_GetLastError=(CLIENT_GETLASTERROR)GetProcAddress(hProcessModule,"CLIENT_GetLastError");
	if(CLIENT_GetLastError == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_GetLastError failed[%d]\n",GetLastError());
		return -1;
	}

	CLIENT_StopRealPlay=(CLIENT_STOPREALPLAY)GetProcAddress(hProcessModule,"CLIENT_StopRealPlay");
	if(CLIENT_StopRealPlay == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress CLIENT_StopRealPlay failed[%d]\n",GetLastError());
		return -1;
	}

	hProcessModule=NULL;
	hProcessModule	=  LoadLibraryA("dhplay.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload dhnetsdk.dll failed\n");
		return -1;
	}

	PLAY_OpenStream=(PLAY_OPENSTREAM)GetProcAddress(hProcessModule,"PLAY_OpenStream");
	if(PLAY_OpenStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_OpenStream failed[%d]\n",GetLastError());
		return -1;
	}
	
	PLAY_Play=(PLAY_PLAY)GetProcAddress(hProcessModule,"PLAY_Play");
	if(PLAY_Play == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_Play failed[%d]\n",GetLastError());
		return -1;
	}
	
	PLAY_Stop=(PLAY_STOP)GetProcAddress(hProcessModule,"PLAY_Stop");
	if(PLAY_Stop == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_Stop failed[%d]\n",GetLastError());
		return -1;
	}
	
	PLAY_CloseStream=(PLAY_CLOSESTREAM)GetProcAddress(hProcessModule,"PLAY_CloseStream");
	if(PLAY_CloseStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_CloseStream failed[%d]\n",GetLastError());
		return -1;
	}
	
	PLAY_GetLastError=(PLAY_GETLASTERROR)GetProcAddress(hProcessModule,"PLAY_GetLastError");
	if(PLAY_GetLastError == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_GetLastError failed[%d]\n",GetLastError());
		return -1;
	}

	PLAY_InputData=(PLAY_INPUTDATA)GetProcAddress(hProcessModule,"PLAY_InputData");
	if(PLAY_InputData == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_InputData failed[%d]\n",GetLastError());
		return -1;
	}

	PLAY_SetStreamOpenMode=(PLAY_SETSTREAMOPENMODE)GetProcAddress(hProcessModule,"PLAY_SetStreamOpenMode");
	if(PLAY_SetStreamOpenMode == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_SetStreamOpenMode failed[%d]\n",GetLastError());
		return -1;
	}

	PLAY_SetDecCallBackEx=(PLAY_SETDECCALLBACKEX)GetProcAddress(hProcessModule,"PLAY_SetDecCallBackEx");
	if(PLAY_SetDecCallBackEx == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_SetDecCallBackEx failed[%d]\n",GetLastError());
		return -1;
	}



	return 0;
}
#if 1
unsigned long GD_GetInforFromDaHua(int SdkErr)
{
	unsigned long ret=0;
	if(SdkErr == 1){
		ret=GD_PASSWORDERROR;
	}
	if(SdkErr == 2){
		ret=GD_USERNOTEXIST;
	}
	if(SdkErr == 3){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == 4){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == 5){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == 6){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == 7){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == 8){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == 9){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == 10){
		ret=GD_MAXUSERNUM;
	}
//	TRACE("[GD_GetInforFromDaHua]:ret=%08x",ret);
	return ret;
}
#endif