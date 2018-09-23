#ifndef DEF_HD_H_
#define DEF_HD_H_


typedef struct tagDEV_HD_INFO{
 char		m_dstIp[32];		//Ŀ�������IP
 char		m_dstPub[64];		//������
 LONG		m_dstStream;	//���±�
 LONG		m_userId;//UserID

}DEV_HD_INFO;

typedef struct tagDEV_HD{//����ʹ�ò����ṹ
 char		m_deviceIp[64];		//�豸������IP
 LONG		m_devicePort;		//�豸�������˿�
 char		m_deviceType[32];	//�豸����
 char		m_userName[32];		//�û���
 char		m_passWord[32];		//����
 LONG		m_deviceChn;	//�豸ͨ��
 LONG		m_subChn;		//��ͨ��
 LONG		m_playChn;		//����ͨ��

 DEV_HD_INFO m_devInfo;


}DEV_HD_PARAMS;


#endif //DEF_HD_H_