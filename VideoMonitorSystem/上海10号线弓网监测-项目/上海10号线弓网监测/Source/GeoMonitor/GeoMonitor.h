
// GeoMonitor.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "BsMainTask.h"


// CGeoMonitorApp:
// 有关此类的实现，请参阅 GeoMonitor.cpp
//

class CGeoMonitorApp : public CWinApp
{
public:
	CGeoMonitorApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
	CMainTask* GetMainTask(){return m_pCMainTask;}
	CString GetCurrentPath(){return m_currentPath;}
private:
	CMainTask* m_pCMainTask;
	CString m_currentPath;
};

extern CGeoMonitorApp theApp;