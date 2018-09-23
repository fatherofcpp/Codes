#pragma once
#include "DataTypeDef.h"
#include "ValidEdit.h"
#include "afxwin.h"

// CService �Ի���
//��������Ϣ
class CService : public CDialog
{
	DECLARE_DYNAMIC(CService)

public:
	CService(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CService();

// �Ի�������
	enum { IDD = IDD_DIALOG_SERVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	//////////////////////////////////////////
public:
	friend class CDtuConfigDlgDlg;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
private:
	afx_msg void OnBnClickedNetDynamicEth();
	afx_msg void OnBnClickedNetStaticEth();
	afx_msg void OnBnClickedSvr1Valid();
	afx_msg void OnBnClickedSvr2Valid();
	afx_msg void OnBnClickedSvr3Valid();
	afx_msg void OnBnClickedSvr4Valid();

	afx_msg void OnBnClickedPinSer1();
	afx_msg void OnBnClickedPinSer2();
	afx_msg void OnBnClickedPinSer3();
	afx_msg void OnBnClickedPinSer4();
	afx_msg void OnBnClickedCheckEthUse();
	afx_msg void OnIpnFieldchangedIpEth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressMaskEth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressGatewayEth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangePortSer1();
	afx_msg void OnEnChangePortSer2();
	afx_msg void OnEnChangePortSer3();
	afx_msg void OnIpnFieldchangedIpaddressEthDns(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressIpWlan(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressMaskWlan(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressGatewayWlan(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedIpaddressDnsWlan(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditSsidWlan();
	afx_msg void OnEnChangeEditPskWlan();
	afx_msg void OnEnChangeEditProtoWlan();
	afx_msg void OnEnChangeEditSsidHotspot();
	afx_msg void OnEnChangeEditPassphraseHotspot();
	afx_msg void OnEnChangeEditDomainSvr1();
	afx_msg void OnEnChangeEditDomainSvr2();
	afx_msg void OnEnChangeEditDomainSvr3();
	afx_msg void OnEnChangeEditSecret();
	afx_msg void OnBnClickedButtonAutoMac();

private:
	CStatic mGroupService;

	ValidEdit mSerPort1;
	CButton mValidSer1;
	CButton mPinSer1;
	CStatic mTest1;
	ValidEdit mDomainSvr1;

	ValidEdit mSerPort2;
	CButton mValidSer2;
	CButton mPinSer2;
	CStatic mTest2;
	ValidEdit mDomainSvr2;

	ValidEdit mSerPort3;
	CButton mValidSer3;
	CButton mPinSer3;
	CStatic mTest3;
	ValidEdit mDomainSvr3;

	ValidEdit mSerPort4;
	CButton mValidSer4;
	CButton mPinSer4;
	CStatic mTest4;
	ValidEdit mDomainSvr4;

	ValidEdit mServiceSecret;

	CStatic mGroupEth;
	CStatic mIpLableEth;
	CIPAddressCtrl mIpEth;
	CButton mStaticNetEth;
	CButton mDynamicNetEth;
	CIPAddressCtrl mMaskEth;
	CIPAddressCtrl mGateWayEth;
	ValidEdit mMacAddrEth;
	CButton btAutoMac;
	CIPAddressCtrl mDnsEth;
	CButton mUsedEth;

	ValidEdit mSsidHotspot;
	ValidEdit mPassPhraseHotSpot;

private:
	CEdit* pSerPort[MAX_SERVICE_NUM];
	CButton* pSerValid[MAX_SERVICE_NUM];
	CStatic* pTestResult[MAX_SERVICE_NUM];
	CButton* pPingButton[MAX_SERVICE_NUM];
	CEdit* pSerDomain[MAX_SERVICE_NUM];

	bool svrValid[MAX_SERVICE_NUM];

private:
	void PingSerByInd(int ind);//����ָ��������
	int pingNet(char * ip);//��������
	void ReSetDlg();//���öԻ���
	int CountValidSvrNum();//��Ч���ϱ�����������

private:
	void ShowInput(bool show);//��ʾ����
	void SetSvrGroup();//���÷��������û�״̬
	void EnableSvrGroup(bool is_show, int ind);//�ûҷ�����
	void DisplaySaveButton();//��ʾ���水ť
	void SvrValidChangedAction(int server_ind);
public:
	void SetServiceInf(st_net_conf* serviceInf);
	void InitDlg();
	int SaveDlg();
private:
	st_net_conf* pNetConf;//��������Ϣ
	///////////////////////////////////////////////////////////////add
};
