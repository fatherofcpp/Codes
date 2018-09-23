// UserInterface/ProtocolTemplate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "UserInterface/ProtocolTemplate.h"
#include "afxdialogex.h"
#include "CommonFunction.h"
#include "dataConvert.h"

#include "xmlAnalyze.h"
#include "log.h"
// CProtocolTemplate �Ի���

IMPLEMENT_DYNAMIC(CParamDefModbusSetDlg, CDialog)

CParamDefModbusSetDlg::CParamDefModbusSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamDefModbusSetDlg::IDD, pParent)
{
	isModify = false;

	pCProtModbusSet = new CProtModbusSet;
	pCParamDefModbusDlg = new CParamDefModbusDlg;
	outerDev = new str_outer_dev;
	memset(outerDev, 0, sizeof(*outerDev));
}

CParamDefModbusSetDlg::~CParamDefModbusSetDlg()
{
	delete pCProtModbusSet;
	delete pCParamDefModbusDlg;
}

void CParamDefModbusSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD, btAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE, btDelete);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, btModify);
	DDX_Control(pDX, IDC_LIST_PROTOCOL_TEMPLATE, listProtTemplate);
	DDX_Control(pDX, IDC_BUTTON_LOAD_LOCAL_DEF_SUBTYPE, btLoadLocalDef);
	DDX_Control(pDX, IDC_BUTTON_SAVE_LOCAL_DEF_SUBTYPE, btSaveLocalDef);
}


BEGIN_MESSAGE_MAP(CParamDefModbusSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CParamDefModbusSetDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CParamDefModbusSetDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CParamDefModbusSetDlg::OnBnClickedButtonModify)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROTOCOL_TEMPLATE, &CParamDefModbusSetDlg::OnNMClickListProtocolTemplate)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROTOCOL_TEMPLATE, &CParamDefModbusSetDlg::OnNMDblclkListProtocolTemplate)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LOCAL_DEF_SUBTYPE, &CParamDefModbusSetDlg::OnBnClickedButtonLoadLocalDefSubtype)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LOCAL_DEF_SUBTYPE, &CParamDefModbusSetDlg::OnBnClickedButtonSaveLocalDefSubtype)
END_MESSAGE_MAP()


// CProtocolTemplate ��Ϣ�������


