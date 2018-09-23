#include "Log.h"
#define log_print qDebug

CLog gLog;
bool isLog = true;
bool isPrint = true;

CLog::CLog()  //构造函数，设置日志文件的默认路径
{
    int ret = pthread_mutex_init(&log_lock,NULL);
    if (ret != 0)
    {
        printf("[CLog]: mutex_init log_lock failed\n");
        exit(0);
    }
    m_tBuf = new char[BUFSIZE];
    memset(m_tBuf, 0, BUFSIZE);
}


CLog::~CLog()
{
    pthread_mutex_destroy(&log_lock);
    delete[] m_tBuf;
}


/*================================================================
 * 函数名：    InitLog
 * 参数：      LPCTST lpszLogPath
 * 功能描述:   初始化日志(设置日志文件的路径)
 * 返回值：    void
 ================================================================*/
void CLog::InitLog(char* lpszLogPath)
{
    m_strLogPath = lpszLogPath;
}

void CLog::Add(const char* fmt, ...)
{
    if (!isLog)
    {
        return;
    }

    if (m_strLogPath.empty())
        return ;

    /*-----------------------进入临界区(写文件)------------------------------------*/
    pthread_mutex_lock(&log_lock);
    try
    {
        va_list argptr;          //分析字符串的格式
        va_start(argptr, fmt);
        vsprintf(m_tBuf, fmt, argptr);
        va_end(argptr);
    }
    catch (...)
    {
        m_tBuf[0] = 0;
    }

    FILE *fp = fopen(m_strLogPath.c_str(), "a"); //以添加的方式输出到文件
    if (fp)
    {
        fprintf(fp,"%s:  ", "idc_ui");  //加入当前程序名

        struct tm *t;
        time_t tt;
        time(&tt);
        t=localtime(&tt);

        fprintf(fp,"%4d-%02d-%02d %02d:%02d:%02d ",
                t->tm_year+1900,
                t->tm_mon+1,
                t->tm_mday,
                t->tm_hour,
                t->tm_min,
                t->tm_sec);
        fprintf(fp, "%s\n", m_tBuf);

        if(isPrint)
        {
            log_print("\n[idc_ui %4d-%02d-%02d %02d:%02d:%02d]:%s\n",
                      t->tm_year+1900,
                      t->tm_mon+1,
                      t->tm_mday,
                      t->tm_hour,
                      t->tm_min,
                      t->tm_sec,
                      m_tBuf);
        }

        fseek(fp,0L,SEEK_END);
        int filelen = ftell(fp);
        fclose(fp);
        if(filelen > 4*1024*1024)
        {
            string tmpLog;
            tmpLog = m_strLogPath + ".log";
            remove(tmpLog.c_str());
            rename(m_strLogPath.c_str(),tmpLog.c_str());//需要先删除原文件
        }

    }
    pthread_mutex_unlock(&log_lock);
}



