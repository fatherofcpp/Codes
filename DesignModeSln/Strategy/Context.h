#pragma once
//╫Удр
#include "IStrategy.h"
class CContext
{
public:
	CContext(IStrategy *pStrategy);
	~CContext(void);
	void Operate(void);
private:
	IStrategy *m_pStrategy;
};
