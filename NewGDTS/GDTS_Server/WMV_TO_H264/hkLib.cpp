

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "hkLib.h"

NET_DVR_INIT              NET_DVR_Init				= NULL;                 
NET_DVR_LOGIN_V30         NET_DVR_Login_V30		= NULL;            
NET_DVR_SETCONNECTTIME    NET_DVR_SetConnectTime	= NULL;       
NET_DVR_SETRECONNECT      NET_DVR_SetReconnect		= NULL;         
NET_DVR_REALPLAY_V40      NET_DVR_RealPlay_V40		= NULL;         
NET_DVR_STOPREALPLAY      NET_DVR_StopRealPlay		= NULL;         
NET_DVR_LOGOUT_V30        NET_DVR_Logout_V30		= NULL;           
NET_DVR_CLEANUP           NET_DVR_Cleanup			= NULL;  
NET_DVR_GETLASTERROR	  NET_DVR_GetLastError		= NULL;


                                                         
PLAYM4_STOP               PlayM4_Stop				= NULL;                  
PLAYM4_GETPORT            PlayM4_GetPort			= NULL;               
PLAYM4_SETSTREAMOPENMODE  PlayM4_SetStreamOpenMode	= NULL;     
PLAYM4_OPENSTREAM         PlayM4_OpenStream		= NULL;            
PLAYM4_SETDECCALLBACKMEND PlayM4_SetDecCallBackMend= NULL;    
PLAYM4_PLAY               PlayM4_Play				= NULL;                  
PLAYM4_INPUTDATA          PlayM4_InputData			= NULL;  
PLAYM4_CLOSESTREAM		  PlayM4_CloseStream		= NULL;
PLAYM4_FREEPORT			  PlayM4_FreePort			=NULL;

