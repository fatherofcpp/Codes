#pragma once

//#include "cudevocxctrl1.h"
#include "common.h"

#include "H264Stream.h"
#include "SoucrcFilter.h"

#if 0
#include "afxwin.h"
#import "Svl_COMObject.tlb"

typedef Svl_COMObject::Svl_InterfacePtr SvlInterfacePtr;
typedef Svl_COMObject::Svl_Class SvlClass;
#endif
// CH3cVMDlg �Ի���

typedef struct tagDEV_H3CVM{
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
}DEV_H3CVM_PARAMS;

#pragma once


// CH3cVMDlg �Ի���

class CH3cVMDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CH3cVMDlg)

public:
	CH3cVMDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CH3cVMDlg();

	CSoucrcFilter m_SourceFilter;

	DEV_H3CVM_PARAMS  m_h3cVMParams[16];//
// �Ի�������
	enum { IDD = IDD_H3CVM_DIALOG };

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
