/*
//	FIFO HEAD FILE
//	Author:
//	Date:
*/
#pragma once
#include <Windows.h>
#include <list>
#include <string>
#include <memory>
#include "Noncopyable.h"
using namespace std;

class CFIFO:
	public noncopyable
{
public:
	CFIFO();
	~CFIFO();
	//static CFIFO* GetInstance();
	static shared_ptr<CFIFO> GetInstance()
	{
		if(m_pInstance == 0)
			m_pInstance = make_shared<CFIFO>();
			//m_pInstance = shared_ptr<CFIFO>(new CFIFO);
		return m_pInstance;
	}
	bool isEmpty();
	void Write(string);
	string Read();
private:
	list<string> *m_list;	
	CRITICAL_SECTION g_ThreadCode;
	//static CFIFO* m_pInstance;
	static shared_ptr<CFIFO> m_pInstance;
};