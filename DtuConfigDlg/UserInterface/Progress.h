#pragma once
// CProgress 对话框

class CProgress : public CDialog
{
	DECLARE_DYNAMIC(CProgress)

public:
	CProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgress();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////////////
private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CProgressCtrl mProgressButton;
	CStatic mProgressName;

public:
	void SetMinRangle(int val);
	void SetMaxRangle(int val);
	void SetCurStep(int val);
	void SetProgressName(CString& progress_name);
	void SetAutoProgress(bool is_auto);
	bool GetRunFlag();

private:
	int minRangle;//最小进度值
	int maxRangle;//最大进度值
	int curStep;//当前进度值
	bool AutoProgress;//无法估算进度，自动进度
	CString progressName;//进度条名称
	bool progressRun;//线程中关闭progressRun为true的进度对话框

	////////////////////////////////////add
public:
	afx_msg void OnClose();
};
