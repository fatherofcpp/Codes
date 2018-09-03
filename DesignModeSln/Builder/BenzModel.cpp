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
	cout << "±¼³Û·¢¶¯..." << endl;
}
void CBenzModel::Stop()
{
	cout << "±¼³ÛÍ£³µ..." << endl;
}
void CBenzModel::Alarm()
{
	cout << "±¼³ÛÃùµÑ" << endl;
}
void CBenzModel::EngineBoom()
{
	cout << "±¼³ÛÒýÇæÉùÒôÊÇÕâÑù...." << endl;
}