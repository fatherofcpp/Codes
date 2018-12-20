
// FrameAnalyzeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FrameAnalyze.h"
#include "FrameAnalyzeDlg.h"
#include "afxdialogex.h"
#include "CmOpenCvDll.h"
#include "CmCvvImage.h"
#include "CmFunction/CmFunction.h"
#include "BsMainTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const bool gShowCv = true;

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


// CFrameAnalyzeDlg 对话框




CFrameAnalyzeDlg::CFrameAnalyzeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFrameAnalyzeDlg::IDD, pParent),
	m_cBtZt("整体"),
	m_cBtDb("底部"),
	m_cBtZsj("转向架"),
	m_cBtQb("裙摆"),
	m_cTitle("图像评估"),
	m_cWidth(512),
	m_cHeight(512)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CFrameAnalyzeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_Image);
}

BEGIN_MESSAGE_MAP(CFrameAnalyzeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CFrameAnalyzeDlg::OnBnClickedButtonZt)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ZSJ, &CFrameAnalyzeDlg::OnBnClickedButtonZs)
	ON_BN_CLICKED(IDC_BUTTON_DB, &CFrameAnalyzeDlg::OnBnClickedButtonDb)
	ON_BN_CLICKED(IDC_BUTTON_QB, &CFrameAnalyzeDlg::OnBnClickedButtonQb)
END_MESSAGE_MAP()


// CFrameAnalyzeDlg 消息处理程序

BOOL CFrameAnalyzeDlg::OnInitDialog()
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
	const int c_timer_delt_ms = 10;
	m_IDEvent = 1;
	SetTimer(m_IDEvent, c_timer_delt_ms, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFrameAnalyzeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFrameAnalyzeDlg::OnPaint()
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
		LoadBg();

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFrameAnalyzeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFrameAnalyzeDlg::AddLd( const char* msg )
{
	m_ld = msg;
}

void CFrameAnalyzeDlg::AddDbd( const char* msg )
{
	m_dbd = msg;
}

void CFrameAnalyzeDlg::AddFbl( const char* msg )
{
	m_fbl = msg;
}

void CFrameAnalyzeDlg::AddTotal( const char* msg )
{
	m_total = msg;
}

void CFrameAnalyzeDlg::AddReverse1( const char* msg )
{
	m_reverse1 = msg;
}

void CFrameAnalyzeDlg::AddReverse2( const char* msg )
{
	m_reverse2 = msg;
}

void CFrameAnalyzeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == m_IDEvent)
	{
		Data2Ui();

		static bool s_set_max = true;
		if (s_set_max)
		{
			s_set_max = false;
			//ShowWindow(SW_MINIMIZE);
			ShowWindow(SW_MAXIMIZE);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CFrameAnalyzeDlg::PreTranslateMessage(MSG* pMsg)
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

void CFrameAnalyzeDlg::InitUi()
{
	USES_CONVERSION; 

	CRect rect;      
	GetClientRect(&rect);     //取客户区大小    
	m_oldPoint.x = rect.right - rect.left;  
	m_oldPoint.y = rect.bottom - rect.top;  


	GetDlgItem(IDC_EDIT_LD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DBD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FBL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TOTAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_REVERSE1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_REVERSE2)->EnableWindow(FALSE);

	{
		CString strVal;
		strVal = A2W(m_cTitle.c_str());
		SetWindowText(strVal);
	}

	{
		CString strVal;
		strVal = A2W(m_cBtZt.c_str());
		GetDlgItem(IDC_BUTTON_LOAD)->SetWindowText(strVal);

		strVal = A2W(m_cBtZsj.c_str());
		GetDlgItem(IDC_BUTTON_ZSJ)->SetWindowText(strVal);

		strVal = A2W(m_cBtDb.c_str());
		GetDlgItem(IDC_BUTTON_DB)->SetWindowText(strVal);

		strVal = A2W(m_cBtQb.c_str());
		GetDlgItem(IDC_BUTTON_QB)->SetWindowText(strVal);
	}

	if (!gShowCv)
	{
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, "{8,1238},{8,688},{608,688},{608,1238},{8,1238}");
		m_Image.SetStdAreaOrg(buffer, 5, m_cWidth, m_cHeight, SATT_MIDDLE_POLENUM);
		m_Image.Clear();
	}
}

void CFrameAnalyzeDlg::Data2Ui()
{
	USES_CONVERSION; 

	CString strVal;

	if (!m_ld.empty())
	{
		strVal = A2W(m_ld.c_str());
		GetDlgItem(IDC_EDIT_LD)->SetWindowText(strVal);
	}

	if (!m_dbd.empty())
	{
		strVal = A2W(m_dbd.c_str());
		GetDlgItem(IDC_EDIT_DBD)->SetWindowText(strVal);
	}

	if (!m_fbl.empty())
	{
		strVal = A2W(m_fbl.c_str());
		GetDlgItem(IDC_EDIT_FBL)->SetWindowText(strVal);
	}

	if (!m_total.empty())
	{
		strVal = A2W(m_total.c_str());
		GetDlgItem(IDC_EDIT_TOTAL)->SetWindowText(strVal);
	}

	if (!m_reverse1.empty())
	{
		strVal = A2W(m_reverse1.c_str());
		GetDlgItem(IDC_EDIT_REVERSE1)->SetWindowText(strVal);
	}

	if (!m_reverse2.empty())
	{
		strVal = A2W(m_reverse2.c_str());
		GetDlgItem(IDC_EDIT_REVERSE2)->SetWindowText(strVal);
	}
}

CString CFrameAnalyzeDlg::BootOpenFile( bool open)
{
	CString strFile = _T("");
	CFileDialog dlgFile(open, NULL, NULL, OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	return strFile;
}


void CFrameAnalyzeDlg::OnBnClickedButtonZt()
{
	USES_CONVERSION; 

	CString strVal = BootOpenFile();
	if (strVal.IsEmpty())
	{
		return;
	}

	std::string file_path = W2A(strVal);

	AnalyzeFrame(file_path, E_AREA_ZT);
}




void CFrameAnalyzeDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	KillTimer(m_IDEvent);

	CDialogEx::OnCancel();
}


void CFrameAnalyzeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType==SIZE_RESTORED || nType==SIZE_MAXIMIZED)  
	{  
		ReSize();  
	}  

	if (!gShowCv)
	{
		if (GetDlgItem(IDC_STATIC_IMAGE) == NULL)
		{
			return;
		}

		CRect rectImage;
		m_Image.GetWindowRect(&rectImage);
		CMyCanvas::GetStretchRate(rectImage, m_cWidth, m_cHeight,
			m_cWidth, m_cHeight, 0, rectImage, rectImage);
		m_Image.m_ImgRect = rectImage;

		m_Image.SetStdArea();
	}
}

