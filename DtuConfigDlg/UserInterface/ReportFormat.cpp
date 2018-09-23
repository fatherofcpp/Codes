// ReportFormat.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "ReportFormat.h"

#include "dataConvert.h"
#include "CommonFunction.h"
#include "log.h"

// CReportFormat 对话框

IMPLEMENT_DYNAMIC(CReportFormat, CDialog)

	CReportFormat::CReportFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CReportFormat::IDD, pParent)
{
	pReportFormatStore = new st_report_store_inf;
	memset(pReportFormatStore, 0, sizeof(*pReportFormatStore));
	devTypeNum = gDevTypeNum;
	m_pListCtrl = new CListCtrl[devTypeNum];
	currentDevTypeInd = 0;

	//init reportParam
	pCParamDevSet = new CParamDevTypeSet;
}

CReportFormat::~CReportFormat()
{
	delete pReportFormatStore;
	delete[] m_pListCtrl;

	delete pCParamDevSet;
}

void CReportFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REPORT_PARAM, mCombReportParam);
	DDX_Control(pDX, IDC_BUTTON_ADD, mBtAddParam);
	DDX_Control(pDX, IDC_BUTTON_DELETE, mBtDeleteParam);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, mBtModifyParam);
	DDX_Control(pDX, IDC_COMBO_DEVTYPE, mComBDevType);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT_PARAM_SET, mBtAllDevDefaultParamSet);
	DDX_Control(pDX, IDC_LIST_ALL_DEV_REPORT_VIEW, listAllDevReportView);
	DDX_Control(pDX, IDC_BUTTON_CUR_DEV_DEFAULT_PARAM, btCurDevDefaultReportParam);
}


BEGIN_MESSAGE_MAP(CReportFormat, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CReportFormat::OnBnClickedButtonAddParam)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CReportFormat::OnBnClickedButtonDeleteParam)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CReportFormat::OnBnClickedButtonModifyParam)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVTYPE, &CReportFormat::OnCbnSelchangeComboDevtype)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_PARAM_SET, &CReportFormat::OnBnClickedButtonAllDevDefaultParamSet)
	ON_BN_CLICKED(IDC_BUTTON_CUR_DEV_DEFAULT_PARAM, &CReportFormat::OnBnClickedButtonCurDevDefaultParam)
END_MESSAGE_MAP()

void CReportFormat::SetReportFormat( st_report_store_inf* report_format )
{
	memcpy(pReportFormatStore, report_format, sizeof(*pReportFormatStore));
}

void CReportFormat::InitDlg()
{
	ShowInput(true);

	SetListControlAllDev();
	SetListControlAllDevReportView();
}

int CReportFormat::SaveDlg()
{
	int ret = 0;
	if (pReportFormatStore->report_format.is_modify)
	{
		ClearReportFormat();
		GetListControlAllDev();
	}

	return ret;
}

void CReportFormat::GetReportFormat( st_report_store_inf* report_format )
{
	memcpy(report_format, pReportFormatStore, sizeof(*report_format));
}


// CReportFormat 消息处理程序


void CReportFormat::OnBnClickedButtonAddParam()
{
	CString str;
	int nRow = 0;
	int i = 1;

	int com_ind = mCombReportParam.GetCurSel();
	if(com_ind < 0)
	{
		return;
	}

	string name_new = GetAllDevTypeReportParamNameByValue(
		pCParamDevSet->pCParamDevTypeArr[currentDevTypeInd].pStParamInfoArr[com_ind].param_id);
	bool is_exist = IsParamNameExist(name_new);
	if (is_exist)
	{
		MessageBox("参数已存在");
		return;
	}

	int item_count = m_pListCtrl[currentDevTypeInd].GetItemCount();
	str.Format("%d", item_count + 1);

	nRow = m_pListCtrl[currentDevTypeInd].InsertItem(item_count, str);//插入行

	str.Format("%s", name_new.c_str());

	m_pListCtrl[currentDevTypeInd].SetItemText(nRow, i++, str);//设置数据

	str.Format("%s", gDevTypeName[currentDevTypeInd]);
	m_pListCtrl[currentDevTypeInd].SetItemText(nRow, i++, str);

	SetModify(pReportFormatStore->report_format.is_modify);

	SetListControlAllDevReportView();
}


