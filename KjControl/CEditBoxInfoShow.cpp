#include "stdafx.h"
#include "CEditBoxInfoShow.h"
#include "FIFO.h"
#include <atltrace.h>
CEditBoxInfoShow::CEditBoxInfoShow():
m_dlg(NULL)
{}

CEditBoxInfoShow::~CEditBoxInfoShow()
{}

int CEditBoxInfoShow::Run()
{
	string str;
	while(this->m_running)
	{
		if(!CFIFO::GetInstance()->isEmpty())
		{
			str = CFIFO::GetInstance()->Read();
			((CEdit*)m_dlg->GetDlgItem(m_id))->ReplaceSel(str.c_str());
		}
		//Sleep(10);
	}
	TRACE("CEditBoxInfoShow thread exit ... ...\r\n");
	return 0;
}