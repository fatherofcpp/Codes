/*
	MOXA Serial Port Tool Wrapped: the base tool is Pcomm Lite, Version 1.6
*/
//#pragma once 
#include "stdafx.h"
#include <Windows.h>
#include <string>
#include "PCOMM.H"

#pragma comment(lib, "PCOMM.lib")
using namespace std;

typedef void (__stdcall	*DataReadCallBack)(void* context, char* buff, int len);
class MoxaSerialport
{
public:
	MoxaSerialport(int com, int bd);
	~MoxaSerialport();
	int Open();
	int Close();
	int  Send(char* buff, int len);
	int  Read(string& str);
	void SetCallBack(DataReadCallBack callback, void* pContext);
	void Start();
	void Stop();
	void SetAffinity(DWORD CPUNo);
	void SetReadLen(int len) {m_len = len;}
	static void Recvthread(LPVOID param);

private:
	int BDConvert(int bd);
	int spcom;
	int spbd;
	int m_len;
	HANDLE hThread;
	HANDLE hExit;
	DataReadCallBack m_Callback;
	void* m_pContext;
};