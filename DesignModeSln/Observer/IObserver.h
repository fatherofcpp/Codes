#pragma once
#include <iostream>
using std::string;
class IObserver
{
public:
	IObserver(string _name)
	{
		this->m_name = _name;
	}
	virtual ~IObserver(void)
	{
	}
	virtual void Update(string context) = 0;
	virtual string GetName() = 0;//Ϊc++�������ӵĺ���������ɾ��ʱ���ҹ۲��ߡ�
protected:
	string m_name;
};