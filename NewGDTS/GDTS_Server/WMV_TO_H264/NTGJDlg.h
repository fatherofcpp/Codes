#pragma once

#include "stdafx.h"
#include "H264Stream.h"
#include "h3cLib.h"
#include "netvideoctrl1.h"
#include "common.h"

// CNTGJDlg 对话框

class CNTGJDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CNTGJDlg)

public:
	CNTGJDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNTGJDlg();

//	CH264Stream *m_h264[16];
	CNetvideoctrl1 m_netVideo[16];

	DEV_SHSL_PARAMS  m_devShslParams;
///	DEV_H3C_PARAMS m_h3cParams[16];
	CONVERT_DEV_TYPE m_type;

// 对话框数据
	enum { IDD = IDD_NTGJ_DIALOG };
protected:
	//int SourceCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	void OnVideoDataMessageNetvideoctrl1(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);

	void PushData2Buffer(void* pMain,long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl2(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl3(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl4(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl5(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl6(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl7(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl8(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl9(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl10(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl11(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl12(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl13(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl14(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl15(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);
	void OnVideoDataMessageNetvideoctrl16(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser);

public:
	afx_msg void OnBnClickedOk();
	//读取配置文件信息
	int GetInforFromIni(void* iniPath);
	//如果存在配置文件不全的，则调用该函数补全.
	int GetInforFromIni4Company(void * pThisDir,int nIndex);

	int SetDefaultParams(void * pParams);
	//解析BMS下发的命令信息
	int ParseRequest(char * pXmlBuf,CString *str);
	//设置拉流参数
	int SetParams(void *pStream,void * pParams);
	// 启动拉流函数
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

	CString CheckExit(CString str);

	CString ChangeChannel(CString sId,int CH,CString str);


	//encode
	int InitEncode(void *pParams);
	int EncodeFrame(void *pIn,void *pOut);
	int CloseEncode(void *pParams);
};
