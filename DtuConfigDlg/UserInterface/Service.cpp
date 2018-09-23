// Service.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "Service.h"
#include "CommonFunction.h"

// CService 对话框

IMPLEMENT_DYNAMIC(CService, CDialogEx)

	CService::CService(CWnd* pParent /*=NULL*/)
	: CDialog(CService::IDD, pParent)
{
	pNetConf = new st_net_conf;
	memset(pNetConf, 0, sizeof(*pNetConf));

	pSerPort[0] = &mSerPort1;
	pSerPort[1] = &mSerPort2;
	pSerPort[2] = &mSerPort3;
	pSerPort[3] = &mSerPort4;

	pSerValid[0] = &mValidSer1;
	pSerValid[1] = &mValidSer2;
	pSerValid[2] = &mValidSer3;
	pSerValid[3] = &mValidSer4;

	pTestResult[0] = &mTest1;
	pTestResult[1] = &mTest2;
	pTestResult[2] = &mTest3;
	pTestResult[3] = &mTest4;

	pPingButton[0] = &mPinSer1;
	pPingButton[1] = &mPinSer2;
	pPingButton[2] = &mPinSer3;
	pPingButton[3] = &mPinSer4;

	pSerDomain[0] = &mDomainSvr1;
	pSerDomain[1] = &mDomainSvr2;
	pSerDomain[2] = &mDomainSvr3;
	pSerDomain[3] = &mDomainSvr4;

	mDomainSvr1.SetValidType(VALID_HTTP_ADDR);
	mDomainSvr2.SetValidType(VALID_HTTP_ADDR);
	mDomainSvr3.SetValidType(VALID_HTTP_ADDR);
	mDomainSvr4.SetValidType(VALID_HTTP_ADDR);

	mDomainSvr1.SetMaxLen(MAX_NAME_LEN);
	mDomainSvr2.SetMaxLen(MAX_NAME_LEN);
	mDomainSvr3.SetMaxLen(MAX_NAME_LEN);
	mDomainSvr4.SetMaxLen(MAX_NAME_LEN);


	mSerPort1.SetValidType(VALID_INT);
	mSerPort2.SetValidType(VALID_INT);
	mSerPort3.SetValidType(VALID_INT);
	mSerPort4.SetValidType(VALID_INT);

	mSerPort1.SetMaxNum(0xFFFF);
	mSerPort1.SetMinNum(1);
	mSerPort2.SetMaxNum(0xFFFF);
	mSerPort2.SetMinNum(1);
	mSerPort3.SetMaxNum(0xFFFF);
	mSerPort3.SetMinNum(1);
	mSerPort4.SetMaxNum(0xFFFF);
	mSerPort4.SetMinNum(1);

	ZeroMemory(svrValid,sizeof(svrValid));

	mMacAddrEth.SetValidType(VALID_ALL_TYPE);
	mMacAddrEth.SetMaxLen(MAX_NAME_LEN);

	mSsidHotspot.SetMaxLen(32);
	mPassPhraseHotSpot.SetValidType(VALID_HOTSPOT_PASS);
	mPassPhraseHotSpot.SetMaxLen(32);


	mServiceSecret.SetValidType(VALID_HEX);
	mServiceSecret.SetMaxLen(16);
	mServiceSecret.SetPermitNull(true);
}

CService::~CService()
{
	delete pNetConf;
}

