// DaHuaDlg.h : ͷ�ļ�
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

// CDaHuaDlg �Ի���
class CDaHuaDlg : public CDialog
{
// ����
public:
	CDaHuaDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DAHUA_DIALOG };

	DEV_DH_PARAMS m_devParams;

	HANDLE m_wowzalock;
	HANDLE m_Lock;

	CArray<MyAvFrame*,MyAvFrame*> p_YuvArray;
	CCriticalSection m_streamlock;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��



// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
		// ������������
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
};
