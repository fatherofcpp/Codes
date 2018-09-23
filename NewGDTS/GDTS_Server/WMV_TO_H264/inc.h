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
	BYTE *data[3];								//data[0]：视频数据起始地址
	DWORD linesize[3];							//
	DWORD  datasize;							//解码后视频数据长度
	DWORD  width;								//视频宽度
	DWORD  height;								//视频高度
	long  timeStamp;							//当前视频时间戳
	long  deltTime;								//
} AVPictureOut;

typedef BOOL (*VIDEOCALLBACK)(AVPictureOut *pOut,VOID *CallBackParams);
typedef BOOL (*WARNNINGMSGCALLBACK)(CHAR *msg,DWORD msgType,VOID *CallBackParams);
typedef struct 
{

	HWND			hWnd;

	DWORD			Audio_on_flag;						//打开声音线程
	DWORD			Video_on_flag;						//打开视频线程
	DWORD			Audio_is_Play;						//是否播放声音
	DWORD			Audio_Delay_Packet;					//声音延迟播放多少个包，一般设为2－4；
	DWORD			TimeAdjustFlag;						//是否根据时间戳调整播放时间
	DWORD			balance;							//视频实时性和流畅性平衡值（50－600），600实时性强，50流畅性好

	DWORD			receiveBuffSize;					//接收流的缓冲区大小

	CHAR			*Url_Name;							//流的URL地址

	OUTFORMAT		VideoFormat;						//视频解码后的图像格式
	RECT			videoDisplayRect;
	DWORD			VideoRend_flag;						//是否显示视频图像
	VIDEOCALLBACK	VideoCallBack;						//视频解码回调函数：每解出一帧视频会回调此函数；不用设为NULL
	VOID			*CallBackParams;					//回调函数的参数

	WARNNINGMSGCALLBACK	WarnningMsgFunc;				//告警信息回调函数；不用设为NULL

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
	int i_key;//1:I帧 0：非I帧
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
