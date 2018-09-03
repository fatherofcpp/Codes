#include "StdAfx.h"
#include "CommonEmployee.h"


CCommonEmployee::CCommonEmployee(void)
{
	m_job = "";
}


CCommonEmployee::~CCommonEmployee(void)
{
}

void CCommonEmployee::SetJob( string job )
{
	m_job = job;
}

void CCommonEmployee::Accept( IVisitor *pVisitor )
{
	pVisitor->Visit(*this);
}

string CCommonEmployee::GetJob()
{
	return m_job;
}
