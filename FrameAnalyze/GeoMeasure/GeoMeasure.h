
// GeoMeasure.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGeoMeasureApp:
// �йش����ʵ�֣������ GeoMeasure.cpp
//

class CGeoMeasureApp : public CWinApp
{
public:
	CGeoMeasureApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	CString GetCurrentPath(){return m_currentPath;}
private:
	CString m_currentPath;
};

extern CGeoMeasureApp theApp;