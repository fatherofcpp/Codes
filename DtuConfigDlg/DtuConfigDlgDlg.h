
// DtuConfigDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Coltor.h"
#include "OuterDev.h"
#include "Service.h"
#include "OuterDevTree.h"
#include "ReportFormat.h"
#include "ProtocolTemplate.h"
#include "Transfer.h"
#include "xmlAnalyze.h"
#include "include/mty.h"
#include "afxwin.h"
#include "Progress.h"
#include "log.h"
#include "NetBroadCast.h"
#include "HelpDocument.h"


// CDtuConfigDlgDlg �Ի���
class CDtuConfigDlgDlg : public CDialog
{
// ����
public:
	CDtuConfigDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CDtuConfigDlgDlg();
// �Ի�������
	enum { IDD = IDD_DTUCONFIGDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//////////////////////////////////////////////////////////////////////
	//����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//��Ϣ��Ӧ
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSaveCom();
	afx_msg void OnBnClickedButtonLoadCom();
	afx_msg void OnBnClickedButtonLoadLocal();
	afx_msg void OnBnClickedButtonSaveLocal();
	afx_msg void OnBnClickedButtonRestart();
	afx_msg void OnBnClickedButtonUpgrad();
	afx_msg void OnBnClickedButtonHistorydata();
	afx_msg void OnBnClickedButtonSaveHisdata();
	afx_msg void OnBnClickedButtonCleanhisdsta();
	afx_msg void OnBnClickedButtonStartReport();
	afx_msg void OnBnClickedButtonEndReport();
	afx_msg void OnBnClickedButtonCopyHisdataUdisk();
	afx_msg void OnBnClickedButtonCopyLogUdisk();
	afx_msg void OnBnClickedButtonBackFactory();
	afx_msg void OnBnClickedButtonNetBroad();
	afx_msg void OnBnClickedButtonSdReset();

public:
	//ͨѶ�ӿ� 
	int GetDataFromCollector();
	int SetDataToCollector();

	//������������
	int GetAge();
	int SetAge();

	//��-�ɼ�����������
	int GetCollectorInf();
	int SetCollectorInf();

	//��-��������
	int GetBuilInf();
	int SetBuilInf();

	//��-ʱ������
	int GetTimeInf();
	int SetTimeInf();

	//��-�ӿ��豸����
	int GetPortInf();
	int SetPortInf();

	int GetPortDevNum(int port_ind);
	int StartSetPortInf(int port_ind);
	int EndSetPortInf(int port_ind);

	//��-eth������
	int GetEthNetInf();
	int SetEthNetInf();
	//���ȵ����ã�
	int GetHotSpotInf();
	int SetHotSpotInf();

	//��-����������
	int GetServiceInf();
	int SetServiceInf();

	//�ϱ���ʽ����:
	int GetReportFormatInf();
	int SetReportFormatInf();
	//����������Чĳ��
	int SetEffectNow(VEC_STRING& vec_name_string);
	//���òɼ����������ã��ӿ��豸���ã�������������Ч
	int SetReStart();

	//���Զ���������
	int GetDefSubTypeInf();
	int SetDefSubTypeInf();
	int GetDefSubTypeNum(int& num);
	int StartSetDefSubTypeInf();
	int EndSetDefSubTypeInf();
	int GetDefSubTypeConf(int def_sub_type_ind, int& way_num);//���ػ�·��
	int SetDefSubTypeConf(int def_sub_type_ind, int order_ind);

	//д-���豸��ʷ����
	int SetCleanHisData();

	//������ʷ���ݵ�u��
	int SetCopyHisDataUDisk(st_time& date_time);
	int GetMonthHisDataInf(st_time& date_time, VEC_TIME& data_vec);
	int CopyHisDataDayUDisk(st_time& date_time);

	//������־��u��
	int SetCopyLogUDisk();

	//�ָ���������
	int SetBackFactory();

