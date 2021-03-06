
// GeoMonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GeoMonitor.h"
#include "GeoMonitorDlg.h"
#include "afxdialogex.h"

#include "BsGlobal.h"
#include "CmFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGeoMonitorDlg 对话框




CGeoMonitorDlg::CGeoMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGeoMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGeoMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGeoMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGeoMonitorDlg 消息处理程序

BOOL CGeoMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitUi();
	SetTimer(1, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGeoMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGeoMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGeoMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CGeoMonitorDlg::~CGeoMonitorDlg()
{

}

void CGeoMonitorDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

BOOL CGeoMonitorDlg::PreTranslateMessage(MSG* pMsg)
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

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGeoMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		Data2Ui();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CGeoMonitorDlg::InitUi()
{
	USES_CONVERSION; 

	GetDlgItem(IDC_EDIT_TEMP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_GEOPARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ARCSTRENGTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ARCRATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WARN_NOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_VERSION)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FPS)->EnableWindow(FALSE);

	{
		CString strVal;
		char msg[256];

		std::string cur_time = NSP_STP_CM::GetNowTimeStr();
#ifdef _DEBUG
		sprintf(msg, "%s Debug, Begin %s", gVersion.c_str(), cur_time.c_str());
#else
		sprintf(msg, "%s Release, Begin %s", gVersion.c_str(), cur_time.c_str());
#endif
		strVal = A2W(msg);
		GetDlgItem(IDC_EDIT_VERSION)->SetWindowText(strVal);
	}
}


void CGeoMonitorDlg::AddTemp( const char* msg)
{
	m_temp = msg;
}

void CGeoMonitorDlg::AddGeoParam( const char* msg)
{
	m_geoParam = msg;
}

void CGeoMonitorDlg::AddArcStrength( const char* msg)
{
	m_arcStrength = msg;
}

void CGeoMonitorDlg::AddArcRate( const char* msg )
{
	m_arcRate = msg;
}

void CGeoMonitorDlg::AddCurrent( const char* msg )
{
	m_current = msg;
}

void CGeoMonitorDlg::AddWarn( const char* msg )
{
	m_warn = msg;
}

void CGeoMonitorDlg::AddStructFps( const char* msg )
{
	m_structFps = msg;
}

void CGeoMonitorDlg::AddTempFps( const char* msg )
{
	m_tempFps = msg;
}

void CGeoMonitorDlg::AddArcCollectorFps( const char* msg )
{
	m_arcColFps = msg;
}

void CGeoMonitorDlg::AddGeoUpFps( const char* msg )
{
	m_geoUpFps = msg;
}

void CGeoMonitorDlg::AddGeoDownLFps( const char* msg )
{
	m_geoDownLFps = msg;
}

void CGeoMonitorDlg::AddGeoDownRFps( const char* msg )
{
	m_geoDownRFps = msg;
}

void CGeoMonitorDlg::Data2Ui()
{
	USES_CONVERSION; 

	CString strVal;

	if (!m_temp.empty())
	{
		strVal = A2W(m_temp.c_str());
		GetDlgItem(IDC_EDIT_TEMP)->SetWindowText(strVal);
	}

	if (!m_geoParam.empty())
	{
		strVal = A2W(m_geoParam.c_str());
		GetDlgItem(IDC_EDIT_GEOPARAM)->SetWindowText(strVal);
	}

	if (!m_arcStrength.empty())
	{
		strVal = A2W(m_arcStrength.c_str());
		GetDlgItem(IDC_EDIT_ARCSTRENGTH)->SetWindowText(strVal);
	}

	if (!m_arcRate.empty())
	{
		strVal = A2W(m_arcRate.c_str());
		GetDlgItem(IDC_EDIT_ARCRATE)->SetWindowText(strVal);
	}

	if (!m_current.empty())
	{
		strVal = A2W(m_current.c_str());
		GetDlgItem(IDC_EDIT_CURRENT)->SetWindowText(strVal);
	}

	if (!m_warn.empty())
	{
		strVal = A2W(m_warn.c_str());
		GetDlgItem(IDC_EDIT_WARN_NOW)->SetWindowText(strVal);
	}

	if (!m_structFps.empty() || !m_tempFps.empty() || !m_arcColFps.empty() 
		|| !m_geoUpFps.empty() || !m_geoDownLFps.empty() || !m_geoDownRFps.empty())
	{
		std::string show = m_structFps + m_tempFps + m_arcColFps + m_geoUpFps + m_geoDownLFps + m_geoDownRFps;
		strVal = A2W(show.c_str());
		GetDlgItem(IDC_EDIT_FPS)->SetWindowText(strVal);
	}
}





