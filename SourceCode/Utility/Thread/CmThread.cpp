#include "stdafx.h"
#include "CmThread.h"

NSP_STP_CM::CThread::CThread(void)
	:m_Thread(NULL),m_ID(0xFFFFFFFF),m_bStart(false)
{
	strcpy(m_threadName, "");
}

NSP_STP_CM::CThread::~CThread(void)
{
}

bool NSP_STP_CM::CThread::ThreadStart(THREAD_RUN thread_run, void * param, const int mask)
{
	if (!m_bStart)
	{
		m_Thread = (HANDLE)::_beginthreadex(
			NULL,				
			0,					
			thread_run,				
			param,				
			0,					
			&m_ID );
		m_bStart = (m_Thread != NULL && m_Thread != INVALID_HANDLE_VALUE);

		SetThreadAffinityMask(mask);

		SetPriority(THREAD_PRIORITY_NORMAL);
	}
	return m_bStart;
}
void NSP_STP_CM::CThread::ThreadStop()
{
	if (m_bStart)
	{
		m_bStart = false;
		WaitForSingleObject(m_Thread,INFINITE);
		CloseHandle(m_Thread);
		m_Thread = NULL;
	}
}

// 线程设置硬亲缘性
void NSP_STP_CM::CThread::SetThreadAffinityMask(int mask)
{
	if (m_Thread == NULL)
	{
		return;
	}
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	DWORD processors_num = system_info.dwNumberOfProcessors;
	int max_mask = (int)(0x01 << processors_num) - 1;
	if (mask > max_mask || mask < 0x01)
	{
		return;
	}
	::SetThreadAffinityMask(m_Thread,mask);
}

void NSP_STP_CM::CThread::SetPriority( int aPriority )
{
	if (m_Thread == NULL)
	{
		return;
	}
	::SetThreadPriority(m_Thread,aPriority);
}

int NSP_STP_CM::CThread::GetPriority() const
{
	if (m_Thread == NULL)
	{
		return THREAD_PRIORITY_NORMAL;
	}
	return ::GetThreadPriority(m_Thread);
}

