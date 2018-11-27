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
	BYTE* p_Ydata;//Y值
	char p_vidDet[9];//8 YiChang 1 StreamOn
	char pSenDat[1024];//send data
	DWORD i_time;
	int i_key;//1:I帧 0：非I帧
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
