#include "StdAfx.h"
#include "PageGroup.h"
#include <iostream>
using std::cout;
using std::endl;
CPageGroup::CPageGroup(void)
{
}
CPageGroup::~CPageGroup(void)
{
}
void CPageGroup::Find()
{
	cout << "�ҵ�������..." << endl;
}
void CPageGroup::Add()
{
	cout << "�ͻ�Ҫ������һ��ҳ��..." << endl;
}
void CPageGroup::Delete()
{
	cout << "�ͻ�Ҫ��ɾ��һ��ҳ��..." << endl;
}
void CPageGroup::Change()
{
	cout << "�ͻ�Ҫ���޸�һ��ҳ��..." << endl;
}
void CPageGroup::Plan()
{
	cout << "�ͻ�Ҫ��ҳ�����ƻ�..." << endl;
}