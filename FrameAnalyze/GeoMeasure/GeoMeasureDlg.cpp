
// GeoMeasureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GeoMeasure.h"
#include "GeoMeasureDlg.h"
#include "afxdialogex.h"

#include "BsAnalyeStruct.h"
#include "BsAnalyzeGeo.h"
#include "CmOpenCvDll.h"
#include "BsMainTask.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CGeoMeasureDlg �Ի���




CGeoMeasureDlg::CGeoMeasureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGeoMeasureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_isContinueShow = false;
}

void CGeoMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGeoMeasureDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MEASURE, &CGeoMeasureDlg::OnBnClickedButtonMeasure)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CGeoMeasureDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_STRUCT_M, &CGeoMeasureDlg::OnBnClickedButtonStructM)
	ON_BN_CLICKED(IDC_BUTTON_FRAME_DEAL, &CGeoMeasureDlg::OnBnClickedButtonFrameDeal)
END_MESSAGE_MAP()


// CGeoMeasureDlg ��Ϣ�������

BOOL CGeoMeasureDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitUi();
	SetTimer(1, 100, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGeoMeasureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGeoMeasureDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGeoMeasureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGeoMeasureDlg::OnBnClickedButtonMeasure()
{
	USES_CONVERSION; 

	CString strVal = BootOpenFile();
	if (strVal.IsEmpty())
	{
		return;
	}
	std::string file_path = W2A(strVal);

	CAnalyzeGeo geo_ana;
	{
		geo_ana.m_areaWith = BsMainTask::m_s_areaWith;
		geo_ana.m_areaHeight = BsMainTask::m_s_areaHeight;
		geo_ana.m_top = BsMainTask::m_s_top;
		geo_ana.m_bottom = BsMainTask::m_s_bottom;
		geo_ana.m_left = BsMainTask::m_s_left;
		geo_ana.m_rigth = BsMainTask::m_s_rigth;
		geo_ana.m_threPoint = BsMainTask::m_s_threPoint;
	}
	geo_ana.analyze_geo(file_path);

	{
		char msg[256];
		sprintf(msg, "����ֵ=%.3f����", geo_ana.m_geo.lc);
		AddLc(msg);
	}
	{
		char msg[256];
		sprintf(msg, "����ֵ=%.3f����", geo_ana.m_geo.dg);
		AddDg(msg);
	}

	{
		CStatic* pStc =(CStatic*)GetDlgItem(IDC_STATIC_DG);
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
}


void CGeoMeasureDlg::OnBnClickedButtonShow()
{
	USES_CONVERSION; 

	CString strVal = BootOpenFile();
	if (strVal.IsEmpty())
	{
		return;
	}

	NSP_STP_CM::DisImg(W2A(strVal), 0.5, 3 * 1000);
}


void CGeoMeasureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	USES_CONVERSION; 

	if (nIDEvent == 1)
	{
		Data2Ui();
		int check_state = ((CButton*)(GetDlgItem(IDC_CHECK_REALTIME_PIC)))->GetCheck();
		m_isContinueShow = (check_state == 1);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CGeoMeasureDlg::PreTranslateMessage(MSG* pMsg)
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

void CGeoMeasureDlg::InitUi()
{
	USES_CONVERSION; 

	GetDlgItem(IDC_EDIT_DG)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_VER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_STRUCT)->EnableWindow(FALSE);

	{
		CString strVal;
		char msg[256];
#ifdef _DEBUG
		sprintf(msg, "%s Debug", gVersion.c_str());
#else
		sprintf(msg, "%s Release", gVersion.c_str());
#endif
		strVal = A2W(msg);
		GetDlgItem(IDC_EDIT_VER)->SetWindowText(strVal);
	}
}

void CGeoMeasureDlg::Data2Ui()
{
	USES_CONVERSION; 

	CString strVal;

	if (!m_dg.empty())
	{
		strVal = A2W(m_dg.c_str());
		GetDlgItem(IDC_EDIT_DG)->SetWindowText(strVal);
	}

	if (!m_lc.empty())
	{
		strVal = A2W(m_lc.c_str());
		GetDlgItem(IDC_EDIT_LC)->SetWindowText(strVal);
	}

	if (!m_struct.empty())
	{
		strVal = A2W(m_struct.c_str());
		GetDlgItem(IDC_EDIT_STRUCT)->SetWindowText(strVal);
	}
}

void CGeoMeasureDlg::AddDg( const char* msg )
{
	m_dg = msg;
}

void CGeoMeasureDlg::AddLc( const char* msg )
{
	m_lc = msg;
}

CString CGeoMeasureDlg::BootOpenFile( bool open /*= TRUE*/ )
{
	CString strFile = _T("");
	//CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.cfg)|*.cfg|All Files (*.*)|*.*||"), NULL);
	CFileDialog dlgFile(open, NULL, NULL, OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	return strFile;
}

void CGeoMeasureDlg::OnBnClickedButtonStructM()
{
	USES_CONVERSION; 

	//test
	{
		CAnalyeStruct struct_ana;
		std::string file_path;
		for (int i = 0; i < 12; i ++)
		{
			char tmp[260];
			sprintf(tmp, "G:\\com_nw\\2�Ϻ�10����\\4���β���\\���β���\\GeoMeasure\\Debug\\�ṹ���\\%d.jpg", i+1);
			file_path = tmp;
			std::string res = struct_ana.analyze_struct(file_path);
			AddStruct(res.c_str());

			if (m_isContinueShow)
			{
				NSP_STP_CM::COpenCvDll cv_dll;
				const int fps = 100;
				const int delay_tms = 1000 / fps;
				cv_dll.ContinueImg(file_path, 0, 0.3, delay_tms, "");
			}
		}
		return;
	}
	//test

	CString strVal = BootOpenFile();
	if (strVal.IsEmpty())
	{
		return;
	}

	CAnalyeStruct struct_ana;
	std::string res = struct_ana.analyze_struct(W2A(strVal));

	{
		AddStruct(res.c_str());
	}
}

void CGeoMeasureDlg::AddStruct( const char* msg )
{
	m_struct = msg;
}


void CGeoMeasureDlg::OnBnClickedButtonFrameDeal()
{
	USES_CONVERSION; 
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString strVal = BootOpenFile();
	//if (strVal.IsEmpty())
	//{
	//	return;
	//}

	//std::string file_path = W2A(strVal);

	std::string file_path = "E:\\1.jpg";
	int ret = NSP_STP_CM::ImgRectAdd(file_path, 50, 50, 100, 150);
	if (ret != 1)
	{
	}
}
