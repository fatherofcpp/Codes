
// TestDlg.h : ͷ�ļ�
//

#pragma once

//
#include "CmFunction/CmDefType.h"
#include "xml/xml.hpp"
#include "MString/CmMString.h"
#include "DataBase/CmDBSqlite.h"
#include "Exception/MasException.h"
#include "CmFunction/CmFunction.h"
//
using namespace NSP_STP_CM;

// CTestDlg �Ի���
class CTestDlg : public CDialogEx
{
// ����
public:
	CTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

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
	int test(MCS_CHAR* pBuff);
	static void CALLBACK thread_exception(void* pParam);
	 
};
