// MicrovideoDlg.h : ͷ�ļ�
//

#pragma once

#include "MyFunction.h"

#include <evhttp.h>

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#include "inc.h"
#include "command.h"
#include "CriticalSection.h"
#include "afxwin.h"


typedef struct tagDEV_MICROVIDEO{//�Ͼ��ж�ʹ�ò����ṹ
 
	CString SrcUrl;
	CString ServerIP;
	int nPort;
	CString PubName;
	int StreamNumber;
	
}DEV_MICROVIDEO_PARAMS;

// CMicrovideoDlg �Ի���
class CMicrovideoDlg : public CDialog
{
// ����
public:
	CMicrovideoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMicrovideoDlg(void);

// �Ի�������
	enum { IDD = IDD_MICROVIDEO_DIALOG };

	HANDLE m_wowzalock;
	HANDLE m_Lock;

	DEV_MICROVIDEO_PARAMS m_MicrovideoParams;
	STREAMPARAMS StreamParams;
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
	int GetInforFromIni4Company(void * pThisDir,int nIndex);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);
	int SetParams(void *pStream,void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);
	CString ChangeChannel(CString sId,int CH,CString str);


};
