#pragma once
#include "yellowhuman.h"
#include <iostream>
using std::cout;
using std::endl;
class CYellowMaleHuman :
	public CYellowHuman
{
public:
	CYellowMaleHuman(void)
	{
	}
	~CYellowMaleHuman(void)
	{
	}
	void Sex()
	{
		cout << "该黄种人的性别为男..." << endl;
	}
};