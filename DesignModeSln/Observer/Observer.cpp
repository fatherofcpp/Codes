/*
概念：定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，
所有依赖于它的对象都得到通知并被自动更新。
main()，
IObservable，被观察者接口
CHanFeiZiObservable，被观察者韩非子
IObserver，观察者接口
CLiSiObserver，观察者李斯
CZhouSiObserver观察者周斯
说明：将观察者聚集到被观察者韩非子身边，韩非子的每一个举动都会通知给观察者，如李斯或周斯。
注意：最多允许一个对象既是观察者也是被观察者。就像数据库中的触发器一样，
成为一个复杂的链就很难维护了。观察者类似于委托的处理方式。
*/

#include "stdafx.h"
#include "HanFeiziObservable.h"
#include "LiSiObserver.h"
#include "ZhouSiObserver.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;

void DoNew()
{
	//IHanFeiZi.h, HanFeiZiNew.h, ILiSi.h, LiSi.h
	// cout << "----------用新的方法试试----------" << endl;

	//CHanFeiZiNew hanfeizi;

	//hanfeizi.HaveBreakfast();

	//hanfeizi.HaveFun();
}


void DoNewNew()
{
	//IObservable.h, HanfeiziObservable.h, IObserver.h, LiSiObserver.h
	cout << "----------用更新的方法再试试----------" << endl;
	IObserver *pLiSi = new CLiSiObserver();
	IObserver *pZhouSi = new CZhouSiObserver();

	CHanFeiziObservable *pHanFeiZi = new CHanFeiziObservable();

	pHanFeiZi->AddObserver(pLiSi);
	pHanFeiZi->AddObserver(pZhouSi);
	pHanFeiZi->HaveBreakfast();

	delete pLiSi;
	pLiSi = NULL;
	delete pHanFeiZi;
	pHanFeiZi = NULL;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//比较原始的方法，用线程来观察。
	//DoIt();

	//把李斯这个类聚集到韩非子这个类上，这样的话耦合度太高了，还是用更抽象的方式。
	DoNew();

	//更抽象的方式，想要观察韩非子的人多了去了，不可能只允许李斯观察。
	DoNewNew();

	return 0;
}