void CReportFormat::OnBnClickedButtonDeleteParam()
{
	while(m_pListCtrl[currentDevTypeInd].GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int nItem = m_pListCtrl[currentDevTypeInd].GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);

		m_pListCtrl[currentDevTypeInd].DeleteItem(nItem);

		int iItem = -1;
		while ((iItem = m_pListCtrl[currentDevTypeInd].GetNextItem(iItem, LVNI_ALL)) > -1 )
		{
			char item[32] = "";
			sprintf(item, "%d", iItem + 1);
			m_pListCtrl[currentDevTypeInd].SetItemText(iItem, 0, item);
		}
	}

	SetModify(pReportFormatStore->report_format.is_modify);
	SetListControlAllDevReportView();
}


void CReportFormat::OnBnClickedButtonModifyParam()
{
	int item_count = 0;
	int nRow = 0;
	CString str = "12";
	char item[64];
	memset(item, 0, sizeof(item));

	int com_ind = mCombReportParam.GetCurSel();
	if(com_ind < 0)
	{
		return;
	}

	string name_new = GetAllDevTypeReportParamNameByValue(
		pCParamDevSet->pCParamDevTypeArr[currentDevTypeInd].pStParamInfoArr[com_ind].param_id);
	bool is_exist = IsParamNameExist(name_new);
	if (is_exist)
	{
		MessageBox("参数已存在");
		return;
	}

	item_count = m_pListCtrl[currentDevTypeInd].GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (item_count == -1)
	{
		return;
	}

	m_pListCtrl[currentDevTypeInd].DeleteItem(item_count);

	sprintf(item, "%d", item_count + 1);
	nRow = m_pListCtrl[currentDevTypeInd].InsertItem(item_count, item);//插入行
	int i = 1;

	str.Format("%s", name_new.c_str());

	m_pListCtrl[currentDevTypeInd].SetItemText(nRow, i++, str);//设置数据

	str.Format("%s", gDevTypeName[currentDevTypeInd]);
	m_pListCtrl[currentDevTypeInd].SetItemText(nRow, i++, str);

	SetModify(pReportFormatStore->report_format.is_modify);

	SetListControlAllDevReportView();
}

void CReportFormat::SetCombReportParam()
{
	mCombReportParam.ResetContent();
	for (int i = 0; i < pCParamDevSet->pCParamDevTypeArr[currentDevTypeInd].paramInfNum; i++)
	{
		CString str;
		int param_id = pCParamDevSet->pCParamDevTypeArr[currentDevTypeInd].pStParamInfoArr[i].param_id;
		str.Format("[%d]%s",i,
			GetAllDevTypeReportParamNameByValue(param_id));
		mCombReportParam.InsertString(i,str);
	}
	mCombReportParam.SetCurSel(0);
}

BOOL CReportFormat::OnInitDialog()
{
	CDialog::OnInitDialog();
	ShowInput(false);

	SetCombDevType();
	SetCombReportParam();

	CreateAllDevList();
	CreateListAllDevReportView();
	return TRUE;
}


void CReportFormat::OnCbnSelchangeComboDevtype()
{
	int ind = mComBDevType.GetCurSel();
	assert(ind >= 0);
	currentDevTypeInd = ind;

	ShowListByDevTypeInd();
	SetCombReportParam();
}

