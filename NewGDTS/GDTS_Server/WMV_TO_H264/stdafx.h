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

#define DEF_CS_NTGJ_FLAG "402848a13f218b39013f46df0bc00003"				//URL���Ϻ��������ķ�������ʶ
#define DEF_CS_MICROVIDEO_FLAG "402848a13fc2d7ba013fc2d7bad10000"	//URL���Ͼ��ж����ķ�������ʶ
#define DEF_CS_DFHS_FLAG "402848a13f218b39013f46e04c6e0006"				//URL����ͨ�ط����µķ�������ʶ
#define DEF_CS_NTQY_FLAG "402848a13f218b39013f219d7bb60001"				//URL����ͨ���˵ķ�������ʶ
#define DEF_CS_NTHD_FLAG "402848a13f218b39013f46dfe3340005"				//URL����ͨ�����ķ�������ʶ
#define DEF_CS_RTSP_FLAG "rtsp"	//URL��RTSP���ķ�������ʶ
#define DEF_CS_NP_FLAG "np"	//URL�ж����������ķ�������ʶ
#define DEF_CS_VNMP_FLAG "vnmp"	//URL�г������ķ�������ʶ
//������豸����
enum CONVERT_DEV_TYPE{
DEF_MICROVIDEO_DEV,	//�Ͼ��ж������в�Ʒ��
DEF_SHSL_DEV,		//�Ϻ�����
DEF_DFHS_DEV,		//���ݿƴ�
DEF_NTQY_DEV,		//��ͨ����
DEF_NTHD_DEV,		//��ͨ����
DEF_H3C_DEV,		//�����豸
DEF_RTSP_DEV,		//RTSPЭ������
DEF_NETPOSA_DEV,	//��������
DEF_VNMP_DEV,		//����
DEF_H3CVM_DEV,		//����VM8000
DEF_DH_DEV,			//���豸
DEF_HK_DEV,
DEF_SANTACHI_DEV,	//������
DEF_VMS_DEV,		//�����豸
DEF_H3CIPC_DEV,		//����IPC
DEF_H3CIMOS_DEV,		//����IMOS

DEF_RESERVE			//����

};

#define MICROVIDEO_INDEX	0
#define NETPOSA_INDEX		1
#define VNMP_INDEX			2
#define H3CVM_INDEX			3
#define DHDEV_INDEX			4
#define HKDEV_INDEX			5
#define SANTACHIDEV_INDEX	6
#define VMSDEV_INDEX		7
#define H3CIPCDEV_INDEX		8
#define RTSP_INDEX			9
#define H3CIMOS_INDEX		10
#define SHSL_INDEX			11
#define NTQY_INDEX			12
#define NTHD_INDEX			13
#define DFHS_INDEX			14




#define DEF_MAX_COMPANY		15

extern int g_rtmpPort;
extern int g_maxLiveNum;
extern int g_ListenPort;
extern int g_PtzPort;
extern int g_BitRate;//
extern int g_VideoWidth;//
extern int g_VideoHeight;//
extern int g_bUseTc;
extern int g_PubType;
extern int g_bStop;
extern int g_FramePort;
extern char g_FrameIp[64];

extern CONVERT_DEV_TYPE g_csType;

#include "H264Stream.h"

extern CH264Stream *m_h264[16];
extern int SourceCount;

extern CString g_CurrentDir;

#define USENEWSTR 1
#define ADDLastSize 1
#define ADDFRAMELEN 1


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


