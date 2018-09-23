// PortA.cpp : 实现文件
//

/*
端口数据utf8->端口对话框gb232
端口对话框gb232->回路数据gb232
回路数据gb232->回路对话框gb232
回路对话框gb232->回路数据utf8
回路数据utf8->端口对话框gb232
端口对话框gb232->数据utf8
*/
#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "PortA.h"
#include "log.h"
#include "dataConvert.h"
#include "CommonFunction.h"
#include "DtuConfigDlgDlg.h"

// COuterDevPort 对话框

IMPLEMENT_DYNAMIC(COuterDevPort, CDialogEx)

	COuterDevPort::COuterDevPort(CWnd* pParent /*=NULL*/)
	: CDialog(COuterDevPort::IDD, pParent)
{
	pAddDev = new CAddDev;
	pCheckWay = new CShowDevAllWayData;
	pPortDev = new st_port_dev;
	memset(pPortDev,0,sizeof(*pPortDev));
	portInd = 0;
	dataFrame = NULL;
	pProgress = new CProgress;

}

COuterDevPort::~COuterDevPort()
{
	delete pAddDev;
	delete pCheckWay;
	delete pPortDev;
	delete pProgress;

}

void COuterDevPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PORTA, mList);
	DDX_Control(pDX, IDC_BUTTON_DELETE, mDeleteDev);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, mModifyDev);
	DDX_Control(pDX, IDC_BUTTON_CHECK, mCheckDev);
}


BEGIN_MESSAGE_MAP(COuterDevPort, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &COuterDevPort::OnBnClickedDeleteDev)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &COuterDevPort::OnBnClickedAddDev)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &COuterDevPort::OnBnClickedModify)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &COuterDevPort::OnBnClickedCheckSelectedDev)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PORTA, &COuterDevPort::OnNMClickListPorta)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PORTA, &COuterDevPort::OnNMDblclkListPorta)
END_MESSAGE_MAP()


// COuterDevPort 消息处理程序


void COuterDevPort::OnBnClickedDeleteDev()
{
	ShowDelModCheckButton(FALSE);
	while(mList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int n_item = mList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);

		//记录删除设备信息
		st_dev_inf dev_inf;
		memset(&dev_inf,0,sizeof(dev_inf));
		GetListInf(n_item, dev_inf);

		mList.DeleteItem(n_item);//删除
		int item = -1;
		while ((item = mList.GetNextItem(item, LVNI_ALL)) > -1 )
		{
			char name[32] = "";
			sprintf(name, "%d", item + 1);
			mList.SetItemText(item, 0, name);
		}

		pPortDev->is_modify = true;
		DisplaySaveButton();
	}
}

