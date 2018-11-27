// WMV_TO_H264Dlg.h : ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "CriticalSection.h"
#include "H264Stream.h"

#include "microvideodlg.h"

#include "common.h"

// CWMV_TO_H264Dlg �Ի���
class CWMV_TO_H264Dlg : public CDialog
{
// ����
public:
	CWMV_TO_H264Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CWMV_TO_H264Dlg(void);
	int SourceCount;//reversed
	
	CString   thisDir; //�����ļ�·��
// �Ի�������
	enum { IDD = IDD_WMV_TO_H264_DIALOG };
	HANDLE m_Lock;//reversed

	HANDLE m_wowzalock;//reversed

	CMicrovideoDlg *pNtgjDlg;//class pointer of microvideo class

	int g_Stop;//exit this dialog
	CWinThread* m_hChangeThread;//get exit code of thread

	enum CONVERT_DEV_TYPE m_devType;//device type

	CCriticalSection m_ConfLock;//mutex lock of config

	int	cycleThreadIndOld;
	int	cycleThreadIndNew;//cycleThreadInd to check if cycleDetect is running
	int killTim;//certain kill time

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
	DECLARE_EVENTSINK_MAP()


	afx_msg void OnBnClickedCancel();//exit

	afx_msg void OnBnClickedOk();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
