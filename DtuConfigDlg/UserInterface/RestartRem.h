#pragma once


// CRestartRem �Ի���

class CRestartRem : public CDialog
{
	DECLARE_DYNAMIC(CRestartRem)

public:
	CRestartRem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRestartRem();

// �Ի�������
	enum { IDD = IDD_DIALOG_WHETHER_RESTART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	////////////////////////////////
private:
	afx_msg void OnBnClickedButtonYes();
	afx_msg void OnBnClickedButtonLater();
};
