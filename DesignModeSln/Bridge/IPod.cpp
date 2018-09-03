#include "StdAfx.h"
#include "IPod.h"
#include <iostream>
using std::cout;
using std::endl;
CIPod::CIPod(void)
{
}
CIPod::~CIPod(void)
{
}
void CIPod::BeProducted()
{
	cout << "生产出的ipod是这个样子的..." << endl;
}
void CIPod::BeSelled()
{
	cout << "生产出的ipod卖出去了..." << endl;
}