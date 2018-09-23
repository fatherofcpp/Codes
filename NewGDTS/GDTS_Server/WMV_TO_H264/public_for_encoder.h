#ifndef _ENCODER_PUBLIC_H
#define _ENCODER_PUBLIC_H

#define STREAM_VIDEO				0
#define STREAM_AUDIO				1

typedef  struct { 
  WORD  wFormatTag; 
  WORD  nChannels; 
  DWORD nSamplesPerSec; 
  DWORD nAvgBytesPerSec; 
  WORD  nBlockAlign; 
  WORD  wBitsPerSample; 
  WORD  cbSize; 
} M_WAVEFORMATEX; 

typedef enum{
	TYPE_RGB24=0,
	TYPE_RGB32,
	TYPE_YV12,
	TYPE_NV12,
	TYPE_IMC2,
	TYPE_IMC4,
	TYPE_YUY2,
	TYPE_UYVY

}OUTFORMAT;





typedef struct
{
	DWORD   osdStructSize;
	DWORD	osdType;			//文字，时间，时间＋日期，位图
	DWORD	displayType;		//透明，半透明
	DWORD	frontColor;
	DWORD	backColor;

	DWORD	start_x;		//相对值
	DWORD	start_y;
	DWORD	osdWidth;
	DWORD	osdHeight;

	DWORD	osdDataLen;
	BYTE	*osdData;
}OSDTYPE;


typedef struct{
	DWORD width;
    DWORD height;
	DWORD bitrate;		//以K为单位  step
	DWORD quality;
	DWORD vbr;
	OUTFORMAT input_format;
    DWORD IFrame_distance;//间隔多少个帧后设置关键帧  extradata_size;
	DWORD framerate;
	//BYTE extradata[32];

}AVCodecContext2;





typedef struct
{

	DWORD saveTofileFlag;
	DWORD accordSize;
	DWORD accordTimePeriod;
	DWORD accordStartTime;
	CHAR *saveFileName;
}SAVEASFINFO;


//typedef BOOL (*VIDEOCALLBACK)(AVPictureOut *pOut,VOID *CallBackParams);
//typedef BOOL (*WARNNINGMSGCALLBACK)(CHAR *CallBackParams);
typedef BOOL (*WARNNINGMSGCALLBACK)(CHAR *msg,DWORD msgType,VOID *CallBackParams);
#define MAXVIDOENUMS	2

// typedef struct 
// {
// 
// 
// 	DWORD			Audio_on_nums;						//声音线程
// 	DWORD			Video_on_nums;						//视频线程
// 
// 	DWORD			sendBuffSize;					//接收流的缓冲区大小
// 
// 	CHAR			Url_Name[256];							//服务器的URL地址、端口和上传文件名
// 	CHAR			Directory_Name[128];
// 
// 	M_WAVEFORMATEX	audioParams[MAXVIDOENUMS];
// 	AVCodecContext  videoParams[MAXVIDOENUMS];
// 
// 	WARNNINGMSGCALLBACK	WarnningMsgFunc;				//告警信息回调函数；不用设为NULL
// 
// }ENCODERSTREAMPARAMS;
typedef struct 
{


	DWORD			Audio_on_nums;						//声音线程
	DWORD			Video_on_nums;						//视频线程

	DWORD			sendBuffSize;					//接收流的缓冲区大小

	CHAR			Url_Name[256];							//服务器的URL地址、端口和上传文件名
	CHAR			Directory_Name[128];

	M_WAVEFORMATEX	audioParams[MAXVIDOENUMS];
	AVCodecContext2  videoParams[MAXVIDOENUMS];

	WARNNINGMSGCALLBACK	WarnningMsgFunc;				//告警信息回调函数；不用设为NULL
	VOID			*CallBackParams;
	DWORD			timeStampType;//时间戳类型	0:编码器自动根据实际获得数据的时间计算时间戳
	//			1:根据帧率计算时间戳
	//			2:使用用户自己计算的时间戳（每帧时间）
	//			3:使用用户自己计算的时间戳（累计时间）

	DWORD			videoBuffNums;//视频缓冲区的个数（2-5个）
	DWORD			audioBuffNums;//声音缓冲区的个数（2-5个）


}ENCODERSTREAMPARAMS;
#endif _ENCODER_PUBLIC_H