#pragma once
extern char m_DirPath[_MAX_PATH];
static char m_LogPath[_MAX_PATH];
extern char m_AppPath[_MAX_PATH];
extern char gAppName[256];//��̨���г�������
extern char gServiceName[256];//��̨��������
void InitLog();

void LogDebugFile(char* fmt, ...);