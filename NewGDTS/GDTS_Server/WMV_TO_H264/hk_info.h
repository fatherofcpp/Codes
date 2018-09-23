#ifndef DEF_HK_H_
#define DEF_HK_H_


typedef struct tagDEV_HK_INFO{
 char		m_dstIp[32];		//目标服务器IP
 char		m_dstPub[64];		//发布名
 LONG		m_dstStream;	//流下标
 LONG		m_userId;//UserID
 LONG		m_realPlayHandle;//RealPlayHandle
}DEV_HK_INFO;

typedef struct tagDEV_HK{//海康使用参数结构
 char		m_proxyIp[64];		//转发服务器IP
 LONG		m_proxyPort;		//转发服务器端口
 char		m_userName[32];		//用户名
 char		m_passWord[32];		//密码
 LONG		m_deviceChn;	//设备通道
 LONG		m_playChn;		//播放通道

 DEV_HK_INFO m_devInfo;


}DEV_HK_PARAMS;


#endif //DEF_HK_H_