
// FrameCollector.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CameraTest.h"


// CFrameCollectorApp:
// �йش����ʵ�֣������ FrameCollector.cpp
//

class CFrameCollectorApp : public CWinApp
{
public:
	CFrameCollectorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

public:
	//��β����'\'��'/'
	const CString &getCurrentPath()const;
	BOOL getConfigureInfo(const CString &strSection);
	CCameraTest& getCamera(){return m_Camera;}

private:
	CCameraTest	m_Camera;
};

extern CFrameCollectorApp theApp;