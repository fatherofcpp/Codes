
// ProcessWatchDlg.h : ͷ�ļ�
//

#pragma once
#include "CmFunction/CmFunction.h"
#include "Thread/CmThreadPool.h"
#include "Log/CmLog.h"
#include "Socket/CmTransUdp.h"

typedef struct tagStConf
{
	tagStConf()
		: udpPortRecv(0)
		, disconnectSec(0)
		, isReboot(0)
		, waitSystemTs(0)
	{

	}

	int udpPortRecv;
	int disconnectSec;
	int isReboot;
	int waitSystemTs;
}StConf;

// CProcessWatchDlg �Ի���
class CProcessWatchDlg : public CDialogEx
{
// ����
public:
	CProcessWatchDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PROCESSWATCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);


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
	int TaskStart();
	int TaskStop();
	int LoadConfig();
	int Init();
	static unsigned int WINAPI thread_recv(void* pParam);
	NSP_STP_CM::CThreadPool m_CThreadPool;
	StConf m_StConf;
	virtual void OnCancel();
};
