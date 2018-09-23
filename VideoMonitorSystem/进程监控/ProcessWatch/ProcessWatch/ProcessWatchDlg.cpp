
// ProcessWatchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessWatch.h"
#include "ProcessWatchDlg.h"
#include "afxdialogex.h"

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


// CProcessWatchDlg �Ի���




CProcessWatchDlg::CProcessWatchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProcessWatchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProcessWatchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CProcessWatchDlg ��Ϣ�������

BOOL CProcessWatchDlg::OnInitDialog()
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
	CString title = _T("���̿��� 2018-04-18 09:01");// + theApp.m_cmdParam;
	SetWindowText(title);

	int ret = Init();
	if (ret != 0)
	{
		gLog.Add("%s Init err exit\n", __FUNCTION__);
		exit(0);
	}
	TaskStart();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CProcessWatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProcessWatchDlg::OnPaint()
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
HCURSOR CProcessWatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

unsigned int WINAPI CProcessWatchDlg::thread_recv( void* pParam )
{
	USES_CONVERSION;
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CProcessWatchDlg* p_this = (CProcessWatchDlg*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	theApp.m_cmdParam;
	const int sleep_m = 20;
	const int no_data_delt_tm = p_this->m_StConf.disconnectSec * 1000;
	const int wait_sys_ms = p_this->m_StConf.waitSystemTs * 1000;

	NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
	p_CUdp->SetLocalPort(p_this->m_StConf.udpPortRecv);
	timeval r_time_out;
	r_time_out.tv_sec = 0;
	r_time_out.tv_usec = sleep_m * 1000;
	p_CUdp->SetReadTimeOut(r_time_out);
	int ret = p_CUdp->BindPort();
	if (ret != 0)
	{
		gLog.Add("%s p_CUdp->BindPort fail\n", __FUNCTION__);
		delete p_CUdp;
		return 0;
	}

	char rcv_buf[256];
	memset(rcv_buf, 0, sizeof(rcv_buf));

	int nodata_pre_ms = ::GetTickCount();

	//Sleep(wait_sys_ms);
	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_CThreadPool.GetThreadArr()[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		memset(rcv_buf, 0, sizeof(rcv_buf));
		int ret = p_CUdp->RecvFrmService(rcv_buf, sizeof(rcv_buf));
		int nodata_now_ms = ::GetTickCount();
		int delt_ms = abs(nodata_now_ms - nodata_pre_ms);
		if (ret > 0)
		{
			nodata_pre_ms = nodata_now_ms;
		}
		else
		{
			if (delt_ms > no_data_delt_tm)
			{
				//do
				if (p_this->m_StConf.isReboot)
				{
					gLog.Add("%s before reboot\n", __FUNCTION__);
					BOOL is_reboot = NSP_STP_CM::MySystemShutdown(true);
					gLog.Add("%s after reboot, is_reboot=%d\n", __FUNCTION__, is_reboot);
				}
				nodata_pre_ms = nodata_now_ms;
			}
		}


	}

	return 0;
}

int CProcessWatchDlg::TaskStart()
{
	USES_CONVERSION;

	m_CThreadPool.SetThreadCount(1);
	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_geoparam_recv", __FUNCTION__);
		thread_run(&m_CThreadPool.GetThreadArr()[thread_ind], thread_recv, p_st_thread_param, 0xFF);
	}
	return 0;
}

int CProcessWatchDlg::TaskStop()
{
	m_CThreadPool.ThreadsStop();
	return 0;
}



void CProcessWatchDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	TaskStop();
	CDialogEx::OnCancel();
}

int CProcessWatchDlg::LoadConfig()
{
	USES_CONVERSION;

	int ret = -1;

	const int SIZE = 1024;
	TCHAR Buffer[SIZE];
	memset(Buffer, 0, sizeof(Buffer));

	CString current_path = theApp.m_currentPath;
	CString strIniFile(current_path + _T("\\configProcessWatch.ini"));

	CString strSection;

	strSection.Format(_T("system"));

	m_StConf.waitSystemTs = GetPrivateProfileInt(strSection, _T("waitSystemTs"), -1, strIniFile);
	if (m_StConf.waitSystemTs <= 0)
	{
		goto LEAVE;
	}

	m_StConf.udpPortRecv = GetPrivateProfileInt(strSection, _T("udpPortRecv"), -1, strIniFile);
	if (m_StConf.udpPortRecv <= 0)
	{
		goto LEAVE;
	}

	m_StConf.disconnectSec = GetPrivateProfileInt(strSection, _T("disconnectSec"), -1, strIniFile);
	if (m_StConf.disconnectSec <= 0)
	{
		goto LEAVE;
	}

	m_StConf.isReboot = GetPrivateProfileInt(strSection, _T("isReboot"), 0, strIniFile);

	ret = 0;
LEAVE:
	return ret;
}

int CProcessWatchDlg::Init()
{
	USES_CONVERSION;
	//log init
	CString current_path = theApp.m_currentPath;
	current_path += "\\ProcessWatchLog.log";
	gLog.InitLog(W2A(current_path), "ProcessWatchLog");
	gLog.Add("%s ################Program Start################\n", __FUNCTION__);

	return LoadConfig();
}

BOOL CProcessWatchDlg::PreTranslateMessage( MSG* pMsg )
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
