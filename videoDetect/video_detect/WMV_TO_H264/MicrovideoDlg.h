#pragma once

#include "stdafx.h"
#include "H264Stream.h"
#include "microvideo.h"

#include "common.h"
// CMicrovideoDlg �Ի���

class CMicrovideoDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CMicrovideoDlg)

public:
	CMicrovideoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMicrovideoDlg();

// �Ի�������
	enum { IDD = IDD_MICROVIDEO_DIALOG };

	int SourceCount;//0-32 index of source when getting information from config each time 
	int SourceInd;//0-1000 index of source in config when being get information


	CH264Stream *m_h264[MAX_ONLINE];//way to pull video stream
	DEV_MICROVIDEO_PARAMS m_MicrovideoParams;//information of microvideo in locReport
	DEV_MICROVIDEO_PARAMS m_MicrovideoParamsTsk;//information of microvideo in netReport
	STREAMPARAMS StreamParams[MAX_ONLINE];//params of stream

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	int GetInforFromIni(void* iniPath);//get information from config
	int SetDefaultParams(void * pParams);//set default params 

	int SetParams(void *pStream,void * pParams);//set params
	// ������������
	int StartPull(void *pStream,void* pParams);//pull video stream
	int ClosePull(void *pStream,void * pParams);//close video stream





};
