#pragma once
#include "afxwin.h"


// CHelpDocument �Ի���

class CHelpDocument : public CDialog
{
	DECLARE_DYNAMIC(CHelpDocument)

public:
	CHelpDocument(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHelpDocument();

// �Ի�������
	enum { IDD = IDD_DIALOG_HELP_DOCUMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CEdit editDocument;
public:
	void InitDlg();
	void GetDocument();
	virtual BOOL OnInitDialog();
};
