/*
//	CEditBoxInfoShow.h Í·ÎÄ¼þ
//
//
*/
#pragma once
#include "afxwin.h"
#include "Thread.h"

class CEditBoxInfoShow: public CThread
{
public:
	CEditBoxInfoShow(void);
	virtual ~CEditBoxInfoShow(void);
	virtual int Run();
	void SetParent(CDialogEx* dlg) {m_dlg = dlg;};
	void SetCtrlID(int id){m_id = id;};
private:
	CDialogEx* m_dlg;
	int m_id;
};

