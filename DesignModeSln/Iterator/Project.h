#pragma once
#include "iproject.h"
#include "IProjectIterator.h"
#include <iostream>
#include <vector>
using std::string;
using std::vector;
class CProject :
	public IProject
{
public:
	CProject(void);
	CProject(string name, int num, int cost);
	~CProject(void);
	string GetProjectInfo();
	void Add(string name, int num, int cost);
	IProjectIterator * GetIterator();
	void Erase();
private:
	string m_name;
	int m_num;
	int m_cost;
	vector<IProject*> m_projectList;
};