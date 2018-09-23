// AddDev.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "AddDev.h"
#include "afxdialogex.h"
#include "dataConvert.h"
#include "CommonFunction.h"
#include "xmlAnalyze.h"
#include "Transfer.h"
#include "log.h"
#include "DtuConfigDlgDlg.h"

// CAddDev 对话框

IMPLEMENT_DYNAMIC(CAddDev, CDialogEx)

	UINT __cdecl AdaptThread(void *p);


CAddDev::CAddDev(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDev::IDD, pParent)
{
	pDevInf = new st_dev_inf;
	memset(pDevInf, 0, sizeof(*pDevInf));
	pProgress = new CProgress;
	dataFrame = NULL;

	mDevName.SetMaxLen(MAX_NAME_LEN);
	mDevId.SetValidType(VALID_INT);
	mDevId.SetMaxLen(14);
	mDevId.SetMinNum(1);
	mDevId.SetMaxNum(99999999999998);

	mDevPos.SetMaxLen(MAX_NAME_LEN);
	mHardVersion.SetMaxLen(MAX_NAME_LEN);
	mSoftVersion.SetMaxLen(MAX_NAME_LEN);

	wayCount = MAX_WAY_NUM;

	pWayBit[0] = &mWayBit1;
	pWayBit[1] = &mWayBit2;
	pWayBit[2] = &mWayBit3;
	pWayBit[3] = &mWayBit4;
	pWayBit[4] = &mWayBit5;
	pWayBit[5] = &mWayBit6;
	pWayBit[6] = &mWayBit7;
	pWayBit[7] = &mWayBit8;
	pWayBit[8] = &mWayBit9;
	pWayBit[9] = &mWayBit10;
	pWayBit[10] = &mWayBit11;
	pWayBit[11] = &mWayBit12;

	pWayPos[0] = &mWayPos1;
	pWayPos[1] = &mWayPos2;
	pWayPos[2] = &mWayPos3;
	pWayPos[3] = &mWayPos4;
	pWayPos[4] = &mWayPos5;
	pWayPos[5] = &mWayPos6;
	pWayPos[6] = &mWayPos7;
	pWayPos[7] = &mWayPos8;
	pWayPos[8] = &mWayPos9;
	pWayPos[9] = &mWayPos10;
	pWayPos[10] = &mWayPos11;
	pWayPos[11] = &mWayPos12;

	pWayName[0] = &mWayName1;
	pWayName[1] = &mWayName2;
	pWayName[2] = &mWayName3;
	pWayName[3] = &mWayName4;
	pWayName[4] = &mWayName5;
	pWayName[5] = &mWayName6;
	pWayName[6] = &mWayName7;
	pWayName[7] = &mWayName8;
	pWayName[8] = &mWayName9;
	pWayName[9] = &mWayName10;
	pWayName[10] = &mWayName11;
	pWayName[11] = &mWayName12;

	pWayDevType[0] = &mWayDevType1;
	pWayDevType[1] = &mWayDevType2;
	pWayDevType[2] = &mWayDevType3;
	pWayDevType[3] = &mWayDevType4;
	pWayDevType[4] = &mWayDevType5;
	pWayDevType[5] = &mWayDevType6;
	pWayDevType[6] = &mWayDevType7;
	pWayDevType[7] = &mWayDevType8;
	pWayDevType[8] = &mWayDevType9;
	pWayDevType[9] = &mWayDevType10;
	pWayDevType[10] = &mWayDevType11;
	pWayDevType[11] = &mWayDevType12;

	pGdNodeId[0] = &mGdNodeId1;
	pGdNodeId[1] = &mGdNodeId2;
	pGdNodeId[2] = &mGdNodeId3;
	pGdNodeId[3] = &mGdNodeId4;
	pGdNodeId[4] = &mGdNodeId5;
	pGdNodeId[5] = &mGdNodeId6;
	pGdNodeId[6] = &mGdNodeId7;
	pGdNodeId[7] = &mGdNodeId8;
	pGdNodeId[8] = &mGdNodeId9;
	pGdNodeId[9] = &mGdNodeId10;
	pGdNodeId[10] = &mGdNodeId11;
	pGdNodeId[11] = &mGdNodeId12;

	pGdClassId[0] = &mGdClassId1;
	pGdClassId[1] = &mGdClassId2;
	pGdClassId[2] = &mGdClassId3;
	pGdClassId[3] = &mGdClassId4;
	pGdClassId[4] = &mGdClassId5;
	pGdClassId[5] = &mGdClassId6;
	pGdClassId[6] = &mGdClassId7;
	pGdClassId[7] = &mGdClassId8;
	pGdClassId[8] = &mGdClassId9;
	pGdClassId[9] = &mGdClassId10;
	pGdClassId[10] = &mGdClassId11;
	pGdClassId[11] = &mGdClassId12;

	pGdItemId[0] = &mGdItemId1;
	pGdItemId[1] = &mGdItemId2;
	pGdItemId[2] = &mGdItemId3;
	pGdItemId[3] = &mGdItemId4;
	pGdItemId[4] = &mGdItemId5;
	pGdItemId[5] = &mGdItemId6;
	pGdItemId[6] = &mGdItemId7;
	pGdItemId[7] = &mGdItemId8;
	pGdItemId[8] = &mGdItemId9;
	pGdItemId[9] = &mGdItemId10;
	pGdItemId[10] = &mGdItemId11;
	pGdItemId[11] = &mGdItemId12;

	pWayDevAddr[0] = &mWayDevAddr1;
	pWayDevAddr[1] = &mWayDevAddr2;
	pWayDevAddr[2] = &mWayDevAddr3;
	pWayDevAddr[3] = &mWayDevAddr4;
	pWayDevAddr[4] = &mWayDevAddr5;
	pWayDevAddr[5] = &mWayDevAddr6;
	pWayDevAddr[6] = &mWayDevAddr7;
	pWayDevAddr[7] = &mWayDevAddr8;
	pWayDevAddr[8] = &mWayDevAddr9;
	pWayDevAddr[9] = &mWayDevAddr10;
	pWayDevAddr[10] = &mWayDevAddr11;
	pWayDevAddr[11] = &mWayDevAddr12;

	pWayDetail[0] = &mWayDetail1;
	pWayDetail[1] = &mWayDetail2;
	pWayDetail[2] = &mWayDetail3;
	pWayDetail[3] = &mWayDetail4;
	pWayDetail[4] = &mWayDetail5;
	pWayDetail[5] = &mWayDetail6;
	pWayDetail[6] = &mWayDetail7;
	pWayDetail[7] = &mWayDetail8;
	pWayDetail[8] = &mWayDetail9;
	pWayDetail[9] = &mWayDetail10;
	pWayDetail[10] = &mWayDetail11;
	pWayDetail[11] = &mWayDetail12;

	for(int i = 0; i < MAX_WAY_NUM; i++)
	{
		pWayPos[i]->SetMaxLen(MAX_NAME_LEN);
		pWayName[i]->SetMaxLen(MAX_NAME_LEN);
		pGdNodeId[i]->SetValidType(VALID_INT);
		pGdNodeId[i]->SetMinNum(1);
		pGdNodeId[i]->SetMaxNum(999999);
		pGdClassId[i]->SetValidType(VALID_INT);
		pGdClassId[i]->SetMinNum(1);
		pGdClassId[i]->SetMaxNum(999999);
		pGdItemId[i]->SetValidType(VALID_INT);
		pGdItemId[i]->SetMinNum(1);
		pGdItemId[i]->SetMaxNum(999999);
		pWayDevAddr[i]->SetValidType(VALID_INT);
		pWayDevAddr[i]->SetMinNum(0);
		pWayDevAddr[i]->SetMaxNum(999999);
		pWayDetail[i]->SetMaxLen(MAX_NAME_LEN);
	}
	wayMask = 0;


	devRs485Cont = new CDevAdaptRs485Cont();

}

