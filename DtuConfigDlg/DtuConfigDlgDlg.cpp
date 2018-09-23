
// DtuConfigDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "DtuConfigDlgDlg.h"
#include <register.h>
#include "dataConvert.h"
#include "CommonFunction.h"
#include "FileReadWrite.h"
#include "HistoryData.h"
#include "RestartRem.h"
#include "PassWordDlg.h"

static char comMap[32][16] = {};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USETHREADTSK 1
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CDtuConfigDlgDlg 对话框

CDtuConfigDlgDlg::CDtuConfigDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDtuConfigDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pColtor = new CColtor;
	pOuterDev = new COuterDev;
	pService = new CService;
	pOuterDevTreeShow = new COuterDevTreeShow;
	pProgress = new CProgress;
	pReportFormat = new CReportFormat;
	pCHelpDocument = new CHelpDocument;
	pCParamDefModbusSetDlg = new CParamDefModbusSetDlg;

	bSetLocalTimer = false;

	localXml = "";
	saveHisPath = "";

	taskParma = NULL;
	transType = UART_TRANSF;
	memset(&taskThreadState, 0, sizeof(taskThreadState));

	memset(&netIpPort, 0, sizeof(netIpPort));
}

void CDtuConfigDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_BUTTON_SAVE_COM, mSaveComButton);
	DDX_Control(pDX, IDC_COMBO_COM, mCombUart);
	DDX_Control(pDX, IDC_COM, mLabelCom);
	DDX_Control(pDX, IDC_BUTTON_LOAD_COM, mLoadComButton);
	DDX_Control(pDX, IDC_BUTTON_LOAD_LOCAL, mLoadLocalButton);
	DDX_Control(pDX, IDC_BUTTON_SAVE_LOCAL, mSaveLocalButton);
	DDX_Control(pDX, IDC_BUTTON_RESTART, mRestart);
	DDX_Control(pDX, IDC_BUTTON_HISTORYDATA, mReadHistoryData);
	DDX_Control(pDX, IDC_CHECK_DELETE_HISTORYDATA, mDeleteHistoryData);
	DDX_Control(pDX, IDC_BUTTON_SAVE_HISDATA, mSaveHisData);
	DDX_Control(pDX, IDC_BUTTON_UPGRAD, mUpgradeVersion);
	DDX_Control(pDX, IDC_BUTTON_CLEANHISDSTA, mCleanHisData);
	DDX_Control(pDX, IDC_BUTTON_COPY_HISDATA_UDISK, btCopyHisDataUDisk);
	DDX_Control(pDX, IDC_BUTTON_COPY_LOG_UDISK, btCopyLogUDisk);
	DDX_Control(pDX, IDC_BUTTON_BACK_FACTORY, btBackFactory);
	DDX_Control(pDX, IDC_BUTTON_SD_RESET, btReSetSD);
}

BEGIN_MESSAGE_MAP(CDtuConfigDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDtuConfigDlgDlg::OnTcnSelchangeTab)

	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BUTTON_SAVE_COM, &CDtuConfigDlgDlg::OnBnClickedButtonSaveCom)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_COM, &CDtuConfigDlgDlg::OnBnClickedButtonLoadCom)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LOCAL, &CDtuConfigDlgDlg::OnBnClickedButtonLoadLocal)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LOCAL, &CDtuConfigDlgDlg::OnBnClickedButtonSaveLocal)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CDtuConfigDlgDlg::OnBnClickedButtonRestart)
	ON_BN_CLICKED(IDC_BUTTON_UPGRAD, &CDtuConfigDlgDlg::OnBnClickedButtonUpgrad)
	ON_BN_CLICKED(IDC_BUTTON_HISTORYDATA, &CDtuConfigDlgDlg::OnBnClickedButtonHistorydata)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_HISDATA, &CDtuConfigDlgDlg::OnBnClickedButtonSaveHisdata)
	ON_BN_CLICKED(IDC_BUTTON_CLEANHISDSTA, &CDtuConfigDlgDlg::OnBnClickedButtonCleanhisdsta)

	ON_BN_CLICKED(IDC_BUTTON_COPY_HISDATA_UDISK, &CDtuConfigDlgDlg::OnBnClickedButtonCopyHisdataUdisk)
	ON_BN_CLICKED(IDC_BUTTON_COPY_LOG_UDISK, &CDtuConfigDlgDlg::OnBnClickedButtonCopyLogUdisk)
	ON_BN_CLICKED(IDC_BUTTON_BACK_FACTORY, &CDtuConfigDlgDlg::OnBnClickedButtonBackFactory)
	ON_BN_CLICKED(IDC_BUTTON_NET_BROAD, &CDtuConfigDlgDlg::OnBnClickedButtonNetBroad)
	ON_BN_CLICKED(IDC_BUTTON_SD_RESET, &CDtuConfigDlgDlg::OnBnClickedButtonSdReset)
END_MESSAGE_MAP()

UINT __cdecl HeartBeatThread(void *p)
{
	return 0;
	//CDtuConfigDlgDlg* p_this = (CDtuConfigDlgDlg*)p;
	//int ret = 0;
	//CString str;
	//int sleep_tm = 2 * 1000;
	//int heart_beat_count = 0;
	//while (1)
	//{
	//	ret = p_this->SetHeartBeat();
	//	if (ret != 0)
	//	{
	//		str.Format("[%s]心跳失败\n", __FUNCTION__);
	//		TRACE(str);
	//	}
	//	else
	//	{
	//		str.Format("[%s]心跳%d\n", __FUNCTION__, heart_beat_count++);
	//		TRACE(str);
	//	}
	//	Sleep(sleep_tm);
	//}
	//return 0;
}

UINT __cdecl TaskThread(void *p)
{
	CDtuConfigDlgDlg* p_this = (CDtuConfigDlgDlg*)p;
	int ret = 0;
	CTaskParam* data_frame = NULL;
	CString str;

	p_this->taskThreadState.is_run = true;
	while (1)
	{
		Sleep(10);
		if (p_this->taskThreadState.is_kill)
		{
			p_this->taskThreadState.is_run = false;
			TRACE("[%s] thread exit\n", __FUNCTION__);
			return 0;
		}

		if (p_this->taskParma != NULL)
		{
			data_frame = p_this->taskParma;
			if (data_frame->tskEnd)
			{
				continue;
			}
			switch (data_frame->threadParam.type_thread)
			{
			case UPGRAD_THREAD:
				ret = p_this->SetUpgrad((char*)data_frame->threadParam.file_data, data_frame->threadParam.data_len);
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;

			case LOAD_COLTOR_THREAD:
#if 0
				ret = pThis->FormatTmpData();
#else
				ret = p_this->GetDataFromCollector();
#endif
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();

				break;
			case SAVE_COLTOR_THREAD:
				ret = p_this->SetDataToCollector();
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;
			case HISTORY_DATA_THREAD:
				ret = p_this->GetHistoryData(data_frame->threadParam.pre_result);
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;

			case SAVE_HISTORY_DATA_THREAD:
				ret = p_this->SaveHistoryData();
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;

			case CLEAN_HISDATA_THREAD:
				ret = p_this->SetCleanHisData();
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;

			case COPY_HISDATA_UDISK_THREAD:
				ret = p_this->SetCopyHisDataUDisk(data_frame->threadParam.date_tsk);
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;

			case COPY_LOG_UDISK_THREAD:
				ret = p_this->SetCopyLogUDisk();
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;
			case BACK_FACTORY_THREAD:
				ret = p_this->SetBackFactory();
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;
			case WAIT_TIME_THREAD:
				Sleep(data_frame->threadParam.date_tsk.SS * 1000);
				ret = 0; 
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;
			case RESET_SDCARD_THREAD:
				ret = p_this->SetResetSDCard();
				data_frame->tskResult = ret;
				data_frame->tskEnd = true;
				p_this->CloseProgress();
				break;
				//add
			default:
				break;
			}
		}

	}

	TRACE("[%s] thread exit\n", __FUNCTION__);
	return 0;
}

// CDtuConfigDlgDlg 消息处理程序

