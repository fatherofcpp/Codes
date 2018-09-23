#pragma once

#include "common.h"
#include "h3cLib.h"

// CH3CIMOSDlg �Ի���

class CH3CIMOSDlg : public CDialog,public CCommon
{
	DECLARE_DYNAMIC(CH3CIMOSDlg)

public:
	CH3CIMOSDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CH3CIMOSDlg();

	DEV_H3C_PARAMS m_h3cParams[16];

// �Ի�������
	enum { IDD = IDD_H3CIMOS_DIALOG };

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
