#include "StdAfx.h"
#include "Push.h"

CPush::CPush(void)
{
}

CPush::~CPush(void)
{
}

int CPush::Init(void)
{
	/*HMODULE m_hProcessModule = LoadLibrary("encoder.dll");
	if (NULL == m_hProcessModule)
	{
		//	m_list.AddString("¼ÓÔØ·â×°Ä£¿éÊ§°Ü");
		return -1;
	}
	m_encoderstreamProcessOpen = (ENCODERSTREAMPROCESSOPEN)GetProcAddress(m_hProcessModule,"encoderstreamProcessOpen");
	if (NULL == m_encoderstreamProcessOpen)
	{
		//	m_list.AddString("·â×°Ä£¿é´íÎó1");
		return -2;
	}

	m_encoderstreamProcessStart = (ENCODERSTREAMPROCESSSTART)GetProcAddress(m_hProcessModule,"encoderstreamProcessStart");
	if (NULL == m_encoderstreamProcessStart)
	{
		//	m_list.AddString("·â×°Ä£¿é´íÎó2");
		return -3;;
	}

	m_encoderstreamProcessClose = (ENCODERSTREAMPROCESSCLOSE)GetProcAddress(m_hProcessModule,"encoderstreamProcessClose");
	if (NULL == m_encoderstreamProcessClose)
	{
		//	m_list.AddString("·â×°Ä£¿é´íÎó3");
		return -4;
	}

	m_encoderstreamProcessPutDataAddress = (ENCODERSTREAMPROCESSPUTDATA)GetProcAddress(m_hProcessModule,"encoderstreamProcessPutDataAddress");
	if (NULL == m_encoderstreamProcessPutDataAddress)
	{
		//	m_list.AddString("·â×°Ä£¿é´íÎó4");
		return -5;
	}

	m_encoderstreamProcessPutDataAddress_ch = (ENCODERSTREAMPROCESSPUTDATA_CH)GetProcAddress(m_hProcessModule,"encoderstreamProcessPutDataAddress_chzhh3c");
	if (NULL == m_encoderstreamProcessPutDataAddress_ch)
	{
		//	m_list.AddString("·â×°Ä£¿é´íÎó5");
		return -6;
	}


	HMODULE hProcessModule	=  LoadLibraryA("multiprocess.dll");
	ProcessOpen=(STREAMPROCESSOPEN)GetProcAddress(hProcessModule,"streamProcessOpen");
	ProcessStart=(STREAMPROCESSSTART)GetProcAddress(hProcessModule,"streamProcessStart");
	ProcessClose=(STREAMPROCESSCLOSE)GetProcAddress(hProcessModule,"streamProcessClose");*/
	return 0;
}