BOOL CDtuConfigDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	int ret = 0;
	CString str;

	//init path
	InitLogPath();

	//load config if exist
	LoadLocalConfig();

	//create child dlg
	CreateChildDlg();

	//选择通信方式:串口，网络
	//transType = NET_TRANSF;
	SetCommunicateWay();

	//test
	//end test

	AfxBeginThread( TaskThread,this,THREAD_PRIORITY_ABOVE_NORMAL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

int CDtuConfigDlgDlg::GetDataFromCollector()
{
	ZeroMemory(gXmlAnalyze.pColtorInf,sizeof(*gXmlAnalyze.pColtorInf));
	ZeroMemory(gXmlAnalyze.pServiceInf,sizeof(*gXmlAnalyze.pServiceInf));
	gXmlAnalyze.pCProtModbusSet->ClearDefModbusArr();
	ZeroMemory(gXmlAnalyze.pOuterDev,sizeof(*gXmlAnalyze.pOuterDev));
	ZeroMemory(gXmlAnalyze.pReportFormatStoreInf,sizeof(*gXmlAnalyze.pReportFormatStoreInf));

	pColtor->SetColtorInf(gXmlAnalyze.pColtorInf);
	pService->SetServiceInf(gXmlAnalyze.pServiceInf);
	pCParamDefModbusSetDlg->SetModbusDefArray(gXmlAnalyze.pCProtModbusSet);
	pOuterDev->SetOuterDev((void*)(gXmlAnalyze.pOuterDev));
	pReportFormat->SetReportFormat(gXmlAnalyze.pReportFormatStoreInf);
	gXmlAnalyze.SavePreOuterDev();

	int rtn = 0;
	int sleep_m = 5;

	rtn = GetAge();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetAge fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetBuilInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetBuilInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetCollectorInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetCollectorInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetTimeInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetTimeInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetEthNetInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetEthNetInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetHotSpotInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetHotSpotInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetDefSubTypeInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetDefSubTypeInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetServiceInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetServiceInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetReportFormatInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetReportFormatInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	rtn = GetPortInf();
	if(rtn != 0)
	{
		gLog.Add("[%s]GetDevInf fail ret=%d", __FUNCTION__, rtn);
		return rtn;
	}
	Sleep(sleep_m);

	pColtor->SetColtorInf(gXmlAnalyze.pColtorInf);
	pService->SetServiceInf(gXmlAnalyze.pServiceInf);
	pCParamDefModbusSetDlg->SetModbusDefArray(gXmlAnalyze.pCProtModbusSet);
	pOuterDev->SetOuterDev((void*)(gXmlAnalyze.pOuterDev));
	pReportFormat->SetReportFormat(gXmlAnalyze.pReportFormatStoreInf);

	gXmlAnalyze.SavePreOuterDev();

	return 0;
}

int CDtuConfigDlgDlg::SetDataToCollector()
{
	int rtn = 0;
	int sleep_m = 5;
	if(pColtor->pColtorInf->dev_time.is_modify)
	{
		rtn = SetTimeInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetTimeInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pColtor->pColtorInf->build_conf.is_modify)
	{
		rtn = SetBuilInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetBuilInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pColtor->pColtorInf->col_conf.is_modify)
	{
		rtn = SetCollectorInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetCollectorInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pCParamDefModbusSetDlg->GetIsModify())
	{
		rtn = SetDefSubTypeInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetDefSubTypeInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pOuterDev->modify)
	{
		rtn = SetPortInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetDevInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pService->pNetConf->coltor_net.eth.is_modify)
	{
		rtn = SetEthNetInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetEthNetInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pService->pNetConf->coltor_net.hotspot.is_modify)
	{
		rtn = SetHotSpotInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetHotSpotInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pService->pNetConf->service_net.is_modify)
	{
		rtn = SetServiceInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetServiceInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if(pReportFormat->pReportFormatStore->report_format.is_modify)
	{
		rtn = SetReportFormatInf();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetReportFormatInf fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	if (pColtor->pColtorInf->dev_time.is_modify ||
		pColtor->pColtorInf->col_conf.is_modify ||
		pColtor->pColtorInf->build_conf.is_modify ||
		pCParamDefModbusSetDlg->GetIsModify() ||
		pOuterDev->modify ||
		pService->pNetConf->coltor_net.eth.is_modify ||
		pService->pNetConf->coltor_net.hotspot.is_modify ||
		pService->pNetConf->service_net.is_modify ||
		pReportFormat->pReportFormatStore->report_format.is_modify
		)
	{
		rtn = GetAge();
		if(rtn != 0)
		{
			gLog.Add("[%s]GetAge fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);

		rtn = SetAge();
		if(rtn != 0)
		{
			gLog.Add("[%s]SetAge fail ret=%d", __FUNCTION__, rtn);
			return rtn;
		}
		Sleep(sleep_m);
	}

	return 0;
}

void CDtuConfigDlgDlg::CreateChildDlg()
{
	//create child dlg
	int page = 0;

	m_tab.InsertItem(page ++, "建筑与采集器信息");
	m_tab.InsertItem(page ++, "服务器与网络信息");
	m_tab.InsertItem(page ++, "自定义子类型");
	m_tab.InsertItem(page ++, "外接设备信息");
	m_tab.InsertItem(page ++, "上报格式定义");
	m_tab.InsertItem(page ++, "外接设备连接关系");
	m_tab.InsertItem(page ++, "帮助文档");
	m_tab.SetCurSel(0);

	pColtor->Create(IDD_DIALOG_COLLECTOR, this);
	pOuterDev->Create(IDD_DIALOG_DEV, this);
	pService->Create(IDD_DIALOG_SERVICE, this);
	pReportFormat->Create(IDD_DIALOG_REPORT_FORMAT, this);
	pOuterDevTreeShow->Create(IDD_DIALOG_TREE_SHOW, this);
	pCParamDefModbusSetDlg->Create(IDD_DIALOG_PROTO_TEMPLATE, this);
	pCHelpDocument->Create(IDD_DIALOG_HELP_DOCUMENT, this);
	
	CRect rs;
	m_tab.GetClientRect(&rs);

	rs.top += 35;//tested
	//rs.bottom -= 10;
	rs.left += 13;//tested
	//rs.right -= 40;

	pColtor->MoveWindow(&rs);
	pOuterDev->MoveWindow(&rs);
	pService->MoveWindow(&rs);
	pReportFormat->MoveWindow(&rs);
	pOuterDevTreeShow->MoveWindow(&rs);
	pCParamDefModbusSetDlg->MoveWindow(&rs);
	pCHelpDocument->MoveWindow(&rs);

	HideChildPage();
	pColtor->ShowWindow(true);
}

int CDtuConfigDlgDlg::InitDlg()
{
	//采集器信息
	pColtor->InitDlg();

	//外设信息
	pOuterDev->SetParentPointer(this);
	pOuterDev->InitDlg();

	//服务器信息，初始化下拉列表必须在子窗口创建之后
	pService->InitDlg();

	//上报格式
	pReportFormat->InitDlg();

	//协议模板
	pCParamDefModbusSetDlg->InitDlg();

	//帮助文档
	pCHelpDocument->InitDlg();

	return 0;
}


void CDtuConfigDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDtuConfigDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDtuConfigDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDtuConfigDlgDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	str_outer_dev* pouter_dev = new str_outer_dev;

	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
	case 0://采集器信息
		HideChildPage();
		pColtor->ShowWindow(true);
		break;
	case 1://服务器信息
		HideChildPage();
		pService->ShowWindow(true);
		break;
	case 2://自定义子类型信息 
		HideChildPage();
		//更新外接设备到自定义子类型信息
		pOuterDev->SaveDlg();
		pouter_dev->port_num = pOuterDev->portNum;
		for(int i = 0; i < pouter_dev->port_num; i++)
		{
			memcpy(&pouter_dev->port_dev[i], pOuterDev->pPort[i]->pPortDev, sizeof(pouter_dev->port_dev[i]));
		}
		pCParamDefModbusSetDlg->SetOuterDev(pouter_dev);	
		pCParamDefModbusSetDlg->ShowWindow(true);
		break;
	case 3://外接设备信息
		HideChildPage();
		//更新自定义子类型信息到外接设备的回路数
		pCParamDefModbusSetDlg->SaveDlg();
		pOuterDev->SetModbusDefArray(pCParamDefModbusSetDlg->pCProtModbusSet);
		pOuterDev->ShowWindow(true);
		break;
	case 4://所有主类型上报参量信息
		HideChildPage();
		pReportFormat->ShowWindow(true);
		break;

	case 5://外接设备预览
		HideChildPage();
		//更新自定义子类型信息到外接设备的回路
		pCParamDefModbusSetDlg->SaveDlg();
		UpdataOuterDevTree();
		pOuterDevTreeShow->ShowWindow(true);
		break;

	case 6://帮助文档
		HideChildPage();
		pCHelpDocument->ShowWindow(true);
		break;
	default:
		break;
	} 
	delete pouter_dev;
	return;
}

CDtuConfigDlgDlg::~CDtuConfigDlgDlg()
{
	delete pColtor;
	delete pOuterDev;
	delete pService;
	delete pOuterDevTreeShow;
	delete pProgress;
	delete pReportFormat;
	delete pCParamDefModbusSetDlg;
	delete pCHelpDocument;
}

void CDtuConfigDlgDlg::OnBnClickedButtonSaveCom()
{

	CString str;
	int ret = 0;
	ret = SetConnectFdTimeOut();
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	ret = pColtor->SaveDlg();
	if(ret != 0)
	{
		return;
	}

	ret = pCParamDefModbusSetDlg->SaveDlg();
	if(ret != 0)
	{
		return;
	}

	//此处仅为保存修改状态
	pOuterDev->SaveDlg();

	//外设信息
	str_outer_dev* pouter_dev = new str_outer_dev;
	memset(pouter_dev, 0, sizeof(*pouter_dev));
	pouter_dev->port_num = pOuterDev->portNum;
	for(int j = 0; j < pouter_dev->port_num; j++)
	{
		memcpy(&pouter_dev->port_dev[j], pOuterDev->pPort[j]->pPortDev, sizeof(pouter_dev->port_dev[j]));
	}

	ret = pService->SaveDlg();	
	if(ret != 0)
	{
		return;
	}

	ret = pReportFormat->SaveDlg();
	if(ret != 0)
	{
		return;
	}

	gXmlAnalyze.SetColtorInf(pColtor->pColtorInf);
	gXmlAnalyze.SetCParamDevDefModbusSet(pCParamDefModbusSetDlg->pCProtModbusSet);
	gXmlAnalyze.SetOuterDev(pouter_dev);
	delete pouter_dev;
	gXmlAnalyze.SetServiceInf(pService->pNetConf);
	gXmlAnalyze.SetReportFormatInf(pReportFormat->pReportFormatStore);

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = SAVE_COLTOR_THREAD;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);

	CRestartRem* restart_rem = new CRestartRem;
	ret = restart_rem->DoModal();
	if(IDOK == ret)
	{
		OnBnClickedButtonRestart();
	}
	else
	{
		st_thread_tsk_param thread_tsk_param;
		memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
		thread_tsk_param.type_thread = LOAD_COLTOR_THREAD;
		ret = AddThreadTsk(thread_tsk_param);
		ParseTskResult(ret);
	}
	delete restart_rem;

}

void CDtuConfigDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == LOCAL_TIME_ID)
	{
		pColtor->UpdataDevTime();
	}

	CDialog::OnTimer(nIDEvent);
}

