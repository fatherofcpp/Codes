// WMV_TO_H264.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWMV_TO_H264App:
// �йش����ʵ�֣������ WMV_TO_H264.cpp
//

class CWMV_TO_H264App : public CWinApp
{
public:
	CWMV_TO_H264App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWMV_TO_H264App theApp;