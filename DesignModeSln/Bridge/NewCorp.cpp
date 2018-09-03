#include "StdAfx.h"
#include "NewCorp.h"
CNewCorp::CNewCorp( IProduct *pproduct )
{
	this->m_pProduct = pproduct;
}
CNewCorp::~CNewCorp(void)
{
}
void CNewCorp::MakeMoney()
{
	//每个公司都是一样，先生产
	this->m_pProduct->BeProducted();

	//然后销售
	this->m_pProduct->BeSelled();
}