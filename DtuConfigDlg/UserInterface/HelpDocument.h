#pragma once
#include "afxwin.h"


// CHelpDocument 对话框

class CHelpDocument : public CDialog
{
	DECLARE_DYNAMIC(CHelpDocument)

public:
	CHelpDocument(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHelpDocument();

// 对话框数据
	enum { IDD = IDD_DIALOG_HELP_DOCUMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CEdit editDocument;
public:
	void InitDlg();
	void GetDocument();
	virtual BOOL OnInitDialog();
};
