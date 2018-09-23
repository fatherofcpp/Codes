#pragma once

#include "stdafx.h"
#include "H264Stream.h"
#include "h3cLib.h"
#include "netvideoctrl1.h"
#include "common.h"

// CNTGJDlg �Ի���

class CNTGJDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CNTGJDlg)

public:
	CNTGJDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNTGJDlg();

//	CH264Stream *m_h264[16];
	CNetvideoctrl1 m_netVideo[16];

	DEV_SHSL_PARAMS  m_devShslParams;
///	DEV_H3C_PARAMS m_h3cParams[16];
	CONVERT_DEV_TYPE m_type;

// �Ի�������
	enum { IDD = IDD_NTGJ_DIALOG };
protected:
	//int SourceCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	//��ȡ�����ļ���Ϣ
	int GetInforFromIni(void* iniPath);
	//������������ļ���ȫ�ģ�����øú�����ȫ.
	int GetInforFromIni4Company(void * pThisDir,int nIndex);

	int SetDefaultParams(void * pParams);
	//����BMS�·���������Ϣ
	int ParseRequest(char * pXmlBuf,CString *str);
	//������������
	int SetParams(void *pStream,void * pParams);
	// ������������
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

	CString CheckExit(CString str);

	CString ChangeChannel(CString sId,int CH,CString str);


	//encode
	int InitEncode(void *pParams);
	int EncodeFrame(void *pIn,void *pOut);
	int CloseEncode(void *pParams);
};
