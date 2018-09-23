#ifndef DEF_NTQY_H_
#define DEF_NTQY_H_


typedef struct tagDEV_NTQY_INFO{
 char  m_deviceId[128];		//设备ID
 LONG	  m_deviceChn;	//设备通道
 char  m_dstIp[32];		//目标服务器IP
 char  m_dstPub[255];		//发布名
 LONG	  m_dstStream;	//流下标
 void    *m_handle;//开始视频的句柄
}DEV_NTQY_INFO;

typedef struct tagDEV_NTQY{//南通汽运使用参数结构
 char  m_proxyIp[64];		//转发服务器IP
 LONG	  m_proxyPort;		//转发服务器端口
 char  m_userName[32];		//用户名
 char  m_passWord[32];		//密码

 DEV_NTQY_INFO m_devInfo[16];


}DEV_NTQY_PARAMS;


#endif //DEF_NTQY_H_