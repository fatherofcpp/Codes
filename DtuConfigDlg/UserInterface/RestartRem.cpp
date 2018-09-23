// RestartRem.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "RestartRem.h"

// CRestartRem 对话框

IMPLEMENT_DYNAMIC(CRestartRem, CDialog)

CRestartRem::CRestartRem(CWnd* pParent /*=NULL*/)
	: CDialog(CRestartRem::IDD, pParent)
{

}

CRestartRem::~CRestartRem()
{
}

void CRestartRem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRestartRem, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_YES, &CRestartRem::OnBnClickedButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_LATER, &CRestartRem::OnBnClickedButtonLater)
END_MESSAGE_MAP()


// CRestartRem 消息处理程序


void CRestartRem::OnBnClickedButtonYes()
{
	CDialog::OnOK();
}


void CRestartRem::OnBnClickedButtonLater()
{
	CDialog::OnCancel();
}
