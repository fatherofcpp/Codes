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
	char m_type[64];			//����������
	char m_ver[64];			//�������汾
	char  m_proxyIp[32];		//������IP
	LONG  m_proxyPort;		//�������˿�
	char  m_userName[32];	//�û���
	char  m_passWord[32];	//����
	int  m_deviceId;		//�豸ID

	unsigned long m_playHandle;
	 
	char  m_dstIp[32];		//Ŀ�������IP
	char  m_dstPub[128];		//������
	LONG	 m_dstStream;	//���±�

	DEV_H3CIPC_WAY m_ipcWay;
}DEV_H3CIPC_PARAMS;
// Ch3cIpcDlg �Ի���

class Ch3cIpcDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(Ch3cIpcDlg)

public:
	Ch3cIpcDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Ch3cIpcDlg();


	DEV_H3CIPC_PARAMS  m_h3cIpcParams[16];//
	int FirstLog;
	HANDLE m_Locker;

// �Ի�������
	enum { IDD = IDD_H3CIPC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//��ȡ�����ļ���Ϣ
	int ParseRequest(char * pXmlBuf,CString *str);
	//int GetInforFromIni(void* iniPath);
	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	//������������ļ���ȫ�ģ�����øú�����ȫ.
	int SetDefaultParams(void * pParams);
	//������������
	int SetParams(void *pStream,void * pParams);
	// ������������
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

	CString ChangeChannel(CString sId,int CH,CString str);

	CString CheckExit(CString str);
};