CAddDev::~CAddDev()
{
	delete pProgress;
	delete pDevInf;
	delete devRs485Cont;
}

void CAddDev::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, mDevId);
	DDX_Control(pDX, IDC_EDIT_NAME, mDevName);
	DDX_Control(pDX, IDC_EDIT_WAY_MASK, mWayMask);
	DDX_Control(pDX, IDC_COMBO_BAUD, mBaud);
	DDX_Control(pDX, IDC_COMBO_PARITY, mParity);
	DDX_Control(pDX, IDC_COMBO_TYPE, mDevSubType);
	DDX_Control(pDX, IDC_CHECK_WM1, mWayBit1);
	DDX_Control(pDX, IDC_CHECK_WM2, mWayBit2);
	DDX_Control(pDX, IDC_CHECK_WM3, mWayBit3);
	DDX_Control(pDX, IDC_CHECK_WM4, mWayBit4);
	DDX_Control(pDX, IDC_CHECK_WM5, mWayBit5);
	DDX_Control(pDX, IDC_CHECK_WM6, mWayBit6);
	DDX_Control(pDX, IDC_CHECK_WM7, mWayBit7);
	DDX_Control(pDX, IDC_CHECK_WM8, mWayBit8);
	DDX_Control(pDX, IDC_CHECK_WM9, mWayBit9);
	DDX_Control(pDX, IDC_CHECK_WM10, mWayBit10);
	DDX_Control(pDX, IDC_CHECK_WM11, mWayBit11);
	DDX_Control(pDX, IDC_CHECK_WM12, mWayBit12);
	DDX_Control(pDX, IDC_EDIT_NAME1, mWayName1);
	DDX_Control(pDX, IDC_EDIT_NAME2, mWayName2);
	DDX_Control(pDX, IDC_EDIT_NAME3, mWayName3);
	DDX_Control(pDX, IDC_EDIT_NAME4, mWayName4);
	DDX_Control(pDX, IDC_EDIT_NAME5, mWayName5);
	DDX_Control(pDX, IDC_EDIT_NAME6, mWayName6);
	DDX_Control(pDX, IDC_EDIT_NAME7, mWayName7);
	DDX_Control(pDX, IDC_EDIT_NAME8, mWayName8);
	DDX_Control(pDX, IDC_EDIT_NAME9, mWayName9);
	DDX_Control(pDX, IDC_EDIT_NAME10, mWayName10);
	DDX_Control(pDX, IDC_EDIT_NAME11, mWayName11);
	DDX_Control(pDX, IDC_EDIT_NAME12, mWayName12);
	DDX_Control(pDX, IDC_EDIT_POS1, mWayPos1);
	DDX_Control(pDX, IDC_EDIT_POS2, mWayPos2);
	DDX_Control(pDX, IDC_EDIT_POS3, mWayPos3);
	DDX_Control(pDX, IDC_EDIT_POS4, mWayPos4);
	DDX_Control(pDX, IDC_EDIT_POS5, mWayPos5);
	DDX_Control(pDX, IDC_EDIT_POS6, mWayPos6);
	DDX_Control(pDX, IDC_EDIT_POS7, mWayPos7);
	DDX_Control(pDX, IDC_EDIT_POS8, mWayPos8);
	DDX_Control(pDX, IDC_EDIT_POS9, mWayPos9);
	DDX_Control(pDX, IDC_EDIT_POS10, mWayPos10);
	DDX_Control(pDX, IDC_EDIT_POS11, mWayPos11);
	DDX_Control(pDX, IDC_EDIT_POS12, mWayPos12);

	DDX_Control(pDX, IDC_COMBO_NAME1, mWayDevType1);
	DDX_Control(pDX, IDC_COMBO_NAME2, mWayDevType2);
	DDX_Control(pDX, IDC_COMBO_NAME3, mWayDevType3);
	DDX_Control(pDX, IDC_COMBO_NAME4, mWayDevType4);
	DDX_Control(pDX, IDC_COMBO_NAME5, mWayDevType5);
	DDX_Control(pDX, IDC_COMBO_NAME6, mWayDevType6);
	DDX_Control(pDX, IDC_COMBO_NAME7, mWayDevType7);
	DDX_Control(pDX, IDC_COMBO_NAME8, mWayDevType8);
	DDX_Control(pDX, IDC_COMBO_NAME9, mWayDevType9);
	DDX_Control(pDX, IDC_COMBO_NAME10, mWayDevType10);
	DDX_Control(pDX, IDC_COMBO_NAME11, mWayDevType11);
	DDX_Control(pDX, IDC_COMBO_NAME12, mWayDevType12);
	DDX_Control(pDX, IDC_EDIT_DEV_POS, mDevPos);
	DDX_Control(pDX, IDC_COMBO_DATABIT, mDataBit);
	DDX_Control(pDX, IDC_COMBO_STOP, mStop);
	DDX_Control(pDX, IDC_EDIT7, mHardVersion);
	DDX_Control(pDX, IDC_EDIT8, mSoftVersion);
	DDX_Control(pDX, IDC_EDIT_NODE_ID1, mGdNodeId1);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID1, mGdClassId1);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID1, mGdItemId1);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR1, mWayDevAddr1);
	DDX_Control(pDX, IDC_EDIT_DETAIL1, mWayDetail1);
	DDX_Control(pDX, IDC_EDIT_NODE_ID2, mGdNodeId2);
	DDX_Control(pDX, IDC_EDIT_NODE_ID3, mGdNodeId3);
	DDX_Control(pDX, IDC_EDIT_NODE_ID4, mGdNodeId4);
	DDX_Control(pDX, IDC_EDIT_NODE_ID5, mGdNodeId5);
	DDX_Control(pDX, IDC_EDIT_NODE_ID6, mGdNodeId6);
	DDX_Control(pDX, IDC_EDIT_NODE_ID7, mGdNodeId7);
	DDX_Control(pDX, IDC_EDIT_NODE_ID8, mGdNodeId8);
	DDX_Control(pDX, IDC_EDIT_NODE_ID9, mGdNodeId9);
	DDX_Control(pDX, IDC_EDIT_NODE_ID10, mGdNodeId10);
	DDX_Control(pDX, IDC_EDIT_NODE_ID11, mGdNodeId11);
	DDX_Control(pDX, IDC_EDIT_NODE_ID12, mGdNodeId12);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID2, mGdClassId2);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID3, mGdClassId3);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID4, mGdClassId4);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID5, mGdClassId5);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID6, mGdClassId6);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID7, mGdClassId7);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID8, mGdClassId8);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID9, mGdClassId9);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID10, mGdClassId10);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID11, mGdClassId11);
	DDX_Control(pDX, IDC_EDIT_CLASS_ID12, mGdClassId12);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID2, mGdItemId2);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID3, mGdItemId3);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID4, mGdItemId4);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID5, mGdItemId5);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID6, mGdItemId6);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID7, mGdItemId7);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID8, mGdItemId8);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID9, mGdItemId9);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID10, mGdItemId10);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID11, mGdItemId11);
	DDX_Control(pDX, IDC_EDIT_ITEM_ID12, mGdItemId12);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR2, mWayDevAddr2);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR3, mWayDevAddr3);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR4, mWayDevAddr4);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR5, mWayDevAddr5);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR6, mWayDevAddr6);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR7, mWayDevAddr7);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR8, mWayDevAddr8);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR9, mWayDevAddr9);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR10, mWayDevAddr10);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR11, mWayDevAddr11);
	DDX_Control(pDX, IDC_EDIT_WAY_DDR12, mWayDevAddr12);
	DDX_Control(pDX, IDC_EDIT_DETAIL2, mWayDetail2);
	DDX_Control(pDX, IDC_EDIT_DETAIL3, mWayDetail3);
	DDX_Control(pDX, IDC_EDIT_DETAIL4, mWayDetail4);
	DDX_Control(pDX, IDC_EDIT_DETAIL5, mWayDetail5);
	DDX_Control(pDX, IDC_EDIT_DETAIL6, mWayDetail6);
	DDX_Control(pDX, IDC_EDIT_DETAIL7, mWayDetail7);
	DDX_Control(pDX, IDC_EDIT_DETAIL8, mWayDetail8);
	DDX_Control(pDX, IDC_EDIT_DETAIL9, mWayDetail9);
	DDX_Control(pDX, IDC_EDIT_DETAIL10, mWayDetail10);
	DDX_Control(pDX, IDC_EDIT_DETAIL11, mWayDetail11);
	DDX_Control(pDX, IDC_EDIT_DETAIL12, mWayDetail12);
	DDX_Control(pDX, IDC_EDIT_DEV_TYPE, mDevType);
	DDX_Control(pDX, IDC_BUTTON_ADATP, btAdapt);
}


