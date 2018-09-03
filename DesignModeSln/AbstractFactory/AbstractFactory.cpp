/*
ģ����
���󹤳����ṩһ������һϵ����ػ��໥��������Ľӿڣ�������ָ�����Ǿ�����ࡣ
���ڹ���������˵�����󹤳���ʵ��һϵ�в�Ʒ�����������󹤳���ע�ز�Ʒ����ϡ�
7.1.����
main()��Ů�
IHuman����Ʒ�ӿ�
CYellowHuman�������Ʒ֮һ
CYellowFemaleHuman�������Ʒ֮һ
CYellowMaleHuman�������Ʒ֮��
CWhiteHuman�������Ʒ֮��
CWhiteFemaleHuman�������Ʒ֮��
CWhiteMaleHuman�������Ʒ֮��
CBlackHuman�������Ʒ֮��
CBlackFemaleHuman�������Ʒ֮��
CBlackMaleHuman�������Ʒ֮��
IHumanFactory�����󹤳�
CStandardHumanFactory�����󹤳����ࣨ������п��ޣ�
CFemaleHumanFactory������֮һ
CMaleHumanFactory������֮��
*/

#include "stdafx.h"
#include "IHuman.h"
#include "IHumanFactory.h"
#include "FemaleHumanFactory.h"
#include "MaleHumanFactory.h"
#include "MaleHumanFactory.cpp"
#include "YellowFemaleHuman.h"
#include "YellowMaleHuman.h"
#include "WhiteFemaleHuman.h"
#include "WhiteMaleHuman.h"

void DoIt()
{
	IHumanFactory *pFemaleHumanFactory = new CFemaleHumanFactory<CYellowFemaleHuman>();
	IHuman *pYellowFemaleHuman = pFemaleHumanFactory->CreateYellowHuman();
	pYellowFemaleHuman->Cry();
	pYellowFemaleHuman->Laugh();
	pYellowFemaleHuman->Talk();
	pYellowFemaleHuman->Sex();
	delete pYellowFemaleHuman;
	delete pFemaleHumanFactory;

	IHumanFactory *pMaleHumanFactory = new CMaleHumanFactory<CYellowMaleHuman>();
	IHuman *pYellowMaleHuman = pMaleHumanFactory->CreateYellowHuman();
	pYellowMaleHuman->Cry();
	pYellowMaleHuman->Laugh();
	pYellowMaleHuman->Talk();
	pYellowMaleHuman->Sex();
	delete pYellowMaleHuman;
	delete pMaleHumanFactory;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoIt();
	return 0;
}

