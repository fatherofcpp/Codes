#pragma once
#include <iostream>
using std::cout;
using std::endl;
using std::string;
class COuterUserHomeInfo
{
public:
	COuterUserHomeInfo(void)
	{
	}
	~COuterUserHomeInfo(void)
	{
	}
	string GetHomeAddress()
	{
		cout << "这里是员工的家庭地址..." << endl;
		return "0";
	}
	string GetHomeTelNumber()
	{
		cout << "员工的家庭电话是..." << endl;
		return "0";
	}
};