#include "stdafx.h"
#include "UdpSimple.h"

UdpSimple::UdpSimple():m_ser(0)
{}

UdpSimple::~UdpSimple()
{
	closesocket(m_ser);
}

int UdpSimple::initlized(int localport, string remoteIP, int remotePort)
{
	m_localport = localport;
	m_remoteip = remoteIP;
	m_remoteport = remotePort;

	memset(&m_serAddr,0,sizeof(SOCKADDR_IN));
	memset(&m_remoteAddr,0,sizeof(SOCKADDR_IN));

	// socket initial
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
	//local 
	m_serAddr.sin_family = AF_INET;
	m_serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serAddr.sin_port = htons(m_localport);
	//remote 
	m_remoteAddr.sin_family = AF_INET;
	m_remoteAddr.sin_addr.s_addr = inet_addr(m_remoteip.c_str());
	m_remoteAddr.sin_port = htons(m_remoteport);

	m_ser = socket(AF_INET,SOCK_DGRAM,0);
	if (m_ser == INVALID_SOCKET)
	{
		WSACleanup();
		return 0;
	}
	unsigned int TimeOut = 3600000000;
	::setsockopt(m_ser,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	if (bind(m_ser,(SOCKADDR*)&m_serAddr,sizeof(SOCKADDR_IN)))
	{
		WSACleanup();
		return 0;
	}
	return 1;
}

int UdpSimple::Send(BYTE* buff, int len)
{
	int ret = sendto(m_ser, (char*)buff, len, 0, (SOCKADDR*)&m_remoteAddr, sizeof(SOCKADDR));
	return ret;
}

int UdpSimple::Recv(BYTE* buff, int len)
{
	int lenSockAddr = sizeof(SOCKADDR);	
	int ret = recvfrom(m_ser, (char*)buff, len, 0, (SOCKADDR*)&m_remoteAddr, &lenSockAddr);
	return ret;
}