#include "StdAfx.h"
#include "BsDllCamVideo.h"
#include "CamVideo/VideoObjectDevSer.h"
#include "PantographMonitor.h"

CCamVideoDll* g_pCCamVideoDll;

CCamVideoDll::CCamVideoDll(void)
{
	m_pCVideoObjectService = NULL;
}


CCamVideoDll::~CCamVideoDll(void)
{
	m_pCVideoObjectService->releaseService();
}

int CCamVideoDll::LoadDll()
{
	USES_CONVERSION; 

	CString current_path = theApp.GetCurrentPath();
	CString current_dll_path = current_path + "\\CamVideo\\"; 
	SetDllDirectory(current_dll_path);

	HMODULE h_Module = LoadLibrary(A2W("VideoDll.dll"));

	if (!h_Module)
	{
		return -1;
	}

	createService = (CREATESERVICE_CAMVIDEO)GetProcAddress(h_Module, "createService");;
	if (!createService)
	{
		return -1;
	}

	releaseAllService = (RELEASEALLSERVICE_CAMVIDEO)GetProcAddress(h_Module, "releaseAllService");
	if (!releaseAllService)
	{
		return -1;
	}

	releaseService = (RELEASESERVICE_CAMVIDEO)GetProcAddress(h_Module, "releaseService");
	if (!releaseService)
	{
		return -1;
	}

	m_pCVideoObjectService = createService();
	if (m_pCVideoObjectService == NULL)
	{
		return -1;
	}

	return 0;
}
