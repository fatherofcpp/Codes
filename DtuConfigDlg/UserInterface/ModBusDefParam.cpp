// UserInterface/ModBusDefParam.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "UserInterface/ModBusDefParam.h"
#include "afxdialogex.h"
#include "CommonFunction.h"
#include "dataConvert.h"

// CModBusDefParam 对话框

IMPLEMENT_DYNAMIC(CParamDefModbusDlg, CDialog)

CParamDefModbusDlg::CParamDefModbusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamDefModbusDlg::IDD, pParent)
{
	pCProtModbus = new CProtModbus;
	maxWayNum = MAX_WAY_NUM;
	listParamArr = new CListCtrl[maxWayNum];

	editProtName.SetMaxLen(64);
	editRegisterAddr.SetValidType(VALID_HEX);
	editRegisterAddr.SetMaxLen(7);
	editRegisterLen.SetValidType(VALID_INT);
	editRegisterLen.SetMaxNum(100);
	editWayNum.SetValidType(VALID_INT);
	editWayNum.SetMaxNum(12);
	editWayNum.SetMinNum(1);

	pOuterDev = NULL;
}

CParamDefModbusDlg::~CParamDefModbusDlg()
{
	delete pCProtModbus;
	delete[] listParamArr;
}

void CParamDefModbusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROT_TYPE, combSubType);
	DDX_Control(pDX, IDC_EDIT_PROT_NAME, editProtName);
	DDX_Control(pDX, IDC_COMBO_UNIT, combUnit);
	DDX_Control(pDX, IDC_EDIT_REGISTER_ADDR, editRegisterAddr);
	DDX_Control(pDX, IDC_EDIT_REGISTER_LEN, editRegisterLen);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, btModify);
	DDX_Control(pDX, IDC_BUTTON_OK, btOk);
	DDX_Control(pDX, IDC_COMBO_DEV_TYPE, combDevType);
	DDX_Control(pDX, IDC_EDIT_WAY_NUM, editWayNum);
	DDX_Control(pDX, IDC_STATIC_PARAM_MAP_INF, labParamMapInf);
	DDX_Control(pDX, IDC_COMBO_WAY_IND, combWayInd);
	DDX_Control(pDX, IDC_CHECK_REG_WAY_SAME, checkRegIsSame);
}


BEGIN_MESSAGE_MAP(CParamDefModbusDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CParamDefModbusDlg::OnBnClickedButtonModify)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PARAM_DYNAMIC, &CParamDefModbusDlg::OnNMClickListParam)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PARAM_DYNAMIC, &CParamDefModbusDlg::OnNMDblclkListParam)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CParamDefModbusDlg::OnBnClickedButtonOk)
	ON_CBN_SELCHANGE(IDC_COMBO_DEV_TYPE, &CParamDefModbusDlg::OnCbnSelchangeComboDevType)
	ON_CBN_SELCHANGE(IDC_COMBO_WAY_IND, &CParamDefModbusDlg::OnCbnSelchangeComboWayInd)
	ON_EN_CHANGE(IDC_EDIT_WAY_NUM, &CParamDefModbusDlg::OnEnChangeEditWayNum)
END_MESSAGE_MAP()

void CParamDefModbusDlg::SetModbusParam(CProtModbus* mod_def, VEC_INT vec_id)
{
	*pCProtModbus = mod_def;
	vecId = vec_id;
}

// CModBusDefParam 消息处理程序