BEGIN_MESSAGE_MAP(CAddDev, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CAddDev::OnBnClickedOK)
	ON_BN_CLICKED(IDC_CHECK_WM1, &CAddDev::OnBnClickedWMBit1)
	ON_BN_CLICKED(IDC_CHECK_WM2, &CAddDev::OnBnClickedWMBit2)
	ON_BN_CLICKED(IDC_CHECK_WM3, &CAddDev::OnBnClickedWMBit3)
	ON_BN_CLICKED(IDC_CHECK_WM4, &CAddDev::OnBnClickedWMBit4)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CAddDev::OnCbnSelchangeType)
	ON_BN_CLICKED(IDC_CHECK_WM5, &CAddDev::OnBnClickedCheckWm5)
	ON_BN_CLICKED(IDC_CHECK_WM6, &CAddDev::OnBnClickedCheckWm6)
	ON_BN_CLICKED(IDC_CHECK_WM7, &CAddDev::OnBnClickedCheckWm7)
	ON_BN_CLICKED(IDC_CHECK_WM8, &CAddDev::OnBnClickedCheckWm8)
	ON_BN_CLICKED(IDC_CHECK_WM9, &CAddDev::OnBnClickedCheckWm9)
	ON_BN_CLICKED(IDC_CHECK_WM10, &CAddDev::OnBnClickedCheckWm10)
	ON_BN_CLICKED(IDC_CHECK_WM11, &CAddDev::OnBnClickedCheckWm11)
	ON_BN_CLICKED(IDC_CHECK_WM12, &CAddDev::OnBnClickedCheckWm12)
	ON_BN_CLICKED(IDC_BUTTON_GET_FACTORY, &CAddDev::OnBnClickedButtonGetFactory)
	ON_BN_CLICKED(IDC_BUTTON_ADATP, &CAddDev::OnBnClickedButtonAdapt)
END_MESSAGE_MAP()


// CAddDev 消息处理程序