void CReportFormat::CreateAllDevList()
{
	CRect rect;
	this->GetClientRect(&rect);
	rect.top += (rect.bottom * 2 / 100);
	rect.bottom = rect.bottom * 90 / 100;
	rect.left += (rect.right * 1 / 100);
	rect.right = rect.right * 45 / 100;

	CString strTemp;
	for (int i = 0; i < devTypeNum; i++)
	{
		m_pListCtrl[i].Create(WS_CHILD | LVS_REPORT | LVS_EDITLABELS ,rect,this,i);

		LONG l_style;
		l_style = GetWindowLong(m_pListCtrl[i].m_hWnd, GWL_STYLE);//获取当前窗口style
		l_style &= ~LVS_TYPEMASK; //清除显示方式位
		l_style |= LVS_REPORT; //设置style
		SetWindowLong(m_pListCtrl[i].m_hWnd, GWL_STYLE, l_style);//设置style

		m_pListCtrl[i].SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); 

		//
		//DWORD dw_style = m_pListCtrl[i].GetExtendedStyle();
		//dw_style |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
		//dw_style |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
		//m_pListCtrl[i].SetExtendedStyle(dw_style); //设置扩展风格
		//
		int column = 0;
		int width = 100;
		m_pListCtrl[i].InsertColumn(column++, "参数序号", LVCFMT_LEFT, width );
		m_pListCtrl[i].InsertColumn(column++, "参数名称", LVCFMT_LEFT, width* 140 / 100 );
		m_pListCtrl[i].InsertColumn(column++, "设备类型", LVCFMT_LEFT, width* 180 / 100 );
	}

	ShowListByDevTypeInd();
}

void CReportFormat::SetListControlAllDev()
{
	ClearListDataAllDev();

	for(int i = 0; i < pReportFormatStore->report_format.dev_type_num; i++)//设备类型下标
	{
		//根据设备类型找到列表下标
		int list_ind = GetDevTypeIndByValue(pReportFormatStore->report_format.dev_type_report[i].dev_type);
		assert(list_ind >= 0);

		int row_ind = 1;//行号1-n
		for(int j = 0; j < pReportFormatStore->report_format.dev_type_report[i].param_num; j++)//参数下标
		{
			char name[64];
			memset(name, 0, sizeof(name));

			sprintf(name, "%d", row_ind);
			int list_item_ind = m_pListCtrl[list_ind].InsertItem(row_ind++, name);//插入行

			int column_ind = 1;
			sprintf(name, "%s", 
				GetAllDevTypeReportParamNameByValue(
				pReportFormatStore->report_format.dev_type_report[i].report_param_id[j]));
			m_pListCtrl[list_ind].SetItemText(list_item_ind, column_ind++, name);

			sprintf(name, "%s", gDevTypeName[list_ind]);
			m_pListCtrl[list_ind].SetItemText(list_item_ind, column_ind++, name);
		}

	}
	return;
}

void CReportFormat::SetCombDevType()
{
	CString	str;
	for (int i = 0; i < gDevTypeNum; i++)
	{
		str.Format("%s",gDevTypeName[i]);
		mComBDevType.InsertString(i,str);
	}

	mComBDevType.SetCurSel(currentDevTypeInd);
}

void CReportFormat::ShowListByDevTypeInd()
{
	for (int i = 0; i < devTypeNum; i++)
	{
		m_pListCtrl[i].ShowWindow(false);    
	}
	m_pListCtrl[currentDevTypeInd].ShowWindow(true);   
}

void CReportFormat::GetListControlAllDev()
{
	int dev_type_report_ind = 0;

	for (int i = 0; i < devTypeNum; i++)
	{
		int item = -1;
		int row_ind = -1;//行号
		bool first_enter = true;//列表有数据时，对类型总数及类型值只做一次修改。
		while ((item = m_pListCtrl[i].GetNextItem(item, LVNI_ALL)) > -1 )
		{
			CString strText;
			row_ind++;
			int column = 0;//列号
			strText = m_pListCtrl[i].GetItemText(item, ++column);
			pReportFormatStore->report_format.dev_type_report[dev_type_report_ind].report_param_id[row_ind]
			= GetAllDevTypeReportParamValueByName(strText.GetString());
			pReportFormatStore->report_format.dev_type_report[dev_type_report_ind].param_num ++;

			if(first_enter)
			{
				pReportFormatStore->report_format.dev_type_report[dev_type_report_ind].dev_type = (e_dev_type)gDevTypeValue[i];
				pReportFormatStore->report_format.dev_type_num ++;
				first_enter = false;
			}
		}

		if(!first_enter)
		{
			dev_type_report_ind ++;
		}
	}

	return;
}

