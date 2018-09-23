#pragma once
#include "afxwin.h"
#include "vector"
using namespace std;

// CPassWordDlg 对话框

class CPassWordDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassWordDlg)

public:
	CPassWordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPassWordDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonEnsure();
	CEdit editUserName;
	CEdit editPassWord;

public:
	string GetUserName(){return userName;};
	string GetPassWord(){return passWord;};
private:
	string userName;
	string passWord;
};
