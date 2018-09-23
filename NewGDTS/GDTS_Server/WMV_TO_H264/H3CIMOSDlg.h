#pragma once

#include "common.h"
#include "h3cLib.h"

// CH3CIMOSDlg 对话框

class CH3CIMOSDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CH3CIMOSDlg)

public:
	CH3CIMOSDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CH3CIMOSDlg();

	DEV_H3C_PARAMS m_h3cParams[16];

// 对话框数据
	enum { IDD = IDD_H3CIMOS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//读取配置文件信息
	int ParseRequest(char * pXmlBuf,CString *str);
	//int GetInforFromIni(void* iniPath);
	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	//如果存在配置文件不全的，则调用该函数补全.
	int SetDefaultParams(void * pParams);
	//设置拉流参数
	int SetParams(void *pStream,void * pParams);
	// 启动拉流函数
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

	CString ChangeChannel(CString sId,int CH,CString str);

	CString CheckExit(CString str);
};
