
// WatchDogProgress.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWatchDogProgressApp:
// �йش����ʵ�֣������ WatchDogProgress.cpp
//

class CWatchDogProgressApp : public CWinApp
{
public:
	CWatchDogProgressApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	CString GetCurrentPath();
};

extern CWatchDogProgressApp theApp;