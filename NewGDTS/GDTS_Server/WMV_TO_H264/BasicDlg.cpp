// BasicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "BasicDlg.h"


// CBasicDlg 对话框

IMPLEMENT_DYNAMIC(CBasicDlg, CDialog)

CBasicDlg::CBasicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBasicDlg::IDD, pParent)
	, m_deviceIp(_T(""))
	, m_devicePort(_T(""))
	, m_deviceUser(_T(""))
	, m_devicePasswd(_T(""))
	, m_deviceId(_T(""))
	, m_serverIp(_T(""))
	, m_serverPort(_T(""))
	, m_pubName(_T(""))
{

}

CBasicDlg::~CBasicDlg()
{
}

void CBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_company);
	DDX_Text(pDX, IDC_DEV_IP, m_deviceIp);
	DDX_Text(pDX, IDC_DEVICE_PORT, m_devicePort);
	DDX_Text(pDX, IDC_DEVICE_USER, m_deviceUser);
	DDX_Text(pDX, IDC_DEVICE_PASSWD, m_devicePasswd);
	DDX_Text(pDX, IDC_DEVICE_ID, m_deviceId);
	DDX_Text(pDX, IDC_SERVER_IP, m_serverIp);
	DDX_Text(pDX, IDC_SERVIER_PORT, m_serverPort);
	DDX_Text(pDX, IDC_PUBNAME, m_pubName);
}


BEGIN_MESSAGE_MAP(CBasicDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CBasicDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CBasicDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBasicDlg 消息处理程序

void CBasicDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CBasicDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	m_company.InsertString(0,"东方网力");
	m_company.InsertString(1,"长天智远");

	m_company.SetCurSel(0);

	m_deviceIp.Format("192.168.0.166");
	m_devicePort.Format("4444");
	m_deviceUser.Format("admin");
	m_devicePasswd.Format("admin");
	m_deviceId.Format("aa");

	m_serverIp.Format("192.168.0.161");
	m_serverPort.Format("1935");
	m_pubName.Format("live");

	UpdateData(FALSE);

	return TRUE;
}

extern HMODULE   GetCurrentModule();

char notes[]=";目前Section的排列，必须是system放在第一个，放在第二个的是当前该设备采用的协议厂商。";

void CBasicDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData();

	CString thisDir,comFlag;
	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	thisDir+="\\config_test.ini";

	char tmpCompany[16];

	m_company.GetWindowTextA(tmpCompany,16);
	if(0 == strcmp(tmpCompany,"东方网力"))
	{
		comFlag = "np";		
	}
	else if(0 == strcmp(tmpCompany,"长天智远"))
		comFlag = "vnmp";
	else
		comFlag = "np";	


	if(0 ==strcmp(comFlag,"np") || 0 ==strcmp(comFlag,"vnmp"))
	{
	FILE *pf= fopen( (LPSTR)(LPCTSTR)thisDir ,"wb");
	fwrite(notes,1,strlen(notes),pf);
	fclose(pf);

	::WritePrivateProfileString("system","RtmpPort",m_serverPort,thisDir);
	::WritePrivateProfileString("system","MaxLiveNum","1",thisDir);

	//m_deviceIp.se
	::WritePrivateProfileString(comFlag,"ProxyIp0",m_deviceIp,thisDir);
	::WritePrivateProfileString(comFlag,"ProxyPort0",m_devicePort,thisDir);
	::WritePrivateProfileString(comFlag,"UserName0",m_deviceUser,thisDir);
	::WritePrivateProfileString(comFlag,"PassWd0",m_devicePasswd,thisDir);
	::WritePrivateProfileString(comFlag,"DeviceName0",m_deviceId,thisDir);
	::WritePrivateProfileString(comFlag,"Dstip0",m_serverIp,thisDir);
	::WritePrivateProfileString(comFlag,"Dstpub0",m_pubName,thisDir);
	::WritePrivateProfileString(comFlag,"Dststream0","0",thisDir);
	}

	OnOK();
}
