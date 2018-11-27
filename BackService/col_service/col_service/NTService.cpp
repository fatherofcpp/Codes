// NTService.cpp
//
// Implementation of CNTService


#include "stdafx.h"
#include "Common.h"

// static variables
CNTService* CNTService::m_pThis = NULL;

CNTService::CNTService(const char* szServiceName)
{
	LogDebugFile("[%s]enter",__FUNCTION__);
    // copy the address of the current object so we can access it from
    // the static member callback functions. 
    // WARNING: This limits the application to only one CNTService object. 
    m_pThis = this;
    
    // Set the default service name and version
    strncpy(m_szServiceName, szServiceName, sizeof(m_szServiceName)-1);
    m_iMajorVersion = 1;
    m_iMinorVersion = 0;
    m_hEventSource = NULL;

    // set up the initial service status 
    m_hServiceStatus = NULL;
    m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_Status.dwCurrentState = SERVICE_STOPPED;
    m_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_Status.dwWin32ExitCode = 0;
    m_Status.dwServiceSpecificExitCode = 0;
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;
    m_bIsRunning = FALSE;
}

CNTService::~CNTService()
{
    LogDebugFile("CNTService::~CNTService()");
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// Default command line argument parsing

// Returns TRUE if it found an arg it recognised, FALSE if not
// Note: processing some arguments causes output to stdout to be generated.
BOOL CNTService::ParseStandardArgs(int argc, char* argv[])
{
	LogDebugFile("[%s]enter",__FUNCTION__);

    // See if we have any command line args we recognise
    if (argc <= 1) {
		return FALSE;
	}

    if (_stricmp(argv[1], "-install") == 0) {

        // Request to install.
        if (IsInstalled()) {
            printf("%s is already installed\n", m_szServiceName);
        } else {
            // Try and install the copy that's running
            if (Install()) {
                printf("%s installed\n", m_szServiceName);
            } else {
                printf("%s failed to install. Error %d\n", m_szServiceName, GetLastError());
            }
        }
        return TRUE; // say we processed the argument

    } else if (_stricmp(argv[1], "-uninstall") == 0) {

        // Request to uninstall.
        if (!IsInstalled()) {
            printf("%s is not installed\n", m_szServiceName);
        } else {
            // Try and remove the copy that's installed
            if (Uninstall()) {
                // Get the executable file path
                char szFilePath[_MAX_PATH];
                ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
                printf("%s removed. (You must delete the file (%s) yourself.)\n",
                       m_szServiceName, szFilePath);
            } else {
                printf("Could not remove %s. Error %d\n", m_szServiceName, GetLastError());
            }
        }
        return TRUE; // say we processed the argument
    
    }
	 else if (_stricmp(argv[1], "-start") == 0) {

        // Request to install.
        if (Start()) {
            printf("%s Started\n", m_szServiceName);
        } else {
            printf("Could not start %s\n", m_szServiceName);
        }
        return TRUE; // say we processed the argument
    }
	else if (_stricmp(argv[1], "-stop") == 0) {

        // Request to install.
        if (Stop()) {
            printf("%s Stopped\n", m_szServiceName);
        } else {
            printf("Could not stop %s\n", m_szServiceName);
        }
        return TRUE; // say we processed the argument
    }    

    // Don't recognise the args
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////
// Install/uninstall routines

// Test if the service is currently installed
BOOL CNTService::IsInstalled()
{
	LogDebugFile("[%s]enter",__FUNCTION__);

    BOOL bResult = FALSE;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (hSCM) {

        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM,
                                           m_szServiceName,
                                           SERVICE_QUERY_CONFIG);
        if (hService) {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }

        ::CloseServiceHandle(hSCM);
    }
    
    return bResult;
}

BOOL CNTService::Install()
{
	LogDebugFile("[%s]enter",__FUNCTION__);

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) 
	{
		printf("Install::OpenSCManager fail\n");
		return FALSE;
	}
    // Get the executable file path
    char szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));

    // Create the service
    SC_HANDLE hService = ::CreateService(hSCM,
                                         m_szServiceName,
                                         m_szServiceName,
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS,
                                         SERVICE_AUTO_START,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         szFilePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (!hService) {
		printf("Install::CreateService fail\n");

        ::CloseServiceHandle(hSCM);
        return FALSE;
    }

    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.
    char szKey[256];
    HKEY hKey = NULL;
    strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
    strcat(szKey, m_szServiceName);
    if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) != ERROR_SUCCESS) {
        ::CloseServiceHandle(hService);
        ::CloseServiceHandle(hSCM);
		printf("Install::RegCreateKey fail\n");

        return FALSE;
    }

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.
    ::RegSetValueEx(hKey,
                    "EventMessageFile",
                    0,
                    REG_EXPAND_SZ, 
                    (CONST BYTE*)szFilePath,
                    strlen(szFilePath) + 1);     

    // Set the supported types flags.
    DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
    ::RegSetValueEx(hKey,
                    "TypesSupported",
                    0,
                    REG_DWORD,
                    (CONST BYTE*)&dwData,
                     sizeof(DWORD));
    ::RegCloseKey(hKey);

    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_INSTALLED, m_szServiceName);

    // tidy up
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

