
// KjControlDlg.h : ͷ�ļ�
//

#pragma once
#include "Work.h"
#include "afxwin.h"
#include "CEditBoxInfoShow.h"
#include <memory>
using namespace std;
// CKjControlDlg �Ի���
class CKjControlDlg : public CDialogEx
{
// ����
public:
	CKjControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KJCONTROL_DIALOG };

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
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCheckKsPlsOc();
	afx_msg void OnBnClickedCheckKsGbOc();
	afx_msg void OnBnClickedCheckJsPlsOc();
	afx_msg void OnBnClickedCheckJsGbOc();
	afx_msg void OnBnClickedCheckJs1Act();
	afx_msg void OnBnClickedCheckJs2Act();
	afx_msg void OnBnClickedCheckDpjPlsOc();
	afx_msg void OnBnClickedButtonCarPlsClr();
	afx_msg void OnBnClickedButtonPlcPlsCnt();
	afx_msg void OnBnClickedButtonDpjPlsCnt();
	afx_msg void OnBnClickedButtonGetCarPos();
	afx_msg void OnBnClickedButtonCarStep();
	afx_msg void OnBnClickedButtonCarHome();
	afx_msg void OnDestroy();

	CEdit m_Edit;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CWork m_work;	

	static void EditBoxMsgShow(LPVOID);
	HANDLE m_hHandleEditBox;
	//CEditBoxInfoShow* m_editbox;
	shared_ptr<CEditBoxInfoShow> m_editbox;
	void InitCtrl();
public:
	afx_msg void OnBnClickedButtonDemo();

	static void DemoAction(LPVOID param);
};
