#include "StdAfx.h"
#include "MainTsk.h"
#include "WatchDogProgress.h"
#include "Log/CmLog.h"
#include "CmFunction.h"

CMainTsk* CMainTsk::s_pCMainTsk = NULL;
CCriticalSection CMainTsk::s_lock;

CMainTsk::CMainTsk(void)
{
	USES_CONVERSION;
	//log init
	CString current_path = theApp.GetCurrentPath();
	current_path += "\\WatchDogProgress.log";
	gLog.InitLog(W2A(current_path), "WatchDogProgress");
	gLog.Add("%s ################Program Start################\n", __FUNCTION__);

	//config
	int ret = LoadConfig();
	if (ret != 0)
	{
		gLog.Add("%s LoadConfig,ret=%d\n", __FUNCTION__, ret);
		exit(0);
	}

	//net init
	ret = NSP_STP_CM::SocketInit();
	if(ret != 0)
	{
		gLog.Add("%s SocketInit\n", __FUNCTION__);
		exit(0);
	}
}


CMainTsk::~CMainTsk(void)
{
}

int CMainTsk::TskStart()
{
	SetThreadCount(m_ConfigInf.AppMax);
	for (int app_ind = 0; app_ind < m_ConfigInf.AppMax; app_ind ++)
	{
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = app_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_watch_dog path=%s", __FUNCTION__, m_ConfigInf.AppInfArr[app_ind].AppPath.c_str());
		thread_run(&m_pCThreadArr[app_ind], thread_watch_dog, p_st_thread_param, 0xFF);
	}
	return 0;
}

int CMainTsk::TskStop()
{
	ThreadsStop();
	return 0;
}

CMainTsk* CMainTsk::GetInstance()
{
	s_lock.Lock();
	if (!s_pCMainTsk)
	{
		s_pCMainTsk = new CMainTsk;
	}
	s_lock.Unlock();
	return s_pCMainTsk;
}

void CMainTsk::ReleaseInstance()
{
	s_lock.Lock();
	if (s_pCMainTsk)
	{
		delete s_pCMainTsk;
		s_pCMainTsk = NULL;
	}
	s_lock.Unlock();
}

unsigned int WINAPI CMainTsk::thread_watch_dog( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CMainTsk* p_this = (CMainTsk*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int app_ind = thread_ind;
	const int sleep_m = 1000;
	const AppInf* p_AppInf = &p_this->m_ConfigInf.AppInfArr[app_ind];
	gLog.Add("%s path=%s,param=%s\n"
		, __FUNCTION__
		, p_AppInf->AppPath.c_str()
		, p_AppInf->AppParam.c_str()
		);

	STARTUPINFO si; 
	PROCESS_INFORMATION pi; //进程信息：
	memset(&si, 0, sizeof(si)); 
	memset(&pi, 0, sizeof(pi)); 
	si.cb = sizeof(si); 
	bool sub_progress_run = false;

	while(1)
	{
		Sleep(sleep_m);

		int check_res = p_this->ThreadStateCheck(thread_ind);
		if (check_res == NSP_STP_CM::E_THREAD_STATE_STOPED)
		{
			break;
		}
		else if (check_res == NSP_STP_CM::E_THREAD_STATE_PAUSED)
		{
			continue;
		}

		if (sub_progress_run)
		{
			int ret = WaitForSingleObject( pi.hProcess, sleep_m);
			if (ret == WAIT_OBJECT_0)
			{
				//子进程退出
				sub_progress_run = false;
			}
			else
			{
				sub_progress_run = true;
			}
		}
		else
		{ 
			// 创建子进程，判断是否执行成功
			char cmd_line[1024];
			memset(cmd_line, 0, sizeof(cmd_line));
			sprintf(cmd_line, "%s %s"
				, p_AppInf->AppPath.c_str()
				, p_AppInf->AppParam.c_str()
				);

			if(!CreateProcessA( NULL, cmd_line, NULL, NULL, FALSE, 0, NULL, NULL, (LPSTARTUPINFOA)&si, &pi)) 
			{ 
				//进程创建失败
				sub_progress_run = false;
			} 
			else
			{
				//进程创建成功
				sub_progress_run = true;
			}

			gLog.Add("%s CreateProcess suc_create_progress=%d, path=%s,param=%s,dwProcessID=%d,dwThreadID=%d\n"
				, __FUNCTION__
				, sub_progress_run
				, p_AppInf->AppPath.c_str()
				, p_AppInf->AppParam.c_str()
				, pi.dwProcessId
				, pi.dwThreadId
				);
		}
	}

	return 0;
}

int CMainTsk::ThreadStateCheck( int thread_ind )
{
	bool is_thread_run = m_pCThreadArr[thread_ind].GetThreadStatus();
	if (!is_thread_run)
	{
		return NSP_STP_CM::E_THREAD_STATE_STOPED;
	}
	bool is_thread_pause = GetThreadPause();
	if (is_thread_pause)
	{
		return NSP_STP_CM::E_THREAD_STATE_PAUSED;
	}
	return NSP_STP_CM::E_THREAD_STATE_RUNNING;
}

int CMainTsk::LoadConfig()
{
	USES_CONVERSION;

	int ret = -1;

	const int SIZE = 1024;
	TCHAR Buffer[SIZE];
	memset(Buffer, 0, sizeof(Buffer));

	CString current_path = theApp.GetCurrentPath();
	CString strIniFile(current_path + _T("\\WatchDogProgressConfig.ini"));

	CString strSection;

	strSection.Format(_T("system"));
	m_ConfigInf.AppMax = GetPrivateProfileInt(strSection, _T("AppMax"), 0, strIniFile);
	if (m_ConfigInf.AppMax < 0)
	{
		goto LEAVE;
	}

	for (int app_ind = 0; app_ind < m_ConfigInf.AppMax; app_ind ++)
	{
		strSection.Format(_T("App%d"), app_ind);

		GetPrivateProfileString(strSection, _T("AppPath"), _T(""), Buffer, SIZE, strIniFile);
		m_ConfigInf.AppInfArr[app_ind].AppPath = W2A(Buffer);

		GetPrivateProfileString(strSection, _T("AppParam"), _T(""), Buffer, SIZE, strIniFile);
		m_ConfigInf.AppInfArr[app_ind].AppParam = W2A(Buffer);
	}

	ret = 0;
LEAVE:
	return ret;
}
