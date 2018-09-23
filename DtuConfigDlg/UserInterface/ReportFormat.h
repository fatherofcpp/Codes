#pragma once

#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "ValidEdit.h"
#include "afxwin.h"
#include "afxcmn.h"
// CReportFormat 对话框

class CReportFormat : public CDialog
{
	DECLARE_DYNAMIC(CReportFormat)

public:
	CReportFormat(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReportFormat();

	// 对话框数据
	enum { IDD = IDD_DIALOG_REPORT_FORMAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////////////////

private:
	//重载
	virtual BOOL OnInitDialog();//对话框初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	//静态控件
	CComboBox mCombReportParam;
	CButton mBtAddParam;
	CButton mBtDeleteParam;
	CButton mBtModifyParam;
	CButton mBtAllDevDefaultParamSet;//所有设备默认上报参数
	CComboBox mComBDevType;
	// 所有设备上报参量预览列表
	CListCtrl listAllDevReportView;
	// 当前设备默认上报参量
	CButton btCurDevDefaultReportParam;
	//动态控件
	CListCtrl* m_pListCtrl;//列表数组,列表下标与下列设备类型对应

	//控件响应
	afx_msg void OnBnClickedButtonAddParam();
	afx_msg void OnBnClickedButtonDeleteParam();
	afx_msg void OnBnClickedButtonModifyParam();
	afx_msg void OnCbnSelchangeComboDevtype();
	afx_msg void OnBnClickedButtonAllDevDefaultParamSet();
	afx_msg void OnBnClickedButtonCurDevDefaultParam();
	//控件接口
	void SetCombReportParam();// 设置下拉框选项,根据设备类型下标
	void SetCombDevType();

	void CreateAllDevList();//创建列表
	void ShowListByDevTypeInd();//根据设备类型下标显示列表
	void ClearListDataAllDev();//清空所有列表已有数据

	void GetListControlAllDev();//列表->私有数据
	void SetListControlAllDev();//私有数据->列表

	void DisplaySaveButton();//显示保存按钮

	bool IsParamNameExist(string param_name);//增加参数，修改参数时判断当前设备类型下是否存在改参数名
	void SetAllDevDefaultParam();//按默认全选方式设置每个设备类型下的上报参数

	void CreateListAllDevReportView();//创建列表
	void ClearListDataAllDevReportView();//清空所有列表已有数据
	void SetListControlAllDevReportView();//私有数据->列表
	void SetDefaultParamByListInd(int list_ind);//按默认全选方式设置当前设备类型下的上报参数
	void ClearListDataByListInd(int list_ind);//清空当前设备列表已有数据

	//私有数据接口
	void SetModify(bool& modify);
	void ClearReportFormat();

	void ShowInput(bool show);//显示

public:
	//防控权限
	friend class CDtuConfigDlgDlg;

public:
	//对外接口
	void GetReportFormat(st_report_store_inf* report_format);
	void SetReportFormat(st_report_store_inf* report_format);
	void InitDlg();//私有数据->对话框
	int SaveDlg();//对话框->私有数据

private:
	//私有数据
	int devTypeNum;//设备类型个数
	int currentDevTypeInd;//当前列表下标,设备类型的下标	
	CParamDevTypeSet* pCParamDevSet;//电表参数对照,下拉框CDevTypeReportParam
	st_report_store_inf* pReportFormatStore;//存储上报格式,当前配置
	///////////////////////////////add
};