int CDtuConfigDlgDlg::GetCollectorInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetCollectorInfCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, READ_COLLECTOR_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetCollectorInfRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::GetTimeInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetTimeInfCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,READ_TIME_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetTimeInfRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::GetPortInf()
{
	int rtn = 0;
	int len = 0;

	for(int i = 0; i < gXmlAnalyze.pOuterDev->port_num; i++)
	{
		//读接口设备数
		rtn = GetPortDevNum(i);
		if (rtn != 0)
		{
			gLog.Add("[%s]Parse GetPortDevNum fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return -1;
		}

		for(int j = 0; j < gXmlAnalyze.pOuterDev->port_dev[i].dev_num; j++)
		{
			len = gXmlAnalyze.FormGetPortDevCmd(i, j);
			rtn = gTransf.SendReqAndReceiveBody(len, READ_PORT_DEV_CMD);
			if(rtn == 0)
			{
				rtn = gXmlAnalyze.ParseGetPortDevRsp(i, j);
				if(rtn == 0)
				{
					continue;
				}
				else
				{
					gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
					return rtn;
				}
			}
			else
			{
				return rtn;
			}
		}

	}
	return 0;
}

int CDtuConfigDlgDlg::GetServiceInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetServInfCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,READ_SERVICE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetServInfRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetCollectorInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetCollectorInfCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_COLLECTOR_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetCollectorInfRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetTimeInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetTimeInfCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_TIME_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetTimeInfRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetPortInf()
{
	int rtn = 0;
	int len = 0;

	for(int i = 0; i < gXmlAnalyze.pOuterDev->port_num; i++)
	{
		//开始
		rtn = StartSetPortInf(i);
		if (rtn != 0)
		{
			return -12;
		}

		for (int j = 0; j < gXmlAnalyze.pOuterDev->port_dev[i].dev_num; j++)
		{
			len = gXmlAnalyze.FormSetPortDevCmd(i, j);
			rtn = gTransf.SendReqAndReceiveBody(len,SET_PORT_DEV_CMD);
			if(rtn == 0)
			{
				rtn = gXmlAnalyze.ParseSetPortDevRsp();
				if(rtn == 0)
				{
					continue;
				}
				else
				{
					gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
					return rtn;
				}
			}
			else
			{
				return rtn;
			}
		}

		//结束
		rtn = EndSetPortInf(i);
		if (rtn != 0)
		{
			return -13;
		}
	}
	return 0;
}


int CDtuConfigDlgDlg::SetServiceInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetServInfCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_SERVICE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetServInfRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::FormatTmpData()
{
	//采集器信息
	st_coltor_inf coltorInf;
	char name[MAX_NAME_LEN];
	int ret = 0;
	ZeroMemory(name,sizeof(name));
	memset(&coltorInf, 0, sizeof(coltorInf));

	coltorInf.col_conf.id = 23;
	sprintf(name,"%s","名称name");
	ret = gb2312_to_utf8(name, strlen(name),coltorInf.col_conf.name);

	sprintf(name,"%s","基站name");
	ret = gb2312_to_utf8(name, strlen(name),coltorInf.build_conf.build_pos.pos_name);
	coltorInf.build_conf.build_code = 11111;
	coltorInf.build_conf.build_pos.province = 130000;
	coltorInf.build_conf.build_pos.city = 130100;
	coltorInf.build_conf.build_pos.county = 130101;

	CTime tm; 
	tm = CTime::GetCurrentTime();//获得当前时间 
	coltorInf.dev_time.is_modify = false;

	coltorInf.dev_time.dev_time.YY = tm.GetYear();
	coltorInf.dev_time.dev_time.MM= tm.GetMonth();
	coltorInf.dev_time.dev_time.DD= tm.GetDay(); 
	coltorInf.dev_time.dev_time.HH= tm.GetHour();
	coltorInf.dev_time.dev_time.MI= tm.GetMinute();
	coltorInf.dev_time.dev_time.SS= tm.GetSecond();

	coltorInf.dev_time.local_time = GetTickCount();

	coltorInf.col_conf.sample_m = 5;
	coltorInf.col_conf.heart_s = 30;

	gXmlAnalyze.SetColtorInf(&coltorInf);

	//服务器信息
	st_net_conf serviceInf;
	memset(&serviceInf, 0, sizeof(serviceInf));

	int serviceNum = 3;
	for(int i = 0; i < serviceNum; i++)
	{
		sprintf(serviceInf.service_net.ip_port_domain[i].domain,"192.168.1.%d",i);
		serviceInf.service_net.ip_port_domain[i].port = 1110 + i;
		serviceInf.service_net.ip_port_domain[i].valid = true;
	}
	sprintf(serviceInf.service_net.secret, "0123456789abcdef");

	sprintf(serviceInf.coltor_net.eth.ip,"192.168.1.1");
	serviceInf.coltor_net.eth.netType = STATIC;
	serviceInf.coltor_net.eth.sysType = RTOS;

	gXmlAnalyze.SetServiceInf(&serviceInf);

	//自定义协议信息
	CProtModbusSet* def_subtype = new CProtModbusSet;
	gXmlAnalyze.SetCParamDevDefModbusSet(def_subtype);

	//外设信息
	str_outer_dev* pouter_dev = new str_outer_dev;
	memset(pouter_dev, 0, sizeof(*pouter_dev));
	pouter_dev->port_num = 8;
	for(int j = 0; j < pouter_dev->port_num; j++)
	{
		pouter_dev->port_dev[j].dev_num = 32;
		for(int i = 0; i < pouter_dev->port_dev[j].dev_num; i++)
		{
			pouter_dev->port_dev[j].dev_inf[i].dev_addr.id = i + j;
			sprintf(name,"外D[%d]P[%d]",i,j);
			ret = gb2312_to_utf8(name, strlen(name),pouter_dev->port_dev[j].dev_inf[i].name);
			pouter_dev->port_dev[j].dev_inf[i].sub_type = AM_3PHASE_01;
			pouter_dev->port_dev[j].dev_inf[i].way_inf.way_mask = 3;
			sprintf(pouter_dev->port_dev[j].dev_inf[i].way_inf.way_data[0].way_name,"wayName-0");
			sprintf(pouter_dev->port_dev[j].dev_inf[i].way_inf.way_data[1].way_name,"wayName-1");
			pouter_dev->port_dev[j].dev_inf[i].rs485_conf.baud = B9600;
			pouter_dev->port_dev[j].dev_inf[i].rs485_conf.pari = ODD;
		}
	}
	gXmlAnalyze.SetOuterDev(pouter_dev);
	delete pouter_dev;


	//上报格式信息
	st_report_store_inf report_format;
	memset(&report_format, 0, sizeof(report_format));
	gXmlAnalyze.SetReportFormatInf(&report_format);

	pColtor->SetColtorInf(gXmlAnalyze.pColtorInf);
	pService->SetServiceInf(gXmlAnalyze.pServiceInf);
	pCParamDefModbusSetDlg->SetModbusDefArray(gXmlAnalyze.pCProtModbusSet);
	pOuterDev->SetOuterDev((void*)(gXmlAnalyze.pOuterDev));
	pReportFormat->SetReportFormat(gXmlAnalyze.pReportFormatStoreInf);
	gXmlAnalyze.SavePreOuterDev();
	return 0;
}

void CDtuConfigDlgDlg::testXml()
{
	/*
	<resp>
	<d_type>1</d_type>
	<![CDATA[xxxxxx]]>
	</resp>
	*/
	int ret = 0;
	u_w_measure w_measure;
	ZeroMemory(&w_measure,sizeof(w_measure));
	gXmlAnalyze.ClearBuff();
	int tmpLen = 0;//消息体起始位置

	tmpLen += sprintf(gXmlAnalyze.pBuff + tmpLen, "<resp>\r\n");
	//tmpLen += sprintf(gXmlAnalyze.m_pBuff + tmpLen, "<d_type>%s</d_type>\r\n","ty pe");
	tmpLen += sprintf(gXmlAnalyze.pBuff + tmpLen, "<![CDATA[%s]]>\r\n","123");
	tmpLen += sprintf(gXmlAnalyze.pBuff + tmpLen, "</resp>\r\n");
	e_dev_type type = AM_1PHASE;
	ret = gXmlAnalyze.ParseGetRealTimeDataRsp(w_measure,type);

	st_col_conf base_inf;
	memset(&base_inf,0,sizeof(base_inf));
	//net_pro_parser_resp_4_read_col_conf(gXmlAnalyze.m_pBuff,&base_inf);
}

int CDtuConfigDlgDlg::SetReStart()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetReStartCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_REBOOT_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetReStartRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetAge()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetAgeCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_AGE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetAgeRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

void CDtuConfigDlgDlg::OnBnClickedButtonLoadCom()
{
	int ret = 0;
	CString str;

	ret = SetConnectFdTimeOut();
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = LOAD_COLTOR_THREAD;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);
}

