#include "StdAfx.h"
#include "Son.h"
#include <iostream>
using std::cout;
using std::endl;
CSon::CSon(void) : CHandler(3)
{
}
CSon::~CSon(void)
{
}
void CSon::Response( IWomen *pwomen )
{
	cout << "ĸ���������ʾ��" << endl;
	cout << pwomen->GetRequest().c_str() << endl;
	cout << "���ӵĴ��ǣ�ͬ��" << endl;
}