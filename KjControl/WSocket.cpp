//////////////////////////////////////////////////////////////////////
//WYD's Socket
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSocket.h"
#include <TIME.H>
#include <stdio.h> 
//#include "myFile.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWSocket::CWSocket()
{
	WSADATA wsaData;

	m_sSocket = NULL;
	m_bConnected = FALSE;
	m_sServer = NULL;
	m_bListened = FALSE;
	m_nServerPort = -1;
	m_hServerThread = NULL;

	m_bNeedClose = FALSE;
	m_bNeedCloseServer = FALSE;

	if (WSAStartup(0x202, &wsaData) == 0)
	{
		m_bWSAStartup = TRUE;
		err = 0;
	}
	else
	{
		m_bWSAStartup = FALSE;
		err = WSAGetLastError();
	}
}

CWSocket::CWSocket(SOCKET s)
{
	m_sSocket = NULL;
	m_bWSAStartup = FALSE;
	m_sServer = NULL;
	m_bListened = FALSE;
	m_nServerPort = -1;
	m_hServerThread = NULL;

	m_bNeedClose = FALSE;
	m_bNeedCloseServer = FALSE;

	m_bConnected = IsSockConnected(s);
	if(m_bConnected) m_sSocket = s;

	if (!m_bConnected)
	{
		WSADATA wsaData;
		if (WSAStartup(0x202, &wsaData) == 0)
		{
			m_bWSAStartup = TRUE;
			err = 0;
		}
		else
		{
			m_bWSAStartup = FALSE;
			err = WSAGetLastError();
		}
	}
}

void CWSocket::operator=(SOCKET s)
{
	if (IsSockConnected(s))
	{
		if(m_bConnected) Close();
		m_sSocket = s;
		m_bConnected = TRUE;
		m_bNeedClose = FALSE;
	}
}

CWSocket::~CWSocket()
{
	if (m_bNeedClose) Close();
	if (m_bNeedCloseServer) CloseServer();
	if (m_bWSAStartup) WSACleanup();
}

BOOL CWSocket::Connection(LPCSTR pstrHost, int nPort,int timeout)
{
	if (m_bConnected) Close();

	LPHOSTENT lpHost;
	struct sockaddr_in server;

	//Lookup host
	lpHost = gethostbyname(pstrHost);
	if(lpHost == NULL) return FALSE;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = *((u_long FAR *)(lpHost->h_addr));
	server.sin_port=htons(nPort);

	m_sSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sSocket <= 0)
	{
		err = WSAGetLastError();
		return FALSE;
	}
	u_long mode = 1;
	ioctlsocket(m_sSocket, FIONBIO, &mode); //����Ϊ������ģʽ
	timeval tm;
	fd_set set;
	char error[4]={0};
	int len;
	if( connect(m_sSocket, (struct sockaddr *)&server, sizeof(SOCKADDR)) == 
		-1)
	{
		tm.tv_sec = timeout;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(m_sSocket, &set);
		if( select(m_sSocket+1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(m_sSocket, SOL_SOCKET, SO_ERROR, error, &len);
			if(((int)error) != 0) 
			{
				err = WSAGetLastError();
				closesocket(m_sSocket);
				m_sSocket = NULL;
				return FALSE;
			}
		} 
		else 
		{
			err = WSAGetLastError();
			closesocket(m_sSocket);
			m_sSocket = NULL;
			return FALSE;
		}
	}
	mode = 0;
	ioctlsocket(m_sSocket, FIONBIO, &mode); //����Ϊ����ģʽ
	m_bNeedClose = TRUE;
	m_bConnected = TRUE;
	return TRUE;
}

BOOL CWSocket::Connection(LPCSTR pstrHost, int nPort)
{
	if (m_bConnected) Close();

	LPHOSTENT lpHost;
	struct sockaddr_in server;

	//Lookup host
	lpHost = gethostbyname(pstrHost);
	if(lpHost == NULL) return FALSE;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = *((u_long FAR *)(lpHost->h_addr));
	server.sin_port=htons(nPort);

	m_sSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sSocket <= 0)
	{
		err = WSAGetLastError();
		return FALSE;
	}

	if (connect(m_sSocket, (LPSOCKADDR)&server, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		err = WSAGetLastError();
		closesocket(m_sSocket);
		m_sSocket = NULL;
		return FALSE;
	}

	m_bNeedClose = TRUE;
	m_bConnected = TRUE;
	return TRUE;
}

void CWSocket::Close()
{
	if (!m_bConnected) return;
	if (m_sSocket == NULL) return;

	shutdown(m_sSocket, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sSocket);
	m_sSocket = NULL;
	m_bConnected = FALSE;
}

void CWSocket::CloseServer()
{
	if(!m_bListened) return;
	if(m_sServer == NULL) return;

	shutdown(m_sServer, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sServer);
	m_sServer = NULL;
	m_bListened = FALSE;
}


