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
		cout << "姓名叫做..." << endl;
		return "0";
	}
	string GetMobileNumber()
	{
		cout << "这个人的手机号码是0001..." << endl;
		return "0";
	}
};