#pragma once
#include "Thread/CmThreadPool.h"

#include <string>

class CAbstractServiceNet :
	public NSP_STP_CM::CThreadPool
{
public:
	CAbstractServiceNet(void);
	virtual ~CAbstractServiceNet(void);

public:
	virtual void AddConnectCount();
	virtual void DelConnectCount();
	virtual int GetConnectCount();

public:
	static unsigned int WINAPI thread_connect_accept(void* pParam);
public:
	//�����˿�
	int m_tcpPort;
	//�����ϵ�ͨ�Ÿ���
	int m_connectCount;
	CCriticalSection m_connectCountLock;

	NSP_STP_CM::THREAD_RUN m_pFuncThreadCommunicate;//���߳�
	bool m_isMainThreadRun;//���߳�����
};

