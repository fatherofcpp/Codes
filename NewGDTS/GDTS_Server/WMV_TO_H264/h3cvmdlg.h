#pragma once

//#include "cudevocxctrl1.h"
#include "common.h"

#include "H264Stream.h"
#include "SoucrcFilter.h"

#if 0
#include "afxwin.h"
#import "Svl_COMObject.tlb"

typedef Svl_COMObject::Svl_InterfacePtr SvlInterfacePtr;
typedef Svl_COMObject::Svl_Class SvlClass;
#endif
// CH3cVMDlg 对话框

typedef struct tagDEV_H3CVM{
	/*
	LONG CamId;
	CString Dstip;
	CString Dstpub;
	*/
	char m_type[64];			//服务器类型
	char m_ver[64];			//服务器版本
	char  m_proxyIp[32];		//服务器IP
	LONG  m_proxyPort;		//服务器端口
	char  m_userName[32];	//用户名
	char  m_passWord[32];	//密码
	int  m_deviceId;		//设备ID

	unsigned long m_playHandle;
	 
	char  m_dstIp[32];		//目标服务器IP
	char  m_dstPub[128];		//发布名
	LONG	 m_dstStream;	//流下标
}DEV_H3CVM_PARAMS;

#pragma once


// CH3cVMDlg 对话框

class CH3cVMDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CH3cVMDlg)

public:
	CH3cVMDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CH3cVMDlg();

	CSoucrcFilter m_SourceFilter;

	DEV_H3CVM_PARAMS  m_h3cVMParams[16];//
// 对话框数据
	enum { IDD = IDD_H3CVM_DIALOG };

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
