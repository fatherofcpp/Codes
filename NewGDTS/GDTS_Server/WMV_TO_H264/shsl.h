
#ifndef _DEF_SHSL_H_
#define _DEF_SHSL_H_
//上海三利使用 参数及相应定义
#include "stdafx.h"

#define DEF_SHSL_TYPE_DVR	121
#define DEF_SHSL_TYPE_DVS	117

#define DEF_SHSL_PROXYPORT_DEFAULT	17891	//转发服务器默认端口

typedef struct tagDEV_SHSL{//上海三利使用参数结构
 LONG     m_deviceType;		//设备类型，dvr 121，dvs 117
 CString  m_deviceId;		//设备ID，形如 dns：7856
 LONG     m_devicePort;		//设备端口，也称视频通道
 CString  m_proxyIp;		//转发服务器IP
 LONG	  m_proxyPort;		//转发服务器端口
 CString  m_deviceName;		//设备名称，也称车牌
 CString  m_userName;		//用户名
 CString  m_passWord;		//密码

 //add for NTGJ
 CString  m_dstIp;
 CString  m_dstPub;
 int  m_dstStream;

}DEV_SHSL_PARAMS;


#endif





