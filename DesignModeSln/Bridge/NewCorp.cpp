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
	//ÿ����˾����һ����������
	this->m_pProduct->BeProducted();

	//Ȼ������
	this->m_pProduct->BeSelled();
}