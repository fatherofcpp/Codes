
// WatchDogProgress.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "WatchDogProgress.h"
#include "WatchDogProgressDlg.h"
#include "MainTsk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWatchDogProgressApp

BEGIN_MESSAGE_MAP(CWatchDogProgressApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWatchDogProgressApp ����

CWatchDogProgressApp::CWatchDogProgressApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CWatchDogProgressApp ����

CWatchDogProgressApp theApp;


// CWatchDogProgressApp ��ʼ��

BOOL CWatchDogProgressApp::InitInstance()
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

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//
	int ret = CMainTsk::GetInstance()->TskStart();
	if (ret != 0)
	{
		CMainTsk::ReleaseInstance();
		return FALSE;
	}
	//

	CWatchDogProgressDlg dlg;
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

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	CMainTsk::GetInstance()->ThreadsStop();
	CMainTsk::ReleaseInstance();

	return FALSE;
}

CString CWatchDogProgressApp::GetCurrentPath()
{
	CString strCurrentPath(_T(""));
	if (strCurrentPath.IsEmpty())
	{
		WCHAR buffer[MAX_PATH];
		memset(buffer, 0, sizeof(buffer));
		::GetModuleFileName(NULL, buffer, MAX_PATH);
		CString strPath(buffer);
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		strCurrentPath = strPath;
	}
	return strCurrentPath;
}

