#include "stdafx.h"
#include "netapi.h"
#include "log.h"
#include "xmlAnalyze.h"
#include "dataConvert.h"

int CNetApi::InitNet(int i_family, int i_socktype, int i_protocol)
{
	int i_val;
	int fd = socket( i_family, i_socktype, i_protocol );
	if( fd == -1 )
	{
		if( WSAGetLastError ( ) != WSAEAFNOSUPPORT )
		{
			TRACE("InitNet:can not create socket (%i)\n", WSAGetLastError());
		}
		return -1;
	}

	unsigned long i_dummy = 1;
	if( ioctlsocket( fd, FIONBIO, &i_dummy ) != 0 )
	{
		TRACE("InitNet:can not set non-block model\n");
	}

	i_val = 1;
	setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&i_val, sizeof( i_val ) );
	return fd;
}

void CNetApi::NetClose()
{
	if (sockFd > 0)
	{
		shutdown(sockFd, 2);
		closesocket(sockFd);
		sockFd = -1;
		TRACE("[%s]\n", __FUNCTION__);
	}
}

int CNetApi::ConnectTCP()
{
	TRACE("[%s]\n", __FUNCTION__);
	int i_val, i_handle = -1;
	bool b_unreach = false;

	sockFd = InitNet(AF_INET, SOCK_STREAM, 0);
	if( sockFd == -1 )
	{
		TRACE("[ConnectTCP]:InitNet failed, (%d)\n", WSAGetLastError( ) );
		return sockFd;
	}

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIp) /* *((unsigned long*)hp->h_addr)*/;
	server.sin_port = htons(serverPort);	

	int con_res = connect( sockFd, (struct sockaddr*)&server, sizeof(server));
	//非阻塞模式connect直接返回-1
	if( 1)
	{
		int i_val_size = sizeof( i_val );
		div_t d;
		struct timeval tv;

		if( WSAGetLastError() != WSAEWOULDBLOCK )
		{
			if( WSAGetLastError () == WSAENETUNREACH )
			{
				b_unreach = true;
			}
			else
			{
				TRACE("[ConnectTCP]:connect %d port of %s failed, (%d)\n", serverPort,serverIp, WSAGetLastError( ) );
			}
			NetClose();
		}

		d = div( 5 * 100, 100 );
		//TRACE("[ConnectTCP]:beginning to connect to %d port of %s ......\n", serverPort, serverIp);
		do{
			fd_set fds;
			FD_ZERO( &fds );
			FD_SET( sockFd, &fds );
			tv.tv_sec = 0;
			tv.tv_usec = (d.quot > 0) ? 100000 : (1000 * d.rem);
			i_val = select( sockFd + 1, NULL, &fds, NULL, &tv );
			if( d.quot <= 0 )
			{
				TRACE("[ConnectTCP]:connect to %d port of %s time out\n", serverPort, serverIp);
				NetClose();
				sockFd = -1;
				break;
			}
			d.quot--;
		}while( ( i_val == 0 ) || ( ( i_val < 0 ) &&
			( WSAGetLastError() == WSAEWOULDBLOCK )
			) );

		if( sockFd == -1 )
		{
			TRACE("[ConnectTCP]2:failed\n");
			return sockFd; 
		}

		if( i_val < 0 )
		{
			TRACE("[ConnectTCP]: [%s-%d] select failed, abort connection\n", serverIp, serverPort );
			NetClose();
			return -1;
		}

		if( getsockopt( sockFd, SOL_SOCKET, SO_ERROR, (char *)&i_val,(int *)&i_val_size ) == -1 || i_val != 0 )
		{
			if( i_val == ENETUNREACH )
			{
				b_unreach = true;
			}
			else
			{
				TRACE("[ConnectTCP]:connect port %d of hoat %s failed, error code:%d \n", serverIp, serverPort, WSAGetLastError( ));
			}
			NetClose();
			return -1;
		}

	}
	else
	{

	}

	i_handle = sockFd; 
	if( i_handle == -1 )
	{
		if( b_unreach )
		{
			TRACE("[ConnectTCP]:cann't identify router path while connect to port %d of hoat %s\n", serverPort, serverIp);
		}
		return -1;
	}
	return i_handle;
}

