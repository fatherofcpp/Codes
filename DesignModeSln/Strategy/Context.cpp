#include "StdAfx.h"
#include "Context.h"
CContext::CContext(IStrategy *pStrategy)
{
	this->m_pStrategy = pStrategy;
}
CContext::~CContext(void)
{
	delete this->m_pStrategy;
}
void CContext::Operate(void)
{
	this->m_pStrategy->Operate();
}
