
// FrameCollectorDlg.h : ͷ�ļ�
//

#pragma once

#include "MyCanvas.h"
#include "resource.h"

// CFrameCollectorDlg �Ի���
class CFrameCollectorDlg : public CDialogEx
{
// ����
public:
	CFrameCollectorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CFrameCollectorDlg();

// �Ի�������
	enum { IDD = IDD_FRAMECOLLECTOR_DIALOG };

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
	//ͼ��TCP����
	static void __stdcall receiveImageData_Tcp(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//��������
	static void __stdcall receiveImageData_Udp(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);

	static unsigned int __stdcall Camera_Stop(LPVOID lParam);

	static unsigned int __stdcall Camera_Close(LPVOID lParam);

protected:
	CMyCanvas m_Image;
public:
	//��ʾ ������ɼ���ɺ� ֪ͨ ͼ��ˢ����ʾ ����> �����ڶ��� 
	void ShowImage(unsigned char *pbuf, unsigned int w, unsigned int h);
private:
	bool	m_bInit;
	bool	m_bPlaying;
	bool	m_bPointRun;
	bool	m_bForwardUdp;
	unsigned char* ImageBuffer;
	bool	m_bImageReady;
public:
	LRESULT OnShowImage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