void CAddDev::OnBnClickedOK()
{
	//save dialog
	CString str;
	int ret = 0;

	int ind = mDevSubType.GetCurSel();
	if (ind < 0)
	{
		MessageBox("设备子类型无效");
		return;
	}
	e_dev_sub_type sub_type = (e_dev_sub_type)gSubTypeValue[ind];

	pDevInf->dev_addr.id = GetDevAddr();
	if (pDevInf->dev_addr.id == DEV_ID_ERR)
	{
		return;
	}

	for (unsigned int i = 0; i < vecIdSubType.size(); i++)
	{
		if (pDevInf->dev_addr.id == vecIdSubType[i].id && sub_type == vecIdSubType[i].sub_type)
		{
			str.Format("地址&子类型已经存在");
			MessageBox(str);
			return ;
		}
	}

	mDevName.GetWindowText(str);
	if (0 == str.GetLength())
	{
		MessageBox("设备名称不合法");
		return;
	}
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), pDevInf->name);

	mDevPos.GetWindowText(str);
	if (0 == str.GetLength())
	{
		MessageBox("设备编号不合法");
		return;
	}
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), pDevInf->pos_name);

	mHardVersion.GetWindowText(str);
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), pDevInf->hw_ver);
	mSoftVersion.GetWindowText(str);
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), pDevInf->sw_ver);

	mWayMask.GetWindowText(str);
	int mask_len = str.GetLength();
	int pre_len = 2;
	if (pre_len == mask_len)
	{
		MessageBox("掩码不合法");
		return;
	}
	str_to_int(str.GetString(),pDevInf->way_inf.way_mask,16);

	ret = SetDevSubTypeValue();
	if (ret != 0)
	{
		MessageBox("设备子类型不合法");
		return;
	}

	ret = SetDataBitValue();
	if (ret != 0)
	{
		MessageBox("数据位不合法");
		return;
	}
	ret = SetStopValue();
	if (ret != 0)
	{
		MessageBox("停止位不合法");
		return;
	}
	ret = SetBaudValue();
	if (ret != 0)
	{
		MessageBox("波特率不合法");
		return;
	}
	ret = SetParityValue();
	if (ret != 0)
	{
		MessageBox("校验不合法");
		return;
	}
	ret = SetWayInf();
	if (ret != 0)
	{
		MessageBox("回路信息不合法");
		return;
	}
	CDialog::OnOK();
}


void CAddDev::OnBnClickedWMBit1()
{
	EnableWayControlByMaskChanged();
}

void CAddDev::OnBnClickedWMBit2()
{
	EnableWayControlByMaskChanged();
}

void CAddDev::OnBnClickedWMBit3()
{
	EnableWayControlByMaskChanged();
}

void CAddDev::OnBnClickedWMBit4()
{
	EnableWayControlByMaskChanged();
}

void CAddDev::EnableWayControlByMaskChanged()
{
	CString str;

	for(int i = 0; i < wayCount; i++)
	{
		int way_ind = wayCount - i - 1;
		int flag = 0x01 << way_ind;
		if(pWayBit[i]->GetCheck())
		{
			wayMask |= flag;
			pWayName[i]->EnableWindow(TRUE);
			pWayDevType[i]->EnableWindow(TRUE);
			pWayPos[i]->EnableWindow(TRUE);
			pGdNodeId[i]->EnableWindow(TRUE);
			pGdClassId[i]->EnableWindow(TRUE);
			pGdItemId[i]->EnableWindow(TRUE);
			//pWayDevAddr[i]->EnableWindow(TRUE);
			pWayDetail[i]->EnableWindow(TRUE);
			str.Format("%d",way_ind);
			pWayDevAddr[i]->SetWindowText(str);
		}
		else
		{
			wayMask &= ~flag;
			pWayName[i]->EnableWindow(FALSE);
			pWayDevType[i]->EnableWindow(FALSE);
			pWayPos[i]->EnableWindow(FALSE);
			pGdNodeId[i]->EnableWindow(FALSE);
			pGdClassId[i]->EnableWindow(FALSE);
			pGdItemId[i]->EnableWindow(FALSE);
			//pWayDevAddr[i]->EnableWindow(FALSE);
			pWayDetail[i]->EnableWindow(FALSE);
		}
	}

	str.Format("0x%x",wayMask);
	mWayMask.SetWindowText(str);
}


void CAddDev::OnCbnSelchangeType()
{
	CString str;
	int ind = 0;
	wayMask = 0;
	mWayMask.SetWindowText("0x00");

	ind = mDevSubType.GetCurSel();
	wayCount = gWayCountValue[ind];

	//设备类型
	int dev_type = gSubTypeValue[ind] / 100;
	mDevType.SetWindowText(	GetDevTypeNameByValue(dev_type));

	ShowWayControlByWayCount();
}

void CAddDev::ShowWayControlByWayCount()
{
	CString str;
	for(int i = 0; i < MAX_WAY_NUM; i++)
	{
		pWayBit[i]->SetCheck(false);
		pWayName[i]->SetWindowText("");
		pWayDevType[i]->SetCurSel(-1);
		pWayPos[i]->SetWindowText("");
		pGdNodeId[i]->SetWindowText("");
		pGdClassId[i]->SetWindowText("");
		pGdItemId[i]->SetWindowText("");
		pWayDevAddr[i]->SetWindowText("");
		pWayDetail[i]->SetWindowText("");

		pWayBit[i]->ShowWindow(FALSE);
		pWayName[i]->ShowWindow(FALSE);
		pWayDevType[i]->ShowWindow(FALSE);
		pWayPos[i]->ShowWindow(FALSE);
		pGdNodeId[i]->ShowWindow(FALSE);
		pGdClassId[i]->ShowWindow(FALSE);
		pGdItemId[i]->ShowWindow(FALSE);
		pWayDevAddr[i]->ShowWindow(FALSE);
		pWayDetail[i]->ShowWindow(FALSE);
	}

	for(int i = 0; i < wayCount; i++)
	{
		pWayBit[i]->ShowWindow(TRUE);
		pWayName[i]->ShowWindow(TRUE);
		pWayDevType[i]->ShowWindow(TRUE);
		pWayPos[i]->ShowWindow(TRUE);
		pGdNodeId[i]->ShowWindow(TRUE);
		pGdClassId[i]->ShowWindow(TRUE);
		pGdItemId[i]->ShowWindow(TRUE);
		pWayDevAddr[i]->ShowWindow(TRUE);
		pWayDetail[i]->ShowWindow(TRUE);

		pWayName[i]->EnableWindow(FALSE);
		pWayDevType[i]->EnableWindow(FALSE);
		pWayPos[i]->EnableWindow(FALSE);
		pGdNodeId[i]->EnableWindow(FALSE);
		pGdClassId[i]->EnableWindow(FALSE);
		pGdItemId[i]->EnableWindow(FALSE);
		pWayDevAddr[i]->EnableWindow(FALSE);
		pWayDetail[i]->EnableWindow(FALSE);

		str.Format("%d",wayCount - i);
		pWayBit[i]->SetWindowText(str);
	}
}


