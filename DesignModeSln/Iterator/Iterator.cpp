/*
����ṩһ�ַ���˳�����һ���ۺ϶����и���Ԫ�أ����ֲ��豩¶�ö�����ڲ���ʾ��
main()���ͻ�
IProject����Ʒ�ӿ�
CProject����Ʒ��
IIterator���������ӿ�
IProjectIterator����Ʒ�������ӿ�
CProjectIterator����Ʒ������ʵ����
˵����CProjectʵ�ֲ�Ʒ�࣬�ܹ�����һ����������ָ�롣
�������������װ��Ʒ�����һ�����顣���Ե�����������Next����ʱ�����Ա���������������Ԫ�ء�
����˵�������ô���ʵ��vector<int>::iterator��vector<int>::const_iterator��
*/

#include "stdafx.h"
#include "IProject.h"
#include "Project.h"
#include "ProjectIterator.h"
#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;
void DoIt()
{
	cout << "----------δʹ�õ���ģʽ----------" << endl;
	vector<IProject*> projectList;

	projectList.push_back(new CProject("�����ս��Ŀ", 10, 100000));
	projectList.push_back(new CProject("Ťתʱ����Ŀ", 100, 10000000));
	projectList.push_back(new CProject("���˸�����Ŀ", 10000, 1000000000));

	for (int i = 4; i < 6; i ++)
	{
		string name = "";
		name.append("��");
		//name.append(CConvert::ToString(i));
		name.append("����Ŀ");
		projectList.push_back(new CProject(name, i * 5, i * 1000000));
	}

	vector<IProject*>::const_iterator projectIt = projectList.begin();
	for (; projectIt != projectList.end(); projectIt++)
		cout << (*projectIt)->GetProjectInfo().c_str() << endl;

	vector<IProject*>::reverse_iterator projectDelIt = projectList.rbegin();
	for (; projectDelIt != projectList.rend(); projectDelIt++)
	{
		delete (*projectDelIt);
		(*projectDelIt) = NULL;
	}
	projectList.clear();
}
void DoNew()
{
	cout << "----------ʹ�õ���ģʽ----------" << endl;
	IProject *pproject = new CProject();
	pproject->Add("�����ս��Ŀ", 10, 100000);
	pproject->Add("Ťתʱ����Ŀ", 100, 10000000);
	pproject->Add("���˸�����Ŀ", 10000, 1000000000);

	for (int i = 4; i < 6; i ++)
	{
		string name = "";
		name.append("��");
		//name.append(CConvert::ToString(i));
		name.append("����Ŀ");
		pproject->Add(name, i * 5, i * 1000000);
	}

	IProjectIterator *pprojectIt = pproject->GetIterator();
	while(pprojectIt->HasNext())
	{
		IProject *p = dynamic_cast<IProject*>(pprojectIt->Next());
		cout << p->GetProjectInfo().c_str() << endl;
	}
	delete pprojectIt;
	pprojectIt = NULL;
	pproject->Erase();
	delete pproject;
	pproject = NULL;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//ʹ��Iteratorģʽ֮ǰ
	DoIt();

	//ʹ��Iterator
	DoNew();

	return 0;
}