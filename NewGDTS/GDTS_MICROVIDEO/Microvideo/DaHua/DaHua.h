// DaHua.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDaHuaApp:
// �йش����ʵ�֣������ DaHua.cpp
//

class CDaHuaApp : public CWinApp
{
public:
	CDaHuaApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDaHuaApp theApp;