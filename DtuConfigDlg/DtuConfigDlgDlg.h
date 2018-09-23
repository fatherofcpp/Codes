
// DtuConfigDlgDlg.h : 头文件
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


// CDtuConfigDlgDlg 对话框
class CDtuConfigDlgDlg : public CDialog
{
// 构造
public:
	CDtuConfigDlgDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDtuConfigDlgDlg();
// 对话框数据
	enum { IDD = IDD_DTUCONFIGDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//////////////////////////////////////////////////////////////////////
	//重载
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//消息响应
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
	//通讯接口 
	int GetDataFromCollector();
	int SetDataToCollector();

	//设置配置年龄
	int GetAge();
	int SetAge();

	//读-采集器基本配置
	int GetCollectorInf();
	int SetCollectorInf();

	//读-建筑配置
	int GetBuilInf();
	int SetBuilInf();

	//读-时间配置
	int GetTimeInf();
	int SetTimeInf();

	//读-接口设备配置
	int GetPortInf();
	int SetPortInf();

	int GetPortDevNum(int port_ind);
	int StartSetPortInf(int port_ind);
	int EndSetPortInf(int port_ind);

	//读-eth网配置
	int GetEthNetInf();
	int SetEthNetInf();
	//读热点配置：
	int GetHotSpotInf();
	int SetHotSpotInf();

	//读-服务器配置
	int GetServiceInf();
	int SetServiceInf();

	//上报格式配置:
	int GetReportFormatInf();
	int SetReportFormatInf();
	//设置立即生效某项
	int SetEffectNow(VEC_STRING& vec_name_string);
	//设置采集器基本配置，接口设备配置，服务器配置有效
	int SetReStart();

	//读自定义子类型
	int GetDefSubTypeInf();
	int SetDefSubTypeInf();
	int GetDefSubTypeNum(int& num);
	int StartSetDefSubTypeInf();
	int EndSetDefSubTypeInf();
	int GetDefSubTypeConf(int def_sub_type_ind, int& way_num);//返回回路数
	int SetDefSubTypeConf(int def_sub_type_ind, int order_ind);

	//写-清设备历史数据
	int SetCleanHisData();

	//导出历史数据到u盘
	int SetCopyHisDataUDisk(st_time& date_time);
	int GetMonthHisDataInf(st_time& date_time, VEC_TIME& data_vec);
	int CopyHisDataDayUDisk(st_time& date_time);

	//导出日志到u盘
	int SetCopyLogUDisk();

	//恢复出厂设置
	int SetBackFactory();

	//写 - 远程升级
	int SetUpgrad(char*data, int data_len);
	//写-开始远程升级
	int SetStartUpgrad(int pack_num, int pack_len_max);
	//写-远程升级数据
	int SetUpgradData(int pack_ind,char* data,int data_len);
	//写-结束远程升级
	int SetEndUpgrad();

	//心跳
	int SetHeartBeat();

	//格式化SD卡
	int SetResetSDCard();
	////////////////////////////////////////////////////////////////////
	//预留
	//写-开始串口上报
	int SetStartReport();
	//写-停止串口上报
	int SetEndReport();

	//读-历史数据
	int GetHistoryData(int pre_result);

	//保存-历史数据
	int SaveHistoryData();
	//写-开始保存历史数据
	int SetStartSaveHisData();
	//写-结束保存历史数据
	int SetEndSaveHisData();

	////////////////////////////////////////////////////////////////////
public:
	//逻辑接口
	void InitLogPath();
	void LoadLocalConfig();
	//设置串口,网络超时;串口短连接，网络长连接
	int SetConnectFdTimeOut(int time_out_type = COMMON_TIMEOUT);
	int AddThreadTsk(st_thread_tsk_param& thread_tsk_param);//线程运行任务

	void ParseTskResult(int result);
	void SetCommunicateWay();
	///////////////////////////////////////////////////////////////////////////////////
	//测试使用
	int FormatTmpData();
	void testXml();
	///////////////////////////////////////////////////////////////////////////////////
public:
	//控件接口
	int InitDlg();//子对话框数据->对话框
	void CreateChildDlg();//创建子对话框
	void InitCombUart();//串口下拉框赋值
	void ShowLoadComButton(bool show);//加载按钮与串口号的显示
	void UpdataOuterDevTree();//更新外接设备浏览对话框
	void CloseProgress();//关闭进度条
	void HideChildPage();//隐藏子页面
public:
	CTabCtrl m_tab;//分页
	CButton mSaveComButton;//保存到采集器
	CComboBox mCombUart;//串口号下拉
	CStatic mLabelCom;//串口注释
	CButton mLoadComButton;//加载采集器
	CButton mLoadLocalButton;//加载本地
	CButton mSaveLocalButton;//保存本地
	CButton mRestart;//重启
	CButton mReadHistoryData;//读历史数据
	CButton mDeleteHistoryData;//删历史数据
	CButton mSaveHisData;//保存历史数据
	CButton mUpgradeVersion;//升级
	CButton mCleanHisData;//清空历史数据
	// 拷贝历史数据到u盘
	CButton btCopyHisDataUDisk;
	// 拷贝日志到u盘
	CButton btCopyLogUDisk;
	// 恢复出厂设置
	CButton btBackFactory;
	// 格式化SD卡
	CButton btReSetSD;
public:
	bool bSetLocalTimer;//是否设置了本地时间的定时器
	CString localXml;//本地xml路径.../xml.xml
	CString saveHisPath;//历史数据保存路径
	CTaskParam* taskParma;//线程任务类
	CCriticalSection threadTskLock;//线程任务锁
	st_thread_run_state taskThreadState;//线程任务是否在运行
	e_transfer_type transType;//通讯方式:串口，网络
	st_ip_port_domain netIpPort;//网络通信参数

public:
	CColtor* pColtor;//采集器与建筑信息
	COuterDev* pOuterDev;//外接设备信息
	CService* pService;//服务器与网络信息
	COuterDevTreeShow* pOuterDevTreeShow;//外接设备浏览信息
	CReportFormat* pReportFormat;//上报格式信息
	CParamDefModbusSetDlg* pCParamDefModbusSetDlg;//自定义协议模板
	CHelpDocument* pCHelpDocument;//帮助文档
	CProgress* pProgress;//线程任务的进度条
};
