#include "StdAfx.h"
#include "UdpExtend.h"
#include "utility.h"
#include <ATLTRACE.H>
//#include "Ccfg\Cfg.h"
//#include "CLog\Log.h"
//#include "CStatus.h"
//#include "PLCProtocal.h"
//#include "Protocal2JobPlatform.h"
//#include "Console.h"
//#include "EditLog.h"

using namespace std;

CUdp2Extend::CUdp2Extend(void):
	CThread(false),
		m_ser(0) 
{
}

CUdp2Extend::~CUdp2Extend()
{
	TRACE("CUdp2Extend id = %d, exit\r\n",this->GetThreadID());
}

int CUdp2Extend::initlized(unsigned short localport, string remoteIP, unsigned short remoteport)
{
	m_port = localport;
	m_remoteport = remoteport;
	m_remoteIP = remoteIP;

	memset(&m_serAddr,0,sizeof(SOCKADDR_IN));	
	memset(&m_addrClient,0,sizeof(SOCKADDR_IN));

	return 0;
}

int CUdp2Extend::Send(char* buff, int len)
{
	m_currcmd.clear();
	m_currcmd.assign(buff,len);
	int ret = sendto(m_ser, buff, len, 0, (SOCKADDR*)&m_addrClient, sizeof(SOCKADDR));
	return ret;
}

int CUdp2Extend::Recv(char* buff, int len)
{
	int ret = recvfrom(m_ser,m_recvbuff,10240,0,(SOCKADDR*)&m_addrClient,&len);
	return ret;
}

int CUdp2Extend::Run()
{
	TRACE("CUdp2Extend thread entering, threadID = %d ... ...\r\n", this->m_nID);
	DWORD dw = 0;
	WSADATA data;
	int err = 0;
	WORD version = MAKEWORD(2,2);
	err = WSAStartup(version,&data);
	if (err)
	{
		return 0;
	}
	if (LOBYTE(version) != 2 || HIBYTE(version) != 2)
	{
		WSACleanup();
	}
	m_serAddr.sin_family = AF_INET;
	m_serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serAddr.sin_port = htons(m_port);

	m_addrClient.sin_family = AF_INET;
	m_addrClient.sin_addr.s_addr = inet_addr(m_remoteIP.c_str());
	m_addrClient.sin_port = htons(m_remoteport);

	m_ser = socket(AF_INET,SOCK_DGRAM,0);
	if (m_ser == INVALID_SOCKET)
	{
		WSACleanup();
		return 0;
	}
#if 0 
	unsigned long ul = 1;
	err = ioctlsocket(m_ser, FIONBIO, (unsigned long *)&ul);
	if(err == SOCKET_ERROR)
	{
		WSACleanup();
		return 0;
	}
#endif
#if 1
	unsigned int TimeOut = 1000;//接收超时时间为10秒
	::setsockopt(m_ser,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
#endif
	if (bind(m_ser,(SOCKADDR*)&m_serAddr,sizeof(SOCKADDR_IN)))
	{
		DWORD err = GetLastError();
		WSACleanup();
		return 0;
	}
	
	int len = sizeof(SOCKADDR);
	int ret = 0;
	string str;
	TRACE("CUdp2Extend thread entering 2nd, threadID = %d ... ...\r\n", this->m_nID);
	while(this->m_running)
	{
		//TRACE("CUdp2Extend thread running, threadID = %d ... ...\r\n", this->m_nID);
		memset(m_recvbuff,'\0',10240);
		ret = recvfrom(m_ser,m_recvbuff,10240,0,(SOCKADDR*)&m_addrClient,&len);
		if(ret > 0)
		{
			str.assign(m_recvbuff, ret);
			m_func(str, m_pContext);
		}
		if(ret == SOCKET_ERROR)
		{
			continue;
		}
		
		Sleep(200);
	}
	TRACE("CUdp2Extend thread exit, threadID = %d ... ...\r\n", this->m_nID);
	closesocket(m_ser);
	return 0;
}

