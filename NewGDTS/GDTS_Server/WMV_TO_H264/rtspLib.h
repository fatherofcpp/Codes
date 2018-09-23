/********************************************************************
	created:	2014/06/11
	filename: 	rtspLib.h
	file ext:	h
	author:		zhh
	
	purpose:	��LIVE555�Ļ����ϼ����˻ص������ȹ��ܣ����RTSP����������������
*********************************************************************/

#ifndef DEF_RTSPLIB_H_
#define DEF_RTSPLIB_H_

typedef unsigned long long  MU_INT64;
#define   VIDEOFRAMETYPE  int
#define   H264    1



typedef struct DSS_RTSP{

	char *s_Ver;
	char  *s_Type;
	char  rtspUrl[128]; //rtsp_url

	int rtsp_usetcp; 
	void *pushCallback;

	char      i_live555eventLoopWatchVariable; //for pull rtsp for pullthread  
	void     *RtspClient;  //PUSH:ΪFramedSource*; PULL:ΪRTSPClient*


}DSS_RTSP;



typedef  struct _spsdata {
      unsigned  char *data;
      int       size;
} extra_spsdata;

/**
 *
 * ��������: H264�����ݵĻص�����
 * ����ʱ�䣺2013-12-25
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�
 * ���:
		data: h264 RAW���ݴ�ŵĵ�ַ����������00 00 00 01��4������
		datasize: h264 RAW���ݵĴ�С
		framewidth����h264����ͼ��Ŀ��
		frameheight����h264����ͼ��Ŀ��
		timestamp��ʱ���
		keyflag:�Ƿ�Ϊ�ؼ�֡
		extra_data��SPS��PPS���ݰ�
		extra_data_size��SPS��PPS���ݰ���С
		lUserParam���û����ݣ�Ϊdss_live555_pullstart��һ��������ָ��
 * ���� ������0 �ɹ��� -1ʧ��
 */
typedef void ( *metadata_callback_f)(unsigned char *data,int datasize, int framewidth ,int frameheight ,  MU_INT64 timestamp, int keyflag,  VIDEOFRAMETYPE frametype ,unsigned char *extra_data ,int extra_data_size,   long lUserParam);

typedef void ( *decdata_callback_f )(unsigned char *data, int datasize, int framewidth ,int frameheight , MU_INT64 timestamp, int keyflag, VIDEOFRAMETYPE frametype ,unsigned char *extra_data ,int extra_data_size,    long lUserParam );

/**
 *
 * ��������: RTSP����������H264��������Ļص�������ÿһ�ζ���Ҫ���룬�����ڻص����޷��õ���Ҫ���û���
 * ����ʱ�䣺2014-1-9
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�
 * ���:
		fTo: ���h264 RAW���ݵĿռ�,�Ѿ�������ռ�
		FrameSize: ���h264 RAW���ݵĿռ�����ֵ
		fNumTruncatedBytes:����
		lUserParam���û����ݣ�Ϊdss_live555_pushstart��һ��������ָ��
 * ���� ������0 �ɹ��� -1ʧ��
 */
typedef int (*pushrtsp_callback)(unsigned char* fTo, unsigned *FrameSize , unsigned *fNumTruncatedBytes,  long  lUserParam); 


/**
 *
 * ��������: ��ȡrtsp��ǰ��ȫ�ֳ�ʼ����ֻ����һ��
 * ����ʱ�䣺2013-12-25
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�p_st
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
typedef int (__stdcall* DSS_LIVE555_PULLINIT)( );


/**
 *
 * ��������: ��ȡrtsp������
 * ����ʱ�䣺2013-12-25
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�	p_st��			DSS_RTSP�ṹָ�룬��������Ҫ��RTSP URL��Ϣ
			p_CallBack��	h264���ݻص�����
			p_CallBackDec   YUV���ݻص�����
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
typedef int (__stdcall* DSS_LIVE555_PULLSTART)(void* p_st, void* p_CallBack,  void* p_CallBackDec);


/**
 *
 * ��������: ȫ���˳�����������ΪNULL
 * ����ʱ�䣺2013-12-25
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�	p_st��			DSS_RTSP�ṹָ��
			
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
typedef int (__stdcall* DSS_LIVE555_PULLEXIT)();


/**
 *
 * ��������: �ر�RTSP��������
 * ����ʱ�䣺2013-12-25
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�	p_st��			DSS_RTSP�ṹָ��
			
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
typedef int (__stdcall* DSS_LIVE555_PULLSTOP)(void *p_st);

/**
 *
 * ��������: ȫ�ֳ�ʼ��RTSP��������
 * ����ʱ�䣺2014-1-9
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�	
			
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
 typedef int (__stdcall* DSS_LIVE555_PUSHINIT)();

 /**
 *
 * ��������: ����RTSP��������
 * ����ʱ�䣺2014-1-9
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�	p_st��			DSS_RTSP�ṹָ�룬�������RTSP��URL��ַ��
							ͬʱH264����ص�ҲҪע�ᡣ
			
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
 typedef int (__stdcall* DSS_LIVE555_PUSHSTART)(void* p_st);


 /**
 *
 * ��������: ֹͣRTSP��������
 * ����ʱ�䣺2014-1-9
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�p_st��			DSS_RTSP�ṹָ�롣	
			
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
 typedef int (__stdcall* DSS_LIVE555_PUSHSTOP)(void* p_st);


 /**
 *
 * ��������: ȫ������RTSP����
 * ����ʱ�䣺2014-1-9
 * ������:   
 * �޸�����
 * �汾��  ��V1.1
 * �޸���
 * �޸�����
 * �޸�����
 * ��Σ�	
			
 * ���:
 * ���� ������0 �ɹ��� -1ʧ��
 */
 typedef int (__stdcall* DSS_LIVE555_PUSHEXIT)();


//typedef BOOL (CALLMETHOD* PLAY_SETDECCALLBACKEX)(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2), long nUser);



extern DSS_LIVE555_PULLINIT			dss_live555_pullinit;
extern DSS_LIVE555_PULLSTART		dss_live555_pullstart;
extern DSS_LIVE555_PULLEXIT			dss_live555_pullexit;
extern DSS_LIVE555_PULLSTOP			dss_live555_pullstop;

extern DSS_LIVE555_PUSHINIT			dss_live555_pushinit;
extern DSS_LIVE555_PUSHSTART		dss_live555_pushstart;
extern DSS_LIVE555_PUSHSTOP			dss_live555_pushstop;
extern DSS_LIVE555_PUSHEXIT			dss_live555_pushexit;

int loadRTSPLib(const char *dir);

#endif
