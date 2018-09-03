#include "StdAfx.h"
#include "Clothes.h"
#include <iostream>
using std::cout;
using std::endl;
CClothes::CClothes(void)
{
}
CClothes::~CClothes(void)
{
}
void CClothes::BeProducted()
{
	cout << "生产出的衣服是这个样子的..." << endl;
}
void CClothes::BeSelled()
{
	cout << "生产出的衣服卖出去了..." << endl;
}