#include "stdafx.h"
#include "TCPClient.h"


#pragma comment(lib, "Ws2_32")

TCPClient::TCPClient()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	CString str;
	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		//cout<<"Socket initialize failed! ..." <<endl;
		str = "Socket initialize failed! ...";
		MessageBox(NULL,str,"tips", MB_OK);
		return;
	}

	if(LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		//cout<<"Socket library version isn't match ..."<<endl;
		str = "Socket library version isn't match ...";
		MessageBox(NULL,str,"tips", MB_OK);
		return;
	}

	sockClient = NULL;
}

TCPClient::~TCPClient()
{
	WSACleanup();
}

//void TCPClient::Inital(int localport, std::string servip, int servport)
void TCPClient::Inital(std::string localip, int localport, std::string servip, int servport)
{
	m_localip = localip;
	m_localport = localport;
	m_servip = servip;
	m_servport = servport;

	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//LPHOSTENT lpHost = gethostbyname(m_servip.c_str());
	
	int TimeOut = 10000; 
	if(::setsockopt(sockClient,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		//OutputDebugString("Send timeout Set failed!...\n");
		TRACE("Send timeout Set failed!...\n");
		return ;
	}

	TimeOut = 1000;
	if(::setsockopt(sockClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		//OutputDebugString("Recv timeout Set failed!...\n");
		TRACE("Recv timeout Set failed!...\n");
		return ;
	}

	//addrSrv.sin_addr.S_un.S_addr = *((u_long FAR *)(lpHost->h_addr));;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_servip.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port   = htons(m_servport);

	//addrCli.sin_addr.s_addr = htonl(INADDR_ANY);
	addrCli.sin_addr.s_addr = inet_addr(m_localip.c_str());
	addrCli.sin_family = AF_INET;
	addrCli.sin_port   = htons(m_localport);
	if (bind(sockClient, (SOCKADDR*)&addrCli, sizeof(SOCKADDR_IN)))
	{
		DWORD err = GetLastError();
		TRACE("bind error, error coed = %d\n", err);
		WSACleanup();
		return ;
	}
}

int TCPClient::Connect()
{
	if(connect(sockClient, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR)) != SOCKET_ERROR)
		return 1;
	else
		return 0;
}

void TCPClient::Disconnect()
{
	if(sockClient != NULL)
		closesocket(sockClient);
}

int TCPClient::Send(char* buff, int len)
{
	int ret = send(sockClient, buff, len, 0);
	return ret;
}

int TCPClient::Recv(char* buff, int len)
{
	int ret = recv(sockClient, buff, len, 0);
	return ret;
}