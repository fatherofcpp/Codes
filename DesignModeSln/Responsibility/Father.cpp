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
	cout << "女儿向父亲请示：" << endl;
	cout << pwomen->GetRequest().c_str() << endl;
	cout << "父亲的答复是：同意" << endl;
}