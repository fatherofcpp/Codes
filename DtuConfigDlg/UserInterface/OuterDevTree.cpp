// OuterDevTree.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "OuterDevTree.h"
#include "afxdialogex.h"
#include "CommonFunction.h"
#include "dataConvert.h"


// COuterDevTree �Ի���

IMPLEMENT_DYNAMIC(COuterDevTreeShow, CDialog)

COuterDevTreeShow::COuterDevTreeShow(CWnd* pParent /*=NULL*/)
	: CDialog(COuterDevTreeShow::IDD, pParent)
{
	pOuterDev = new str_outer_dev;
	memset(pOuterDev,0,sizeof(*pOuterDev));
}

COuterDevTreeShow::~COuterDevTreeShow()
{
	delete pOuterDev;
}

void COuterDevTreeShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OUTERDEV, m_tree);
}


BEGIN_MESSAGE_MAP(COuterDevTreeShow, CDialog)
END_MESSAGE_MAP()

HTREEITEM COuterDevTreeShow::CreateTreeElement( CString & name,HTREEITEM hCurItem )
{
	HTREEITEM ret = NULL;
	TVINSERTSTRUCT tvStruct ;
	TVITEM tvItem ;
	tvItem.mask = TVIF_TEXT|TVIF_PARAM ;
	tvItem.pszText = (LPSTR)name.GetString() ;
	tvItem.lParam = (LPARAM)NULL ;

	tvStruct.hInsertAfter = TVI_LAST ;
	tvStruct.item = tvItem ;
	if(hCurItem==NULL){
		tvStruct.hParent = TVI_ROOT ;
	}else{
		tvStruct.hParent = hCurItem ;
	}
	ret = m_tree.InsertItem(&tvStruct) ;

	m_tree.Expand(hCurItem,TVE_EXPAND);

	m_tree.Invalidate() ; //�ػ��ͻ���
	return ret;
}

void COuterDevTreeShow::ShowTree()
{
	m_tree.DeleteAllItems();
	SetTreeStyle();

	CString str;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));
	HTREEITEM hColtorItem = NULL;
	HTREEITEM hPortItem = NULL;
	HTREEITEM hDevItem = NULL;
	HTREEITEM hWayItem = NULL;

	str.Format("�ɼ���");
	hColtorItem = CreateTreeElement(str,NULL);

	for(int j = 0; j < pOuterDev->port_num; j++)
	{
		str.Format("�˿�%d",j+1);
		hPortItem = CreateTreeElement(str,hColtorItem);
		for(int i = 0; i < pOuterDev->port_dev[j].dev_num; i++)
		{
			ZeroMemory(name,sizeof(name));
			utf8_to_gbk(name,pOuterDev->port_dev[j].dev_inf[i].name, 
				strlen(pOuterDev->port_dev[j].dev_inf[i].name));

			str.Format("�豸[��ַ:%lld,������:%s,����:%s]",
				pOuterDev->port_dev[j].dev_inf[i].dev_addr.id,
				GetSubTypeNameByValue(pOuterDev->port_dev[j].dev_inf[i].sub_type),
				name);

			hDevItem = CreateTreeElement(str,hPortItem);

			for(int k = 0; k < GetWayNumBySubTypeValue(pOuterDev->port_dev[j].dev_inf[i].sub_type); k++)
			{
				if (strlen(pOuterDev->port_dev[j].dev_inf[i].way_inf.way_data[k].way_name) > 0 )
				{
					char way_name[MAX_WAYNAME_LEN];
					ZeroMemory(way_name,sizeof(way_name));
					utf8_to_gbk(way_name,pOuterDev->port_dev[j].dev_inf[i].way_inf.way_data[k].way_name,
						strlen(pOuterDev->port_dev[j].dev_inf[i].way_inf.way_data[k].way_name));

					str.Format("��·%d:[����:%s]",k+1,way_name);
					hWayItem = CreateTreeElement(str,hDevItem);
				}
			}
		}
	}
}

void COuterDevTreeShow::SetOuterDev( str_outer_dev* outer_dev )
{
	memcpy(pOuterDev,outer_dev,sizeof(*pOuterDev));
}

void COuterDevTreeShow::SetTreeStyle()
{
	//m_tree.SetTextColor(RGB(0,0,220));//����tree�ı���ɫ*/	
	m_tree.SetBkColor(RGB(230,230,230));//����tree�ı���ɫ*/
	m_tree.SetLineColor(RGB(0, 150, 220));
	m_tree.ModifyStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_EDITLABELS|TVS_SHOWSELALWAYS);
}


void COuterDevTreeShow::ExpandAllNode(HTREEITEM hItem, CTreeCtrl& treeShow)
{
	HTREEITEM hChild = treeShow.GetChildItem(hItem);
	while(hChild)
	{
		treeShow.Expand(hItem, TVE_EXPAND);
		ExpandAllNode(hChild, treeShow);
		hChild = treeShow.GetNextSiblingItem(hChild);
	}
}
// COuterDevTree ��Ϣ�������


BOOL COuterDevTreeShow::PreTranslateMessage(MSG* pMsg)
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