BOOL CParamDefModbusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CreateAllWayList();

	// TODO:  在此添加额外的初始化
	CString str;
	int ret = 0;	
	char name[255];
	ZeroMemory(name,sizeof(name));

	int know_dev_num = 4;//gDevTypeNum
	for (int i = 0; i < know_dev_num; i++)
	{
		str.Format("%s",gDevTypeName[i]);
		combDevType.InsertString(i,str);
	}

	int dev_type = pCProtModbus->pStDefSubTypeInfo->dev_type;
	int dev_ind = GetDevTypeIndByValue(dev_type);
	combDevType.SetCurSel(dev_ind);

	SetCombDefProt();
	int id = pCProtModbus->pStDefSubTypeInfo->sub_type;
	int ind = id % 100 % 11;
	if (ind < 0)
	{
		ind = 0;
	}
	combSubType.SetCurSel(ind);

	checkRegIsSame.SetCheck(pCProtModbus->pStDefSubTypeInfo->is_reg_same);

	str.Format("%d",pCProtModbus->pStDefSubTypeInfo->way_num);
	editWayNum.SetWindowText(str);

	SetCombWayChoos();

	int dev_type_ind = combDevType.GetCurSel();
	assert(dev_type_ind >= 0);
	pCProtModbus->SetDevType(gDevTypeValue[dev_type_ind]);
	SetListParamAllWay();

	for (int i = 0; i < gDefProtUnitNum; i++)
	{
		str.Format("%s",gDefProtUnitName[i]);
		combUnit.InsertString(i,str);
	}
	combUnit.SetCurSel(2);

	if (strlen(pCProtModbus->pStDefSubTypeInfo->sub_type_name) != 0)
	{
		ZeroMemory(name,sizeof(name));
		ret = utf8_to_gbk(name, pCProtModbus->pStDefSubTypeInfo->sub_type_name,
			strlen(pCProtModbus->pStDefSubTypeInfo->sub_type_name));
		str.Format("%s",name);
		editProtName.SetWindowText(str);
	}
	else
	{
		str = GetSubTypeNameByValue(pCProtModbus->pStDefSubTypeInfo->sub_type);
		editProtName.SetWindowText(str);
	}

	str.Format("%s","1");
	editRegisterAddr.SetWindowText(str);
	str.Format("%s","1");
	editRegisterLen.SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CParamDefModbusDlg::PreTranslateMessage(MSG* pMsg)
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

void CParamDefModbusDlg::OnBnClickedButtonModify()
{
	CString str;
	int nRow = 0;
	int i = 1;
	st_param_info modbus_param;
	memset(&modbus_param, 0, sizeof(modbus_param));
	int way_ind = combWayInd.GetCurSel();
	assert(way_ind >= 0); 

	int item_count = listParamArr[way_ind].GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (item_count == -1)
	{
		return;
	}

	GetListInf(item_count, modbus_param, way_ind);

	listParamArr[way_ind].DeleteItem(item_count);

	int unit_ind = combUnit.GetCurSel();
	assert(unit_ind >= 0);
	double unit = gDefProtUnitValue[unit_ind];
	int all_ind = GetAllDevTypeReportParamIndByValue(modbus_param.param_id);
	double all_unit = gAllDevTypeReportParamUnit[all_ind];
	
	double rate = unit / all_unit;
	modbus_param.param_unit = MapUnitDoubleToInt(rate);

	editRegisterAddr.GetWindowText(str);
	if (str.GetLength() == 0)
	{
		MessageBox("寄存器地址非法");
		return;
	}
	str_to_int(str.GetString(), modbus_param.param_register_addr, 16);

	editRegisterLen.GetWindowText(str);
	if (str.GetLength() == 0)
	{
		MessageBox("寄存器长度非法");
		return;
	}
	str_to_int(str.GetString(), modbus_param.param_register_len);

	SetListInf(item_count, modbus_param, way_ind);//根据设备信息设置list相应行信息

}

void CParamDefModbusDlg::OnCbnSelchangeComboDevType()
{
	st_param_info param_info;
	for (int way_ind = 0; way_ind < maxWayNum; way_ind++)
	{
		int item = -1;
		while ((item = listParamArr[way_ind].GetNextItem(item, LVNI_ALL)) > -1 )
		{
			memset(&param_info, 0, sizeof(param_info));
			GetListInf(item, param_info, way_ind);
			if(param_info.param_register_len != 0)
			{
				MessageBox("切换设备主类型会导致当前编辑参数信息被清空");
				goto LEAVE;
			}
		}
	}

LEAVE:
	int dev_type_ind = combDevType.GetCurSel();
	assert(dev_type_ind >= 0);
	pCProtModbus->ChangeDevType(gDevTypeValue[dev_type_ind]);

	SetCombDefProt();
	SetListParamAllWay();
}


