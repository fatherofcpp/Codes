#pragma once



#if 0
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avcodec.lib")
#endif

#ifdef __cplusplus 
extern "C" { 
#endif

#include <avcodec.h>
#include <avformat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <avformat.h>
#include <swscale.h>
#include <mmsystem.h>
#include <process.h>
#include "public_for_encoder.h"

#include "avLib.h"
#include "gd_errorType.h"



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
	picture = GDavcodec_alloc_frame();
	if (!picture)
		return NULL;
	size = GDavpicture_get_size(pix_fmt, width, height);
	picture_buf =(uint8_t*) GDav_malloc(size);
	if (!picture_buf) {
		GDav_free(picture);
		return NULL;
	}
	GDavpicture_fill((AVPicture *)picture, picture_buf,
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
		ZeroMemory(p_data,i_width*i_height*3/2+1);
		i_time=0;
	};
	~MyAvFrame()
	{
		if (NULL!=p_data)
		{
			delete[] p_data;
		}
		i_time=0;
	};
	int i_width;
	int i_height;
	BYTE* p_data;
	DWORD i_time;
	int i_key;//1:I֡ 0����I֡
	int i_realsize;
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
//1024
typedef HANDLE (*ENCODERSTREAMPROCESSOPEN)(ENCODERSTREAMPARAMS *streamParams,BOOL *status);
typedef BOOL (*ENCODERSTREAMPROCESSSTART)(HANDLE pstreamTask);
typedef VOID (*ENCODERSTREAMPROCESSCLOSE)(HANDLE pstreamTask);
typedef BOOL (*ENCODERSTREAMPROCESSPUTDATA)(HANDLE pstreamTask,BYTE *dataIn,DWORD dataSize,DWORD metiaType,DWORD index);


extern STREAMPROCESSOPEN ProcessOpen;
extern STREAMPROCESSSTART ProcessStart;
extern STREAMPROCESSCLOSE ProcessClose;

//1024
extern ENCODERSTREAMPROCESSOPEN	encoderstreamProcessOpen;
extern ENCODERSTREAMPROCESSSTART	encoderstreamProcessStart;
extern ENCODERSTREAMPROCESSCLOSE	encoderstreamProcessClose;
extern ENCODERSTREAMPROCESSPUTDATA encoderstreamProcessPutDataAddress;
//extern HANDLE m_ffmpegmutex;
#define DEFAULTWIDTH 704 //352 //
#define DEFAULTHEIGHT 576 //288 //
#include "Log.h"
extern CLog m_Log;

#ifdef __cplusplus 
} 
#endif
