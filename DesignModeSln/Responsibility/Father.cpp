#include "StdAfx.h"
#include "Father.h"
#include <iostream>
using std::cout;
using std::endl;
CFather::CFather(void) : CHandler(1)
{
}
CFather::~CFather(void)
{
}
void CFather::Response( IWomen *pwomen )
{
	cout << "Ů��������ʾ��" << endl;
	cout << pwomen->GetRequest().c_str() << endl;
	cout << "���׵Ĵ��ǣ�ͬ��" << endl;
}