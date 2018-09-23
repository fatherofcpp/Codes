
// KjControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KjControl.h"
#include "KjControlDlg.h"
#include "afxdialogex.h"
#include "FIFO.h"
#include <ATLTRACE.H>
#include "Cfg.h"

#include "utility.h"


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


// CKjControlDlg �Ի���




CKjControlDlg::CKjControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKjControlDlg::IDD, pParent), m_editbox(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKjControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CONTENTB, m_Edit);
}

BEGIN_MESSAGE_MAP(CKjControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, &CKjControlDlg::OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CKjControlDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CKjControlDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_CHECK_KS_GB_OC, &CKjControlDlg::OnBnClickedCheckKsGbOc)
	ON_BN_CLICKED(IDC_CHECK_JS_GB_OC, &CKjControlDlg::OnBnClickedCheckJsGbOc)
	ON_BN_CLICKED(IDC_CHECK_DPJ_PLS_OC, &CKjControlDlg::OnBnClickedCheckDpjPlsOc)
	ON_BN_CLICKED(IDC_BUTTON_CAR_PLS_CLR, &CKjControlDlg::OnBnClickedButtonCarPlsClr)
	ON_BN_CLICKED(IDC_BUTTON_PLC_PLS_CNT, &CKjControlDlg::OnBnClickedButtonPlcPlsCnt)
	ON_BN_CLICKED(IDC_BUTTON_DPJ_PLS_CNT, &CKjControlDlg::OnBnClickedButtonDpjPlsCnt)
	ON_BN_CLICKED(IDC_BUTTON_GET_CAR_POS, &CKjControlDlg::OnBnClickedButtonGetCarPos)
	ON_BN_CLICKED(IDC_BUTTON_CAR_STEP, &CKjControlDlg::OnBnClickedButtonCarStep)
	ON_BN_CLICKED(IDC_BUTTON_CAR_HOME, &CKjControlDlg::OnBnClickedButtonCarHome)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DEMO, &CKjControlDlg::OnBnClickedButtonDemo)
END_MESSAGE_MAP()


// CKjControlDlg ��Ϣ�������

BOOL CKjControlDlg::OnInitDialog()
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
	// ���������ļ�����ȡ�����ļ���Ϣ
	char szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strFileName = szPath;
	strFileName.Delete(strFileName.ReverseFind('\\')+1, strFileName.GetLength ()-strFileName.ReverseFind('\\')-1);
	strFileName += "config_auto.ini";
	Ccfg::GetInstance()->Load(strFileName.GetString());
	InitCtrl();

	//m_editbox = new CEditBoxInfoShow;
	m_editbox = make_shared<CEditBoxInfoShow>();
	m_editbox->SetParent(this);
	m_editbox->SetCtrlID(IDC_EDIT_CONTENTB);
	m_editbox->SetRelease(true);

	SetTimer(0,1000,NULL);
	SetTimer(1,1000,NULL);
	CFIFO::GetInstance()->Write("m_work Initialize ... ...\r\n");
	m_work.Initialize();
	m_work.SetRelease(true);

	CFIFO::GetInstance()->Write("KjControl Initialize ok\r\n");
	CFIFO::GetInstance()->Write("Ӧ�ó���ȴ�����ִ�� ... ... \r\n");
	CFIFO::GetInstance()->Write(strFileName.GetString()+ string("\r\n"));



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CKjControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKjControlDlg::OnPaint()
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
HCURSOR CKjControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKjControlDlg::OnBnClickedButtonSelectAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(auto i = IDC_CHECK1; i <= IDC_CHECK16; i++)
	{
		((CButton*)GetDlgItem(i))->SetCheck(1);
	}
}

void CKjControlDlg::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(auto i = IDC_CHECK1; i <= IDC_CHECK16; i++)
	{
		((CButton*)GetDlgItem(i))->SetCheck(0);
	}
}

void CKjControlDlg::OnBnClickedButtonApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	memset(m_work.zxjRun, 0, 16*sizeof(int));
	for(auto i = IDC_CHECK1; i<= IDC_CHECK16; ++i)
	{
		if(((CButton*)GetDlgItem(i))->GetCheck())
			m_work.zxjRun[i-IDC_CHECK1] = 1;
		else
			m_work.zxjRun[i-IDC_CHECK1] = 0;
	}
	OutputDebugString("Apply finished!\r\n");
}

