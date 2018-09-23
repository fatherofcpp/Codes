#pragma once
#include "afxwin.h"


// CHistoryData 对话框

class CHistoryData : public CDialog
{
	DECLARE_DYNAMIC(CHistoryData)

public:
	CHistoryData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistoryData();

// 对话框数据
	enum { IDD = IDD_DIALOG_HISTORYDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit mHistoryData;
private:
	char* historyData;
	int dataLen;
public:
	void SetData(char* data, int data_len);
};
