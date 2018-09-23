
// FrameCollector.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "FrameCollector.h"
#include "FrameCollectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFrameCollectorApp

BEGIN_MESSAGE_MAP(CFrameCollectorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFrameCollectorApp ����

CFrameCollectorApp::CFrameCollectorApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFrameCollectorApp ����

CFrameCollectorApp theApp;


// CFrameCollectorApp ��ʼ��

BOOL CFrameCollectorApp::InitInstance()
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

	CFrameCollectorDlg dlg;
	m_pMainWnd = &dlg;

	CString str_section = _T("Camera");
	if (!getConfigureInfo(str_section))
	{
		AfxMessageBox(_T("���������ļ�ʧ�ܣ�"));
		return FALSE;
	}

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
	return FALSE;
}

const CString & CFrameCollectorApp::getCurrentPath() const
{
	static CString strCurrentPath(_T(""));
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

BOOL CFrameCollectorApp::getConfigureInfo( const CString &strSection )
{
	return m_Camera.getConfigureInfo(strSection);
}

