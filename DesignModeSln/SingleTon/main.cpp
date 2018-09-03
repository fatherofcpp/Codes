/*
main()，大臣

CEmperor，需要单例的类

说明：很多大臣拜见的皇帝，只有一个。体现在面向对象方面，CEmperor定义一个静态指针，和一个静态函数，私有化构造函数、析构函数、构造函数复制、重载赋值语句。

注意：线程安全，采用互斥体的方式实现。
*/
#include "stdafx.h"

#include "Emperor.h"

void DoIt()
{
	CEmperor *pEmperor1 = CEmperor::GetInstance();
	pEmperor1->SetEmperorTag("95");
	pEmperor1->EmperorInfo();

	CEmperor *pEmperor2 = CEmperor::GetInstance();
	pEmperor2->EmperorInfo();

	CEmperor *pEmperor3 = CEmperor::GetInstance();
	pEmperor3->EmperorInfo();

	CEmperor *pEmperor4 = pEmperor3;
	pEmperor4->EmperorInfo();

	CEmperor *pEmperor5 = NULL;
	pEmperor5 = pEmperor4;
	pEmperor5->EmperorInfo();

	CEmperor::ReleaseInstance();
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoIt();
	return 0;
}

