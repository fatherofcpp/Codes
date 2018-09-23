#pragma once


// CRestartRem 对话框

class CRestartRem : public CDialog
{
	DECLARE_DYNAMIC(CRestartRem)

public:
	CRestartRem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRestartRem();

// 对话框数据
	enum { IDD = IDD_DIALOG_WHETHER_RESTART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	////////////////////////////////
private:
	afx_msg void OnBnClickedButtonYes();
	afx_msg void OnBnClickedButtonLater();
};
