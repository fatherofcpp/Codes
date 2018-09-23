#ifndef DEF_NTQY_H_
#define DEF_NTQY_H_


typedef struct tagDEV_NTQY_INFO{
 char  m_deviceId[128];		//�豸ID
 LONG	  m_deviceChn;	//�豸ͨ��
 char  m_dstIp[32];		//Ŀ�������IP
 char  m_dstPub[255];		//������
 LONG	  m_dstStream;	//���±�
 void    *m_handle;//��ʼ��Ƶ�ľ��
}DEV_NTQY_INFO;

typedef struct tagDEV_NTQY{//��ͨ����ʹ�ò����ṹ
 char  m_proxyIp[64];		//ת��������IP
 LONG	  m_proxyPort;		//ת���������˿�
 char  m_userName[32];		//�û���
 char  m_passWord[32];		//����

 DEV_NTQY_INFO m_devInfo[16];


}DEV_NTQY_PARAMS;


#endif //DEF_NTQY_H_