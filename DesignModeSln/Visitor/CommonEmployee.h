#pragma once
#include "employee.h"
#include "IVisitor.h"
#include <iostream>
using std::string;
class CCommonEmployee :
	public CEmployee
{
public:
	CCommonEmployee(void);
	~CCommonEmployee(void);
public:
	string GetJob();
	void SetJob(string job);
	void Accept(IVisitor *pVisitor);
private:
	string m_job;
};