BOOL CNTService::Uninstall()
{
	LogDebugFile("[%s]enter",__FUNCTION__);

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    BOOL bResult = FALSE;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       m_szServiceName,
                                       DELETE);
    if (hService) {
        if (::DeleteService(hService)) {
            LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_REMOVED, m_szServiceName);
            bResult = TRUE;
        } else {
            LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_NOTREMOVED, m_szServiceName);
        }
        ::CloseServiceHandle(hService);
    }
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}


//////////////////////////////////////////////////////////////////////////////////////////////

BOOL CNTService::Start()
{
	LogDebugFile("[%s]enter",__FUNCTION__);

	  // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    BOOL bResult = FALSE;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       m_szServiceName,
                                       SERVICE_ALL_ACCESS);
    if (hService) {
		QueryServiceStatus(hService, &m_Status);
        if(m_Status.dwCurrentState == SERVICE_STOPPED)
		{
			if (::StartService(hService,0,NULL)) {
				bResult = TRUE;
			}
		}
		else
		{
			printf("%s is not stopped, can not start\n", m_szServiceName);
		}

        ::CloseServiceHandle(hService);
    }
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}

///////////////////////////////////////////////////////////////////////////////////////

BOOL CNTService::Stop()
{
	LogDebugFile("[%s]enter",__FUNCTION__);

	  // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    BOOL bResult = FALSE;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       m_szServiceName,
                                       SERVICE_ALL_ACCESS);
    if (hService) 
	{
        QueryServiceStatus(hService, &m_Status);
        if(m_Status.dwCurrentState == SERVICE_RUNNING)
        {
			ControlService(hService, SERVICE_CONTROL_STOP, &m_Status);
			bResult = TRUE;
        }
		else
		{
			printf("%s is not running, can not stop\n", m_szServiceName);
		}

        ::CloseServiceHandle(hService);
    }
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions

// This function makes an entry into the application event log
void CNTService::LogEvent(WORD wType, DWORD dwID,
                          const char* pszS1,
                          const char* pszS2,
                          const char* pszS3)
{
    const char* ps[3];
    ps[0] = pszS1;
    ps[1] = pszS2;
    ps[2] = pszS3;

    int iStr = 0;
    for (int i = 0; i < 3; i++) {
        if (ps[i] != NULL) iStr++;
    }
        
    // Check the event source has been registered and if
    // not then register it now
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSource(NULL,  // local machine
                                               m_szServiceName); // source name
    }

    if (m_hEventSource) {
        ::ReportEvent(m_hEventSource,
                      wType,
                      0,
                      dwID,
                      NULL, // sid
                      iStr,
                      0,
                      ps,
                      NULL);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration

BOOL CNTService::StartService()
{
	LogDebugFile("[%s]enter",__FUNCTION__);

    SERVICE_TABLE_ENTRY st[] = {
        {m_szServiceName, ServiceMain},
        {NULL, NULL}
    };

    LogDebugFile("[%s]before StartServiceCtrlDispatcher()",__FUNCTION__);
    BOOL b = ::StartServiceCtrlDispatcher(st);
    LogDebugFile("[%s]after StartServiceCtrlDispatcher()",__FUNCTION__);

    return b;
}

// static member function (callback)
void CNTService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	LogDebugFile("[%s]enter",__FUNCTION__);

    // Get a pointer to the C++ object
    CNTService* pService = m_pThis;
    
    // Register the control request handler
    pService->m_Status.dwCurrentState = SERVICE_START_PENDING;
    pService->m_hServiceStatus = RegisterServiceCtrlHandler(pService->m_szServiceName,
                                                           Handler);
    if (pService->m_hServiceStatus == NULL) {
        pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_CTRLHANDLERNOTINSTALLED);
        return;
    }

    // Start the initialisation
    if (pService->Initialize()) {

        // Do the real work. 
        // When the Run function returns, the service has stopped.
        pService->m_bIsRunning = TRUE;
        pService->m_Status.dwWin32ExitCode = 0;
        pService->m_Status.dwCheckPoint = 0;
        pService->m_Status.dwWaitHint = 0;
        pService->Run();
    }

    // Tell the service manager we are stopped
    pService->SetStatus(SERVICE_STOPPED);

    LogDebugFile("[%s]Leaving ServiceMain()",__FUNCTION__);
}

///////////////////////////////////////////////////////////////////////////////////////////
// status functions

