
// UdiskVerifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UdiskVerify.h"
#include "UdiskVerifyDlg.h"
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


// CUdiskVerifyDlg �Ի���




CUdiskVerifyDlg::CUdiskVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUdiskVerifyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUdiskVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUdiskVerifyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUdiskVerifyDlg ��Ϣ�������

BOOL CUdiskVerifyDlg::OnInitDialog()
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

void CUdiskVerifyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUdiskVerifyDlg::OnPaint()
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
HCURSOR CUdiskVerifyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////
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

static CString GetDiskSize( ULONGLONG nSize,int nType )  
{  
	CString tmp;  
	if (nType == DiskSize_B)  
	{  
		ULONGLONG nG ,nM,nK,nB,nKB,nMKB,nT;  
		nT =0;  
		//nT = nSize/(ULONGLONG)(1024*1024*1024*1024);  
		//nGMKB =  nSize%(ULONGLONG)(1024*1024*1024*1024);  
		nG = nSize/(ULONGLONG)(1000*1000*1000);  
		nMKB =  nSize%(ULONGLONG)(1000*1000*1000);  
		nM = nMKB/(ULONGLONG)(1000*1000);  
		nKB =nMKB%(ULONGLONG)(1000*1000);  
		nK = nKB/(ULONGLONG)(1000);  
		nB =nKB%(ULONGLONG)(1000);  

		if ((int)nT != 0)  
		{  
			tmp.Format("%d,%03d,%03d,%03d,%03d B",(int)nT, (int)nG,(int)nM,(int)nK,(int)nB );  
		}  
		else if ((int)nT == 0 &&(int)nG != 0 )  
		{  
			tmp.Format("%d,%03d,%03d,%03d B", (int)nG,(int)nM,(int)nK,(int)nB  );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM != 0 )  
		{  
			tmp.Format("%d,%03d,%03d B",(int)nM,(int)nK,(int)nB    );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM == 0 &&(int)nK != 0 )  
		{  
			tmp.Format("%d,%03d B",(int)nK,(int)nB );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM == 0 &&(int)nK == 0  &&(int)nB != 0 )  
		{  
			tmp.Format("%d B",(int)nB  );  
		}  
		else  
		{  
			tmp.Format("0 B");  
		}  
	}  
	else if (nType == DiskSize_K)  
	{  
		ULONGLONG nG ,nM,nK,nB,nKB,nMKB,nT;  
		nT =0;  
		//nT = nSize/(ULONGLONG)(1024*1024*1024*1024);  
		//nGMKB =  nSize%(ULONGLONG)(1024*1024*1024*1024);  
		nG = nSize/(ULONGLONG)(1000*1000*1024);  
		nMKB =  nSize%(ULONGLONG)(1000*1000*1024);  
		nM = nMKB/(ULONGLONG)(1000*1024);  
		nKB =nMKB%(ULONGLONG)(1000*1024);  
		nK = nKB/(ULONGLONG)(1024);  
		nB =nKB%(ULONGLONG)(1024);  

		if ((int)nT != 0)  
		{  
			tmp.Format("%d,%03d,%03d,%03d.%02d K",(int)nT, (int)nG,(int)nM,(int)nK,(int)(((float)(int)nB)/10.24)   );  
		}  
		else if ((int)nT == 0 &&(int)nG != 0 )  
		{  
			tmp.Format("%d,%03d,%03d.%02d K", (int)nG,(int)nM,(int)nK,(int)(((float)(int)nB)/10.24)    );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM != 0 )  
		{  
			tmp.Format("%d,%03d.%02d K",(int)nM,(int)nK,(int)(((float)(int)nB)/10.24)  );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM == 0 &&(int)nK != 0 )  
		{  
			tmp.Format("%d.%02d K",(int)nK,(int)(((float)(int)nB)/10.24)   );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM == 0 &&(int)nK == 0  &&(int)nB != 0 )  
		{  
			tmp.Format("0.%02d K",(int)(((float)(int)nB)/10.24)    );  
		}  
		else  
		{  
			tmp.Format("0.0 K");  
		}  
	}  
	else if (nType == DiskSize_M)  
	{  
		ULONGLONG nG ,nM,nK,nB,nKB,nMKB,nT;  
		nT =0;  
		//nT = nSize/(ULONGLONG)(1024*1024*1024*1024);  
		//nGMKB =  nSize%(ULONGLONG)(1024*1024*1024*1024);  
		nG = nSize/(ULONGLONG)(1000*1024*1024);  
		nMKB =  nSize%(ULONGLONG)(1000*1024*1024);  
		nM = nMKB/(ULONGLONG)(1024*1024);  
		nKB =nMKB%(ULONGLONG)(1024*1024);  
		nK = nKB/(ULONGLONG)(1024);  
		nB =nKB%(ULONGLONG)(1024);  

		if ((int)nT != 0)  
		{  
			tmp.Format("%d,%03d,%03d.%02d M",(int)nT, (int)nG,(int)nM,(int)(((float)(int)nK)/10.24)    );  
		}  
		else if ((int)nT == 0 &&(int)nG != 0 )  
		{  
			tmp.Format("%d,%03d.%02d M", (int)nG,(int)nM,(int)(((float)(int)nK)/10.24)     );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM != 0 )  
		{  
			tmp.Format("%d.%02d M",(int)nM,(int)(((float)(int)nK)/10.24)   );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM == 0 &&(int)nK != 0 )  
		{  
			tmp.Format("0.%02d M",(int)(((float)(int)nK)/10.24)    );  
		}  
		else  
		{  
			tmp.Format("0.0 K");  
		}  
	}  
	else if (nType == DiskSize_G)  
	{  
		ULONGLONG nG ,nM,nK,nB,nKB,nMKB,nT;  
		nT =0;  
		//nT = nSize/(ULONGLONG)(1024*1024*1024*1024);  
		//nGMKB =  nSize%(ULONGLONG)(1024*1024*1024*1024);  
		nG = nSize/(ULONGLONG)(1024*1024*1024);  
		nMKB =  nSize%(ULONGLONG)(1024*1024*1024);  
		nM = nMKB/(ULONGLONG)(1024*1024);  
		nKB =nMKB%(ULONGLONG)(1024*1024);  
		nK = nKB/(ULONGLONG)(1024);  
		nB =nKB%(ULONGLONG)(1024);  

		if ((int)nT != 0)  
		{  
			tmp.Format("%d,%03d.%02d G",(int)nT, (int)nG,(int)(((float)(int)nM)/10.24));  
		}  
		else if ((int)nT == 0 &&(int)nG != 0 )  
		{  
			tmp.Format("%d.%02d G", (int)nG,(int)(((float)(int)nM)/10.24)  );  
		}  
		else if ((int)nT == 0 &&(int)nG == 0  &&(int)nM != 0 )  
		{  
			tmp.Format("0.%02d G", (int)(((float)(int)nM)/10.24)   );  
		}  
		else  
		{  
			tmp.Format("0.0 G");  
		}  
	}  
	else if (nType == DiskSize_T)  
	{  
		ULONGLONG nG ,nM,nK,nB,nKB,nMKB,nT;  
		nT =0;  
		//nT = nSize/(ULONGLONG)(1024*1024*1024*1024);  
		//nGMKB =  nSize%(ULONGLONG)(1024*1024*1024*1024);  
		nG = nSize/(ULONGLONG)(1024*1024*1024);  
		nMKB =  nSize%(ULONGLONG)(1024*1024*1024);  
		nM = nMKB/(ULONGLONG)(1024*1024);  
		nKB =nMKB%(ULONGLONG)(1024*1024);  
		nK = nKB/(ULONGLONG)(1024);  
		nB =nKB%(ULONGLONG)(1024);  

		if ((int)nT != 0)  
		{  
			tmp.Format("%d.%03d T",(int)nT, (int)nG);  
		}  
		else if ((int)nT == 0 &&(int)nG != 0 )  
		{  
			tmp.Format("0.%03d G", (int)nG );  
		}  
		else  
		{  
			tmp.Format("0.0 G");  
		}  
	}  
	return tmp;  
}  