void CFrameAnalyzeDlg::ReSize()
{
	float fsp[2];  
	POINT new_point; //获取现在对话框的大小  
	CRect recta;      
	GetClientRect(&recta);     //取客户区大小    
	new_point.x = recta.right-recta.left;  
	new_point.y = recta.bottom-recta.top;  
	fsp[0] = (float)new_point.x / m_oldPoint.x;  
	fsp[1] = (float)new_point.y / m_oldPoint.y;  


	HWND  hwnd_child = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while(hwnd_child)      
	{      
		CPoint old_topleft_point; //左上角  
		CPoint top_left_point;
		CPoint old_bottom_right_point; //右下角  
		CPoint bottom_right_point; 
		CRect rect;  
		int woc = ::GetDlgCtrlID(hwnd_child);//取得ID  
		GetDlgItem(woc)->GetWindowRect(rect);    
		ScreenToClient(rect);    
		old_topleft_point = rect.TopLeft();    
		top_left_point.x = long(old_topleft_point.x * fsp[0]);    
		top_left_point.y = long(old_topleft_point.y * fsp[1]);    
		old_bottom_right_point = rect.BottomRight();    
		bottom_right_point.x = long(old_bottom_right_point.x * fsp[0]);    
		bottom_right_point.y = long(old_bottom_right_point.y * fsp[1]);    
		rect.SetRect(top_left_point, bottom_right_point);    
		GetDlgItem(woc)->MoveWindow(rect, TRUE);  
		hwnd_child=::GetWindow(hwnd_child, GW_HWNDNEXT);      
	}  
	m_oldPoint = new_point; 
}

void CFrameAnalyzeDlg::LoadBg()
{
	CRect rc;  
	GetWindowRect(&rc);  
	CImage mImage;  
	if(mImage.Load(_T("res\\BG.png")) == S_OK)  
	{  
		mImage.Draw(GetDC()->GetSafeHdc(), CRect(0,0,rc.Width(),rc.Height()));  
	} 
}


void CFrameAnalyzeDlg::OnBnClickedButtonZs()
{
	USES_CONVERSION; 

	CString strVal = BootOpenFile();
	if (strVal.IsEmpty())
	{
		return;
	}

	std::string file_path = W2A(strVal);

	AnalyzeFrame(file_path, E_AREA_ZSJ);
}