void CKjControlDlg::OnBnClickedCheckKsGbOc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Sleep(100);
	if(((CButton*)GetDlgItem(IDC_CHECK_KS_GB_OC))->GetCheck())
		m_work.KsLightOC(true);
	else
		m_work.KsLightOC(false);
}

void CKjControlDlg::OnBnClickedCheckJsGbOc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Sleep(100);
	if(((CButton*)GetDlgItem(IDC_CHECK_JS_GB_OC))->GetCheck())
		m_work.JsLightOC(true);
	else
		m_work.JsLightOC(false);
}

void CKjControlDlg::OnBnClickedCheckDpjPlsOc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Sleep(500);
	string cmd1 = "{jiecheks}";
	string cmd2 = "{jiechejs}";
	if(((CButton*)GetDlgItem(IDC_CHECK_DPJ_PLS_OC))->GetCheck())
	{
		///m_work.GetDpjPulseCmd()->WriteToPort(const_cast<char*>(cmd1.c_str()), cmd1.size());
	}
	else
	{
		///m_work.GetDpjPulseCmd()->WriteToPort(const_cast<char*>(cmd2.c_str()), cmd2.size());
	}
}

//С�������������
void CKjControlDlg::OnBnClickedButtonCarPlsClr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string str;
	DWORD ret;
	
	str = "{c,clr}";
	do
	{
		m_work.GetPLCSock()->Send(const_cast<char*>(str.c_str()), str.size());
		ret = m_work.EventWait(m_work.GetPLCResponseOK(), 300);
		CFIFO::GetInstance()->Write(str+"\r\n");
		Sleep(20);
	}while(ret != WAIT_OBJECT_0);
	
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
}

//PLC�������
void CKjControlDlg::OnBnClickedButtonPlcPlsCnt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string str;
	DWORD ret;
	
	str = "{c,pci}";
	do
	{
		m_work.GetPLCSock()->Send(const_cast<char*>(str.c_str()), str.size());
		ret = m_work.EventWait(m_work.GetPLCResponseOK(), 300);
		CFIFO::GetInstance()->Write(str+"\r\n");
		Sleep(20);
	}while(ret != WAIT_OBJECT_0);
	
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");

	GetDlgItem(IDC_EDIT_PLC_PLS)->SetWindowTextA("");
}

//��Ƭ���������
void CKjControlDlg::OnBnClickedButtonDpjPlsCnt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	///long v = m_work.GetCurrPulseVaule();
	/*CString str;
	str.Format("%ld", v);
	GetDlgItem(IDC_EDIT_DPJ_PLS)->SetWindowTextA(str);*/
}

//��ȡС����ǰ����
void CKjControlDlg::OnBnClickedButtonGetCarPos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string str;
	DWORD ret;
	
	str = "{c,dis}";
	do
	{
		m_work.GetPLCSock()->Send(const_cast<char*>(str.c_str()), str.size());
		ret = m_work.EventWait(m_work.GetPLCResponseOK(), 300);
		CFIFO::GetInstance()->Write(str+"\r\n");
		Sleep(20);
	}while(ret != WAIT_OBJECT_0);
	
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
}


void CKjControlDlg::OnBnClickedButtonCarStep()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string str;
	DWORD ret;
	CString cstr;
	GetDlgItem(IDC_EDIT_CAR_STEP)->GetWindowTextA(cstr);
	long v = atoi(cstr.GetString());
	cstr.Format("{c,step,%ld,100}", v);
	str = cstr;
	do
	{
		m_work.GetPLCSock()->Send(const_cast<char*>(str.c_str()), str.size());
		ret = m_work.EventWait(m_work.GetPLCResponseRV(), 300);
		CFIFO::GetInstance()->Write(str+"\r\n");
		Sleep(20);
	}while(ret != WAIT_OBJECT_0);
	
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
	m_work.EventWait(m_work.GetPLCResponseOK(), INFINITE);
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
}


void CKjControlDlg::OnBnClickedButtonCarHome()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string str;
	DWORD ret;
	
	str = "{c,home}";
	do
	{
		m_work.GetPLCSock()->Send(const_cast<char*>(str.c_str()), str.size());
		ret = m_work.EventWait(m_work.GetPLCResponseRV(), 300);
		CFIFO::GetInstance()->Write(str+"\r\n");
		Sleep(20);
	}while(ret != WAIT_OBJECT_0);
	
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
	m_work.EventWait(m_work.GetPLCResponseOK(), INFINITE);
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
}