void COuterDevPort::OnBnClickedAddDev()
{
	CString str;
	int ret = 0;

	ShowDelModCheckButton(FALSE);

	ret = mList.GetItemCount();
	if (ret >= MAX_DEV_NUM)
	{
		str.Format("已达上限%d",MAX_DEV_NUM);
		MessageBox(str);
		return;
	}

	st_dev_inf dev_inf;
	memset(&dev_inf,0,sizeof(dev_inf));
	dev_inf.rs485_conf.port = GetPortInd();
	dev_inf.dev_addr.id = 0;
	dev_inf.sub_type = (e_dev_sub_type)(-1);

	st_devid_subtype id_subtype;
	ZeroMemory(&id_subtype,sizeof(id_subtype));
	id_subtype.id = dev_inf.dev_addr.id;
	id_subtype.sub_type = dev_inf.sub_type;

	VEC_DEVID_SUBTYPE vec_id_subtype;
	GetAllPortDevIdSubTypeExceptParam(vec_id_subtype,id_subtype);

	VEC_CSTRING vec_mw_name;
	GetAllPortWayNameExceptParam(vec_mw_name,dev_inf.way_inf);

	VEC_INT vec_way_id;
	GetAllPortWayIdExceptParam(vec_way_id, dev_inf.way_inf);//统计除了参数way_inf之外的所有接口设备的回路id集合

	//如果选中某设备，则以改设备信息在子对话框显示
	int n_item = 0;
	if ((n_item = mList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		memset(&dev_inf,0,sizeof(dev_inf));
		GetListInf(n_item, dev_inf);
	}
	else
	{
		//否则子对话框为空
	}

	pAddDev->SetDevInf(&dev_inf, vec_id_subtype, vec_mw_name, vec_way_id, this);

	ret = pAddDev->DoModal();
	if(IDOK == ret)
	{
		int item_count = mList.GetItemCount();
		SetListInf(item_count, *(pAddDev->pDevInf));//根据设备信息设置list相应行信息

		//保存回路信息
		st_devid_subtype id_subtype;
		ZeroMemory(&id_subtype,sizeof(id_subtype));
		id_subtype.id = pAddDev->pDevInf->dev_addr.id;
		id_subtype.sub_type = pAddDev->pDevInf->sub_type;
		SetWayInfByIdSubType(id_subtype,pAddDev->pDevInf->way_inf);

		//set modify
		pPortDev->is_modify = true;
		DisplaySaveButton();
	}
}

void COuterDevPort::CreateList()
{
	LONG l_style;
	l_style = GetWindowLong(mList.m_hWnd, GWL_STYLE);//获取当前窗口style
	l_style &= ~LVS_TYPEMASK; //清除显示方式位
	l_style |= LVS_REPORT; //设置style
	SetWindowLong(mList.m_hWnd, GWL_STYLE, l_style);//设置style

	DWORD dw_style = mList.GetExtendedStyle();
	dw_style |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dw_style |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	mList.SetExtendedStyle(dw_style); //设置扩展风格

	int n_index = 0;
	int width = 200;
	mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
	mList.InsertColumn(n_index++, "地址", LVCFMT_LEFT, width/2 );
	mList.InsertColumn(n_index++, "名称", LVCFMT_LEFT, width/2 );
	mList.InsertColumn(n_index++, "设备类型", LVCFMT_LEFT, width*65/100);
	mList.InsertColumn(n_index++, "设备子类型", LVCFMT_LEFT, width*130/100);
	mList.InsertColumn(n_index++, "设备编号", LVCFMT_LEFT, width*130/100);
	mList.InsertColumn(n_index++, "回路掩码", LVCFMT_LEFT, width*30/100);
	mList.InsertColumn(n_index++, "数据位", LVCFMT_LEFT, width* 24/100 );
	mList.InsertColumn(n_index++, "停止位", LVCFMT_LEFT, width* 24/100 );
	mList.InsertColumn(n_index++, "波特率", LVCFMT_LEFT, width* 24/100 );
	mList.InsertColumn(n_index++, "校验", LVCFMT_LEFT, width*18/100 );
	mList.InsertColumn(n_index++, "硬件版本号", LVCFMT_LEFT, width*3/8 );
	mList.InsertColumn(n_index++, "软件版本号", LVCFMT_LEFT, width*3/8 );
	mList.InsertColumn(n_index++, "状态", LVCFMT_LEFT, width* 70/100);

	ShowDelModCheckButton(FALSE);

}

void COuterDevPort::InitDlg()
{
	InitWayInfWithIdSubType();

	//在手动加载时候，先删除已经存在的数据.
	while(mList.GetNextItem(-1,LVNI_ALL) != -1)
	{
		int nItem = mList.GetNextItem(-1,LVNI_ALL);
		mList.DeleteItem(nItem);
	}

	for(int i = 0; i < pPortDev->dev_num; i++)
	{
		SetListInf(i, pPortDev->dev_inf[i]);//根据设备信息设置list相应行信息
	}
}

void COuterDevPort::SetPortDev(st_port_dev* pOuterDev)
{
	memcpy(pPortDev, pOuterDev, sizeof(*pPortDev));
}

void COuterDevPort::SaveDlg()
{
	int item = -1;
	int dev_ind = -1;//行号

	while ((item = mList.GetNextItem(item, LVNI_ALL)) > -1 )
	{
		dev_ind++;
		GetListInf(item, pPortDev->dev_inf[dev_ind]);
	}

	pPortDev->dev_num = dev_ind + 1;

	assert(pPortDev->dev_num <= MAX_DEV_NUM);
	return;
}


void COuterDevPort::OnBnClickedModify()
{
	ShowDelModCheckButton(FALSE);

	int n_item = 0;
	if ((n_item = mList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		st_dev_inf dev_inf;
		memset(&dev_inf,0,sizeof(dev_inf));
		GetListInf(n_item, dev_inf);

		st_devid_subtype id_sub_type;
		ZeroMemory(&id_sub_type,sizeof(id_sub_type));
		id_sub_type.id = dev_inf.dev_addr.id;
		id_sub_type.sub_type = dev_inf.sub_type;

		VEC_DEVID_SUBTYPE vec_id_subtype;
		GetAllPortDevIdSubTypeExceptParam(vec_id_subtype,id_sub_type);

		VEC_CSTRING vec_mw_name;
		GetAllPortWayNameExceptParam(vec_mw_name,dev_inf.way_inf);

		VEC_INT vec_way_id;
		GetAllPortWayIdExceptParam(vec_way_id, dev_inf.way_inf);//统计除了参数way_inf之外的所有接口设备的回路id集合

		pAddDev->SetDevInf(&dev_inf, vec_id_subtype, vec_mw_name, vec_way_id, this);

		int ret = pAddDev->DoModal();
		if(IDOK == ret)
		{
			int item_count = mList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
			if (item_count == -1)
			{
				return;
			}

			mList.DeleteItem(item_count);

			SetListInf(item_count, *(pAddDev->pDevInf));//根据设备信息设置list相应行信息

			//保存回路信息
			st_devid_subtype id_sub_type;
			ZeroMemory(&id_sub_type,sizeof(id_sub_type));
			id_sub_type.id = pAddDev->pDevInf->dev_addr.id;
			id_sub_type.sub_type = pAddDev->pDevInf->sub_type;
			SetWayInfByIdSubType(id_sub_type, pAddDev->pDevInf->way_inf);

			//set modify
			pPortDev->is_modify = true;
			DisplaySaveButton();
		}
	}
}

UINT __cdecl GetRealDataThread(void *p)
{
	COuterDevPort* p_this = (COuterDevPort*)p;
	st_dev_inf* p_dev_inf = (st_dev_inf*)p_this->dataFrame->threadParam.dev_inf;
	VEC_DEV_ALL_WAY_DATA_INF* p_vec_way_check_inf = 
		(VEC_DEV_ALL_WAY_DATA_INF*)p_this->dataFrame->threadParam.way_check_inf;

	int ret = 0;
	CTaskParam* data_frame = NULL;
	CString str;
	st_dev_inf dev_inf;
	memcpy(&dev_inf,p_dev_inf,sizeof(dev_inf));
	int way_ind = 0;
	int way_num = GetWayNumBySubTypeValue(dev_inf.sub_type);

	if (p_this->dataFrame != NULL)
	{
		data_frame = p_this->dataFrame;
		switch (data_frame->threadParam.type_thread)
		{
		case REALTIME_DATA_THREAD:
			for (int i = 0; i < way_num; i++)
			{
				way_ind = way_num - i - 1;//读取实时数据的方向
				bool way_used = IsWayUsedByMask(way_ind, dev_inf.way_inf.way_mask);
				if (way_used)
				{
					u_w_measure w_measure;
					ZeroMemory(&w_measure,sizeof(w_measure));

					e_dev_type dev_type = (e_dev_type)(dev_inf.sub_type / 100);
					e_dev_sub_type sub_type = dev_inf.sub_type;
					dev_inf.way_inf.way_data[way_ind].way_id;
					ret = p_this->GetRealData(p_this->GetPortInd(), 
						dev_inf.dev_addr.id, sub_type, way_ind, w_measure);
					if (ret == 0)
					{
						st_way_data_inf way_check_inf;
						way_check_inf.dev_type = dev_type;
						way_check_inf.way_ind = way_ind;
						memcpy(&way_check_inf.measure,&w_measure,sizeof(w_measure));
						p_vec_way_check_inf->push_back(way_check_inf);
					}
					else
					{
						ret = -1;
						break;
					}
				}
			}

			if (ret == 0)
			{
				data_frame->tskResult = 0;
			}
			else
			{
				data_frame->tskResult = -1;
			}

			p_this->CloseProgress();
			break;

		default:
			break;
		}
	}

	return 0;
}

int COuterDevPort::AddThreadTsk( e_thread_type type,st_dev_inf* dev_inf,VEC_DEV_ALL_WAY_DATA_INF* way_check_inf)
{
	CString str("读取实时数据");
	int ret = 0;
	switch (type)
	{
	case REALTIME_DATA_THREAD:
		taskLock.Lock();
		if (dataFrame == NULL)
		{
			st_thread_tsk_param thread_param;
			memset(&thread_param, 0, sizeof(thread_param));
			thread_param.type_thread = REALTIME_DATA_THREAD;
			thread_param.dev_inf = dev_inf;
			thread_param.way_check_inf = way_check_inf;

			CTaskParam* data_frame = new CTaskParam(thread_param);
			dataFrame = data_frame; 

			AfxBeginThread( GetRealDataThread, this,THREAD_PRIORITY_ABOVE_NORMAL);

			pProgress->SetProgressName(str);
			pProgress->DoModal();

			if (data_frame->tskResult == 0)
			{
				str.Format("读取实时数据成功");
			}
			else
			{
				str.Format("读取实时数据失败");
			}
			ret = data_frame->tskResult; 
			gLog.Add(str);
			if(data_frame)
			{
				delete data_frame;
				dataFrame = NULL;
			}
		}
		taskLock.Unlock();
		break;
	default:
		break;
	}
	return ret;

}

void COuterDevPort::OnBnClickedCheckSelectedDev()
{
	int ret = 0;
	CString str;
	ret = pParent->pParent->SetConnectFdTimeOut(REALTIME_DATA_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	ShowDelModCheckButton(FALSE);

	int n_item = -1;
	if ((n_item = mList.GetNextItem(n_item, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		st_dev_inf dev_inf;
		memset(&dev_inf,0,sizeof(dev_inf));
		int column_ind = GetListInf(n_item, dev_inf);

		VEC_DEV_ALL_WAY_DATA_INF dev_all_way_data;//回路检测信息
		ret = AddThreadTsk(REALTIME_DATA_THREAD,&dev_inf,&dev_all_way_data);
		if (ret == 0)
		{
			pCheckWay->SetDevAllWayData(dev_inf.dev_type, dev_all_way_data, &gXmlAnalyze.pReportFormatStoreInf->report_format);
			ret = pCheckWay->DoModal();
			if(IDOK == ret)
			{
				str = "成功";
			}
			else
			{
				str.Format("回路数据无效");
			}
		}
		else
		{
			str.Format("回路数据读取失败");
		}

		pParent->pParent->ParseTskResult(ret);


		BOOL is_set = mList.SetItemText(n_item, ++column_ind,str);
	}
}

int COuterDevPort::GetRealData(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type, int way_ind, u_w_measure& w_measure)
{
	int rtn = 0;
	int len = 0;

	e_dev_type dev_type = (e_dev_type)(sub_type / 100);

	len = gXmlAnalyze.FormGetRealTimeDataCmd(port_ind, dev_id,sub_type, way_ind);
	rtn = gTransf.SendReqAndReceiveBody(len, READ_REALTIME_DATA_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetRealTimeDataRsp(w_measure, dev_type);
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
	return 0;
}

void COuterDevPort::DisplaySaveButton()
{
	bool modify = false;

	modify = true;
	if (modify == true)
	{
		if (gComNum != 0)
		{
			GetParent()->GetParent()->GetDlgItem(IDC_BUTTON_SAVE_COM)->EnableWindow(true);
		}
	}
}


void COuterDevPort::OnNMClickListPorta(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if ((mList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)) > -1 )
	{
		ShowDelModCheckButton(TRUE);
	}
	else
	{
		ShowDelModCheckButton(FALSE);
	}
}

void COuterDevPort::ShowDelModCheckButton( bool show )
{
	mDeleteDev.EnableWindow(show);
	mModifyDev.EnableWindow(show);
	mCheckDev.EnableWindow(show);
}

void COuterDevPort::GetWayInfByIdSubType(st_devid_subtype& id_subtype, st_way_inf& way_inf )
{
	for (unsigned int i = 0; i < vecWayInfWithIdSubType.size(); i++)
	{
		if (vecWayInfWithIdSubType[i].dev_id_subtype.id == id_subtype.id &&
			vecWayInfWithIdSubType[i].dev_id_subtype.sub_type == id_subtype.sub_type)
		{
			memcpy(way_inf.way_data,vecWayInfWithIdSubType[i].way_inf.way_data,sizeof(way_inf.way_data));
		}
	}
}

void COuterDevPort::SetWayInfByIdSubType(st_devid_subtype& id_subtype,st_way_inf& way_inf)
{
	st_way_inf_id_subtype way_inf_id_sub_type;
	ZeroMemory(&way_inf_id_sub_type,sizeof(st_way_inf_id_subtype));
	way_inf_id_sub_type.dev_id_subtype.id = id_subtype.id;
	way_inf_id_sub_type.dev_id_subtype.sub_type = id_subtype.sub_type;
	memcpy(&way_inf_id_sub_type.way_inf,&way_inf,sizeof(way_inf_id_sub_type.way_inf));
	bool is_new = true;
	for (MU_INT32 i = 0; i < vecWayInfWithIdSubType.size(); i ++)
	{
		if (vecWayInfWithIdSubType[i].dev_id_subtype.id == id_subtype.id &&
			vecWayInfWithIdSubType[i].dev_id_subtype.sub_type == id_subtype.sub_type)
		{
			vecWayInfWithIdSubType[i] = way_inf_id_sub_type;
			is_new = false;
		}
	}
	if (is_new)
	{
		vecWayInfWithIdSubType.push_back(way_inf_id_sub_type);
	}
}

void COuterDevPort::GetAllPortDevIdSubTypeExceptParam(VEC_DEVID_SUBTYPE& vec_id_subtype,st_devid_subtype& id_subtype)
{
	CString str;

	str_outer_dev* pouter_dev = new str_outer_dev;
	memset(pouter_dev, 0, sizeof(*pouter_dev));

	pParent->SaveDlg();

	pouter_dev->port_num = pParent->portNum;
	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		memcpy(&pouter_dev->port_dev[i],pParent->pPort[i]->pPortDev,sizeof(pouter_dev->port_dev[i]));
	}

	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		for(int j = 0; j < pouter_dev->port_dev[i].dev_num; j++)
		{
			st_devid_subtype str_id_subtype;
			str_id_subtype.id = pouter_dev->port_dev[i].dev_inf[j].dev_addr.id;
			str_id_subtype.sub_type = pouter_dev->port_dev[i].dev_inf[j].sub_type;
			if(str_id_subtype.id != id_subtype.id 
				|| str_id_subtype.sub_type != id_subtype.sub_type)
			{
				vec_id_subtype.push_back(str_id_subtype);
			}
		}
	}

	delete pouter_dev;
}


void COuterDevPort::OnNMDblclkListPorta(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedModify();
}

void COuterDevPort::SetPortInd( int ind )
{
	portInd = ind;
}

void COuterDevPort::InitWayInfWithIdSubType()
{
	vecWayInfWithIdSubType.clear();

	for(int i = 0; i < pPortDev->dev_num; i ++) 
	{
		st_way_inf_id_subtype way_inf_id_subtype;
		ZeroMemory(&way_inf_id_subtype,sizeof(st_way_inf_id_subtype));
		way_inf_id_subtype.dev_id_subtype.id = pPortDev->dev_inf[i].dev_addr.id;
		way_inf_id_subtype.dev_id_subtype.sub_type = pPortDev->dev_inf[i].sub_type;
		memcpy(&way_inf_id_subtype.way_inf, &pPortDev->dev_inf[i].way_inf,sizeof(way_inf_id_subtype.way_inf));
		vecWayInfWithIdSubType.push_back(way_inf_id_subtype);
	}
}

int COuterDevPort::GetPortInd()
{
	return portInd;
}


BOOL COuterDevPort::PreTranslateMessage(MSG* pMsg)
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

void COuterDevPort::SetParentPointer( COuterDev* parent )
{
	pParent = parent;
}

void COuterDevPort::GetAllPortWayNameExceptParam(VEC_CSTRING& vec_way_name, st_way_inf way_inf )
{
	CString str;

	str_outer_dev* pouter_dev = new str_outer_dev;
	memset(pouter_dev, 0, sizeof(*pouter_dev));

	pParent->SaveDlg();

	pouter_dev->port_num = pParent->portNum;
	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		memcpy(&pouter_dev->port_dev[i],pParent->pPort[i]->pPortDev,sizeof(pouter_dev->port_dev[i]));
	}

	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		for(int j = 0; j < pouter_dev->port_dev[i].dev_num; j++)
		{
			for(int k = 0; k < MAX_WAY_NUM; k++)
			{
				str = pouter_dev->port_dev[i].dev_inf[j].way_inf.way_data[k].way_name;
				if(0 != str.GetLength())
				{
					bool same = false;
					for(int m = 0; m < MAX_WAY_NUM; m++)
					{
						if (0 == strcmp(way_inf.way_data[m].way_name,str.GetString()))
						{
							same = true;
						}
					}
					if (!same)
					{
						vec_way_name.push_back(str);
					}
				}
			}
		}
	}
	delete pouter_dev;
}

void COuterDevPort::GetAllPortWayIdExceptParam( VEC_INT& vec_way_id,st_way_inf way_inf )
{
	str_outer_dev* pouter_dev = new str_outer_dev;
	memset(pouter_dev, 0, sizeof(*pouter_dev));

	pParent->SaveDlg();

	pouter_dev->port_num = pParent->portNum;
	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		memcpy(&pouter_dev->port_dev[i],pParent->pPort[i]->pPortDev,sizeof(pouter_dev->port_dev[i]));
	}

	for(int i = 0; i < pouter_dev->port_num; i++)
	{
		for(int j = 0; j < pouter_dev->port_dev[i].dev_num; j++)
		{
			for(int k = 0; k < MAX_WAY_NUM; k++)
			{
				int way_id = pouter_dev->port_dev[i].dev_inf[j].way_inf.way_data[k].way_id;
				if(0 != way_id)
				{
					bool same = false;
					for(int m = 0; m < MAX_WAY_NUM; m++)
					{
						if (way_inf.way_data[m].way_id == way_id)
						{
							same = true;
						}
					}
					if (!same)
					{
						vec_way_id.push_back(way_id);
					}
				}
			}
		}
	}
	delete pouter_dev;
}

