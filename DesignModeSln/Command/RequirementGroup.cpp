#include "StdAfx.h"
#include "RequirementGroup.h"
#include <iostream>
using std::cout;
using std::endl;
CRequirementGroup::CRequirementGroup(void)
{
}
CRequirementGroup::~CRequirementGroup(void)
{
}
void CRequirementGroup::Find()
{
	cout << "�ҵ�������..." << endl;
}
void CRequirementGroup::Add()
{
	cout << "�ͻ�Ҫ������һ������..." << endl;
}
void CRequirementGroup::Delete()
{
	cout << "Ҫ��ɾ��һ������..." << endl;
}
void CRequirementGroup::Change()
{
	cout << "�ͻ�Ҫ���޸�һ������..." << endl;
}
void CRequirementGroup::Plan()
{
	cout << "�ͻ�Ҫ���������ƻ�..." << endl;
}