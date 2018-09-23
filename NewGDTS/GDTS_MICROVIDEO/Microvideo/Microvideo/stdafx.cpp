// stdafx.cpp : 只包括标准包含文件的源文件
// Microvideo.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
 CLog m_Log;

CString g_CurrentDir;
int g_maxLiveNum=16;

int g_port=10000;
int g_devId=0;

#if 0
char g_ip[64]="192.168.0.58";
#else
char g_ip[64]="192.168.200.64";
#endif

//char g_ip[64]="127.0.0.1";

