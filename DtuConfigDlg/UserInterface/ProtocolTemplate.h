#pragma once
#include "DataTypeDef.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ModBusDefParam.h"

// CProtocolTemplate 对话框

class CParamDefModbusSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CParamDefModbusSetDlg)

public:
	CParamDefModbusSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParamDefModbusSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROTO_TEMPLATE };
	//
	friend class CDtuConfigDlgDlg;
public:
	void SetModbusDefArray(CProtModbusSet* param_def_modbus_set);//设置数据
	void InitDlg();//数据->对话框
	int SaveDlg();//对话框->数据
	bool GetIsModify(){return isModify;};
	void SetOuterDev(str_outer_dev* outer_dev);
private:
	void CreateList();//创建列表
	void DisplaySaveButton();//显示保存按钮
	void ShowDelModCheckButton(bool show);//显示修改，删除，实时数据的按钮
	int  GetListInf(int row_ind, CProtModbus& mod_temp);//从list相应行提取设备信息
	void SetListInf(int row_ind, CProtModbus& mod_temp);//根据设备信息设置list相应行信息
	void InitMapSubTypeProt();//初始化协议id与参量信息映射
	void AddMapIdProt(int id, int prot_modbus_arr_ind);//增加协议id与参量信息映射
	void DelMapIdProt(int id);//删除协议id与参量信息映射
	void GetAllIdExceptParam(VEC_INT& vec_id,int& except_id);//统计所有端口除了参数之外的其它所有id集合
	int GetParamInfById(int id, CParamModbus& param_modbus);//通过id获取模板的参量信息
	int SetParamInfById(int id, CParamModbus& param_modbus);//通过id保存模板的参量信息
	void ShowInput(bool show);//显示

private:
	CProtModbusSet* pCProtModbusSet;//自定义子类型数组

	CParamDefModbusDlg* pCParamDefModbusDlg;//协议对话框
	MAP_INT_INT mapSubTypeProt;//子类型,CParamDevSet
	bool isModify;
	str_outer_dev* outerDev;//当前外接设备，自定义子类型使用情况
	//
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton btAdd;
	CButton btDelete;
	CButton btModify;
	// 协议模板列表
	CListCtrl listProtTemplate;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnNMClickListProtocolTemplate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListProtocolTemplate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadLocalDefSubtype();
	afx_msg void OnBnClickedButtonSaveLocalDefSubtype();
	// 加载本地自定义子类型
	CButton btLoadLocalDef;
	// 保存本地自定义子类型
	CButton btSaveLocalDef;
};
