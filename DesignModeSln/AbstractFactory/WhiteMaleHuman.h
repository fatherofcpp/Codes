#pragma once
#include "whitehuman.h"
#include <iostream>
using std::cout;
using std::endl;
class CWhiteMaleHuman :
	public CWhiteHuman
{
public:
	CWhiteMaleHuman(void)
	{
	}
	~CWhiteMaleHuman(void)
	{
	}
	void Sex()
	{
		cout << "�ð����˵��Ա�Ϊ��..." << endl;
	}
};