void CReportFormat::SetModify(bool& modify)
{
	modify = true;
	DisplaySaveButton();
}

BOOL CReportFormat::PreTranslateMessage( MSG* pMsg )
{
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

void CReportFormat::DisplaySaveButton()
{
	bool modify = 
		pReportFormatStore->report_format.is_modify;

	if (modify == true)
	{
		if (gComNum != 0)
		{
			GetParent()->GetDlgItem(IDC_BUTTON_SAVE_COM)->EnableWindow(true);
		}
	}
}

void CReportFormat::ClearReportFormat()
{
	// 修改状态不能清空
	memset(pReportFormatStore->report_format.dev_type_report, 0, sizeof(pReportFormatStore->report_format.dev_type_report));
	pReportFormatStore->report_format.dev_type_num = 0;
}

void CReportFormat::ClearListDataAllDev()
{
	//在手动加载时候，先删除已经存在的数据.
	for (int i = 0; i < devTypeNum; i++)
	{
		while(m_pListCtrl[i].GetNextItem(-1,LVNI_ALL) != -1)
		{
			int item = m_pListCtrl[i].GetNextItem(-1,LVNI_ALL);
			BOOL is_delete = m_pListCtrl[i].DeleteItem(item);
			is_delete = is_delete;
		}
	}
}

bool CReportFormat::IsParamNameExist( string param_name )
{
	bool exist = false;
	int item = -1;
	int row_ind = -1;//行号

	while ((item = m_pListCtrl[currentDevTypeInd].GetNextItem(item, LVNI_ALL)) > -1 )
	{
		CString strText;
		row_ind++;
		int column = 0;//列号
		strText = m_pListCtrl[currentDevTypeInd].GetItemText(item, ++column);//param_name
		if (0 == strcmp(param_name.c_str(), strText.GetString()))
		{
			exist = true;
		}
	}
	return exist;
}

void CReportFormat::SetAllDevDefaultParam()
{
	ClearListDataAllDev();

	for(int i = 0; i < devTypeNum; i++)//设备类型下标
	{
		//根据设备类型找到列表下标
		int list_ind = i;

		int row_ind = 1;//行号1-n
		for(int j = 0; j < pCParamDevSet->pCParamDevTypeArr[i].paramInfNum; j++)//参数下标
		{
			char name[64];
			memset(name, 0, sizeof(name));

			sprintf(name, "%d", row_ind);
			int list_item_ind = m_pListCtrl[list_ind].InsertItem(row_ind++, name);//插入行

			int column_ind = 1;
			int param_id = pCParamDevSet->pCParamDevTypeArr[i].pStParamInfoArr[j].param_id;
			sprintf(name, "%s", 
				GetAllDevTypeReportParamNameByValue(param_id));
			m_pListCtrl[list_ind].SetItemText(list_item_ind, column_ind++, name);

			sprintf(name, "%s", gDevTypeName[list_ind]);
			m_pListCtrl[list_ind].SetItemText(list_item_ind, column_ind++, name);
		}

	}
	SetModify(pReportFormatStore->report_format.is_modify);
	return;
}



void CReportFormat::OnBnClickedButtonAllDevDefaultParamSet()
{
	// TODO: 在此添加控件通知处理程序代码
	SetAllDevDefaultParam();
	SetListControlAllDevReportView();
}

void CReportFormat::ShowInput( bool show )
{
	mBtAddParam.EnableWindow(show);
	mBtDeleteParam.EnableWindow(show);
	mBtModifyParam.EnableWindow(show);
	mBtAllDevDefaultParamSet.EnableWindow(show);
	btCurDevDefaultReportParam.EnableWindow(show);
	mCombReportParam.EnableWindow(show);
	mComBDevType.EnableWindow(show);
}

void CReportFormat::CreateListAllDevReportView()
{
	LONG l_style;
	l_style = GetWindowLong(listAllDevReportView.m_hWnd, GWL_STYLE);//获取当前窗口style
	l_style &= ~LVS_TYPEMASK; //清除显示方式位
	l_style |= LVS_REPORT; //设置style
	SetWindowLong(listAllDevReportView.m_hWnd, GWL_STYLE, l_style);//设置style

	DWORD dw_style = listAllDevReportView.GetExtendedStyle();
	dw_style |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dw_style |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	listAllDevReportView.SetExtendedStyle(dw_style); //设置扩展风格

	int n_index = 0;
	int width = 200;
	listAllDevReportView.InsertColumn(n_index++, "", LVCFMT_LEFT, 30 );
	listAllDevReportView.InsertColumn(n_index++, "设备类型", LVCFMT_LEFT, width*120/100 );
	listAllDevReportView.InsertColumn(n_index++, "上报参量个数", LVCFMT_LEFT, width*110/100 );
}

void CReportFormat::ClearListDataAllDevReportView()
{
	while(listAllDevReportView.GetNextItem(-1,LVNI_ALL) != -1)
	{
		int n_item = listAllDevReportView.GetNextItem(-1,LVNI_ALL);
		listAllDevReportView.DeleteItem(n_item);//删除
	}
}

void CReportFormat::SetListControlAllDevReportView()
{
	ClearListDataAllDevReportView();

	for(int list_ind = 0; list_ind < devTypeNum; list_ind++)//设备类型下标
	{
		int dev_type = gDevTypeValue[list_ind];
		int report_num = 0;
		int ret = 0;
		BOOL suc = FALSE;
		char name[MAX_NAME_LEN];
		ZeroMemory(name,sizeof(name));

		char item[64];
		memset(item, 0, sizeof(item));

		int n_item = -1;
		while ((n_item = m_pListCtrl[list_ind].GetNextItem(n_item , LVNI_ALL)) > -1 )
		{
			report_num ++;
		}

		sprintf(item, "%d", list_ind + 1);
		list_ind = listAllDevReportView.InsertItem(list_ind, item);//插入行,上一行

		int ColumnInd = 1;
		sprintf(item, "%s", gDevTypeName[list_ind]);
		suc = listAllDevReportView.SetItemText(list_ind, ColumnInd++, item);

		sprintf(item, "%d", report_num);
		suc = listAllDevReportView.SetItemText(list_ind, ColumnInd++, item);

	}
	return;
}


void CReportFormat::OnBnClickedButtonCurDevDefaultParam()
{
	SetDefaultParamByListInd(currentDevTypeInd);

	SetListControlAllDevReportView();
}

void CReportFormat::SetDefaultParamByListInd(int list_ind)
{
	ClearListDataByListInd(list_ind);

	int row_ind = 1;//行号1-n
	for(int j = 0; j < pCParamDevSet->pCParamDevTypeArr[list_ind].paramInfNum; j++)//参数下标
	{
		char name[64];
		memset(name, 0, sizeof(name));

		sprintf(name, "%d", row_ind);
		int list_item_ind = m_pListCtrl[list_ind].InsertItem(row_ind++, name);//插入行

		int column_ind = 1;
		int param_id = pCParamDevSet->pCParamDevTypeArr[list_ind].pStParamInfoArr[j].param_id;
		sprintf(name, "%s", 
			GetAllDevTypeReportParamNameByValue(param_id));
		m_pListCtrl[list_ind].SetItemText(list_item_ind, column_ind++, name);

		sprintf(name, "%s", gDevTypeName[list_ind]);
		m_pListCtrl[list_ind].SetItemText(list_item_ind, column_ind++, name);
	}

	SetModify(pReportFormatStore->report_format.is_modify);
	return;
}

void CReportFormat::ClearListDataByListInd(int list_ind)
{
	while(m_pListCtrl[list_ind].GetNextItem(-1,LVNI_ALL) != -1)
	{
		int item = m_pListCtrl[list_ind].GetNextItem(-1,LVNI_ALL);
		BOOL is_delete = m_pListCtrl[list_ind].DeleteItem(item);
		is_delete = is_delete;
	}
}
