#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DataTypeDef.h"
#include "ValidEdit.h"

typedef vector<st_ip_port_domain> VEC_IPPORT;

// CNetBroadCast �Ի���

class CNetBroadCast : public CDialog
{
	DECLARE_DYNAMIC(CNetBroadCast)

public:
	CNetBroadCast(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNetBroadCast();

// �Ի�������
	enum { IDD = IDD_DIALOG_NET_BROADCAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl listParam;
	CIPAddressCtrl mIp;
	CButton btBroadCast;
	CButton btOk;
	CStatic lbBroadState;

public:
	st_ip_port_domain* GetIpPort(){return &ipPort;};
private:
	void CreateList();//�����б�
	int  GetListInf(int row_ind, st_ip_port_domain& ip_port);//��list��Ӧ����ȡ�豸��Ϣ
	void SetListInf(int row_ind, st_ip_port_domain& ip_port);//�����豸��Ϣ����list��Ӧ����Ϣ
	void ClearList();//����б�

	void ShowControl(bool is_show);
private:
	int init_net();
	int send_udp();
	int recv_udp();
private:
	SOCKADDR_IN toAddr;//�㲥�ͻ�����Ϣ
	SOCKADDR_IN fromAddr;//�㲥�ͻ�����Ϣ

	SOCKET sockFdUdp;//udp fd
	st_ip_port_domain ipPort;
	VEC_IPPORT vecIpPort;//�㲥��Ӧ����
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBroadcast();
	afx_msg void OnBnClickedButtonFinished();
	afx_msg void OnNMClickListIpPort(NMHDR *pNMHDR, LRESULT *pResult);
	CButton btUseWifi;
	afx_msg void OnBnClickedButtonUseWifi();
};
