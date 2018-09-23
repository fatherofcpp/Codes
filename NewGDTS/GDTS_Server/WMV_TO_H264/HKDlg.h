#pragma once
#include "stdafx.h"
#include "H264Stream.h"
#include "microvideo.h"

#include "common.h"
#include "hk_info.h"

// CHKDlg 对话框

class CHKDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CHKDlg)

public:
	CHKDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHKDlg();

	DEV_HK_PARAMS m_devParams[16];

// 对话框数据
	enum { IDD = IDD_HK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int GetInforFromIni(void* iniPath);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// 启动拉流函数
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);
};
