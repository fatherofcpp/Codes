
// GeoMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "BsMainTask.h"


// CGeoMonitorApp:
// �йش����ʵ�֣������ GeoMonitor.cpp
//

class CGeoMonitorApp : public CWinApp
{
public:
	CGeoMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	CMainTask* GetMainTask(){return m_pCMainTask;}
	CString GetCurrentPath(){return m_currentPath;}
private:
	CMainTask* m_pCMainTask;
	CString m_currentPath;
};

extern CGeoMonitorApp theApp;