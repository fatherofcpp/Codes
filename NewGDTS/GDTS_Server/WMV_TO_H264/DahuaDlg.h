#pragma once
#include "stdafx.h"
#include "H264Stream.h"

#include "common.h"


typedef struct tagDEV_DH_INFO{
	char		m_dstIp[32];		//目标服务器IP
	char		m_dstPub[64];		//发布名
	long		m_dstStream;	//流下标
	long		m_loginid;		//登录标识
	//LLONG		realLiveHd;
	long		realPlayerHd;	//播放标识

}DEV_DH_INFO;

typedef struct tagDEV_DH{//浙江大华使用参数结构
	char		m_proxyIp[64];		//转发服务器IP
	long		m_proxyPort;		//转发服务器端口
	char		m_userName[32];		//用户名
	char		m_passWord[32];		//密码
	
	char		m_devName[64];		//设备通道
	char		m_channel[64];		//播放通道

	DEV_DH_INFO m_devInfo;


}DEV_DH_PARAMS;

// CDahuaDlg 对话框

class CDahuaDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CDahuaDlg)

public:
	CDahuaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDahuaDlg();

	DEV_DH_PARAMS m_devParams[16];

// 对话框数据
	enum { IDD = IDD_DAHUA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// 启动拉流函数
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);


	int ParsePTZ(char * pBuf,int nLen);
};