BOOL CAddDev::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString str;
	int ind = 0;
	int ret = 0;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	for (int i = 0; i < gDataBitNum; i++)
	{
		str.Format("%s",gDataBitName[i]);
		mDataBit.InsertString(i,str);
	}

	for (int i = 0; i < gStopNum; i++)
	{
		str.Format("%s",gStopName[i]);
		mStop.InsertString(i,str);
	}

	for (int i = 0; i < gBaudNum; i++)
	{
		str.Format("%s",gBaudName[i]);
		mBaud.InsertString(i,str);
	}

	for (int i = 0; i < gParityNum; i++)
	{
		str.Format("%s",gParityName[i]);
		mParity.InsertString(i,str);
	}

	for (int i = 0; i < gSubTypeNum; i++)
	{
		str.Format("%s",gSubTypeName[i]);
		mDevSubType.InsertString(i,str);
	}


	for (int i = 0; i < MAX_WAY_NUM; i++)
	{
		pWayDevAddr[i]->EnableWindow(FALSE);
	}

	//init way name for sdry
	for (int i = 0; i < MAX_WAY_NUM; i++)
	{
		for (int j = 0; j < gWayDevTypeNum; j++)
		{
			str.Format("%s",gWayDevTypeName[j]);
			pWayDevType[i]->InsertString(j,str);
		}
	}

	//init dlg
	if (pDevInf->dev_addr.id != 0)
	{
		e_dev_sub_type sub_type = (e_dev_sub_type)pDevInf->sub_type;
		if (sub_type == WM_01)
		{
			str.Format("%014lld",pDevInf->dev_addr.id);
		}
		else if (sub_type == AM_3PHASE_01 ||
			sub_type == AM_1PHASE_01 ||
			sub_type == AM_DC_01 
			)
		{
			str.Format("%012lld",pDevInf->dev_addr.id);
		}
		else
		{
			str.Format("%lld",pDevInf->dev_addr.id);
		}
		mDevId.SetWindowText(str);
	}

	if (strlen(pDevInf->name) != 0)
	{
		ZeroMemory(name,sizeof(name));
		ret = utf8_to_gbk(name, pDevInf->name, strlen(pDevInf->name));
		str.Format("%s",name);
		mDevName.SetWindowText(str);
	}

	if (strlen(pDevInf->pos_name) != 0)
	{
		ZeroMemory(name,sizeof(name));
		ret = utf8_to_gbk(name, pDevInf->pos_name, strlen(pDevInf->pos_name));
		str.Format("%s",name);
		mDevPos.SetWindowText(str);
	}

	if (strlen(pDevInf->hw_ver) != 0)
	{
		ZeroMemory(name,sizeof(name));
		ret = utf8_to_gbk(name, pDevInf->hw_ver, strlen(pDevInf->hw_ver));
		str.Format("%s",name);
		mHardVersion.SetWindowText(str);
	}
	mHardVersion.EnableWindow(FALSE);

	if (strlen(pDevInf->sw_ver) != 0)
	{
		ZeroMemory(name,sizeof(name));
		ret = utf8_to_gbk(name, pDevInf->sw_ver, strlen(pDevInf->sw_ver));
		str.Format("%s",name);
		mSoftVersion.SetWindowText(str);
	}
	mSoftVersion.EnableWindow(FALSE);

	str.Format("0x%x",pDevInf->way_inf.way_mask);
	mWayMask.SetWindowText(str);

	InitDataBitName();
	InitStopName();
	InitBaudName();
	InitParityName();
	InitDevSubTypeName();

	//设备类型
	ind = mDevSubType.GetCurSel();
	int dev_type = gSubTypeValue[ind] / 100;
	mDevType.SetWindowText(	GetDevTypeNameByValue(dev_type));
	mDevType.EnableWindow(FALSE);

	ind = mDevSubType.GetCurSel();
	wayCount = gWayCountValue[ind];

	ShowWayControlByWayCount();
	InitWayControl();

	ControlLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddDev::SetDevInf( st_dev_inf* dev_inf , 
	VEC_DEVID_SUBTYPE& vec_id_subtype,
	VEC_CSTRING& vec_way_name, 
	VEC_INT vec_way_id,
	COuterDevPort* p_parent)
{
	memcpy(pDevInf,dev_inf,sizeof(*pDevInf));
	vecIdSubType = vec_id_subtype;
	vecWayName = vec_way_name;
	vecWayId = vec_way_id;
	pParent = p_parent;
}

void CAddDev::InitDevSubTypeName()
{
	for (int i = 0; i < gSubTypeNum; i++)
	{
		if (pDevInf->sub_type == gSubTypeValue[i])
		{
			mDevSubType.SetCurSel(i);
			return ;
		}
	}
	mDevSubType.SetCurSel(0);
}

void CAddDev::InitBaudName()
{
	for (int i = 0; i < gBaudNum; i++)
	{
		if (pDevInf->rs485_conf.baud == gBaudValue[i])
		{
			mBaud.SetCurSel(i);
			return;
		}
	}
	mBaud.SetCurSel(3);
}

void CAddDev::InitParityName()
{
	for (int i = 0; i < gParityNum; i++)
	{
		if (pDevInf->rs485_conf.pari == gParityValue[i])
		{
			mParity.SetCurSel(i);
			return;
		}
	}
	mParity.SetCurSel(0);
}

int CAddDev::SetDevSubTypeValue()
{
	int ind = 0;
	ind = mDevSubType.GetCurSel();
	if (ind < 0)
	{
		return -1;
	}
	pDevInf->sub_type = (e_dev_sub_type)gSubTypeValue[ind];
	pDevInf->dev_type = (e_dev_type)(pDevInf->sub_type / 100);
	return 0;
}

int CAddDev::SetBaudValue()
{
	int ind = 0;
	ind = mBaud.GetCurSel();
	if (ind < 0)
	{
		return -1;
	}
	pDevInf->rs485_conf.baud = (e_baudrate)gBaudValue[ind];
	return 0;
}

int CAddDev::SetParityValue()
{
	int ind = 0;
	ind = mParity.GetCurSel();
	if (ind < 0)
	{
		return -1;
	}
	pDevInf->rs485_conf.pari = (e_parity)gParityValue[ind];
	return 0;
}


void CAddDev::OnBnClickedCheckWm5()
{
	EnableWayControlByMaskChanged();
}


