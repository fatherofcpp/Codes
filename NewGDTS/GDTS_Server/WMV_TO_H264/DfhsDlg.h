#pragma once
#include "cudevocxctrl1.h"
#include "common.h"
#include "dfhs.h"
#include "H264Stream.h"


// CDfhsDlg �Ի���

class CDfhsDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CDfhsDlg)

public:
	CDfhsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDfhsDlg();

	//int SourceCount;
//	CH264Stream *m_h264[16];
	DEV_DFHS_PARAMS m_devDfhsParams;

// �Ի�������
	enum { IDD = IDD_DFHS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CCudevocxctrl1 m_CudevOcx;
	DECLARE_EVENTSINK_MAP()
	void OCXInnerNotifyCudevocxctrl1(long nEvent);
	int InitProxyServer(void * pParams);

	int GetInforFromIni(void* iniPath);
	int GetInforFromIni4Company(void * pThisDir,int nIndex);

	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	//������������
	int SetParams(void *pStream,void * pParams);
	// ������������
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

	void MySleep(int iDelay,int iCount);
	afx_msg void OnBnClickedCancel();
};
