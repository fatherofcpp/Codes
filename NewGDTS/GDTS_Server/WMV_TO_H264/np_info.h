#ifndef DEF_NP_H_
#define DEF_NP_H_

#include "./np_sdk/include/DataType.h"

#pragma comment(lib,"./np_sdk/lib/np_netsdk.lib")
#pragma comment(lib,"./np_sdk/lib/np_playsdk.lib")

typedef struct tagDEV_NP_INFO{
	char		m_dstIp[32];		//Ŀ�������IP
	char		m_dstPub[32];		//������
	LONG		m_dstStream;	//���±�
	NP_NET_HD m_loginid;
	NP_NET_HD realLiveHd;
	NP_PLAY_HD realPlayerHd;

}DEV_NP_INFO;

typedef struct tagDEV_NP{//��������ʹ�ò����ṹ
	char		m_proxyIp[64];		//ת��������IP
	LONG		m_proxyPort;		//ת���������˿�
	char		m_userName[32];		//�û���
	char		m_passWord[32];		//����
	
	char		m_devName[64];

	DEV_NP_INFO m_devInfo;


}DEV_NP_PARAMS;


#endif //DEF_HK_H_