void CDtuConfigDlgDlg::InitCombUart()
{
	CString str;

	int ret = 0;
	int ind = 0;
	BOOL is_get = false;
	CArray<CString,CString> com_array;//串口集合 
	is_get = GetSystemSerialComport(com_array);
	if (is_get)
	{
		gComNum = com_array.GetSize();
		for (int i = 0; i < gComNum; i++)
		{
			str = com_array.ElementAt(0);
			com_array.RemoveAt(0);
			sprintf(comMap[i],"%s",str.GetString());
		}
	}

	for (int i = 0; i < gComNum; i++)
	{
		str.Format("%s",comMap[i]);
		mCombUart.InsertString(i,str);
	}

#if 0
	char com_name[32] = "";
	ret = GetComFrmReg(com_name);
	if (ret == 0)
	{
		str = com_name;
	}
	else
	{
		gLog.Add("[%s]GetComFrmReg fail ret=%d,use default value", __FUNCTION__, ret);
	}

	for (int i = 0; i < gComNum; i++)
	{
		if(str == comMap[i])
		{
			ind = i;
		}
	}
#endif
	mCombUart.SetCurSel(ind);

	if (gComNum == 0)
	{
		/*mLoadComButton.EnableWindow(FALSE);
		mSaveComButton.EnableWindow(FALSE);
		mRestart.EnableWindow(FALSE);
		mSaveHisData.EnableWindow(FALSE);
		mUpgradeVersion.EnableWindow(FALSE);
		mCleanHisData.EnableWindow(FALSE);
		mReadHistoryData.EnableWindow(FALSE);
		mDeleteHistoryData.EnableWindow(FALSE);
		btCopyHisDataUDisk.EnableWindow(FALSE);
		btCopyLogUDisk.EnableWindow(FALSE);
		btBackFactory.EnableWindow(FALSE);*/
	}

}

int CDtuConfigDlgDlg::SetConnectFdTimeOut(int time_out_type)
{
	int ret = 0;
	int time_out_m = 0;
	switch(time_out_type)
	{
	case COMMON_TIMEOUT:
		time_out_m = 1*1000;//60*1000
		break;
	case CLEAN_HISTORY_DATA_TIMEOUT:
		time_out_m = 60*1000;//60*1000
		break;
	case REALTIME_DATA_TIMEOUT:
		time_out_m = 24*1000;//60*1000
		break;
	case TEST_TIMEOUT:
		time_out_m = 3*1000;//60*1000
		break;
	case COPY_HIS_DATA_UDISK_TIMEOUT:
		time_out_m = 3*60*1000;//60*1000
		break;
	case COPY_LOG_UDISK_TIMEOUT:
		time_out_m = 30*1000;//60*1000
		break;
	case BACK_FACTORY_TIMEOUT:
		time_out_m = 60*1000;//60*1000
		break;
	case ADAPT_DEV_TIMEOUT:
		time_out_m = 15*1000;//60*1000
		break;
	case RESET_SDCARD_TIMEOUT:
		time_out_m = 4*60*1000;//60*1000
		break;
	default:
		time_out_m = 16*1000;//60*1000
		break;
	}

	if (transType == UART_TRANSF)
	{
		int uart_ind = 0;
		uart_ind = mCombUart.GetCurSel();
		if (uart_ind < 0)
		{
			return -1;
		}
		CString str_uart_name;
		str_uart_name = comMap[uart_ind];
		//SetComToReg(str_uart_name.GetString());

		st_serial_conf serial;
		memset(&serial, 0, sizeof(serial));
		serial.buff_size = 4096;//4096
		serial.byte_size = 8;//8
		serial.baud = 115200;//115200,9600
		serial.parity = 0;

		sprintf(serial.com_name,"%s",str_uart_name);
		serial.read_timeout_m = time_out_m;//60*1000
		serial.write_timeout_m = 8*1000;//8*1000

		gTransf.SetSerialConf(&serial);
		return ret;
	}
	else if(transType == NET_TRANSF)
	{
		int net_time_out_add = 5 * 1000;//网络传输，增加超时时间
		time_out_m += net_time_out_add ;
		struct timeval time_out;
		time_out.tv_sec = time_out_m / 1000;
		time_out.tv_usec = (time_out_m % 1000) * 1000;

		gTransf.SetReadTimeOut(time_out);
		return 0;
	}
	else
	{
		return -1;
	}

}

void CDtuConfigDlgDlg::ShowLoadComButton( bool show )
{
	mLabelCom.EnableWindow(show);
	mCombUart.EnableWindow(show);
}

HMODULE   GetCurrentModule() 
{    
	MEMORY_BASIC_INFORMATION   mbi; 
	static   int   dummy; 
	VirtualQuery(   &dummy,   &mbi,   sizeof(mbi)   ); 
	return   reinterpret_cast <HMODULE> (mbi.AllocationBase); 
}

void CDtuConfigDlgDlg::InitLogPath()
{
	CString this_dir;
	CString logName;
	HMODULE hmodule = GetCurrentModule();
	DWORD module_file = GetModuleFileName(hmodule, this_dir.GetBufferSetLength(1000+1), 1000); 
	assert(module_file);
	int left_end = this_dir.ReverseFind(L'\\');
	this_dir = this_dir.Left(left_end);
	gCurrentDir = this_dir;
	logName = this_dir + "\\saveLog.log";
	//init log
	gLog.InitLog(logName);
}


void CDtuConfigDlgDlg::OnBnClickedButtonLoadLocal()
{
	int ret = 0;
	CString str;

	ZeroMemory(gXmlAnalyze.pColtorInf,sizeof(*gXmlAnalyze.pColtorInf));
	gXmlAnalyze.pCProtModbusSet->ClearDefModbusArr();
	ZeroMemory(gXmlAnalyze.pOuterDev,sizeof(*gXmlAnalyze.pOuterDev));
	ZeroMemory(gXmlAnalyze.pServiceInf,sizeof(*gXmlAnalyze.pServiceInf));
	ZeroMemory(gXmlAnalyze.pReportFormatStoreInf,sizeof(*gXmlAnalyze.pReportFormatStoreInf));
	pColtor->SetColtorInf(gXmlAnalyze.pColtorInf);
	pCParamDefModbusSetDlg->SetModbusDefArray(gXmlAnalyze.pCProtModbusSet);
	pOuterDev->SetOuterDev((void*)(gXmlAnalyze.pOuterDev));
	pService->SetServiceInf(gXmlAnalyze.pServiceInf);
	pReportFormat->SetReportFormat(gXmlAnalyze.pReportFormatStoreInf);
	gXmlAnalyze.SavePreOuterDev();


	localXml = BootOpenFile();
	ret = gXmlAnalyze.LoadLocalXml(localXml.GetString());
	if(ret == 0)
	{
		gXmlAnalyze.pColtorInf->dev_time.local_time = GetTickCount();

		pColtor->SetColtorInf(gXmlAnalyze.pColtorInf);
		pCParamDefModbusSetDlg->SetModbusDefArray(gXmlAnalyze.pCProtModbusSet);
		pOuterDev->SetOuterDev((void*)(gXmlAnalyze.pOuterDev));
		pService->SetServiceInf(gXmlAnalyze.pServiceInf);
		pReportFormat->SetReportFormat(gXmlAnalyze.pReportFormatStoreInf);


		pColtor->pColtorInf->dev_time.is_modify = true;
		pColtor->pColtorInf->build_conf.is_modify = true;
		pColtor->pColtorInf->col_conf.is_modify = true;
		pReportFormat->pReportFormatStore->report_format.is_modify = true;
		pOuterDev->modify = true;
		pService->pNetConf->coltor_net.eth.is_modify = true;
		pService->pNetConf->coltor_net.hotspot.is_modify = true;
		pService->pNetConf->service_net.is_modify = true;
		pCParamDefModbusSetDlg->isModify = true;

		if (!bSetLocalTimer)
		{
			SetTimer(LOCAL_TIME_ID,1000,NULL);
			bSetLocalTimer = true;
		}
		InitDlg();
	}
	else
	{
		str.Format("[%s]加载本地配置失败,ret=%d",__FUNCTION__, ret);
		MessageBox("加载本地配置失败,请重新加载采集器配置或者本地配置");
		gLog.Add(str);
	}
}


