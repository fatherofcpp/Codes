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
	virtual IHuman * CreateHuman() = 0;
};