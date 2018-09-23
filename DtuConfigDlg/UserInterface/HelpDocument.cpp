// HelpDocument.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "HelpDocument.h"
#include "CommonFunction.h"
#include "afxdialogex.h"


// CHelpDocument �Ի���

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


// CHelpDocument ��Ϣ�������

void CHelpDocument::GetDocument()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CFileDialog fDlg(TRUE, _TEXT("txt"), NULL, 4|2,  
	//_TEXT("�ı��ļ�(*.txt,*.ini,*.log)|*.txt;*.ini;*.log|ȫ���ļ�(*.*)|*.*||"));  
	CString fpath = gCurrentDir + "\\˵���ĵ�.txt";//fDlg.GetPathName();

	// ���� DoModal() �������򿪶Ի���  
	if (1)//(fDlg.DoModal() == IDOK)  
	{  
		CFile fFile(fpath,CFile::modeRead);  
		CStringA strFile ;  
		fFile.Read(strFile.GetBufferSetLength((int)fFile.GetLength()),(int)fFile.GetLength());  
		// �ر��ļ����  
		fFile.Close();  
		// �ͷŻ�������  
		strFile.ReleaseBuffer();  

		// �򻷾�ΪUnicdoe��ԭ��CStringA������ֱ����ʾ������ؼ���  
		// ����ʹ��CStringת��ΪUnicdoe�ַ�  
		editDocument.SetWindowText(CString(strFile));
	}  
}

BOOL CHelpDocument::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_RETURN: //�س�  
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CHelpDocument::InitDlg()
{
	GetDocument();
}
