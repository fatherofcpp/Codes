#pragma once
#include "BsServicePis.h"

class CServicePtu :
	public CServicePis
{
public:
	CServicePtu(void);
	~CServicePtu(void);
public:
	//��ʼ��
	virtual int TaskStart();

	//��ջ���
	virtual void ReleaseRes();

	//���ñ��ض˿�
	void SetUdpPtuPort(int udpPtuPort){m_udpPort = udpPtuPort;}

private:
	int m_udpPort;
};