void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SVR_PORT2, mSerPort2);
	DDX_Control(pDX, IDC_EDIT_SVR_PORT3, mSerPort3);
	DDX_Control(pDX, IDC_ETH_GR, mGroupEth);
	DDX_Control(pDX, IDC_STATIC_ETH_IP, mIpLableEth);
	DDX_Control(pDX, IDC_IPADDRESS_ETH_IP, mIpEth);
	DDX_Control(pDX, IDC_RADIO_ETH_STATIC, mStaticNetEth);
	DDX_Control(pDX, IDC_RADIO_ETH_DYNAMIC, mDynamicNetEth);
	DDX_Control(pDX, IDC_EDIT_SVR_PORT1, mSerPort1);
	DDX_Control(pDX, IDC_STATIC_SERVICE, mGroupService);
	DDX_Control(pDX, IDC_CHECK_SVR1, mValidSer1);
	DDX_Control(pDX, IDC_CHECK_SVR2, mValidSer2);
	DDX_Control(pDX, IDC_CHECK_SVR3, mValidSer3);
	DDX_Control(pDX, IDC_BUTTON_PING1, mPinSer1);
	DDX_Control(pDX, IDC_BUTTON_PING2, mPinSer2);
	DDX_Control(pDX, IDC_BUTTON_PING3, mPinSer3);
	DDX_Control(pDX, IDC_TEST_SV1, mTest1);
	DDX_Control(pDX, IDC_TEST_SV2, mTest2);
	DDX_Control(pDX, IDC_TEST_SV3, mTest3);
	DDX_Control(pDX, IDC_IPADDRESS_ETH_MASK, mMaskEth);
	DDX_Control(pDX, IDC_IPADDRESS_ETH_GATEWAY, mGateWayEth);
	DDX_Control(pDX, IDC_EDIT_ETH_MAC, mMacAddrEth);
	DDX_Control(pDX, IDC_IPADDRESS_ETH_DNS, mDnsEth);
	DDX_Control(pDX, IDC_CHECK_ETH_USE, mUsedEth);
	DDX_Control(pDX, IDC_EDIT_SSID_HOTSPOT, mSsidHotspot);
	DDX_Control(pDX, IDC_EDIT_PASSPHRASE_HOTSPOT, mPassPhraseHotSpot);
	DDX_Control(pDX, IDC_EDIT_DOMAIN_SVR1, mDomainSvr1);
	DDX_Control(pDX, IDC_EDIT_DOMAIN_SVR2, mDomainSvr2);
	DDX_Control(pDX, IDC_EDIT_DOMAIN_SVR3, mDomainSvr3);
	DDX_Control(pDX, IDC_EDIT_SECRET, mServiceSecret);
	DDX_Control(pDX, IDC_CHECK_SVR4, mValidSer4);
	DDX_Control(pDX, IDC_EDIT_DOMAIN_SVR4, mDomainSvr4);
	DDX_Control(pDX, IDC_EDIT_SVR_PORT4, mSerPort4);
	DDX_Control(pDX, IDC_BUTTON_PING4, mPinSer4);
	DDX_Control(pDX, IDC_TEST_SV4, mTest4);
	DDX_Control(pDX, IDC_BUTTON_AUTO_MAC, btAutoMac);
}