void CDtuConfigDlgDlg::OnBnClickedButtonSaveLocal()
{
	int ret = 0;
	CString str;

	//save dlg
	ret = pColtor->SaveDlg();
	if(ret != 0)
	{
		return;
	}

	ret = pCParamDefModbusSetDlg->SaveDlg();
	if(ret != 0)
	{
		return;
	}

	pOuterDev->SaveDlg();
	ret = pService->SaveDlg();	
	if(ret != 0)
	{
		return;
	}

	ret = pReportFormat->SaveDlg();	
	if(ret != 0)
	{
		return;
	}

	gXmlAnalyze.SetColtorInf(pColtor->pColtorInf);
	gXmlAnalyze.SetCParamDevDefModbusSet(pCParamDefModbusSetDlg->pCProtModbusSet);

	str_outer_dev* pouter_dev = new str_outer_dev;
	memset(pouter_dev, 0, sizeof(*pouter_dev));
	pouter_dev->port_num = pOuterDev->portNum;
	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		memcpy(&pouter_dev->port_dev[i],pOuterDev->pPort[i]->pPortDev,sizeof(pouter_dev->port_dev[i]));
	}
	gXmlAnalyze.SetOuterDev(pouter_dev);
	delete pouter_dev;
	gXmlAnalyze.SetServiceInf(pService->pNetConf);
	gXmlAnalyze.SetReportFormatInf(pReportFormat->pReportFormatStore);

	//end save dlg

	localXml = BootOpenFile(FALSE);
	ret = gXmlAnalyze.SaveLocalXml(localXml.GetString());
	if (ret == 0)
	{
		str.Format("保存本地配置文件%s成功",localXml.GetString());
		MessageBox(str);
	}
	else
	{
		str.Format("保存本地配置文件%s失败",localXml.GetString());
		MessageBox(str);
	}
}

