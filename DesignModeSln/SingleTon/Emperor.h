#pragma once
/*
单例模式，就是在系统中只允许产生这个类的一个实例
CEmperor，需要单例的类
说明：很多大臣拜见的皇帝，只有一个。体现在面向对象方面，CEmperor定义一个静态指针，和一个静态函数，
私有化构造函数、析构函数、构造函数复制、重载赋值语句。
注意：线程安全，采用互斥体的方式实现。
*/
#include <iostream>
#include <windows.h>
using std::cout;
using std::endl;
using std::string;
class CEmperor
{
public:
	static CEmperor * GetInstance();
	static void ReleaseInstance();
	
	void EmperorInfo(void);
	void SetEmperorTag(string tag);
private:
	CEmperor(void);
	virtual ~CEmperor(void);

	string m_EmperorTag;
	
	static CEmperor *m_pEmperor;
	static HANDLE m_pMutex;
	
};

