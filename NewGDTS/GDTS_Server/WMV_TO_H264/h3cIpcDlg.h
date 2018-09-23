#pragma once
#include "common.h"
#include "h3cIPClib.h"

typedef struct tagDEV_H3CIPCWAY{
	CHAR m_szSDKUserID[IMOS_MW_NAME_LEN];
	ULONG m_ulStreamHd;
	BOOL m_bStreamOn; 
//	CVideoForm m_VideoPlayForm;

}DEV_H3CIPC_WAY;

typedef struct tagDEV_H3CIPC{
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

	DEV_H3CIPC_WAY m_ipcWay;
}DEV_H3CIPC_PARAMS;
// Ch3cIpcDlg 对话框

class Ch3cIpcDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(Ch3cIpcDlg)

public:
	Ch3cIpcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ch3cIpcDlg();


	DEV_H3CIPC_PARAMS  m_h3cIpcParams[16];//
	int FirstLog;
	HANDLE m_Locker;

// 对话框数据
	enum { IDD = IDD_H3CIPC_DIALOG };

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
