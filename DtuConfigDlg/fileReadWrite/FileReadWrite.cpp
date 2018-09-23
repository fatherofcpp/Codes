#include "stdafx.h"
#include "FileReadWrite.h"
#include "dataConvert.h"

int GetFileLen( const char *pfile )
{
	FILE *pf=NULL;
	int len = 0;
	int rtn = 0;

	pf = fopen(pfile,"rb");
	if(pf)
	{
		fseek(pf,0,SEEK_END);
		len = ftell(pf);	
		fseek(pf,0,SEEK_SET);

		fclose(pf);	
		return len;
	}
	else
	{
		return -1;
	}	
}

int GetFileData( const char *pfile, int len ,char* dst)
{
	FILE *pf=NULL;
	int rtn = 0;

	pf = fopen(pfile,"rb");
	if(pf)
	{
		fread(dst,1,len,pf);
		fclose(pf);	
		return 0;
	}
	else
	{
		return -1;
	}	
}

int WriteCVS(CString data,CString file_path)
{
	BOOL bret = FALSE;
	CStdioFile   clsFile;
	data.Append("\n");

	encry_local_data((unsigned char*)data.GetString(),data.GetLength());

	bret = clsFile.Open(file_path,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	if (!bret)
	{
		TRACE("[%s]Open ß∞‹!",__FUNCTION__);
		return -1;
	}
	clsFile.SeekToEnd();

	clsFile.WriteString(data);
	clsFile.Close(); 
	return 0;
}

BOOL GetSystemSerialComport( CArray<CString,CString>&com_array )
{
	HKEY hKey=NULL;  

	com_array.RemoveAll();  

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),0,KEY_READ,&hKey)!=ERROR_SUCCESS)  
	{  
		return FALSE;  
	}  

	CString valuename,databuffer;  
	DWORD valuenamebufferlength=200,valuetype,databuddersize=200;  

	int i=0;  
	while(RegEnumValue(hKey,i++,valuename.GetBuffer(200),&valuenamebufferlength,NULL,&valuetype,(BYTE*)databuffer.GetBuffer(200),&databuddersize) != ERROR_NO_MORE_ITEMS)  
	{  
		com_array.Add(CString(databuffer));  

		databuddersize=200;  
		valuenamebufferlength=200;  
	}  

	RegCloseKey(hKey);  

	return TRUE;  
}
