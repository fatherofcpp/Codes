#include "StdAfx.h"
#include "Husband.h"
#include <iostream>
using std::cout;
using std::endl;
CHusband::CHusband(void) : CHandler(2)
{
}
CHusband::~CHusband(void)
{
}
void CHusband::Response( IWomen *pwomen )
{
	cout << "�������ɷ���ʾ��" << endl;
	cout << pwomen->GetRequest().c_str() << endl;
	cout << "�ɷ�Ĵ��ǣ�ͬ��" << endl;
}