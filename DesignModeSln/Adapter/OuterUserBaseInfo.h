#pragma once
#include <iostream>
using std::cout;
using std::endl;
using std::string;
class COuterUserBaseInfo
{
public:
	COuterUserBaseInfo(void)
	{
	}
	~COuterUserBaseInfo(void)
	{
	}
	string GetUserName()
	{
		cout << "��������..." << endl;
		return "0";
	}
	string GetMobileNumber()
	{
		cout << "����˵��ֻ�������0001..." << endl;
		return "0";
	}
};