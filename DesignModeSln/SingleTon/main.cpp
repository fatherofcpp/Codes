/*
main()����

CEmperor����Ҫ��������

˵�����ܶ�󳼰ݼ��Ļʵۣ�ֻ��һ������������������棬CEmperor����һ����ָ̬�룬��һ����̬������˽�л����캯�����������������캯�����ơ����ظ�ֵ��䡣

ע�⣺�̰߳�ȫ�����û�����ķ�ʽʵ�֡�
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