DWORD CALLBACK CWSocket::TimeoutControl(LPVOID lpParm)
{
	TPARA* para = (TPARA*)lpParm;
	time_t stime = time(NULL);
	BOOL bTimeover = FALSE;

	while(!bTimeover)
	{
		if(para->bExit)
		{
			para->IsExit = TRUE;
			return 0;
		}

		Sleep(1);

		time_t ntime = time(NULL);
		if((ntime - stime) > para->OutTime) bTimeover = TRUE;
	}

	if(para->bExit)
	{
		para->IsExit = TRUE;
		return 0;
	}

	if(para->s != NULL)
	{
		para->pbConnected[0] = FALSE;
		shutdown(para->s, SD_RECEIVE);
		Sleep(5);
		closesocket(para->s);
	}

	para->IsExit = TRUE;
	return 0;
}

int CWSocket::ReadData(char FAR* buf, int& len, int timeout)
{
	if(!m_bConnected || m_sSocket == NULL)
	{
		err = -1;
		return -1;
	}

	HANDLE hThread;
	DWORD dwThreadId;
	TPARA para;

	para.OutTime = timeout;
	para.s = m_sSocket;
	para.bExit = FALSE;
	para.IsExit = FALSE;
	para.pbConnected = &m_bConnected;
	hThread = CreateThread(NULL, NULL, TimeoutControl, (LPVOID)(&para), 0, &dwThreadId);
	if (hThread == NULL) return -1;

	int nRet = recv(m_sSocket, buf, len, 0);
	if(nRet == SOCKET_ERROR) err = WSAGetLastError();

	para.bExit = TRUE;
	while(!para.IsExit) Sleep(1);
    CloseHandle(hThread);
	hThread=NULL;
	return nRet;
}

int CWSocket::SendData(const char FAR *buf, int len, int timeout)
{
	if(!m_bConnected || m_sSocket == NULL)
	{
		err = -1;
		return -1;
	}

	if(!IsSockConnected(m_sSocket))
	{
		m_bConnected = FALSE;
		err = -1;
		return -1;
	}

	int nRet = send(m_sSocket, buf, len, 0);

	if(nRet == SOCKET_ERROR) err = WSAGetLastError();

	return nRet;
}

BOOL CWSocket::IsSockConnected(SOCKET s)
{
	int nRet = 0;
	struct fd_set Fd_Recv;
    struct timeval Time_Recv;

	memset(&Fd_Recv, 0, sizeof(struct fd_set));
    FD_CLR(s, &Fd_Recv); 
    FD_SET(s, &Fd_Recv); 
    Time_Recv.tv_sec = 0;
    Time_Recv.tv_usec = 0;

	nRet = select(s, &Fd_Recv, NULL, NULL, &Time_Recv);

    return (nRet == 0);
}

