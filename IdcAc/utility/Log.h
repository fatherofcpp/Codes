/************************************************************************
* 文件名：    log.h
* 文件描述：  用于记录日志的类CLog
* 版本号：    2.0
************************************************************************/
#ifndef _LOG_H
#define _LOG_H

#include <include_all_qt.h>

class CLog
{
public:
    CLog();
    ~CLog();

public:
    void	InitLog(char* lpszLogPath);
    void	Add(const char* fmt, ...);		//输出文字，参数就跟printf一样

protected:
    enum {BUFSIZE = (1024 * 33)};  //工作缓冲区
    char*		m_tBuf;

    string	 m_strLogPath;
    pthread_mutex_t log_lock;
};
extern CLog gLog;
extern bool isLog;
extern bool isPrint;
#endif
