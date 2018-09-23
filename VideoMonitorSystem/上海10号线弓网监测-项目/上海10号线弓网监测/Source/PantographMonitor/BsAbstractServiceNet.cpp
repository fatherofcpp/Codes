#include "StdAfx.h"
#include "BsAbstractServiceNet.h"

#include "CmFunction.h"
#include "Log/CmLog.h"
#include "Socket/CmTransTcp.h"
#include "BsDefStruct.h"
#include "BsFunction.h"
#include "BsMonitor.h"
#include "DataBase/CmDBMySql.h"
#include "BsGlobal.h"

static const int s_max_connect_count = 8;

CAbstractServiceNet::CAbstractServiceNet(void)
{
	m_tcpPort = 0;
	m_connectCount = 0;

	m_isMainThreadRun = false;
}


CAbstractServiceNet::~CAbstractServiceNet(void)
{
}

void CAbstractServiceNet::AddConnectCount()
{
	CCriticalSection* p_lock = &m_connectCountLock;
	p_lock->Lock();
	m_connectCount ++;
	p_lock->Unlock();
}

void CAbstractServiceNet::DelConnectCount()
{
	CCriticalSection* p_lock = &m_connectCountLock;
	p_lock->Lock();
	m_connectCount --;
	p_lock->Unlock();
}

int CAbstractServiceNet::GetConnectCount()
{
	return m_connectCount;
}

unsigned int WINAPI CAbstractServiceNet::thread_connect_accept( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAbstractServiceNet* p_this = (CAbstractServiceNet*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	int ret = 0;
	const int sleep_m = 20;

	NSP_STP_CM::CTcp* p_CTcp = new NSP_STP_CM::CTcp;
	p_CTcp->SetLocalPort(p_this->m_tcpPort);
	ret = p_CTcp->BindPort();
	if (ret != 0)
	{
		gLog.Add("%s accept_connect fail\n", __FUNCTION__);
		return 0;
	}
	sockaddr_in addr_client;
	int sockaddr_size = sizeof(sockaddr);

	p_this->m_isMainThreadRun = true;

	while(1)
	{
		Sleep(sleep_m);
		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		int sock_client = accept(p_CTcp->GetSockFd(), (SOCKADDR*)&addr_client, &sockaddr_size); 
		if (sock_client < 0)
		{
			//gLog.Add("[%s] accept fail\n",__FUNCTION__);
		}
		else
		{
			int connect_count = p_this->GetConnectCount();
			if (connect_count < s_max_connect_count)
			{
				NSP_STP_CM::CThread* p_CThread = new NSP_STP_CM::CThread;
				{
					NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
					memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
					p_st_thread_param->pParam = p_this;
					p_st_thread_param->threadInd = thread_ind;
					p_st_thread_param->pVoid = sock_client;
					strcpy(p_st_thread_param->threadName, __FUNCTION__);
					thread_run(p_CThread, p_this->m_pFuncThreadCommunicate, p_st_thread_param, CpuMaskRemain());
				}
				delete p_CThread;
			}
			gLog.Add("[%s] accept sock_client=%d\n",__FUNCTION__,sock_client);
		}
	}

	p_CTcp->CloseBind();
	delete p_CTcp;

	p_this->m_isMainThreadRun = false;

	return 0;
}