BEGIN_MESSAGE_MAP(CService, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ETH_DYNAMIC, &CService::OnBnClickedNetDynamicEth)
	ON_BN_CLICKED(IDC_RADIO_ETH_STATIC, &CService::OnBnClickedNetStaticEth)
	ON_BN_CLICKED(IDC_CHECK_SVR1, &CService::OnBnClickedSvr1Valid)
	ON_BN_CLICKED(IDC_CHECK_SVR2, &CService::OnBnClickedSvr2Valid)
	ON_BN_CLICKED(IDC_CHECK_SVR3, &CService::OnBnClickedSvr3Valid)
	ON_BN_CLICKED(IDC_BUTTON_PING1, &CService::OnBnClickedPinSer1)
	ON_BN_CLICKED(IDC_BUTTON_PING2, &CService::OnBnClickedPinSer2)
	ON_BN_CLICKED(IDC_BUTTON_PING3, &CService::OnBnClickedPinSer3)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_ETH_IP, &CService::OnIpnFieldchangedIpEth)
	ON_EN_CHANGE(IDC_EDIT_SVR_PORT1, &CService::OnEnChangePortSer1)
	ON_EN_CHANGE(IDC_EDIT_SVR_PORT2, &CService::OnEnChangePortSer2)
	ON_EN_CHANGE(IDC_EDIT_SVR_PORT3, &CService::OnEnChangePortSer3)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_ETH_MASK, &CService::OnIpnFieldchangedIpaddressMaskEth)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_ETH_GATEWAY, &CService::OnIpnFieldchangedIpaddressGatewayEth)
	ON_BN_CLICKED(IDC_CHECK_ETH_USE, &CService::OnBnClickedCheckEthUse)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_ETH_DNS, &CService::OnIpnFieldchangedIpaddressEthDns)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_IP_WLAN, &CService::OnIpnFieldchangedIpaddressIpWlan)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_MASK_WLAN, &CService::OnIpnFieldchangedIpaddressMaskWlan)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_GATEWAY_WLAN, &CService::OnIpnFieldchangedIpaddressGatewayWlan)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_DNS_WLAN, &CService::OnIpnFieldchangedIpaddressDnsWlan)
	ON_EN_CHANGE(IDC_EDIT_SSID_WLAN, &CService::OnEnChangeEditSsidWlan)
	ON_EN_CHANGE(IDC_EDIT_PSK_WLAN, &CService::OnEnChangeEditPskWlan)
	ON_EN_CHANGE(IDC_EDIT_PROTO_WLAN, &CService::OnEnChangeEditProtoWlan)
	ON_EN_CHANGE(IDC_EDIT_SSID_HOTSPOT, &CService::OnEnChangeEditSsidHotspot)
	ON_EN_CHANGE(IDC_EDIT_PASSPHRASE_HOTSPOT, &CService::OnEnChangeEditPassphraseHotspot)
	ON_EN_CHANGE(IDC_EDIT_DOMAIN_SVR1, &CService::OnEnChangeEditDomainSvr1)
	ON_EN_CHANGE(IDC_EDIT_DOMAIN_SVR2, &CService::OnEnChangeEditDomainSvr2)
	ON_EN_CHANGE(IDC_EDIT_DOMAIN_SVR3, &CService::OnEnChangeEditDomainSvr3)
	ON_EN_CHANGE(IDC_EDIT_SECRET, &CService::OnEnChangeEditSecret)
	ON_BN_CLICKED(IDC_CHECK_SVR4, &CService::OnBnClickedSvr4Valid)
	ON_BN_CLICKED(IDC_BUTTON_PING4, &CService::OnBnClickedPinSer4)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_MAC, &CService::OnBnClickedButtonAutoMac)
END_MESSAGE_MAP()


// CService 消息处理程序
void CService::SetServiceInf(st_net_conf* serviceInf)
{
	memcpy(pNetConf, serviceInf, sizeof(*pNetConf));
}

void CService::InitDlg()
{
	ShowInput(true);

	ReSetDlg();
	
	CString str;
	//服务器信息
	for(int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		if (pNetConf->service_net.ip_port_domain[i].valid)
		{
			str.Format("%d",pNetConf->service_net.ip_port_domain[i].port);
			pSerPort[i]->SetWindowText(str);
			pSerValid[i]->SetCheck(true);
			svrValid[i] = true;
			str.Format("%s",pNetConf->service_net.ip_port_domain[i].domain);
			pSerDomain[i]->SetWindowText(str);
		}
		else
		{
			pTestResult[i]->SetWindowText("未测试");
			//pSerPort[i]->SetWindowText("");
			pSerValid[i]->SetCheck(false);
			svrValid[i] = false;
		}
	}
	str.Format("%s",pNetConf->service_net.secret);
	mServiceSecret.SetWindowText(str);

	//网络信息
	//eth
	if (pNetConf->coltor_net.eth.used)
	{
		mUsedEth.SetCheck(true);

		mIpEth.SetWindowText(pNetConf->coltor_net.eth.ip);
		mMaskEth.SetWindowText(pNetConf->coltor_net.eth.mask);
		mGateWayEth.SetWindowText(pNetConf->coltor_net.eth.gateWay);
		mDnsEth.SetWindowText(pNetConf->coltor_net.eth.dns);
		mMacAddrEth.SetWindowText(pNetConf->coltor_net.eth.mac);

		if(pNetConf->coltor_net.eth.netType == STATIC)
		{
			mStaticNetEth.SetCheck(true);
			mIpEth.EnableWindow(true);
			mMaskEth.EnableWindow(true);
			mGateWayEth.EnableWindow(true);
			mDnsEth.EnableWindow(true);
		//	mMacAddrEth.EnableWindow(true);
		}
		else
		{
			mDynamicNetEth.SetCheck(true);
			mIpEth.EnableWindow(false);
			mMaskEth.EnableWindow(false);
			mGateWayEth.EnableWindow(false);
			mDnsEth.EnableWindow(false);
		//	mMacAddrEth.EnableWindow(true);
		}
	}
	else
	{
		mUsedEth.SetCheck(false);
		if(pNetConf->coltor_net.eth.netType == STATIC)
		{
			mStaticNetEth.SetCheck(true);
		}
		else
		{
			mDynamicNetEth.SetCheck(true);
		}
		mIpEth.SetWindowText(pNetConf->coltor_net.eth.ip);
		mIpEth.EnableWindow(false);
		mMaskEth.SetWindowText(pNetConf->coltor_net.eth.mask);
		mMaskEth.EnableWindow(false);
		mGateWayEth.SetWindowText(pNetConf->coltor_net.eth.gateWay);
		mGateWayEth.EnableWindow(false);
		mDnsEth.SetWindowText(pNetConf->coltor_net.eth.dns);
		mDnsEth.EnableWindow(false);
		mMacAddrEth.SetWindowText(pNetConf->coltor_net.eth.mac);
		mMacAddrEth.EnableWindow(false);
		btAutoMac.EnableWindow(false);
	}

	//hotspot
	mSsidHotspot.SetWindowText(pNetConf->coltor_net.hotspot.ssid);
	mPassPhraseHotSpot.SetWindowText(pNetConf->coltor_net.hotspot.passphrase);
}

