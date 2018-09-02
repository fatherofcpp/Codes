#pragma once

#include "CmAbstractTransf.h"
namespace NSP_STP_CM
{
	class CUdp :
		public AbstractTransf
	{
	public:
		CUdp(int family = AF_INET);
		virtual ~CUdp();

		virtual int ConnectService();
		virtual int DisconnectService();
		virtual int Send2Service(const char *p_data, int i_data);
		virtual int RecvFrmService(char *p_data, int i_data);

	public:
		void CloseSockFd();
		int GetSockFd(){return m_sockFd;}
		void SetSockFd(int sock_fd){m_sockFd = sock_fd;}
		void SetServerIp(const char* ip);
		void SetServerPort(int port);
		void SetReadTimeOut( timeval& time_out);
		void SetWriteTimeOut( timeval& time_out);
		void SetLocalPort(int port){m_localPort = port;}
		int BindPort();
		void CloseBind();

	private:
		int	InitNet();
		int BindFrmServer();
	private:
		int m_family;// AF_INET;
		int m_protocol;// IPPROTO_UDP
		int m_sockType;// SOCK_DGRAM;
		int m_sockFd;
		char m_serverIp[32];
		int m_serverPort;
		int m_localPort;
		timeval  m_readTimeOut;
		timeval  m_writeTimeOut;
	};
}