void CAddDev::OnBnClickedCheckWm6()
{
	EnableWayControlByMaskChanged();
}


void CAddDev::OnBnClickedCheckWm7()
{
	EnableWayControlByMaskChanged();
}


void CAddDev::OnBnClickedCheckWm8()
{
	EnableWayControlByMaskChanged();
}


void CAddDev::OnBnClickedCheckWm9()
{
	EnableWayControlByMaskChanged();
}


void CAddDev::OnBnClickedCheckWm10()
{
	EnableWayControlByMaskChanged();
}


void CAddDev::OnBnClickedCheckWm11()
{
	EnableWayControlByMaskChanged();
}


void CAddDev::OnBnClickedCheckWm12()
{
	EnableWayControlByMaskChanged();
}

void CAddDev::InitWayControl()
{
	char name[MAX_WAYNAME_LEN];
	ZeroMemory(name,sizeof(name));
	int way_ind = 0;
	int ret = 0;
	CString str;
	for(int i = 0; i < wayCount; i++)
	{
		int flag = 0x01 << (wayCount - i - 1);
		if (pDevInf->way_inf.way_mask & flag)
		{
			pWayBit[i]->SetCheck(true);
			way_ind = wayCount - 1 - i;
			pWayName[i]->EnableWindow(TRUE);
			pWayDevType[i]->EnableWindow(TRUE);
			pWayPos[i]->EnableWindow(TRUE);
			pGdNodeId[i]->EnableWindow(TRUE);
			pGdClassId[i]->EnableWindow(TRUE);
			pGdItemId[i]->EnableWindow(TRUE);
			//pWayDevAddr[i]->EnableWindow(TRUE);
			pWayDetail[i]->EnableWindow(TRUE);

			ZeroMemory(name,sizeof(name));
			ret = utf8_to_gbk(name,pDevInf->way_inf.way_data[way_ind].pos_name,
				strlen(pDevInf->way_inf.way_data[way_ind].pos_name));
			pWayPos[i]->SetWindowText(name);

			ZeroMemory(name,sizeof(name));
			ret = utf8_to_gbk(name,pDevInf->way_inf.way_data[way_ind].way_name,
				strlen(pDevInf->way_inf.way_data[way_ind].way_name));
			pWayName[i]->SetWindowText(name);

			for (int j = 0; j < gWayDevTypeNum; j++)
			{
				if (pDevInf->way_inf.way_data[way_ind].way_type == gWayDevTypeValue[j])
				{
					ret = pWayDevType[i]->SetCurSel(j);
					break;
				}
			}

			str.Format("%d", pDevInf->way_inf.way_data[way_ind].way_id);
			pGdNodeId[i]->SetWindowText(str);

			str.Format("%d", pDevInf->way_inf.way_data[way_ind].class_id);
			pGdClassId[i]->SetWindowText(str);

			str.Format("%d", pDevInf->way_inf.way_data[way_ind].item_id);
			pGdItemId[i]->SetWindowText(str);

			str.Format("%d", pDevInf->way_inf.way_data[way_ind].way_addr);
			pWayDevAddr[i]->SetWindowText(str);

			ZeroMemory(name,sizeof(name));
			ret = utf8_to_gbk(name,pDevInf->way_inf.way_data[way_ind].detail,
				strlen(pDevInf->way_inf.way_data[way_ind].detail));
			pWayDetail[i]->SetWindowText(name);
		}
		else
		{
			pWayBit[i]->SetCheck(false);
		}
	}
}

int CAddDev::SetWayInf()
{
	CString str;
	char name[MAX_WAYNAME_LEN];
	ZeroMemory(name,sizeof(name));
	int way_ind = 0;
	int ret = 0;
	for(int i = 0; i < wayCount; i++)
	{
		way_ind = wayCount - 1 - i;
		if (pWayBit[i]->GetCheck())
		{
			pWayPos[i]->GetWindowText(str);
			if (str.GetLength() == 0)
			{
				return -11;
			}
			ZeroMemory(name,sizeof(name));
			ret = gb2312_to_utf8(str.GetString(),
				str.GetLength(), name);
			sprintf(pDevInf->way_inf.way_data[way_ind].pos_name,"%s",name);

			pWayName[i]->GetWindowText(str);
			if (str.GetLength() == 0)
			{
				return -1;
			}
			ZeroMemory(name,sizeof(name));
			ret = gb2312_to_utf8(str.GetString(),
				str.GetLength(), name);
			sprintf(pDevInf->way_inf.way_data[way_ind].way_name,"%s",name);

			int way_dev_type_ind = pWayDevType[i]->GetCurSel();
			if (way_dev_type_ind < 0)
			{
				return -1;
			}
			pDevInf->way_inf.way_data[way_ind].way_type = 
				(e_way_type)gWayDevTypeValue[way_dev_type_ind];

			pGdNodeId[i]->GetWindowText(str);
			if (str.GetLength() == 0)
			{
				return -1;
			}
			str_to_int(str.GetString(), pDevInf->way_inf.way_data[way_ind].way_id, gHexType);

			pGdClassId[i]->GetWindowText(str);
			if (str.GetLength() == 0)
			{
				return -1;
			}
			str_to_int(str.GetString(), pDevInf->way_inf.way_data[way_ind].class_id, gHexType);

			pGdItemId[i]->GetWindowText(str);
			if (str.GetLength() == 0)
			{
				return -1;
			}
			str_to_int(str.GetString(), pDevInf->way_inf.way_data[way_ind].item_id, gHexType);

			pWayDevAddr[i]->GetWindowText(str);
			if (str.GetLength() == 0)
			{
				return -1;
			}
			str_to_int(str.GetString(), pDevInf->way_inf.way_data[way_ind].way_addr, gHexType);

			pWayDetail[i]->GetWindowText(str);
			if (str.GetLength() == 0)
			{
				return -1;
			}
			ZeroMemory(name,sizeof(name));
			ret = gb2312_to_utf8(str.GetString(),
				str.GetLength(), name);
			sprintf(pDevInf->way_inf.way_data[way_ind].detail,"%s",name);
			///////////////////////////////////////////////
			//判断与其它设备的回路名称不重复
			for (MU_INT32 j = 0; j < vecWayName.size(); j++)
			{
				if (0 == strcmp(pDevInf->way_inf.way_data[way_ind].way_name,
					vecWayName[j].GetString()))
				{
					//	return -2;
				}
			}
			for (MU_INT32 j = 0; j < vecWayId.size(); j++)
			{
				if (pDevInf->way_inf.way_data[way_ind].way_id == vecWayId[j])
				{
					return -21;
				}
			}
			//////////////////////////////////////////////////////
		}
		else
		{
			ZeroMemory(pDevInf->way_inf.way_data[way_ind].way_name,
				sizeof(pDevInf->way_inf.way_data[way_ind].way_name));
		}
	}
	//判断当前设备的回路名称不重复
	for(int i = 0; i < wayCount; i++)
	{
		CString str_name = pDevInf->way_inf.way_data[i].way_name;
		for (int j = i+1; j < wayCount; j++)
		{
			CString com_name = pDevInf->way_inf.way_data[j].way_name;
			if (str_name == com_name && 0 != str_name.GetLength())
			{
				//	return -3;
			}
		}
	}
	for(int i = 0; i < wayCount; i++)
	{
		int way_id = pDevInf->way_inf.way_data[i].way_id;
		for (int j = i+1; j < wayCount; j++)
		{
			int way_id_compare = pDevInf->way_inf.way_data[j].way_id;
			if (way_id_compare == way_id && 0 != way_id)
			{
				return -31;
			}
		}
	}

	return 0;
}

