#pragma once
#include "istrategy.h"
//²ßÂÔÖ®¶þ
class CGivenGreenLight :
	public IStrategy
{
public:
	CGivenGreenLight(void);
	~CGivenGreenLight(void);
	void Operate(void);
};

