
// MakeLisenceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MakeLisence.h"
#include "MakeLisenceDlg.h"
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


// CMakeLisenceDlg 对话框




CMakeLisenceDlg::CMakeLisenceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMakeLisenceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakeLisenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMakeLisenceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMakeLisenceDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMakeLisenceDlg 消息处理程序

BOOL CMakeLisenceDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMakeLisenceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMakeLisenceDlg::OnPaint()
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
HCURSOR CMakeLisenceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static CString BootOpenFolder()
{
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(bi));
	//bi.hwndOwner = m_hWnd;   
	//bi.pidlRoot = NULL;   
	//bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择保存历史数据的目录：";   
	//bi.ulFlags = 0;   
	//bi.lpfn = NULL;   
	//bi.lParam = 0;   
	//bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		str.Format("%s",  szPath);
	}
	else
	{
		str.Format("");
	}

	return str;
}

enum   
{  
	DiskSize_B = 0,  
	DiskSize_K = 1,  
	DiskSize_M = 2,  
	DiskSize_G = 3,  
	DiskSize_T = 4,  
};  

enum   
{  
	GDI_VOLUMENAME = 0,  
	GDI_VOLUMESERIALNUMBER = 1,  
	GDI_VOLUMEFILESYSTEM = 2,  
	GDI_VOLUMETYPE = 3,  
	GDI_VOLUMESIZE = 4,  
	GDI_VOLUMEFREESIZE = 5,  
};  

static CString GetDriverInfomation(CString strRootPathName)  
{  
	CString strResult = _T("");  

	/**********获取驱动器名字、序列号和文件系统部分**********/  
	LPCTSTR lpRootPathName = strRootPathName;  
	LPTSTR lpVolumeNameBuffer = (LPTSTR) new wchar_t[_MAX_FNAME];  
	DWORD nVolumeNameSize = _MAX_FNAME;  
	DWORD nVolumeSerialNumber = 0;//便于驱动器无效时做判断  
	DWORD nMaximumComponentLength;  
	DWORD nFileSystemFlags;  
	DWORD nFileSystemNameSize = 20;  
	LPTSTR lpFileSystemNameBuffer = (LPTSTR) new wchar_t[nFileSystemNameSize];//文件系统(NTFS,FAT)多大有定义好的宏吗  
	GetVolumeInformation(  
		lpRootPathName,  
		lpVolumeNameBuffer,  
		nVolumeNameSize,  
		&nVolumeSerialNumber,   
		&nMaximumComponentLength,  
		&nFileSystemFlags,  
		lpFileSystemNameBuffer,  
		nFileSystemNameSize);  

	strResult.Format(_T("%X"),nVolumeSerialNumber);  

	delete[] lpVolumeNameBuffer;
	delete[] lpFileSystemNameBuffer;

	/**********返回所要求的驱动器的信息**********/  

	return strResult;  
}  


void CMakeLisenceDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret = 0;
	char buf[32];
	memset(buf, 0, sizeof(buf));
	char cmd[128];
	memset(cmd, 0, sizeof(cmd));

	CString folder = BootOpenFolder();
	CString str_xl = GetDriverInfomation(folder);  

	strcpy(buf, str_xl.GetString());
	int len = str_xl.GetLength();
	encry_lisence( (unsigned char *)buf,  len );


	FILE *pf=NULL;
	CString file_path = folder + L"lisence.ini";
	//SetFileAttributes(file_path,FILE_ATTRIBUTE_NORMAL); 
	sprintf(cmd, "attrib -a -s -h -r %s", file_path.GetString());
	system(cmd);

	pf = fopen(file_path.GetString(), "wb");
	if(pf)
	{
		fwrite(buf, sizeof(char), len, pf);
		fclose(pf);	

		//set unvisual
		//SetFileAttributes(file_path,FILE_ATTRIBUTE_HIDDEN); 


		sprintf(cmd, "attrib +s +a +h +r %s", file_path.GetString());
		system(cmd);

		AfxMessageBox("生成成功");  
	}
	else
	{
		AfxMessageBox("生成失败");  
	}	

	return;
}

