#pragma once
#include "include/mty.h"
int GetFileLen(const char *pfile);
int GetFileData(const char *pfile, int len,char* dst);

int WriteCVS(CString data,CString file_path);

BOOL GetSystemSerialComport(CArray<CString,CString>&com_array);  //获取系统串口列表


