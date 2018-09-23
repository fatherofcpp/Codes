
// PantographMonitorDlg.h : 头文件
//

#pragma once
#include <deque>

// CPantographMonitorDlg 对话框
class CPantographMonitorDlg : public CDialogEx
{
// 构造
public:
	CPantographMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CPantographMonitorDlg();
// 对话框数据
	enum { IDD = IDD_PANTOGRAPHMONITOR_DIALOG };

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
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void AddTemp(const char* msg);
	void AddGeoParam(const char* msg);
	void AddArcStrength(const char* msg);
	void AddArcRate(const char* msg);
	void AddCurrent(const char* msg);
	void AddWarn(const char* msg);
	void AddStructFps(const char* msg);
	void AddTempFps(const char* msg);
	void AddArcCollectorFps(const char* msg);
	void AddGeoUpFps(const char* msg);
	void AddGeoDownLFps(const char* msg);
	void AddGeoDownRFps(const char* msg);
	void AddShowMsg(std::string msg);
	std::string PickShowMsg();
private:
	void InitUi();
	void Data2Ui();
private:
	std::string m_temp;

	std::string m_geoParam;

	std::string m_arcStrength;

	std::string m_arcRate;

	std::string m_current;

	std::string m_warn;

	std::string m_structFps;
	std::string m_tempFps;
	std::string m_arcColFps;
	std::string m_geoUpFps;
	std::string m_geoDownLFps;
	std::string m_geoDownRFps;

	std::deque<std::string> m_showMsg;
	CCriticalSection m_showMsgLock;
};
