
// WatchDogProgressDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CWatchDogProgressDlg �Ի���
class CWatchDogProgressDlg : public CDialogEx
{
// ����
public:
	CWatchDogProgressDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WATCHDOGPROGRESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editShow;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
