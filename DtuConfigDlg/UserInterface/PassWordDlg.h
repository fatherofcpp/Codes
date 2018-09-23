#pragma once
#include "afxwin.h"
#include "vector"
using namespace std;

// CPassWordDlg �Ի���

class CPassWordDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassWordDlg)

public:
	CPassWordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassWordDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
