// UserInterface\PassWordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "PassWordDlg.h"
#include "afxdialogex.h"


// CPassWordDlg �Ի���

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


// CPassWordDlg ��Ϣ�������
 

BOOL CPassWordDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_RETURN: //�س�  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	editUserName.GetWindowText(str);
	userName = str;

	editPassWord.GetWindowText(str);
	passWord = str;
	CDialog::OnOK();

}
