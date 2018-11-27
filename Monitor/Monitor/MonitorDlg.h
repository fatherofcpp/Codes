
// MonitorDlg.h : 头文件
//

#pragma once

#define WM_SHOWTASK (WM_USER +1)

typedef  int (WINAPI * HUNG_FUNNT) (HWND);


// CMonitorDlg 对话框
class CMonitorDlg : public CDialogEx
{
// 构造
public:
	CMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcPaint();
private:
	NOTIFYICONDATA m_nid;
	CStatic m_pict_ctrl;
	int m_cnt;
	HUNG_FUNNT m_IsHungNT;
	DWORD m_ProcID;
	HWND m_hwnd;
protected:
	afx_msg LRESULT OnShowtask(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMenuExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
private:
	void LogDebugFile(CString &Content);
	bool GetProcIDByExeName(DWORD &ProcessID, char* ExeName);
	HWND GetHwndByPid(DWORD ProcessID);
	bool IsTimeEqual(FILETIME &A, FILETIME &B);
private:
	void WatchDog(void);
	bool ProcIsExist(void);
	bool ProcIsOK(void);
	void StartProc(void);
};
