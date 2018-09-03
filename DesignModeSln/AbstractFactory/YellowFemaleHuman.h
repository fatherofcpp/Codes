#pragma once
#include "yellowhuman.h"
#include <iostream>
using std::cout;
using std::endl;
class CYellowFemaleHuman :
	public CYellowHuman
{
public:
	CYellowFemaleHuman(void)
	{
	}
	~CYellowFemaleHuman(void)
	{
	}
	void Sex()
	{
		cout << "该黄种人的性别为女..." << endl;
	}
};