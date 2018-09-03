#include "StdAfx.h"
#include "OuterUserInfo.h"
#include "OuterUserBaseInfo.h"
#include "OuterUserHomeInfo.h"
#include "OuterUserOfficeInfo.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;
COuterUserInfo::COuterUserInfo(void)
{
	m_pOuterUser = new COuterUser();
}
COuterUserInfo::~COuterUserInfo(void)
{
	delete m_pOuterUser;
}
string COuterUserInfo::GetUserName()
{
	COuterUserBaseInfo *pBaseInfo = m_pOuterUser->GetUserBaseInfo();
	pBaseInfo->GetUserName();
	delete pBaseInfo;
	pBaseInfo = NULL;
	return "0";
}
string COuterUserInfo::GetHomeAddress()
{
	COuterUserHomeInfo *pHomeInfo = m_pOuterUser->GetUserHomeInfo();
	pHomeInfo->GetHomeAddress();
	delete pHomeInfo;
	pHomeInfo = NULL;
	return "0";
}
string COuterUserInfo::GetMobileNumber()
{
	COuterUserBaseInfo *pBaseInfo = m_pOuterUser->GetUserBaseInfo();
	pBaseInfo->GetMobileNumber();
	delete pBaseInfo;
	pBaseInfo = NULL;
	return "0";
}
string COuterUserInfo::GetOfficeTelNumber()
{
	COuterUserOfficeInfo *pOfficeInfo = m_pOuterUser->GetUserOfficeInfo();
	pOfficeInfo->GetOfficeTelNumber();
	delete pOfficeInfo;
	pOfficeInfo = NULL;
	return "0";
}
string COuterUserInfo::GetJobPosition()
{
	COuterUserOfficeInfo *pOfficeInfo = m_pOuterUser->GetUserOfficeInfo();
	pOfficeInfo->GetJobPosition();
	delete pOfficeInfo;
	pOfficeInfo = NULL;
	return "0";
}
string COuterUserInfo::GetHomeTelNumber()
{
	COuterUserHomeInfo *pHomeInfo = m_pOuterUser->GetUserHomeInfo();
	pHomeInfo->GetHomeTelNumber();
	delete pHomeInfo;
	pHomeInfo = NULL;
	return "0";
}