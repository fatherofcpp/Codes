#include "stdafx.h"
#include "CmTransTcp.h"
#include "CmFunction/CmFunction.h"

#include "Log/CmLog.h"

#define PRINT_TCP  
const int errSocket = -1;
NSP_STP_CM::CTcp::CTcp(int family):
	m_family(family)
{
	m_sockType = SOCK_STREAM;
	m_protocol = IPPROTO_IP;

    m_sockFd = errSocket;
    m_readTimeOut.tv_sec = 4;
    m_readTimeOut.tv_usec = 500 * 1000;

    m_writeTimeOut.tv_sec = 4;
    m_writeTimeOut.tv_usec = 500 * 1000;

	SetConnectState(false);

	int err = SocketInit();
	if(err != 0)
	{
		gLog.Add("%s WSAStartup err\n", __FUNCTION__);
		exit(0);
	}
}

NSP_STP_CM::CTcp::~CTcp()
{
	DisconnectService();
}

int NSP_STP_CM::CTcp::ConnectService()
{
	if (GetConnectState())
	{
		return m_sockFd;
	}

    m_sockFd = InitNet();
    if( m_sockFd == errSocket )
    {
        PRINT_TCP("[%s]:InitNet failed\n", __FUNCTION__);
        return m_sockFd;
    }

    sockaddr_in server;
    memset(&server, 0, sizeof(server));
#if(_WIN32_WINNT < 0x0600)
	server.sin_family = static_cast<short>(m_family);
#else
	server.sin_family = static_cast<ADDRESS_FAMILY>(m_family);
#endif
    server.sin_addr.s_addr = inet_addr(m_serverIp) /* *((unsigned long*)hp->h_addr)*/;
	server.sin_port = htons(static_cast<u_short>(m_serverPort));

    ConnectSocket2Server((sockaddr*)&server, sizeof(server));

	SetConnectState(true);
    return m_sockFd;
}

int NSP_STP_CM::CTcp::DisconnectService()
{
	if (!GetConnectState())
	{
		return 0;
	}

    CloseSockFd();
	SetConnectState(false);
    return 0;
}

int NSP_STP_CM::CTcp::Send2Service(const char *p_data, int i_data)
{
    //const int send_data_save = i_data;
    fd_set fds_w, fds_e;
    int i_send;//本次发送数据
    int i_total = 0;//总发送数据
    int i_ret = 0;
    const int step_select_m = 500;//单次select 500毫秒
    const int select_count_total //总select次数
            = ((m_writeTimeOut.tv_sec * 1000) + (m_writeTimeOut.tv_usec / 1000)) / step_select_m;
    int select_count_now = 0;//当前select次数
    while( i_data > 0 )
    {
        timeval  time_out;
        while (select_count_now ++ < select_count_total)
        {
            FD_ZERO( &fds_w );
            FD_SET( static_cast<unsigned int>(m_sockFd), &fds_w );
            FD_ZERO( &fds_e );
            FD_SET( static_cast<unsigned int>(m_sockFd), &fds_e );
            time_out.tv_sec = step_select_m / 1000;
            time_out.tv_usec = step_select_m * 1000;
            i_ret = select(m_sockFd + 1, NULL, &fds_w, &fds_e, &time_out);
            if( i_ret > 0 )
            {
                break;
            }
            else if (errno == EINTR)
            {
                select_count_now--;
            }
        }

        if( i_ret <= 0 )
        {
            PRINT_TCP("[%s]:SOCKET [%d] NetWrite SOCKET select error, error msg: %s\n",
                  __FUNCTION__, m_sockFd, strerror(errno) );
            return i_total > 0 ? i_total : -1;
        }

        if((i_send = send( m_sockFd, (const char *)p_data, i_data, 0)) < 0 )
        {
            return i_total > 0 ? i_total : -1;
        }
        p_data += i_send;
        i_data -= i_send;
        i_total += i_send;
    }

	return i_total;
}

int NSP_STP_CM::CTcp::RecvFrmService(char *p_data, int i_data)
{
    //const int read_data_save = i_data;
    bool b_retry = true;
    fd_set fds_r;
    fd_set fds_e;
    int i_recv;//本次接收数据
    int i_total = 0;//总接收数据
    int i_ret = 0;
    const int step_select_m = 500;//单次select 500毫秒
    const int select_count_total //总select次数
            = ((m_readTimeOut.tv_sec * 1000) + (m_readTimeOut.tv_usec / 1000)) / step_select_m;
    int select_count_now = 0;//当前select次数
    int last_err = 0;
    while( i_data > 0 )
    {
        timeval  time_out;
        while (select_count_now ++ < select_count_total)
        {
            FD_ZERO( &fds_r );
            FD_SET( static_cast<unsigned int>(m_sockFd), &fds_r );
            FD_ZERO( &fds_e );
            FD_SET( static_cast<unsigned int>(m_sockFd), &fds_e );
            time_out.tv_sec = step_select_m / 1000;
            time_out.tv_usec = (step_select_m % 1000) * 1000;
            i_ret = select(m_sockFd + 1, &fds_r, NULL, &fds_e, &time_out);
            if( i_ret > 0 )
            {
                break;
            }
            else
            {
                last_err = errno;
                if (errno == EINTR)
                {
                    select_count_now--;
                }
            }
        }

        if( i_ret <= 0 )
        {
            PRINT_TCP("[%s]SOCKET [%d] select fail, error msg;%s,select_count_now=%d, select_count_total = %d\n",
                  __FUNCTION__,m_sockFd, strerror(errno), select_count_now, select_count_total );
            return i_total > 0 ? i_total : -1;
        }

        if( ( i_recv = recv( m_sockFd, (char *)p_data, i_data, 0) ) < 0 )
        {
			last_err = WSAGetLastError();

			if( last_err == WSAEWOULDBLOCK )
			{
				/* only happens with p_vs (SSL) - not really an error */
				//套接口标识为非阻塞模式，但接收操作会产生阻塞
			}
			else if( last_err == WSAEMSGSIZE )
			{
				//数据报太大无法全部装入缓冲区，故被剪切。
				i_total += i_data;
			}
			else if( last_err == WSAEINTR ) 
			{
				//阻塞进程被WSACancelBlockingCall()取消
				continue;
			}

			PRINT_TCP("[%s]SOCKET [%d] recv fail, error code:%d,select_count_now=%d, select_count_total = %d\n",
				__FUNCTION__,m_sockFd, last_err, select_count_now, select_count_total );
			return i_total > 0 ? i_total : -1;
		}
        else if( i_recv == 0 )
        {
            b_retry = false;
        }

        p_data += i_recv;
        i_data -= i_recv;
        i_total += i_recv;
        if( !b_retry )
        {
            break;
        }
    }

	return i_total;
}

