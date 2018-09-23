#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DataTypeDef.h"
#include "ValidEdit.h"

typedef vector<st_ip_port_domain> VEC_IPPORT;

// CNetBroadCast 对话框

class CNetBroadCast : public CDialog
{
	DECLARE_DYNAMIC(CNetBroadCast)

public:
	CNetBroadCast(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNetBroadCast();

// 对话框数据
	enum { IDD = IDD_DIALOG_NET_BROADCAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	void CreateList();//创建列表
	int  GetListInf(int row_ind, st_ip_port_domain& ip_port);//从list相应行提取设备信息
	void SetListInf(int row_ind, st_ip_port_domain& ip_port);//根据设备信息设置list相应行信息
	void ClearList();//清空列表

	void ShowControl(bool is_show);
private:
	int init_net();
	int send_udp();
	int recv_udp();
private:
	SOCKADDR_IN toAddr;//广播客户端信息
	SOCKADDR_IN fromAddr;//广播客户端信息

	SOCKET sockFdUdp;//udp fd
	st_ip_port_domain ipPort;
	VEC_IPPORT vecIpPort;//广播响应集合
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBroadcast();
	afx_msg void OnBnClickedButtonFinished();
	afx_msg void OnNMClickListIpPort(NMHDR *pNMHDR, LRESULT *pResult);
	CButton btUseWifi;
	afx_msg void OnBnClickedButtonUseWifi();
};
