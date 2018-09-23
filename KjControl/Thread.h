#pragma once
#include <Windows.h>
#include <string>

using namespace std;
#include <process.h>


class CThread
{
public:

	static unsigned int __stdcall  CallBack(void* pth)
	{
		CThread *ppth = (CThread*)pth;
		while(ppth->m_running && !ppth->m_release)
		{
			Sleep(1000);
			TRACE("wait for run!, ID = %d \r\n", ppth->m_nID);
		}
		if (ppth->m_running)
		{
			ppth->Run();
		}
		ppth->SetRunning(false);
		if (ppth->DeleteOnExit() && !ppth->IsDestructor())
			delete ppth;
		_endthreadex(0);
		return NULL;
	}
	virtual int Run() = 0;

	bool IsRunning()
	{
		return this->m_running;
	}

	void SetRunning(bool x)
	{
		this->m_running = x;
	}

	bool IsReleased()
	{
		return this->m_release;
	}

	void SetRelease(bool x)
	{
		this->m_release = x;
	}

	bool DeleteOnExit()
	{
		return this->m_b_delete_on_exit;
	}

	void SetDeleteOnExit(bool x = true)
	{
		this->m_b_delete_on_exit = x;
	}

	bool IsDestructor()
	{
		return this->m_b_destructor;
	}

	int GetThreadID() {return m_nID;}
	CThread(bool release = true)
		:m_hHandle(0)
	,m_running(true)
	,m_release(false)
	,m_b_delete_on_exit(false)
	,m_b_destructor(false)
	{
		m_hHandle = (HANDLE)_beginthreadex(NULL,0,&CallBack,this,0,(unsigned int*)&m_nID);
		m_release = release;
	}

	virtual ~CThread(void)
	{
		m_b_destructor = true;
		while (this->m_running)
			Sleep(10);
		if (this->m_running)
		{
			this->SetRelease(true);
			this->SetRunning(false);
			Sleep(1000);
			if (this->m_hHandle)
				CloseHandle(this->m_hHandle);
		}
	}
private:
	CThread(const CThread& ) {}
	CThread& operator=(const CThread& ) { return *this; }
protected:
	HANDLE m_hHandle;
	int m_nID;
	bool m_release;
	bool m_running;
	bool m_b_destructor;
    
protected:
    bool m_b_delete_on_exit;

};


