#pragma once
#include "ihumanfactory.h"
class CYellowHumanFactory :
	public IHumanFactory
{
public:
	CYellowHumanFactory(void);
	~CYellowHumanFactory(void);
	virtual IHuman * CreateHuman(void);
};