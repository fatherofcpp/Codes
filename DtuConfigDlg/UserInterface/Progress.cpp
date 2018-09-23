// Progress.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "Progress.h"
#include "CommonFunction.h"

// CProgress 对话框

IMPLEMENT_DYNAMIC(CProgress, CDialogEx)

CProgress::CProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CProgress::IDD, pParent)
{
	minRangle = 0;
	maxRangle = 100;
	curStep = 0;
	progressName = "";
	AutoProgress = true;
	progressRun = false;
}

CProgress::~CProgress()
{
}

void CProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_STATE, mProgressButton);
	DDX_Control(pDX, IDC_PROGRESS_NAME, mProgressName);
}


BEGIN_MESSAGE_MAP(CProgress, CDialog)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void CProgress::SetMinRangle( int val )
{
	minRangle = val;
}

void CProgress::SetMaxRangle( int val )
{
	maxRangle = val;
}

void CProgress::SetProgressName( CString& progress_name )
{
	progressName = progress_name;
}


LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	return 1;
}

// CProgress 消息处理程序


BOOL CProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED); 
	} 
	mProgressButton.SetRange(minRangle,maxRangle);
	mProgressName.SetWindowText(progressName);
	SetTimer(1,200,NULL);

	//hMouse = SetWindowsHookEx(WH_MOUSE,MouseProc,NULL,GetCurrentThreadId());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProgress::OnTimer( UINT_PTR nIDEvent )
{
	CString str;
	static int enter_time = 0;
	enter_time ++;
	int cur_step = 0;
	if(AutoProgress)
	{
		cur_step = enter_time % maxRangle;
		str.Format("%s中...",progressName);
	}
	else
	{
		cur_step = curStep;
		str.Format("%s中...已完成%02d%%",progressName,cur_step);
	}

	mProgressButton.SetPos(cur_step);
	//mProgressButton.SetStep(cur_step);
	//mProgressButton.StepIt();
	mProgressName.SetWindowText(str);

	progressRun = true;

	CDialog::OnTimer(nIDEvent);
}

void CProgress::SetCurStep( int val )
{
	curStep = val;
}

void CProgress::SetAutoProgress( bool is_auto )
{
	AutoProgress = is_auto;
}



BOOL CProgress::PreTranslateMessage(MSG* pMsg)
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
	return CDialog::PreTranslateMessage(pMsg);
}

bool CProgress::GetRunFlag()
{
	return progressRun;
}


void CProgress::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACE("[%s]", __FUNCTION__);
	progressRun = false;

	CDialog::OnClose();
}
