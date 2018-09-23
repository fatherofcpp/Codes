#pragma once
#include "stdafx.h"
#include "H264Stream.h"
#include "rtspLib.h"

#include "common.h"

#include "rtspHead.h"

// CRtspDlg �Ի���

class CRtspDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CRtspDlg)

public:
	CRtspDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRtspDlg();

// �Ի�������
	enum { IDD = IDD_RTSP_DIALOG };

	DSS_RTSP m_dssRtsp[16];
	CH264Stream *m_h264[16];
	DEV_RTSP_PARAMS m_devParams[16];
	//int SourceCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();


	//int GetInforFromIni(void* iniPath);
	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int SetDefaultParams(void * pParams);
#if 0
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// ������������
#endif
	int SetParams(void *pStream,void* pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

	afx_msg void OnBnClickedCancel();
};
