#pragma once
#include "istrategy.h"
//����֮һ
class CBackDoor :
	public IStrategy
{
public:
	CBackDoor(void);
	~CBackDoor(void);
	void Operate(void);
};

