#pragma once
#include "ihumanfactory.h"
#include "IHuman.h"

template<class T> 
class CStandardHumanFactory :
	public IHumanFactory
{
public:
	CStandardHumanFactory(void)
	{
	}
	~CStandardHumanFactory(void)
	{
	}
	IHuman * CreateHuman()
	{
		return new T;
	}
};