void CFrameAnalyzeDlg::OnBnClickedButtonDb()
{
	USES_CONVERSION; 

	CString strVal = BootOpenFile();
	if (strVal.IsEmpty())
	{
		return;
	}

	std::string file_path = W2A(strVal);

	AnalyzeFrame(file_path, E_AREA_DB);
}


void CFrameAnalyzeDlg::OnBnClickedButtonQb()
{
	USES_CONVERSION; 

	CString strVal = BootOpenFile();
	if (strVal.IsEmpty())
	{
		return;
	}

	std::string file_path = W2A(strVal);

	AnalyzeFrame(file_path, E_AREA_QB);
}

void CFrameAnalyzeDlg::AnalyzeFrame(std::string file_path, int frame_pos)
{
	USES_CONVERSION; 
	char msg[512];

	{
		float brightness;	//亮度0-255
		float contrast;	//对比度0-100
		float zh;//综合评分
		char resolution[512] = "";	//分辨率

		char img_path[260];
		strcpy(img_path, file_path.c_str());

		NSP_STP_CM::EvaluationImage(img_path, brightness, contrast, zh, resolution, frame_pos);

		float ld = brightness;
		sprintf(msg, "亮度值%.3f", ld);
		AddLd(msg);

		float dbd = contrast;
		sprintf(msg, "对比度值%.3f", dbd);
		AddDbd(msg);

		sprintf(msg, "分辨率值%s", resolution);
		AddFbl(msg);

		const float c_jg = 60.0;
		if (zh > c_jg)
		{
			sprintf(msg, "综合评分%.3f, 合格", zh);
		}
		else
		{
			sprintf(msg, "综合评分%.3f, 不合格", zh);
		}
		AddTotal(msg);

		std::string analyze_area;
		if (frame_pos == E_AREA_ZT)
		{
			analyze_area = m_cBtZt;
		}
		else if (frame_pos == E_AREA_DB)
		{
			analyze_area = m_cBtDb;
		}
		else if (frame_pos == E_AREA_ZSJ)
		{
			analyze_area = m_cBtZsj;
		}
		else if (frame_pos == E_AREA_QB)
		{
			analyze_area = m_cBtQb;
		}
		else
		{
			analyze_area = "未定义区域";
		}
		sprintf(msg, "评估区域:%s", analyze_area.c_str());
		AddReverse1(msg);

#ifdef _DEBUG
		sprintf(msg, "版本信息%s Debug", gVersion.c_str());
#else
		sprintf(msg, "版本信息%s Release", gVersion.c_str());
#endif
		AddReverse2(msg);
	}

	if (gShowCv)
	{
		CStatic* pStc =(CStatic*)GetDlgItem(IDC_STATIC_IMAGE);
		CDC* pDC = pStc->GetDC();
		if (1)
		{
			CRect rect;
			pStc->GetClientRect(&rect);
			int color = COLOR_INACTIVECAPTION;
			DWORD dw = GetSysColor(color);
			CBrush br(dw);
			pDC->FillRect( rect, &br );
		}
		NSP_STP_CM::ShowImgStatic(file_path, pStc, pDC);
		ReleaseDC(pDC);
	}
	else
	{
		int iscolor = 1;
		int width = 0;
		int height = 0;
		int frame_size = 0;
		unsigned char* pData = NULL;
		NSP_STP_CM::LoadImgFrame(file_path, iscolor, pData, width, height, frame_size);

		ImgInfo* p_img_info = new ImgInfo();
		p_img_info->setBuf(pData);
		p_img_info->setSize(frame_size);

		int channels = 0;
		if (iscolor == 0)
		{
			channels = 1;
		}
		else
		{
			channels = 3;
		}
		m_Image.LoadImge(p_img_info, width, height, channels);
	}

	{
		CString strVal;
		strVal = A2W(m_cBtZt.c_str());
		GetDlgItem(IDC_BUTTON_LOAD)->SetWindowText(strVal);

		strVal = A2W(m_cBtZsj.c_str());
		GetDlgItem(IDC_BUTTON_ZSJ)->SetWindowText(strVal);

		strVal = A2W(m_cBtDb.c_str());
		GetDlgItem(IDC_BUTTON_DB)->SetWindowText(strVal);

		strVal = A2W(m_cBtQb.c_str());
		GetDlgItem(IDC_BUTTON_QB)->SetWindowText(strVal);
	}

	if(0)
	{
		NSP_STP_CM::COpenCvDll cv_dll;
		double fScale = 0.2;
		sprintf(msg, "缩放比例%.3f", fScale);
		cv_dll.ContinueImg(file_path, 0, fScale, 100, msg);
	}
}