int CService::SaveDlg()
{
	//memset(&pServiceInf, 0, sizeof(strServiceInf));
	//服务器信息

	CString str;
	int serviceNum = 0;
	for(int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		if (pSerValid[i]->GetCheck())
		{
			if (pNetConf->service_net.ip_port_domain[i].valid != true)
			{
				pNetConf->service_net.ip_port_domain[i].valid = true;
				pNetConf->service_net.is_modify = true;
			}
			
			pSerDomain[i]->GetWindowText(str);
			if (str == "")
			{
				MessageBox("domain无效");
				return -11;//服务器ip为空
			}
			if(0 != strcmp(pNetConf->service_net.ip_port_domain[i].domain,str.GetString()))
			{
				sprintf(pNetConf->service_net.ip_port_domain[i].domain, "%s", str);
				pNetConf->service_net.is_modify = true;
			}

			pSerPort[i]->GetWindowText(str);
			if (str == "")
			{
				MessageBox("端口无效");
				return -12;//服务器port为空
			}
			if(pNetConf->service_net.ip_port_domain[i].port != atoi(str.GetString()))
			{	
				pNetConf->service_net.ip_port_domain[i].port = atoi(str);
				pNetConf->service_net.is_modify = true;
			}
			serviceNum ++;
		}
		else
		{
			if (pNetConf->service_net.ip_port_domain[i].valid != false)
			{
				pNetConf->service_net.ip_port_domain[i].valid = false;
				pNetConf->service_net.is_modify = true;
			}
		}
	}

	mServiceSecret.GetWindowText(str);
	if (gCompanyType == ZI_HENG_COMPANY)
	{
		if (strcmp(str.GetString(), "12345") != 0)
		{
			MessageBox("服务器MD5序列号自定义字符串必须是12345");
			return -2;//静态ip为空
		}
	}
	if(0 != strcmp(pNetConf->service_net.secret,str.GetString()))
	{
		sprintf(pNetConf->service_net.secret, "%s", str);
		pNetConf->service_net.is_modify = true;
	}

	if (serviceNum <= 0)
	{
	//	MessageBox("服务器个数必须1-4");
	//	return -13;
	}

	//网络信息
	//eth
	if (mUsedEth.GetCheck())
	{
		if (pNetConf->coltor_net.eth.used != true)
		{
			pNetConf->coltor_net.eth.used = true;
			pNetConf->coltor_net.eth.is_modify = true;
		}
		int state = 0;
		state = mStaticNetEth.GetCheck();
		if(state)
		{
			if(pNetConf->coltor_net.eth.netType != STATIC)
			{
				pNetConf->coltor_net.eth.netType = STATIC;
				pNetConf->coltor_net.eth.is_modify = true;
			}
			mIpEth.GetWindowText(str);
			if(str == "0.0.0.0")
			{
				MessageBox("以太网ip无效");
				return -2;//静态ip为空
			}
			mMaskEth.GetWindowText(str);
			if(str == "0.0.0.0")
			{
				MessageBox("以太网掩码无效");
				return -3;
			}
			mGateWayEth.GetWindowText(str);
			if(str == "0.0.0.0")
			{
				MessageBox("以太网网关无效");
				return -4;
			}
			mDnsEth.GetWindowText(str);
			if(str == "0.0.0.0")
			{
				MessageBox("以太网dns无效");
				return -5;
			}
			mMacAddrEth.GetWindowText(str);
			if(str == "0.0.0.0" || str.GetLength() == 0)
			{
				MessageBox("以太网mac无效");
				return -51;
			}
			mIpEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.ip,str.GetString()))
			{
				sprintf(pNetConf->coltor_net.eth.ip,"%s",str);
				pNetConf->coltor_net.eth.is_modify = true;
			}

			mMaskEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.mask,str.GetString()))
			{
				sprintf(pNetConf->coltor_net.eth.mask,"%s",str);
				pNetConf->coltor_net.eth.is_modify = true;
			}

			mGateWayEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.gateWay,str.GetString()))
			{
				sprintf(pNetConf->coltor_net.eth.gateWay,"%s",str);
				pNetConf->coltor_net.eth.is_modify = true;
			}

			mDnsEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.dns,str.GetString()))
			{
				sprintf(pNetConf->coltor_net.eth.dns,"%s",str);
				pNetConf->coltor_net.eth.is_modify = true;
			}

			mMacAddrEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.mac,str.GetString()))
			{
				sprintf(pNetConf->coltor_net.eth.mac,"%s",str);
				pNetConf->coltor_net.eth.is_modify = true;
			}
		}
		else
		{
			if(pNetConf->coltor_net.eth.netType != DYNAMIC)
			{
				pNetConf->coltor_net.eth.netType = DYNAMIC;
				pNetConf->coltor_net.eth.is_modify = true;
			}
			mMacAddrEth.GetWindowText(str);
			if(str == "0.0.0.0")
			{
				MessageBox("以太网mac无效");
				return -511;
			}
			mMacAddrEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.mac,str.GetString()))
			{
				sprintf(pNetConf->coltor_net.eth.mac,"%s",str);
				pNetConf->coltor_net.eth.is_modify = true;
			}
		}
	}
	else	
	{
		if (pNetConf->coltor_net.eth.used != false)
		{
			pNetConf->coltor_net.eth.used = false;
			pNetConf->coltor_net.eth.is_modify = true;
		}
	}

	//hotspot
	mSsidHotspot.GetWindowText(str);
	if (str == "")
	{
		MessageBox("热点名称无效");
		return -121;
	}
	mPassPhraseHotSpot.GetWindowText(str);
	if (str == "" || str.GetLength() < 8)
	{
		MessageBox("热点密码无效");
		return -122;
	}

	mSsidHotspot.GetWindowText(str);
	if(0 != strcmp(pNetConf->coltor_net.hotspot.ssid,str.GetString()))
	{
		sprintf(pNetConf->coltor_net.hotspot.ssid,"%s",str);
		pNetConf->coltor_net.hotspot.is_modify = true;
	}

	mPassPhraseHotSpot.GetWindowText(str);
	if(0 != strcmp(pNetConf->coltor_net.hotspot.passphrase,str.GetString()))
	{
		sprintf(pNetConf->coltor_net.hotspot.passphrase,"%s",str);
		pNetConf->coltor_net.hotspot.is_modify = true;
	}

	return 0;
}

