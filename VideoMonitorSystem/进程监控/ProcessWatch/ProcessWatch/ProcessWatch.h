
// ProcessWatch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProcessWatchApp:
// �йش����ʵ�֣������ ProcessWatch.cpp
//

class CProcessWatchApp : public CWinApp
{
public:
	CProcessWatchApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	CString m_cmdParam;
	CString m_currentPath;
};

extern CProcessWatchApp theApp;