#pragma once
#include "DataTypeDef.h"

// CModifyTime �Ի���

class CModifyTime : public CDialog
{
	DECLARE_DYNAMIC(CModifyTime)

public:
	CModifyTime(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyTime();

// �Ի�������
	enum { IDD = IDD_DIALOG_UPDATA_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
