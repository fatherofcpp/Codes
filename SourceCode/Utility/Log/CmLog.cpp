#include "stdafx.h"

#include "CmLog.h"
#include "CmFunction/CmDefStruct.h"
#include "CmFunction/CmFunction.h"

#define log_print TRACE

NSP_STP_CM::CLog gLog;
static bool s_is_log = true;
static bool s_is_print = true;


const int BUFSIZE = (1024 * 64);

NSP_STP_CM::CLog::CLog()
{
	m_tBuf = new char[BUFSIZE];
	memset(m_tBuf, 0, BUFSIZE);
}


NSP_STP_CM::CLog::~CLog()
{
	delete[] m_tBuf;
}

void NSP_STP_CM::CLog::InitLog(char* lpszLogPath, char* line_head)
{
	m_strLogPath = lpszLogPath;
	m_lineHead = line_head;
}

void NSP_STP_CM::CLog::Add(const char* fmt, ...)
{
	if (!s_is_log)
	{
		return;
	}

	if (m_strLogPath.empty())
		return ;

	m_logLock.Lock();
	try
	{
		va_list argptr;          
		va_start(argptr, fmt);
		vsprintf(m_tBuf, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}

	FILE *fp = fopen(m_strLogPath.c_str(), "a"); 
	if (fp)
	{
		fprintf(fp,"%s:  ", m_lineHead.c_str());  //加入当前程序名

		tag_st_time cur_time;
		GetNowTime(&cur_time);


		fprintf(fp,"%4d-%02d-%02d %02d:%02d:%02d:%03d ",
			cur_time.year,
			cur_time.mon,
			cur_time.day,
			cur_time.hour,
			cur_time.min,
			cur_time.sec,
			cur_time.ms);
		fprintf(fp, "%s", m_tBuf);

		if(s_is_print)
		{
			log_print("[%s %4d-%02d-%02d %02d:%02d:%02d:%03d]:%s",
				m_lineHead.c_str(),
				cur_time.year,
				cur_time.mon,
				cur_time.day,
				cur_time.hour,
				cur_time.min,
				cur_time.sec,
				cur_time.ms,
				m_tBuf);
		}

		fseek(fp,0L,SEEK_END);
		int filelen = ftell(fp);
		fclose(fp);
		if(filelen > 4*1024*1024)
		{
			std::string tmpLog;
			tmpLog = m_strLogPath + ".log";
			remove(tmpLog.c_str());
			rename(m_strLogPath.c_str(),tmpLog.c_str());
		}

	}
	m_logLock.Unlock();
}



