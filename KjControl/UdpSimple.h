/*
	UdpSimple.h
	�򵥵�UDP��/����
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
	
	unsigned int m_localport;   //�����˿�
	unsigned int m_remoteport;	//Զ�̶˿�
	string	m_remoteip;			//Զ��IP

	char m_recvbuff[1024];

};