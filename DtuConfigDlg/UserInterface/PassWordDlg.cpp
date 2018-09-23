// UserInterface\PassWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "PassWordDlg.h"
#include "afxdialogex.h"


// CPassWordDlg 对话框

IMPLEMENT_DYNAMIC(CPassWordDlg, CDialog)

CPassWordDlg::CPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassWordDlg::IDD, pParent)
{

}

CPassWordDlg::~CPassWordDlg()
{
}

void CPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USER, editUserName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, editPassWord);
}


BEGIN_MESSAGE_MAP(CPassWordDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ENSURE, &CPassWordDlg::OnBnClickedButtonEnsure)
END_MESSAGE_MAP()


// CPassWordDlg 消息处理程序
 

BOOL CPassWordDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_RETURN: //回车  
			return TRUE;  
		case VK_ESCAPE: //ESC  
			return TRUE;  
		}  
	} 
	if(pMsg->message == WM_RBUTTONDOWN) 
	{  
		return TRUE;
	} 
	return CDialog::PreTranslateMessage(pMsg);
}


void CPassWordDlg::OnBnClickedButtonEnsure()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	editUserName.GetWindowText(str);
	userName = str;

	editPassWord.GetWindowText(str);
	passWord = str;
	CDialog::OnOK();

}
