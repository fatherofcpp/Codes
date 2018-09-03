#pragma once
#include "ihumanfactory.h"
class CWhiteHumanFactory :
	public IHumanFactory
{
public:
	CWhiteHumanFactory(void);
	~CWhiteHumanFactory(void);
	virtual IHuman * CreateHuman(void);
};