void CService::OnBnClickedNetDynamicEth()
{
	mIpEth.EnableWindow(false);
	mMaskEth.EnableWindow(false);
	mGateWayEth.EnableWindow(false);
	mDnsEth.EnableWindow(false);
	//mMacAddrEth.EnableWindow(false);
	DisplaySaveButton();
}

void CService::OnBnClickedNetStaticEth()
{
	mIpEth.EnableWindow(true);
	mMaskEth.EnableWindow(true);
	mGateWayEth.EnableWindow(true);
	mDnsEth.EnableWindow(true);
	//mMacAddrEth.EnableWindow(true);
	DisplaySaveButton();
}

void CService::SetSvrGroup()
{
	for(int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		if (pNetConf->service_net.ip_port_domain[i].valid)
		{
			EnableSvrGroup(true,i);
		}
		else
		{
			EnableSvrGroup(false,i);
		}
	}
}

void CService::OnBnClickedSvr1Valid()
{
	SvrValidChangedAction(0);	
}

void CService::OnBnClickedSvr2Valid()
{
	SvrValidChangedAction(1);		
}

void CService::OnBnClickedSvr3Valid()
{
	SvrValidChangedAction(2);		
}

void CService::OnBnClickedPinSer1()
{
	PingSerByInd(0);
}


