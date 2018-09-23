// NetBroadCast.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "NetBroadCast.h"
#include "afxdialogex.h"
#include "dataConvert.h"


// CNetBroadCast �Ի���

IMPLEMENT_DYNAMIC(CNetBroadCast, CDialog)

	CNetBroadCast::CNetBroadCast(CWnd* pParent /*=NULL*/)
	: CDialog(CNetBroadCast::IDD, pParent)
{
	memset(&ipPort, 0, sizeof(ipPort));
}

CNetBroadCast::~CNetBroadCast()
{
	if(INVALID_SOCKET != sockFdUdp)
	{
		closesocket(sockFdUdp);
		sockFdUdp = INVALID_SOCKET;
	}
}

void CNetBroadCast::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IP_PORT, listParam);
	DDX_Control(pDX, IDC_IPADDRESS_IP, mIp);
	DDX_Control(pDX, IDC_BUTTON_BROADCAST, btBroadCast);
	DDX_Control(pDX, IDC_BUTTON_FINISHED, btOk);
	DDX_Control(pDX, IDC_BROAD_STATE, lbBroadState);
	DDX_Control(pDX, IDC_BUTTON_USE_WIFI, btUseWifi);
}


BEGIN_MESSAGE_MAP(CNetBroadCast, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROADCAST, &CNetBroadCast::OnBnClickedButtonBroadcast)
	ON_BN_CLICKED(IDC_BUTTON_FINISHED, &CNetBroadCast::OnBnClickedButtonFinished)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IP_PORT, &CNetBroadCast::OnNMClickListIpPort)
	ON_BN_CLICKED(IDC_BUTTON_USE_WIFI, &CNetBroadCast::OnBnClickedButtonUseWifi)
END_MESSAGE_MAP()

