#pragma once
#include "Thread/CmThreadPool.h"

#include <string>
#include <deque>

namespace NSP_STP_CM
{
	struct tag_st_time;
}

class CTimeStamp
	: public NSP_STP_CM::CThreadPool
{
public:
	static CTimeStamp* GetInstance();
	static void RealseInstance();
public:
	int TaskStart();
	unsigned long long GetTimeStampMs(){return m_timeStampMs;}
	void parseTime(NSP_STP_CM::tag_st_time* p_st_time, unsigned long long time_ms);
	std::string parseTime(unsigned long long time_ms);
private:
	CTimeStamp(void);
	~CTimeStamp(void);
private:
	static unsigned int WINAPI thread_time_updata(void* pParam);
	static unsigned int WINAPI thread_time_store(void* pParam);
private:
	void formatTime(const NSP_STP_CM::tag_st_time* p_st_time);
private:
	static CTimeStamp* m_pCTimeStamp;
	static CCriticalSection m_Lock;

	unsigned long long m_timeStampMs;

	std::deque<unsigned long long> m_contTimeMs;
	CCriticalSection m_timeMsLock;
};

