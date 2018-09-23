#include "stdafx.h"
#include "TCP.h"

TCP::TCP():m_sSock(NULL),
	m_bConnected(false),
	m_bWSAStartup(false)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		TRACE("Socket initialize failed! ...\r\n");
		//return FALSE;
		m_bWSAStartup = false;
		return;
	}

	if(LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		TRACE("Socket library version isn't match ...\r\n");
		//return FALSE;
		m_bWSAStartup = false;
		return;
	}
	m_bWSAStartup = true;
}

TCP::~TCP()
{
	Close();
	if(m_bWSAStartup)
		WSACleanup();
}

bool TCP::Connection(string ip, int port)
{
	m_sSock = socket(AF_INET, SOCK_STREAM, 0);

	int TimeOut = 1000; 
	if(::setsockopt(m_sSock,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		TRACE("Send timeout Set failed!...\r\n");
		return false;
	}
	TimeOut = 1000;
	if(::setsockopt(m_sSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		TRACE("Recv timeout Set failed!...\n");
		return false;
	}

	addrSrv.sin_addr.s_addr = inet_addr(ip.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port   = htons(port);
	
	if(connect(m_sSock, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR)) != SOCKET_ERROR)
		m_bConnected = true;
	else
	{
		m_bConnected = false;
		return false;
	}
	return true;
}

int TCP::Send(const char* buff, int len)
{
	int ret = send(m_sSock, buff, len, 0);
	return ret;
}

int TCP::Recv(char* buff, int len)
{
	int ret = recv(m_sSock, buff, len, 0); 	
	return ret;
}

bool TCP::isConnected()
{
	return m_bConnected;
}

void TCP::Close()
{
	if (!m_bConnected) return;
	if (m_sSock == NULL) return;
	shutdown(m_sSock, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sSock);
	m_sSock = NULL;
	m_bConnected = FALSE;
}
