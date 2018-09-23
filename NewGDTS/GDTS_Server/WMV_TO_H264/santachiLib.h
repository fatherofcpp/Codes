/********************************************************************
	created:	2014/05/21
	filename: 	santichiLib.h
	file ext:	h
	author:		zhh
	
	purpose:	实现动态加载金三立动态库，减少在程序启动后对所需库的依赖。
				目前实现动态加载的主要来自decNetLib.lib及play.lib
				两个文件
*********************************************************************/

#ifndef DEF_SANTACHILIB_H_
#define DEF_SANTACHILIB_H_


#include	<winsock2.h> 
#include    <Ws2tcpip.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"./santachiLib/dev_type.h"
#include	"./santachiLib/msg_def.h"
#pragma comment( lib,"Ws2_32")

#include "./santachiLib/play.h"
//typedef int (CALLBACK *DECODE_CALLBACK) (long hHandle, char *pData, long lSize, ST_FRAME_INFO *pInfo, DWORD dwUser); 


/**************************************************************************
* Function Name  : st_net_initClientNetLib
* Description    : 初始化网络库，最先调用
* Parameters     : 无		
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_INITCLIENTNETLIB)(void);

/**************************************************************************
* Function Name  : st_net_initServerInfo
* Description    : 分配全局资源，创建的user_id信息，一个设备连接对应一个ID
* Parameters     : ip_addr(设备IP地址)、port(端口号)、user_name(用户名)、user_pwd(密码)	
* Return Type    : 成功       :   返回user_id;
*                  失败       :   返回0;
* Last Modified  : 
***************************************************************************/
typedef unsigned long (_stdcall* ST_NET_INITSERVERINFO)(char *ip_addr, int port, char *user_name, char *user_pwd);