int NSP_STP_CM::CTcp::InitNet()
{
	int i_val;
	int fd = socket( m_family, m_sockType, m_protocol );
	if( fd == -1 )
	{
		if( WSAGetLastError ( ) != WSAEAFNOSUPPORT )
		{
			PRINT_TCP("InitNet:can not create socket (%i)\n", WSAGetLastError());
		}
		return -1;
	}

	unsigned long i_dummy = 1;
	if( ioctlsocket( fd, FIONBIO, &i_dummy ) != 0 )
	{
		PRINT_TCP("InitNet:can not set non-block model\n");
	}

	i_val = 1;
	setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&i_val, sizeof( i_val ) );
	return fd;
}

int NSP_STP_CM::CTcp::ConnectSocket2Server(sockaddr *p_server, int len)
{
    int i_val = -1;
    bool b_unreach = false;

    //int con_res = 
		connect( m_sockFd, (sockaddr*)p_server, len);
    //非阻塞模式connect直接返回-1
    if( 1)
    {
        timeval tv;
        int select_cnt = 5;
        do{
            fd_set fds;
            FD_ZERO( &fds );
            FD_SET( static_cast<unsigned int>(m_sockFd), &fds );
            tv.tv_sec = 0;
            tv.tv_usec = 100000;
            i_val = select( m_sockFd + 1, NULL, &fds, NULL, &tv );
            if( select_cnt <= 0 )
            {
                PRINT_TCP("[%s]:connect to %d port of %s time out\n", __FUNCTION__, m_serverPort, m_serverIp);
                DisconnectService();
                m_sockFd = errSocket;
                break;
            }
            select_cnt--;
        }while( ( i_val == 0 ) || ( ( i_val < 0 ) &&
                                    (  WSAGetLastError() == WSAEWOULDBLOCK )
                                    ) );

        if( m_sockFd == errSocket )
        {
            //connect time out
            return m_sockFd;
        }

        if( i_val <= 0 )
        {
            PRINT_TCP("[%s]: [%s-%d] select failed, abort connection\n", __FUNCTION__, m_serverIp, m_serverPort );
            DisconnectService();
            return -1;
        }

        int i_val_size = sizeof( i_val );
        if( getsockopt( m_sockFd, SOL_SOCKET, SO_ERROR, (char *)&i_val,(int *)&i_val_size ) == -1 || i_val != 0 )
        {
            if( i_val == ENETUNREACH )
            {
                b_unreach = true;
            }
            else
            {
                PRINT_TCP("[%s]:connect to port %d of host %s failed, error message:%s\n",
                      __FUNCTION__, m_serverPort, m_serverIp, strerror( i_val ));
            }
            DisconnectService();
            return -1;
        }
    }
    else
    {

    }

    if( m_sockFd == errSocket )
    {
        if( b_unreach )
        {
            PRINT_TCP("[%s]:cann't identify router path while connect to port %d of hoat %s\n",
                  __FUNCTION__, m_serverPort, m_serverIp);
        }
        return -1;
    }
    return m_sockFd;
}

void NSP_STP_CM::CTcp::SetServerIp(const char *ip)
{
    if (ip)
    {
        strcpy(m_serverIp,ip);
    }
}

void NSP_STP_CM::CTcp::SetServerPort(int port)
{
    if (port> 0)
    {
        m_serverPort = port;
    }
}

void NSP_STP_CM::CTcp::SetReadTimeOut(timeval &time_out)
{
    memcpy(&m_readTimeOut, &time_out, sizeof(time_out));
}

void NSP_STP_CM::CTcp::SetWriteTimeOut(timeval &time_out)
{
    memcpy(&m_writeTimeOut, &time_out, sizeof(time_out));
}

int NSP_STP_CM::CTcp::BindPort()
{
//	const int sleep_m = 500;
	int ret = 0;

	m_sockFd = InitNet();
	if( m_sockFd == errSocket )
	{
		PRINT_TCP("[%s]:InitNet failed\n", __FUNCTION__);
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

int NSP_STP_CM::CTcp::BindFrmServer()
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
		PRINT_TCP("[%s] bind fail\n", __FUNCTION__);
		return -1;
	}

	const int back_log = 20;
	ret = listen(m_sockFd, back_log);
	if (ret != 0) 
	{   
		PRINT_TCP("[%s] listen fail\n", __FUNCTION__);
		return -2;
	}   

	return 0;
}

void NSP_STP_CM::CTcp::CloseBind()
{
	CloseSockFd();
}

void NSP_STP_CM::CTcp::CloseSockFd()
{
	if(m_sockFd > 0)
	{
		shutdown(m_sockFd, 2);
		closesocket(m_sockFd);
		m_sockFd = errSocket;
	}
}