int COuterDevPort::GetListInf(int row_ind, st_dev_inf& dev_inf )
{
	CString str;
	int ret = 0;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	memset(&dev_inf,0,sizeof(dev_inf));
	int column_ind = 0;
	str = mList.GetItemText(row_ind, ++column_ind);
	str_to_int(str.GetString(),dev_inf.dev_addr.id,gHexType);

	str = mList.GetItemText(row_ind, ++column_ind);
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	strcpy(dev_inf.name,name);

	str = mList.GetItemText(row_ind, ++column_ind);
	dev_inf.dev_type = (e_dev_type)GetDevTypeValueByName(str.GetString());
	str = mList.GetItemText(row_ind, ++column_ind);
	dev_inf.sub_type = (e_dev_sub_type)GetSubTypeValueByName(str.GetString());
	assert(dev_inf.sub_type >= 0);

	str = mList.GetItemText(row_ind, ++column_ind);
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	strcpy(dev_inf.pos_name, name);

	str = mList.GetItemText(row_ind, ++column_ind);
	str_to_int(str.GetString(),dev_inf.way_inf.way_mask,16);

	str = mList.GetItemText(row_ind, ++column_ind);
	str_to_int(str.GetString(), dev_inf.rs485_conf.databits, gHexType);
	str = mList.GetItemText(row_ind, ++column_ind);
	str_to_int(str.GetString(), dev_inf.rs485_conf.stop, gHexType);
	str = mList.GetItemText(row_ind, ++column_ind);
	dev_inf.rs485_conf.baud = (e_baudrate)GetBaudValueByName(str.GetString());
	str = mList.GetItemText(row_ind, ++column_ind);
	dev_inf.rs485_conf.pari = (e_parity)GetParityValueByName(str.GetString());

	str = mList.GetItemText(row_ind, ++column_ind);
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	strcpy(dev_inf.hw_ver,name);

	str = mList.GetItemText(row_ind, ++column_ind);
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	strcpy(dev_inf.sw_ver,name);

	st_devid_subtype st_id_sub_type;
	ZeroMemory(&st_id_sub_type,sizeof(st_id_sub_type));
	st_id_sub_type.id = dev_inf.dev_addr.id;
	st_id_sub_type.sub_type = dev_inf.sub_type;
	GetWayInfByIdSubType(st_id_sub_type,dev_inf.way_inf);

	dev_inf.rs485_conf.port = GetPortInd();

	return column_ind;
}

