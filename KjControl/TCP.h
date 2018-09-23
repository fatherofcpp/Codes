/*
	TCP WRAP CLASS
	use block mode
*/
#pragma once
#include <WinSock2.h>
#include <string>
using namespace std;

class TCP
{
public:
	TCP();
	~TCP();
	// TCP Client
	bool Connection(string ip, int port);
	int Send(const char* buff, int len);
	int Recv(char* buff, int len);
	bool isConnected();
	void Close();
	// TCP Server

private:
	SOCKET m_sSock;
	SOCKADDR_IN addrSrv;
	
	bool m_bConnected;
	bool m_bWSAStartup;

};