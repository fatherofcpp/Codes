// col_service.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Common.h"


int _tmain(int argc, _TCHAR* argv[])
{

	InitLog();

	//load config
/*
[system]
app_name = test.exe
service_name = COL Service
app_path = ./test.exe
	*/

	int rtn = 0;
	char config_name[256];
	sprintf(config_name, "%s\\config.ini", m_DirPath);

	char param[256];
	ZeroMemory(param,sizeof(param));
	GetPrivateProfileString("system", "app_name", "", param, 256, config_name);
	if(0 != strlen(param))
	{
		strcpy(gAppName, param);
	}
	else
	{
		LogDebugFile("[%s] load app_name fail, exit",__FUNCTION__);
		return 0;
	}

	ZeroMemory(param,sizeof(param));
	GetPrivateProfileString("system", "service_name", "", param, 256, config_name);
	if(0 != strlen(param))
	{
		strcpy(gServiceName, param);
	}
	else
	{
		LogDebugFile("[%s] load service_name fail, exit",__FUNCTION__);
		return 0;
	}

	ZeroMemory(param,sizeof(param));
	GetPrivateProfileString("system", "app_path", "", param, 256, config_name);
	if(0 != strlen(param))
	{
		strcpy(m_AppPath, param);
	}
	else
	{
		LogDebugFile("[%s] load app_path fail, exit",__FUNCTION__);
		return 0;
	}

    // Create the service object
    CMyService MyService(gServiceName);
	LogDebugFile("[%s]enter main",__FUNCTION__);
    
    // Parse for standard arguments (install, uninstall, version etc.)
    if (!MyService.ParseStandardArgs(argc, argv)) {

        // Didn't find any standard args so start the service
        // Uncomment the DebugBreak line below to enter the debugger
        // when the service is started.
        //DebugBreak();
		printf("please add the param '-install', '-uninstall', '-start', or '-stop'\n");

		LogDebugFile("[%s]before StartService",__FUNCTION__);
        MyService.StartService();
		LogDebugFile("[%s]after StartService",__FUNCTION__);
    }

    // When we get here, the service has been stopped
    return MyService.m_Status.dwWin32ExitCode;
}

