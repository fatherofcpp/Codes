#pragma once
/*
����ģʽ��������ϵͳ��ֻ�������������һ��ʵ��
CEmperor����Ҫ��������
˵�����ܶ�󳼰ݼ��Ļʵۣ�ֻ��һ������������������棬CEmperor����һ����ָ̬�룬��һ����̬������
˽�л����캯�����������������캯�����ơ����ظ�ֵ��䡣
ע�⣺�̰߳�ȫ�����û�����ķ�ʽʵ�֡�
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

