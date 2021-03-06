// WMV_TO_H264Dlg.h : 头文件
//

#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "H264Stream.h"
#include "netvideoctrl1.h"
#include "ntgjdlg.h"
#include "microvideodlg.h"
#include "DfhsDlg.h"
#include "NtqyDlg.h"
#include "NthdDlg.h"
#include "RtspDlg.h"
#include "NetPosaDlg.h"
#include "VNMPDlg.h"
#include "H3CVMDlg.h"
#include "common.h"

#include "command.h"

typedef struct strSubInfor{
	MICP_HDR m_MicpHdr;//frame head
	int m_MicpSize;//receiced frame head size

	int m_RecvSize;//received data size
	int bIsHead;//1:是 0：不是 
	char *pRecvBuf;//frame data

	void *p;//连接的唯一性记录
	void *p1;//连接的唯一性记录
	
}SUB_INFOR;

// CWMV_TO_H264Dlg 对话框
class CWMV_TO_H264Dlg : public CDialog
{
// 构造
public:
	CWMV_TO_H264Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~CWMV_TO_H264Dlg(void);
	//int SourceCount;
// 对话框数据
	enum { IDD = IDD_WMV_TO_H264_DIALOG };
	HANDLE m_Lock;

	HANDLE m_wowzalock;

	CNTGJDlg *pCommonDlg[DEF_MAX_COMPANY];

	int g_Stop;
	CWinThread* m_hChangeThread;
	CWinThread* m_hPTZThread;

	SUB_INFOR m_subInfor[16];


	enum CONVERT_DEV_TYPE m_devType;
	char m_pubNameFlag[64];
	char m_secName[32];

	DEV_SHSL_PARAMS  m_devShslParams;

	CString ChangeCh(CString CamID, int CH,CString pubserver);
	int ClosePullFromSub(void* pStream, CONVERT_DEV_TYPE tmpType);
	int CreateNewSub(PVOID ref_mts);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
	CString   thisDir; //配置文件路径

	

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
	DECLARE_EVENTSINK_MAP()

	//保存正确的配置到ini文件中
	int SaveConfigIni(void * pParams,int nIndex);
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedOk();
	
};
