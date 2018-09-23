// DaHuaDlg.h : 头文件
//

#pragma once
#include <evhttp.h>

#include "xml/include/xmlnode.h"
#include "xml/include/xmlparse.h"
#include "inc.h"

#include "command.h"
#include "CriticalSection.h"
#include "afxwin.h"

#include "MyFunction.h"




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

// CDaHuaDlg 对话框
class CDaHuaDlg : public CDialog
{
// 构造
public:
	CDaHuaDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DAHUA_DIALOG };

	DEV_DH_PARAMS m_devParams;

	HANDLE m_wowzalock;
	HANDLE m_Lock;

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
		// 启动拉流函数
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
};