void CParamDefModbusDlg::OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonModify();
}

void CParamDefModbusDlg::OnNMClickListParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int way_ind = combWayInd.GetCurSel();
	assert(way_ind >= 0); 

	if ((listParamArr[way_ind].GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		ShowModButton(TRUE);

		st_param_info modbus_param;
		memset(&modbus_param, 0, sizeof(modbus_param));
		int item_count = listParamArr[way_ind].GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		GetListInf(item_count, modbus_param, way_ind);
		CString str;

		str.Format("选中参量名称:[%d][%s]", item_count + 1, GetAllDevTypeReportParamNameByValue(modbus_param.param_id));
		labParamMapInf.SetWindowText(str);
	}
	else
	{
		ShowModButton(FALSE);
	}
}

void CParamDefModbusDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int dev_ind = 0;
	CString str;

	dev_ind = combDevType.GetCurSel();
	assert(dev_ind >= 0);
	pCProtModbus->pStDefSubTypeInfo->dev_type = gDevTypeValue[dev_ind];


	combSubType.GetWindowText(str);
	assert(str.GetLength() > 0);
	int pre_sub_type = pCProtModbus->pStDefSubTypeInfo->sub_type;//保存的子类型 
	int sub_type = GetSubTypeValueByName(str.GetString());//界面的子类型
	assert(sub_type >= 0);

	for (unsigned int i = 0; i < vecId.size(); i++)
	{
		if (sub_type == vecId[i])
		{
			str.Format("自定义协议已经存在");
			MessageBox(str);
			return ;
		}
	}
	//判断当前自定义子类型是否被外接设备使用,如果使用要提示删除外接设备存在的当前子类型
	for (int i = 0; i < pOuterDev->port_num; i++)
	{
		for (int j = 0; j < pOuterDev->port_dev[i].dev_num; j++)
		{
			if (pOuterDev->port_dev[i].dev_inf[j].sub_type == sub_type)
			{
				//判断子类型，子类型名称，回路数是否改变
				MessageBox("提示:本协议为只读模式,若想修改,先删除使用当前自定义协议的设备");
				return;
			}

			if (pOuterDev->port_dev[i].dev_inf[j].sub_type == pre_sub_type)
			{
				//判断子类型，子类型名称，回路数是否改变
				MessageBox("提示:本协议为只读模式,若想修改,先删除使用当前自定义协议的设备");
				return;
			}
		}
	}

	editWayNum.GetWindowText(str);
	int way_num = 0;
	str_to_int(str.GetString(), way_num);
	if (str.GetLength() == 0 || way_num > 12 || way_num <= 0)
	{
		MessageBox("自定义协议回路数非法");
		return;
	}
	pCProtModbus->pStDefSubTypeInfo->way_num = way_num;
	pCProtModbus->pStDefSubTypeInfo->is_reg_same = (0 != checkRegIsSame.GetCheck());

	editProtName.GetWindowText(str);
	if (str.GetLength() == 0)
	{
		MessageBox("自定义协议名称非法");
		return;
	}

	int sub_type_ind = GetSubTypeIndByValue(sub_type);
	assert(sub_type_ind >= 0);

	//检测自定义子类型名称是否唯一
	for (int i = 0; i < gSubTypeNum; i ++)
	{
		if (0 == strcmp(str.GetString(), gSubTypeName[i]))
		{
			if (i != sub_type_ind)
			{
				MessageBox("自定义协议名称已存在");
				return;
			}
		}
	}
	gb2312_to_utf8(str.GetString(),
		str.GetLength(), pCProtModbus->pStDefSubTypeInfo->sub_type_name);

	//自定义子类型名称同步到gSubTypeName
	SetSubTypeNameByInd(sub_type_ind, (char*)str.GetString());

	//set subtype
	pCProtModbus->pStDefSubTypeInfo->sub_type = sub_type;

	//save list
	for (int way_ind = 0; way_ind < maxWayNum; way_ind++)
	{
		if (pCProtModbus->pStDefSubTypeInfo->is_reg_same && way_ind > 0)
		{
			break;
		}
		CParamDevType* p_CParamDevType 
			= &pCProtModbus->pCParamModbus->pCParamDevTypeArr[way_ind];
		//获取类表信息
		int item = -1;
		int ind = 0;
		while ((item = listParamArr[way_ind].GetNextItem(item, LVNI_ALL)) > -1 )
		{
			GetListInf(item, p_CParamDevType->pStParamInfoArr[ind++], way_ind);
		}
		p_CParamDevType->paramInfNum = ind;
	}

	CDialog::OnOK();
}

