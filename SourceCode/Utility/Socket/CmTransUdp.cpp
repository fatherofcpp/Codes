#include "stdafx.h"

#include "CmTransUdp.h"
#include "CmFunction/CmFunction.h"

#include "Log/CmLog.h"

#define PRINT_UDP  

NSP_STP_CM::CUdp::CUdp(int family):
	m_family(family)
{
	m_sockType = SOCK_DGRAM;
	m_protocol = IPPROTO_UDP;

	m_sockFd = -1;
	m_readTimeOut.tv_sec = 0;
	m_readTimeOut.tv_usec = 2 * 1000;

	m_writeTimeOut.tv_sec = 0;
	m_writeTimeOut.tv_usec = 2 * 1000;

	int err = SocketInit();
	if(err != 0)
	{
		gLog.Add("%s WSAStartup err\n", __FUNCTION__);
		exit(0);
	}

}

NSP_STP_CM::CUdp::~CUdp()
{
	DisconnectService();
}

int NSP_STP_CM::CUdp::ConnectService()
{
	if(m_sockFd > 0)
	{
		return m_sockFd;
	}

	m_sockFd = InitNet();
	if( m_sockFd == -1 )
	{
		PRINT_UDP("[%s]:InitNet failed\n", __FUNCTION__);
		return m_sockFd;
	}

	return m_sockFd;
}

int NSP_STP_CM::CUdp::DisconnectService()
{
	CloseSockFd();
	return 0;
}

int NSP_STP_CM::CUdp::Send2Service(const char *p_data, int i_data)
{
	ConnectService();

	sockaddr_in server;
	memset(&server, 0, sizeof(server));
#if(_WIN32_WINNT < 0x0600)
	server.sin_family = static_cast<short>(m_family);
#else
	server.sin_family = static_cast<ADDRESS_FAMILY>(m_family);
#endif
	server.sin_addr.s_addr = inet_addr(m_serverIp) /* *((unsigned long*)hp->h_addr)*/;
	server.sin_port = htons(static_cast<u_short>(m_serverPort));
	int nlen = sizeof(server);

	int ret =  sendto(m_sockFd, p_data, i_data, 0, (sockaddr*)&server, nlen);
	return ret;
}

int NSP_STP_CM::CUdp::RecvFrmService(char *p_data, int i_data)
{
	fd_set fds_r;
	fd_set fds_e;
	int i_ret = 0;
	FD_ZERO( &fds_r );
	FD_SET( static_cast<unsigned int>(m_sockFd), &fds_r );
	FD_ZERO( &fds_e );
	FD_SET(  static_cast<unsigned int>(m_sockFd), &fds_e );

	i_ret = select(m_sockFd + 1, &fds_r, NULL, &fds_e, &m_readTimeOut);

	if( i_ret > 0 && FD_ISSET(m_sockFd, &fds_r))
	{
		SOCKADDR_IN from_addr;
		int svr_addr_len = sizeof(from_addr);
		int rev_rst = recvfrom(m_sockFd, p_data, i_data, 0, (LPSOCKADDR)&from_addr, &svr_addr_len);  
		return rev_rst;
	}
	else 
	{
		return -1;
	}
	return -1;
}

int NSP_STP_CM::CUdp::InitNet()
{
	int i_val;
	int fd = socket( m_family, m_sockType, m_protocol );
	if( fd == -1 )
	{
		if( WSAGetLastError ( ) != WSAEAFNOSUPPORT )
		{
			PRINT_UDP("InitNet:can not create socket (%i)\n", WSAGetLastError());
		}
		return -1;
	}

	unsigned long i_dummy = 1;
	if( ioctlsocket( fd, FIONBIO, &i_dummy ) != 0 )
	{
		PRINT_UDP("InitNet:can not set non-block model\n");
	}

	i_val = 1;
	setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&i_val, sizeof( i_val ) );
	return fd;
}

void NSP_STP_CM::CUdp::SetServerIp(const char *ip)
{
	if (ip)
	{
		strcpy(m_serverIp,ip);
	}
}

void NSP_STP_CM::CUdp::SetServerPort(int port)
{
	if (port> 0)
	{
		m_serverPort = port;
	}
}

void NSP_STP_CM::CUdp::SetReadTimeOut(timeval &time_out)
{
	memcpy(&m_readTimeOut, &time_out, sizeof(time_out));
}

void NSP_STP_CM::CUdp::SetWriteTimeOut(timeval &time_out)
{
	memcpy(&m_writeTimeOut, &time_out, sizeof(time_out));
}

int NSP_STP_CM::CUdp::BindPort()
{
	int ret = 0;

	m_sockFd = InitNet();
	if( m_sockFd == -1 )
	{
		PRINT_UDP("[%s]:InitNet failed\n", __FUNCTION__);
		return -1;
	}

	ret = BindFrmServer();
	if (ret != 0)
	{
		CloseBind();
		return -2;
	}

	return ret;
}

int NSP_STP_CM::CUdp::BindFrmServer()
{
	int ret = 0;
	sockaddr_in addr_svr; 
	addr_svr.sin_addr.s_addr = htonl(INADDR_ANY);
#if(_WIN32_WINNT < 0x0600)
	addr_svr.sin_family = static_cast<short>(m_family);
#else
	addr_svr.sin_family = static_cast<ADDRESS_FAMILY>(m_family);
#endif
	int port_svr = m_localPort;
	addr_svr.sin_port = htons(static_cast<u_short>(port_svr));

	ret = bind(m_sockFd, (sockaddr*)&addr_svr, sizeof(sockaddr));   
	if (ret != 0) 
	{
		PRINT_UDP("[%s] bind fail\n", __FUNCTION__);
		return -1;
	}

	return 0;
}

void NSP_STP_CM::CUdp::CloseBind()
{
	CloseSockFd();
}

void NSP_STP_CM::CUdp::CloseSockFd()
{
	if(m_sockFd > 0)
	{
		shutdown(m_sockFd, 2);
		closesocket(m_sockFd);
		m_sockFd = -1;
	}
}
