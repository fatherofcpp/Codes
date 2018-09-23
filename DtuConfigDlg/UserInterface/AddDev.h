#pragma once
#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "ValidEdit.h"
#include "afxwin.h"
#include "Progress.h"

// CAddDev 对话框
class CAddDev : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDev)

public:
	CAddDev(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDev();

	// 对话框数据
	enum { IDD = IDD_DIALOG_ADDDEV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////////////////////////
public:
	friend class COuterDevPort;
private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedWMBit1();
	afx_msg void OnBnClickedWMBit2();
	afx_msg void OnBnClickedWMBit3();
	afx_msg void OnBnClickedWMBit4();
	afx_msg void OnBnClickedCheckWm5();
	afx_msg void OnBnClickedCheckWm6();
	afx_msg void OnBnClickedCheckWm7();
	afx_msg void OnBnClickedCheckWm8();
	afx_msg void OnBnClickedCheckWm9();
	afx_msg void OnBnClickedCheckWm10();
	afx_msg void OnBnClickedCheckWm11();
	afx_msg void OnBnClickedCheckWm12();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedButtonGetFactory();
	afx_msg void OnBnClickedButtonAdapt();

public:
	// 数据位
	CComboBox mDataBit;
	// 停止位
	CComboBox mStop;
	CComboBox mBaud;//	9600;115200
	CComboBox mParity;//NONE;ODD;EVEN;MARK;SPACE;
private:
	ValidEdit mDevId;
	ValidEdit mDevName;
	ValidEdit mWayMask;

	CButton btAdapt;// 自适应下行通讯参数,波特率，校验，数据位，停止位

	CComboBox mDevSubType;
	CEdit mDevType;

	// 设备区域名称
	ValidEdit mDevPos;
	// 硬件版本号
	ValidEdit mHardVersion;
	// 软件版本号
	ValidEdit mSoftVersion;

	CButton mWayBit1;
	CButton mWayBit2;
	CButton mWayBit3;
	CButton mWayBit4;
	CButton mWayBit5;
	CButton mWayBit6;
	CButton mWayBit7;
	CButton mWayBit8;
	CButton mWayBit9;
	CButton mWayBit10;
	CButton mWayBit11;
	CButton mWayBit12;

	ValidEdit mWayPos1;
	ValidEdit mWayPos2;
	ValidEdit mWayPos3;
	ValidEdit mWayPos4;
	ValidEdit mWayPos5;
	ValidEdit mWayPos6;
	ValidEdit mWayPos7;
	ValidEdit mWayPos8;
	ValidEdit mWayPos9;
	ValidEdit mWayPos10;
	ValidEdit mWayPos11;
	ValidEdit mWayPos12;

	ValidEdit mWayName1;
	ValidEdit mWayName2;
	ValidEdit mWayName3;
	ValidEdit mWayName4;
	ValidEdit mWayName5;
	ValidEdit mWayName6;
	ValidEdit mWayName7;
	ValidEdit mWayName8;
	ValidEdit mWayName9;
	ValidEdit mWayName10;
	ValidEdit mWayName11;
	ValidEdit mWayName12;

	CComboBox mWayDevType1;
	CComboBox mWayDevType2;
	CComboBox mWayDevType3;
	CComboBox mWayDevType4;
	CComboBox mWayDevType5;
	CComboBox mWayDevType6;
	CComboBox mWayDevType7;
	CComboBox mWayDevType8;
	CComboBox mWayDevType9;
	CComboBox mWayDevType10;
	CComboBox mWayDevType11;
	CComboBox mWayDevType12;

	ValidEdit mGdNodeId1;
	ValidEdit mGdNodeId2;
	ValidEdit mGdNodeId3;
	ValidEdit mGdNodeId4;
	ValidEdit mGdNodeId5;
	ValidEdit mGdNodeId6;
	ValidEdit mGdNodeId7;
	ValidEdit mGdNodeId8;
	ValidEdit mGdNodeId9;
	ValidEdit mGdNodeId10;
	ValidEdit mGdNodeId11;
	ValidEdit mGdNodeId12;

	ValidEdit mGdClassId1;
	ValidEdit mGdClassId2;
	ValidEdit mGdClassId3;
	ValidEdit mGdClassId4;
	ValidEdit mGdClassId5;
	ValidEdit mGdClassId6;
	ValidEdit mGdClassId7;
	ValidEdit mGdClassId8;
	ValidEdit mGdClassId9;
	ValidEdit mGdClassId10;
	ValidEdit mGdClassId11;
	ValidEdit mGdClassId12;

	ValidEdit mGdItemId1;
	ValidEdit mGdItemId2;
	ValidEdit mGdItemId3;
	ValidEdit mGdItemId4;
	ValidEdit mGdItemId5;
	ValidEdit mGdItemId6;
	ValidEdit mGdItemId7;
	ValidEdit mGdItemId8;
	ValidEdit mGdItemId9;
	ValidEdit mGdItemId10;
	ValidEdit mGdItemId11;
	ValidEdit mGdItemId12;

	ValidEdit mWayDevAddr1;
	ValidEdit mWayDevAddr2;
	ValidEdit mWayDevAddr3;
	ValidEdit mWayDevAddr4;
	ValidEdit mWayDevAddr5;
	ValidEdit mWayDevAddr6;
	ValidEdit mWayDevAddr7;
	ValidEdit mWayDevAddr8;
	ValidEdit mWayDevAddr9;
	ValidEdit mWayDevAddr10;
	ValidEdit mWayDevAddr11;
	ValidEdit mWayDevAddr12;

	ValidEdit mWayDetail1;
	ValidEdit mWayDetail2;
	ValidEdit mWayDetail3;
	ValidEdit mWayDetail4;
	ValidEdit mWayDetail5;
	ValidEdit mWayDetail6;
	ValidEdit mWayDetail7;
	ValidEdit mWayDetail8;
	ValidEdit mWayDetail9;
	ValidEdit mWayDetail10;
	ValidEdit mWayDetail11;
	ValidEdit mWayDetail12;

private:
	int wayCount;//回路个数
	int wayMask;//回路掩码
	CButton* pWayBit[MAX_WAY_NUM];//界面回路编号
	ValidEdit* pWayPos[MAX_WAY_NUM]; //回路编号
	ValidEdit* pWayName[MAX_WAY_NUM];//回路名称
	CComboBox* pWayDevType[MAX_WAY_NUM];//回路设备类型
	ValidEdit* pGdNodeId[MAX_WAY_NUM];//回路gd_node_id
	ValidEdit* pGdClassId[MAX_WAY_NUM];//回路gd_class_id
	ValidEdit* pGdItemId[MAX_WAY_NUM];//回路gd_item_id
	ValidEdit* pWayDevAddr[MAX_WAY_NUM];//回路设备地址
	ValidEdit* pWayDetail[MAX_WAY_NUM];//回路详细说明

private:
	void InitDevSubTypeName();//根据pOuterDev->type值找到名称数组下标
	int SetDevSubTypeValue();
	void InitDataBitName();//
	int SetDataBitValue();
	void InitStopName();//
	int SetStopValue();
	void InitBaudName();//根据pOuterDev->baudRate值找到名称数组下标
	int SetBaudValue();
	void InitParityName();//根据pOuterDev->parity值找到名称数组下标
	int SetParityValue();
	void ShowWayControlByWayCount();//根据回路个数,显示可用的回路状态控件,控件值为空
	void InitWayControl();//根据回路个数及掩码，初始化回路控件的值及状态
	void EnableWayControlByMaskChanged();//回路选中状态改变时更新掩码值及控件使用权限
	MU_INT64 GetDevAddr();//获取界面设备地址

	void ControlLayout();//回路控件布局
private:
	int SetWayInf();//保存回路信息
	int GetDevFactoryInf(int port_ind, MU_INT64 dev_id,  e_dev_sub_type sub_type);//获取设备出厂信息

public:
	//设备信息，所有端口的id subtype集合，
	void SetDevInf(st_dev_inf* dev_inf, 
		VEC_DEVID_SUBTYPE& vec_id_subtype,
		VEC_CSTRING& vec_way_name, 
		VEC_INT vec_way_id,
		COuterDevPort* p_parent);

	int Adapt(st_rs485& rs485, CDevAdaptSendFrame* send_frame);

	CTaskParam* GetDataFrame(){return dataFrame;};
	CProgress* GetProgress(){return pProgress;};
	COuterDevPort* GetParent()
	{
		return pParent;
	};
	void CloseProgress();//关闭进度条

	CDevAdaptRs485Cont* GetCDevAdaptRs485Cont(){return devRs485Cont;};
private:
	VEC_DEVID_SUBTYPE vecIdSubType;//此端口其他设备id subtype集合
	VEC_CSTRING vecWayName;//所有回路名称集合
	VEC_INT vecWayId;//所有回路编号，唯一
	st_dev_inf* pDevInf;//设备信息 
	CDevAdaptRs485Cont* devRs485Cont;//485通讯参数

	CTaskParam* dataFrame;
	CProgress* pProgress;
	COuterDevPort* pParent;//

	///////////////////////////////////////////////////////////////////////////////////add

};
