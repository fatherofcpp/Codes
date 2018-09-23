/********************************************************************
	created:	2014/03/27
	filename: 	vnmpLib.h
	file base:	VideoNetSDK
	file ext:	h
	author:		zhh
	
	purpose:	ʵ�ֶ�̬���س�����Զ��̬�⣬�����ڳ����������������������
				Ŀǰʵ�ֶ�̬���ص���Ҫ����VideoNetSDK.dll��VideoNetPlay.dll
				�����ļ�
*********************************************************************/

#ifndef DEF_VNMPLIB_H_
#define DEF_VNMPLIB_H_


#include "./vnmp/include/VideoDeviceSDK.h"
#include "./vnmp/include/VideoNetSDK.h"
#include "./vnmp/include/VideoNetPlay.h"
//#include "./np_sdk/include/np_playsdk.h"

//#pragma comment(lib,"./np_sdk/lib/np_netsdk.lib")
//#pragma comment(lib,"./np_sdk/lib/np_playsdk.lib")

typedef struct tagDEV_VNMP_INFO{
	char		m_dstIp[32];		//Ŀ�������IP
	char		m_dstPub[64];		//������
	long		m_dstStream;	//���±�
	long		m_loginHandle;
	long		m_videoHandle;


}DEV_VNMP_INFO;

typedef struct tagDEV_VNMP{//�������ʹ�ò����ṹ
	char		m_proxyIp[64];		//ת��������IP
	long		m_proxyPort;		//ת���������˿�
	char		m_userName[32];		//�û���
	char		m_passWord[32];		//����
	
	char		m_cameraId[64];		//�����Ψһ��ʶID

	DEV_VNMP_INFO m_devInfo;


}DEV_VNMP_PARAMS;



typedef int (__stdcall* VIDEO_DEVICE_INIT)();
typedef int (__stdcall* VIDEO_DEVICE_DESTROY)();
typedef int (__stdcall* VIDEO_DEVICE_LOGIN)(VIDEODEVICE *videodev);
typedef int (__stdcall* VIDEO_DEVICE_LOGOUT)(VIDEODEVICE *videodev);
typedef int (__stdcall* VIDEO_DEVICE_CAPTURING_START)(VIDEODEVICE *videodev,
													 unsigned long user, VIDEO_CAPTURE_CALLBACK capture_callback);
typedef int (__stdcall* VIDEO_DEVICE_CAPTURING_STOP)(VIDEODEVICE *videodev);

typedef int (__stdcall* VIDEO_DEVICE_CAMERACONTROL)(VIDEODEVICE *videodev, int ControlType, int ControlSpeed);

typedef int (__stdcall* VIDEO_PLAY_SETSTREAMOPENMODE)(int nPort, DWORD nMode);
typedef int (__stdcall* VIDEO_PLAY_SETDISPLAYBUF)(int nPort, DWORD nNum);
typedef int (__stdcall* VIDEO_PLAY_SETBUFFERTYPE)(int nPort, int BufferType);
typedef int (__stdcall* VIDEO_PLAY_SETYUVCALLBACK)(int nPort, DecCBFun yuv_callback);
typedef int (__stdcall* VIDEO_PLAY_PLAY)(int nPort, unsigned char *buffer,int bsize, HWND hWnd);
typedef void (__stdcall* VIDEO_PLAY_INPUTVIDEODATA)(int nPort,DWORD nSize, PBYTE pBuf);
typedef int (__stdcall* VIDEO_PLAY_STOP)(int nPort);


extern VIDEO_DEVICE_INIT				video_device_init;
extern VIDEO_DEVICE_DESTROY				video_device_destroy;
extern VIDEO_DEVICE_LOGIN				video_device_login;
extern VIDEO_DEVICE_LOGOUT				video_device_logout;
extern VIDEO_DEVICE_CAPTURING_START		video_device_capturing_start;
extern VIDEO_DEVICE_CAPTURING_STOP		video_device_capturing_stop;

extern VIDEO_DEVICE_CAMERACONTROL		video_device_cameracontrol;

extern VIDEO_PLAY_SETSTREAMOPENMODE		video_play_SetStreamOpenMode;
extern VIDEO_PLAY_SETDISPLAYBUF			video_play_SetDisplayBuf;
extern VIDEO_PLAY_SETBUFFERTYPE			video_play_SetBufferType;
extern VIDEO_PLAY_SETYUVCALLBACK		video_play_SetYUVCallBack;
extern VIDEO_PLAY_PLAY					video_play_Play;
extern VIDEO_PLAY_INPUTVIDEODATA		video_play_InputVideoData;
extern VIDEO_PLAY_STOP					video_play_Stop;




int loadVNMPLib();

#endif
