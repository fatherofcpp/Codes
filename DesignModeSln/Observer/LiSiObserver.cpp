#include "StdAfx.h"
#include "LiSiObserver.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;
CLiSiObserver::CLiSiObserver(void) : IObserver("��˹")
{
}
CLiSiObserver::~CLiSiObserver(void)
{
}
void CLiSiObserver::Update( string context )
{
	cout << "��˹���۲쵽�����ӻ����ʼ���ϰ�㱨��..." << endl;
	this->ReportToQinShiHuang(context);
	cout << "��˹���㱨��ϣ����ϰ��͸��������ܲ��Գ�..." << endl;
}
void CLiSiObserver::ReportToQinShiHuang( string report )
{
	cout << "��˹�����棬���ϰ壡�������л��--->" << report.c_str() << endl;
}
string CLiSiObserver::GetName()
{
	return m_name;
}