/*
������ģʽ��ʹ��֮���Ƚ����⣬�����ڳ������ģʽ����Ҫ���ڲ�ͬϵͳ֮��Ĵ���
�ǽ�һ����Ľӿ�ת���ɿͻ�ϣ��������һ���ӿڡ�Adapterģʽʹ��ԭ�����ڽӿڲ����ݶ�����һ��������Щ��
����һ������Ҳ��һ���Ƚϼ򵥵�ģʽ��ֱ���ϴ����ˡ�

main()��������
IUserInfo����ϵͳ�ڽӿ�
CUserInfo����ϵͳ��ʵ����
IOuterUser����ϵͳ�ӿ�
COuterUser����ϵͳʵ����
COuterUserInfo����ϵͳ��������
˵����COuterUserInfoʵ��IUserInfo�ӿڣ����ⲿϵͳʵ����COuterUserת���ɱ�ϵͳ�ڵĽӿ�IUserInfo��
ʹ���ⲿ���ݸ�ʹ�ñ�ϵͳ�ڲ�����һ����
ע�⣺COuterUserInfo�̳���IUserInfo�����ͬʱ�̳���COuterUser��������������
���COuterUserInfoֻ��ʹ����COuterUser���Ƕ�����������
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