void COuterDevPort::SetListInf( int row_ind, st_dev_inf& dev_inf )
{
	int ret = 0;

	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	char item[64];
	memset(item, 0, sizeof(item));

	sprintf(item, "%d", row_ind + 1);
	row_ind = mList.InsertItem(row_ind, item);//插入行,上一行

	int ColumnInd = 1;

	e_dev_sub_type sub_type = (e_dev_sub_type)dev_inf.sub_type;
	if (sub_type == WM_01 ||
		sub_type == WM_02 ||
		sub_type == WM_03 ||
		sub_type == WM_04 ||
		sub_type == GM_01 ||
		sub_type == HM_01 ||
		sub_type == HM_02
		)
	{
		//水表
		sprintf(item, "%014lld", dev_inf.dev_addr.id);
	}
	else if (sub_type == AM_3PHASE_01 ||
		sub_type == AM_1PHASE_01 ||
		sub_type == AM_DC_01
		)
	{
		//07协议	
		sprintf(item, "%012lld", dev_inf.dev_addr.id);
	}
	else
	{
		sprintf(item, "%lld", dev_inf.dev_addr.id);
	}
	mList.SetItemText(row_ind, ColumnInd++, item);

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,dev_inf.name, strlen(dev_inf.name));
	sprintf(item, "%s", name);
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", GetDevTypeNameByValue(dev_inf.dev_type));
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", GetSubTypeNameByValue(dev_inf.sub_type));
	mList.SetItemText(row_ind, ColumnInd++, item);

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,dev_inf.pos_name, strlen(dev_inf.pos_name));
	sprintf(item, "%s", name);
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "0x%x", dev_inf.way_inf.way_mask);
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", GetDataBitNameByValue(dev_inf.rs485_conf.databits));
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", GetStopNameByValue(dev_inf.rs485_conf.stop));
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", GetBaudNameByValue(dev_inf.rs485_conf.baud));
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", GetParityNameByValue(dev_inf.rs485_conf.pari));
	mList.SetItemText(row_ind, ColumnInd++, item);

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,dev_inf.hw_ver, strlen(dev_inf.hw_ver));
	sprintf(item, "%s", name);
	mList.SetItemText(row_ind, ColumnInd++, item);

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,dev_inf.sw_ver, strlen(dev_inf.sw_ver));
	sprintf(item, "%s", name);
	mList.SetItemText(row_ind, ColumnInd++, item);

	sprintf(item, "%s", "未测试");
	mList.SetItemText(row_ind, ColumnInd++, item);
}

void COuterDevPort::CloseProgress()
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

