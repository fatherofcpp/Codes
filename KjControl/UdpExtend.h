/*
//	UdpExtend.h Í·ÎÄ¼þ
//
//
*/
#pragma once
#include "stdafx.h"
#include "thread.h"
#include <WinSock2.h>
#include <vector>
#include <string>
#include <queue>

typedef void (CALLBACK* DEALP)(string str, LPVOID context);
class CUdp2Extend :
	public CThread
{
public:
	CUdp2Extend(void);
	virtual ~CUdp2Extend(void);
	virtual int Run();
	int initlized(unsigned short localport, string remoteIP, unsigned short remoteport);	
	void SetCallBack(DEALP func, void* context) {m_func = func, m_pContext = context;};
	int Send(char* buff, int len);
	int Recv(char* buff, int len);
	string& Getcurrcmd(){return m_currcmd;};
	void    Setcurrcmd(string str){ m_currcmd = str;}
protected:
	std::string m_currcmd;
private:
	SOCKET m_ser;
	SOCKADDR_IN m_serAddr;
	SOCKADDR_IN m_addrClient;
	
	unsigned int m_port;
	unsigned int m_remoteport;
	string m_remoteIP;
	char buf[1024];
	char m_recvbuff[10240];	

	DEALP m_func;
	LPVOID   m_pContext;
};
