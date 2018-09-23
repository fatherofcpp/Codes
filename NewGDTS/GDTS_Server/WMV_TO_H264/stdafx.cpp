#include "stdafx.h"


//以下信息是从config.ini配置文件中读取，如果读取有误，将采用该默认值。
int g_rtmpPort=1935;//rtmp协议发送到流服务器服务器的端口
int g_maxLiveNum=16;//该设备支持的最大路数
int g_ListenPort=9999;//该WEB默认监听的端口
int g_PtzPort=9876;//PTZ监听的端口
int g_BitRate=500;//
int g_VideoWidth=352;//
int g_VideoHeight=288;//
int g_bUseTc=0;//全局的设置是否需要转码，针对H264是否符合要求,如手机上看则全需要转码操作.
int g_PubType=0;//0:RTMP 1:mss
int g_bStop=0;
int g_FramePort=10000;//监听拉流端的帧
char g_FrameIp[64]="127.0.0.0";

CH264Stream *m_h264[16];
int SourceCount=0;

CString g_CurrentDir;

CONVERT_DEV_TYPE g_csType;