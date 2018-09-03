#pragma once
#include "IHuman.h"
class IHumanFactory
{
public:
	IHumanFactory(void)
	{
	}
	virtual ~IHumanFactory(void)
	{
	}
	virtual IHuman * CreateYellowHuman() = 0;
	virtual IHuman * CreateWhiteHuman() = 0;
	virtual IHuman * CreateBlackHuman() = 0;
};