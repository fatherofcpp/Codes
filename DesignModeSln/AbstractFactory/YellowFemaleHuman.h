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
		cout << "�û����˵��Ա�ΪŮ..." << endl;
	}
};