BOOL CParamDefModbusSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowInput(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CParamDefModbusSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_RETURN: //�س�  
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

void CParamDefModbusSetDlg::SetModbusDefArray( CProtModbusSet* param_def_modbus_set )
{
	*pCProtModbusSet = param_def_modbus_set;
}

void CParamDefModbusSetDlg::InitDlg()
{
	ShowInput(true);

	CreateList();
	pCParamDefModbusDlg->SetOuterDev(outerDev);

	InitMapSubTypeProt();

	//���ֶ�����ʱ����ɾ���Ѿ����ڵ�����.
	while(listProtTemplate.GetNextItem(-1,LVNI_ALL) != -1)
	{
		int nItem = listProtTemplate.GetNextItem(-1,LVNI_ALL);
		listProtTemplate.DeleteItem(nItem);
	}

	for(int i = 0; i < pCProtModbusSet->protModbusNum; i++)
	{
		if (pCProtModbusSet->pCProtModbusArr[i].pStDefSubTypeInfo->dev_type > 0)
		{
			SetListInf(i, pCProtModbusSet->pCProtModbusArr[i]);//�����豸��Ϣ����list��Ӧ����Ϣ
		}
	}
}

void CParamDefModbusSetDlg::CreateList()
{
	LONG l_style;
	l_style = GetWindowLong(listProtTemplate.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	l_style &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	l_style |= LVS_REPORT; //����style
	SetWindowLong(listProtTemplate.m_hWnd, GWL_STYLE, l_style);//����style

	DWORD dw_style = listProtTemplate.GetExtendedStyle();
	dw_style |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dw_style |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	listProtTemplate.SetExtendedStyle(dw_style); //������չ���

	int n_index = 0;
	int width = 200;
	listProtTemplate.InsertColumn(n_index++, "", LVCFMT_LEFT, width * 18 / 100 );
	listProtTemplate.InsertColumn(n_index++, "�豸����", LVCFMT_LEFT, width * 100 /100);
	listProtTemplate.InsertColumn(n_index++, "Э������", LVCFMT_LEFT, width * 100 /100);
	listProtTemplate.InsertColumn(n_index++, "��·����", LVCFMT_LEFT, width * 100/100 );
	listProtTemplate.InsertColumn(n_index++, "����·�Ĵ�����Ϣ�Ƿ����һ��·��ͬ", LVCFMT_LEFT, width * 140/100 );

	ShowDelModCheckButton(FALSE);
}

void CParamDefModbusSetDlg::DisplaySaveButton()
{
	bool modify = false;

	modify = true;
	if (modify == true)
	{
		if (gComNum != 0)
		{
			GetParent()->GetDlgItem(IDC_BUTTON_SAVE_COM)->EnableWindow(true);
		}
	}
}

void CParamDefModbusSetDlg::ShowDelModCheckButton( bool show )
{
	btDelete.EnableWindow(show);
	btModify.EnableWindow(show);
	btSaveLocalDef.EnableWindow(show);
}

void CParamDefModbusSetDlg::SetListInf( int row_ind, CProtModbus& mod_temp )
{
	int ret = 0;

	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	char item[64];
	memset(item, 0, sizeof(item));

	sprintf(item, "%d", row_ind + 1);
	row_ind = listProtTemplate.InsertItem(row_ind, item);//������,��һ��

	int ColumnInd = 1;

	sprintf(item, "%s", GetDevTypeNameByValue(mod_temp.pStDefSubTypeInfo->dev_type));
	listProtTemplate.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", GetSubTypeNameByValue(mod_temp.pStDefSubTypeInfo->sub_type));
	listProtTemplate.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%d", mod_temp.pStDefSubTypeInfo->way_num);
	listProtTemplate.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%d", mod_temp.pStDefSubTypeInfo->is_reg_same);
	listProtTemplate.SetItemText(row_ind, ColumnInd++, item);

	SetParamInfById(mod_temp.pStDefSubTypeInfo->sub_type, *mod_temp.pCParamModbus);

}

int CParamDefModbusSetDlg::GetListInf( int row_ind, CProtModbus& mod_temp)
{
	CString str;
	int ret = 0;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	int column_ind = 0;

	str = listProtTemplate.GetItemText(row_ind, ++column_ind);
	mod_temp.pStDefSubTypeInfo->dev_type = GetDevTypeValueByName(str.GetString());

	str = listProtTemplate.GetItemText(row_ind, ++column_ind);
	mod_temp.pStDefSubTypeInfo->sub_type = GetSubTypeValueByName(str.GetString());
	assert(mod_temp.pStDefSubTypeInfo->sub_type >= 0);

	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	strcpy(mod_temp.pStDefSubTypeInfo->sub_type_name,name);

	str = listProtTemplate.GetItemText(row_ind, ++column_ind);
	str_to_int(str.GetString(), mod_temp.pStDefSubTypeInfo->way_num, gHexType);

	str = listProtTemplate.GetItemText(row_ind, ++column_ind);
	int val = 0;
	str_to_int(str.GetString(), val, gHexType);
	mod_temp.pStDefSubTypeInfo->is_reg_same = (val != 0);

	ret = GetParamInfById( mod_temp.pStDefSubTypeInfo->sub_type, *mod_temp.pCParamModbus);

	return ret;
}

void CParamDefModbusSetDlg::InitMapSubTypeProt()
{
	mapSubTypeProt.clear();

	for(int i = 0; i < pCProtModbusSet->protModbusNum; i++)
	{
		int sub_type = pCProtModbusSet->pCProtModbusArr[i].pStDefSubTypeInfo->sub_type;
		int sub_type_ind = GetSubTypeIndByValue(sub_type);
		if(sub_type_ind >= 0)
		{
			int p_key = (int)(pCProtModbusSet->pCProtModbusArr[i].pCParamModbus);
			mapSubTypeProt.insert
				(make_pair(sub_type, p_key));
		}
	}

}


void CParamDefModbusSetDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	int ret = 0;

	ShowDelModCheckButton(FALSE);

	ret = listProtTemplate.GetItemCount();
	if (ret >= MAX_DEFINE_MODBUS_PROTOCOL)
	{
		str.Format("�Ѵ�����%d",MAX_DEFINE_MODBUS_PROTOCOL);
		MessageBox(str);
		return;
	}

	CProtModbus* prot_modbus = new CProtModbus;

	VEC_INT vec_id;
	GetAllIdExceptParam(vec_id, prot_modbus->pStDefSubTypeInfo->sub_type);

	//���ѡ��ĳ�豸�����Ը��豸��Ϣ���ӶԻ�����ʾ

	int n_item = 0;
	if ((n_item = listProtTemplate.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		GetListInf(n_item, *prot_modbus);
	}
	else
	{
		//�����ӶԻ���Ϊ��
	}

	pCParamDefModbusDlg->SetModbusParam(prot_modbus, vec_id);
	delete prot_modbus;
	ret = pCParamDefModbusDlg->DoModal();
	if(IDOK == ret)
	{
		int item_count = listProtTemplate.GetItemCount();

		//����map
		AddMapIdProt(pCParamDefModbusDlg->pCProtModbus->pStDefSubTypeInfo->sub_type, item_count);

		SetListInf(item_count, *(pCParamDefModbusDlg->pCProtModbus));//�����豸��Ϣ����list��Ӧ����Ϣ

	/*	int dev_type_ind = GetDevTypeIndByValue(pCParamDefModbusDlg->pCParamDefModbus->pProtInfo->dev_type);
		for(int i = 0; i < pCParamDefModbusDlg->pCParamDefModbus->pCParamDevSet->pParamDevSingleArr[dev_type_ind].paramNum; i++)
		{
			pCParamDefModbusDlg->pCParamDefModbus->pCParamDevSet->pParamDevSingleArr[dev_type_ind].paramInfArr[i];
		}*/

		//set modify
		isModify = true;
		DisplaySaveButton();
	}
}


void CParamDefModbusSetDlg::OnBnClickedButtonDelete()
{
	int sub_type = 0;
	ShowDelModCheckButton(FALSE);
	while(listProtTemplate.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int n_item = listProtTemplate.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		CProtModbus* prot_modbus = new CProtModbus;
		GetListInf(n_item, *prot_modbus);
		sub_type = prot_modbus->pStDefSubTypeInfo->sub_type;
		delete prot_modbus;

		//�жϵ�ǰ�Զ����������Ƿ�����豸ʹ��,�类ʹ��Ҫ��ʾ��ɾ������豸���ڵĵ�ǰ������
		for (int i = 0; i < outerDev->port_num; i++)
		{
			for (int j = 0; j < outerDev->port_dev[i].dev_num; j++)
			{
				if (outerDev->port_dev[i].dev_inf[j].sub_type == sub_type)
				{
					MessageBox("����ɾ������豸�еĵ�ǰ�Զ���Э���豸");
					return;
				}
			}
		}

		//ɾ��map
		DelMapIdProt(sub_type);

		listProtTemplate.DeleteItem(n_item);//ɾ��
		int item = -1;
		while ((item = listProtTemplate.GetNextItem(item, LVNI_ALL)) > -1 )
		{
			char name[32] = "";
			sprintf(name, "%d", item + 1);
			listProtTemplate.SetItemText(item, 0, name);
		}

		isModify = true;
		DisplaySaveButton();
	}
}


void CParamDefModbusSetDlg::OnBnClickedButtonModify()
{
	CString str;
	int ret = 0;
	int pre_sub_type = 0;//�޸Ĺ��̣������������Ƹı䣬�޷��ҵ���Ӧ������

	ShowDelModCheckButton(FALSE);

	CProtModbus* prot_modbus = new CProtModbus;
	int n_item = 0;
	if ((n_item = listProtTemplate.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		GetListInf(n_item, *prot_modbus);
	}
	else
	{
		return;
	}
	pre_sub_type = prot_modbus->pStDefSubTypeInfo->sub_type;

	VEC_INT vec_id;
	GetAllIdExceptParam(vec_id, prot_modbus->pStDefSubTypeInfo->sub_type);

	pCParamDefModbusDlg->SetModbusParam(prot_modbus, vec_id);
	delete prot_modbus;

	ret = pCParamDefModbusDlg->DoModal();
	if(IDOK == ret)
	{
		int item_count = listProtTemplate.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		if (item_count == -1)
		{
			return;
		}
		//ɾ��map
		DelMapIdProt(pre_sub_type);
		listProtTemplate.DeleteItem(item_count);

		//����map
		AddMapIdProt(pCParamDefModbusDlg->pCProtModbus->pStDefSubTypeInfo->sub_type, item_count);
		SetListInf(item_count, *(pCParamDefModbusDlg->pCProtModbus));//�����豸��Ϣ����list��Ӧ����Ϣ
		//set modify
		isModify = true;
		DisplaySaveButton();
	}
}

void CParamDefModbusSetDlg::GetAllIdExceptParam( VEC_INT& vec_id,int& except_id )
{
	SaveDlg();
	for (int i = 0; i < pCProtModbusSet->protModbusNum; i++)
	{
		if (pCProtModbusSet->pCProtModbusArr[i].pStDefSubTypeInfo->sub_type != except_id)
		{
			vec_id.push_back(pCProtModbusSet->pCProtModbusArr[i].pStDefSubTypeInfo->sub_type);
		}
	}
}

int CParamDefModbusSetDlg::GetParamInfById( int id, CParamModbus& param_modbus)
{
	int second = GetMapSecondByFirst(id, mapSubTypeProt);
	if(second == -1)
	{
		return -1;
	}

	CParamModbus* p_key = (CParamModbus*)second;
	param_modbus = p_key;

	return 0;
}

int CParamDefModbusSetDlg::SetParamInfById( int id, CParamModbus& param_modbus)
{
	int second = GetMapSecondByFirst(id, mapSubTypeProt);
	if(second == -1)
	{
		return -1;
	}

	CParamModbus* p_key = (CParamModbus*)second;
	*p_key = &param_modbus;
	return 0;
}

int CParamDefModbusSetDlg::SaveDlg()
{
	//���Э����Ϣ
	for (int i = 0; i < pCProtModbusSet->protModbusNum; i++)
	{
		pCProtModbusSet->pCProtModbusArr[i].ClearStDefSubTypeInf();
	}

	int item = -1;
	int row_ind = -1;//�к�

	while ((item = listProtTemplate.GetNextItem(item, LVNI_ALL)) > -1 )
	{
		row_ind ++;
		assert(row_ind + 1 <= MAX_DEFINE_MODBUS_PROTOCOL);

		CProtModbus* modbus_template = new CProtModbus;
		GetListInf(item, *modbus_template);
		int sub_type_ind = GetSubTypeIndByValue(modbus_template->pStDefSubTypeInfo->sub_type);
		assert(sub_type_ind >= 0);
		delete modbus_template;

		GetListInf(item, pCProtModbusSet->pCProtModbusArr[row_ind]);

	}

	//�������Զ�����������Ϣ�󣬰����������ƣ������ͻ�·������������Ϣ���µ�ȫ������
	//gSubTypeName,gWayCountValue,���ⲿ����
	//������Զ������������ƻ�·��Ϣ
	ClearDefSubTypeNameWayCount();
	for (int i = 0; i < pCProtModbusSet->protModbusNum; i++)
	{
		int sub_type_ind = 
			GetSubTypeIndByValue(pCProtModbusSet->pCProtModbusArr[i].
			pStDefSubTypeInfo->sub_type);
		if(sub_type_ind >= 0)
		{
			if (IsDefSubType(gSubTypeValue[sub_type_ind]) == true)
			{
				char name[64];
				ZeroMemory(name,sizeof(name));
				utf8_to_gbk(name, pCProtModbusSet->pCProtModbusArr[i].pStDefSubTypeInfo->sub_type_name,
					strlen(pCProtModbusSet->pCProtModbusArr[i].pStDefSubTypeInfo->sub_type_name));
				SetSubTypeNameByInd(sub_type_ind, name);

				SetWayCountBySubTypeInd(sub_type_ind, 
					pCProtModbusSet->pCProtModbusArr[i].pStDefSubTypeInfo->way_num);
			}
		}

	}
	return 0;
}



void CParamDefModbusSetDlg::OnNMClickListProtocolTemplate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if ((listProtTemplate.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		ShowDelModCheckButton(TRUE);
	}
	else
	{
		ShowDelModCheckButton(FALSE);
	}
}


void CParamDefModbusSetDlg::OnNMDblclkListProtocolTemplate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	OnBnClickedButtonModify();
}

void CParamDefModbusSetDlg::AddMapIdProt( int id, int prot_modbus_arr_ind)
{
	int sub_type_ind = GetSubTypeIndByValue(id);
	assert(sub_type_ind >= 0);

	assert(prot_modbus_arr_ind >=0 && prot_modbus_arr_ind < MAX_DEFINE_MODBUS_PROTOCOL);

	int p_key = (int)(pCProtModbusSet->pCProtModbusArr[prot_modbus_arr_ind].pCParamModbus);
	mapSubTypeProt.insert
		(make_pair(id, p_key));
}

void CParamDefModbusSetDlg::DelMapIdProt( int id)
{
	for (MAP_INT_INT::iterator i = mapSubTypeProt.begin(); i != mapSubTypeProt.end();)
	{
		if (i->first == id)
		{
			i = mapSubTypeProt.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void CParamDefModbusSetDlg::ShowInput( bool show )
{
	btAdd.EnableWindow(show);
	btDelete.EnableWindow(show);
	btModify.EnableWindow(show);
	btLoadLocalDef.EnableWindow(show);
	btSaveLocalDef.EnableWindow(show);
}


void CParamDefModbusSetDlg::OnBnClickedButtonLoadLocalDefSubtype()
{
	int ret = 0;
	CString str;

	CProtModbus* def_sub_type_modbus = new CProtModbus;

	VEC_INT vec_id;
	GetAllIdExceptParam(vec_id, def_sub_type_modbus->pStDefSubTypeInfo->sub_type);

	ShowDelModCheckButton(FALSE);

	CString file_path = BootOpenFile();
	ret = gXmlAnalyze.DefSubTypeLocalLoad(file_path.GetString(), def_sub_type_modbus);
	if(ret == 0)
	{
		for (unsigned int i = 0; i < vec_id.size(); i++)
		{
			if (def_sub_type_modbus->pStDefSubTypeInfo->sub_type == vec_id[i])
			{
				str.Format("�Զ���Э���Ѿ�����,����ʧ��");
				MessageBox(str);
				str.Format("[%s]���ر����Զ���������ʧ��,�Զ���Э���Ѿ�����", __FUNCTION__);
				gLog.Add(str);
				delete def_sub_type_modbus;
				return;
			}
		}

		int item_count = listProtTemplate.GetItemCount();
		//����map
		AddMapIdProt(def_sub_type_modbus->pStDefSubTypeInfo->sub_type, item_count);
		SetListInf(item_count, *(def_sub_type_modbus));//�����豸��Ϣ����list��Ӧ����Ϣ
		//set modify
		isModify = true;
		DisplaySaveButton();

		MessageBox("���ر����Զ��������ͳɹ�");
	}
	else
	{
		str.Format("[%s]���ر����Զ���������ʧ��,ret=%d",__FUNCTION__, ret);
		MessageBox("���ر����Զ���������ʧ��");
		gLog.Add(str);
	}
	delete def_sub_type_modbus;
}


void CParamDefModbusSetDlg::OnBnClickedButtonSaveLocalDefSubtype()
{
	int ret = 0;
	CString str;
	CProtModbus* def_sub_type_modbus = new CProtModbus;
	
	ShowDelModCheckButton(FALSE);

	int n_item = 0;
	if ((n_item = listProtTemplate.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		GetListInf(n_item, *def_sub_type_modbus);
	}
	else
	{
		return;
	}

	CString file_path = BootOpenFile(false);
	ret = gXmlAnalyze.DefSubTypeLocalSave(file_path.GetString(), def_sub_type_modbus);
	if(ret == 0)
	{
		MessageBox("���汾���Զ��������ͳɹ�");
	}
	else
	{
		str.Format("[%s]���汾���Զ���������ʧ��,ret=%d",__FUNCTION__, ret);
		MessageBox("���汾���Զ���������ʧ��");
		gLog.Add(str);
	}
	delete def_sub_type_modbus;
}

void CParamDefModbusSetDlg::SetOuterDev( str_outer_dev* outer_dev )
{
	memcpy(outerDev, outer_dev, sizeof(*outerDev));
}
