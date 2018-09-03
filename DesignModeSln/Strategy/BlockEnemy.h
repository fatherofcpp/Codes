#pragma once
#include "istrategy.h"
//²ßÂÔÖ®Èı
class CBlockEnemy :
	public IStrategy
{
public:
	CBlockEnemy(void);
	~CBlockEnemy(void);
	void Operate(void);
};

