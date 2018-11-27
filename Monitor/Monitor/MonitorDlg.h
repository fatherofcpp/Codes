
// MonitorDlg.h : ͷ�ļ�
//

#pragma once

#define WM_SHOWTASK (WM_USER +1)

typedef  int (WINAPI * HUNG_FUNNT) (HWND);


// CMonitorDlg �Ի���
class CMonitorDlg : public CDialogEx
{
// ����
public:
	CMonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
