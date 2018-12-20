#include "StdAfx.h"
#include "BsMainTask.h"

#include "Socket/CmTransUdp.h"
#include "FrameAnalyze.h"
#include "FrameAnalyzeDlg.h"
#include "CmOpenCvDll.h"
#include "Log/CmLog.h"
#include "CmFunction/CmFunction.h"
#include<math.h>

std::string gVersion = "2017-12-4";

BsMainTask::BsMainTask(void)
{
	USES_CONVERSION;

	int ret = 0;
	//log init
	CString current_path = theApp.GetCurrentPath();
	current_path += "\\FrameAnalyze.log";
	gLog.InitLog(W2A(current_path), "FrameAnalyze");
	gLog.Add("%s ################Program Start################\n", __FUNCTION__);

	//version
#ifdef _DEBUG
	gLog.Add("%s %s Debug\n", __FUNCTION__, gVersion.c_str());
#else
	gLog.Add("%s %s Release\n", __FUNCTION__, gVersion.c_str());
#endif

	//cpu num
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	DWORD processors_num = system_info.dwNumberOfProcessors;
	int max_mask = (int)(0x01 << processors_num) - 1;
	gLog.Add("%s processors_num=%d\n", __FUNCTION__, processors_num);

	//net init
	ret = NSP_STP_CM::SocketInit();
	if(ret != 0)
	{
		gLog.Add("%s SocketInit\n", __FUNCTION__);
		exit(0);
	}

	//config
	ret = LoadConfig();
	if (ret != 0)
	{
		gLog.Add("%s LoadConfig,ret=%d\n", __FUNCTION__, ret);
		exit(0);
	}
}


BsMainTask::~BsMainTask(void)
{
}

unsigned int WINAPI BsMainTask::thread_recv_geo( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	BsMainTask* p_this = (BsMainTask*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;
	int ret = 0;


	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}


	}

	return 0;
}

unsigned int WINAPI BsMainTask::thread_recv_handle_geo( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	BsMainTask* p_this = (BsMainTask*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;

	int ret = 0;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		if (strlen(p_this->m_handleBuf) > 0)
		{
			char file_path[E_CAMERA_COUNT][260];
			int geo_count = sscanf(p_this->m_handleBuf, "{JH}&%260[^&]&%260[^&]&%260[^&]", file_path[E_CAMERA_LEFT], file_path[E_CAMERA_RIGHT], file_path[E_CAMERA_UP]);
			memset(p_this->m_handleBuf, 0, sizeof(p_this->m_handleBuf));
		}
	}

	return 0;
}

unsigned int WINAPI BsMainTask::thread_send_geo( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	BsMainTask* p_this = (BsMainTask*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 50;


	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}



	}

	return 0;
}

int BsMainTask::TaskStart()
{
	return 0;
	SetThreadCount(3);
	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_recv_geo", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_recv_geo, p_st_thread_param, 0xF);
	}
	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_recv_handle_geo", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_recv_handle_geo, p_st_thread_param, 0xF);
	}
	{
		int thread_ind = 2;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_send_geo", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_send_geo, p_st_thread_param, 0xF);
	}
	return 0;
}

int BsMainTask::LoadConfig()
{
	USES_CONVERSION;

	int ret = -1;

	const int SIZE = 1024;
	TCHAR Buffer[SIZE];
	memset(Buffer, 0, sizeof(Buffer));

	CString current_path = theApp.GetCurrentPath();
	CString strIniFile(current_path + _T("\\configFrameAnalyze.ini"));

	CString strSection;

	strSection.Format(_T("system"));

	std::string test;
	GetPrivateProfileString(strSection, _T("test"), _T("1"), Buffer, SIZE, strIniFile);
	test = W2A(Buffer);
	if (test.empty())
	{
		goto LEAVE;
	}


	int err_threshold = -99999;
	int top = GetPrivateProfileInt(strSection, _T("threTop"), 1, strIniFile);
	if (top == err_threshold)
	{
		goto LEAVE;
	}


	ret = 0;
LEAVE:
	return ret;
}

void BsMainTask::TaskStop()
{
	ThreadsStop();
}
