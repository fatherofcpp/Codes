#include "StdAfx.h"
#include "ZhouSiObserver.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;
CZhouSiObserver::CZhouSiObserver(void) : IObserver("��˹")
{
}
CZhouSiObserver::~CZhouSiObserver(void)
{
}
void CZhouSiObserver::Update( string context )
{
	cout << "��˹���۲쵽�����ӻ���Լ�Ҳ��ʼ���..." << endl;
	this->Cry(context);
	cout << "��˹������Ŀ�����..." << endl;
}
void CZhouSiObserver::Cry( string report )
{
	cout << "��˹��Ϊ��" << report.c_str() << ", �������������ұ���ѽ��" << endl;
}
string CZhouSiObserver::GetName()
{
	return m_name;
}