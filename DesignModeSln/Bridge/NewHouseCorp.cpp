#include "StdAfx.h"
#include "NewHouseCorp.h"
#include <iostream>
using std::cout;
using std::endl;
CNewHouseCorp::CNewHouseCorp(CHouse *pHouse) : CNewCorp(pHouse)
{
}
CNewHouseCorp::~CNewHouseCorp(void)
{
}
void CNewHouseCorp::MakeMoney()
{
	this->CNewCorp::MakeMoney();
	cout << "房地产公司赚大钱了..." << endl;
}