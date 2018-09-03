#pragma once
class IProduct
{
public:
	IProduct(void)
	{
	}
	virtual ~IProduct(void)
	{
	}
	virtual void BeProducted() = 0;
	virtual void BeSelled() = 0;
};