BOOL CWSocket::CreateServer(int port, int backlog)
{
	if(m_bListened == TRUE) CloseServer();

	struct	sockaddr_in local;

	//��������socket
    m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(m_sServer == SOCKET_ERROR)
    {
        err = WSAGetLastError();
        return FALSE;
    }

	//�����	
	local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_family = AF_INET;
    local.sin_port = htons(port);

	if(bind(m_sServer, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
		err = WSAGetLastError();
		closesocket(m_sServer);
        return FALSE;
    }

	//��ʼ����
    if(listen(m_sServer, backlog) != 0)
	{
		err = WSAGetLastError();
		closesocket(m_sServer);
        return FALSE;
    }

	m_nServerPort = port;
	m_bListened = TRUE;
	m_bNeedCloseServer = TRUE;

	return TRUE;
}

SOCKET CWSocket::Listen(char *ClientIP)
{
	if(!m_bListened) return -1;
	if(m_sServer == NULL) return -1;

	SOCKET	sClient;	
	int		iAddrSize;
	struct	sockaddr_in addr;

	iAddrSize = sizeof(addr);

	sClient = accept(m_sServer, (struct sockaddr *)&addr, &iAddrSize);
	
	if(sClient == SOCKET_ERROR)
	{
		err = WSAGetLastError();
        return SOCKET_ERROR;
    }

	if(ClientIP != NULL)
	{
		sprintf(ClientIP, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, 
										 addr.sin_addr.S_un.S_un_b.s_b2,
										 addr.sin_addr.S_un.S_un_b.s_b3,
										 addr.sin_addr.S_un.S_un_b.s_b4);
	}

	return sClient;
}

BOOL CWSocket::StartServer(DEALPROC lpDealFunc)
{
	if(!m_bListened) return -1;
	if(m_sServer == NULL) return -1;

	DWORD dwThreadId;
	SERVERPARA *para;

	para = new SERVERPARA;
	para->s = m_sServer;
	para->port = m_nServerPort;
	para->lpDealFunc = lpDealFunc;

	m_hServerThread = CreateThread(NULL, 0, ServerProc, (LPVOID)(para), 0, &dwThreadId);

	if(m_hServerThread == NULL)
	{
		delete para;
		err = WSAGetLastError();
        return FALSE;
	}
	
	return TRUE;
}

BOOL CWSocket::StopServer()
{
	CloseServer();

	return TRUE;
}

DWORD CALLBACK CWSocket::ServerProc(LPVOID lpParm)
{
	SERVERPARA *para = (SERVERPARA*)lpParm;
	
	if(para == NULL) return -1;

	SOCKET		s = para->s;
	int			port = para->port;
	DEALPROC	lpDealFunc = para->lpDealFunc;
	delete para;

	SOCKET	sClient;	
	int		iAddrSize;
	struct	sockaddr_in addr;
	char	IP[32];
	HANDLE	hThread;
    DWORD	dwThreadId;
	DEALPARA *parac;

	iAddrSize = sizeof(addr);

	while(1)
	{
		sClient = accept(s, (struct sockaddr *)&addr, &iAddrSize);//����
		
		if(sClient == SOCKET_ERROR) break;

		sprintf(IP, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, 
								   addr.sin_addr.S_un.S_un_b.s_b2,
								   addr.sin_addr.S_un.S_un_b.s_b3,
								   addr.sin_addr.S_un.S_un_b.s_b4);

		parac = new DEALPARA;
		memset(parac->IP, 0, sizeof(parac->IP));
		parac->s = sClient;
		parac->port = port;
		parac->lpDealFunc = lpDealFunc;
		memcpy(parac->IP, IP, strlen(IP));	

		//���������ӣ���һ���߳�		
        hThread = CreateThread(NULL, 0, DealProc, (LPVOID)(parac), 0, &dwThreadId);

		if(hThread == NULL) delete parac;

		CloseHandle(hThread);
		hThread=NULL;
	}

	return 0;
}

DWORD CALLBACK CWSocket::DealProc(LPVOID lpParm)
{
	DEALPARA *para = (DEALPARA*)lpParm;
	
	if(para == NULL) return -1;

	SOCKET		s = para->s;
	int			port = para->port;
	DEALPROC	lpDealFunc = para->lpDealFunc;
	char* IP=new char[32];
	memcpy(IP, para->IP, sizeof(IP));
	delete para;

	try{
		lpDealFunc(s, port, IP);
	}catch(...){}
	
	if(s!=NULL)
	{
		try{closesocket(s);}catch(...)
		{
// 			CString temp="���������߳��쳣!\r\n";
// 			myFile *mf=new myFile("systemlog.log",false,false);
// 			mf->writeend(temp);
// 			delete mf;
// 			mf=NULL;
			
			
		}
		s=NULL;
	}
	delete IP;
	IP=NULL;
    
	return 0;
}

SOCKET CWSocket::GetSocket()
{
	return m_sSocket;
}

SOCKET CWSocket::GetServerSocket()
{
	return m_sServer;
}

int CWSocket::GetError()
{
	return err;
}

void CWSocket::inttochars(int i,char* b)
{
	b[0]=(char)(i&0x000000ff);
	b[1]=(char)((i&0x0000ff00)>>8);
	b[2]=(char)((i&0x00ff0000)>>16);
	b[3]=(char)((i&0xff000000)>>24);
}

int CWSocket::SendInt(int shu, int timeout)
{
	char m[4];
	inttochars(shu,m);
	int fan= SendData(m,4,timeout);
	if(fan<=0){return -1;}
	return 1;
}

void CWSocket::charstoint(int& i,char * j)
{
	i=(((int)j[3]<<24&0xff000000)|((int)j[2]<<16&0x00ff0000)|((int)j[1]<<8&0x0000ff00)|((int)j[0]&0x000000ff));
}

int CWSocket::ReadInt(int timeout)
{
	char du[4];
	int len = 4;
	int d=ReadData(du,len,timeout);
	if(d!=4)
	{
		return -1;
	}
	int fan;
	charstoint(fan,du);
	return fan;
}

bool CWSocket::readfully(char *du, int len, int timeout)
{
	int shu=0;
	int cha=0;
	int qian=0;
	int du1=0;
	bool tiao=false;
	while(1)
	{
		cha=len-shu;
		char *temp=new char[cha];
		try{
			qian=shu;
			du1=ReadData(temp,cha,timeout);
			shu+=du1;
			memcpy(&du[qian],temp,du1);
			cha=len-shu;
			if(cha==0)
			{
				tiao=true;
			}
		}
		catch(...)
		{
		// 	CString temp="�������쳣!\r\n";
		// 	myFile *mf=new myFile("systemlog.log",false,false);
		// 	mf->writeend(temp);
		// 	delete mf;
		// 	mf=NULL;
		}
		delete temp;
		temp=NULL;
		if(tiao)
		{
			break;
		}
	}

	return true;
}
