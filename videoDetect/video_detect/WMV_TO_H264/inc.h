#pragma once




#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"libjpeg.lib")

#ifdef __cplusplus 
extern "C" { 
#endif

//#include <avcodec.h>
//#include <avformat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
//#include <swscale.h>
#include <mmsystem.h>
#include <process.h>
#include "public_for_encoder.h"
	#include "IntSiVa_Common.h"
#include "YUVTOBMP.h"

#include "jpeglib.h"


#define MAXBUFFSIZE 1024*1024*5
typedef struct AVPictureOut {
	BYTE *data[3];								//data[0]����Ƶ������ʼ��ַ
	DWORD linesize[3];							//
	DWORD  datasize;							//�������Ƶ���ݳ���
	DWORD  width;								//��Ƶ���
	DWORD  height;								//��Ƶ�߶�
	long  timeStamp;							//��ǰ��Ƶʱ���
	long  deltTime;								//
} AVPictureOut;

typedef BOOL (*VIDEOCALLBACK)(AVPictureOut *pOut,VOID *CallBackParams);
typedef BOOL (*WARNNINGMSGCALLBACK)(CHAR *msg,DWORD msgType,VOID *CallBackParams);
typedef struct 
{

	HWND			hWnd;

	DWORD			Audio_on_flag;						//�������߳�
	DWORD			Video_on_flag;						//����Ƶ�߳�
	DWORD			Audio_is_Play;						//�Ƿ񲥷�����
	DWORD			Audio_Delay_Packet;					//�����ӳٲ��Ŷ��ٸ�����һ����Ϊ2��4��
	DWORD			TimeAdjustFlag;						//�Ƿ����ʱ�����������ʱ��
	DWORD			balance;							//��Ƶʵʱ�Ժ�������ƽ��ֵ��50��600����600ʵʱ��ǿ��50�����Ժ�

	DWORD			receiveBuffSize;					//�������Ļ�������С

	CHAR			*Url_Name;							//����URL��ַ

	OUTFORMAT		VideoFormat;						//��Ƶ������ͼ���ʽ
	RECT			videoDisplayRect;
	DWORD			VideoRend_flag;						//�Ƿ���ʾ��Ƶͼ��
	VIDEOCALLBACK	VideoCallBack;						//��Ƶ����ص�������ÿ���һ֡��Ƶ��ص��˺�����������ΪNULL
	VOID			*CallBackParams;					//�ص������Ĳ���

	WARNNINGMSGCALLBACK	WarnningMsgFunc;				//�澯��Ϣ�ص�������������ΪNULL

}STREAMPARAMS;
static AVFrame *alloc_picture(int pix_fmt, int width, int height)
{
	AVFrame *picture;
	uint8_t *picture_buf;
	int size;
#if 1
	picture = avcodec_alloc_frame();
	if (!picture)
		return NULL;
	size = avpicture_get_size(pix_fmt, width, height);
	picture_buf =(uint8_t*) av_malloc(size);
	if (!picture_buf) {
		av_free(picture);
		return NULL;
	}
	avpicture_fill((AVPicture *)picture, picture_buf,
		pix_fmt, width, height);
#endif
	return picture;
}
class MyAvFrame
{
public:
	MyAvFrame(int width,int height)
	{
		i_width=width;
		i_height=height;
		p_data=new BYTE[i_width*i_height*3/2+1];
		p_Ydata=new BYTE[i_width*i_height*1+1];

		ZeroMemory(p_data,i_width*i_height*3/2+1);
		ZeroMemory(p_Ydata,i_width*i_height*1+1);

		ZeroMemory(p_vidDet,sizeof(p_vidDet));
		ZeroMemory(pSenDat,sizeof(pSenDat));

		i_time=0;
	};
	~MyAvFrame()
	{
		if (NULL!=p_data)
		{
			delete[] p_data;
		}
		if (NULL!=p_Ydata)
		{
			delete[] p_Ydata;
		}

		i_time=0;
	};
	int i_width;//video width
	int i_height;//video height
	BYTE* p_data;//YUV
	BYTE* p_Ydata;//Yֵ
	char p_vidDet[9];//8 YiChang 1 StreamOn
	char pSenDat[1024];//send data
	DWORD i_time;
	int i_key;//1:I֡ 0����I֡
};


//CArray<MyAvFrame*,MyAvFrame*> p_Array;//p_Array[16];



typedef struct
{
	DWORD	codecId;
	M_WAVEFORMATEX mfx;
	DWORD extradata_size;
	BYTE  extradata[32];
}AUDIOCONTEXT;

typedef struct
{
	DWORD	codecId;
	DWORD width;
	DWORD height;
	DWORD extradata_size;
	BYTE  extradata[32];
	DWORD framerate;
}VIDEOCONTEXT;

typedef struct
{
	DWORD streamType;
	DWORD streamNo;
	DWORD bitRate;
	AUDIOCONTEXT audioContext;
	VIDEOCONTEXT videoContext;
}STREAMCONTEXT;
typedef struct
{
	DWORD			streamNums;
	STREAMCONTEXT	*streamInfo;
	VOID			*params;
}STREAMINFO_OUTPUT;
typedef struct
{
	BYTE           m_headerType; 	
	BYTE           m_packetType; 	
	BYTE           m_nChannel; 	
	LONG           m_nInfoField[4]; 	
	//LONG           m_nInfoField2;
}RtmpPacketHeader; 

typedef struct
{
	BYTE 					*pOut;
	DWORD 					Outlen;
	DWORD  					sequence;
	DWORD  					keyframe;
	DWORD 					timeStamp;
	DWORD 					frameNums;
	DWORD					bufferSize;
	DWORD					sizesInBuffer;
	RtmpPacketHeader		rtmpPackets;
}FrameOut;


typedef HANDLE (*STREAMPROCESSOPEN)(STREAMPARAMS *streamParams,BOOL *status);
typedef BOOL (*STREAMPROCESSSTART)(HANDLE pstreamTask);
typedef VOID (*STREAMPROCESSCLOSE)(HANDLE pstreamTask);
extern STREAMPROCESSOPEN ProcessOpen;
extern STREAMPROCESSSTART ProcessStart;
extern STREAMPROCESSCLOSE ProcessClose;

//extern HANDLE m_ffmpegmutex;
#define DEFAULTWIDTH 704 //352 //
#define DEFAULTHEIGHT 576 //288 //
#include "Log.h"
extern CLog m_Log;
extern CLog SenLog;
//video detect struct
typedef	struct
{
	//IntSiVa_General_API* APP[5];

	IntSiVa_InPut Param;//height width YiChangXiShu
	IntSiVa_OutPut Out;//output
	IntSiVa_InPutFrame Input;//GrayData
}VideoDetect;
extern VideoDetect AVidDet[MAX_DETECT_NUM];

extern IntSiVa_General_API* APP[MAX_DETECT_NUM];//32//1


//end video detect struct
//http
typedef struct
{
	char SerNam[255];//path of local service
}ConfParam;//http config param
typedef struct 
{
	char DiuShi[16]; 
	char ZheDang[16]; 
	char DuiJiao[16]; 
	char LiangDu[16]; 
	char DongJie[16]; 
	char XueHua[16]; 
	char ShanShuo[16]; 
	char GunWen[16];

	char CmdId[64];
	char SrvIP[64];
	char SrvPort[16];
	char StreamName[64];
	char StreamProtocol[64];
}_YiChang;//information of post 


//end http


#ifdef __cplusplus 
} 
#endif
