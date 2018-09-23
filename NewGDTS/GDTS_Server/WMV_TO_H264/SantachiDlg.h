#pragma once

#include "common.h"

typedef struct tagDEV_SANTACHI_INFO{
 char		m_dstIp[32];		//目标服务器IP
 char		m_dstPub[64];		//发布名
 LONG		m_dstStream;	//流下标
 LONG		m_userId;//UserID
 LONG		m_realPlayHandle;//RealPlayHandle
 HANDLE		m_hMsgheartBeatThread;
}DEV_SANTACHI_INFO;

typedef struct tagDEV_SANTACHI{//金三立使用参数结构
 char		m_proxyIp[64];		//编码器IP
 LONG		m_proxyPort;		//编码器端口
 char		m_userName[32];		//用户名
 char		m_passWord[32];		//密码
 LONG		m_deviceChn;	//设备通道
 LONG		m_playChn;		//播放通道

 DEV_SANTACHI_INFO m_devInfo;


}DEV_SANTACHI_PARAMS;

// CSantachiDlg 对话框

class CSantachiDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CSantachiDlg)

public:
	CSantachiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSantachiDlg();

	DEV_SANTACHI_PARAMS m_devParams[16];


// 对话框数据
	enum { IDD = IDD_SANTACHI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

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
