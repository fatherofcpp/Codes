
/*
桥梁模式，将抽象部分与它的实现部分分离，使它们都可以独立地变化。实现分离的办法就是增加一个类，
main()，客户
IProduct，产品接口
CHouse，房子
CIPod，ipod
CClothes，服装
CNewCorp，桥梁类，MakeMoney()是桥梁方法
CNewHouseCorp，只能生产房子，所以构造函数是CHouse*
CShanZhaiCorp，什么赚钱就生产什么，所以构造函数是IProduct*
说明：客户直接使用CNewHouseCorp和CShanZhaiCorp类，在main()函数里构造产品，
然后传到这两个类里。这两个类的MakeMoney()函数，先调用基类的MakeMoney()，然后分别执行各自的逻辑。
注意：CNewCorp起到了桥梁的作用。可以分别增加产品和公司。
*/

#include "stdafx.h"
#include "NewHouseCorp.h"
#include "Clothes.h"
#include "IPod.h"
#include "ShanZhaiCorp.h"
#include <iostream>
using std::cout;
using std::endl;

void DoNewRun1()
{
	cout << "----------房地产公司是这样运行的----------" << endl;
	CHouse house;
	CNewHouseCorp newHouseCorp(&house);
	newHouseCorp.MakeMoney();
	cout << endl;

	cout << "----------山寨公司是这样运行的----------" << endl;
	CClothes clothes;
	CShanZhaiCorp shanZhaiCorp(&clothes);
	shanZhaiCorp.MakeMoney();
	cout << endl;
}

void DoNewRun2()
{
	cout << "----------房地产公司是这样运行的----------" << endl;
	CHouse house;
	CNewHouseCorp newHouseCorp(&house);
	newHouseCorp.MakeMoney();
	cout << endl;

	cout << "----------山寨公司是这样运行的----------" << endl;
	CIPod ipod;
	CShanZhaiCorp shanZhaiCorp(&ipod);
	shanZhaiCorp.MakeMoney();
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//只有两家公司，一家是房地产公司，另一家公司是衣服赚钱就生产衣服
	DoNewRun1();

	//只有两家公司，一家是房地产公司，另一家公司是ipod赚钱就生产ipod
	DoNewRun2();

	return 0;
}
