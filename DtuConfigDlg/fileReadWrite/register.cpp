#include <stdafx.h>
#include <register.h>

static LPCSTR gSubKey = "Software\\CollectorTool\\COMNUM";
static HKEY gRootKey = HKEY_LOCAL_MACHINE;
static char gName[32] = "COM";

int SetComToReg(const char* com_name)
{
	HKEY hKey;// 打开指定子键
	DWORD dwDisposition = 0;//REG_CREATED_NEW_KEY;REG_OPENED_EXISTING_KEY
	// 如果不存在不创建
	LONG lRet =  RegCreateKeyEx(
		gRootKey,
		gSubKey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
		NULL,
		&hKey,
		&dwDisposition
		);

	if(lRet != ERROR_SUCCESS)
	{
		return -1;
	}

	lRet = RegSetValueEx(
		hKey,
		gName,
		0,
		REG_SZ,
		(BYTE*)com_name,
		strlen(com_name));

	if(lRet == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}
	else
	{
		RegCloseKey(hKey);
		return -2;
	}
	return 0;
}

int GetComFrmReg(char* com_name)
{
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	memset(tchData,0,sizeof(tchData));
	
	lRet = RegOpenKeyEx(
		gRootKey,
		gSubKey,
		0,
		KEY_ALL_ACCESS,
		&hKey
		);    //打开注册表

	if(lRet == ERROR_SUCCESS)//读操作成功
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(
			hKey,
			gName,
			NULL,
			NULL,
			(LPBYTE)tchData,
			&dwSize
			);    //如果打开成功，则读
		if(lRet == ERROR_SUCCESS)
		{
			strcpy(com_name,tchData);
			RegCloseKey(hKey);
			return 0;
		}
		else
		{
			RegCloseKey(hKey);
			return -1;
		}
	}
	else
	{
		return -2;
	}
}