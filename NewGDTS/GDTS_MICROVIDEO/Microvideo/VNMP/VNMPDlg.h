// VNMPDlg.h : ͷ�ļ�
//

#pragma once

//#include "MyFunction.h"

#include <evhttp.h>

#include "xml/include/xmlnode.h"
#include "xml/include/xmlparse.h"
#include "inc.h"

#include "command.h"
#include "CriticalSection.h"
#include "afxwin.h"

#include "vnmpLib.h"
#include "MyFunction.h"

// CVNMPDlg �Ի���
class CVNMPDlg : public CDialog
{
// ����
public:
	CVNMPDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VNMP_DIALOG };

	HANDLE m_wowzalock;
	HANDLE m_Lock;

	CArray<MyAvFrame*,MyAvFrame*> p_YuvArray;
	CCriticalSection m_streamlock;
	DEV_VNMP_PARAMS m_devParams;


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
//	int ClosePull(void *pStream,void * pParams);
};
