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
	cout << "������..." << endl;
}
void CBMWModel::Stop()
{
	cout << "����ͣ��..." << endl;
}
void CBMWModel::Alarm()
{
	cout << "��������" << endl;
}
void CBMWModel::EngineBoom()
{
	cout << "������������������...." << endl;
}