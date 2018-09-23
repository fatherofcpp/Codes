#include "StdAfx.h"
#include "UdpBroadcast.h"
//#include <WinSock2.h>
//#include "..\Console.h"

static int StringFind(const char *pSrc, const char *pDst)
{
	int i, j;
	for (i=0; pSrc[i]!='\0'; i++)
	{
		if(pSrc[i]!=pDst[0])
			continue;		
		j = 0;
		while(pDst[j]!='\0' && pSrc[i+j]!='\0')
		{
			j++;
			if(pDst[j]!=pSrc[i+j])
			break;
		}
		if(pDst[j]=='\0')
			return i;
	}
	return -1;
}

CUdpBroadcast::CUdpBroadcast(void):m_ser(0)
{
	hThread = NULL;
	isExit = FALSE;
	mValue = 0;
	valueOk = NULL;
}
int CUdpBroadcast::initlized(unsigned short port)
{
	m_port = port;
	memset(&m_serAddr,0,sizeof(SOCKADDR_IN));	

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
	m_addrto.sin_family = AF_INET;
	//m_addrto.sin_addr.s_addr = INADDR_BROADCAST;
	m_addrto.sin_addr.s_addr = inet_addr("192.168.1.255");
	m_addrto.sin_port = htons(m_port);

	m_serAddr.sin_family = AF_INET;
	m_serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serAddr.sin_port = htons(m_port);

	m_ser = socket(AF_INET,SOCK_DGRAM,0);
	if (m_ser == INVALID_SOCKET)
	{
		WSACleanup();
		return 0;
	}
	
	if (bind(m_ser,(SOCKADDR*)&m_serAddr,sizeof(SOCKADDR_IN)))
	{
		WSACleanup();
		return 0;
	}

	bool opt = true;
	setsockopt(m_ser,SOL_SOCKET,SO_BROADCAST,(char FAR*)&opt,sizeof(opt));
	unsigned int TimeOut = 500;//接收超时时间为10秒
	setsockopt(m_ser,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	valueOk = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(valueOk);
	return 0;
}

bool CUdpBroadcast::SendData(char* buf, int len)
{
	char Buff[1024] = {0};
	int nlen = sizeof(m_addrto);
	m_addrto.sin_addr.s_addr = INADDR_BROADCAST;
	int ret = sendto(m_ser, buf, len, 0, (SOCKADDR*)&m_addrto, nlen);
	if(ret==SOCKET_ERROR)
	{
		TRACE("发送消息到传输程序失败！\r\n");
		return false;
	}
	else
	{
		TRACE("发送消息到传输程序成功！\r\n");
		return true;
	}
}

CUdpBroadcast::~CUdpBroadcast(void)
{
	closesocket(m_ser);
}

void CUdpBroadcast::StartRead()
{
	isExit = FALSE;
	hThread = (HANDLE)_beginthread(readcallback, 0, this);
}

void CUdpBroadcast::StopRead()
{
	isExit = TRUE;
	//Sleep(2000);
	if(hThread != NULL)
	{
		CloseHandle(hThread);
		hThread = NULL;
	}
}

void CUdpBroadcast::readcallback(LPVOID param)
{
	CUdpBroadcast* dlg = (CUdpBroadcast*)param;
	char Buff[1024]= {0};
	int len = sizeof(SOCKADDR);
	//CConsole::GetInstance()->Output("broadcast-thread Open \r\n");
	while(1)
	{
		if(dlg->isExit)
			break;
		
		ZeroMemory(&dlg->m_addrto, sizeof(SOCKADDR_IN));
		memset(Buff,'\0',1024);
		
		int ret = 0;
		ret = recvfrom(dlg->m_ser, Buff, 1024, 0, (SOCKADDR*)&dlg->m_addrto, &len);
		if(ret>0)
		{
			string str;
			str.assign(Buff, ret);
			
			int ret = StringFind(str.c_str(), "{getzg}CHN16&");
			if(ret>=0)
			{
				//CConsole::GetInstance()->Output("broadcast-recv: IP=%s, PORT=%d, %s \r\n",inet_ntoa(dlg->m_addrto.sin_addr), ntohs(dlg->m_addrto.sin_port), str.c_str());
				ret = ret + strlen("{getzg}CHN16&");
				string sub = str.substr(ret, str.size());
				
				dlg->mValue = atoi(sub.c_str());
				//CConsole::GetInstance()->Output("broadcast-sub = %s, value = %d \r\n",sub.c_str(), dlg->mValue);
				SetEvent(dlg->valueOk);
			}
			str.clear();
		}
		Sleep(500);
	}
	//CConsole::GetInstance()->Output("broadcast-thread Close \r\n");
}

int	CUdpBroadcast::GetMeasureValue()
{
	DWORD ret = WaitForSingleObject(valueOk, INFINITE);
	if(ret == WAIT_OBJECT_0)
	{
		ResetEvent(valueOk);
		return mValue;
	}
	else
	{
		ResetEvent(valueOk);
		mValue = -1;
		return mValue;
	}
}