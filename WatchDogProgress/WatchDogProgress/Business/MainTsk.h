#pragma once
#include "Thread/CmThreadPool.h"
#include "string"
#define APP_MAX_NUM 32
typedef struct tagAppInf
{
	tagAppInf()
		: AppParam("")
		, AppPath("")
	{

	}
	std::string AppParam;
	std::string AppPath;
}AppInf;

typedef struct tagConfigInf
{
	tagConfigInf()
		: AppMax(0)
	{

	}
	int AppMax;
	AppInf AppInfArr[APP_MAX_NUM];
}ConfigInf;

class CMainTsk: public NSP_STP_CM::CThreadPool
{
private:
	CMainTsk(void);
	~CMainTsk(void);
	static CMainTsk* s_pCMainTsk;
	static CCriticalSection s_lock;
public:
	static CMainTsk* GetInstance();
	static void ReleaseInstance();
public:
	int TskStart();
	int TskStop();
private:
	static unsigned int WINAPI thread_watch_dog(void* pParam);
	virtual int ThreadStateCheck(int thread_ind);
	int LoadConfig();
private:
	ConfigInf m_ConfigInf;
};