void CParamDefModbusDlg::ShowModButton( bool show )
{
	btModify.EnableWindow(show);
}

void CParamDefModbusDlg::SetCombDefProt()
{
	int com_ind = 0;
	int dev_type_ind = combDevType.GetCurSel();
	assert(dev_type_ind >= 0);

	combSubType.ResetContent();
	int dev_type = gDevTypeValue[dev_type_ind];
	for (int i = 0; i < gSubTypeNum; i++)
	{
		if ((gSubTypeValue[i] / 100 == dev_type) &&
			IsDefSubType(gSubTypeValue[i]) == true)
		{
			CString str;
			str.Format("%s",gSubTypeName[i]);
			combSubType.InsertString(com_ind ++,str);
		}
	}

	combSubType.SetCurSel(0);
}

void CParamDefModbusDlg::SetOuterDev( str_outer_dev* outer_dev )
{
	pOuterDev = outer_dev;
}

void CParamDefModbusDlg::OnCbnSelchangeComboWayInd()
{
	ShowListByWayInd();
}

void CParamDefModbusDlg::SetCombWayChoos()
{
	CString str;
	editWayNum.GetWindowText(str);
	int way_num = 0;
	str_to_int(str.GetString(), way_num);
	ModifyDefProtWayChooseNum(way_num);
	 
	combWayInd.ResetContent();
	for (int i = 0; i < gDefProtWayChooseNum; i++)
	{
		str.Format("%s",gDefProtWayChooseName[i]);
		combWayInd.InsertString(i,str);
	}
	combWayInd.SetCurSel(0);
}


void CParamDefModbusDlg::OnEnChangeEditWayNum()
{
	SetCombWayChoos();
}

void CParamDefModbusDlg::CreateAllWayList()
{
	CRect rect;
	this->GetClientRect(&rect);
	rect.top += (rect.bottom * 7 / 100);//7
	rect.bottom = rect.bottom * 86 / 100;//86
	rect.left += (rect.right * 46 / 100);//46
	rect.right = (rect.right * 95 / 100);//95

	CString strTemp;
	for (int i = 0; i < maxWayNum; i++)
	{
		listParamArr[i].Create(WS_CHILD | LVS_REPORT | LVS_EDITLABELS ,rect,this,IDC_LIST_PARAM_DYNAMIC);

		LONG l_style;
		l_style = GetWindowLong(listParamArr[i].m_hWnd, GWL_STYLE);//获取当前窗口style
		l_style &= ~LVS_TYPEMASK; //清除显示方式位
		l_style |= LVS_REPORT; //设置style
		SetWindowLong(listParamArr[i].m_hWnd, GWL_STYLE, l_style);//设置style

		listParamArr[i].SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); 

		//
		//DWORD dw_style = listParamArr[i].GetExtendedStyle();
		//dw_style |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
		//dw_style |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
		//listParamArr[i].SetExtendedStyle(dw_style); //设置扩展风格
		//
		int n_index = 0;
		int width = 200;
		listParamArr[i].InsertColumn(n_index++, "", LVCFMT_LEFT, 30 );
		listParamArr[i].InsertColumn(n_index++, "参数名称", LVCFMT_LEFT, width*90/100 );
		listParamArr[i].InsertColumn(n_index++, "单位", LVCFMT_LEFT, width* 30/100 );
		listParamArr[i].InsertColumn(n_index++, "寄存器地址", LVCFMT_LEFT, width*41/100 );
		listParamArr[i].InsertColumn(n_index++, "寄存器长度", LVCFMT_LEFT, width*45/100 );
	}

	ShowListByWayInd();
}