void CNTService::SetStatus(DWORD dwState)
{
    LogDebugFile("CNTService::SetStatus(%lu, %lu)", m_hServiceStatus, dwState);
    m_Status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_Status);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Service initialization

BOOL CNTService::Initialize()
{
    LogDebugFile("[%s]Entering Initialize()",__FUNCTION__);

    // Start the initialization
    SetStatus(SERVICE_START_PENDING);
    
    // Perform the actual initialization
    BOOL bResult = OnInit(); 
    
    // Set final state
    m_Status.dwWin32ExitCode = GetLastError();
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;
    if (!bResult) {
        LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_FAILEDINIT);
        SetStatus(SERVICE_STOPPED);
        return FALSE;    
    }
    
    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STARTED);
    SetStatus(SERVICE_RUNNING);

    LogDebugFile("[%s]Leaving Initialize()",__FUNCTION__);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// main function to do the real work of the service

// This function performs the main work of the service. 
// When this function returns the service has stopped.
void CNTService::Run()
{
    LogDebugFile("Entering CNTService::Run()");

    while (m_bIsRunning) {
        LogDebugFile("[%s]Sleeping...",__FUNCTION__);
        Sleep(5000);
    }

    // nothing more to do
    LogDebugFile("Leaving CNTService::Run()");
}

//////////////////////////////////////////////////////////////////////////////////////
// Control request handlers

// static member function (callback) to handle commands from the
// service control manager
void CNTService::Handler(DWORD dwOpcode)
{
    // Get a pointer to the object
    CNTService* pService = m_pThis;
    
    LogDebugFile("CNTService::Handler(%lu)", dwOpcode);
    switch (dwOpcode) {
    case SERVICE_CONTROL_STOP: // 1
        pService->SetStatus(SERVICE_STOP_PENDING);
        pService->OnStop();
        pService->m_bIsRunning = FALSE;
        pService->LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STOPPED);
        break;

    case SERVICE_CONTROL_PAUSE: // 2
        pService->OnPause();
        break;

    case SERVICE_CONTROL_CONTINUE: // 3
        pService->OnContinue();
        break;

    case SERVICE_CONTROL_INTERROGATE: // 4
        pService->OnInterrogate();
        break;

    case SERVICE_CONTROL_SHUTDOWN: // 5
        pService->OnShutdown();
        break;

    default:
        if (dwOpcode >= SERVICE_CONTROL_USER) {
            if (!pService->OnUserControl(dwOpcode)) {
                pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
            }
        } else {
            pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
        }
        break;
    }

    // Report current status
    LogDebugFile("Updating status (%lu, %lu)",
                       pService->m_hServiceStatus,
                       pService->m_Status.dwCurrentState);
    ::SetServiceStatus(pService->m_hServiceStatus, &pService->m_Status);
}
        
// Called when the service is first initialized
BOOL CNTService::OnInit()
{
    LogDebugFile("[%s]CNTService::OnInit()",__FUNCTION__);
	return TRUE;
}

// Called when the service control manager wants to stop the service
void CNTService::OnStop()
{
    LogDebugFile("[%s]CNTService::OnStop()",__FUNCTION__);
}

// called when the service is interrogated
void CNTService::OnInterrogate()
{
    LogDebugFile("CNTService::OnInterrogate()");
}

// called when the service is paused
void CNTService::OnPause()
{
    LogDebugFile("CNTService::OnPause()");
}

// called when the service is continued
void CNTService::OnContinue()
{
    LogDebugFile("CNTService::OnContinue()");
}

// called when the service is shut down
void CNTService::OnShutdown()
{
    LogDebugFile("CNTService::OnShutdown()");
}

// called when the service gets a user control message
BOOL CNTService::OnUserControl(DWORD dwOpcode)
{
    LogDebugFile("CNTService::OnUserControl(%8.8lXH)", dwOpcode);
    return FALSE; // say not handled
}


////////////////////////////////////////////////////////////////////////////////////////////
// Debugging support

void CNTService::DebugMsg(const char* pszFormat, ...)
{
#if 0
    char buf[1024];
    sprintf(buf, "[%s](%lu): ", m_szServiceName, GetCurrentThreadId());
	va_list arglist;
	va_start(arglist, pszFormat);
    vsprintf(&buf[strlen(buf)], pszFormat, arglist);
	va_end(arglist);
    strcat(buf, "\n");
    OutputDebugString(buf);
#else
	char buf[1024];
	sprintf(buf, "[%s](%lu): ", m_szServiceName, GetCurrentThreadId());
	va_list arglist;
	va_start(arglist, pszFormat);
	vsprintf(&buf[strlen(buf)], pszFormat, arglist);
	va_end(arglist);
	strcat(buf, "\n");
	printf(buf);
#endif
}
