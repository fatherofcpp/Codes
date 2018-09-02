
#pragma once
#include <string>
namespace NSP_STP_CM
{
	class CLog
	{
	public:
		CLog();
		~CLog();

	public:
		void InitLog(char* lpszLogPath, char* line_head);
		void Add(const char* fmt, ...);	

	private:
		char* m_tBuf;
		std::string	m_strLogPath;
		std::string m_lineHead;
		CCriticalSection m_logLock;
	};
}
extern NSP_STP_CM::CLog gLog;