void CService::OnBnClickedPinSer2()
{
	PingSerByInd(1);
}


void CService::OnBnClickedPinSer3()
{
	PingSerByInd(2);
}

void CService::PingSerByInd( int ind )
{
	CString str;
	int ret = 0;
	st_ip_port_domain ip_port_domain;
	memset(&ip_port_domain, 0, sizeof(ip_port_domain));

	pSerDomain[ind]->GetWindowText(str);
	if (str == "0.0.0.0" || str.GetLength() == 0)
	{
		pTestResult[ind]->SetWindowText("失败");
		return;
	}
	sprintf(ip_port_domain.domain, "%s", str);

	ret = pingNet(ip_port_domain.domain);
	if (ret == 0)
	{
		pTestResult[ind]->SetWindowText("成功");
	}
	else
	{
		pTestResult[ind]->SetWindowText("失败");
	}
}

void CService::DisplaySaveButton()
{
	bool modify = false;
	CString str;
	for(int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		if (pSerValid[i]->GetCheck())
		{
			if (pNetConf->service_net.ip_port_domain[i].valid != true)
			{
				modify = true;
				//TRACE("pServiceInf->serviceIpPort.ipPort[i].valid != true\n");
			}

			pSerDomain[i]->GetWindowText(str);
			if(0 != strcmp(pNetConf->service_net.ip_port_domain[i].domain,str.GetString()))
			{
				modify = true;
			}

			pSerPort[i]->GetWindowText(str);
			if(pNetConf->service_net.ip_port_domain[i].port != atoi(str.GetString()))
			{	
				modify = true;
			}
		}
		else
		{
			if (pNetConf->service_net.ip_port_domain[i].valid != false)
			{
				modify = true;
				//TRACE("pServiceInf->serviceIpPort.ipPort[%d].valid != false,=%d\n",
				//	i,pServiceInf->serviceIpPort.ipPort[i].valid);
			}
		}
	}
	mServiceSecret.GetWindowText(str);
	if(0 != strcmp(pNetConf->service_net.secret,str.GetString()))
	{
		modify = true;
	}
	//网络信息
	//eth
	if (mUsedEth.GetCheck())
	{
		if (pNetConf->coltor_net.eth.used != true)
		{
			modify = true;
		}
		int state = 0;
		state = mStaticNetEth.GetCheck();
		if(state)
		{
			if(pNetConf->coltor_net.eth.netType != STATIC)
			{
				modify = true;
				//TRACE("pServiceInf->coltorNet.netInf.netType != STATIC\n");
			}
			mIpEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.ip,str.GetString()))
			{
				modify = true;
				//TRACE("0 != strcmp(pServiceInf->coltorNet.netInf.ip,str.GetString())\n");
			}
			mMaskEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.mask,str.GetString()))
			{
				modify = true;
				//TRACE("0 != strcmp(pServiceInf->coltorNet.netInf.mask,str.GetString())\n");
			}
			mGateWayEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.gateWay,str.GetString()))
			{
				modify = true;
				//TRACE("0 != strcmp(pServiceInf->coltorNet.netInf.gateWay,str.GetString())\n");
			}
			mDnsEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.dns,str.GetString()))
			{
				modify = true;
				//TRACE("0 != strcmp(pServiceInf->coltorNet.netInf.dns,str.GetString())\n");
			}
			mMacAddrEth.GetWindowText(str);
			if(0 != strcmp(pNetConf->coltor_net.eth.mac,str.GetString()))
			{
				modify = true;
				//TRACE("0 != strcmp(pServiceInf->coltorNet.netInf.mac,str.GetString())\n");
			}
		}
		else
		{
			if(pNetConf->coltor_net.eth.netType != DYNAMIC)
			{
				modify = true;
				//TRACE("pServiceInf->coltorNet.netInf.netType != DYNAMIC\n");
			}
		}	
	}
	else
	{
		if (pNetConf->coltor_net.eth.used != false)
		{
			modify = true;
		}
	}

	//hotspot
	mSsidHotspot.GetWindowText(str);
	if(0 != strcmp(pNetConf->coltor_net.hotspot.ssid,str.GetString()))
	{
		modify = true;
	}

	mPassPhraseHotSpot.GetWindowText(str);
	if(0 != strcmp(pNetConf->coltor_net.hotspot.passphrase,str.GetString()))
	{
		modify = true;
	}

	if (modify == true)
	{
		if (gComNum != 0)
		{
			GetParent()->GetDlgItem(IDC_BUTTON_SAVE_COM)->EnableWindow(true);
		}
	}
	else
	{
	}
}

