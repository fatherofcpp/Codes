
// DtuConfigDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDtuConfigDlgApp:
// �йش����ʵ�֣������ DtuConfigDlg.cpp
//

class CDtuConfigDlgApp : public CWinApp
{
public:
	CDtuConfigDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDtuConfigDlgApp theApp;