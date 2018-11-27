#pragma once
extern char m_DirPath[_MAX_PATH];
static char m_LogPath[_MAX_PATH];
extern char m_AppPath[_MAX_PATH];
extern char gAppName[256];//后台运行程序名称
extern char gServiceName[256];//后台服务名称
void InitLog();

void LogDebugFile(char* fmt, ...);