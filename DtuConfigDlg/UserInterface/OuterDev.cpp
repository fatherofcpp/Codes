// OuterDev.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "OuterDev.h"

// COuterDev 对话框

IMPLEMENT_DYNAMIC(COuterDev, CDialog)

	COuterDev::COuterDev(CWnd* pParent /*=NULL*/)
	: CDialog(COuterDev::IDD, pParent)
{
	for(int i = 0; i < MAX_PORT_NUM; i++)
	{
		pPort[i] = new COuterDevPort;
	}

	portNum = 0;
	modify = false;

	pCProtModbusSet = new CProtModbusSet;

}

COuterDev::~COuterDev()
{
	for(int i = 0; i < MAX_PORT_NUM; i++)
	{
		delete pPort[i];
	}

	delete pCProtModbusSet;

}

void COuterDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTabPort);
}


BEGIN_MESSAGE_MAP(COuterDev, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &COuterDev::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// COuterDev 消息处理程序


void COuterDev::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int CurSel = mTabPort.GetCurSel();
	for(int i = 0; i < portNum; i++)
	{
		pPort[i]->ShowWindow(false);
	}
	pPort[CurSel]->ShowWindow(true);

	//隐藏删除修改按钮
	pPort[CurSel]->mDeleteDev.EnableWindow(FALSE);
	pPort[CurSel]->mModifyDev.EnableWindow(FALSE);
	pPort[CurSel]->mCheckDev.EnableWindow(FALSE);
	*pResult = 0;
}

void COuterDev::CreateChildDlg()
{
	CString str;

	for(int i = 0; i < MAX_PORT_NUM; i++)
	{
		if (pPort[i]->GetSafeHwnd())
		{
			pPort[i]->DestroyWindow();
		}
	}

	mTabPort.DeleteAllItems();
	for(int i = 0; i < portNum; i++)
	{
		str.Format("端口%d",i + 1);
		mTabPort.InsertItem(i , str);
		pPort[i]->Create(IDD_DIALOG_PORTA,this);
		pPort[i]->CreateList();
	}

	CRect rs;
	mTabPort.GetClientRect(&rs);

	rs.top += 35;
	//rs.bottom -= 10;
	rs.left += 13;
	//rs.right -= 2;

	for(int i = 0; i < portNum; i++)
	{
		pPort[i]->MoveWindow(&rs);
	}

	for(int i = 0; i < portNum; i++)
	{
		pPort[i]->ShowWindow(false);
	}

	if (portNum > 0)
	{
		pPort[0]->ShowWindow(true);
	}

	mTabPort.SetCurSel(0);
}

void COuterDev::SetOuterDev(void*pOuterDev)
{
	str_outer_dev* p = (str_outer_dev*)pOuterDev;
	portNum = p->port_num;
	for(int i = 0; i < portNum; i++)
	{
		pPort[i]->SetPortDev(&(p->port_dev[i]));
	}
}

void COuterDev::InitDlg()
{
	CreateChildDlg();
	for(int i = 0; i < portNum; i++)
	{
		pPort[i]->InitDlg();
		pPort[i]->SetPortInd(i);
		pPort[i]->SetParentPointer(this);
	}
}

void COuterDev::SaveDlg()
{
	for(int i = 0; i < portNum; i++)
	{
		pPort[i]->SaveDlg();
	}
	for(int i = 0; i < portNum; i++)
	{
		if(pPort[i]->pPortDev->is_modify)
		{
			modify = true;
		}
	}
}



BOOL COuterDev::PreTranslateMessage(MSG* pMsg)
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

void COuterDev::SetParentPointer( CDtuConfigDlgDlg* parent )
{
	pParent = parent;
}

void COuterDev::SetModbusDefArray( CProtModbusSet* param_def_modbus_set )
{
	*pCProtModbusSet = param_def_modbus_set;

}
