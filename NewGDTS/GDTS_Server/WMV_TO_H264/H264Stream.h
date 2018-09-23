#pragma once
#include "inc.h"
#include "afxmt.h"
#include "netvideoctrl1.h"
#include "rtmp.h"

#include "shsl.h"

class StreamInfo
{
public:
	CString s_Company;
	CString s_DevType;
	CString s_ServerIP;
	CString s_PubServer;
	CString s_PubType;
	int		i_Width;
	int		i_Height;
	CString s_PubName;
	CString s_DevID;
	CString s_CH;
	CString s_Pipe;
	StreamInfo(void)
	{
		s_Company="microvideo";
		s_DevType="mss";
		s_ServerIP="";
		s_PubServer="";
		s_PubType="wowza";
		i_Width=352;//352;720
		i_Height=288;//288;
		s_PubName="";
		s_DevID="";
		s_CH="";
		s_Pipe="";
	}
};

typedef struct FILEINFORMATION{
	char fileName[32];
	int fileLen;
	int offset;
	char *pData;
	int  bRun;
}FileInformation;



typedef struct stNew_DEV_PARAMS{
	char    m_companyName[32];//��������

	char	m_proxyIp[32];	//ת��������IP
	int		m_proxyPort;	//ת���������˿�
	char	m_userName[32];	//�û���
	char	m_passWord[32];	//����
	char	m_srcUrl[64];	//��url

	char	m_deviceId[32];	//�豸ID
	char	m_deviceType[32];//�豸���ͣ�dvr 121��dvs 117
	char	m_deviceChn[64];//�豸ͨ��
	char	m_subChn[64];	//��ͨ��
	//char	m_deviceName[64];//�豸����

	char	m_dstIp[32];	//Ŀ�������IP
	char	m_dstPub[64];	//������
	int		m_dstStream;	//���±�


}New_DEV_PARAMS;

class CH264Stream
{
public:
	CH264Stream(void);
	~CH264Stream(void);
	
	BYTE* encodebuff;
	BYTE* decodebuff;
	CCriticalSection m_streamlock;
//	HANDLE h_encoder;
//	HANDLE h_stream;

	//���������ݻ���ȡ
	//CArray<MyAvFrame*,MyAvFrame*> p_Array;

	CArray<MyAvFrame*,MyAvFrame*> p_EncodeArray;
	CArray<MyAvFrame*,MyAvFrame*> p_YuvArray;

//�Ϻ�����
	//CNetvideoctrl1 *pNetVideo;
	enum CONVERT_DEV_TYPE m_devType;

	int b_stop;//��ʶ��·�Ƿ���Ҫֹͣ�������ݣ��Ǵ�ռ���������ĽǶȿ��ǡ�
				//-1 ��ʾ��һ��������0��ʾ��·�Ѿ����ã�1��ʾ��·����һ��ʱ��û�йۿ�����Ҫֹͣ

	HANDLE m_hPullThread;
	HANDLE m_hPushThread;
	HANDLE m_hXmlThread;
	HANDLE m_hEncodeThread;

	//1024
	HANDLE h_encoder;

	int b_Encode;
	int b_PullLive;

	//rtmp����
	CRTMP g_rtmp;
//	int i_Width;
//	int i_Height;
	bool first;
	bool b_start;
	DWORD nowtime;
	DWORD oldtime;
	DWORD usetime;
	StreamInfo m_Info;

	LONGLONG curTime;
	bool b_restart;
	int b_novideo;
	FileInformation m_novideoFile;

	int thisIndex;

	//WMV����
	HANDLE h_stream;

	//add
	STARTUPINFO si; 
	PROCESS_INFORMATION pi; //������Ϣ��

	New_DEV_PARAMS m_DevParams;
	BYTE*p_PostData;
	//end add

public://ΪH264�Ľ���ʹ��
	AVFrame *YUV420P_picture2;
	AVFormatContext *oc2;
	AVStream *st2;
	AVCodecContext *c2;
	AVCodec *codec2;

private:
	AVFrame *YUV420P_picture;
	AVFormatContext *oc;
	AVStream *st;
	AVCodecContext *c;
	AVCodec *codec;
	ENCODERSTREAMPARAMS encoderparams;

	STREAMINFO_OUTPUT Params;

public:

	//int Init(CString SrcUrl,CString ServerIP,CString PubName,int StreamNumber,HWND m_hwnd);
	int initDeH264(int Width, int Height);
	int decoderH264(BYTE *dataIn,DWORD dataSize,BYTE* dataOut,DWORD *outLen);
	int destroyDeH264();

	int Encode(BYTE * p_inputbuff, BYTE* p_outputbuff);
	int Encode(BYTE* p_outputbuff);
	int StartPush(int Width, int Height);
	int StartPull(void *pStream, void *pParams);
	//int StartGet(void);
	//1024
	VOID enocderInit(WARNNINGMSGCALLBACK	WarnningMsgFunc);
	BOOL encoderOpen();
	BOOL encoderStart();
	VOID encoderStop();
	BOOL encoderPut(BYTE *dataIn,DWORD dataSize,DWORD metiaType);
	BOOL encoderPut(DWORD metiaType);	

};
