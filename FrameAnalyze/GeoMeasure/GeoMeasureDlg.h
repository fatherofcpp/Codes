
// GeoMeasureDlg.h : ͷ�ļ�
//

#pragma once

#include <string>

// CGeoMeasureDlg �Ի���
class CGeoMeasureDlg : public CDialogEx
{
// ����
public:
	CGeoMeasureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GEOMEASURE_DIALOG };

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
	void AddDg(const char* msg);
	void AddLc(const char* msg);
	void AddStruct(const char* msg);
private:
	void InitUi();
	void Data2Ui();
	CString BootOpenFile(bool open = TRUE);//TRUE��ʾ��,FALSE��ʾ����
private:
	std::string m_dg;
	std::string m_lc;
	std::string m_struct;
public:
	bool m_isContinueShow;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonStructM();
	afx_msg void OnBnClickedButtonMeasure();
	afx_msg void OnBnClickedButtonShow();
	afx_msg void OnBnClickedButtonFrameDeal();
};
