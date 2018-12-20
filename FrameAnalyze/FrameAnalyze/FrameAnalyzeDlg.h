
// FrameAnalyzeDlg.h : 头文件
//

#pragma once

#include <string>
#include "bsmycanvas.h"

// CFrameAnalyzeDlg 对话框
class CFrameAnalyzeDlg : public CDialogEx
{
// 构造
public:
	CFrameAnalyzeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FRAMEANALYZE_DIALOG };

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
	void AddLd(const char* msg);
	void AddDbd(const char* msg);
	void AddFbl(const char* msg);
	void AddTotal(const char* msg);
	void AddReverse1(const char* msg);
	void AddReverse2(const char* msg);
private:
	void InitUi();
	void Data2Ui();
	CString BootOpenFile(bool open = TRUE);//TRUE表示打开,FALSE表示保存
	void LoadBg();
	void ReSize();  
	void AnalyzeFrame(std::string file_path, int frame_pos);
private:
	std::string m_ld;
	std::string m_dbd;
	std::string m_fbl;
	std::string m_total;
	std::string m_reverse1;
	std::string m_reverse2;

	POINT m_oldPoint;  
	UINT_PTR m_IDEvent;
	CMyCanvas m_Image;
private:
	typedef enum tagEArea
	{
		E_AREA_ZT=-1,
		E_AREA_DB,
		E_AREA_QB,
		E_AREA_ZSJ,
	};

	const std::string m_cBtZt;
	const std::string m_cBtDb;
	const std::string m_cBtZsj;
	const std::string m_cBtQb;
	const std::string m_cTitle;
	const int m_cWidth;
	const int m_cHeight;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonZt();
	afx_msg void OnBnClickedButtonZs();
	afx_msg void OnBnClickedButtonDb();
	afx_msg void OnBnClickedButtonQb();
};
