#pragma once
#include <winsock2.h>
#include <string>

class TCPClient
{
public:
	TCPClient();
	virtual ~TCPClient();
	void Inital(std::string localip, int localport, std::string servip, int servport);
	int Connect();
	void Disconnect();
	int Send(char* buff, int len);
	int Recv(char* buff, int len);
private:
	SOCKET sockClient;
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN addrCli;
	int m_localport;
	int m_servport;
	std::string m_servip;
	std::string m_localip;
};