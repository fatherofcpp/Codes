#include <stdafx.h>
#include <register.h>

static LPCSTR gSubKey = "Software\\CollectorTool\\COMNUM";
static HKEY gRootKey = HKEY_LOCAL_MACHINE;
static char gName[32] = "COM";

int SetComToReg(const char* com_name)
{
	HKEY hKey;// ��ָ���Ӽ�
	DWORD dwDisposition = 0;//REG_CREATED_NEW_KEY;REG_OPENED_EXISTING_KEY
	// ��������ڲ�����
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
		);    //��ע���

	if(lRet == ERROR_SUCCESS)//�������ɹ�
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(
			hKey,
			gName,
			NULL,
			NULL,
			(LPBYTE)tchData,
			&dwSize
			);    //����򿪳ɹ������
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