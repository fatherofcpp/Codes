/*
main()，赵云

CContext，锦囊

IStrategy，策略接口

CBackDoor，策略之一

CGivenGreenLight，策略之二

CBlockEnemy，策略之三

说明：一个策略放到一个锦囊里。当用的时候，找到这个锦囊，从锦囊里拿出策略来使用。

注意：锦囊只是简单的装载和调用策略，锦囊里没有逻辑。策略会有更大的自主权，运行更多的逻辑。
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
	cout << "----------刚刚到吴国的时候拆第一个----------" << endl;
	pContext = new CContext(new CBackDoor());
	pContext->Operate();
	delete pContext;

	cout << "\14\n\n\n\n\17" << endl;
	cout << "----------刘备乐不思蜀了，拆第二个了----------" << endl;
	pContext = new CContext(new CGivenGreenLight());
	pContext->Operate();
	delete pContext;

	cout << "\14\n\n\n\n\17" << endl;
	cout << "----------孙权的小兵追了，咋办？拆第三个----------" << endl;
	pContext = new CContext(new CBlockEnemy());
	pContext->Operate();
	delete pContext;

	return 0;
}

