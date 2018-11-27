// myservice.cpp

#include "stdafx.h"
#include "Common.h"

CMyService::CMyService(const char* szServiceName)
:CNTService(szServiceName)
{
	m_iStartParam = 0;
	m_iIncParam = 1;
	m_iState = m_iStartParam;
}

BOOL CMyService::OnInit()
{
	LogDebugFile("[%s]Enter CMyService::OnInit",__FUNCTION__);
	// Read the registry parameters
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\Parameters
    HKEY hkey;
	char szKey[1024];
	strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szKey, m_szServiceName);
	strcat(szKey, "\\Parameters");
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     szKey,
                     0,
                     KEY_QUERY_VALUE,
                     &hkey) == ERROR_SUCCESS) {
        // Yes we are installed
        DWORD dwType = 0;
        DWORD dwSize = sizeof(m_iStartParam);
        RegQueryValueEx(hkey,
                        "Start",
                        NULL,
                        &dwType,
                        (BYTE*)&m_iStartParam,
                        &dwSize);
        dwSize = sizeof(m_iIncParam);
        RegQueryValueEx(hkey,
                        "Inc",
                        NULL,
                        &dwType,
                        (BYTE*)&m_iIncParam,
                        &dwSize);
        RegCloseKey(hkey);
    }

	// Set the initial state
	m_iState = m_iStartParam;

	return TRUE;
}

void CMyService::Run()
{
	m_watchdog.Init();
	LogDebugFile("[%s]enter CMyService::Run",__FUNCTION__);

    while (m_bIsRunning) {
		m_watchdog.Task();

		// Sleep for a while
        //m_watchdog.LogDebugFile("My service is sleeping (%lu)...", m_iState);
        Sleep(1000);

		// Update the current state
		m_iState += m_iIncParam;
    }
}

// Process user control requests
BOOL CMyService::OnUserControl(DWORD dwOpcode)
{
	LogDebugFile("[%s]enter",__FUNCTION__);

    switch (dwOpcode) {
    case SERVICE_CONTROL_USER + 0:

        // Save the current status in the registry
        SaveStatus();
        return TRUE;

    default:
        break;
    }
    return FALSE; // say not handled
}

// Save the current status in the registry
void CMyService::SaveStatus()
{
    LogDebugFile("[%s]Saving current status",__FUNCTION__);
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\...
    HKEY hkey = NULL;
	char szKey[1024];
	strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szKey, m_szServiceName);
	strcat(szKey, "\\Status");
    DWORD dwDisp;
	DWORD dwErr;
    LogDebugFile("[%s]Creating key: %s",__FUNCTION__, szKey);
    dwErr = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
                           	szKey,
                   			0,
                   			"",
                   			REG_OPTION_NON_VOLATILE,
                   			KEY_WRITE,
                   			NULL,
                   			&hkey,
                   			&dwDisp);
	if (dwErr != ERROR_SUCCESS) {
		DebugMsg("Failed to create Status key (%lu)", dwErr);
		return;
	}	

    // Set the registry values
	LogDebugFile("[%s]Saving 'Current' as %ld", __FUNCTION__,m_iState); 
    RegSetValueEx(hkey,
                  "Current",
                  0,
                  REG_DWORD,
                  (BYTE*)&m_iState,
                  sizeof(m_iState));


    // Finished with key
    RegCloseKey(hkey);

}
