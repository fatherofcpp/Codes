#pragma once
// CProgress �Ի���

class CProgress : public CDialog
{
	DECLARE_DYNAMIC(CProgress)

public:
	CProgress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgress();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	int minRangle;//��С����ֵ
	int maxRangle;//������ֵ
	int curStep;//��ǰ����ֵ
	bool AutoProgress;//�޷�������ȣ��Զ�����
	CString progressName;//����������
	bool progressRun;//�߳��йر�progressRunΪtrue�Ľ��ȶԻ���

	////////////////////////////////////add
public:
	afx_msg void OnClose();
};