int CNetApi::OnSend(char *p_data, int i_data )
{
	fd_set fds_w, fds_e;
	int i_send;//本次发送数据
	int i_total = 0;//总发送数据
	int i_ret;
	const int step_select_m = 500;//单次select 500毫秒
	const int select_count_total //总select次数
		= ((writeTimeOut.tv_sec * 1000) + (writeTimeOut.tv_usec / 1000)) / step_select_m;		
	int select_count_now = 0;//当前select次数
	while( i_data > 0 )
	{
		struct timeval  time_out;
		time_out.tv_sec = step_select_m / 1000;
		time_out.tv_usec = step_select_m * 1000;

		while (select_count_now ++ < select_count_total)
		{
			FD_ZERO( &fds_w );
			FD_SET( sockFd, &fds_w );
			FD_ZERO( &fds_e );
			FD_SET( sockFd, &fds_e );
			i_ret = select(sockFd + 1, NULL, &fds_w, &fds_e, &time_out);
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
			TRACE("[NetWrite]:SOCKET [%d] NetWrite SOCKET select error, error code: %d\n", sockFd, WSAGetLastError() );
			return i_total > 0 ? i_total : -1;
		}
		if((i_send = send( sockFd, (const char *)p_data, i_data, 0)) < 0 )
		{
			return i_total > 0 ? i_total : -1;
		}
		p_data += i_send;
		i_data -= i_send;
		i_total += i_send;
	}
	return i_total;
}

int CNetApi::OnReceive(char *p_data, int i_data, bool b_retry )
{
	fd_set fds_r;
	fd_set fds_e;
	int i_recv;//本次接收数据
	int i_total = 0;//总接收数据
	int i_ret = 0;
	const int step_select_m = 500;//单次select 500毫秒
	const int select_count_total //总select次数
		= ((readTimeOut.tv_sec * 1000) + (readTimeOut.tv_usec / 1000)) / step_select_m;		
	int select_count_now = 0;//当前select次数
	int last_err = 0;
	while( i_data > 0 )
	{
		struct timeval  time_out;
		time_out.tv_sec = step_select_m / 1000;
		time_out.tv_usec = (step_select_m % 1000) * 1000;
		
		while (select_count_now ++ < select_count_total)
		{
			FD_ZERO( &fds_r );
			FD_SET( sockFd, &fds_r );
			FD_ZERO( &fds_e );
			FD_SET( sockFd, &fds_e );
			i_ret = select(sockFd + 1, &fds_r, NULL, &fds_e, &time_out);
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
			last_err = WSAGetLastError();
			TRACE("[%s]SOCKET [%d] select fail, error code;%d,select_count_now=%d, select_count_total = %d\n",
				__FUNCTION__,sockFd, last_err, select_count_now, select_count_total );
			return i_total > 0 ? i_total : -1;
		}

		if( ( i_recv = recv( sockFd, (char *)p_data, i_data, 0) ) < 0 )
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
			TRACE("[%s]SOCKET [%d] recv fail, error code;%d,select_count_now=%d, select_count_total = %d\n",
				__FUNCTION__,sockFd, last_err, select_count_now, select_count_total );
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


CNetApi::CNetApi()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int	err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	assert(err == 0);

	sockFd = -1;
	readTimeOut.tv_sec = 0;
	readTimeOut.tv_usec = 500 * 1000;

	writeTimeOut.tv_sec = 0;
	writeTimeOut.tv_usec = 500 * 1000;
}

CNetApi::~CNetApi()
{
	NetClose();
}

void CNetApi::SetServerIp( char* ip )
{
	if (ip)
	{
		strcpy(serverIp,ip);
	}
}

void CNetApi::SetServerPort( int port )
{
	if (port)
	{
		serverPort = port;
	}
}

void CNetApi::SetReadTimeOut( struct timeval& time_out )
{
	memcpy(&readTimeOut, &time_out, sizeof(time_out));
}

void CNetApi::SetWriteTimeOut( struct timeval& time_out )
{
	memcpy(&writeTimeOut, &time_out, sizeof(time_out));
}



