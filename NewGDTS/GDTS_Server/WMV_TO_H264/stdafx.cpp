#include "stdafx.h"


//������Ϣ�Ǵ�config.ini�����ļ��ж�ȡ�������ȡ���󣬽����ø�Ĭ��ֵ��
int g_rtmpPort=1935;//rtmpЭ�鷢�͵����������������Ķ˿�
int g_maxLiveNum=16;//���豸֧�ֵ����·��
int g_ListenPort=9999;//��WEBĬ�ϼ����Ķ˿�
int g_PtzPort=9876;//PTZ�����Ķ˿�
int g_BitRate=500;//
int g_VideoWidth=352;//
int g_VideoHeight=288;//
int g_bUseTc=0;//ȫ�ֵ������Ƿ���Ҫת�룬���H264�Ƿ����Ҫ��,���ֻ��Ͽ���ȫ��Ҫת�����.
int g_PubType=0;//0:RTMP 1:mss
int g_bStop=0;
int g_FramePort=10000;//���������˵�֡
char g_FrameIp[64]="127.0.0.0";

CH264Stream *m_h264[16];
int SourceCount=0;

CString g_CurrentDir;

CONVERT_DEV_TYPE g_csType;