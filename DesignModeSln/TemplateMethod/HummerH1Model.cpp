#include "StdAfx.h"
#include "HummerH1Model.h"
#include <iostream>
using std::cout;
using std::endl;
CHummerH1Model::CHummerH1Model(void)
{
	m_isAlarm = true;
}
CHummerH1Model::~CHummerH1Model(void)
{
}
void CHummerH1Model::Start()
{
	cout << "����H1����..." << endl;
}
void CHummerH1Model::Stop()
{
	cout << "����H1ͣ��..." << endl;
}
void CHummerH1Model::Alarm()
{
	cout << "����H1����" << endl;
}
void CHummerH1Model::EngineBoom()
{
	cout << "����H1��������������...." << endl;
}
bool CHummerH1Model::IsAlarm()
{
	return this->m_isAlarm;
}
void CHummerH1Model::SetAlarm( bool tag )
{
	this->m_isAlarm = tag;
}