/*
模板类
抽象工厂，提供一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。
对于工厂方法来说，抽象工厂可实现一系列产品的生产，抽象工厂更注重产品的组合。
7.1.解释
main()，女娲
IHuman，产品接口
CYellowHuman，抽象产品之一
CYellowFemaleHuman，具体产品之一
CYellowMaleHuman，具体产品之二
CWhiteHuman，抽象产品之二
CWhiteFemaleHuman，具体产品之三
CWhiteMaleHuman，具体产品之四
CBlackHuman，抽象产品之三
CBlackFemaleHuman，具体产品之五
CBlackMaleHuman，具体产品之六
IHumanFactory，抽象工厂
CStandardHumanFactory，抽象工厂基类（此类可有可无）
CFemaleHumanFactory，工厂之一
CMaleHumanFactory，工厂之二
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

