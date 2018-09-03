#include "StdAfx.h"
#include "Project.h"
#include "ProjectIterator.h"
#include <iostream>
#include <vector>
using std::string;
using std::vector;
CProject::CProject( void )
{
	m_name = "";
	m_num = 0;
	m_cost = 0;
}
CProject::CProject(string name, int num, int cost) :m_name(name), m_num(num), m_cost(cost)
{
}
CProject::~CProject(void)
{
}
string CProject::GetProjectInfo()
{
	string info = "";
	info.append("��Ŀ�����ǣ�");
	info.append(this->m_name);
	info.append("\t��Ŀ������");
	info.append("��Ŀ����");
	info.append("\t��Ŀ���ã�");
	info.append("��Ŀ����");
	return info;
}
void CProject::Add( string name, int num, int cost )
{
	this->m_projectList.push_back(new CProject(name, num, cost));
}
IProjectIterator * CProject::GetIterator()
{
	return new CProjectIterator(this->m_projectList);
}
void CProject::Erase()
{
	vector<IProject*>::reverse_iterator projectDelIt = m_projectList.rbegin();
	for (; projectDelIt != m_projectList.rend(); projectDelIt++)
	{
		delete (*projectDelIt);
		(*projectDelIt) = NULL;
	}
	m_projectList.clear();
}