/**************************************************************************
* Function Name  : st_net_userLogin
* Description    : 登陆设备
* Parameters     : user_id(用户ID)	
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
*				   出错		  :   ACK_ERROR_NAME (用户名出错)
								  ACK_ERROR_PWD  (密  码出错)
								  ACK_ERROR_OTHER(其  它错误)
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_USERLOGIN)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_deinitServerInfo
* Description    : 回收全局资源
* Parameters     : user_id(用户ID,st_net_initServerInfo的返回值)		
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_DEINITSERVERINFO)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_getDeviceType
* Description    : 获取设备类型
* Parameters     : user_id(用户ID)		
* Return Type    : 成功       :   返回设备类型;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_GETDEVICETYPE)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_getParam
* Description    : 获取设备参数
* Parameters     : user_id(用户ID)、type(参数类型)、param(参数结构体指针)、
				   size 参数结构体大小	
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_GETPARAM)(unsigned long user_id, int type, void *param, int size);

/**************************************************************************
* Function Name  : st_net_getHeartbeatStatus
* Description    : 获取心跳状态信息
* Parameters     : user_id(用户ID)		
* Return Type    : 无心跳       :   返回0;
*                  有心跳       :   返回1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_GETHEARTBEATSTATUS)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_registerServerAlarmInfoCallback
* Description    : 服务器报警信息注册回调，设备端将向ALARM_INFO_CALLBACK回调中发送报警信息
* Parameters     : user_id(用户ID)、callback(回调函数)、context(保存信息)			
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_REGISTERSERVERALARMINFOCALLBACK)(unsigned long user_id, ALARM_INFO_CALLBACK callback, void *context);

/**************************************************************************
* Function Name  : st_net_openChannelStream
* Description    : 打开视频通道连接，不发送视频数据
* Parameters     : user_id(用户ID)、channel(通道号:0开始), 
				   protocol_type(协议类型)：PROTOCOL_TCP(TCP)、PROTOCOL_UDP(UDP)、PROTOCOL_MCAST(多播)
				   channel_type (通道类型): 0(主通道)、1(从通道) 
				   realTime		(视频播放模式):0(实时)、1(流畅) //该参数功能已去掉	
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_OPENCHANNELSTREAM)(unsigned long user_id, int channel, int protocol_type, int channel_type, int realTime);

/**************************************************************************
* Function Name  : st_net_startChannelStream
* Description    : 开始发送通道视频数据,必须先打开与相应视频通道连接
* Parameters     : user_id(用户ID)、channel(通道号:0开始), 
				   stream_type (流类型): 0(复合流)、1(视频流)、2(音频流)
				   channel_type (通道类型): 0(主通道)、1(从通道) 	
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_STARTCHANNELSTREAM)(unsigned long user_id, int channel, int stream_type, int channel_type);

/**************************************************************************
* Function Name  : st_net_registerChannelStreamCallback
* Description    : 注册通道流回调，设备端将向CHANNEL_STREAM_CALLBACK回调中发送通道流数据
* Parameters     : user_id(用户ID)、channel(通道号)、callback(回调函数)、context(保存信息)		
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_REGISTERCHANNELSTREAMCALLBACK)(unsigned long user_id, int channel, CHANNEL_STREAM_CALLBACK callback, void *context);

/**************************************************************************
* Function Name  : st_net_closeChannelStream
* Description    : 关闭视频通道连接
* Parameters     : user_id(用户ID)、channel(通道号:0开始), 
				   channel_type (通道类型): 0(主通道)、1(从通道) 
				   realTime		(视频播放模式):0(实时)、1(流畅) //该参数功能已去掉	
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_CLOSECHANNELSTREAM)(unsigned long user_id, int channel, int channel_type, int realTime);

/**************************************************************************
* Function Name  : st_net_userLogout
* Description    : 注销登陆
* Parameters     : user_id(用户ID)			
* Return Type    : 成功       :   返回0;
*                  失败       :   返回-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_USERLOGOUT)(unsigned long user_id);


typedef int (_stdcall* ST_PLAYER_INITPLAYLIB)(void);
typedef int (_stdcall* ST_PLAYER_INPUTNETFRAME)(unsigned long player_id, char *buf, int size);
typedef unsigned long (_stdcall* ST_PLAYER_CREATEPLAYINSTANCE)(int dev_type , int play_type);
typedef int (_stdcall* ST_PLAYER_SETPLAYWND)(unsigned long player_id, HWND hwnd);
typedef int (_stdcall* ST_PLAYER_STARTPLAY)(unsigned long player_id);
typedef int (_stdcall*  ST_PLAYER_SETCUSHIONRANGE)(unsigned long player_id, BOOL bEnable, int iMin, int iMax);
typedef int (_stdcall* ST_PLAYER_REGISTERDECCALLBACK)(unsigned long player_id, DECODE_CALLBACK DecCallback, DWORD dwUser);
typedef int (_stdcall* ST_PLAYER_STOPPLAY)(unsigned long player_id);
typedef int (_stdcall* ST_PLAYER_DESTROYPLAYINSTANCE)(unsigned long player_id);

//typedef BOOL (CALLMETHOD* PLAY_SETDECCALLBACKEX)(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2), long nUser);


extern	ST_NET_INITCLIENTNETLIB                  					st_net_initClientNetLib						;	
extern	ST_NET_INITSERVERINFO                    					st_net_initServerInfo						;	
extern	ST_NET_USERLOGIN                         					st_net_userLogin							;	
extern	ST_NET_DEINITSERVERINFO                  					st_net_deinitServerInfo						;	
extern	ST_NET_GETDEVICETYPE                     					st_net_getDeviceType						;	
extern	ST_NET_GETPARAM                          					st_net_getParam								;	
extern	ST_NET_GETHEARTBEATSTATUS                					st_net_getHeartbeatStatus					;	
extern	ST_NET_REGISTERSERVERALARMINFOCALLBACK   					st_net_registerServerAlarmInfoCallback		;	
extern	ST_NET_OPENCHANNELSTREAM                 					st_net_openChannelStream					;	
extern	ST_NET_STARTCHANNELSTREAM                					st_net_startChannelStream					;	
extern	ST_NET_REGISTERCHANNELSTREAMCALLBACK     					st_net_registerChannelStreamCallback		;	
extern	ST_NET_CLOSECHANNELSTREAM                					st_net_closeChannelStream					;	
extern	ST_NET_USERLOGOUT                        					st_net_userLogout							;	
extern	ST_PLAYER_INITPLAYLIB                    					st_player_initPlayLib						;	
extern	ST_PLAYER_INPUTNETFRAME                  					st_player_inputNetFrame						;	
extern	ST_PLAYER_CREATEPLAYINSTANCE             					st_player_createPlayInstance				;	
extern	ST_PLAYER_SETPLAYWND                     					st_player_setPlayWnd						;	
extern	ST_PLAYER_STARTPLAY                      					st_player_startPlay							;	
extern	ST_PLAYER_SETCUSHIONRANGE                					st_player_setCushionRange					;	
extern	ST_PLAYER_REGISTERDECCALLBACK            					st_player_registerDecCallback				;	
extern	ST_PLAYER_STOPPLAY                       					st_player_stopPlay							;	
extern	ST_PLAYER_DESTROYPLAYINSTANCE            					st_player_destroyPlayInstance				;	  

//extern PLAY_SETDECCALLBACKEX			PLAY_SetDecCallBackEx;

int loadSantachiLib(const char *dir);

#endif
