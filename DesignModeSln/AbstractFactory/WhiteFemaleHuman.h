#pragma once
#include "whitehuman.h"
#include <iostream>
using std::cout;
using std::endl;
class CWhiteFemaleHuman :
	public CWhiteHuman
{
public:
	CWhiteFemaleHuman(void)
	{
	}
	~CWhiteFemaleHuman(void)
	{
	}
	void Sex()
	{
		cout << "�ð����˵��Ա�ΪŮ..." << endl;
	}
};