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
	//监听端口
	int m_tcpPort;
	//连接上的通信个数
	int m_connectCount;
	CCriticalSection m_connectCountLock;

	NSP_STP_CM::THREAD_RUN m_pFuncThreadCommunicate;//子线程
	bool m_isMainThreadRun;//主线程运行
};

