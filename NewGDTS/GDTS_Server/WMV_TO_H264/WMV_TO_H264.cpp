// WMV_TO_H264.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"

#include "VerifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include "C:/Program Files/Visual Leak Detector/include/vld.h"

//#pragma comment(lib,"C:/Program Files/Visual Leak Detector/lib/Win32/vld.lib")

// CWMV_TO_H264App

BEGIN_MESSAGE_MAP(CWMV_TO_H264App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWMV_TO_H264App ����

CWMV_TO_H264App::CWMV_TO_H264App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CWMV_TO_H264App ����

CWMV_TO_H264App theApp;


// CWMV_TO_H264App ��ʼ��

BOOL CWMV_TO_H264App::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(330);
#if 0
	CVerifyDlg *pDlg=NULL;

	pDlg = new CVerifyDlg();
	pDlg->Create(IDD_DIALOG_VERIFY,0);
	
	int nlocate = pDlg->GetLocalinfo();
	if(nlocate != 0)
	{
		//pDlg->ShowWindow(SW_SHOW);
		//
		delete pDlg;
		CVerifyDlg vDlg;
		INT_PTR vResponse = vDlg.DoModal();
		exit(0);
		
	}

	delete pDlg;

#endif
	//INT_PTR vResponse = vDlg.DoModal();
	//if (vResponse == IDOK)
	//{
	//	// TODO: �ڴ˷��ô����ʱ��
	//	//  ��ȷ�������رնԻ���Ĵ���
	//}
	//else if (vResponse == IDCANCEL)
	//{
	//	// TODO: �ڴ˷��ô����ʱ��
	//	//  ��ȡ�������رնԻ���Ĵ���
	//}
	//

	CWMV_TO_H264Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}


	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
