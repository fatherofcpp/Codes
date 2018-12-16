
// TestDlg.h : 头文件
//

#pragma once

//
#include "CmFunction/CmDefType.h"
#include "xml/xml.hpp"
#include "MString/CmMString.h"
#include "DataBase/CmDBSqlite.h"
#include "Exception/MasException.h"
#include "CmFunction/CmFunction.h"
//
using namespace NSP_STP_CM;

// CTestDlg 对话框
class CTestDlg : public CDialogEx
{
// 构造
public:
	CTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

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
	int test(MCS_CHAR* pBuff);
	static void CALLBACK thread_exception(void* pParam);
	 
};
