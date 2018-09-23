
// FrameCollectorDlg.h : 头文件
//

#pragma once

#include "MyCanvas.h"
#include "resource.h"

// CFrameCollectorDlg 对话框
class CFrameCollectorDlg : public CDialogEx
{
// 构造
public:
	CFrameCollectorDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CFrameCollectorDlg();

// 对话框数据
	enum { IDD = IDD_FRAMECOLLECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	//图像TCP数据
	static void __stdcall receiveImageData_Tcp(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//特殊数据
	static void __stdcall receiveImageData_Udp(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);

	static unsigned int __stdcall Camera_Stop(LPVOID lParam);

	static unsigned int __stdcall Camera_Close(LPVOID lParam);

protected:
	CMyCanvas m_Image;
public:
	//显示 由相机采集完成后 通知 图层刷新显示 ——> 拷贝第二次 
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