static CString GetDriverInfomation( BYTE bAlpha,int iGTI_TYPE )  
{  
	CString strResult = _T("");  

	if(!::IsCharAlpha((TCHAR)bAlpha))  
	{  
		strResult = _T("������������Ч!");  
		return strResult;  
	}  
	else  
	{  
		/**********��ȡ���������֡����кź��ļ�ϵͳ����**********/  
		CString strRootPathName;  
		strRootPathName.Format(_T("%c:\\"),bAlpha);  
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

		/**********��ȡ���������Ͳ���**********/  
		CString strDriveType;  

		/**********��ȡ�������ܴ�С��ʣ���С����**********/  
		//LPCTSTR lpDirectoryName = (LPTSTR) new wchar_t[2];  
		//lpDirectoryName = (LPCTSTR)strRootPathName.Mid(0,2);  
		_ULARGE_INTEGER FreeBytesAvailable,TotalNumberOfBytes,TotalNumberOfFreeBytes ;  

		__int64 iVolumeSize = 0,iVolumeFreeSize = 0;  

		GetDiskFreeSpaceEx(strRootPathName.Mid(0,2),&FreeBytesAvailable,&TotalNumberOfBytes,&TotalNumberOfFreeBytes );  
		iVolumeSize = TotalNumberOfBytes.QuadPart / 1024 / 1024;  
		iVolumeFreeSize = FreeBytesAvailable.QuadPart / 1024 / 1024;  

		/**********���ݲ����ó���Ӧ����������Ϣ**********/  
		switch(iGTI_TYPE)  
		{  
		case GDI_VOLUMENAME:  
			strResult.Format(_T("������ %c ������Ϊ:%s."),bAlpha,lpVolumeNameBuffer);  
			break;  
		case GDI_VOLUMESERIALNUMBER:  
			strResult.Format(_T("%X"),nVolumeSerialNumber);  
			break;  
		case GDI_VOLUMEFILESYSTEM:  
			strResult.Format(_T("������ %c ���ļ�ϵͳΪ:%s."),bAlpha,lpFileSystemNameBuffer);  
			break;  
		case GDI_VOLUMESIZE:  
			strResult.Format(_T("������ %c ���ܴ�СΪ:%.2fGB."),bAlpha,(float)iVolumeSize/1024);  
			break;  
		case GDI_VOLUMEFREESIZE:  
			strResult.Format(_T("������ %c ��ʣ���СΪ:%.2fGB."),bAlpha,(float)iVolumeFreeSize/1024);  
			break;  
		case GDI_VOLUMETYPE:  
			switch(GetDriveType(lpRootPathName))  
			{  
			case DRIVE_UNKNOWN:  
				strDriveType = _T("Unknown Type!");  
				break;  
			case DRIVE_NO_ROOT_DIR:  
				strResult = _T("��ȡ����������ʱ�������ô���!");  
				return strResult;  
			case DRIVE_REMOVABLE:  
				strDriveType = _T("Removable Disk");  
				break;  
			case DRIVE_FIXED:  
				strDriveType = _T("Hard Disk");  
				break;  
			case DRIVE_REMOTE:  
				strDriveType = _T("Remote Device");  
				break;  
			case DRIVE_CDROM:  
				strDriveType = _T("CD");  
				break;  
			case DRIVE_RAMDISK:  
				strDriveType = _T("RAM");  
				break;  
			default:  
				strResult = _T("Unknown Mistake!");  
				return strResult;  
			}  
			strResult.Format(_T("Driver %c Of Type:%s."),bAlpha,strDriveType);  
			break;  
		default:  
			strResult = _T("��ȡ��������Ϣʱ�������ô���!");  
			break;  
		}  

		delete[] lpVolumeNameBuffer;
		delete[] lpFileSystemNameBuffer;

	}  

	/**********������Ҫ�������������Ϣ**********/  

	return strResult;  
}  

