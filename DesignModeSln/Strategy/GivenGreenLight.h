#pragma once
#include "istrategy.h"
//����֮��
class CGivenGreenLight :
	public IStrategy
{
public:
	CGivenGreenLight(void);
	~CGivenGreenLight(void);
	void Operate(void);
};

