#pragma once
class IHuman
{
public:

	IHuman(void)
	{
	}

	virtual ~IHuman(void)
	{
	}

	virtual void Laugh() = 0;
	virtual void Cry() = 0;
	virtual void Talk() = 0;
	virtual void Sex() = 0;
};