

#include "stdafx.h"
#include "h3cLib.h"


ULONG STDCALL IMOS_Initiate ( IN CHAR  szServerAdd[IMOS_IPADDR_LEN], 
  IN ULONG  ulServerPort, 
  IN BOOL_T  bUIFlag, 
  IN BOOL_T  bXPFlag   
 ) 
 ULONG STDCALL IMOS_CleanUp ( IN USER_LOGIN_ID_INFO_S *  pstUserLogIDInfo  )  

 ULONG STDCALL IMOS_Encrypt ( IN const CHAR *  pcInput, 
  IN ULONG  ulInLen, 
  OUT CHAR  szOutput[IMOS_PASSWD_ENCRYPT_LEN]   
 ) 

	 ULONG STDCALL IMOS_LoginEx ( IN CHAR  szUserLoginName[IMOS_NAME_LEN], 
  IN CHAR  szPassword[IMOS_PASSWD_ENCRYPT_LEN], 
  IN CHAR  szSvrIpAddress[IMOS_IPADDR_LEN], 
  IN CHAR  szCltIpAddress[IMOS_IPADDR_LEN], 
  OUT LOGIN_INFO_S *  pstSDKLoginInfo   
 ) 

 VOID STDCALL IMOS_LogoutEx ( IN USER_LOGIN_ID_INFO_S *  pstUserLogIDInfo  ) 

  ULONG STDCALL IMOS_AutoKeepAlive ( IN USER_LOGIN_ID_INFO_S *  pstUserLoginIDInfo  )  


  ULONG STDCALL IMOS_StartPlayer ( IN USER_LOGIN_ID_INFO_S *  pstUserLoginIDInfo, 
  IN ULONG  ulPlayWndNum, 
  INOUT PLAY_WND_INFO_S *  pstPlayWndInfo   
 ) 


 IMOS_EXPORT ULONG STDCALL  IMOS_GetChannelCode (IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, OUT CHAR *pcChannelCode) 
IMOS_EXPORT VOID STDCALL  IMOS_FreeChannelCode (IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, IN const CHAR *pcChannelCode) 



IMOS_EXPORT ULONG STDCALL  IMOS_StartMonitor (IN USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, IN CHAR szCameraCode[IMOS_RES_CODE_LEN], IN CHAR szMonitorCode[IMOS_RES_CODE_LEN], IN ULONG ulStreamType, IN ULONG ulOperateCode) 
IMOS_EXPORT ULONG STDCALL  IMOS_StopMonitor (IN USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, IN CHAR szMonitorCode[IMOS_RES_CODE_LEN], IN ULONG ulOperateCode) 


ULONG STDCALL IMOS_SetParseVideoDataCB ( IN const USER_LOGIN_ID_INFO_S *  pstUserLoginIDInfo, 
  IN const CHAR *  pcChannelCode, 
  IN XP_PARSE_VIDEO_DATA_CALLBACK_PF  pfnParseVideoDataCBFun, 
  IN BOOL_T  bContinue, 
  IN LONG  lUserParam   
 ) 








