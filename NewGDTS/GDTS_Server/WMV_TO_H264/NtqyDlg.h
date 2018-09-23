#pragma once

#include "common.h"
#include "ntqy.h"
#include "h3cLib.h"
#include "H264Stream.h"
#include "APILNTPlayer.h"
// CNtqyDlg 对话框

class CNtqyDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CNtqyDlg)

public:
	CNtqyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNtqyDlg();


	//int SourceCount;
//	CH264Stream *m_h264[16];
	DEV_NTQY_PARAMS m_devParams;
//	DEV_H3C_PARAMS m_h3cParams[16];
	CONVERT_DEV_TYPE m_type;
	

// 对话框数据
	enum { IDD = IDD_NTQY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
     static void CALLBACK MyCallBackFun(const char *ipc,int datatype,const char *buf,unsigned int len,void*user);
	 static void CALLBACK MsgBackFunc(int type,int code,const char *msg,const char *ipc);

public:
	int GetInforFromIni(void* iniPath);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);
	int GetInforFromIni4Company(void * pThisDir,int nIndex);


	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

		//设置拉流参数
	int SetParams(void *pStream,void * pParams);
	// 启动拉流函数
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);
};
