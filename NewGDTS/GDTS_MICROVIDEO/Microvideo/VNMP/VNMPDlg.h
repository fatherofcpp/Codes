// VNMPDlg.h : 头文件
//

#pragma once

//#include "MyFunction.h"

#include <evhttp.h>

#include "xml/include/xmlnode.h"
#include "xml/include/xmlparse.h"
#include "inc.h"

#include "command.h"
#include "CriticalSection.h"
#include "afxwin.h"

#include "vnmpLib.h"
#include "MyFunction.h"

// CVNMPDlg 对话框
class CVNMPDlg : public CDialog
{
// 构造
public:
	CVNMPDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VNMP_DIALOG };

	HANDLE m_wowzalock;
	HANDLE m_Lock;

	CArray<MyAvFrame*,MyAvFrame*> p_YuvArray;
	CCriticalSection m_streamlock;
	DEV_VNMP_PARAMS m_devParams;


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
		
	// 启动拉流函数
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
//	int ClosePull(void *pStream,void * pParams);
};