	//д - Զ������
	int SetUpgrad(char*data, int data_len);
	//д-��ʼԶ������
	int SetStartUpgrad(int pack_num, int pack_len_max);
	//д-Զ����������
	int SetUpgradData(int pack_ind,char* data,int data_len);
	//д-����Զ������
	int SetEndUpgrad();

	//����
	int SetHeartBeat();

	//��ʽ��SD��
	int SetResetSDCard();
	////////////////////////////////////////////////////////////////////
	//Ԥ��
	//д-��ʼ�����ϱ�
	int SetStartReport();
	//д-ֹͣ�����ϱ�
	int SetEndReport();

	//��-��ʷ����
	int GetHistoryData(int pre_result);

	//����-��ʷ����
	int SaveHistoryData();
	//д-��ʼ������ʷ����
	int SetStartSaveHisData();
	//д-����������ʷ����
	int SetEndSaveHisData();

	////////////////////////////////////////////////////////////////////
public:
	//�߼��ӿ�
	void InitLogPath();
	void LoadLocalConfig();
	//���ô���,���糬ʱ;���ڶ����ӣ����糤����
	int SetConnectFdTimeOut(int time_out_type = COMMON_TIMEOUT);
	int AddThreadTsk(st_thread_tsk_param& thread_tsk_param);//�߳���������

	void ParseTskResult(int result);
	void SetCommunicateWay();
	///////////////////////////////////////////////////////////////////////////////////
	//����ʹ��
	int FormatTmpData();
	void testXml();
	///////////////////////////////////////////////////////////////////////////////////
public:
	//�ؼ��ӿ�
	int InitDlg();//�ӶԻ�������->�Ի���
	void CreateChildDlg();//�����ӶԻ���
	void InitCombUart();//����������ֵ
	void ShowLoadComButton(bool show);//���ذ�ť�봮�ںŵ���ʾ
	void UpdataOuterDevTree();//��������豸����Ի���
	void CloseProgress();//�رս�����
	void HideChildPage();//������ҳ��
public:
	CTabCtrl m_tab;//��ҳ
	CButton mSaveComButton;//���浽�ɼ���
	CComboBox mCombUart;//���ں�����
	CStatic mLabelCom;//����ע��
	CButton mLoadComButton;//���زɼ���
	CButton mLoadLocalButton;//���ر���
	CButton mSaveLocalButton;//���汾��
	CButton mRestart;//����
	CButton mReadHistoryData;//����ʷ����
	CButton mDeleteHistoryData;//ɾ��ʷ����
	CButton mSaveHisData;//������ʷ����
	CButton mUpgradeVersion;//����
	CButton mCleanHisData;//�����ʷ����
	// ������ʷ���ݵ�u��
	CButton btCopyHisDataUDisk;
	// ������־��u��
	CButton btCopyLogUDisk;
	// �ָ���������
	CButton btBackFactory;
	// ��ʽ��SD��
	CButton btReSetSD;
public:
	bool bSetLocalTimer;//�Ƿ������˱���ʱ��Ķ�ʱ��
	CString localXml;//����xml·��.../xml.xml
	CString saveHisPath;//��ʷ���ݱ���·��
	CTaskParam* taskParma;//�߳�������
	CCriticalSection threadTskLock;//�߳�������
	st_thread_run_state taskThreadState;//�߳������Ƿ�������
	e_transfer_type transType;//ͨѶ��ʽ:���ڣ�����
	st_ip_port_domain netIpPort;//����ͨ�Ų���

public:
	CColtor* pColtor;//�ɼ����뽨����Ϣ
	COuterDev* pOuterDev;//����豸��Ϣ
	CService* pService;//��������������Ϣ
	COuterDevTreeShow* pOuterDevTreeShow;//����豸�����Ϣ
	CReportFormat* pReportFormat;//�ϱ���ʽ��Ϣ
	CParamDefModbusSetDlg* pCParamDefModbusSetDlg;//�Զ���Э��ģ��
	CHelpDocument* pCHelpDocument;//�����ĵ�
	CProgress* pProgress;//�߳�����Ľ�����
};