int CAddDev::GetDevFactoryInf( int port_ind, MU_INT64 dev_id,  e_dev_sub_type sub_type)
{
	int rtn = 0;
	int len = 0;

	len = gXmlAnalyze.FormGetDevFactoryCmd(port_ind, dev_id, sub_type);
	rtn = gTransf.SendReqAndReceiveBody(len, READ_DEV_FACTORY_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseGetDevFactoryRsp(port_ind, dev_id, sub_type);
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


void CAddDev::OnBnClickedButtonGetFactory()
{
	CString str = "";
	int port_ind = pDevInf->rs485_conf.port;
	MU_INT64 dev_id = pDevInf->dev_addr.id;
	e_dev_type dev_type = pDevInf->dev_type;
	e_dev_sub_type sub_type = pDevInf->sub_type;
	int rtn = GetDevFactoryInf(port_ind, dev_id, sub_type);
	if (rtn != 0)
	{
		str = "获取出厂设置失败";
		MessageBox(str);
	}
	else
	{
		int dev_ind = GetDevIndByIdSubType(gXmlAnalyze.pOuterDev, dev_id, sub_type, port_ind);
		assert(dev_ind >= 0);
		str = gXmlAnalyze.pOuterDev->port_dev[port_ind].dev_inf[dev_ind].hw_ver;
		mHardVersion.SetWindowText(str);
		str = gXmlAnalyze.pOuterDev->port_dev[port_ind].dev_inf[dev_ind].sw_ver;
		mHardVersion.SetWindowText(str);
	}
}

void CAddDev::InitDataBitName()
{
	for (int i = 0; i < gDataBitNum; i++)
	{
		if (pDevInf->rs485_conf.databits == gDataBitValue[i])
		{
			mDataBit.SetCurSel(i);
			return;
		}
	}
	mDataBit.SetCurSel(1);
}

int CAddDev::SetDataBitValue()
{
	int ind = 0;
	ind = mDataBit.GetCurSel();
	if (ind < 0)
	{
		return -1;
	}
	pDevInf->rs485_conf.databits = gDataBitValue[ind];
	return 0;
}

void CAddDev::InitStopName()
{
	for (int i = 0; i < gStopNum; i++)
	{
		if (pDevInf->rs485_conf.stop == gStopValue[i])
		{
			mStop.SetCurSel(i);
			return;
		}
	}
	mStop.SetCurSel(0);
}

int CAddDev::SetStopValue()
{
	int ind = 0;
	ind = mStop.GetCurSel();
	if (ind < 0)
	{
		return -1;
	}
	pDevInf->rs485_conf.stop = gStopValue[ind];
	return 0;
}

UINT __cdecl AdaptThread(void *p)
{
	CAddDev* p_this = (CAddDev*)p;
	int sub_type = p_this->GetDataFrame()->threadParam.subType;
	MU_INT64 dev_addr = p_this->GetDataFrame()->threadParam.devAddr;

	st_rs485 rs485;
	memset(&rs485, 0, sizeof(rs485));
	int ret = -1;
	CString str;

	CDevAdaptRs485Cont* dev_rs485_cont = p_this->GetCDevAdaptRs485Cont();
	dev_rs485_cont->SetSubType(sub_type);
	deque<st_rs485> rs485_vec = dev_rs485_cont->GetRs485Cont();
	for (MU_INT32 i = 0; i < rs485_vec.size(); i++)
	{
		TRACE("[%s] i=%d",__FUNCTION__, i);
		rs485 = rs485_vec[i];
		COuterDevPort* p_COuterDevPort = p_this->GetParent();
		COuterDev* p_COuterDev = p_COuterDevPort->GetParentPointer();
		CProtModbusSet* p_def_modbus_set = p_COuterDev->GetDevDefModbusSet();
		CProtModbusSet* def_modbus_set = p_this->GetParent()->GetParentPointer()->GetDevDefModbusSet();
		CDevAdaptSendFrame* adapt_send_frame = new CDevAdaptSendFrame(sub_type, dev_addr, def_modbus_set);
		if (adapt_send_frame->sendFrameLen == 0)
		{
			ret = -1;
		}
		else
		{
			ret = p_this->Adapt(rs485, adapt_send_frame);
		}
		delete adapt_send_frame;
		if (ret == 0)
		{
			dev_rs485_cont->SetRs485(sub_type, rs485);
			break;
		}
	}

	if (ret == 0)
	{
		int comd_ind = 0;
		comd_ind = GetBaudIndByValue(rs485.baud);
		p_this->mBaud.SetCurSel(comd_ind);

		comd_ind = GetParityIndByValue(rs485.pari);
		p_this->mParity.SetCurSel(comd_ind);

		comd_ind = GetStopIndByValue(rs485.stop);
		p_this->mStop.SetCurSel(comd_ind);

		comd_ind = GetDataBitIndByValue(rs485.databits);
		p_this->mDataBit.SetCurSel(comd_ind);

		p_this->GetDataFrame()->tskResult = 0;
	}
	else
	{
		p_this->GetDataFrame()->tskResult = -1;
	}

	p_this->CloseProgress();

	return 0;
}

void CAddDev::OnBnClickedButtonAdapt()
{
	bool suc = false;
	CString str;

	st_rs485 rs485;
	memset(&rs485, 0, sizeof(rs485));

	int ind = mDevSubType.GetCurSel();
	assert(ind >= 0);
	int sub_type = gSubTypeValue[ind];

	MU_INT64 dev_addr = GetDevAddr();
	if (dev_addr == DEV_ID_ERR)
	{
		return;
	}

	int ret = pParent->GetParentPointer()->GetParent()->SetConnectFdTimeOut(ADAPT_DEV_TIMEOUT);
	if (ret != 0)
	{
		MessageBox("打开连接失败");
		return;
	}

	st_thread_tsk_param thread_param;
	memset(&thread_param, 0, sizeof(thread_param));
	thread_param.subType = sub_type;
	thread_param.devAddr = dev_addr;

	CTaskParam* data_frame = new CTaskParam(thread_param);
	dataFrame = data_frame; 

	AfxBeginThread( AdaptThread, this, THREAD_PRIORITY_ABOVE_NORMAL);

	str.Format("自适应");
	pProgress->SetProgressName(str);
	pProgress->DoModal();

	if (data_frame->tskResult == 0)
	{
		str.Format("自适应成功");
	}
	else
	{
		str.Format("自适应失败");
	}
	gLog.Add(str);
	if(data_frame)
	{
		delete data_frame;
		dataFrame = NULL;
	}
	MessageBox(str);
}

int CAddDev::Adapt(st_rs485& rs485, CDevAdaptSendFrame* send_frame)
{
	int rtn = 0;
	int len = 0;
	int port_ind = pParent->GetPortInd();

	len = gXmlAnalyze.FormAdaptDevCmd(rs485, send_frame, port_ind);
	rtn = gTransf.SendReqAndReceiveBody(len, SET_ADAPT_DEV_CMD);
	if(rtn == 0)
	{
		rtn = gXmlAnalyze.ParseAdaptDevRsp();
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


BOOL CAddDev::PreTranslateMessage(MSG* pMsg)
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
	return CDialogEx::PreTranslateMessage(pMsg);
}

MU_INT64 CAddDev::GetDevAddr()
{
	MU_INT64 id = 0;
	CString str;
	mDevId.GetWindowText(str);
	sscanf(str.GetString(),"%lld",&id);

	int ind = mDevSubType.GetCurSel();
	if (ind < 0)
	{
		MessageBox("设备子类型无效");
		id = DEV_ID_ERR;
		return id;
	}
	e_dev_sub_type sub_type = (e_dev_sub_type)gSubTypeValue[ind];

	//根据子类型判断输入范围
	if (sub_type == WM_01 ||
		sub_type == WM_02 ||
		sub_type == WM_03 ||
		sub_type == WM_04 ||
		sub_type == GM_01 ||
		sub_type == HM_01 ||
		sub_type == HM_02
		)
	{
		//水表汽表	
		int id_len = str.GetLength();
		if (id_len != 14)
		{
			MessageBox("地址长度必须14位");
			id = DEV_ID_ERR;
			return id;
		}
		if (id < 1 || id > 99999999999998)
		{
			MessageBox("id范围是1~99999999999998");
			id = DEV_ID_ERR;
			return id;
		}
	}
	else if (sub_type == AM_3PHASE_01 ||
		sub_type == AM_1PHASE_01 ||
		sub_type == AM_DC_01 
		)
	{
		//07协议	
		int id_len = str.GetLength();
		if (id_len != 12)
		{
			MessageBox("07协议地址长度必须12位");
			id = DEV_ID_ERR;
			return id;
		}
		if (id < 1 || id > 999999999998)
		{
			MessageBox("07协议id范围是000000000001~999999999998");
			id = DEV_ID_ERR;
			return id;
		}
	}
	else
	{
		//modBus协议
		if (id < 1 || id > 247)
		{
			MessageBox("id无效");
			id = DEV_ID_ERR;
			return id;
		}
	}
	return id;
}

void CAddDev::ControlLayout()
{
	//布局
#define WAY_INF_NUM 9
	CDialogEx* ctr[MAX_WAY_NUM][WAY_INF_NUM];
	CRect rect_arr[MAX_WAY_NUM];//每行首个控件

	for(int i = 0; i < MAX_WAY_NUM; i++)
	{
		ctr[i][0] = (CDialogEx*)pWayBit[i];
		ctr[i][1] = (CDialogEx*)pWayPos[i];
		ctr[i][2] = (CDialogEx*)pWayName[i];
		ctr[i][3] = (CDialogEx*)pWayDevType[i];
		ctr[i][4] = (CDialogEx*)pGdNodeId[i];
		ctr[i][5] = (CDialogEx*)pGdClassId[i];
		ctr[i][6] = (CDialogEx*)pGdItemId[i];
		ctr[i][7] = (CDialogEx*)pWayDevAddr[i];
		ctr[i][8] = (CDialogEx*)pWayDetail[i];
	}

	for (int i = 0; i < MAX_WAY_NUM; i++)
	{
		if (i == 0)
		{
			ctr[i][0]->GetWindowRect(rect_arr[i]);
			int width = rect_arr[i].Width();  
			width *= 2;
			rect_arr[i].right += width; 

			//找参考点
			CRect rect;			
			mWayMask.GetWindowRect(rect);

			//平移
			int off_left = rect.left - 39;
			int off_top = rect.top - 227;
			rect_arr[i].left -= off_left ;
			rect_arr[i].right -= off_left ;
			rect_arr[i].top -= off_top ;
			rect_arr[i].bottom -= off_top ;

			//放大
			int add_bottom = 4;
			int add_right = 18;
			rect_arr[i].bottom += add_bottom ;
			rect_arr[i].right += add_right;

		}
		else
		{
			int height = rect_arr[0].bottom - rect_arr[0].top;
			int down_off = (i)* height * 3 / 2;

			rect_arr[i] = rect_arr[0];
			rect_arr[i].top =  rect_arr[0].top + down_off;
			rect_arr[i].bottom =  rect_arr[i].top + height;
		}

		for (int j = 0; j < WAY_INF_NUM; j++)
		{
			CRect rect(rect_arr[i]);
			int width = rect_arr[i].right - rect_arr[i].left;
			int right_off = (j) * (width) * 1;
			rect.left = rect_arr[i].left + right_off;
			rect.right = rect.left + width;
			ctr[i][j]->MoveWindow(&rect);
		}
	}
}

void CAddDev::CloseProgress()
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
