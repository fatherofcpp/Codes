/********************************************************************
	created:	2014/05/21
	filename: 	santichiLib.h
	file ext:	h
	author:		zhh
	
	purpose:	ʵ�ֶ�̬���ؽ�������̬�⣬�����ڳ����������������������
				Ŀǰʵ�ֶ�̬���ص���Ҫ����decNetLib.lib��play.lib
				�����ļ�
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
* Description    : ��ʼ������⣬���ȵ���
* Parameters     : ��		
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_INITCLIENTNETLIB)(void);

/**************************************************************************
* Function Name  : st_net_initServerInfo
* Description    : ����ȫ����Դ��������user_id��Ϣ��һ���豸���Ӷ�Ӧһ��ID
* Parameters     : ip_addr(�豸IP��ַ)��port(�˿ں�)��user_name(�û���)��user_pwd(����)	
* Return Type    : �ɹ�       :   ����user_id;
*                  ʧ��       :   ����0;
* Last Modified  : 
***************************************************************************/
typedef unsigned long (_stdcall* ST_NET_INITSERVERINFO)(char *ip_addr, int port, char *user_name, char *user_pwd);

/**************************************************************************
* Function Name  : st_net_userLogin
* Description    : ��½�豸
* Parameters     : user_id(�û�ID)	
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
*				   ����		  :   ACK_ERROR_NAME (�û�������)
								  ACK_ERROR_PWD  (��  �����)
								  ACK_ERROR_OTHER(��  ������)
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_USERLOGIN)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_deinitServerInfo
* Description    : ����ȫ����Դ
* Parameters     : user_id(�û�ID,st_net_initServerInfo�ķ���ֵ)		
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_DEINITSERVERINFO)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_getDeviceType
* Description    : ��ȡ�豸����
* Parameters     : user_id(�û�ID)		
* Return Type    : �ɹ�       :   �����豸����;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_GETDEVICETYPE)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_getParam
* Description    : ��ȡ�豸����
* Parameters     : user_id(�û�ID)��type(��������)��param(�����ṹ��ָ��)��
				   size �����ṹ���С	
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_GETPARAM)(unsigned long user_id, int type, void *param, int size);

/**************************************************************************
* Function Name  : st_net_getHeartbeatStatus
* Description    : ��ȡ����״̬��Ϣ
* Parameters     : user_id(�û�ID)		
* Return Type    : ������       :   ����0;
*                  ������       :   ����1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_GETHEARTBEATSTATUS)(unsigned long user_id);

/**************************************************************************
* Function Name  : st_net_registerServerAlarmInfoCallback
* Description    : ������������Ϣע��ص����豸�˽���ALARM_INFO_CALLBACK�ص��з��ͱ�����Ϣ
* Parameters     : user_id(�û�ID)��callback(�ص�����)��context(������Ϣ)			
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_REGISTERSERVERALARMINFOCALLBACK)(unsigned long user_id, ALARM_INFO_CALLBACK callback, void *context);

/**************************************************************************
* Function Name  : st_net_openChannelStream
* Description    : ����Ƶͨ�����ӣ���������Ƶ����
* Parameters     : user_id(�û�ID)��channel(ͨ����:0��ʼ), 
				   protocol_type(Э������)��PROTOCOL_TCP(TCP)��PROTOCOL_UDP(UDP)��PROTOCOL_MCAST(�ಥ)
				   channel_type (ͨ������): 0(��ͨ��)��1(��ͨ��) 
				   realTime		(��Ƶ����ģʽ):0(ʵʱ)��1(����) //�ò���������ȥ��	
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_OPENCHANNELSTREAM)(unsigned long user_id, int channel, int protocol_type, int channel_type, int realTime);

/**************************************************************************
* Function Name  : st_net_startChannelStream
* Description    : ��ʼ����ͨ����Ƶ����,�����ȴ�����Ӧ��Ƶͨ������
* Parameters     : user_id(�û�ID)��channel(ͨ����:0��ʼ), 
				   stream_type (������): 0(������)��1(��Ƶ��)��2(��Ƶ��)
				   channel_type (ͨ������): 0(��ͨ��)��1(��ͨ��) 	
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_STARTCHANNELSTREAM)(unsigned long user_id, int channel, int stream_type, int channel_type);

/**************************************************************************
* Function Name  : st_net_registerChannelStreamCallback
* Description    : ע��ͨ�����ص����豸�˽���CHANNEL_STREAM_CALLBACK�ص��з���ͨ��������
* Parameters     : user_id(�û�ID)��channel(ͨ����)��callback(�ص�����)��context(������Ϣ)		
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_REGISTERCHANNELSTREAMCALLBACK)(unsigned long user_id, int channel, CHANNEL_STREAM_CALLBACK callback, void *context);

/**************************************************************************
* Function Name  : st_net_closeChannelStream
* Description    : �ر���Ƶͨ������
* Parameters     : user_id(�û�ID)��channel(ͨ����:0��ʼ), 
				   channel_type (ͨ������): 0(��ͨ��)��1(��ͨ��) 
				   realTime		(��Ƶ����ģʽ):0(ʵʱ)��1(����) //�ò���������ȥ��	
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
* Last Modified  : 
***************************************************************************/
typedef int (_stdcall* ST_NET_CLOSECHANNELSTREAM)(unsigned long user_id, int channel, int channel_type, int realTime);

/**************************************************************************
* Function Name  : st_net_userLogout
* Description    : ע����½
* Parameters     : user_id(�û�ID)			
* Return Type    : �ɹ�       :   ����0;
*                  ʧ��       :   ����-1;
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
