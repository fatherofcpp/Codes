#pragma once
#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ValidEdit.h"
// CModBusDefParam 对话框
#define IDC_LIST_PARAM_DYNAMIC 1
class CParamDefModbusDlg : public CDialog
{
	DECLARE_DYNAMIC(CParamDefModbusDlg)

public:
	CParamDefModbusDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParamDefModbusDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROT_PARAM };
	//
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnNMClickListParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnCbnSelchangeComboDevType();
	afx_msg void OnCbnSelchangeComboWayInd();
	afx_msg void OnEnChangeEditWayNum();
public:
	friend class CParamDefModbusSetDlg;
public:
	void SetModbusParam(CProtModbus* mod_def, VEC_INT vec_id);
	void SetOuterDev(str_outer_dev* outer_dev);//指针拷贝，为了动态获取父对话框中的数据

private:
	void ShowModButton(bool show);//显示修改，删除的按钮
	void SetCombDefProt();// 设置自定义子类型集合,根据设备类型下标
	void SetCombWayChoos();// 设置回路选择集合,根据设备类型下标,注意：必须在回路数显示到界面后调用

	//list
	void CreateAllWayList();//创建列表
	void ShowListByWayInd();//根据设备类型下标显示列表
	void SetListParamAllWay();// 设置参数集合,根据设备类型下标
	void ClearListAllWay();//清空所有列表已有数据
	void GetListInf(int row_ind, st_param_info& param_info, int way_ind);//列表->私有数据
	void SetListInf(int row_ind, st_param_info& param_info, int way_ind);//私有数据->列表
private:
	CProtModbus* pCProtModbus;//协议
	VEC_INT vecId;//已存在的id
	//当前外接设备，自定义子类型使用情况
	//指针拷贝，为了动态获取父对话框中的数据
	str_outer_dev* pOuterDev;
	int maxWayNum;//最大回路数
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	// 协议类型
	CComboBox combSubType;
	// 协议名称
	ValidEdit editProtName;
	// 参数单位
	CComboBox combUnit;
	// 寄存器地址
	ValidEdit editRegisterAddr;
	// 寄存器长度
	ValidEdit editRegisterLen;
	CButton btModify;
	// 参数类表
	CListCtrl* listParamArr;
	// 设备类型
	CComboBox combDevType;
	CButton btOk;
	// 回路数
	ValidEdit editWayNum;
	// 参数信息映射
	CStatic labParamMapInf;
	// 回路号
	CComboBox combWayInd;
	// 各回路寄存器信息参照第一回路
	CButton checkRegIsSame;
	//add

};
