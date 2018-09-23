#pragma once
#include "stdafx.h"
#include "H264Stream.h"
#include "microvideo.h"

#include "common.h"
#include "hd_info.h"
// CNthdDlg �Ի���

class CNthdDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CNthdDlg)

public:
	CNthdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNthdDlg();

	DEV_HD_PARAMS m_devParams[16];
	//int SourceCount;
//	CH264Stream *m_h264[16];

	char m_proxyIp[32];	//��ý���������ַ
	int m_proxyPort;	//��ý��������˿�

// �Ի�������
	enum { IDD = IDD_NTHD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	int GetInforFromIni(void* iniPath);
	int GetInforFromIni4Company(void * pThisDir,int nIndex);

	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// ������������
	int SetParams(void *pStream,void* pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

};