void CDtuConfigDlgDlg::OnBnClickedButtonRestart()
{
	int rtn = 0;
	CString str;
	rtn = SetConnectFdTimeOut();
	if (rtn != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	rtn = SetReStart();
	if(rtn != 0)
	{
		str.Format("[%s]重启失败,ret=%d", __FUNCTION__, rtn);
		gLog.Add("%s",str);
		MessageBox("重启失败");
	}
	else
	{
		st_thread_tsk_param thread_tsk_param;
		memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
		thread_tsk_param.type_thread = WAIT_TIME_THREAD;
		thread_tsk_param.date_tsk.SS = 20;//seconds
		int ret = AddThreadTsk(thread_tsk_param);
		ParseTskResult(ret);

		str.Format("重启成功,点击确定,加载最新配置");
		gLog.Add("%s",str);
		MessageBox(str);

		rtn = SetConnectFdTimeOut();
		if (rtn != 0)
		{
			if(transType == UART_TRANSF)
			{
				MessageBox("打开连接失败,检查串口连接是否正常");
			}
			else if(transType == NET_TRANSF)
			{
				MessageBox("打开连接失败,检查网络连接是否正常");
			}

			return;
		}

		memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
		thread_tsk_param.type_thread = LOAD_COLTOR_THREAD;
		ret = AddThreadTsk(thread_tsk_param);
		ParseTskResult(ret);
	}
}

void CDtuConfigDlgDlg::OnBnClickedButtonUpgrad()
{
	int ret = 0;
	int len = 0;
	char* file_data = NULL;
	int data_len = 0;
	CString str;
	ret = SetConnectFdTimeOut();
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	localXml = BootOpenFile();
	if (localXml.GetLength() == 0)
	{
		str.Format("本地文件选择无效");
		MessageBox(str);
		return;
	}

	len = GetFileLen(localXml.GetString());
	if (len <= 0)
	{
		str.Format("读取%s失败",localXml.GetString());
		MessageBox(str);
		return;
	}
	data_len = len + 1;
	file_data = new char[data_len];
	ZeroMemory(file_data,data_len);

	ret = GetFileData(localXml.GetString(),len,file_data);
	if (ret != 0)
	{
		str.Format("读取%s失败",localXml.GetString());
		MessageBox(str);
		gLog.Add(str);
		delete[] file_data;
		return;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = UPGRAD_THREAD;
	thread_tsk_param.file_data = file_data;
	thread_tsk_param.data_len = data_len;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);

	delete[] file_data;

	//远程升级后必须重启生效
	OnBnClickedButtonRestart();

	//CRestartRem* restart_rem = new CRestartRem;
	//ret = restart_rem->DoModal();
	//if(IDOK == ret)
	//{
	//	OnBnClickedButtonRestart();
	//}
	//delete restart_rem;
}

int CDtuConfigDlgDlg::GetAge()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetAgeCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, READ_AGE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetAgeRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetUpgrad(char*data, int data_len)
{
	int ret = 0;
	int send_len = 256;//每包长度,16进制
	int pack_num = 0;//总包数
	int off_set = 0;
	int cur_len = 0;
	bool no_remain = true;//整数次
	int remain = data_len % send_len;

	if (0 == remain)
	{
		pack_num = data_len / send_len;
		no_remain = true;
	}
	else
	{
		pack_num = data_len / send_len + 1;
		no_remain = false;
	}

	ret = SetStartUpgrad(pack_num, send_len);
	if (ret != 0)
	{
		return -1;
	}

	for(int i = 0; i < pack_num; i++)
	{
		TRACE("发送第%d包，共%d包\n",i,pack_num);

		int cur_step = (100*(i + 1)) / pack_num;
		pProgress->SetCurStep(cur_step);

		off_set = send_len * i;
		if (i == pack_num - 1 && !no_remain)
		{
			cur_len = remain;
		}
		else
		{
			cur_len = send_len;
		}

		int fail_time = 0;
		while(1)
		{
			int total_resend = 3;
			ret = SetUpgradData(i,data + off_set, cur_len );
			if (ret == 0)
			{
				break;
			}
			else
			{
				fail_time ++;
				if (fail_time >= total_resend)
				{
					Sleep(10);
					return -2;
				}
			}
		}
	}

	ret = SetEndUpgrad();
	if (ret != 0)
	{
		return -3;
	}

	return 0;
}

int CDtuConfigDlgDlg::SetStartUpgrad(int pack_num, int pack_len_max)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetStartUpgradCmd(pack_num, pack_len_max);
	rtn = gTransf.SendReqAndReceiveBody(len,SET_START_UPGRADE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetStartUpgradRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetUpgradData(int pack_ind,char* data,int data_len)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetUpgradDataCmd(pack_ind,data,data_len);
	rtn = gTransf.SendReqAndReceiveBody(len,SET_UPGRADE_DATA_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetUpgradDataRsp();
		if(rtn == 0)
		{
			TRACE("[%s]Parse suc[%d][%s]\n", __FUNCTION__, pack_ind,gXmlAnalyze.pBuff);
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetEndUpgrad()
{
	int rtn = 0;
	int len = 0;
	len = gXmlAnalyze.FormSetEndUpgradCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_END_UPGRADE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetEndUpgradRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

void CDtuConfigDlgDlg::UpdataOuterDevTree()
{
	str_outer_dev* pouter_dev = new str_outer_dev;
	memset(pouter_dev, 0, sizeof(*pouter_dev));

	pOuterDev->SaveDlg();

	pouter_dev->port_num = pOuterDev->portNum;
	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		//获取最新数据
		memcpy(&pouter_dev->port_dev[i],pOuterDev->pPort[i]->pPortDev,sizeof(pouter_dev->port_dev[i]));
	}
	pOuterDevTreeShow->SetOuterDev(pouter_dev);
	delete pouter_dev;
	pOuterDevTreeShow->ShowTree();
}

BOOL CDtuConfigDlgDlg::PreTranslateMessage(MSG* pMsg)
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


void CDtuConfigDlgDlg::OnBnClickedButtonHistorydata()
{
	int ret = 0;
	CString str;

	ret = SetConnectFdTimeOut();
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	int pre_result = 0;
	VEC_DEV_ALL_WAY_DATA_INF wayCheckInf;//回路检测信息
	u_w_measure w_measure;
	ZeroMemory(&w_measure,sizeof(w_measure));
	e_dev_type dev_type = AM_3PHASE;
	int way_ind = 0;
	int port_ind = 0;
	int dev_ind = 0;

	if (mDeleteHistoryData.GetCheck())
	{
		pre_result = 1;
	}
	else
	{
		pre_result = 0;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = HISTORY_DATA_THREAD;
	thread_tsk_param.pre_result = pre_result;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);
}

int CDtuConfigDlgDlg::GetHistoryData(int pre_result)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetHistoryDataCmd(pre_result);
	rtn = gTransf.SendReqAndReceiveBody(len,READ_HISTORYDATA_CMD);
	if(rtn == 0)
	{
		return 0;
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::AddThreadTsk( st_thread_tsk_param& thread_tsk_param)
{
	threadTskLock.Lock();
	int ret = 0;
	switch (thread_tsk_param.type_thread)
	{
	case UPGRAD_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			memcpy(data_frame->threadParam.file_data, thread_tsk_param.file_data, thread_tsk_param.data_len);

			taskParma = data_frame; 

			CString str("远程升级");
			pProgress->SetProgressName(str);
			pProgress->SetAutoProgress(false);
			pProgress->DoModal();
			pProgress->SetAutoProgress(true);
			if (data_frame->tskResult == 0)
			{
				str.Format("升级成功");
			}
			else
			{
				str.Format("升级失败");
			}
			ret = data_frame->tskResult;

			MessageBox(str);
			gLog.Add(str);
			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case LOAD_COLTOR_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("加载采集器配置");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if(data_frame->tskResult == 0)
			{
				if (!bSetLocalTimer)
				{
					SetTimer(LOCAL_TIME_ID,1000,NULL);
					bSetLocalTimer = true;
				}
				InitDlg();
				ShowLoadComButton(FALSE);
			}
			else
			{
				str.Format("[%s]加载采集器配置失败",__FUNCTION__);
				MessageBox("加载采集器配置失败");
				gLog.Add(str);
				ShowLoadComButton(TRUE);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case SAVE_COLTOR_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("保存采集器配置");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if(data_frame->tskResult == 0)
			{
				MessageBox("保存采集器配置成功\n");
				gLog.Add("[%s]SetDataToCollector success", __FUNCTION__);
			}
			else
			{
				str.Format("[%s]保存采集器配置失败",__FUNCTION__);
				MessageBox("保存采集器配置失败");
				gLog.Add("[%s]SetDataToCollector fail", __FUNCTION__);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case HISTORY_DATA_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("读取历史数据");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if (data_frame->tskResult == 0)
			{
				CHistoryData* pHistoryData = new CHistoryData;
				pHistoryData->SetData(gXmlAnalyze.pBuff,gXmlAnalyze.buffMallocLen);
				int ret_dlg = pHistoryData->DoModal();
			}
			else
			{
				str.Format("[%s]读历史数据失败",__FUNCTION__);
				MessageBox("读历史数据失败");
				gLog.Add(str);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;

	case SAVE_HISTORY_DATA_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("保存历史数据");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if (data_frame->tskResult == 0)
			{
				MessageBox("保存历史数据成功");
			}
			else
			{
				str.Format("[%s]保存历史数据失败",__FUNCTION__);
				MessageBox("保存历史数据失败");
				gLog.Add(str);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case CLEAN_HISDATA_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("清除历史数据");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if (data_frame->tskResult == 0)
			{
				str.Format("[%s]清除历史数据成功",__FUNCTION__);
				MessageBox("清除历史数据成功");
				gLog.Add(str);
			}
			else
			{
				str.Format("[%s]清除历史数据失败",__FUNCTION__);
				MessageBox("清除历史数据失败");
				gLog.Add(str);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case COPY_HISDATA_UDISK_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("拷贝历史数据到u盘");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if(data_frame->tskResult == 0)
			{
				MessageBox("拷贝历史数据到u盘成功\n");
				gLog.Add("[%s]拷贝历史数据到u盘成功", __FUNCTION__);
			}
			else
			{
				str.Format("[%s]拷贝历史数据到u盘失败",__FUNCTION__);
				MessageBox("拷贝历史数据到u盘失败");
				gLog.Add("[%s]拷贝历史数据到u盘失败", __FUNCTION__);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case COPY_LOG_UDISK_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("拷贝日志到u盘");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if(data_frame->tskResult == 0)
			{
				MessageBox("拷贝日志到u盘成功\n");
				gLog.Add("[%s]拷贝日志到u盘成功", __FUNCTION__);
			}
			else
			{
				str.Format("[%s]拷贝日志到u盘失败",__FUNCTION__);
				MessageBox("拷贝日志到u盘失败");
				gLog.Add("[%s]拷贝日志到u盘失败", __FUNCTION__);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case BACK_FACTORY_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("恢复出厂设置");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if(data_frame->tskResult == 0)
			{
				MessageBox("恢复出厂设置成功\n");
				gLog.Add("[%s]恢复出厂设置成功", __FUNCTION__);
			}
			else
			{
				str.Format("[%s]恢复出厂设置失败",__FUNCTION__);
				MessageBox("恢复出厂设置失败");
				gLog.Add("[%s]恢复出厂设置失败", __FUNCTION__);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case WAIT_TIME_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("等待系统启动");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if(data_frame->tskResult == 0)
			{
				//ret = 0;
			}
			else
			{
				//ret = -1;
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
	case RESET_SDCARD_THREAD:
		if (taskParma == NULL)
		{
			CTaskParam* data_frame = new CTaskParam(thread_tsk_param);
			taskParma = data_frame; 
			CString str("格式化SD卡");
			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if(data_frame->tskResult == 0)
			{
				MessageBox("格式化SD卡成功\n");
				gLog.Add("[%s]格式化SD卡成功", __FUNCTION__);
			}
			else
			{
				str.Format("[%s]格式化SD卡失败",__FUNCTION__);
				MessageBox("格式化SD卡失败");
				gLog.Add("[%s]格式化SD卡失败", __FUNCTION__);
			}
			ret = data_frame->tskResult;

			if(taskParma)
			{
				delete taskParma;
				taskParma = NULL;
			}
		}
		break;
		//add
	default:
		break;
	}

	threadTskLock.Unlock();
	return ret;
}

void CDtuConfigDlgDlg::LoadLocalConfig()
{
	/*
	;useLog:1表示记录日志，否则不记录日志
	;company:shyd表示上海移动，sdry表示山东乳液 
	[system]
	useLog = 1
	company = sdry
	*/

	int rtn = 0;
	CString config_name;
	config_name = gCurrentDir+"\\config.ini";

	int do_log = GetPrivateProfileInt("system", "useLog", -1, config_name);
	if(1 != do_log)
	{
		isLog = false;
	}
	else
	{
		isLog = true;
	}

	char param[256];
	ZeroMemory(param,sizeof(param));
	GetPrivateProfileString("system", "company", "", param, 256, config_name);
	CString company = param;
	if("shyd" == company)
	{
		gCompanyType = SHANG_HAI_YI_DONG;
	}
	else if("sdry" == company)
	{
		gCompanyType = SHAN_DONG_RU_YE;
	}
	else if("ziheng" == company)
	{
		gCompanyType = ZI_HENG_COMPANY;
	}
	else
	{
		gCompanyType = DEFAUL_COMPANY;
		gLog.Add("读取config.ini文件中company信息出错，使用默认值：DEFAUL_COMPANY");
	}

}


void CDtuConfigDlgDlg::OnBnClickedButtonSaveHisdata()
{
	int ret = 0;
	CString str;

	ret = SetConnectFdTimeOut();
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	saveHisPath = BootOpenFolder();
	if (saveHisPath == "")
	{
		MessageBox("保存历史数据,选择文件夹失败");
		return;
	}


	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = SAVE_HISTORY_DATA_THREAD;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);
}

#if 0
//xml
int CDtuConfigDlgDlg::SaveHistoryData()
{
	int rtn = 0;
	int len = 0;
	FILE* pf = NULL;
	char node_begin[16] = "<resp>\r\n";
	char node_end[16] = "</resp>\r\n";

	pf = fopen(saveHisPath,"wb");
	if(!pf)
	{
		return -1;
	}
	encry_history_data((unsigned char*)node_begin,strlen(node_begin));
	fwrite(node_begin,sizeof(char),strlen(node_begin),pf);
	fclose(pf);		

	pf = fopen(saveHisPath,"ab");
	if(!pf)
	{
		return -1;
	}

	rtn = SetStartSaveHisData();
	if (rtn != 0)
	{
		rtn = -1;
		goto ERR_STARTSAVE;
	}

	while(1)
	{
		Sleep(10);
		len = gXmlAnalyze.FormSaveHistoryDataCmd();
		rtn = gTransf.SendReqAndReceiveBody(len,SAVE_HISTORYDATA);
		if(rtn == 0)
		{
			rtn = gXmlAnalyze.ParseSaveHistoryDataRsp();
			if(rtn == 0)
			{
				encry_history_data((unsigned char*)gXmlAnalyze.m_pBuff,strlen(gXmlAnalyze.m_pBuff));
				fwrite(gXmlAnalyze.m_pBuff,sizeof(char),strlen(gXmlAnalyze.m_pBuff),pf);
			}
			else
			{
				gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.m_pBuff);
				rtn = -2;
				goto ERR_ENDSAVE;
			}
		}
		else
		{
			rtn = -3;
			goto ERR_ENDSAVE;
		}
	}


ERR_ENDSAVE:
	rtn = SetEndSaveHisData();
	if (rtn != 0)
	{
		rtn = -4;
	}
ERR_STARTSAVE:
	encry_history_data((unsigned char*)node_end,strlen(node_end));

	fwrite(node_end,sizeof(char),strlen(node_end),pf);

	fclose(pf);		

	return rtn;

}
#else
//CVS
int CDtuConfigDlgDlg::SaveHistoryData()
{
	int rtn = 0;
	int len = 0;
	CString file_name;
	bool use_head = true;

	rtn = SetStartSaveHisData();
	if (rtn != 0)
	{
		rtn = -1;
		goto ERR_STARTSAVE;
	}

	while(1)
	{
		Sleep(10);
		len = gXmlAnalyze.FormSaveHistoryDataCmd();
		rtn = gTransf.SendReqAndReceiveBody(len,SAVE_HISTORYDATA_CMD);
		if(rtn == 0)
		{
			rtn = gXmlAnalyze.ParseSaveHistoryDataRsp(saveHisPath,use_head);
			if(rtn == 0)
			{
				use_head = false;			
			}
			else
			{
				gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
				rtn = rtn;
				goto ERR_ENDSAVE;
			}
		}
		else
		{
			rtn = rtn;
			goto ERR_ENDSAVE;
		}
	}


ERR_ENDSAVE:
	rtn = SetEndSaveHisData();
	if (rtn != 0)
	{
		rtn = -4;
	}

ERR_STARTSAVE:	
	return rtn;
}
#endif

int CDtuConfigDlgDlg::SetStartSaveHisData()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetStartSaveHistoryDataCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_START_SAVE_HISTORYDATA_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetStartSaveHistoryRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetEndSaveHisData()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetEndSaveHistoryDataCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_END_SAVE_HISTORYDATA_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetEndSaveHistoryRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

void CDtuConfigDlgDlg::OnBnClickedButtonCleanhisdsta()
{
	int ret = 0;
	CString str;
	string user_name;
	string pass_word;
	bool suc = false;
	CPassWordDlg* pass_word_dlg = new CPassWordDlg;
	ret = pass_word_dlg->DoModal();
	if(IDOK == ret)
	{
		user_name = pass_word_dlg->GetUserName(); 
		pass_word = pass_word_dlg->GetPassWord(); 
		if (0 == strcmp(user_name.c_str(),"nhb") &&
			0 == strcmp(pass_word.c_str(),"nhb") )
		{
			suc = true;
		}
	}
	delete pass_word_dlg;

	if (!suc)
	{
		MessageBox("用户名或密码错误");
		return;
	}

	ret = SetConnectFdTimeOut(CLEAN_HISTORY_DATA_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = CLEAN_HISDATA_THREAD;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);
}

int CDtuConfigDlgDlg::SetCleanHisData()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetCleanHisDataCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, SET_CLEAN_HISTORYDATA_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetCleanHisDataRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}


void CDtuConfigDlgDlg::OnBnClickedButtonStartReport()
{
	CString str;
	int ret = 0;
	ret = SetConnectFdTimeOut(TEST_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	ret = SetStartReport();
	if (ret != 0)
	{
		str.Format("设置开始串口上报失败,ret=%d",ret);
		MessageBox("设置开始串口上报失败");
	}

}


void CDtuConfigDlgDlg::OnBnClickedButtonEndReport()
{
	CString str;
	int ret = 0;
	ret = SetConnectFdTimeOut();
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	ret = SetEndReport();
	if (ret != 0)
	{
		str.Format("设置停止串口上报失败,ret=%d",ret);
		MessageBox("设置停止串口上报失败");
	}
}

int CDtuConfigDlgDlg::SetStartReport()
{
	int rtn = 0;
	int len = 0;
	len = gXmlAnalyze.FormSetStartReportCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_START_REPORT_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetStartReportRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetEndReport()
{
	int rtn = 0;
	int len = 0;
	len = gXmlAnalyze.FormSetEndReportCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_END_REPORT_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetEndReportRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::GetBuilInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetBuildCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, READ_BUILD_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetBuildRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetBuilInf()
{
	int rtn = 0;
	int len = 0;
	len = gXmlAnalyze.FormSetBuildCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_BUILD_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetBuildRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::GetEthNetInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetEthCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,READ_ETH_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetEthRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetEthNetInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetEthCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_ETH_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetEthRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::GetHotSpotInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetHotSpotCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,READ_HOTSPOT_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetHotSpotRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetHotSpotInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetHotSpotCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_HOTSPOT_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetHotSpotRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetEffectNow(VEC_STRING& vec_name_string)
{
	int rtn = 0;
	int len = 0;
	len = gXmlAnalyze.FormSetEffectNowCmd(vec_name_string);
	rtn = gTransf.SendReqAndReceiveBody(len, SET_EFFECT_NOW_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetEffectNowRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::GetReportFormatInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetReportFormatCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,READ_REPORT_FORMAT_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetReportFormatRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetReportFormatInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetReportFormatCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_REPORT_FORMAT_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetReportFormatRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::GetPortDevNum(int port_ind)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetPortDevNumCmd(port_ind);
	rtn = gTransf.SendReqAndReceiveBody(len,READ_PORT_DEV_NUM_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetPortDevNumRsp(port_ind);
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::StartSetPortInf(int port_ind)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormStartSetPortCmd(port_ind);
	rtn = gTransf.SendReqAndReceiveBody(len,SET_START_SET_PORT_DEV_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseStartSetPortCmd();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::EndSetPortInf(int port_ind)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormEndSetPortCmd(port_ind);
	rtn = gTransf.SendReqAndReceiveBody(len,SET_END_SET_PORT_DEV_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseEndSetPortRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}


void CDtuConfigDlgDlg::OnCancel()
{
	while(1)
	{
		if (taskThreadState.is_run)
		{
			taskThreadState.is_kill = true;
		}
		else
		{
			break;
		}
	}

	CDialog::OnCancel();
}

int CDtuConfigDlgDlg::GetDefSubTypeInf()
{
	int rtn = 0;
	int len = 0;

	ClearDefSubTypeNameWayCount();

	//自定义子类型个数
	int def_sub_type_num = 0;
	rtn = GetDefSubTypeNum(def_sub_type_num);
	if (rtn != 0)
	{
		gLog.Add("[%s]Parse GetDefSubTypeNum fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
		return rtn;
	}

	for(int def_sub_type_ind = 0; def_sub_type_ind < def_sub_type_num; def_sub_type_ind++)
	{
		//读自定义子类型配置
		int way_num = 0;
		rtn	= GetDefSubTypeConf(def_sub_type_ind, way_num);
		if (rtn != 0)
		{
			gLog.Add("[%s]Parse GetDefSubTypeConf fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}

		for (int way_ind = 0; way_ind < way_num; way_ind++)
		{
			//读自定义子类型回路配置
			len = gXmlAnalyze.FormGetDefSubTypeWayCmd(def_sub_type_ind, way_ind);
			rtn = gTransf.SendReqAndReceiveBody(len, READ_DEF_SUBTYPE_WAY_CMD);
			if(rtn == 0)
			{
				rtn = gXmlAnalyze.ParseGetDefSubTypeWayRsp(def_sub_type_ind, way_ind);
				if(rtn == 0)
				{
					continue;
				}
				else
				{
					gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
					return rtn;
				}
			}
			else
			{
				return rtn;
			}
		}
	
	}
	return 0;
}

int CDtuConfigDlgDlg::SetDefSubTypeInf()
{
	int rtn = 0;
	int len = 0;

	//开始
	rtn = StartSetDefSubTypeInf();
	if (rtn != 0)
	{
		return -12;
	}

	int order_ind = -1;
	for (int j = 0; j < gXmlAnalyze.pCProtModbusSet->protModbusNum; j++)
	{
		if (gXmlAnalyze.pCProtModbusSet->pCProtModbusArr[j].pStDefSubTypeInfo->dev_type == 0)
		{
			continue;
		}

		order_ind++;

		//设置自定义配置信息
		rtn = SetDefSubTypeConf(j, order_ind);
		if (rtn != 0)
		{
			return -13;
		}

		//设置自定义回路信息
		CProtModbus* p_CParamDevDefModbus = 
			&gXmlAnalyze.pCProtModbusSet->pCProtModbusArr[j];
		int sub_type = GetSubTypeIndByValue(p_CParamDevDefModbus->pStDefSubTypeInfo->sub_type);
		assert(sub_type >= 0);
		int way_num = gWayCountValue[sub_type];
		bool reg_is_same = p_CParamDevDefModbus->pStDefSubTypeInfo->is_reg_same; 
		if (reg_is_same)
		{
			way_num = (way_num > 0) ? 1 : way_num;
		}
		else
		{
			way_num = way_num;
		}

		for (int k = 0; k < way_num; k++)
		{
			//设置自定义子类型回路配置
			len = gXmlAnalyze.FormSetDefSubTypeWayCmd(order_ind, j, k);
			rtn = gTransf.SendReqAndReceiveBody(len, SET_DEF_SUBTYPE_WAY_CMD);
			if(rtn == 0)
			{
				rtn = gXmlAnalyze.ParseSetDefSubTypeWayRsp();
				if(rtn == 0)
				{
					continue;
				}
				else
				{
					gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
					return rtn;
				}
			}
			else
			{
				return rtn;
			}
		}

		
	}

	//结束
	rtn = EndSetDefSubTypeInf();
	if (rtn != 0)
	{
		return -13;
	}

	return 0;
}

int CDtuConfigDlgDlg::GetDefSubTypeNum(int& num)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetDefSubTypeNumCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, READ_DEF_SUBTYPE_NUM_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetDefSubTypeNumRsp(num);
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::StartSetDefSubTypeInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormStartSetDefSubTypeCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_START_SET_DEF_SUBTYPE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseStartSetDefSubTypeCmd();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::EndSetDefSubTypeInf()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormEndSetDefSubTypeCmd();
	rtn = gTransf.SendReqAndReceiveBody(len,SET_END_SET_DEF_SUBTYPE_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseEndSetDefSubTypeCmd();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}


void CDtuConfigDlgDlg::OnBnClickedButtonCopyHisdataUdisk()
{
	int ret = 0;
	CModifyTime* modify_time = new CModifyTime;
	st_time date;
	memset(&date, 0, sizeof(date));
	ret = modify_time->DoModal();
	if(IDOK == ret)
	{
		memcpy(&date, modify_time->pTime, sizeof(date)); 
		delete modify_time;
	}
	else
	{
		delete modify_time;
		MessageBox("日期选择失败");
		return;
	}

	ret = SetConnectFdTimeOut(COPY_HIS_DATA_UDISK_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = COPY_HISDATA_UDISK_THREAD;
	memcpy(&thread_tsk_param.date_tsk, &date, sizeof(date));
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);
}

int CDtuConfigDlgDlg::SetCopyHisDataUDisk(st_time& date_time)
{
	int rtn = 0;
	int len = 0;

	VEC_TIME data_vec;
	rtn = GetMonthHisDataInf(date_time, data_vec);
	if (rtn != 0)
	{
		gLog.Add("[%s]Parse GetMonthHisDataInf fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
		return rtn;
	}
	MU_INT32 day_num = data_vec.size();

	for(MU_INT32 j = 0; j < day_num; j++)
	{
		rtn = CopyHisDataDayUDisk(data_vec[j]);
		if (rtn != 0)
		{
			gLog.Add("[%s]Parse CopyHisDataDayUDisk fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	return 0;
}

int CDtuConfigDlgDlg::SetCopyLogUDisk()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetCopyLogUDiskCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, SET_COPY_LOG_UDISK_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetCopyLogUDiskRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}


void CDtuConfigDlgDlg::OnBnClickedButtonCopyLogUdisk()
{
	int ret = 0;

	ret = SetConnectFdTimeOut(COPY_LOG_UDISK_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = COPY_LOG_UDISK_THREAD;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);
}


void CDtuConfigDlgDlg::OnBnClickedButtonBackFactory()
{
	int ret = 0;
	CString str;
	string user_name;
	string pass_word;
	bool suc = false;
	CPassWordDlg* pass_word_dlg = new CPassWordDlg;
	ret = pass_word_dlg->DoModal();
	if(IDOK == ret)
	{
		user_name = pass_word_dlg->GetUserName(); 
		pass_word = pass_word_dlg->GetPassWord(); 
		if (0 == strcmp(user_name.c_str(),"nhb") &&
			0 == strcmp(pass_word.c_str(),"nhb") )
		{
			suc = true;
		}
	}
	delete pass_word_dlg;

	if (!suc)
	{
		MessageBox("用户名或密码错误");
		return;
	}

	ret = SetConnectFdTimeOut(RESET_SDCARD_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = BACK_FACTORY_THREAD;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);

	//恢复出厂设置后必须重启
	OnBnClickedButtonRestart();
	//CRestartRem* restart_rem = new CRestartRem;
	//ret = restart_rem->DoModal();
	//if(IDOK == ret)
	//{
	//	OnBnClickedButtonRestart();
	//}
	//delete restart_rem;
}

int CDtuConfigDlgDlg::SetBackFactory()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetBackFactoryCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, SET_BACK_FACTORY_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetBackFactoryRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetHeartBeat()
{
#define STATIC_HEAETBEAT_NOT_PARSE 1
	int rtn = 0;
	int len = 0;

#if STATIC_HEAETBEAT_NOT_PARSE
	len = gXmlAnalyze.FormSetHeartBeatCmd();
	rtn = gTransf.OnSend(gXmlAnalyze.pBuff, len);
	if(rtn == 0)
	{
		//char* sendData = gXmlAnalyze.pBuff;
		//int head_len = MESSAGE_BODY_IND;
		//int send_len = len;

		//gLog.Add("[%s]OnSend[head:%02x%02x%02x%02x][len:%02x%02x%02x%02x][sequece:%02x%02x%02x%02x]\n[xml:%s]\n[parity:%02x%02x][tail:%02x%02x%02x%02x]\n", __FUNCTION__,
		//	(MU_INT8)sendData[0],(MU_INT8)sendData[1],(MU_INT8)sendData[2],
		//	(MU_INT8)sendData[3],(MU_INT8)sendData[4],(MU_INT8)sendData[5],
		//	(MU_INT8)sendData[6],(MU_INT8)sendData[7],(MU_INT8)sendData[8],
		//	(MU_INT8)sendData[9],(MU_INT8)sendData[10],(MU_INT8)sendData[11],
		//	&sendData[head_len],
		//	(MU_INT8)sendData[send_len - 6],(MU_INT8)sendData[send_len - 5],
		//	(MU_INT8)sendData[send_len - 4],(MU_INT8)sendData[send_len - 3],(MU_INT8)sendData[send_len - 2],(MU_INT8)sendData[send_len - 1]);

		return 0;
	}
	else
	{
		return rtn;
	}
#else
	len = gXmlAnalyze.FormSetHeartBeatCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, SET_BACK_FACTORY_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetHeartBeatRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
#endif
}

void CDtuConfigDlgDlg::ParseTskResult(int result)
{
	if (result == SEND_FAIL ||
		result == RECEIVE_FAIL
		)
	{
		gLog.Add("[%s]communicate fail", __FUNCTION__);
	}
}

void CDtuConfigDlgDlg::CloseProgress()
{
	while(1)
	{
		if (pProgress->GetRunFlag())
		{
			pProgress->SendMessage(WM_CLOSE);
			break;
		}
		Sleep(10);
	}
}

void CDtuConfigDlgDlg::OnBnClickedButtonNetBroad()
{
	int ret = 0;
	CString str;
	CNetBroadCast* net_broadcast = new CNetBroadCast;
	ret = net_broadcast->DoModal();
	if(IDOK == ret)
	{
		str = "网络参数设置成功";
		memcpy(&netIpPort, net_broadcast->GetIpPort(), sizeof(netIpPort));
		netIpPort.port = 54321;
		transType = NET_TRANSF;
		SetCommunicateWay();
	}
	else
	{
		str.Format("网络参数设置失败");
		MessageBox(str);
	}

	delete net_broadcast ;
}

void CDtuConfigDlgDlg::SetCommunicateWay()
{
	int ret = 0;
	CString str;
	gTransf.SetTransType(transType);
	if (transType == UART_TRANSF)
	{
		//初始化串口号下拉框
		InitCombUart();
	}
	else if(transType == NET_TRANSF)
	{
		//init net
		gTransf.SetIpPort(netIpPort);
	}
}

int CDtuConfigDlgDlg::GetMonthHisDataInf( st_time& date_time, VEC_TIME& data_vec )
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetMonthHisDataInfCmd(date_time);
	rtn = gTransf.SendReqAndReceiveBody(len, SET_COPY_HISDATA_UDISK_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetMonthHisDataInfRsp(data_vec);
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::CopyHisDataDayUDisk( st_time& date_time )
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormSetCopyHisDataUDiskCmd(date_time);
	rtn = gTransf.SendReqAndReceiveBody(len, SET_COPY_HISDATA_UDISK_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetCopyHisDataUDiskRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}


void CDtuConfigDlgDlg::OnBnClickedButtonSdReset()
{
	int ret = 0;
	CString str;
	string user_name;
	string pass_word;
	bool suc = false;
	CPassWordDlg* pass_word_dlg = new CPassWordDlg;
	ret = pass_word_dlg->DoModal();
	if(IDOK == ret)
	{
		user_name = pass_word_dlg->GetUserName(); 
		pass_word = pass_word_dlg->GetPassWord(); 
		if (0 == strcmp(user_name.c_str(),"nhb") &&
			0 == strcmp(pass_word.c_str(),"nhb") )
		{
			suc = true;
		}
	}
	delete pass_word_dlg;

	if (!suc)
	{
		MessageBox("用户名或密码错误");
		return;
	}

	ret = SetConnectFdTimeOut(RESET_SDCARD_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	st_thread_tsk_param thread_tsk_param;
	memset(&thread_tsk_param, 0, sizeof(thread_tsk_param));
	thread_tsk_param.type_thread = RESET_SDCARD_THREAD;
	ret = AddThreadTsk(thread_tsk_param);
	ParseTskResult(ret);

	//格式化sd卡后必须重启
	OnBnClickedButtonRestart();

	//CRestartRem* restart_rem = new CRestartRem;
	//ret = restart_rem->DoModal();
	//if(IDOK == ret)
	//{
	//	OnBnClickedButtonRestart();
	//}
	//delete restart_rem;
}

int CDtuConfigDlgDlg::SetResetSDCard()
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormResetSDCardCmd();
	rtn = gTransf.SendReqAndReceiveBody(len, SET_RESET_SDCARD_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseResetSDCardRsp();
		if(rtn == 0)
		{
			return 0;
		}
		else
		{
			gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

void CDtuConfigDlgDlg::HideChildPage()
{
	pColtor->ShowWindow(false);
	pOuterDev->ShowWindow(false);
	pCParamDefModbusSetDlg->ShowWindow(false);
	pService->ShowWindow(false);
	pReportFormat->ShowWindow(false);
	pOuterDevTreeShow->ShowWindow(false);
	pCHelpDocument->ShowWindow(false);
}

int CDtuConfigDlgDlg::GetDefSubTypeConf(int def_sub_type_ind, int& way_num)
{
	int len = 0;
	int rtn = 0;

	len = gXmlAnalyze.FormGetDefSubTypeConfCmd(def_sub_type_ind);
	rtn = gTransf.SendReqAndReceiveBody(len, READ_DEF_SUBTYPE_CONF_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetDefSubTypeConfRsp(def_sub_type_ind);
		if(rtn == 0)
		{
			bool is_reg_same 
				= gXmlAnalyze.pCProtModbusSet->pCProtModbusArr[def_sub_type_ind]
					.pStDefSubTypeInfo->is_reg_same;
			way_num 
				= gXmlAnalyze.pCProtModbusSet->pCProtModbusArr[def_sub_type_ind]
					.pStDefSubTypeInfo->way_num;
			if (is_reg_same)
			{
				way_num = (way_num > 0) ? 1 : way_num;
			}
			else
			{
				way_num = way_num;
			}
			return rtn;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}

int CDtuConfigDlgDlg::SetDefSubTypeConf( int def_sub_type_ind, int order_ind)
{
	int len = 0;
	int rtn = 0;

	len = gXmlAnalyze.FormSetDefSubTypeConfCmd(def_sub_type_ind, order_ind);
	rtn = gTransf.SendReqAndReceiveBody(len, SET_DEF_SUBTYPE_CONF_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseSetDefSubTypeConfRsp();
		if(rtn == 0)
		{
			return rtn;
		}
		else
		{
			gLog.Add("[%s]Parse fail[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
			return rtn;
		}
	}
	else
	{
		return rtn;
	}
}
