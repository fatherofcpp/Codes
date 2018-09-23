/********************************************************************
	created:	2014/06/11
	filename: 	rtspLib.h
	file ext:	h
	author:		zhh
	
	purpose:	在LIVE555的基础上加入了回调函数等功能，完成RTSP的拉流和推流功能
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
	void     *RtspClient;  //PUSH:为FramedSource*; PULL:为RTSPClient*


}DSS_RTSP;



typedef  struct _spsdata {
      unsigned  char *data;
      int       size;
} extra_spsdata;

/**
 *
 * 函数描述: H264裸数据的回调函数
 * 创建时间：2013-12-25
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：
 * 输出:
		data: h264 RAW数据存放的地址，不包含【00 00 00 01】4个数据
		datasize: h264 RAW数据的大小
		framewidth：该h264数据图像的宽度
		frameheight：该h264数据图像的宽度
		timestamp：时间戳
		keyflag:是否为关键帧
		extra_data：SPS和PPS数据包
		extra_data_size：SPS和PPS数据包大小
		lUserParam：用户数据，为dss_live555_pullstart第一个参数的指针
 * 返回 ：返回0 成功， -1失败
 */
typedef void ( *metadata_callback_f)(unsigned char *data,int datasize, int framewidth ,int frameheight ,  MU_INT64 timestamp, int keyflag,  VIDEOFRAMETYPE frametype ,unsigned char *extra_data ,int extra_data_size,   long lUserParam);

typedef void ( *decdata_callback_f )(unsigned char *data, int datasize, int framewidth ,int frameheight , MU_INT64 timestamp, int keyflag, VIDEOFRAMETYPE frametype ,unsigned char *extra_data ,int extra_data_size,    long lUserParam );

/**
 *
 * 函数描述: RTSP推流操作中H264数据输入的回调函数，每一次都需要传入，否则在回调中无法得到需要的用户数
 * 创建时间：2014-1-9
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：
 * 输出:
		fTo: 存放h264 RAW数据的空间,已经分配完空间
		FrameSize: 存放h264 RAW数据的空间的最大值
		fNumTruncatedBytes:保留
		lUserParam：用户数据，为dss_live555_pushstart第一个参数的指针
 * 返回 ：返回0 成功， -1失败
 */
typedef int (*pushrtsp_callback)(unsigned char* fTo, unsigned *FrameSize , unsigned *fNumTruncatedBytes,  long  lUserParam); 


/**
 *
 * 函数描述: 拉取rtsp流前的全局初始化，只进行一次
 * 创建时间：2013-12-25
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：p_st
 * 输出:
 * 返回 ：返回0 成功， -1失败
 */
typedef int (__stdcall* DSS_LIVE555_PULLINIT)( );


/**
 *
 * 函数描述: 拉取rtsp流操作
 * 创建时间：2013-12-25
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：	p_st，			DSS_RTSP结构指针，包含所需要的RTSP URL信息
			p_CallBack，	h264数据回调函数
			p_CallBackDec   YUV数据回调函数
 * 输出:
 * 返回 ：返回0 成功， -1失败
 */
typedef int (__stdcall* DSS_LIVE555_PULLSTART)(void* p_st, void* p_CallBack,  void* p_CallBackDec);


/**
 *
 * 函数描述: 全局退出函数，参数为NULL
 * 创建时间：2013-12-25
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：	p_st，			DSS_RTSP结构指针
			
 * 输出:
 * 返回 ：返回0 成功， -1失败
 */
typedef int (__stdcall* DSS_LIVE555_PULLEXIT)();


/**
 *
 * 函数描述: 关闭RTSP拉流操作
 * 创建时间：2013-12-25
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：	p_st，			DSS_RTSP结构指针
			
 * 输出:
 * 返回 ：返回0 成功， -1失败
 */
typedef int (__stdcall* DSS_LIVE555_PULLSTOP)(void *p_st);

/**
 *
 * 函数描述: 全局初始化RTSP推流操作
 * 创建时间：2014-1-9
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：	
			
 * 输出:
 * 返回 ：返回0 成功， -1失败
 */
 typedef int (__stdcall* DSS_LIVE555_PUSHINIT)();

 /**
 *
 * 函数描述: 启动RTSP推流操作
 * 创建时间：2014-1-9
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：	p_st，			DSS_RTSP结构指针，必须包含RTSP的URL地址。
							同时H264输入回调也要注册。
			
 * 输出:
 * 返回 ：返回0 成功， -1失败
 */
 typedef int (__stdcall* DSS_LIVE555_PUSHSTART)(void* p_st);


 /**
 *
 * 函数描述: 停止RTSP推流操作
 * 创建时间：2014-1-9
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：p_st，			DSS_RTSP结构指针。	
			
 * 输出:
 * 返回 ：返回0 成功， -1失败
 */
 typedef int (__stdcall* DSS_LIVE555_PUSHSTOP)(void* p_st);


 /**
 *
 * 函数描述: 全局销毁RTSP推流
 * 创建时间：2014-1-9
 * 创建人:   
 * 修改日期
 * 版本号  ：V1.1
 * 修改人
 * 修改内容
 * 修改日期
 * 入参：	
			
 * 输出:
 * 返回 ：返回0 成功， -1失败
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