void CParamDefModbusDlg::ShowListByWayInd()
{
	for (int i = 0; i < maxWayNum; i++)
	{
		listParamArr[i].ShowWindow(false);    
	}
	int way_ind = combWayInd.GetCurSel();
	way_ind = way_ind >= 0 ? way_ind : 0; 
	listParamArr[way_ind].ShowWindow(true);  
}

void CParamDefModbusDlg::ClearListAllWay()
{
	//在手动加载时候，先删除已经存在的数据.
	for (int i = 0; i < maxWayNum; i++)
	{
		while(listParamArr[i].GetNextItem(-1,LVNI_ALL) != -1)
		{
			int item = listParamArr[i].GetNextItem(-1,LVNI_ALL);
			BOOL is_delete = listParamArr[i].DeleteItem(item);
			is_delete = is_delete;
		}
	}
}

void CParamDefModbusDlg::GetListInf(int row_ind, st_param_info& param_info, int way_ind)
{
	CString str;
	int ret = 0;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	int column_ind = 0;
	str = listParamArr[way_ind].GetItemText(row_ind, ++column_ind);
	param_info.param_id = GetAllDevTypeReportParamValueByName(str.GetString());

	str = listParamArr[way_ind].GetItemText(row_ind, ++column_ind);
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);

	double unit = GetValueByNameDefProtUnit(name);
	double rate_unit = unit / 
		gAllDevTypeReportParamUnit[GetAllDevTypeReportParamIndByValue(param_info.param_id)];
	param_info.param_unit = MapUnitDoubleToInt(rate_unit);

	str = listParamArr[way_ind].GetItemText(row_ind, ++column_ind);
	str_to_int(str.GetString(), param_info.param_register_addr, 16);

	str = listParamArr[way_ind].GetItemText(row_ind, ++column_ind);
	str_to_int(str.GetString(), param_info.param_register_len, gHexType);

	return;
}

void CParamDefModbusDlg::SetListInf(int row_ind, st_param_info& param_info, int way_ind)
{
	int ret = 0;
	BOOL suc = FALSE;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	char item[64];
	memset(item, 0, sizeof(item));

	sprintf(item, "%d", row_ind + 1);
	row_ind = listParamArr[way_ind].InsertItem(row_ind, item);//插入行,上一行

	int ColumnInd = 1;

	sprintf(item, "%s", GetAllDevTypeReportParamNameByValue(param_info.param_id));
	suc = listParamArr[way_ind].SetItemText(row_ind, ColumnInd++, item);

	int all_ind = GetAllDevTypeReportParamIndByValue(param_info.param_id);
	double normal_unit = gAllDevTypeReportParamUnit[all_ind];
	double rate_unit = MapUnitIntToDouble(param_info.param_unit); 
	double unit = (rate_unit * normal_unit);
	string name_unit = GetNameByValueDefProtUnit(unit);
	sprintf(item, "%s", name_unit.c_str());
	suc = listParamArr[way_ind].SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "0x%x", param_info.param_register_addr);
	suc = listParamArr[way_ind].SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%d", param_info.param_register_len);
	suc = listParamArr[way_ind].SetItemText(row_ind, ColumnInd++, item);
}

void CParamDefModbusDlg::SetListParamAllWay()
{
	ClearListAllWay();

	for (int way_ind = 0; way_ind < maxWayNum; way_ind++)
	{
		CParamDevType* p_CParamDevType 
			= &pCProtModbus->pCParamModbus->pCParamDevTypeArr[way_ind];
		for(int i = 0; i < p_CParamDevType->paramInfNum; i++)
		{
			SetListInf(i, p_CParamDevType->pStParamInfoArr[i], way_ind);//根据设备信息设置list相应行信息
		}
	}

}
