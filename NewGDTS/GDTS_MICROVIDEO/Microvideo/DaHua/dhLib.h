/********************************************************************
	created:	2014/05/05
	filename: 	dhLib.h
	file ext:	h
	author:		zhh
	
	purpose:	实现动态加载浙江大华动态库，减少在程序启动后对所需库的依赖。
				目前实现动态加载的主要来自dhnetsdk.lib及dhplay.lib
				两个文件
*********************************************************************/

#ifndef DEF_DHLIB_H_
#define DEF_DHLIB_H_


#include "./dhLib/dhnetsdk.h"
#include "./dhLib/dhplay.h"




typedef BOOL (CALL_METHOD* CLIENT_INIT)(fDisConnect cbDisConnect, LDWORD dwUser);

// 设置动态子连接断线回调函数，目前SVR设备的监视和回放是短连接的。
typedef void (CALL_METHOD* CLIENT_SETSUBCONNCALLBACK)(fSubDisConnect cbSubDisConnect, LDWORD dwUser);

// 设置断线重连成功回调函数，设置后SDK内部断线自动重连
typedef void (CALL_METHOD* CLIENT_SETAUTORECONNECT)(fHaveReConnect cbAutoConnect, LDWORD dwUser);

// 向设备注册
typedef LLONG (CALL_METHOD* CLIENT_LOGIN)(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error );

//------------------------------------------------------------------------
// 函数: PLAY_OpenStream
// 描述: 打开流播放
// 参数: nPort,通道号
//		 pFileHeadBuf,目前不使用,填NULL.
//		 nSize,目前不适用,填0.
//		 nBufPoolSize,设置播放器中存放数据流的缓冲区大小.
//			范围是[SOURCE_BUF_MIN,SOURCE_BUF_MAX].一般设为900*1024，如果数
//			据送过来相对均匀,可调小该值,如果数据传输不均匀,可增大该值.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_OPENSTREAM)(LONG nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);

//------------------------------------------------------------------------
// 函数: PLAY_Play
// 描述: 开始播放.如果已经播放,改变当前播放状态为正常速度播放.
// 参数: nPort,通道号
//		 hWnd,播放窗口句柄
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_PLAY)(LONG nPort, HWND hWnd);

// 开始实时监视--扩展
typedef LLONG (CALL_METHOD* CLIENT_REALPLAYEX)(LLONG lLoginID, int nChannelID, HWND hWnd, DH_RealPlayType rType);

// 设置实时监视数据回调--扩展
typedef BOOL (CALL_METHOD* CLIENT_SETREALDATACALLBACKEX)(LLONG lRealHandle, fRealDataCallBackEx cbRealData, LDWORD dwUser, DWORD dwFlag);

//------------------------------------------------------------------------
// 函数: PLAY_Stop
// 描述: 停止播放
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_STOP)(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_CloseStream
// 描述: 关闭流
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_CLOSESTREAM)(LONG nPort);

// 停止实时预览--扩展
typedef BOOL (CALL_METHOD* CLIENT_STOPREALPLAYEX)(LLONG lRealHandle);

// 向设备注销
typedef BOOL (CALL_METHOD* CLIENT_LOGOUT)(LLONG lLoginID);

// SDK退出清理
typedef void (CALL_METHOD* CLIENT_CLEANUP)();

//------------------------------------------------------------------------
// 函数: PLAY_GetLastError
// 描述: 获取错误码
// 参数: nPort,通道号
// 返回: DWORD,获得当前错误的错误码.请参见错误码宏定义.
//------------------------------------------------------------------------
typedef DWORD (CALLMETHOD* PLAY_GETLASTERROR)(LONG nPort);

// 返回函数执行失败代码
typedef DWORD (CALL_METHOD* CLIENT_GETLASTERROR)(void);
// 停止实时预览
typedef BOOL (CALL_METHOD* CLIENT_STOPREALPLAY)(LLONG lRealHandle);

//------------------------------------------------------------------------
// 函数: PLAY_InputData
// 描述: 输入数据流,PLAY_Play之后使用
// 参数: nPort,通道号
//		 pBuf,缓冲区地址
//		 nSize,缓冲区大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.如失败,一般是缓冲区已满,用户可暂停
//			输入，一段时间之后再输入流，确保播放库不丢失数据。
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_INPUTDATA)(LONG nPort,PBYTE pBuf,DWORD nSize);

//------------------------------------------------------------------------
// 函数: PLAY_SetStreamOpenMode
// 描述: 设置流播放模式,必须在播放之前设置.
// 参数: nPort,通道号
//		 nMode,STREAME_REALTIME实时模式(默认);STREAME_FILE文件模式.实时模
//			式,适合播放网络实时数据;文件模式,适合用户把文件数据用流方式输入.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_SETSTREAMOPENMODE)(LONG nPort,DWORD nMode);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecCallBackEx
// 描述: 设置解码回调,替换播放器中的显示部分,由用户自己控制显示,该函数在
//			PLAY_Play之前调用,在PLAY_Stop时自动失效,下次调用PLAY_Play之前
//			需要重新设置.解码部分不控制速度,只要用户从回调函数中返回,解码器
//			就会解码下一部分数据.适用于只解码不显示的情形。
// 参数: nPort,通道号
//		 DecCBFun,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pBuf,解码后的音视频数据
//			nSize,解码后的音视频数据pBuf的长度
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			nReserved1,用户自定义参数
//			nReserved2,保留参数	
//		 nUser,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_SETDECCALLBACKEX)(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2), long nUser);



extern CLIENT_INIT						CLIENT_Init;
extern CLIENT_SETSUBCONNCALLBACK		CLIENT_SetSubconnCallBack;
extern CLIENT_SETAUTORECONNECT			CLIENT_SetAutoReconnect;
extern CLIENT_LOGIN						CLIENT_Login;
extern PLAY_OPENSTREAM					PLAY_OpenStream;
extern PLAY_PLAY						PLAY_Play;
extern CLIENT_REALPLAYEX				CLIENT_RealPlayEx;
extern CLIENT_SETREALDATACALLBACKEX		CLIENT_SetRealDataCallBackEx;
extern PLAY_STOP						PLAY_Stop;
extern PLAY_CLOSESTREAM					PLAY_CloseStream;
extern CLIENT_STOPREALPLAYEX			CLIENT_StopRealPlayEx;
extern CLIENT_LOGOUT					CLIENT_Logout;
extern CLIENT_CLEANUP					CLIENT_Cleanup;
extern PLAY_GETLASTERROR				PLAY_GetLastError;
extern CLIENT_GETLASTERROR				CLIENT_GetLastError;
extern CLIENT_STOPREALPLAY				CLIENT_StopRealPlay;
extern PLAY_INPUTDATA					PLAY_InputData;
extern PLAY_SETSTREAMOPENMODE			PLAY_SetStreamOpenMode;
extern PLAY_SETDECCALLBACKEX			PLAY_SetDecCallBackEx;

int loadDHLib(const char *dir);
unsigned long GD_GetInforFromDaHua(int SdkErr);

#endif
