#include "stdafx.h"
#include "FIFO.h"
#include <iostream>
using namespace std;

//CFIFO* CFIFO::m_pInstance = NULL;
shared_ptr<CFIFO> CFIFO::m_pInstance = NULL;
CFIFO::CFIFO():
	m_list(NULL)
{
	InitializeCriticalSection(&g_ThreadCode);
	m_list = new list<string>;
};

CFIFO::~CFIFO()
{
	DeleteCriticalSection( &g_ThreadCode ); 
	if(m_list != NULL)
	{
		delete m_list;
		m_list = NULL;
		TRACE("desruct CFIFO class,  delete m_list...\r\n");
	}
	TRACE("desruct CFIFO class ...\r\n");
}

bool CFIFO::isEmpty()
{
	if(m_list == NULL)
		return true;
	if(m_list->size() == 0)
		return true;
	else
		return false;
}

void CFIFO::Write(string str)
{
	EnterCriticalSection( &g_ThreadCode );
	if(m_list != NULL)
	{
		m_list->push_back(str);
	}
	LeaveCriticalSection( &g_ThreadCode ); 
}

string CFIFO::Read()
{
	string str = "";
	EnterCriticalSection( &g_ThreadCode );
	if(m_list != NULL && !m_list->empty())
	{
		str = m_list->front();
		m_list->pop_front();
	}
	LeaveCriticalSection( &g_ThreadCode ); 
	return str;
}

