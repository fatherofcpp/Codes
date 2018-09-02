#include "StdAfx.h"
#include "CmThreadPool.h"
#include "Log/CmLog.h"


const static unsigned int g_max_thread_count = 8*2;

NSP_STP_CM::CThreadPool::CThreadPool(void)
{
	m_threadCount = 0;
	m_pCThreadArr = NULL;
	m_isFirstNew = true;
	m_threadPause = false;
}


NSP_STP_CM::CThreadPool::~CThreadPool(void)
{
	if (m_pCThreadArr != NULL)
	{
		delete[] m_pCThreadArr;
		m_pCThreadArr = NULL;
	}
}

void NSP_STP_CM::CThreadPool::SetThreadCount( int thread_count )
{
	if (thread_count <= 0 || thread_count > g_max_thread_count)
	{
		return;
	}
	if (m_isFirstNew)
	{
		m_isFirstNew = false;
		m_threadCount = thread_count;
		m_pCThreadArr = new CThread[m_threadCount];
	}
}

void NSP_STP_CM::CThreadPool::ThreadsStop()
{
	for (int i = 0; i < m_threadCount; i++)
	{
		static int s_thread_num = 0;
		s_thread_num ++;
		gLog.Add("%s s_thread_num=%d,m_Thread=%x,threadName=%s\n", __FUNCTION__, s_thread_num, m_pCThreadArr[i].GetThread(), m_pCThreadArr[i].GetThreadName());

		m_pCThreadArr[i].ThreadStop();
	
		gLog.Add("%s s_thread_num=%d,m_Thread=%x,threadName=%s\n", __FUNCTION__, s_thread_num, m_pCThreadArr[i].GetThread(), m_pCThreadArr[i].GetThreadName());
	}
}

void NSP_STP_CM::CThreadPool::SetThreadPause( bool is_pause )
{
	m_threadPause = is_pause;
}

void NSP_STP_CM::thread_run( CThread* p_CThread, THREAD_RUN thread_run, void * param, const int mask)
{
	if (!p_CThread || !thread_run)
	{
		return;
	}
	st_thread_param* p_st_thread_param = (st_thread_param*)param;
	p_CThread->SetThreadName(p_st_thread_param->threadName);
	p_CThread->ThreadStart(thread_run, param, mask);

	//static int s_thread_num = 0;
	//s_thread_num ++;
	//gLog.Add("%s s_thread_num=%d,m_Thread=%x,threadName=%s\n", __FUNCTION__, s_thread_num, p_CThread->GetThread(), p_CThread->GetThreadName());
}

