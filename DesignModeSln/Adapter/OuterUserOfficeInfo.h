#pragma once
#include <iostream>
using std::cout;
using std::endl;
using std::string;
class COuterUserOfficeInfo
{
public:
	COuterUserOfficeInfo(void)
	{
	}
	~COuterUserOfficeInfo(void)
	{
	}
	string GetOfficeTelNumber()
	{
		cout << "�칫�ҵ绰��..." << endl;
		return "0";
	}
	string GetJobPosition()
	{
		cout << "����˵�ְλ��BOSS..." << endl;
		return "0";
	}
};