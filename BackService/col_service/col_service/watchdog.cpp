// myservice.cpp

#include "stdafx.h"
#include "Common.h"

CWatchDog::CWatchDog()
{

}

CWatchDog::~CWatchDog()
{
}

void CWatchDog::Init()
{

	m_cnt = (NORMAL_CHECK_DELAY_S - FIRST_CHECK_DELAY_S);


	m_is_first = true;
}


bool CWatchDog::GetProcIDByExeName(DWORD &ProcessID, char* ExeName)
{
	PROCESSENTRY32 ProcEntry;
	HANDLE hSnapShot;
	bool ret = false;
	bool is_first = true;
	BOOL hRes = false;

	ProcEntry.dwSize =sizeof(PROCESSENTRY32);

	hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	if(hSnapShot)
	{
		while(1)
		{
			if(is_first)
			{
				hRes = Process32First(hSnapShot, &ProcEntry);
				is_first = false;
			}
			else
			{
				hRes = Process32Next(hSnapShot, &ProcEntry);
			}

			if(hRes == false)
			{
				break;
			}

			if(!strcmp(ExeName, ProcEntry.szExeFile))
			{
				ProcessID = ProcEntry.th32ProcessID;
				ret = true;
				break;
			}
		}

		CloseHandle(hSnapShot);
	}

	return ret;
}

bool CWatchDog::ProcIsExist(void)
{
	bool ret = false;

	if (GetProcIDByExeName(m_ProcID, gAppName))
	{
		ret = true;
	}

	return ret;
}

bool CWatchDog::ProcIsOK(void)
{
	bool ret = false;
	bool need_end;
	HANDLE hProc;
	BOOL hRes;
	DWORD ExitCode;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	SYSTEMTIME SystemTime;
	CTime TmpTime, CurTime;

	hProc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, m_ProcID);
	if(hProc)
	{	
		need_end = true;

		hRes = GetProcessTimes(hProc, &CreationTime, &ExitTime, &KernelTime, &UserTime);
		if (hRes)
		{
			if(FileTimeToSystemTime(&CreationTime, &SystemTime))
			{
				TmpTime = CTime(SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
				CurTime = CTime::GetCurrentTime();

				if(CurTime - TmpTime < RESTART_DELAY_S)
				{
					need_end = false;
					ret = true;
				}
				else
				{
					LogDebugFile("[%s]Proc run too long time!\n",__FUNCTION__);
				}
			}
		}

		if (need_end)
		{
			LogDebugFile("[%s]TerminateProcess\n",__FUNCTION__);

			hRes = TerminateProcess(hProc, 0);
			if (hRes)
			{
				//等待退出
				while(GetExitCodeProcess(hProc, &ExitCode))
				{
					if(ExitCode == STILL_ACTIVE)
					{
						Sleep(1000);
					}
					else
					{
						break;
					}
				}
			}
		}

		CloseHandle(hProc);
		hProc = NULL;
	}

	return ret;
}

void CWatchDog::StartProc(void)//根据进程的全路径来启动进程
{
    DWORD PID;
    HANDLE hAccessToken; //用来接受进程的token

	GetProcIDByExeName(PID, "explorer.exe");//获取当前桌面的PID
    //OpenProcess通过进程id来获取进程handle，OpenProcessToken通过进程handle来获取进程token
    OpenProcessToken(OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE,PID),TOKEN_ALL_ACCESS,&hAccessToken);
 
    STARTUPINFO  si;  //状态信息
    PROCESS_INFORMATION  pi;  //进程信息
    ZeroMemory(&si,  sizeof(STARTUPINFO));  //用0来充值si的内存区
    si.cb=  sizeof(STARTUPINFO);  
    si.lpDesktop  =  TEXT("winsta0\\default");  
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

    //CreateProcessAsUser通过进程的token来创建一个新的进程，使新的进程和上一个进程有相同点拥有者。即用服务直接创建进程时，拥有者是服务进程界面是无法在桌面上看到的，
    //因此用此函数创建拥有者为用户的进程才能在桌面上显示创建的进程
    BOOL  bResult  =  CreateProcessAsUser(hAccessToken,m_AppPath,NULL,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
    if (!bResult)
    {
		LogDebugFile("[%s]StartProc fail\n",__FUNCTION__);
    }
	else
	{
		LogDebugFile("[%s]StartProc suc\n",__FUNCTION__);
	}

    CloseHandle(hAccessToken);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void CWatchDog::Task(void)
{
	if (++m_cnt >= NORMAL_CHECK_DELAY_S)
	{
		m_cnt = 0;

		if (!ProcIsExist())
		{
			LogDebugFile("[%s]Proc Is not Exist\n",__FUNCTION__);

			StartProc();
		}
		else
		{
			if (!ProcIsOK())
			{
				LogDebugFile("[%s]Proc Is not Ok\n",__FUNCTION__);

				StartProc();
			}
			else
			{
				if (m_is_first)
				{
					m_is_first = false;

					LogDebugFile("[%s]Proc Is Ok\n",__FUNCTION__);
				}
			}
		}
	}
}


