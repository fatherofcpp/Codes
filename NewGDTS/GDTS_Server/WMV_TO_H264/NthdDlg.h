#pragma once
#include "stdafx.h"
#include "H264Stream.h"
#include "microvideo.h"

#include "common.h"
#include "hd_info.h"
// CNthdDlg 对话框

class CNthdDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CNthdDlg)

public:
	CNthdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNthdDlg();

	DEV_HD_PARAMS m_devParams[16];
	//int SourceCount;
//	CH264Stream *m_h264[16];

	char m_proxyIp[32];	//流媒体服务器地址
	int m_proxyPort;	//流媒体服务器端口

// 对话框数据
	enum { IDD = IDD_NTHD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	int GetInforFromIni(void* iniPath);
	int GetInforFromIni4Company(void * pThisDir,int nIndex);

	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// 启动拉流函数
	int SetParams(void *pStream,void* pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

};
