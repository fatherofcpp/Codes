#include "StdAfx.h"
#include "OuterUser.h"
#include "OuterUserBaseInfo.h"
#include "OuterUserHomeInfo.h"
#include "OuterUserOfficeInfo.h"

COuterUserBaseInfo * COuterUser::GetUserBaseInfo()
{
	return new COuterUserBaseInfo();
}
COuterUserHomeInfo * COuterUser::GetUserHomeInfo()
{
	return new COuterUserHomeInfo();
}
COuterUserOfficeInfo * COuterUser::GetUserOfficeInfo()
{
	return new COuterUserOfficeInfo();
}