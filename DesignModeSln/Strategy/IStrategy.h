#pragma once

/*
策略接口
说明：一个策略放到一个锦囊里。当用的时候，找到这个锦囊，从锦囊里拿出策略来使用。
注意：锦囊只是简单的装载和调用策略，锦囊里没有逻辑。策略会有更大的自主权，运行更多的逻辑。
*/

class IStrategy
{
public:
	IStrategy(void);
	virtual ~IStrategy(void);
	virtual void Operate(void) = 0;
};

