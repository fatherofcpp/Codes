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

#define DEF_CS_SHSL_FLAG "shsl"				//URL中上海三利流的发布名标识
#define DEF_CS_MICROVIDEO_FLAG "microvideo"	//URL中南京感动流的发布名标识
#define DEF_CS_DFHS_FLAG "dfhs"				//URL中南通地方海事的发布名标识
#define DEF_CS_NTQY_FLAG "ntqy"				//URL中南通汽运的发布名标识

//定义的设备类型
enum CONVERT_DEV_TYPE{
DEF_MICROVIDEO_DEV,	//南京感动（自有产品）
DEF_SHSL_DEV,		//上海三利
DEF_DFHS_DEV,		//苏州科达
DEF_NTQY_DEV,		//南通汽运
DEF_RESERVE			//保留

};

extern int g_rtmpPort;//reversed
extern int g_maxLiveNum;//reversed
extern int	TestCycle;//质检上报周期120秒
extern int	IsSavePicture;//是否保存图片
extern int	IsSaveLog;//是否保存日志
extern int LogSaveType;//0按日期保存不删除；1按1M容量保存超出删除。
extern CString PicturePath;//图片保存位置
extern CString LogPath;//日志保存位置
extern	CString Dstip;//service ip to report to 
extern int	Dstport;//service port to report to
extern CString WorkBeginTime;//begin time of detect
extern CString WorkEndTime;//end time of detect
extern CString VedioDetectPath;//report path of video detect
extern CString ServiceStatePath;//report path of service detect
extern int g_regDetWay;//配置文件中流数,从1开始,1-16 number of source get from config each time
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


