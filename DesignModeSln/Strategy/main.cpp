/*
main()������

CContext������

IStrategy�����Խӿ�

CBackDoor������֮һ

CGivenGreenLight������֮��

CBlockEnemy������֮��

˵����һ�����Էŵ�һ����������õ�ʱ���ҵ�������ң��ӽ������ó�������ʹ�á�

ע�⣺����ֻ�Ǽ򵥵�װ�غ͵��ò��ԣ�������û���߼������Ի��и��������Ȩ�����и�����߼���
*/

#include "stdafx.h"
#include "Context.h"
#include "BackDoor.h"
#include "GivenGreenLight.h"
#include "BlockEnemy.h"
#include <iostream>
using std::cout;
using std::endl;

int _tmain(int argc, _TCHAR* argv[])
{
	CContext *pContext;

	cout << "\14\n\n\n\n\17" << endl;
	cout << "----------�ոյ������ʱ����һ��----------" << endl;
	pContext = new CContext(new CBackDoor());
	pContext->Operate();
	delete pContext;

	cout << "\14\n\n\n\n\17" << endl;
	cout << "----------�����ֲ�˼���ˣ���ڶ�����----------" << endl;
	pContext = new CContext(new CGivenGreenLight());
	pContext->Operate();
	delete pContext;

	cout << "\14\n\n\n\n\17" << endl;
	cout << "----------��Ȩ��С��׷�ˣ�զ�죿�������----------" << endl;
	pContext = new CContext(new CBlockEnemy());
	pContext->Operate();
	delete pContext;

	return 0;
}

