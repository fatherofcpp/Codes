#include "StdAfx.h"
#include "BMWModel.h"
#include <iostream>
using std::cout;
using std::endl;
CBMWModel::CBMWModel(void)
{
}
CBMWModel::~CBMWModel(void)
{
}
void CBMWModel::Start()
{
	cout << "宝马发动..." << endl;
}
void CBMWModel::Stop()
{
	cout << "宝马停车..." << endl;
}
void CBMWModel::Alarm()
{
	cout << "宝马鸣笛" << endl;
}
void CBMWModel::EngineBoom()
{
	cout << "宝马引擎声音是这样...." << endl;
}