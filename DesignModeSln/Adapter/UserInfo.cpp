#include "StdAfx.h"
#include "UserInfo.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;
CUserInfo::CUserInfo(void)
{
}
CUserInfo::~CUserInfo(void)
{
}
string CUserInfo::GetUserName()
{
	cout << "��������..." << endl;
	return "0";
}
string CUserInfo::GetHomeAddress()
{
	cout << "������Ա���ļ�ͥ��ַ..." << endl;
	return "0";
}
string CUserInfo::GetMobileNumber()
{
	cout << "����˵��ֻ�������0000..." << endl;
	return "0";
}
string CUserInfo::GetOfficeTelNumber()
{
	cout << "�칫�ҵ绰��..." << endl;
	return "0";
}
string CUserInfo::GetJobPosition()
{
	cout << "����˵�ְλ��BOSS..." << endl;
	return "0";
}
string CUserInfo::GetHomeTelNumber()
{
	cout << "Ա���ļ�ͥ�绰��..." << endl;
	return "0";
}