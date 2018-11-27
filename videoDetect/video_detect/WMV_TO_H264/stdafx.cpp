#include "stdafx.h"


//以下信息是从config.ini配置文件中读取，如果读取有误，将采用该默认值。
int g_rtmpPort=1935;//rtmp协议发送到流服务器服务器的端口
int g_maxLiveNum=16;//该设备支持的最大路数
int	TestCycle=120;//质检上报周期120秒
int	IsSavePicture=0;//是否保存图片
int	IsSaveLog=0;//是否保存日志
int LogSaveType=1;//0按日期保存不删除；1按1M容量保存超出删除。
CString PicturePath;//图片保存位置
CString LogPath;//日志保存位置
CString Dstip;
int	Dstport;
CString WorkBeginTime;
CString WorkEndTime;
CString VedioDetectPath;
CString ServiceStatePath;
char URL[16][256];
int g_regDetWay;
int LocalPort;
CString LocalServerPath;//local service path of netTsk

int YiChPercent;
int MaxLocTsk;
int MaxNetTsk;

int PRINT=1;
 int noSend=0;//no sendTsk
 int noServerstate=0;//no serverStateTsk
 int parseTest=0;//pareseTsk debug

char SrcList[512][255];

CONVERT_DEV_TYPE g_csType;