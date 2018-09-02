#pragma once
namespace NSP_STP_CM
{
	typedef enum tag_e_thread_state
	{
		E_THREAD_STATE_RUNNING = 0,
		E_THREAD_STATE_PAUSED,
		E_THREAD_STATE_STOPED
	}e_thread_state;

	typedef enum tag_e_cpu_mask
	{
		E_CPU_MASK_1 = 0xF,//picture
		E_CPU_MASK_2 = 0xF,//video
		E_CPU_MASK_3 = 0xF,//mvb
		E_CPU_MASK_4 = 0xF,
		E_CPU_MASK_5 = 0xF,
		E_CPU_MASK_6 = 0xF,
		E_CPU_MASK_7 = 0xF,
		E_CPU_MASK_8 = 0xF,
	}e_cpu_mask;

	typedef unsigned int (WINAPI* THREAD_RUN)(void * param);

	class CThread
	{
	public:
		CThread(void);
		~CThread(void);
		bool ThreadStart(THREAD_RUN thread_run, void * param, const int mask);
		void ThreadStop();
	public:
		bool GetThreadStatus()const{return m_bStart;}
		void SetThreadAffinityMask(int mask);

		void SetPriority( int aPriority );
		int GetPriority() const;

		UINT getTID(){return m_ID;}
		HANDLE GetThread(){return m_Thread;}

		char* GetThreadName(){return m_threadName;}
		void SetThreadName(const char* thread_name){strcpy(m_threadName, thread_name);}
	protected:
		HANDLE m_Thread;
		UINT m_ID;
		bool m_bStart;
		char m_threadName[256];
	};
}