int loadHKLib(const char * dir)
{
	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\HCNetSDK\\lib\\");
	hProcessModule	=  LoadLibraryA("HCNetSDK.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload dhnetsdk.dll failed\n");
		return -1;
	}

	NET_DVR_Init=(NET_DVR_INIT)GetProcAddress(hProcessModule,"NET_DVR_Init");
	if(NET_DVR_Init == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_Init failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_Login_V30=(NET_DVR_LOGIN_V30)GetProcAddress(hProcessModule,"NET_DVR_Login_V30");
	if(NET_DVR_Login_V30 == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_Login_V30 failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_SetConnectTime=(NET_DVR_SETCONNECTTIME)GetProcAddress(hProcessModule,"NET_DVR_SetConnectTime");
	if(NET_DVR_SetConnectTime == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_SetConnectTime failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_SetReconnect=(NET_DVR_SETRECONNECT)GetProcAddress(hProcessModule,"NET_DVR_SetReconnect");
	if(NET_DVR_SetReconnect == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_SetReconnect failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_RealPlay_V40=(NET_DVR_REALPLAY_V40)GetProcAddress(hProcessModule,"NET_DVR_RealPlay_V40");
	if(NET_DVR_RealPlay_V40 == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_RealPlay_V40 failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_StopRealPlay=(NET_DVR_STOPREALPLAY)GetProcAddress(hProcessModule,"NET_DVR_StopRealPlay");
	if(NET_DVR_StopRealPlay == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_StopRealPlay failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_Logout_V30=(NET_DVR_LOGOUT_V30)GetProcAddress(hProcessModule,"NET_DVR_Logout_V30");
	if(NET_DVR_Logout_V30 == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_Logout_V30 failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_Cleanup=(NET_DVR_CLEANUP)GetProcAddress(hProcessModule,"NET_DVR_Cleanup");
	if(NET_DVR_Cleanup == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_Cleanup failed[%d]\n",GetLastError());
		return -1;
	}

	NET_DVR_GetLastError=(NET_DVR_GETLASTERROR)GetProcAddress(hProcessModule,"NET_DVR_GetLastError");
	if(NET_DVR_GetLastError == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress NET_DVR_GetLastError failed[%d]\n",GetLastError());
		return -1;
	}


	hProcessModule=NULL;
	hProcessModule	=  LoadLibraryA("PlayCtrl.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload dhnetsdk.dll failed\n");
		return -1;
	}

	PlayM4_Stop=(PLAYM4_STOP)GetProcAddress(hProcessModule,"PlayM4_Stop");
	if(PlayM4_Stop == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_Stop failed[%d]\n",GetLastError());
		return -1;
	}
	
	PlayM4_GetPort=(PLAYM4_GETPORT)GetProcAddress(hProcessModule,"PlayM4_GetPort");
	if(PlayM4_GetPort == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_GetPort failed[%d]\n",GetLastError());
		return -1;
	}
	
	PlayM4_SetStreamOpenMode=(PLAYM4_SETSTREAMOPENMODE)GetProcAddress(hProcessModule,"PlayM4_SetStreamOpenMode");
	if(PlayM4_SetStreamOpenMode == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_SetStreamOpenMode failed[%d]\n",GetLastError());
		return -1;
	}
	
	PlayM4_OpenStream=(PLAYM4_OPENSTREAM)GetProcAddress(hProcessModule,"PlayM4_OpenStream");
	if(PlayM4_OpenStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_OpenStream failed[%d]\n",GetLastError());
		return -1;
	}
	
	PlayM4_SetDecCallBackMend=(PLAYM4_SETDECCALLBACKMEND)GetProcAddress(hProcessModule,"PlayM4_SetDecCallBackMend");
	if(PlayM4_SetDecCallBackMend == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PLAY_GetLastError failed[%d]\n",GetLastError());
		return -1;
	}

	PlayM4_Play=(PLAYM4_PLAY)GetProcAddress(hProcessModule,"PlayM4_Play");
	if(PlayM4_Play == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_Play failed[%d]\n",GetLastError());
		return -1;
	}

	PlayM4_InputData=(PLAYM4_INPUTDATA)GetProcAddress(hProcessModule,"PlayM4_InputData");
	if(PlayM4_InputData == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_InputData failed[%d]\n",GetLastError());
		return -1;
	}
	
	PlayM4_CloseStream=(PLAYM4_CLOSESTREAM)GetProcAddress(hProcessModule,"PlayM4_CloseStream");
	if(PlayM4_CloseStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_CloseStream failed[%d]\n",GetLastError());
		return -1;
	}

	PlayM4_FreePort=(PLAYM4_FREEPORT)GetProcAddress(hProcessModule,"PlayM4_FreePort");
	if(PlayM4_FreePort == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress PlayM4_FreePort failed[%d]\n",GetLastError());
		return -1;
	}


	return 0;
}

	//´íÎóÂë
unsigned long GD_GetInforFromHK(unsigned long SdkErr)
{
	unsigned long ret=0;
	if(SdkErr == NET_DVR_NOERROR){
		ret=0;
	}
	if(SdkErr == NET_DVR_PASSWORD_ERROR){
		ret=GD_PASSWORDERROR;
	}
	if(SdkErr == NET_DVR_NOINIT){
		ret=GD_NOINIT;
	}
	if(SdkErr == NET_DVR_NETWORK_FAIL_CONNECT){
		ret=GD_CONNECTSERVERFAILED;
	}
	if(SdkErr == NET_DVR_NETWORK_SEND_ERROR){
		ret=GD_SENDERROR;
	}
	if(SdkErr == NET_DVR_NETWORK_RECV_ERROR){
		ret=GD_RECVERROR;
	}
	if(SdkErr == NET_DVR_NETWORK_RECV_TIMEOUT){
		ret=GD_RECVTIMEOUT;
	}
	if(SdkErr == NET_DVR_COMMANDTIMEOUT){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == NET_DVR_PARAMETER_ERROR){
		ret=GD_INVALIDPARAMETERS;
	}
	if(SdkErr == NET_DVR_ALLOC_RESOURCE_ERROR){
		ret=GD_ALLOCRESOURCEERROR;
	}
	if(SdkErr == NET_DVR_NOENOUGH_BUF){
		ret=GD_MEMORYNOTENOUGH;
	}
	if(SdkErr == NET_DVR_CREATESOCKET_ERROR){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == NET_DVR_MAX_NUM){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == NET_DVR_MAX_USERNUM){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == NET_DVR_BINDSOCKET_ERROR){
		ret=GD_ANONYMITYMSG;
	}
	if(SdkErr == NET_DVR_SOCKETCLOSE_ERROR){
		ret=GD_ANONYMITYMSG;
	}

//	TRACE("[GD_GetInforFromHK]:ret=%08x",ret);
	return ret;
};