
// PantographMonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PantographMonitor.h"
#include "PantographMonitorDlg.h"
#include "afxdialogex.h"

#include "BsGlobal.h"
#include "CmFunction.h"

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


// CPantographMonitorDlg �Ի���




CPantographMonitorDlg::CPantographMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPantographMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPantographMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPantographMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPantographMonitorDlg ��Ϣ�������

BOOL CPantographMonitorDlg::OnInitDialog()
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
	SetTimer(1, 1000, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPantographMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPantographMonitorDlg::OnPaint()
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
HCURSOR CPantographMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CPantographMonitorDlg::~CPantographMonitorDlg()
{
}

void CPantographMonitorDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}


BOOL CPantographMonitorDlg::PreTranslateMessage(MSG* pMsg)
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

void CPantographMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		Data2Ui();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPantographMonitorDlg::InitUi()
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
		std::string cur_time = NSP_STP_CM::GetNowTimeStr();

		CString strVal;
		char msg[256];
#ifdef _DEBUG
		sprintf(msg, "%s Debug, Begin %s", gVersion.c_str(), cur_time.c_str());
#else
		sprintf(msg, "%s Release, Begin %s", gVersion.c_str(), cur_time.c_str());
#endif
		strVal = A2W(msg);
		GetDlgItem(IDC_EDIT_VERSION)->SetWindowText(strVal);
	}

	{
		CString strVal;
		char msg[256];
		sprintf(msg, "�Ϻ�10���߹������ϵͳ");
		strVal = A2W(msg);
		SetWindowText(strVal);
	}

	{
		//GetDlgItem(IDC_STATIC_CURRENT)->ShowWindow(FALSE);
		//GetDlgItem(IDC_EDIT_CURRENT)->ShowWindow(FALSE);
		//GetDlgItem(IDC_EDIT_FPS)->ShowWindow(FALSE);
		//GetDlgItem(IDC_STATIC_FPS)->ShowWindow(FALSE);
	}
}

void CPantographMonitorDlg::AddTemp( const char* msg)
{
	m_temp = msg;
}

void CPantographMonitorDlg::AddGeoParam( const char* msg)
{
	m_geoParam = msg;
}

void CPantographMonitorDlg::AddArcStrength( const char* msg)
{
	m_arcStrength = msg;
}

void CPantographMonitorDlg::AddArcRate( const char* msg )
{
	m_arcRate = msg;
}

void CPantographMonitorDlg::AddCurrent( const char* msg )
{
	m_current = msg;
}

void CPantographMonitorDlg::AddWarn( const char* msg )
{
	m_warn = msg;
}

void CPantographMonitorDlg::AddStructFps( const char* msg )
{
	m_structFps = msg;
}

void CPantographMonitorDlg::AddTempFps( const char* msg )
{
	m_tempFps = msg;
}

void CPantographMonitorDlg::AddArcCollectorFps( const char* msg )
{
	m_arcColFps = msg;
}

void CPantographMonitorDlg::AddGeoUpFps( const char* msg )
{
	m_geoUpFps = msg;
}

void CPantographMonitorDlg::AddGeoDownLFps( const char* msg )
{
	m_geoDownLFps = msg;
}

void CPantographMonitorDlg::AddGeoDownRFps( const char* msg )
{
	m_geoDownRFps = msg;
}

void CPantographMonitorDlg::Data2Ui()
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

void CPantographMonitorDlg::AddShowMsg( std::string msg )
{
	m_showMsgLock.Lock();
	unsigned int size = m_showMsg.size();
	if (size <= 1024)
	{
		m_showMsg.push_back(msg);
	}
	m_showMsgLock.Unlock();
}

std::string CPantographMonitorDlg::PickShowMsg()
{
	std::string msg = "";
	m_showMsgLock.Lock();
	unsigned int size = m_showMsg.size();
	if (size > 0)
	{
		msg = m_showMsg[0];
		m_showMsg.pop_front();
	}
	m_showMsgLock.Unlock();
	return msg;
}






