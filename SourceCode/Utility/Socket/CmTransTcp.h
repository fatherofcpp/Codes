#pragma once

#include "CmAbstractTransf.h"
namespace NSP_STP_CM
{
	class CTcp :
		public AbstractTransf
	{
	public:
		CTcp(int family = AF_INET);
		virtual ~CTcp();

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
		int ConnectSocket2Server( sockaddr* p_server, int len);
		int BindFrmServer();
		bool GetConnectState(){return m_isConnected;}
		void SetConnectState(bool is_connect){m_isConnected = is_connect;}
	private:
		int m_family;// AF_INET;
		int m_protocol;// IPPROTO_IP
		int m_sockType;// SOCK_STREAM;
		int m_sockFd;
		char m_serverIp[32];
		int m_serverPort;
		int m_localPort;
		timeval  m_readTimeOut;
		timeval  m_writeTimeOut;
		bool m_isConnected;
	};
}
