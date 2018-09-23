// RestartRem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "RestartRem.h"

// CRestartRem �Ի���

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


// CRestartRem ��Ϣ�������


void CRestartRem::OnBnClickedButtonYes()
{
	CDialog::OnOK();
}


void CRestartRem::OnBnClickedButtonLater()
{
	CDialog::OnCancel();
}
