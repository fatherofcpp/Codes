// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <Mmsystem.h>

#define DEF_CS_SHSL_FLAG "shsl"				//URL���Ϻ��������ķ�������ʶ
#define DEF_CS_MICROVIDEO_FLAG "microvideo"	//URL���Ͼ��ж����ķ�������ʶ
#define DEF_CS_DFHS_FLAG "dfhs"				//URL����ͨ�ط����µķ�������ʶ
#define DEF_CS_NTQY_FLAG "ntqy"				//URL����ͨ���˵ķ�������ʶ

//������豸����
enum CONVERT_DEV_TYPE{
DEF_MICROVIDEO_DEV,	//�Ͼ��ж������в�Ʒ��
DEF_SHSL_DEV,		//�Ϻ�����
DEF_DFHS_DEV,		//���ݿƴ�
DEF_NTQY_DEV,		//��ͨ����
DEF_RESERVE			//����

};

extern int g_rtmpPort;//reversed
extern int g_maxLiveNum;//reversed
extern int	TestCycle;//�ʼ��ϱ�����120��
extern int	IsSavePicture;//�Ƿ񱣴�ͼƬ
extern int	IsSaveLog;//�Ƿ񱣴���־
extern int LogSaveType;//0�����ڱ��治ɾ����1��1M�������泬��ɾ����
extern CString PicturePath;//ͼƬ����λ��
extern CString LogPath;//��־����λ��
extern	CString Dstip;//service ip to report to 
extern int	Dstport;//service port to report to
extern CString WorkBeginTime;//begin time of detect
extern CString WorkEndTime;//end time of detect
extern CString VedioDetectPath;//report path of video detect
extern CString ServiceStatePath;//report path of service detect
extern int g_regDetWay;//�����ļ�������,��1��ʼ,1-16 number of source get from config each time
extern int LocalPort;//local port to listen
extern CString LocalServerPath;//local service path of netTsk

extern int YiChPercent;//percent of YiChang
extern int MaxLocTsk;//max numbers of auto report way
extern int MaxNetTsk;//max numbers of task report way


extern CONVERT_DEV_TYPE g_csType;//device type
extern int PRINT;//if print information
extern int noSend;//no sendTsk
extern int noServerstate;//no serverStateTsk
extern int parseTest;//pareseTsk debug
extern char SrcList[512][255];//SourceList

#define MAX_ONLINE 32	//max numbers of auto report add task report way
#define MAX_DETECT_NUM 32	//max numbers of auto report detect add task report detect way

#define USEUNIT 1

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


