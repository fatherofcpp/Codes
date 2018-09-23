#pragma once
#include "afxwin.h"


// CBasicDlg 对话框

class CBasicDlg : public CDialog
{
	DECLARE_DYNAMIC(CBasicDlg)

public:
	CBasicDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBasicDlg();

// 对话框数据
	enum { IDD = IDD_BASECONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
protected:
	virtual BOOL OnInitDialog(void);
public:
	CComboBox m_company;
	CString m_deviceIp;
	CString m_devicePort;
	CString m_deviceUser;
	CString m_devicePasswd;
	CString m_deviceId;
	CString m_serverIp;
	CString m_serverPort;
	CString m_pubName;
	afx_msg void OnBnClickedOk();
};
