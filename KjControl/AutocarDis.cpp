#include "stdafx.h"
#include "AutocarDis.h"

AutocarDis::AutocarDis():m_fDis(0.0)
{
	::InitializeCriticalSection(&m_cs);
}

AutocarDis::~AutocarDis()
{
	::DeleteCriticalSection(&m_cs);	
	TRACE("class AutocarDis destruct ... \r\n");
}

float AutocarDis::GetDis()
{
	float retv = 0.0;
	EnterCriticalSection(&m_cs);
	retv = m_fDis;
	LeaveCriticalSection(&m_cs);
	return retv;
}

void  AutocarDis::SetDis(float v)
{
	EnterCriticalSection(&m_cs);
	m_fDis = v;
	LeaveCriticalSection(&m_cs);
}