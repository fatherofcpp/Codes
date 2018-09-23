

#ifndef DEF_H3CLIB_H_
#define DEF_H3CLIB_H_


#include "GeneralDef.h"

#include "imos_public.h"
#include "imos_errcode.h"
#include "imos_def.h"

#include "sdk_def.h"
#include "sdk_struct.h"
#include "sdk_func.h"

typedef struct tagDEV_H3C{//H3C使用参数结构
 char  m_proxyIp[64];		//服务器IP
 LONG  m_proxyPort;		//服务器端口
 char  m_userName[32];		//用户名
 char  m_passWord[32];		//密码
 char  m_deviceId[128];		//设备ID
 
 char  m_dstIp[32];		//目标服务器IP
 char  m_dstPub[255];		//发布名
 LONG	  m_dstStream;	//流下标

 LOGIN_INFO_S m_loginInfo;
 char m_szXpCode[IMOS_RES_CODE_LEN];

}DEV_H3C_PARAMS;

typedef ULONG (__stdcall*  IMOS_INITIATE) ( IN CHAR  szServerAdd[IMOS_IPADDR_LEN],IN ULONG  ulServerPort,IN BOOL_T  bUIFlag,IN BOOL_T  bXPFlag); 
typedef ULONG (__stdcall*  IMOS_CLEANUP) ( IN USER_LOGIN_ID_INFO_S *  pstUserLogIDInfo  ) ; 
typedef ULONG (__stdcall*  IMOS_ENCRYPT) ( IN const CHAR *  pcInput,IN ULONG  ulInLen,OUT CHAR  szOutput[IMOS_PASSWD_ENCRYPT_LEN]); 
typedef ULONG (__stdcall*  IMOS_LOGINEX) ( IN CHAR  szUserLoginName[IMOS_NAME_LEN], IN CHAR  szPassword[IMOS_PASSWD_ENCRYPT_LEN], IN CHAR  szSvrIpAddress[IMOS_IPADDR_LEN], IN CHAR  szCltIpAddress[IMOS_IPADDR_LEN], OUT LOGIN_INFO_S *  pstSDKLoginInfo   ); 
typedef VOID  (__stdcall*  IMOS_LOGOUTEX) ( IN USER_LOGIN_ID_INFO_S *  pstUserLogIDInfo  ) ;
typedef ULONG (__stdcall*  IMOS_AUTOKEEPALIVE) ( IN USER_LOGIN_ID_INFO_S *  pstUserLoginIDInfo  ) ; 
typedef ULONG (__stdcall*  IMOS_STARTPLAYER)( IN USER_LOGIN_ID_INFO_S *  pstUserLoginIDInfo, IN ULONG  ulPlayWndNum, INOUT PLAY_WND_INFO_S *  pstPlayWndInfo   );
typedef ULONG (__stdcall*  IMOS_GETCHANNELCODE) (IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, OUT CHAR *pcChannelCode) ;
typedef VOID  (__stdcall*  IMOS_FREECHANNELCODE) (IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, IN const CHAR *pcChannelCode) ;
typedef ULONG (__stdcall*  IMOS_STARTMONITOR) (IN USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, IN CHAR szCameraCode[IMOS_RES_CODE_LEN], IN CHAR szMonitorCode[IMOS_RES_CODE_LEN], IN ULONG ulStreamType, IN ULONG ulOperateCode) ;
typedef ULONG (__stdcall*  IMOS_STOPMONITOR) (IN USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo, IN CHAR szMonitorCode[IMOS_RES_CODE_LEN], IN ULONG ulOperateCode) ;
typedef ULONG (__stdcall*  IMOS_SETPARSEVIDEODATACB) ( IN const USER_LOGIN_ID_INFO_S *  pstUserLoginIDInfo, IN const CHAR *  pcChannelCode, IN XP_PARSE_VIDEO_DATA_CALLBACK_PF  pfnParseVideoDataCBFun, IN BOOL_T  bContinue, IN LONG  lUserParam   );
typedef ULONG (__stdcall*  IMOS_SETDECODEVIDEODATACB) ( IN const USER_LOGIN_ID_INFO_S *  pstUserLoginIDInfo, IN const CHAR *  pcChannelCode, IN XP_DECODE_VIDEO_DATA_CALLBACK_PF  pfnParseVideoDataCBFun, IN BOOL_T  bContinue, IN LONG  lUserParam   );


extern IMOS_INITIATE			IMOS_Initiate; 
extern IMOS_CLEANUP			IMOS_CleanUp;
extern IMOS_ENCRYPT			IMOS_Encrypt;
extern IMOS_LOGINEX			IMOS_LoginEx; 
extern IMOS_LOGOUTEX			IMOS_LogoutEx;
extern IMOS_AUTOKEEPALIVE		IMOS_AutoKeepAlive;
extern IMOS_STARTPLAYER		IMOS_StartPlayer;
extern IMOS_GETCHANNELCODE		IMOS_GetChannelCode	;
extern IMOS_FREECHANNELCODE	IMOS_FreeChannelCode;
extern IMOS_STARTMONITOR		IMOS_StartMonitor;
extern IMOS_STOPMONITOR		IMOS_StopMonitor;
extern IMOS_SETPARSEVIDEODATACB IMOS_SetParseVideoDataCB;
extern IMOS_SETDECODEVIDEODATACB IMOS_SetDecodeVideoDataCB;

int loadH3cLib();

#if 0
int initH3cDevice(void * pParams);

int 



int 

#endif


#endif
