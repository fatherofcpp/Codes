
#ifndef _DEF_SHSL_H_
#define _DEF_SHSL_H_
//�Ϻ�����ʹ�� ��������Ӧ����
#include "stdafx.h"

#define DEF_SHSL_TYPE_DVR	121
#define DEF_SHSL_TYPE_DVS	117

#define DEF_SHSL_PROXYPORT_DEFAULT	17891	//ת��������Ĭ�϶˿�

typedef struct tagDEV_SHSL{//�Ϻ�����ʹ�ò����ṹ
 LONG     m_deviceType;		//�豸���ͣ�dvr 121��dvs 117
 CString  m_deviceId;		//�豸ID������ dns��7856
 LONG     m_devicePort;		//�豸�˿ڣ�Ҳ����Ƶͨ��
 CString  m_proxyIp;		//ת��������IP
 LONG	  m_proxyPort;		//ת���������˿�
 CString  m_deviceName;		//�豸���ƣ�Ҳ�Ƴ���
 CString  m_userName;		//�û���
 CString  m_passWord;		//����

 //add for NTGJ
 CString  m_dstIp;
 CString  m_dstPub;
 int  m_dstStream;

}DEV_SHSL_PARAMS;


#endif