static TCHAR FirstDriveFromMask(ULONG unitmask)    
{    
	char i;    
	for (i = 0; i < 26; ++i)    
	{    
		if (unitmask & 0x1)//������������״̬�Ƿ����˱仯    
			break;    
		unitmask = unitmask >> 1;    
	}    
	return (i + 'A');    
}   


static int UninstallUsb(CString discId)
{
	DWORD accessMode = GENERIC_WRITE | GENERIC_READ;
	DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	HANDLE hDevice;
	long	bResult = 0;
	DWORD retu = 0;
	DWORD dwBytesReturned;
	char szDriv[10];
	if(discId == ""){
		return 0;
	}
	sprintf(szDriv,"\\\\.\\%s:",discId);
	hDevice = CreateFile(szDriv,accessMode,shareMode,NULL,OPEN_EXISTING,0,NULL);
	if(hDevice == INVALID_HANDLE_VALUE){
		return -1;
	}

	dwBytesReturned = 0;
	PREVENT_MEDIA_REMOVAL PMRBuffer;
	PMRBuffer.PreventMediaRemoval = FALSE;
	if(!DeviceIoControl(hDevice,IOCTL_STORAGE_MEDIA_REMOVAL,&PMRBuffer,sizeof(PREVENT_MEDIA_REMOVAL),NULL,0,&dwBytesReturned,NULL)){
		printf("DeviceIoControl IOCTL_STORAGE_MEDIA_REMOVAL failed:%d\n",GetLastError());
	}
	bResult = DeviceIoControl(hDevice,IOCTL_STORAGE_EJECT_MEDIA,NULL,0,NULL,0,&retu,NULL);
	if(!bResult){
		CloseHandle(hDevice);
		return -1;
	}
	CloseHandle(hDevice);
	return 0;
}

