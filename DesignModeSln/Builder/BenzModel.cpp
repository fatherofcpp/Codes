#include "StdAfx.h"
#include "BenzModel.h"
#include <iostream>
using std::cout;
using std::endl;
CBenzModel::CBenzModel(void)
{
}
CBenzModel::~CBenzModel(void)
{
}
void CBenzModel::Start()
{
	cout << "���۷���..." << endl;
}
void CBenzModel::Stop()
{
	cout << "����ͣ��..." << endl;
}
void CBenzModel::Alarm()
{
	cout << "��������" << endl;
}
void CBenzModel::EngineBoom()
{
	cout << "������������������...." << endl;
}