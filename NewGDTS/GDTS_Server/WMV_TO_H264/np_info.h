#ifndef DEF_NP_H_
#define DEF_NP_H_

#include "./np_sdk/include/DataType.h"

#pragma comment(lib,"./np_sdk/lib/np_netsdk.lib")
#pragma comment(lib,"./np_sdk/lib/np_playsdk.lib")

typedef struct tagDEV_NP_INFO{
	char		m_dstIp[32];		//目标服务器IP
	char		m_dstPub[32];		//发布名
	LONG		m_dstStream;	//流下标
	NP_NET_HD m_loginid;
	NP_NET_HD realLiveHd;
	NP_PLAY_HD realPlayerHd;

}DEV_NP_INFO;

typedef struct tagDEV_NP{//东方网力使用参数结构
	char		m_proxyIp[64];		//转发服务器IP
	LONG		m_proxyPort;		//转发服务器端口
	char		m_userName[32];		//用户名
	char		m_passWord[32];		//密码
	
	char		m_devName[64];

	DEV_NP_INFO m_devInfo;


}DEV_NP_PARAMS;


#endif //DEF_HK_H_