#pragma once
#include "ihuman.h"
#include <iostream>
using std::cout;
using std::endl;
class CYellowHuman :
	public IHuman
{
public:
	CYellowHuman(void)
	{
	}
	~CYellowHuman(void)
	{
	}
	void Laugh()
	{
		cout << "��ɫ���ֻ��Ц���Ҹ�ѽ��" << endl;
	}
	void Cry()
	{
		cout << "��ɫ���ֻ��" << endl;
	}
	void Talk()
	{
		cout << "��ɫ���ֻ�˵����һ��˵�Ķ���˫�ֽ�" << endl;
	}
	virtual void Sex() = 0;
};