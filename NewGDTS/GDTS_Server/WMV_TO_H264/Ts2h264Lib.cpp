

#include "stdafx.h"

#include "Ts2h264Lib.h"
#if 1
 INITTSFUNC InitTsFunc=NULL;
 PROCESSTSDATA ProcessTSData=NULL;
 PARSEPES ParsePES=NULL;
int loadTSLib()
{	
	HMODULE hProcessModule=NULL;
	//SetDllDirectory(g_CurrentDir+"\\vms\\lib\\");
	SetDllDirectory(g_CurrentDir);
	hProcessModule	=  LoadLibraryA("ts2h264.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload ts2h264.dll failed\n");
		return -1;
	}
	InitTsFunc=(INITTSFUNC)GetProcAddress(hProcessModule,"InitTsFunc");
	if(InitTsFunc == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress InitTsFunc failed[%d]\n",GetLastError());
		return -1;
	}
	ProcessTSData=(PROCESSTSDATA)GetProcAddress(hProcessModule,"ProcessTSData");
	if(ProcessTSData == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress ProcessTSData failed[%d]\n",GetLastError());
		return -1;
	}
	ParsePES=(PARSEPES)GetProcAddress(hProcessModule,"ParsePES");
	if(ParsePES == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress ParsePES failed[%d]\n",GetLastError());
		return -1;
	}
	return 0;
}
#endif