HBRUSH CKjControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	CString str;
	int id;
	switch(nCtlColor)
	{
	case CTLCOLOR_MSGBOX:
	case CTLCOLOR_EDIT:
		id = pWnd->GetDlgCtrlID();
		/*str.Format("CtrlID = %d\r\n", id);
		TRACE(str.GetString());*/
		if(id==IDC_EDIT_CONTENTB)
		{
			HBRUSH hbrEdit = ::CreateSolidBrush(RGB(0,0,0));
			pDC->SetBkColor(RGB(0,0,0));
			pDC->SetTextColor(RGB(255,255,255));
			hbr =  hbrEdit;
		}
		break;
	case CTLCOLOR_STATIC:
		break;
	}
	return hbr;
}


void CKjControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (0 == nIDEvent)
	{
		int sel = m_Edit.GetSel();
		if(sel >= 1966110000/2)
		{
			m_Edit.SetWindowTextA("");
			m_Edit.SetSel(0, -1);
		}
	}
	if (1 == nIDEvent)
	{
		//��ʾ����ֵ
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CKjControlDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_work.SetRunning(false);
	m_editbox->SetRunning(false);
}

void CKjControlDlg::InitCtrl()
{
	CString cstr; string str; int iv,iw;
	//IDC_STC1
	str = Ccfg::GetInstance()->GetPlcIP();
	cstr.Format("%s", str.c_str());
	GetDlgItem(IDC_STC1)->SetWindowTextA(cstr);
	//IDC_STC2
	iv = Ccfg::GetInstance()->GetPlcPort();
	cstr.Format("%d", iv);
	GetDlgItem(IDC_STC2)->SetWindowTextA(cstr);
	//IDC_STC4
	str = Ccfg::GetInstance()->GetLinecameraIP();
	cstr.Format("%s", str.c_str());
	GetDlgItem(IDC_STC4)->SetWindowTextA(cstr);
	//IDC_STC5
	iv = Ccfg::GetInstance()->GetLinecameraPort();
	cstr.Format("%d", iv);
	GetDlgItem(IDC_STC5)->SetWindowTextA(cstr);
	//IDC_STC6
	iv = Ccfg::GetInstance()->GetAreacameraPort();
	cstr.Format("%d", iv);
	GetDlgItem(IDC_STC6)->SetWindowTextA(cstr);
	//IDC_STC9
	iv = Ccfg::GetInstance()->GetDpjCmdBD();
	str = Ccfg::GetInstance()->GetDpjCmdCom();
	cstr.Format("%s, %d", str.c_str(), iv);
	GetDlgItem(IDC_STC9)->SetWindowTextA(cstr);
	//IDC_STC10
	iv = Ccfg::GetInstance()->GetDpjDataBD();
	str = Ccfg::GetInstance()->GetDpjDataCom();
	cstr.Format("%s, %d", str.c_str(), iv);
	GetDlgItem(IDC_STC10)->SetWindowTextA(cstr);
	//IDC_STC11
	iv = Ccfg::GetInstance()->GetLDS1BD();
	str = Ccfg::GetInstance()->GetLDS1Com();
	cstr.Format("%s, %d", str.c_str(), iv);
	GetDlgItem(IDC_STC11)->SetWindowTextA(cstr);
	//IDC_STC12
	iv = Ccfg::GetInstance()->GetLDS2BD();
	str = Ccfg::GetInstance()->GetLDS2Com();
	cstr.Format("%s, %d", str.c_str(), iv);
	GetDlgItem(IDC_STC12)->SetWindowTextA(cstr);
	//IDC_STC13
	str = Ccfg::GetInstance()->GetServerIP();
	iv = Ccfg::GetInstance()->GetServerPort();
	cstr.Format("%s\t%d", str.c_str(), iv);
	GetDlgItem(IDC_STC13)->SetWindowTextA(cstr);
	//IDC_STC14
	iv = Ccfg::GetInstance()->GetServerDisPort();
	cstr.Format("%s\t%d", str.c_str(), iv);
	GetDlgItem(IDC_STC14)->SetWindowTextA(cstr);
	//IDC_STC15
	iv = Ccfg::GetInstance()->GetHandpadPort();
	cstr.Format("%d",iv);
	GetDlgItem(IDC_STC15)->SetWindowTextA(cstr);
}

void CKjControlDlg::OnBnClickedButtonDemo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(DemoAction, 0, this);
}

void CKjControlDlg::DemoAction(LPVOID param)
{
	CKjControlDlg* dlg = (CKjControlDlg*)param;
	dlg->m_work.STPosAction();
}