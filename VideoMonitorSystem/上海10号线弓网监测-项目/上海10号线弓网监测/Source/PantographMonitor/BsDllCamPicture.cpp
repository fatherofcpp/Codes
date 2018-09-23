#include "StdAfx.h"
#include "BsDllCamPicture.h"
#include "CamPicture/CameraObject.h"
#include "PantographMonitor.h"

CCamPictureDll* g_pCCamPictureDll;

CCamPictureDll::CCamPictureDll(void)
{
	m_pCCamObjectService = NULL;
}


CCamPictureDll::~CCamPictureDll(void)
{
	m_pCCamObjectService->releaseService();
}

int CCamPictureDll::LoadDll()
{
	USES_CONVERSION; 

	CString current_path = theApp.GetCurrentPath();
	CString current_dll_path = current_path + "\\CamPicture\\"; 
	SetDllDirectory(current_dll_path);

	HMODULE h_Module = LoadLibrary(A2W("Cameras.dll"));
	if (!h_Module)
	{
		return -1;
	}

	createService = (CREATESERVICE_CAMPICTURE)GetProcAddress(h_Module, "createService");
	if (!createService)
	{
		return -1;
	}

	releaseAllServices = (RELEASEALLSERVICE_CAMPICTURE)GetProcAddress(h_Module, "releaseAllServices");
	if (!releaseAllServices)
	{
		return -1;
	}

	createService(&m_pCCamObjectService);
	if (m_pCCamObjectService == NULL)
	{
		return -1;
	}

	return 0;
}
