/*
建造者模式，将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。
一段晦涩难懂的文字，实现创建不同表示的方法就是给创建的过程传入创建的参数。

main()，客户
CCarModel，产品模型
CBenzModel，奔驰模型
CBMWModel，宝马模型
ICarBuilder，建造者接口
CBenzBuilder，奔驰建造者
CBMWBuilder，宝马建造者
CDirector，导演
说明：CCarModel实现模板方法，Builder负责开始建造产品。建造产品时，构建的顺序由Director或main决定。
注意：建造者模式和抽象工厂非常类似。建造者更重视产品建造时的逻辑顺序，
而抽象工厂更重视生产出不同型号的产品，抽象工厂不关心顺序。
*/

#include "stdafx.h"
#include "CarModel.h"
#include "BenzModel.h"
#include "BMWModel.h"
#include "BenzBuilder.h"
#include "BMWBuilder.h"
#include "Director.h"
#include <vector>
#include <iostream>
using std::vector;
using std::string;
using std::cout;
using std::endl;

void DoBenzRun()//没有使用模式时，需要把步骤一条一条的传入模型。
{
	cout << "----------生成奔驰模型----------" << endl;
	CBenzModel *pBenz = new CBenzModel();
	vector<string> seq;
	seq.push_back("engine boom");//客户要求run的时候先发动引擎
	seq.push_back("start");//启动起来
	seq.push_back("stop");//开了一段就停下来

	pBenz->SetSequence(&seq);
	pBenz->Run();
	delete pBenz;
}


void DoBuilder()//使用模式后，由benzBuilder和bmwBuilder来生成，并且使用同样的创建顺序。
{
	cout << "----------用同一个顺序，生成模型----------" << endl;
	vector<string> seq;
	seq.push_back("engine boom");
	seq.push_back("start");
	seq.push_back("stop");

	CBenzBuilder benzBuilder;
	benzBuilder.SetSequence(&seq);
	CBenzModel *pBenz = dynamic_cast<CBenzModel*>(benzBuilder.GetCarModel());
	pBenz->Run();

	CBMWBuilder bmwBuilder;
	bmwBuilder.SetSequence(&seq);
	CBMWModel *pBmw = dynamic_cast<CBMWModel*>(bmwBuilder.GetCarModel());
	pBmw->Run();
}
void DoDirector()//使用指导者来封装创建的逻辑，把创建的顺序内聚在指导者类里面。
{
	cout << "----------批量生成模型----------" << endl;
	CDirector director;

	//1W辆A类型的奔驰车
	for(int i = 0; i < 2; i++)
		director.GetABenzModel()->Run();

	//100W辆B类型的奔驰车
	for(int i = 0; i < 2; i++)
		director.GetBBenzModel()->Run();

	//1000W辆C类型的宝马车
	for(int i = 0; i < 2; i++)
		director.GetCBMWModel()->Run();
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoBenzRun();

	DoBuilder();

	DoDirector();

	return 0;
}