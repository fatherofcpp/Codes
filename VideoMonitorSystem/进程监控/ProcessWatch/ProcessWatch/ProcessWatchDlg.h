
// ProcessWatchDlg.h : 头文件
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

// CProcessWatchDlg 对话框
class CProcessWatchDlg : public CDialogEx
{
// 构造
public:
	CProcessWatchDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PROCESSWATCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);


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
	int TaskStart();
	int TaskStop();
	int LoadConfig();
	int Init();
	static unsigned int WINAPI thread_recv(void* pParam);
	NSP_STP_CM::CThreadPool m_CThreadPool;
	StConf m_StConf;
	virtual void OnCancel();
};
