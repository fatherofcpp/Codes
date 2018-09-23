#include "StdAfx.h"
#include "BsDllFrameAnalyze.h"
#include "PantographMonitor.h"
#include "BsDefStruct.h"

CFrameAnalyzeDll* g_pCFrameAnalyzeDll;

CFrameAnalyzeDll::CFrameAnalyzeDll(void)
{
	m_imgdetect_init = NULL;
	m_imgdetect_clear = NULL;
	m_imgdetect_deal = NULL;

	m_width = 0;
	m_height = 0;
}


CFrameAnalyzeDll::~CFrameAnalyzeDll(void)
{
	//step 3
	m_imgdetect_clear();
}

int CFrameAnalyzeDll::LoadDll()
{
	USES_CONVERSION; 

	CString current_path = theApp.GetCurrentPath();
	CString current_dll_path = current_path + "\\FrameDll\\"; 
	SetDllDirectory(current_dll_path);

	HMODULE h_Module = LoadLibrary(A2W("ImgDetectDLL.dll"));
	if (!h_Module)
	{
		return -1;
	}
	m_imgdetect_init = (IMGDETECT_INIT)GetProcAddress(h_Module, "ImgDetect_Init");
	if (!m_imgdetect_init)
	{
		return -1;
	}
	m_imgdetect_clear = (IMGDETECT_CLEAR)GetProcAddress(h_Module, "ImgDetect_Clear");
	if (!m_imgdetect_clear)
	{
		return -1;
	}
	m_imgdetect_deal = (IMGDETECT_DEAL)GetProcAddress(h_Module, "ImgDetect_Deal");
	if (!m_imgdetect_deal)
	{
		return -1;
	}
	
	//step 1
	m_imgdetect_init(m_width, m_height);

	return 0;
}
