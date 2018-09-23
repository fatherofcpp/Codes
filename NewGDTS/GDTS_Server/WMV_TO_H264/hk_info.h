#ifndef DEF_HK_H_
#define DEF_HK_H_


typedef struct tagDEV_HK_INFO{
 char		m_dstIp[32];		//Ŀ�������IP
 char		m_dstPub[64];		//������
 LONG		m_dstStream;	//���±�
 LONG		m_userId;//UserID
 LONG		m_realPlayHandle;//RealPlayHandle
}DEV_HK_INFO;

typedef struct tagDEV_HK{//����ʹ�ò����ṹ
 char		m_proxyIp[64];		//ת��������IP
 LONG		m_proxyPort;		//ת���������˿�
 char		m_userName[32];		//�û���
 char		m_passWord[32];		//����
 LONG		m_deviceChn;	//�豸ͨ��
 LONG		m_playChn;		//����ͨ��

 DEV_HK_INFO m_devInfo;


}DEV_HK_PARAMS;


#endif //DEF_HK_H_