void CService::OnIpnFieldchangedIpEth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	*pResult = 0;
	DisplaySaveButton();
}

void CService::OnEnChangePortSer1()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}

void CService::OnEnChangePortSer2()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}

void CService::OnEnChangePortSer3()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}

int CService::pingNet(char * ip)
{
	int ok = 0;
	BOOL is_delete = FALSE;
	FILE * f = NULL;
	char file_name[256] = "";
	char cmd[256]=""; 
	int ret = 0;

	sprintf(file_name,"%s\\ping.txt",gCurrentDir.GetString());

	sprintf(cmd, "ping %s > %s", ip, file_name);
	ret = system(cmd);
	if (ret < 0)
	{
		return -1;
	}

	f = fopen(file_name, "r");
	if(!f)
	{
		return -1;
	}

	while(!feof(f)) 
	{
		fgets(cmd, 80, f);
		if(strstr(cmd, "(100% 丢失)"))
		{
			ok = -1;
		}
		if(strstr(cmd, "请求找不到主机"))
		{
			ok = -1;
		}
	}

	fclose(f);
	sprintf(cmd, "del %s", file_name);

	ret = system(cmd);
	return ok;
}

void CService::ReSetDlg()
{
	for (int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		pTestResult[i]->SetWindowText("未测试");
		svrValid[i] = false;
	}

	//mUsedEth.SetCheck(false);
	//mStaticNetEth.SetCheck(false);
	//mDynamicNetEth.SetCheck(false);
	mIpEth.ClearAddress();
	mMaskEth.ClearAddress();
	mGateWayEth.ClearAddress();
	mDnsEth.ClearAddress();

	SetSvrGroup();
}

int CService::CountValidSvrNum()
{
	int ret = 0;
	for (int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		if (svrValid[i] == true)
		{
			ret ++;
		}
	}
	return ret;
}

void CService::EnableSvrGroup( bool is_show, int ind )
{
	pSerPort[ind]->EnableWindow(is_show);
	pPingButton[ind]->EnableWindow(is_show);
	pTestResult[ind]->EnableWindow(is_show);
	pSerDomain[ind]->EnableWindow(is_show);
}


BOOL CService::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_RETURN: //回车  
			return TRUE;  
		case VK_ESCAPE: //ESC  
			return TRUE;  
		}  
	} 
	if(pMsg->message == WM_RBUTTONDOWN) 
	{  
		return TRUE;
	} 
	return CDialog::PreTranslateMessage(pMsg);
}


void CService::OnIpnFieldchangedIpaddressMaskEth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	*pResult = 0;
	DisplaySaveButton();
}


void CService::OnIpnFieldchangedIpaddressGatewayEth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	*pResult = 0;
	DisplaySaveButton();
}

void CService::ShowInput( bool show )
{
	for(int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		pSerPort[i]->EnableWindow(show);
		pSerValid[i]->EnableWindow(show);
		pPingButton[i]->EnableWindow(show);
		pSerDomain[i]->EnableWindow(show);
	}

	mUsedEth.EnableWindow(show);
	mIpEth.EnableWindow(show);
	mStaticNetEth.EnableWindow(show);
	mDynamicNetEth.EnableWindow(show);
	mMaskEth.EnableWindow(show);
	mGateWayEth.EnableWindow(show);
	mDnsEth.EnableWindow(show);
	mMacAddrEth.EnableWindow(false);
	btAutoMac.EnableWindow(show);

	mSsidHotspot.EnableWindow(show);
	mPassPhraseHotSpot.EnableWindow(show);

	mServiceSecret.EnableWindow(show);

}