void CNetBroadCast::CreateList()
{
	LONG l_style;
	l_style = GetWindowLong(listParam.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	l_style &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	l_style |= LVS_REPORT; //����style
	SetWindowLong(listParam.m_hWnd, GWL_STYLE, l_style);//����style

	DWORD dw_style = listParam.GetExtendedStyle();
	dw_style |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dw_style |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	listParam.SetExtendedStyle(dw_style); //������չ���

	int n_index = 0;
	int width = 200;
	listParam.InsertColumn(n_index++, "", LVCFMT_LEFT, 30 );
	listParam.InsertColumn(n_index++, "IP", LVCFMT_LEFT, width * 222/100 );
}

int CNetBroadCast::GetListInf( int row_ind, st_ip_port_domain& ip_port )
{
	CString str;
	int ret = 0;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	int column_ind = 0;

	str = listParam.GetItemText(row_ind, ++column_ind);
	strcpy(ip_port.domain, str.GetString());

	return 0;
}

void CNetBroadCast::SetListInf( int row_ind, st_ip_port_domain& ip_port )
{
	int ret = 0;
	BOOL suc = FALSE;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	char item[64];
	memset(item, 0, sizeof(item));

	sprintf(item, "%d", row_ind + 1);
	row_ind = listParam.InsertItem(row_ind, item);//������,��һ��

	int ColumnInd = 1;

	sprintf(item, "%s", ip_port.domain);
	suc = listParam.SetItemText(row_ind, ColumnInd++, item);
}

int CNetBroadCast::init_net()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if(0 != WSAStartup(wVersionRequested, &wsaData))
	{
		TRACE("WSAStartup failed with error: %d\n", GetLastError());
		return EXIT_FAILURE;
	}
	if(2 != HIBYTE(wsaData.wVersion) || 2 != LOBYTE(wsaData.wVersion))
	{
		TRACE("Socket version not supported.\n");
		WSACleanup();
		return EXIT_FAILURE;
	}
	sockFdUdp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(INVALID_SOCKET == sockFdUdp)
	{
		TRACE("socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return EXIT_FAILURE;
	}
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
	toAddr.sin_port = htons(12345);

	//add bind
	SOCKADDR_IN listen_addr;//��������
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(12345);
	int res = bind(sockFdUdp, (struct sockaddr*)&listen_addr, sizeof(listen_addr));
	if (res == SOCKET_ERROR)
	{
		TRACE("socket bind failed with error: %d\n", WSAGetLastError());
		if(INVALID_SOCKET != sockFdUdp)
		{
			closesocket(sockFdUdp);
			sockFdUdp = INVALID_SOCKET;
		}
		WSACleanup();
		return EXIT_FAILURE;
	}

	unsigned long i_dummy = 1;
	if( ioctlsocket( sockFdUdp, FIONBIO, &i_dummy ) != 0 )
	{
		TRACE("[%s]:can not set non-block model\n",__FUNCTION__);
	}

	BOOL bBoardcast = TRUE;
	if(SOCKET_ERROR == setsockopt(sockFdUdp, SOL_SOCKET, SO_BROADCAST, (char*)&bBoardcast, sizeof(bBoardcast)))
	{
		TRACE("setsockopt failed with error code: %d\n", WSAGetLastError());
		if(INVALID_SOCKET != sockFdUdp)
		{
			closesocket(sockFdUdp);
			sockFdUdp = INVALID_SOCKET;
		}
		WSACleanup();
	}
	return 0;
}

int CNetBroadCast::send_udp()
{
	char send_buf[] = {"xhb_req"};
	int send_len = sizeof(send_buf);

	int svr_addr_len = sizeof(toAddr);

	int send_rst = sendto(sockFdUdp, send_buf, send_len, 0, (LPSOCKADDR)&toAddr, svr_addr_len);
	if (send_rst != send_len)
	{
		TRACE("[%s]sendto error: %d\n", __FUNCTION__,WSAGetLastError());
		return -1;
	}
	//TRACE("[%s]sendto suc [%s]\n", __FUNCTION__, send_buf);
	return 0;
}

int CNetBroadCast::recv_udp()
{
	fd_set fds_r;
	fd_set fds_e;
	int i_ret = 0;
	FD_ZERO( &fds_r );
	FD_SET( sockFdUdp, &fds_r );
	FD_ZERO( &fds_e );
	FD_SET( sockFdUdp, &fds_e );

	struct timeval  time_out;
	time_out.tv_sec = 0;
	time_out.tv_usec = 500 * 1000;

	i_ret = select(sockFdUdp + 1, &fds_r, NULL, &fds_e, &time_out);
	if( i_ret > 0 )
	{
		char recv_buf[256];
		int recv_len = sizeof(recv_buf) - 1;
		memset(recv_buf, 0, sizeof(recv_buf));
		int svr_addr_len = sizeof(fromAddr);
		int rev_rst = recvfrom(sockFdUdp, recv_buf, recv_len, 0, (LPSOCKADDR)&fromAddr, &svr_addr_len);  
		if (0 == strcmp(recv_buf, "xhb_ack"))
		{
			st_ip_port_domain ip_port;
			memset(&ip_port, 0, sizeof(ip_port));
			strcpy(ip_port.domain, inet_ntoa(fromAddr.sin_addr));
			ip_port.port = ntohs(fromAddr.sin_port);

			//TRACE("[%s]recvfrom suc: <%s : %d> [%s]\n", 
			//	__FUNCTION__,ip_port.domain, ip_port.port, recv_buf);

			vecIpPort.push_back(ip_port);

			return 0;

		}
		else if (0 == strcmp(recv_buf, "xhb_req"))
		{
			return 0;
		}
		else
		{
			return 0;
		}
	}
	else 
	{
		//TRACE("[%s]select fail\n", __FUNCTION__);
	}
	return -1;
}


// CNetBroadCast ��Ϣ�������


BOOL CNetBroadCast::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	lbBroadState.SetWindowText("");

	CreateList();
	init_net();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CNetBroadCast::OnBnClickedButtonBroadcast()
{
	CString str;
	int ret = 0;
	lbBroadState.SetWindowText("�㲥��,��ȴ�...");
	ShowControl(false);
	ClearList();

	ret = send_udp();
	if (ret != 0)
	{
		str.Format("�㲥����ʧ��");
		ShowControl(true);
		MessageBox(str);
		return;
	}

	vecIpPort.clear();

	while(1)
	{
		ret = recv_udp();
		if (ret != 0)
		{
			break;
		}
		else
		{
			continue;
		}
	}

	//�����б�
	for (MU_INT32 i = 0; i < vecIpPort.size(); i++)
	{
		SetListInf(i, vecIpPort[i]);
	}

	lbBroadState.SetWindowText("�㲥����");
	ShowControl(true);

}


void CNetBroadCast::OnBnClickedButtonFinished()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	mIp.GetWindowText(str);
	if (str == "0.0.0.0" || str.GetLength() == 0)
	{
		str.Format("IP ��Ч");
		MessageBox(str);
		return;
	}
	strcpy(ipPort.domain, str.GetString());

	CDialog::OnOK();
}


void CNetBroadCast::OnNMClickListIpPort(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	if ((listParam.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		st_ip_port_domain ip_port;
		memset(&ip_port, 0, sizeof(ip_port));

		int item_count = listParam.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		GetListInf(item_count, ip_port);
		CString str;
		str.Format("%s", ip_port.domain);
		mIp.SetWindowText(str);

	}

}

void CNetBroadCast::ShowControl( bool is_show )
{
	btBroadCast.EnableWindow(is_show);
	btOk.EnableWindow(is_show);
}

void CNetBroadCast::ClearList()
{
	while(listParam.GetNextItem(-1,LVNI_ALL) != -1)
	{
		int n_item = listParam.GetNextItem(-1,LVNI_ALL);
		listParam.DeleteItem(n_item);//ɾ��
	}
}


void CNetBroadCast::OnBnClickedButtonUseWifi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mIp.SetWindowText("192.168.99.1");
}
