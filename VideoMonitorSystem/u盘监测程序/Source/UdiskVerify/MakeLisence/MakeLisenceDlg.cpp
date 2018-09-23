
// MakeLisenceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MakeLisence.h"
#include "MakeLisenceDlg.h"
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


// CMakeLisenceDlg �Ի���




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


// CMakeLisenceDlg ��Ϣ�������

BOOL CMakeLisenceDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMakeLisenceDlg::OnPaint()
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
HCURSOR CMakeLisenceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static CString BootOpenFolder()
{
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(bi));
	//bi.hwndOwner = m_hWnd;   
	//bi.pidlRoot = NULL;   
	//bi.pszDisplayName = szPath;   
	bi.lpszTitle = "��ѡ�񱣴���ʷ���ݵ�Ŀ¼��";   
	//bi.ulFlags = 0;   
	//bi.lpfn = NULL;   
	//bi.lParam = 0;   
	//bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
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

	/**********��ȡ���������֡����кź��ļ�ϵͳ����**********/  
	LPCTSTR lpRootPathName = strRootPathName;  
	LPTSTR lpVolumeNameBuffer = (LPTSTR) new wchar_t[_MAX_FNAME];  
	DWORD nVolumeNameSize = _MAX_FNAME;  
	DWORD nVolumeSerialNumber = 0;//������������Чʱ���ж�  
	DWORD nMaximumComponentLength;  
	DWORD nFileSystemFlags;  
	DWORD nFileSystemNameSize = 20;  
	LPTSTR lpFileSystemNameBuffer = (LPTSTR) new wchar_t[nFileSystemNameSize];//�ļ�ϵͳ(NTFS,FAT)����ж���õĺ���  
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

	/**********������Ҫ�������������Ϣ**********/  

	return strResult;  
}  


void CMakeLisenceDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

		AfxMessageBox("���ɳɹ�");  
	}
	else
	{
		AfxMessageBox("����ʧ��");  
	}	

	return;
}

