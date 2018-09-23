#include "StdAfx.h"
#include "BsTimeStamp.h"

#include "BsDefStruct.h"
#include "CmFunction.h"
#include "CmDefStruct.h"
#include "BsGlobal.h"
#include "Log/CmLog.h"
#include "BsFunction.h"

CTimeStamp* CTimeStamp::m_pCTimeStamp = NULL;
CCriticalSection CTimeStamp::m_Lock;

CTimeStamp::CTimeStamp(void)
{
	m_timeStampMs = 0;
}

CTimeStamp::~CTimeStamp(void)
{

}

int CTimeStamp::TaskStart()
{
	SetThreadCount(2);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_time_updata", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_time_updata, p_st_thread_param, CpuMaskRemain());
	}

	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_time_store", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_time_store, p_st_thread_param, CpuMaskRemain());
	}

	return 0;
}

unsigned int WINAPI CTimeStamp::thread_time_updata( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CTimeStamp* p_this = (CTimeStamp*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		NSP_STP_CM::tag_st_time nw_time;
		NSP_STP_CM::GetNowTime(&nw_time);
		p_this->formatTime(&nw_time);

	}

	return 0;
}

CTimeStamp* CTimeStamp::GetInstance()
{
	m_Lock.Lock();
	if (!m_pCTimeStamp)
	{
		m_pCTimeStamp = new CTimeStamp;
	}
	m_Lock.Unlock();
	return m_pCTimeStamp;
}

void CTimeStamp::RealseInstance()
{
	m_Lock.Lock();
	if (m_pCTimeStamp)
	{
		delete m_pCTimeStamp;
		m_pCTimeStamp = NULL;
	}
	m_Lock.Unlock();
}

void CTimeStamp::formatTime( const NSP_STP_CM::tag_st_time* p_st_time )
{
	const long long c_ten2 = 100;
	const long long c_ten3 = 1000;
	m_timeStampMs 
		= p_st_time->year * c_ten3 * c_ten2 * c_ten2 * c_ten2 * c_ten2 * c_ten2
		+ p_st_time->mon  * c_ten3 * c_ten2 * c_ten2 * c_ten2 * c_ten2
		+ p_st_time->day * c_ten3 * c_ten2 * c_ten2 * c_ten2
		+ p_st_time->hour * c_ten3 * c_ten2 * c_ten2
		+ p_st_time->min * c_ten3 * c_ten2
		+ p_st_time->sec * c_ten3
		+ p_st_time->ms
		;

	m_timeMsLock.Lock();
	m_contTimeMs.push_back(m_timeStampMs);
	//gLog.Add("%s m_timeStampMs=%lld\n", __FUNCTION__, m_timeStampMs);
	m_timeMsLock.Unlock();
}

void CTimeStamp::parseTime( NSP_STP_CM::tag_st_time* p_st_time, unsigned long long time_ms )
{
	const long long c_ten2 = 100;
	const long long c_ten3 = 1000;

	p_st_time->year = (int)(time_ms / c_ten2 / c_ten2 / c_ten2 / c_ten2 / c_ten2 / c_ten3);
	p_st_time->mon = (int)(time_ms / c_ten2 / c_ten2 / c_ten2 / c_ten2 / c_ten3 % c_ten2);
	p_st_time->day = (int)(time_ms / c_ten2 / c_ten2 / c_ten2 / c_ten3 % c_ten2);
	p_st_time->hour = (int)(time_ms / c_ten2 / c_ten2 / c_ten3 % c_ten2);
	p_st_time->min = (int)(time_ms / c_ten2 / c_ten3 % c_ten2);
	p_st_time->sec = (int)(time_ms / c_ten3  % c_ten2);
	p_st_time->ms = (int)(time_ms % c_ten3);
}

std::string CTimeStamp::parseTime( unsigned long long time_ms )
{
	NSP_STP_CM::tag_st_time cur_time;
	parseTime(&cur_time, GetTimeStampMs());

	MS_CHAR time_inf[256];
	sprintf(time_inf, "%04d-%02d-%02d %02d:%02d:%02d", cur_time.year, cur_time.mon, cur_time.day, cur_time.hour, cur_time.min, cur_time.sec);

	M_STRING now_time = time_inf;
	return now_time;
}

unsigned int WINAPI CTimeStamp::thread_time_store( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CTimeStamp* p_this = (CTimeStamp*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 5 * 1000;
	const unsigned int c_max_num = 1024;
	unsigned long long time_ms[c_max_num];

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		p_this->m_timeMsLock.Lock();
		unsigned int size = p_this->m_contTimeMs.size();
		if (size > c_max_num)
		{
			size = c_max_num;
		}
		for (unsigned int i = 0; i < size; i ++)
		{
			time_ms[i] = p_this->m_contTimeMs[i];
		}
		if (size > 0)
		{
			p_this->m_contTimeMs.clear();
		}
		p_this->m_timeMsLock.Unlock();


		for (unsigned int i = 0; i < size; i ++)
		{
			long long tmms = time_ms[i];
			NSP_STP_CM::tag_st_time cur_time;
			p_this->parseTime(&cur_time, tmms);
			char file_path[MAX_PATH];
			sprintf(file_path, "D:\\pantoMonitorPath\\timeStampPath\\%04d%02d%02d\\%02d%02d\\%02d%02d%02d.dav"
				, cur_time.year
				, cur_time.mon
				, cur_time.day
				, cur_time.hour
				, cur_time.min / 10 * 10
				, cur_time.hour
				, cur_time.min
				, cur_time.sec
				);

			{
				M_STRING folder_path = NSP_STP_CM::str_right_cut_tag(file_path, "\\");
				NSP_STP_CM::folder_create(folder_path);

				FILE* pf = NULL;
				MS_INT32 rtn = 0;
				pf = fopen(file_path, "ab");
				if(pf)
				{
					M_SIZE_T w_rt = fwrite(&tmms, sizeof(char), sizeof(tmms), pf);
					gLog.Add("%s file_path=%s\n", __FUNCTION__, file_path);
					fclose(pf);		
				}
			}
		}			

	}

	return 0;
}
