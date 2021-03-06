
// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"


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


// CTestDlg 对话框




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


// CTestDlg 消息处理程序

BOOL CTestDlg::OnInitDialog()
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
	//int ret = 0;
	char m_pBuff[1024 * 16] = "";
	
	int tmpLen = 0;//消息体起始位置
	tmpLen += sprintf(m_pBuff + tmpLen, "<resp>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "<time>2018-12-9 15:12:45</time>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	<name>%s</name>\r\n","na me");
	tmpLen += sprintf(m_pBuff + tmpLen, "	<meter id=\"1\" id2=\"12\" id3=\"13\">\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	<function id=\"1\" coding=\"cod11\" error=\"err11\">数据11</function>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	<function id=\"2\" coding=\"cod12\" error=\"err12\">数据12</function>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	</meter>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	<meter id=\"2\" id2=\"22\" id3=\"23\">\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	<function id=\"1\" coding=\"cod21\" error=\"err21\">数据21</function>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	<function id=\"2\" coding=\"cod22\" error=\"err22\">数据22</function>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "	</meter>\r\n");
	tmpLen += sprintf(m_pBuff + tmpLen, "</resp>\r\n");
	test(m_pBuff);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestDlg::OnPaint()
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


static int ParseTimeNode(NSP_STP_CM::MXml::XMLNODE* base_root)
{
	int rtn = 0;
	NSP_STP_CM::MXml::XMLNODE* node = NULL;
	node = NSP_STP_CM::MXml::GetInstance()->f_xmlnode_sch_elem(base_root, "time");
	if (node != NULL)
	{
		//2014-02-12 15:30:32
		int yy = 0;
		int mm = 0;
		int dd = 0;
		int hh = 0;
		int mi = 0;
		int ss = 0;

		sscanf(node->data,"%d-%d-%d %d:%d:%d",&yy,&mm,&dd,&hh,&mi,&ss);
		yy = yy;
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
	MS_INT32 rtn = 0;
	NSP_STP_CM::MXml::XMLNODE *base_root = NULL;
	NSP_STP_CM::MXml::XML xml;
	memset(&xml,0,sizeof(xml));

	rtn = NSP_STP_CM::MXml::GetInstance()->f_xmlparse_init(&xml, pBuff);
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
	

	base_root = xml.root;

	NSP_STP_CM::MXml::XMLNODE *time_root = NULL;
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

