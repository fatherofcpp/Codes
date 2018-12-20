
// GeoMeasureDlg.h : 头文件
//

#pragma once

#include <string>

// CGeoMeasureDlg 对话框
class CGeoMeasureDlg : public CDialogEx
{
// 构造
public:
	CGeoMeasureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GEOMEASURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void AddDg(const char* msg);
	void AddLc(const char* msg);
	void AddStruct(const char* msg);
private:
	void InitUi();
	void Data2Ui();
	CString BootOpenFile(bool open = TRUE);//TRUE表示打开,FALSE表示保存
private:
	std::string m_dg;
	std::string m_lc;
	std::string m_struct;
public:
	bool m_isContinueShow;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonStructM();
	afx_msg void OnBnClickedButtonMeasure();
	afx_msg void OnBnClickedButtonShow();
	afx_msg void OnBnClickedButtonFrameDeal();
};
