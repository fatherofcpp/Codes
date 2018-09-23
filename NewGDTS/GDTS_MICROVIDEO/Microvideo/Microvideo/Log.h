/************************************************************************ 
* 文件名：    log.h 
* 文件描述：  用于记录日志的类CLog 
* 版本号：    2.0 
************************************************************************/ 
#ifndef _LOG_H
#define _LOG_H

class CLog
{
public:
	CLog();
	~CLog();

public:
	void	InitLog(LPCTSTR lpszLogPath);		
	void	Add(const char* fmt, ...);		//输出文字，参数就跟printf一样
	
protected:
	enum {BUFSIZE = 3000};  //工作缓冲区
	char		m_tBuf[BUFSIZE];
	
	CString	 m_strLogPath;
	CRITICAL_SECTION  m_crit;  //设置一个临界区
};

#endif