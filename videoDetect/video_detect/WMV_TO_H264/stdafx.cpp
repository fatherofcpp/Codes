#include "stdafx.h"


//������Ϣ�Ǵ�config.ini�����ļ��ж�ȡ�������ȡ���󣬽����ø�Ĭ��ֵ��
int g_rtmpPort=1935;//rtmpЭ�鷢�͵����������������Ķ˿�
int g_maxLiveNum=16;//���豸֧�ֵ����·��
int	TestCycle=120;//�ʼ��ϱ�����120��
int	IsSavePicture=0;//�Ƿ񱣴�ͼƬ
int	IsSaveLog=0;//�Ƿ񱣴���־
int LogSaveType=1;//0�����ڱ��治ɾ����1��1M�������泬��ɾ����
CString PicturePath;//ͼƬ����λ��
CString LogPath;//��־����λ��
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