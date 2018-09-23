#include "StdAfx.h"
#include "BsDllMvb.h"
#include "Mvb/ServerTrans.h"
#include "PantographMonitor.h"

CMvbDll* g_pCMvbDll;

CMvbDll::CMvbDll(void)
{
	m_pCServerTrans = NULL;
}


CMvbDll::~CMvbDll(void)
{
	if (m_pCServerTrans)
	{
		releaseService(&m_pCServerTrans, MVB::E_TRANS_WAY_RS485);
	}
}

int CMvbDll::LoadDll()
{
	USES_CONVERSION; 
	
	CString current_path = theApp.GetCurrentPath();
	CString current_dll_path = current_path + "\\Mvb\\"; 
	SetDllDirectory(current_dll_path);

	HMODULE h_Module = LoadLibrary(A2W("Mvb.dll"));
	if (!h_Module)
	{
		return -1;
	}
	createService = (CREATESERVICE_MVB)GetProcAddress(h_Module, "createService");
	if (!createService)
	{
		return -1;
	}
	releaseService = (RELEASESERVICE_MVB)GetProcAddress(h_Module, "releaseService");
	if (!releaseService)
	{
		return -1;
	}

	m_pCServerTrans = createService(MVB::E_TRANS_WAY_RS485);
	if (!m_pCServerTrans)
	{
		return -1;
	}

	return 0;
}
