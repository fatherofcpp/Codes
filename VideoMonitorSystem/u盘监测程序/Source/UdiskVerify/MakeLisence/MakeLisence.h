
// MakeLisence.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMakeLisenceApp:
// �йش����ʵ�֣������ MakeLisence.cpp
//

class CMakeLisenceApp : public CWinApp
{
public:
	CMakeLisenceApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMakeLisenceApp theApp;