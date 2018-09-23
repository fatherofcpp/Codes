// HelpDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "HelpDocument.h"
#include "CommonFunction.h"
#include "afxdialogex.h"


// CHelpDocument 对话框

IMPLEMENT_DYNAMIC(CHelpDocument, CDialog)

CHelpDocument::CHelpDocument(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDocument::IDD, pParent)
{

}

CHelpDocument::~CHelpDocument()
{
}

void CHelpDocument::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DOCUMENT, editDocument);
}


BEGIN_MESSAGE_MAP(CHelpDocument, CDialog)
END_MESSAGE_MAP()


// CHelpDocument 消息处理程序

void CHelpDocument::GetDocument()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
	//_TEXT("文本文件(*.txt,*.ini,*.log)|*.txt;*.ini;*.log|全部文件(*.*)|*.*||"));  
	CString fpath = gCurrentDir + "\\说明文档.txt";//fDlg.GetPathName();

	// 调用 DoModal() 方法来打开对话框  
	if (1)//(fDlg.DoModal() == IDOK)  
	{  
		CFile fFile(fpath,CFile::modeRead);  
		CStringA strFile ;  
		fFile.Read(strFile.GetBufferSetLength((int)fFile.GetLength()),(int)fFile.GetLength());  
		// 关闭文件句柄  
		fFile.Close();  
		// 释放缓冲区域  
		strFile.ReleaseBuffer();  

		// 因环境为Unicdoe的原因，CStringA不可以直接显示到界面控件上  
		// 所以使用CString转换为Unicdoe字符  
		editDocument.SetWindowText(CString(strFile));
	}  
}

BOOL CHelpDocument::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_RETURN: //回车  
			return TRUE;  
		case VK_ESCAPE: //ESC  
			return TRUE;  
		}  
	} 
	if(pMsg->message == WM_RBUTTONDOWN) 
	{  
		return TRUE;
	} 
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CHelpDocument::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CHelpDocument::InitDlg()
{
	GetDocument();
}
