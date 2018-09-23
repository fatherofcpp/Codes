#pragma once
#include "DataTypeDef.h"

// CCheckWay 对话框

class CShowDevAllWayData : public CDialog
{
	DECLARE_DYNAMIC(CShowDevAllWayData)

public:
	CShowDevAllWayData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowDevAllWayData();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHECK_WAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////////////////////////////////////
private:
	virtual BOOL OnInitDialog();
private:
	afx_msg void OnBnClickedButtonVailidData();
	afx_msg void OnBnClickedButtonInvalidData();
private:
	CListCtrl mList;
private:
	void CreateList();//创建列表
public:
	void SetDevAllWayData(int dev_type, VEC_DEV_ALL_WAY_DATA_INF & way_check_inf, st_report_format* report_format);//设置所有回路的数据
private:
	VEC_DEV_ALL_WAY_DATA_INF devAllWayData;//设备所有回路的数据集合
	e_dev_type devType;//设备类型
	st_report_format* pReportFormat;//当前上报参数
};
