/********************************************************************
	created:	2014/05/05
	filename: 	dhLib.h
	file ext:	h
	author:		zhh
	
	purpose:	ʵ�ֶ�̬�����㽭�󻪶�̬�⣬�����ڳ����������������������
				Ŀǰʵ�ֶ�̬���ص���Ҫ����dhnetsdk.lib��dhplay.lib
				�����ļ�
*********************************************************************/

#ifndef DEF_DHLIB_H_
#define DEF_DHLIB_H_


#include "./dhLib/dhnetsdk.h"
#include "./dhLib/dhplay.h"




typedef BOOL (CALL_METHOD* CLIENT_INIT)(fDisConnect cbDisConnect, LDWORD dwUser);

// ���ö�̬�����Ӷ��߻ص�������ĿǰSVR�豸�ļ��Ӻͻط��Ƕ����ӵġ�
typedef void (CALL_METHOD* CLIENT_SETSUBCONNCALLBACK)(fSubDisConnect cbSubDisConnect, LDWORD dwUser);

// ���ö��������ɹ��ص����������ú�SDK�ڲ������Զ�����
typedef void (CALL_METHOD* CLIENT_SETAUTORECONNECT)(fHaveReConnect cbAutoConnect, LDWORD dwUser);

// ���豸ע��
typedef LLONG (CALL_METHOD* CLIENT_LOGIN)(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error );

//------------------------------------------------------------------------
// ����: PLAY_OpenStream
// ����: ��������
// ����: nPort,ͨ����
//		 pFileHeadBuf,Ŀǰ��ʹ��,��NULL.
//		 nSize,Ŀǰ������,��0.
//		 nBufPoolSize,���ò������д���������Ļ�������С.
//			��Χ��[SOURCE_BUF_MIN,SOURCE_BUF_MAX].һ����Ϊ900*1024�������
//			���͹�����Ծ���,�ɵ�С��ֵ,������ݴ��䲻����,�������ֵ.
// ����: BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_OPENSTREAM)(LONG nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);

//------------------------------------------------------------------------
// ����: PLAY_Play
// ����: ��ʼ����.����Ѿ�����,�ı䵱ǰ����״̬Ϊ�����ٶȲ���.
// ����: nPort,ͨ����
//		 hWnd,���Ŵ��ھ��
// ����: BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_PLAY)(LONG nPort, HWND hWnd);

// ��ʼʵʱ����--��չ
typedef LLONG (CALL_METHOD* CLIENT_REALPLAYEX)(LLONG lLoginID, int nChannelID, HWND hWnd, DH_RealPlayType rType);

// ����ʵʱ�������ݻص�--��չ
typedef BOOL (CALL_METHOD* CLIENT_SETREALDATACALLBACKEX)(LLONG lRealHandle, fRealDataCallBackEx cbRealData, LDWORD dwUser, DWORD dwFlag);

//------------------------------------------------------------------------
// ����: PLAY_Stop
// ����: ֹͣ����
// ����: nPort,ͨ����
// ����: BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_STOP)(LONG nPort);

//------------------------------------------------------------------------
// ����: PLAY_CloseStream
// ����: �ر���
// ����: nPort,ͨ����
// ����: BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_CLOSESTREAM)(LONG nPort);

// ֹͣʵʱԤ��--��չ
typedef BOOL (CALL_METHOD* CLIENT_STOPREALPLAYEX)(LLONG lRealHandle);

// ���豸ע��
typedef BOOL (CALL_METHOD* CLIENT_LOGOUT)(LLONG lLoginID);

// SDK�˳�����
typedef void (CALL_METHOD* CLIENT_CLEANUP)();

//------------------------------------------------------------------------
// ����: PLAY_GetLastError
// ����: ��ȡ������
// ����: nPort,ͨ����
// ����: DWORD,��õ�ǰ����Ĵ�����.��μ�������궨��.
//------------------------------------------------------------------------
typedef DWORD (CALLMETHOD* PLAY_GETLASTERROR)(LONG nPort);

// ���غ���ִ��ʧ�ܴ���
typedef DWORD (CALL_METHOD* CLIENT_GETLASTERROR)(void);
// ֹͣʵʱԤ��
typedef BOOL (CALL_METHOD* CLIENT_STOPREALPLAY)(LLONG lRealHandle);

//------------------------------------------------------------------------
// ����: PLAY_InputData
// ����: ����������,PLAY_Play֮��ʹ��
// ����: nPort,ͨ����
//		 pBuf,��������ַ
//		 nSize,��������С
// ����: BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE.��ʧ��,һ���ǻ���������,�û�����ͣ
//			���룬һ��ʱ��֮������������ȷ�����ſⲻ��ʧ���ݡ�
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_INPUTDATA)(LONG nPort,PBYTE pBuf,DWORD nSize);

//------------------------------------------------------------------------
// ����: PLAY_SetStreamOpenMode
// ����: ����������ģʽ,�����ڲ���֮ǰ����.
// ����: nPort,ͨ����
//		 nMode,STREAME_REALTIMEʵʱģʽ(Ĭ��);STREAME_FILE�ļ�ģʽ.ʵʱģ
//			ʽ,�ʺϲ�������ʵʱ����;�ļ�ģʽ,�ʺ��û����ļ�����������ʽ����.
// ����: BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE.
//------------------------------------------------------------------------
typedef BOOL (CALLMETHOD* PLAY_SETSTREAMOPENMODE)(LONG nPort,DWORD nMode);

//------------------------------------------------------------------------
// ����: PLAY_SetDecCallBackEx
// ����: ���ý���ص�,�滻�������е���ʾ����,���û��Լ�������ʾ,�ú�����
//			PLAY_Play֮ǰ����,��PLAY_Stopʱ�Զ�ʧЧ,�´ε���PLAY_Play֮ǰ
//			��Ҫ��������.���벿�ֲ������ٶ�,ֻҪ�û��ӻص������з���,������
//			�ͻ������һ��������.������ֻ���벻��ʾ�����Ρ�
// ����: nPort,ͨ����
//		 DecCBFun,����ص�����ָ��,����ΪNULL.�ص�����������������:
//			nPort,ͨ����
//			pBuf,����������Ƶ����
//			nSize,����������Ƶ����pBuf�ĳ���
//			pFrameInfo,ͼ���������Ϣ,��μ�FRAME_INFO�ṹ��
//			nReserved1,�û��Զ������
//			nReserved2,��������	
//		 nUser,�û��Զ������
// ����: BOOL,�ɹ�����TRUE,ʧ�ܷ���FALSE.
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
