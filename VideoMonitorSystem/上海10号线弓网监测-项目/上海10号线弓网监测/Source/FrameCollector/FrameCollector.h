
// FrameCollector.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "CameraTest.h"


// CFrameCollectorApp:
// 有关此类的实现，请参阅 FrameCollector.cpp
//

class CFrameCollectorApp : public CWinApp
{
public:
	CFrameCollectorApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
	//结尾不带'\'或'/'
	const CString &getCurrentPath()const;
	BOOL getConfigureInfo(const CString &strSection);
	CCameraTest& getCamera(){return m_Camera;}

private:
	CCameraTest	m_Camera;
};

extern CFrameCollectorApp theApp;