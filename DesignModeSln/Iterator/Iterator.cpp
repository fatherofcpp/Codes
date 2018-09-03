/*
概念：提供一种方法顺序访问一个聚合对象中各个元素，而又不需暴露该对象的内部表示。
main()，客户
IProject，产品接口
CProject，产品类
IIterator，迭代器接口
IProjectIterator，产品迭代器接口
CProjectIterator，产品迭代器实现类
说明：CProject实现产品类，能够返回一个迭代器的指针。
这个迭代器将封装产品类里的一个数组。所以迭代器在运行Next函数时，可以遍历这个数组的所有元素。
简单来说，就是用代码实现vector<int>::iterator或vector<int>::const_iterator。
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
	cout << "----------未使用迭代模式----------" << endl;
	vector<IProject*> projectList;

	projectList.push_back(new CProject("星球大战项目", 10, 100000));
	projectList.push_back(new CProject("扭转时空项目", 100, 10000000));
	projectList.push_back(new CProject("超人改造项目", 10000, 1000000000));

	for (int i = 4; i < 6; i ++)
	{
		string name = "";
		name.append("第");
		//name.append(CConvert::ToString(i));
		name.append("个项目");
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
	cout << "----------使用迭代模式----------" << endl;
	IProject *pproject = new CProject();
	pproject->Add("星球大战项目", 10, 100000);
	pproject->Add("扭转时空项目", 100, 10000000);
	pproject->Add("超人改造项目", 10000, 1000000000);

	for (int i = 4; i < 6; i ++)
	{
		string name = "";
		name.append("第");
		//name.append(CConvert::ToString(i));
		name.append("个项目");
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
	//使用Iterator模式之前
	DoIt();

	//使用Iterator
	DoNew();

	return 0;
}