#pragma once
#include "DataTypeDef.h"

// CModifyTime 对话框

class CModifyTime : public CDialog
{
	DECLARE_DYNAMIC(CModifyTime)

public:
	CModifyTime(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyTime();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATA_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////
public:
	friend class CColtor;
	friend class CDtuConfigDlgDlg;

private:
	CDateTimeCtrl mYMD;
	CDateTimeCtrl mHMS;
private:
	afx_msg void OnBnClickedUpData();
	afx_msg void OnBnClickedCancel();
private:
	st_time* pTime;
};
