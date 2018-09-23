#pragma once
#include "H264Stream.h"

#include "vnmpLib.h"
#include "common.h"

// CVNMPDlg 对话框

class CVNMPDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CVNMPDlg)

public:
	CVNMPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVNMPDlg();

	DEV_VNMP_PARAMS m_devParams[16];
	//int SourceCount;
	//CH264Stream *m_h264[16];

// 对话框数据
	enum { IDD = IDD_VNMP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
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
