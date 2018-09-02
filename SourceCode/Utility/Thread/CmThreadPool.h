#pragma once
#include "Cmthread.h"
namespace NSP_STP_CM
{
	typedef struct tag_st_thread_param
	{
		void* pParam;
		int threadInd;
		int pVoid;
		char threadName[256];
	}st_thread_param;

	//线程开启
	void thread_run(CThread* p_CThread, THREAD_RUN thread_run, void * param, const int mask);

	class CThreadPool
	{
	public:
		CThreadPool(void);
		~CThreadPool(void);
	public:
		//设置线程个数
		void SetThreadCount(int thread_count);
		//线程集结束
		void ThreadsStop();
		//线程集暂停
		void SetThreadPause(bool is_pause);
		bool GetThreadPause(){return m_threadPause;}
	protected:
		CThread* m_pCThreadArr;
		int m_threadCount;
		bool m_isFirstNew;
		bool m_threadPause;
	};
}

