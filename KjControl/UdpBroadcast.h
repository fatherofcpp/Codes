#pragma once
#include "thread.h"
#include <WinSock2.h>
#include <string>

using namespace std;

class CUdpBroadcast 	
{
public:
	CUdpBroadcast(void);	
	~CUdpBroadcast(void);
	int initlized(unsigned short port);	
	bool SendData(char* buf, int len);
	void StartRead();
	void StopRead();
	int	GetMeasureValue();	
private:
	SOCKET m_ser;
	SOCKADDR_IN m_serAddr;
	SOCKADDR_IN m_addrto;
	char buf[1024];
	unsigned int m_port;
	char m_recvbuff[1024];
	string strID;
	HANDLE	hThread;
	BOOL	isExit;
	static void readcallback(LPVOID param);
	int     mValue;
	HANDLE	valueOk;
};