LRESULT CUdiskVerifyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	switch (message)  
	{  
		//WM_DEVICECHANGE��ϵͳӲ���ı䷢����ϵͳ��Ϣ  
	case WM_DEVICECHANGE:  
		{  
			PDEV_BROADCAST_HDR lpdb=(PDEV_BROADCAST_HDR)lParam;  
			switch(wParam)  
			{  
			case WM_DEVICECHANGE:  
				break;  
			case DBT_DEVICEARRIVAL://DBT_DEVICEARRIVAL���豸�����������ҿ���ʹ��  
				{  
					if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//�߼���  
					{  
						PDEV_BROADCAST_VOLUME lpdbv =  (PDEV_BROADCAST_VOLUME)lpdb;  
						switch(lpdbv->dbcv_flags)  
						{  
						case 0:                //U��  
							{  
								CString decDriver;  
								decDriver = FirstDriveFromMask(lpdbv ->dbcv_unitmask);  
								DWORD sizeStruct = lpdbv->dbcv_size;  

								ULARGE_INTEGER FreeAv,TotalBytes,FreeBytes;  
								if (GetDiskFreeSpaceEx(decDriver +L":\\", &FreeAv,&TotalBytes,&FreeBytes))  
								{  
									CString strTotalBytes = GetDiskSize(TotalBytes.QuadPart,DiskSize_B);  
									CString strFreeBytes = GetDiskSize(FreeBytes.QuadPart,DiskSize_B);  
									CString strFreeAV = GetDiskSize(TotalBytes.QuadPart - FreeAv.QuadPart,DiskSize_B);  
									CString strTG = GetDiskSize(TotalBytes.QuadPart,DiskSize_G);  
									CString strFG = GetDiskSize(FreeBytes.QuadPart,DiskSize_G);  
									CString strYG = GetDiskSize(TotalBytes.QuadPart - FreeAv.QuadPart,DiskSize_G);  
									CString strname = GetDriverInfomation(decDriver.GetAt(0),GDI_VOLUMENAME);  
									CString strSeries = GetDriverInfomation(decDriver.GetAt(0),GDI_VOLUMESERIALNUMBER);  
									CString strfilsystem = GetDriverInfomation(decDriver.GetAt(0),GDI_VOLUMEFILESYSTEM);  
									CString strAllInfo;  
									strAllInfo.Format("������:[%s] \n������%s (%s)\n���ÿռ䣺%s (%s)\n���ÿռ�: %s (%s)\n%s\n%s\n%s\n",  
										decDriver +":\\",  strTotalBytes ,strTG,strFreeAV,strYG,strFreeBytes,strFG,strname,strSeries,strfilsystem  
										);/* ��λΪG */  
									TRACE(strAllInfo);

									verifyUdisk(strSeries, decDriver);

									//AfxMessageBox(strAllInfo);  
								}  
								/*CString tmp; 
								tmp.Format(L"��⵽U��:[%s]����! %ld bytes\n",decDriver.GetBuffer(0),sizeStruct); 
								AfxMessageBox(tmp);*/  
							}  
							break;  
						case DBTF_MEDIA:    //����  
							CString tmp;  
							tmp.Format("��⵽����:[%c]����!\n",FirstDriveFromMask(lpdbv ->dbcv_unitmask));  
							AfxMessageBox(tmp);  
							break;  
						}  
					}  
				}  
				break;  
			case DBT_DEVICEREMOVECOMPLETE://DBT_DEVICEREMOVECOMPLETE,�豸ж�ػ��߰γ�  
				{  
					if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)//�߼���  
					{  
						PDEV_BROADCAST_VOLUME lpdbv =  (PDEV_BROADCAST_VOLUME)lpdb;  
						switch(lpdbv->dbcv_flags)  
						{  
						case 0:                //U��  
							{  
								CString decDriver;  
								decDriver = FirstDriveFromMask(lpdbv ->dbcv_unitmask);  
								CString tmp;  
								tmp.Format("��⵽U��:[%s]�γ�!\n",decDriver.GetBuffer(0));  
								TRACE(tmp);
								//AfxMessageBox(tmp);  
							}  
							break;  
						case DBTF_MEDIA:    //����  

							break;  
						}  
					}  
				}  
				break;  
			}  
		}  
		break;  
	}  
	return CDialogEx::WindowProc(message, wParam, lParam);  
}


int CUdiskVerifyDlg::verifyUdisk( CString strSeries, CString decDriver )
{
	//check series
	int ret = 0;
	CString lisence = "";
	CString str_file = decDriver + L"://lisence.ini";
	const char* pfile = str_file.GetString();
	char* pBuff = new char[256];
	memset(pBuff, 0, 256);
	ret = LoadFile(pfile, pBuff);
	if (ret != 0)
	{
		UninstallUsb(decDriver);
		ret = -1;
		AfxMessageBox("u�������֤ʧ��,�ѵ���");  
		return ret;
	}

	lisence.Format("%s", pBuff);
	delete[] pBuff;

	if (0 == strcmp(strSeries.GetString() , lisence.GetString()) 
		&& 0 != strcmp(strSeries.GetString(), "0"))
	{
		ret = 0;
		TRACE("u�������֤�ɹ�");
	}
	else
	{
		UninstallUsb(decDriver);
		ret = -1;
		AfxMessageBox("u�������֤ʧ��,�ѵ���");  
	}

	return ret;

}

