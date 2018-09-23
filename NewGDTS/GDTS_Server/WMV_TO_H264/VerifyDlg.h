#if !defined(AFX_VERIFYDLG_H__9E7DF7AD_8AE1_4644_955B_8BD1DB40F618__INCLUDED_)
#define AFX_VERIFYDLG_H__9E7DF7AD_8AE1_4644_955B_8BD1DB40F618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VerifyDlg.h : header file
//
#include "MD5/MD5.h"
/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg dialog

class CVerifyDlg : public CDialog
{
// Construction
public:
	void char_convert(LPCTSTR lpszStr,char *str);

	void Get_MAC();
	int GetParam(char dst[]);
	int GetLocalinfo();
	CVerifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVerifyDlg)
	enum { IDD = IDD_DIALOG_VERIFY };
	CEdit	m_edit2;
	CEdit	m_edit1;
	//}}AFX_DATA
	CMD5 md;
	unsigned char pauth[16];
	char szParam[1024];
	char szMAC[20];
	void OnCancel();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVerifyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERIFYDLG_H__9E7DF7AD_8AE1_4644_955B_8BD1DB40F618__INCLUDED_)