BOOL CService::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowInput(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CService::OnBnClickedCheckEthUse()
{
	if(!mUsedEth.GetCheck())
	{
		//禁用
		mStaticNetEth.EnableWindow(FALSE);
		mDynamicNetEth.EnableWindow(FALSE);
		mIpEth.EnableWindow(false);
		mMaskEth.EnableWindow(false);
		mGateWayEth.EnableWindow(false);
		mDnsEth.EnableWindow(false);
		mMacAddrEth.EnableWindow(false);
		btAutoMac.EnableWindow(false);
	}
	else
	{
		mStaticNetEth.EnableWindow(TRUE);
		mDynamicNetEth.EnableWindow(TRUE);
		mIpEth.EnableWindow(TRUE);
		mMaskEth.EnableWindow(TRUE);
		mGateWayEth.EnableWindow(TRUE);
		mDnsEth.EnableWindow(TRUE);
		//mMacAddrEth.EnableWindow(TRUE);
		btAutoMac.EnableWindow(TRUE);

	}

	DisplaySaveButton();
}

void CService::OnIpnFieldchangedIpaddressEthDns(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	*pResult = 0;
	DisplaySaveButton();
}

void CService::OnIpnFieldchangedIpaddressIpWlan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DisplaySaveButton();
}


void CService::OnIpnFieldchangedIpaddressMaskWlan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DisplaySaveButton();
}


void CService::OnIpnFieldchangedIpaddressGatewayWlan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DisplaySaveButton();
}


void CService::OnIpnFieldchangedIpaddressDnsWlan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DisplaySaveButton();
}

void CService::OnEnChangeEditSsidWlan()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnEnChangeEditPskWlan()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnEnChangeEditProtoWlan()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnEnChangeEditSsidHotspot()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnEnChangeEditPassphraseHotspot()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}

void CService::OnEnChangeEditDomainSvr1()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnEnChangeEditDomainSvr2()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnEnChangeEditDomainSvr3()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnEnChangeEditSecret()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CService::OnBnClickedSvr4Valid()
{
	SvrValidChangedAction(3);	
}


void CService::OnBnClickedPinSer4()
{
	// TODO: 在此添加控件通知处理程序代码
	PingSerByInd(3);
}

void CService::SvrValidChangedAction( int server_ind )
{
	if(!pSerValid[server_ind]->GetCheck())
	{
		int num = CountValidSvrNum();
		if (num <= 1)
		{
			//	pSerValid[server_ind]->SetCheck(true);
			//	return;
		}
		svrValid[server_ind] = false;

		EnableSvrGroup(false,server_ind);
	}
	else
	{
		EnableSvrGroup(true,server_ind);
		svrValid[server_ind] = true;
	}
	DisplaySaveButton();
}


void CService::OnBnClickedButtonAutoMac()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime tm; 
	tm = CTime::GetCurrentTime();//获得当前时间 
	st_time local_time;
	local_time.YY = tm.GetYear();
	local_time.MM = tm.GetMonth();
	local_time.DD= tm.GetDay(); 
	local_time.HH= tm.GetHour();
	local_time.MI= tm.GetMinute();
	local_time.SS= tm.GetSecond();

	MU_INT64 time_hex = 0;
	time_hex = 
		local_time.YY * 100 * 100 * 100 * 100 * 100 + 
		local_time.MM * 100 * 100 * 100 * 100 + 
		local_time.DD * 100 * 100 * 100 + 
		local_time.HH * 100 * 100 + 
		local_time.MI * 100 + 
		local_time.SS ;

	CString str;
	str.Format("%02d:%02d:%02d:%02d:%02d:%02d",
		local_time.YY % 100,
		local_time.MM,
		local_time.DD,
		local_time.HH,
		local_time.MI,
		local_time.SS
		);
	mMacAddrEth.SetWindowText(str);

}
