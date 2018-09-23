// MicrovideoDlg.h : 头文件
//

#pragma once

#include "MyFunction.h"

#include <evhttp.h>

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#include "inc.h"
#include "command.h"
#include "CriticalSection.h"
#include "afxwin.h"


typedef struct tagDEV_MICROVIDEO{//南京感动使用参数结构
 
	CString SrcUrl;
	CString ServerIP;
	int nPort;
	CString PubName;
	int StreamNumber;
	
}DEV_MICROVIDEO_PARAMS;

// CMicrovideoDlg 对话框
class CMicrovideoDlg : public CDialog
{
// 构造
public:
	CMicrovideoDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMicrovideoDlg(void);

// 对话框数据
	enum { IDD = IDD_MICROVIDEO_DIALOG };

	HANDLE m_wowzalock;
	HANDLE m_Lock;

	DEV_MICROVIDEO_PARAMS m_MicrovideoParams;
	STREAMPARAMS StreamParams;
	CArray<MyAvFrame*,MyAvFrame*> p_YuvArray;
	CCriticalSection m_streamlock;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);
	int SetParams(void *pStream,void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);
	CString ChangeChannel(CString sId,int CH,CString str);


};
