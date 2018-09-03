/*
工厂方法模式的意义是定义一个创建产品对象的工厂接口，将实际创建工作推迟到子类当中.
核心工厂类不再负责产品的创建，这样核心类成为一个抽象工厂角色，仅负责具体工厂子类必须实现的接口，
这样进一步抽象化的好处是使得工厂方法模式可以使系统在不修改具体工厂角色的情况下引进新的产品。
*/

#include "stdafx.h"
#include "IHuman.h"
#include "YellowHuman.h"
#include "WhiteHuman.h"
#include "IHumanFactory.h"
#include "YellowHumanFactory.h"
#include "WhiteHumanFactory.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;
void DoFactoryMethod1()
{
	cout << "----------第一批人是这样的：黄种人工厂来生产黄种人" << endl;
	IHumanFactory *pHumanFactory = new CYellowHumanFactory();
	IHuman *pHuman = pHumanFactory->CreateHuman();
	pHuman->Cry();
	pHuman->Laugh();
	pHuman->Talk();
	delete pHuman;
	delete pHumanFactory;
}
void DoFactoryMethod2()
{
	cout << "----------第二批人是这样的：白种人工厂来生产白种人" << endl;
	IHumanFactory *pHumanFactory = new CWhiteHumanFactory();
	IHuman *pHuman = pHumanFactory->CreateHuman();
	pHuman->Cry();
	pHuman->Laugh();
	pHuman->Talk();
	delete pHuman;
	delete pHumanFactory;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//工厂方法
	cout << "----------工厂方法：" << endl;
	DoFactoryMethod1();
	DoFactoryMethod2();


	return 0;
}
