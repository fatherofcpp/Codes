#include "stdafx.h"

#include "Common.h"
char m_AppPath[_MAX_PATH];
char m_DirPath[_MAX_PATH];
char gAppName[256];
char gServiceName[256];
void InitLog()
{
	char *p;

	::GetModuleFileName(NULL, m_DirPath, sizeof(m_DirPath));
	p = strrchr(m_DirPath, '\\');
	*p = 0;

	sprintf(m_LogPath, "%s\\service.log", m_DirPath);

	LogDebugFile("[%s]InitLog\n",__FUNCTION__);
}

void LogDebugFile(char* fmt, ...)
{
	SYSTEMTIME time;
	va_list ap;
	char p[256];
	FILE* hfile;

	va_start(ap, fmt);
	vsprintf(p, fmt, ap);
	va_end(ap);

	GetLocalTime(&time); 

	hfile = fopen(m_LogPath, "a+");
	fprintf(hfile, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ",
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	fprintf(hfile, "%s\n", p);

	//add
	fseek(hfile,0L,SEEK_END);
	int filelen = ftell(hfile);
	//add
	fclose(hfile);
	//add
	if(filelen>1*1024*1024)
	{
		char tmpLog[128];
		sprintf(tmpLog, "%s.log", m_LogPath);
		remove(tmpLog);
		rename(m_LogPath, tmpLog);//需要先删除原文件
	}
	//add
}