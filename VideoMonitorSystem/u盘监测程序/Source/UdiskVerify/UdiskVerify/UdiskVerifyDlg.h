
// UdiskVerifyDlg.h : ͷ�ļ�
//

#pragma once

#include <Dbt.h>
#include <WinIoCtl.h>
#include "FileReadWrite.h"

// CUdiskVerifyDlg �Ի���
class CUdiskVerifyDlg : public CDialogEx
{
// ����
public:
	CUdiskVerifyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UDISKVERIFY_DIALOG };

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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	int verifyUdisk(CString strSeries, CString decDriver);
};
