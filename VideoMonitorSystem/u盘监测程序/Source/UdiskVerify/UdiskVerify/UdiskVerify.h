
// UdiskVerify.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUdiskVerifyApp:
// �йش����ʵ�֣������ UdiskVerify.cpp
//

class CUdiskVerifyApp : public CWinApp
{
public:
	CUdiskVerifyApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUdiskVerifyApp theApp;