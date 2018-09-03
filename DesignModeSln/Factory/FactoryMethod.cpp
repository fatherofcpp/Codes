/*
��������ģʽ�������Ƕ���һ��������Ʒ����Ĺ����ӿڣ���ʵ�ʴ��������Ƴٵ����൱��.
���Ĺ����಻�ٸ����Ʒ�Ĵ����������������Ϊһ�����󹤳���ɫ����������幤���������ʵ�ֵĽӿڣ�
������һ�����󻯵ĺô���ʹ�ù�������ģʽ����ʹϵͳ�ڲ��޸ľ��幤����ɫ������������µĲ�Ʒ��
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
	cout << "----------��һ�����������ģ������˹���������������" << endl;
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
	cout << "----------�ڶ������������ģ������˹���������������" << endl;
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
	//��������
	cout << "----------����������" << endl;
	DoFactoryMethod1();
	DoFactoryMethod2();


	return 0;
}
