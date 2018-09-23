#pragma once
#include "afxwin.h"


// CHistoryData �Ի���

class CHistoryData : public CDialog
{
	DECLARE_DYNAMIC(CHistoryData)

public:
	CHistoryData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistoryData();

// �Ի�������
	enum { IDD = IDD_DIALOG_HISTORYDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit mHistoryData;
private:
	char* historyData;
	int dataLen;
public:
	void SetData(char* data, int data_len);
};
