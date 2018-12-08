
// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "CmFunction/CmFunction.h"


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


// CTestDlg �Ի���




CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CTestDlg ��Ϣ�������

BOOL CTestDlg::OnInitDialog()
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
	test("123");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestDlg::OnPaint()
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


static int ParseTimeNode(XMLNODE* base_root)
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "time");
	if (node != NULL)
	{
		//2014-02-12 15:30:32
		int yy = 0;
		int mm = 0;
		int dd = 0;
		int hh = 0;
		int mi = 0;
		int ss = 0;

		sscanf(node->data,"%d-%d-%d,%d:%d:%d",&yy,&mm,&dd,&hh,&mi,&ss);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
leave:
	return rtn;
}



MS_INT32 CTestDlg::test( MCS_CHAR* pBuff )
{
	{
		M_STRING str = "";
		str = NSP_STP_CM::str_match_head2sig_last("D:\\aa\\bb\\cc.jg", "\\", false );
		str = NSP_STP_CM::str_match_head2sig_last("D:\\aa\\bb\\cc.jg", "\\", true );
		str = NSP_STP_CM::str_match_tail2sig_last("D:\\aa\\bb\\cc.jg", "\\", false );
		str = NSP_STP_CM::str_match_tail2sig_last("D:\\aa\\bb\\cc.jg", "\\", true );
		str = NSP_STP_CM::str_match_head2sig_first("D:\\aa\\bb\\cc.jg", "\\", false );
		str = NSP_STP_CM::str_match_head2sig_first("D:\\aa\\bb\\cc.jg", "\\", true );
		
		return 0;
	}
	{
		//HE_SetUnhandledException(thread_exception, NULL, this, NULL, NULL, 0);
	}

	{
		CMString str2("200");
		CMString str5(str2);

		CMString str3 = str2;
		CMString str4 = "400";
		
		str3 = str2;
		str4 = "500";

		CMString str1;
		str1 = "100";

		return 0;
	}

	MS_INT32 rtn = 0;
	XMLNODE *base_root = NULL;
	//XMLNODE *node = NULL;
	XML xml;
	memset(&xml,0,sizeof(xml));

	rtn = f_xmlparse_init(&xml, pBuff);
	if (rtn < 0)
	{
		rtn = -1;
		goto leave;
	}

	if(xml.root ==NULL)
	{
		rtn = -1;
		goto leave;
	}	

	if(strcmp(xml.root->name,"save")!=0)
	{
		rtn = -1;
		goto leave;
	}	

	base_root = xml.root;

	XMLNODE *time_root = NULL;
	time_root = base_root;
	rtn = ParseTimeNode(time_root);
	if (rtn < 0)
	{
		goto leave;
	}

leave:
	return rtn;
}

void CALLBACK CTestDlg::thread_exception( void* /*pParam */)
{
	//CTestDlg* p_this = (CTestDlg*)pParam;
	TRACE("%s\n", __FUNCTION__);
	return;
}

