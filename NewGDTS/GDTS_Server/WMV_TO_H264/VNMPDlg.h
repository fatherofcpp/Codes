#pragma once
#include "H264Stream.h"

#include "vnmpLib.h"
#include "common.h"

// CVNMPDlg �Ի���

class CVNMPDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CVNMPDlg)

public:
	CVNMPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVNMPDlg();

	DEV_VNMP_PARAMS m_devParams[16];
	//int SourceCount;
	//CH264Stream *m_h264[16];

// �Ի�������
	enum { IDD = IDD_VNMP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
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
