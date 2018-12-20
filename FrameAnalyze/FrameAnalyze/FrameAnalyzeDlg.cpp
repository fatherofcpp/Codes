
// FrameAnalyzeDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFrameAnalyzeDlg �Ի���




CFrameAnalyzeDlg::CFrameAnalyzeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFrameAnalyzeDlg::IDD, pParent),
	m_cBtZt("����"),
	m_cBtDb("�ײ�"),
	m_cBtZsj("ת���"),
	m_cBtQb("ȹ��"),
	m_cTitle("ͼ������"),
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


// CFrameAnalyzeDlg ��Ϣ�������

BOOL CFrameAnalyzeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitUi();
	const int c_timer_delt_ms = 10;
	m_IDEvent = 1;
	SetTimer(m_IDEvent, c_timer_delt_ms, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFrameAnalyzeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		LoadBg();

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CFrameAnalyzeDlg::InitUi()
{
	USES_CONVERSION; 

	CRect rect;      
	GetClientRect(&rect);     //ȡ�ͻ�����С    
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
	// TODO: �ڴ����ר�ô����/����û���
	KillTimer(m_IDEvent);

	CDialogEx::OnCancel();
}


void CFrameAnalyzeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
	POINT new_point; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;      
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	new_point.x = recta.right-recta.left;  
	new_point.y = recta.bottom-recta.top;  
	fsp[0] = (float)new_point.x / m_oldPoint.x;  
	fsp[1] = (float)new_point.y / m_oldPoint.y;  


	HWND  hwnd_child = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
	while(hwnd_child)      
	{      
		CPoint old_topleft_point; //���Ͻ�  
		CPoint top_left_point;
		CPoint old_bottom_right_point; //���½�  
		CPoint bottom_right_point; 
		CRect rect;  
		int woc = ::GetDlgCtrlID(hwnd_child);//ȡ��ID  
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
		float brightness;	//����0-255
		float contrast;	//�Աȶ�0-100
		float zh;//�ۺ�����
		char resolution[512] = "";	//�ֱ���

		char img_path[260];
		strcpy(img_path, file_path.c_str());

		NSP_STP_CM::EvaluationImage(img_path, brightness, contrast, zh, resolution, frame_pos);

		float ld = brightness;
		sprintf(msg, "����ֵ%.3f", ld);
		AddLd(msg);

		float dbd = contrast;
		sprintf(msg, "�Աȶ�ֵ%.3f", dbd);
		AddDbd(msg);

		sprintf(msg, "�ֱ���ֵ%s", resolution);
		AddFbl(msg);

		const float c_jg = 60.0;
		if (zh > c_jg)
		{
			sprintf(msg, "�ۺ�����%.3f, �ϸ�", zh);
		}
		else
		{
			sprintf(msg, "�ۺ�����%.3f, ���ϸ�", zh);
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
			analyze_area = "δ��������";
		}
		sprintf(msg, "��������:%s", analyze_area.c_str());
		AddReverse1(msg);

#ifdef _DEBUG
		sprintf(msg, "�汾��Ϣ%s Debug", gVersion.c_str());
#else
		sprintf(msg, "�汾��Ϣ%s Release", gVersion.c_str());
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
		sprintf(msg, "���ű���%.3f", fScale);
		cv_dll.ContinueImg(file_path, 0, fScale, 100, msg);
	}
}
