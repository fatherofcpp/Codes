#pragma once
#include "standardhumanfactory.h"


template<class T>
class CFemaleHumanFactory :
	public CStandardHumanFactory<T>
{
public:

	CFemaleHumanFactory(void)
	{
	}

	~CFemaleHumanFactory(void)
	{
	}

	IHuman * CreateYellowHuman()
	{
		return CreateHuman();
	}

	IHuman * CreateWhiteHuman()
	{
		return CreateHuman();
	}

	IHuman * CreateBlackHuman()
	{
		return CreateHuman();
	}

};