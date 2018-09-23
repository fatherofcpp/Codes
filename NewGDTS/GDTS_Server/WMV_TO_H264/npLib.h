/********************************************************************
	created:	2014/03/27
	filename: 	npLib.h
	file ext:	h
	author:		zhh
	
	purpose:	实现动态加载东方网力动态库，减少在程序启动后对所需库的依赖。
				目前实现动态加载的主要来自np_netsdk.dll及np_playsdk.dll
				两个文件
*********************************************************************/

#ifndef DEF_NPLIB_H_
#define DEF_NPLIB_H_


#include "./np_sdk/include/DataType.h"
#include "./np_sdk/include/np_netsdk.h"
#include "./np_sdk/include/np_playsdk.h"

//#pragma comment(lib,"./np_sdk/lib/np_netsdk.lib")
//#pragma comment(lib,"./np_sdk/lib/np_playsdk.lib")

typedef struct tagDEV_NP_INFO{
	char		m_dstIp[32];		//目标服务器IP
	char		m_dstPub[64];		//发布名
	LONG		m_dstStream;	//流下标
	NP_NET_HD m_loginid;
	NP_NET_HD realLiveHd;
	NP_PLAY_HD realPlayerHd;

}DEV_NP_INFO;

typedef struct tagDEV_NP{//东方网力使用参数结构
	char		m_proxyIp[64];		//转发服务器IP
	LONG		m_proxyPort;		//转发服务器端口
	char		m_userName[32];		//用户名
	char		m_passWord[32];		//密码
	
	char		m_devName[64];

	DEV_NP_INFO m_devInfo;


}DEV_NP_PARAMS;



typedef NPSDKError (NP_SDK_METHOD* NP_NET_INIT)();

typedef NPSDKError (NP_SDK_METHOD* NP_NET_GetSDK_VERSION)(
	void* infoBuf,
	INT len,
	INT* realLen /*= NULL*/
	);

typedef NPSDKError (NP_SDK_METHOD* NP_NET_LOGIN)(
	NP_NET_HD* userHD,
	const TCHAR* ipOrHost,
	INT port,
	const TCHAR* userName,
	const TCHAR* passwd
	);


typedef NPSDKError (NP_SDK_METHOD* NP_PLAY_CREATEPLAYER)(
	NP_PLAY_HD* playHD,
	HWND hwnd/* = NULL*/,
	NPPlayStreamType streamType,
	Callback_OnVideoDecodeData fnOnDecoder, 
	void* userParam
	);


typedef NPSDKError (NP_SDK_METHOD* NP_NET_STARTSTREAM)(
	NP_NET_HD userHD,
	NP_NET_HD* streamHD,
	const TCHAR* avPath,
	Callback_OnStreamData fnData,
	CallBack_OnStreamRobbed fnRobbed,
	void* userParam
	);


typedef NPSDKError (NP_SDK_METHOD* NP_NET_STOPSTREAM)(
	NP_NET_HD streamHD
	);


typedef NPSDKError (NP_SDK_METHOD* NP_PLAY_RELEASEPLAYER)(
	NP_PLAY_HD playHD
	);
typedef NPSDKError (NP_SDK_METHOD* NP_NET_CLEANUP)();

typedef void (NP_SDK_METHOD* NP_PLAY_CLEANUP)();

typedef NPSDKError (NP_SDK_METHOD* NP_NET_LOGOUT)(
	NP_NET_HD userHD
	);
typedef NPSDKError (NP_SDK_METHOD* NP_PLAY_PLAYCONTROL)(
	NP_PLAY_HD playHD, 
	NPPlayControl cmd, 
	const void* inParam /*= NULL*/, 
	void* outParam/*= NULL*/
	);
typedef void (NP_SDK_METHOD* NP_PLAY_INIT)();
typedef NPSDKError (NP_SDK_METHOD* NP_PLAY_INPUTDATA)(
	NP_PLAY_HD playHD, 
	const void* data, 
	INT len
	);
typedef NPSDKError (NP_SDK_METHOD* NP_PLAY_GETSDK_VERSION)(
	TCHAR* infoBuf,
	INT len,
	INT* realLen /*= NULL*/
	);
typedef NPSDKError (NP_SDK_METHOD* NP_NET_PTZCONTROL)(
	NP_NET_HD userHD,    
	const TCHAR* avPath,   
	NPNetPTZCommand cmd,   
	INT param    
	);
/// 获取云台状态
/// @param[in]	userHD		登录句柄,由NP_NET_Login()获得
/// @param[in]	camPath		对象名称,通常为"av/"开头的字符串,例如"av/dcam/1"
/// @param[out] statusBuf	存放PTZ的状态信息，内存由使用者分配，信息可以通过NP_NET_InfoParseKeyValue()来解析
/// @param[in]  bufLen		statusBuf的长度
/// @param[out] realLen		返回实际信息的长度
/// @return
///	-	NPSDK_OK	成功
///	-	其它		失败,错误码
typedef NPSDKError (NP_SDK_METHOD* NP_NET_GETPTZSTATUS)(
	NP_NET_HD userHD,
	const TCHAR* camPath,
	void* statusBuf,
	INT bufLen,
	INT* realLen /*= NULL*/
	);

extern NP_PLAY_GETSDK_VERSION NP_PLAY_GetSDK_Version;
extern NP_PLAY_INPUTDATA NP_PLAY_InputData;
extern NP_PLAY_INIT NP_PLAY_Init;
extern NP_NET_INIT NP_NET_Init;
extern NP_NET_CLEANUP NP_NET_Cleanup;
extern NP_NET_GetSDK_VERSION NP_NET_GetSDK_Version;
extern NP_NET_LOGIN NP_NET_Login;
extern NP_PLAY_CREATEPLAYER NP_PLAY_CreatePlayer;
extern NP_PLAY_PLAYCONTROL NP_PLAY_PlayControl;
extern NP_NET_STARTSTREAM NP_NET_StartStream;
extern NP_NET_STOPSTREAM NP_NET_StopStream;
extern NP_PLAY_RELEASEPLAYER NP_PLAY_ReleasePlayer;
extern NP_NET_LOGOUT NP_NET_Logout;
extern NP_PLAY_CLEANUP NP_PLAY_Cleanup;
extern NP_NET_PTZCONTROL NP_NET_PtzControl;
extern NP_NET_GETPTZSTATUS NP_NET_GetPtzStatus;

int loadNpLib();

#endif
