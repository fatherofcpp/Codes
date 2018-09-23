// HistoryData.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "HistoryData.h"

#include "afxdialogex.h"


// CHistoryData 对话框

IMPLEMENT_DYNAMIC(CHistoryData, CDialog)

CHistoryData::CHistoryData(CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryData::IDD, pParent)
{
	dataLen = 1024 * 128;
	historyData = new char[dataLen];
	ZeroMemory(historyData,dataLen);
}

CHistoryData::~CHistoryData()
{
	delete[] historyData;
}

void CHistoryData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HISTORY_DATA, mHistoryData);
}


BEGIN_MESSAGE_MAP(CHistoryData, CDialog)
END_MESSAGE_MAP()


// CHistoryData 消息处理程序


BOOL CHistoryData::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mHistoryData.SetWindowText(historyData);
	//mHistoryData.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHistoryData::SetData( char* data, int data_len )
{
	ZeroMemory(historyData,dataLen);
	memcpy(historyData,data,data_len);
	int line_num = 10;
	
}
