#pragma once

#include "common.h"

typedef struct tagDEV_SANTACHI_INFO{
 char		m_dstIp[32];		//Ŀ�������IP
 char		m_dstPub[64];		//������
 LONG		m_dstStream;	//���±�
 LONG		m_userId;//UserID
 LONG		m_realPlayHandle;//RealPlayHandle
 HANDLE		m_hMsgheartBeatThread;
}DEV_SANTACHI_INFO;

typedef struct tagDEV_SANTACHI{//������ʹ�ò����ṹ
 char		m_proxyIp[64];		//������IP
 LONG		m_proxyPort;		//�������˿�
 char		m_userName[32];		//�û���
 char		m_passWord[32];		//����
 LONG		m_deviceChn;	//�豸ͨ��
 LONG		m_playChn;		//����ͨ��

 DEV_SANTACHI_INFO m_devInfo;


}DEV_SANTACHI_PARAMS;

// CSantachiDlg �Ի���

class CSantachiDlg : public CDialog , public CCommon
{
	DECLARE_DYNAMIC(CSantachiDlg)

public:
	CSantachiDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSantachiDlg();

	DEV_SANTACHI_PARAMS m_devParams[16];


// �Ի�������
	enum { IDD = IDD_SANTACHI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int GetInforFromIni(void* iniPath);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// ������������
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);
};
