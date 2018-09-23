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
	char    m_companyName[32];//厂家名称

	char	m_proxyIp[32];	//转发服务器IP
	int		m_proxyPort;	//转发服务器端口
	char	m_userName[32];	//用户名
	char	m_passWord[32];	//密码
	char	m_srcUrl[64];	//流url

	char	m_deviceId[32];	//设备ID
	char	m_deviceType[32];//设备类型，dvr 121，dvs 117
	char	m_deviceChn[64];//设备通道
	char	m_subChn[64];	//副通道
	//char	m_deviceName[64];//设备名称

	char	m_dstIp[32];	//目标服务器IP
	char	m_dstPub[64];	//发布名
	int		m_dstStream;	//流下标


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

	//待编码数据缓冲取
	//CArray<MyAvFrame*,MyAvFrame*> p_Array;

	CArray<MyAvFrame*,MyAvFrame*> p_EncodeArray;
	CArray<MyAvFrame*,MyAvFrame*> p_YuvArray;

//上海三利
	//CNetvideoctrl1 *pNetVideo;
	enum CONVERT_DEV_TYPE m_devType;

	int b_stop;//标识该路是否需要停止推送数据，是从占用网络带宽的角度考虑。
				//-1 表示第一次启动，0表示该路已经启用，1表示该路持续一段时间没有观看，需要停止

	HANDLE m_hPullThread;
	HANDLE m_hPushThread;
	HANDLE m_hXmlThread;
	HANDLE m_hEncodeThread;

	//1024
	HANDLE h_encoder;

	int b_Encode;
	int b_PullLive;

	//rtmp发送
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

	//WMV拉流
	HANDLE h_stream;

	//add
	STARTUPINFO si; 
	PROCESS_INFORMATION pi; //进程信息：

	New_DEV_PARAMS m_DevParams;
	BYTE*p_PostData;
	//end add

public://为H264的解码使用
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
