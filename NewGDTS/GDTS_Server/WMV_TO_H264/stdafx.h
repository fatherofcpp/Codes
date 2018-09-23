// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <Mmsystem.h>

#define DEF_CS_NTGJ_FLAG "402848a13f218b39013f46df0bc00003"				//URL中上海三利流的发布名标识
#define DEF_CS_MICROVIDEO_FLAG "402848a13fc2d7ba013fc2d7bad10000"	//URL中南京感动流的发布名标识
#define DEF_CS_DFHS_FLAG "402848a13f218b39013f46e04c6e0006"				//URL中南通地方海事的发布名标识
#define DEF_CS_NTQY_FLAG "402848a13f218b39013f219d7bb60001"				//URL中南通汽运的发布名标识
#define DEF_CS_NTHD_FLAG "402848a13f218b39013f46dfe3340005"				//URL中南通航道的发布名标识
#define DEF_CS_RTSP_FLAG "rtsp"	//URL中RTSP流的发布名标识
#define DEF_CS_NP_FLAG "np"	//URL中东方网力流的发布名标识
#define DEF_CS_VNMP_FLAG "vnmp"	//URL中长天流的发布名标识
//定义的设备类型
enum CONVERT_DEV_TYPE{
DEF_MICROVIDEO_DEV,	//南京感动（自有产品）
DEF_SHSL_DEV,		//上海三利
DEF_DFHS_DEV,		//苏州科达
DEF_NTQY_DEV,		//南通汽运
DEF_NTHD_DEV,		//南通航道
DEF_H3C_DEV,		//华三设备
DEF_RTSP_DEV,		//RTSP协议拉流
DEF_NETPOSA_DEV,	//东方网力
DEF_VNMP_DEV,		//长天
DEF_H3CVM_DEV,		//华三VM8000
DEF_DH_DEV,			//大华设备
DEF_HK_DEV,
DEF_SANTACHI_DEV,	//金三立
DEF_VMS_DEV,		//华鼎设备
DEF_H3CIPC_DEV,		//华三IPC
DEF_H3CIMOS_DEV,		//华三IMOS

DEF_RESERVE			//保留

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


