/********************************************************************
	created:	2014/05/05
	filename: 	dhLib.h
	file ext:	h
	author:		zhh
	
	purpose:	实现动态加载浙江大华动态库，减少在程序启动后对所需库的依赖。
				目前实现动态加载的主要来自dhnetsdk.lib及dhplay.lib
				两个文件
*********************************************************************/

#ifndef DEF_HKLIB_H_
#define DEF_HKLIB_H_



#include ".\\HCNetSDK\\incCn\\HCNetSDK.h"
#include ".\\HCNetSDK\\incCn\\plaympeg4.h"

typedef void (CALLBACK *REALDATACALLBACK) (LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser);


typedef BOOL (__stdcall* NET_DVR_INIT)();
typedef LONG (__stdcall* NET_DVR_LOGIN_V30)(char *sDVRIP, WORD wDVRPort, char *sUserName, char *sPassword, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo);
typedef BOOL (__stdcall* NET_DVR_SETCONNECTTIME)(DWORD dwWaitTime, DWORD dwTryTimes);
typedef BOOL (__stdcall* NET_DVR_SETRECONNECT)(DWORD dwInterval, BOOL bEnableRecon);
typedef LONG (__stdcall* NET_DVR_REALPLAY_V40)(LONG lUserID, LPNET_DVR_PREVIEWINFO lpPreviewInfo, REALDATACALLBACK fRealDataCallBack_V30, void* pUser);
typedef BOOL (__stdcall* NET_DVR_STOPREALPLAY)(LONG lRealHandle);
typedef BOOL (__stdcall* NET_DVR_LOGOUT_V30)(LONG lUserID);
typedef BOOL (__stdcall* NET_DVR_CLEANUP)();
typedef DWORD (__stdcall* NET_DVR_GETLASTERROR)();

typedef BOOL (__stdcall* PLAYM4_STOP)(LONG nPort);
typedef BOOL (__stdcall* PLAYM4_GETPORT)(LONG* nPort);
typedef BOOL (__stdcall* PLAYM4_SETSTREAMOPENMODE)(LONG nPort,DWORD nMode);
typedef BOOL (__stdcall* PLAYM4_OPENSTREAM)(LONG nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);
typedef BOOL (__stdcall* PLAYM4_SETDECCALLBACKMEND)(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2), long nUser);
typedef BOOL (__stdcall* PLAYM4_PLAY)(LONG nPort, HWND hWnd);
typedef BOOL (__stdcall* PLAYM4_INPUTDATA)(LONG nPort,PBYTE pBuf,DWORD nSize);
typedef BOOL (__stdcall* PLAYM4_CLOSESTREAM)(LONG nPort);
typedef BOOL (__stdcall* PLAYM4_FREEPORT)(LONG nPort);

//typedef BOOL (CALLMETHOD* PLAY_SETDECCALLBACKEX)(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2), long nUser);


extern NET_DVR_INIT              NET_DVR_Init;                 
extern NET_DVR_LOGIN_V30         NET_DVR_Login_V30;            
extern NET_DVR_SETCONNECTTIME    NET_DVR_SetConnectTime;       
extern NET_DVR_SETRECONNECT      NET_DVR_SetReconnect;         
extern NET_DVR_REALPLAY_V40      NET_DVR_RealPlay_V40;         
extern NET_DVR_STOPREALPLAY      NET_DVR_StopRealPlay;         
extern NET_DVR_LOGOUT_V30        NET_DVR_Logout_V30;           
extern NET_DVR_CLEANUP           NET_DVR_Cleanup; 
extern NET_DVR_GETLASTERROR		 NET_DVR_GetLastError;
	                                                            
extern PLAYM4_STOP               PlayM4_Stop;                  
extern PLAYM4_GETPORT            PlayM4_GetPort;               
extern PLAYM4_SETSTREAMOPENMODE  PlayM4_SetStreamOpenMode;     
extern PLAYM4_OPENSTREAM         PlayM4_OpenStream;            
extern PLAYM4_SETDECCALLBACKMEND PlayM4_SetDecCallBackMend;    
extern PLAYM4_PLAY               PlayM4_Play;                  
extern PLAYM4_INPUTDATA          PlayM4_InputData; 
extern PLAYM4_CLOSESTREAM		  PlayM4_CloseStream;
extern PLAYM4_FREEPORT			PlayM4_FreePort;

//extern PLAY_SETDECCALLBACKEX			PLAY_SetDecCallBackEx;

int loadHKLib(const char *dir);
unsigned long GD_GetInforFromHK(unsigned long SdkErr);

#endif
