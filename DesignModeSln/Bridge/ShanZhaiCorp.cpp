#include "StdAfx.h"
#include "ShanZhaiCorp.h"
#include <iostream>
using std::cout;
using std::endl;
CShanZhaiCorp::CShanZhaiCorp(IProduct *pproduct) : CNewCorp(pproduct)
{
}
CShanZhaiCorp::~CShanZhaiCorp(void)
{
}
void CShanZhaiCorp::MakeMoney()
{
	this->CNewCorp::MakeMoney();
	cout << "ÎÒ×¬Ç®Ñ½..." << endl;
}