/*
适配器模式，使用之处比较特殊，不属于常规设计模式，主要用于不同系统之间的处理。
是将一个类的接口转换成客户希望的另外一个接口。Adapter模式使得原本由于接口不兼容而不能一起工作的那些类
可以一起工作。也是一个比较简单的模式，直接上代码了。

main()，主程序
IUserInfo，本系统内接口
CUserInfo，本系统内实现类
IOuterUser，外系统接口
COuterUser，外系统实现类
COuterUserInfo，本系统内适配类
说明：COuterUserInfo实现IUserInfo接口，将外部系统实现类COuterUser转换成本系统内的接口IUserInfo。
使用外部数据跟使用本系统内部数据一样。
注意：COuterUserInfo继承了IUserInfo，如果同时继承了COuterUser则是类适配器。
如果COuterUserInfo只是使用了COuterUser则是对象适配器。
*/

#include "stdafx.h"
#include "IOuterUser.h"
#include "IUserInfo.h"
#include "UserInfo.h"
#include "OuterUserInfo.h"

void DoIt()
{
	IUserInfo *pYourGirl = new CUserInfo();
	pYourGirl->GetMobileNumber();
	delete pYourGirl;
}

void NowDoIt()
{
	IUserInfo *pYourGirl = new COuterUserInfo();

	pYourGirl->GetMobileNumber();
	
	delete pYourGirl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoIt();

	NowDoIt();

	return 0;
}