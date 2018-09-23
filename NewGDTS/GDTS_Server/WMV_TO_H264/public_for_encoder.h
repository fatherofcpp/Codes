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
	DWORD	osdType;			//���֣�ʱ�䣬ʱ�䣫���ڣ�λͼ
	DWORD	displayType;		//͸������͸��
	DWORD	frontColor;
	DWORD	backColor;

	DWORD	start_x;		//���ֵ
	DWORD	start_y;
	DWORD	osdWidth;
	DWORD	osdHeight;

	DWORD	osdDataLen;
	BYTE	*osdData;
}OSDTYPE;


typedef struct{
	DWORD width;
    DWORD height;
	DWORD bitrate;		//��KΪ��λ  step
	DWORD quality;
	DWORD vbr;
	OUTFORMAT input_format;
    DWORD IFrame_distance;//������ٸ�֡�����ùؼ�֡  extradata_size;
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
// 	DWORD			Audio_on_nums;						//�����߳�
// 	DWORD			Video_on_nums;						//��Ƶ�߳�
// 
// 	DWORD			sendBuffSize;					//�������Ļ�������С
// 
// 	CHAR			Url_Name[256];							//��������URL��ַ���˿ں��ϴ��ļ���
// 	CHAR			Directory_Name[128];
// 
// 	M_WAVEFORMATEX	audioParams[MAXVIDOENUMS];
// 	AVCodecContext  videoParams[MAXVIDOENUMS];
// 
// 	WARNNINGMSGCALLBACK	WarnningMsgFunc;				//�澯��Ϣ�ص�������������ΪNULL
// 
// }ENCODERSTREAMPARAMS;
typedef struct 
{


	DWORD			Audio_on_nums;						//�����߳�
	DWORD			Video_on_nums;						//��Ƶ�߳�

	DWORD			sendBuffSize;					//�������Ļ�������С

	CHAR			Url_Name[256];							//��������URL��ַ���˿ں��ϴ��ļ���
	CHAR			Directory_Name[128];

	M_WAVEFORMATEX	audioParams[MAXVIDOENUMS];
	AVCodecContext2  videoParams[MAXVIDOENUMS];

	WARNNINGMSGCALLBACK	WarnningMsgFunc;				//�澯��Ϣ�ص�������������ΪNULL
	VOID			*CallBackParams;
	DWORD			timeStampType;//ʱ�������	0:�������Զ�����ʵ�ʻ�����ݵ�ʱ�����ʱ���
	//			1:����֡�ʼ���ʱ���
	//			2:ʹ���û��Լ������ʱ�����ÿ֡ʱ�䣩
	//			3:ʹ���û��Լ������ʱ������ۼ�ʱ�䣩

	DWORD			videoBuffNums;//��Ƶ�������ĸ�����2-5����
	DWORD			audioBuffNums;//�����������ĸ�����2-5����


}ENCODERSTREAMPARAMS;
#endif _ENCODER_PUBLIC_H