#pragma once
#include "stdafx.h"
#include "H264Stream.h"

#include "common.h"


typedef struct tagDEV_DH_INFO{
	char		m_dstIp[32];		//Ŀ�������IP
	char		m_dstPub[64];		//������
	long		m_dstStream;	//���±�
	long		m_loginid;		//��¼��ʶ
	//LLONG		realLiveHd;
	long		realPlayerHd;	//���ű�ʶ

}DEV_DH_INFO;

typedef struct tagDEV_DH{//�㽭��ʹ�ò����ṹ
	char		m_proxyIp[64];		//ת��������IP
	long		m_proxyPort;		//ת���������˿�
	char		m_userName[32];		//�û���
	char		m_passWord[32];		//����
	
	char		m_devName[64];		//�豸ͨ��
	char		m_channel[64];		//����ͨ��

	DEV_DH_INFO m_devInfo;


}DEV_DH_PARAMS;

// CDahuaDlg �Ի���

class CDahuaDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CDahuaDlg)

public:
	CDahuaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDahuaDlg();

	DEV_DH_PARAMS m_devParams[16];

// �Ի�������
	enum { IDD = IDD_DAHUA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// ������������
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);


	int ParsePTZ(char * pBuf,int nLen);
};
