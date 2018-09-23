#include "StdAfx.h"
#include "utility.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>

//using namespace std;
template <>
long parse<long>(unsigned char* data)
{
	/* 从字符串中解析出长整型数据并返回 
	*/
	long fn = 0;
	char farr[20] = {0};
	unsigned char * ptr = data;

	int i = 0;
	while(*ptr != '\0')
	{
		if((*ptr <= '9' && *ptr >= '0') || *ptr == '.')		
			farr[i++] = *ptr;		
		ptr++;
	}
	fn = atol(farr);

	return fn;
}

template <>
double parse<double>(unsigned char* data)
{
	/* 从字符串中解析出长整型数据并返回 
	*/
	float fn = 0.0;
	char farr[20] = {0};
	unsigned char * ptr = data;

	int i = 0;
	while(*ptr != '\0')
	{
		if((*ptr <= '9' && *ptr >= '0') || *ptr == '.')		
			farr[i++] = *ptr;		
		ptr++;
	}
	fn = atof(farr);

	return fn;
}

template <>
float parse<float>(unsigned char* data)
{
	float fn = 0.0;
	char farr[20] = {0};
	unsigned char * ptr = data;

	int i = 0;
	while(*ptr != '\0')
	{
		if((*ptr <= '9' && *ptr >= '0') || *ptr == '.')		
			farr[i++] = *ptr;		
		ptr++;
	}
	fn = static_cast<float>(atof(farr));

	return fn;
}

void FindMin(vector<float>& vArr, float& minV)
{
	float minister = 0.0;
	auto it = vArr.begin();
	minister = *it;
	for(;it != vArr.end(); ++it)
	{
		if(minister >= *it)
		{
			minister = *it;
		}
	}
	minV = minister;
}

void FindMax(vector<float>& vArr, float& maxV)
{
	float maxister = 0.0;
	auto it = vArr.begin();
	maxister = *it;
	for(;it != vArr.end(); ++it)
	{
		if(maxister <= *it)
		{
			maxister = *it;
		}
	}
	maxV = maxister;
}

//find string in string, return the first start location or -1 if can not find
int StringFind(const char *pSrc, const char *pDst)
{
	int i, j;
	for (i=0; pSrc[i]!='\0'; i++)
	{
		if(pSrc[i]!=pDst[0])
			continue;		
		j = 0;
		while(pDst[j]!='\0' && pSrc[i+j]!='\0')
		{
			j++;
			if(pDst[j]!=pSrc[i+j])
			break;
		}
		if(pDst[j]=='\0')
			return i;
	}
	return -1;
}

string GetTimeDateString()
{
	SYSTEMTIME lpSystime;
	GetSystemTime(&lpSystime);
	CString str;
	str.Format("%4d-%02d-%02d-%02d-%02d-%02d", lpSystime.wYear, lpSystime.wMonth, lpSystime.wDay, lpSystime.wHour+8, lpSystime.wMinute, lpSystime.wSecond);
	return str.GetString();
}

string GetDateString()
{
	SYSTEMTIME lpSystime;
	GetSystemTime(&lpSystime);
	CString str;
	str.Format("%4d%02d%02d", lpSystime.wYear, lpSystime.wMonth, lpSystime.wDay);
	return str.GetString();
}

int comstr2int(string str)
{
	int pos = str.find("COM", 0);
	if(pos>=0)
	{
		string tmp = str.substr(pos + strlen("COM"),str.size()- pos - strlen("COM"));
		int ret = atoi(tmp.c_str());
		return ret;
	}
	else
	{
		return -1;
	}
}

void Split(char* in, vector<string>& out, char* sep)
{
	int len = strlen(in);
	char* tmp = new char[len+1];
	memset(tmp, 0, len+1);
	memcpy(tmp, in, len);
	char *p;
	p = strtok(tmp, sep);
	while(p){
		out.push_back(p);
		p = strtok(NULL, sep);
	}
	delete [] tmp;
	tmp = NULL;
}

void Split(string in, vector<string>& out, char* sep)
{
	string tmp = in;
	char *p;
	p = strtok(const_cast<char*>(tmp.c_str()), sep);
	while(p){
		out.push_back(p);
		p = strtok(NULL, sep);
	}
}

APPINFO make_appinfo(string s1, string s2, string s3, string s4, string s5, string s6, string s7 )
{
	static APPINFO info;
	info.Station = s1;
	info.Storage = s2;
	info.Gudao   = s3;
	info.TrainType = s4;
	info.TrainNo   = s5;
	info.TrainPort = s6;
	info.Gudaoliewei = s7;
	return info;
}