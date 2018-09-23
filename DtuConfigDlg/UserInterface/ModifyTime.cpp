// ModifyTime.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "ModifyTime.h"


// CModifyTime 对话框

IMPLEMENT_DYNAMIC(CModifyTime, CDialog)

CModifyTime::CModifyTime(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyTime::IDD, pParent)
{
	pTime = new st_time;
	memset(pTime,0,sizeof(*pTime));
}

CModifyTime::~CModifyTime()
{
	delete pTime;
}

void CModifyTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_YMD, mYMD);
	DDX_Control(pDX, IDC_DATETIMEPICKER_HMS, mHMS);
}


BEGIN_MESSAGE_MAP(CModifyTime, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA, &CModifyTime::OnBnClickedUpData)
END_MESSAGE_MAP()


void CModifyTime::OnBnClickedUpData()
{
	CString str;
	mYMD.GetWindowText(str);
	sscanf(str,"%d/%d/%d",
		&pTime->YY,
		&pTime->MM,
		&pTime->DD);

	mHMS.GetWindowText(str);
	sscanf(str,"%d:%d:%d",
		&pTime->HH,
		&pTime->MI,
		&pTime->SS);
	CDialog::OnOK();
}

void CModifyTime::OnBnClickedCancel()
{
	CDialog::OnCancel();
}
