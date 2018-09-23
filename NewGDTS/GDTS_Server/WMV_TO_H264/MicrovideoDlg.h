#pragma once

#include "stdafx.h"
#include "H264Stream.h"
#include "microvideo.h"

#include "common.h"
// CMicrovideoDlg 对话框

class CMicrovideoDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CMicrovideoDlg)

public:
	CMicrovideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMicrovideoDlg();

// 对话框数据
	enum { IDD = IDD_MICROVIDEO_DIALOG };

	//int SourceCount;
	//CH264Stream *m_h264[16];
#if !USENEWSTR
	DEV_MICROVIDEO_PARAMS m_MicrovideoParams;
#endif
	New_DEV_PARAMS m_DevParams[16];

	STREAMPARAMS StreamParams[16];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int GetInforFromIni(void* iniPath);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);
	int SetParams(void *pStream,void * pParams);
	// 启动拉流函数
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

	CString CheckExit(CString str);

	CString ChangeChannel(CString sId,int CH,CString str);

	//encode
	int InitEncode(void *pParams);
	int EncodeFrame(void *pIn,void *pOut);
	int CloseEncode(void *pParams);

	void MySleep(int iDelay,int iCount);

};
