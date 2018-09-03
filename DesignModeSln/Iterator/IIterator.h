#pragma once
class IProject;
class IIterator
{
public:
	IIterator(void)
	{
	}
	virtual ~IIterator(void)
	{
	}
	virtual bool HasNext() = 0;
	virtual IProject * Next() = 0;
};

