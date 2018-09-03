#include "StdAfx.h"
#include "Handler.h"
#include <iostream>
using std::cout;
using std::endl;
CHandler::CHandler(int _level ) : m_level(_level)
{
	m_pNextHandler = NULL;
}
CHandler::~CHandler(void)
{
}
void CHandler::HandleMessage( IWomen *pwomen )
{
	if (pwomen->GetType() == this->m_level)
	{
		this->Response(pwomen);
	}
	else
	{
		if(this->m_pNextHandler != NULL)
			this->m_pNextHandler->HandleMessage(pwomen);
		else
			cout << "----------û�ط���ʾ�ˣ���������----------" << endl;
	}
}
void CHandler::SetNext( CHandler *phandler )
{
	m_pNextHandler = phandler;
}