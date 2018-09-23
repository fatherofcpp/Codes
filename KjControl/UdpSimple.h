/*
	UdpSimple.h
	简单的UDP收/发类
*/
#pragma once
#include "stdafx.h"
#include <WinSock2.h>
#include <string>
//#include "PLCProtocal.h"

using namespace std;

class UdpSimple
{
public:
	UdpSimple(void);	
	~UdpSimple(void);
	int initlized(int localport, string remoteIP, int remotePort);
	int Send(BYTE* buff, int len);
	int Recv(BYTE* buff, int len);
private:
	SOCKET m_ser;
	SOCKADDR_IN m_serAddr;
	SOCKADDR_IN	m_remoteAddr;
	char buf[1024];
	
	unsigned int m_localport;   //本机端口
	unsigned int m_remoteport;	//远程端口
	string	m_remoteip;			//远程IP

	char m_recvbuff[1024];

};