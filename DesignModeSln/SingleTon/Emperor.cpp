#include "StdAfx.h"
#include "Emperor.h"

CEmperor* CEmperor::m_pEmperor = NULL;
HANDLE CEmperor::m_pMutex = CreateMutex(NULL, FALSE, NULL);

CEmperor::CEmperor(void)
{
	cout << __FUNCTION__ << endl;
}
CEmperor::~CEmperor(void)
{
	cout << __FUNCTION__ << endl;
}
void CEmperor::EmperorInfo(void)
{
	cout << __FUNCTION__ << endl;
}
CEmperor* CEmperor::GetInstance()
{
	if (NULL == m_pEmperor)
	{
		WaitForSingleObject(m_pMutex, INFINITE);
		if (NULL == m_pEmperor)
			m_pEmperor = new CEmperor();
		ReleaseMutex(m_pMutex);
	}
	return m_pEmperor;
}
void CEmperor::ReleaseInstance()
{
	if (NULL != m_pEmperor)
	{
		WaitForSingleObject(m_pMutex, INFINITE);
		if (NULL != m_pEmperor)
		{
			delete m_pEmperor;
			m_pEmperor = NULL;
		}
		ReleaseMutex(m_pMutex);
	}
}
void CEmperor::SetEmperorTag( string tag )
{
	m_EmperorTag = tag;
}