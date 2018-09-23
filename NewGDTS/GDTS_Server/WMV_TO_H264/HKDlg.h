#pragma once
#include "stdafx.h"
#include "H264Stream.h"
#include "microvideo.h"

#include "common.h"
#include "hk_info.h"

// CHKDlg �Ի���

class CHKDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CHKDlg)

public:
	CHKDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHKDlg();

	DEV_HK_PARAMS m_devParams[16];

// �Ի�������
	enum { IDD = IDD_HK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int GetInforFromIni(void* iniPath);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// ������������
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);
};
