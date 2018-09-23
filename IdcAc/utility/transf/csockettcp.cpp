#include "csockettcp.h"
#include "include_all_qt.h"
#include "Log.h"
#define TRACE qDebug

CSocketTcp::CSocketTcp()
{
    sockFd = -1;
    readTimeOut.tv_sec = 0;
    readTimeOut.tv_usec = 500 * 1000;

    writeTimeOut.tv_sec = 0;
    writeTimeOut.tv_usec = 500 * 1000;
}

CSocketTcp::~CSocketTcp()
{
}

int CSocketTcp::connect_service()
{
    int i_family = AF_INET;
    int i_protocol = IPPROTO_IP;//IPPROTO_UDP
    int i_sock_type = SOCK_STREAM;
    sockFd = InitNet(i_family, i_sock_type, i_protocol);
    if( sockFd == -1 )
    {
        gLog.Add("[%s]:InitNet failed\n", __FUNCTION__);
        return sockFd;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = i_family;
    server.sin_addr.s_addr = inet_addr(serverIp) /* *((unsigned long*)hp->h_addr)*/;
    server.sin_port = htons(serverPort);

    ConnectSocket2Server((struct sockaddr*)&server, sizeof(server));
    return sockFd;
}

int CSocketTcp::disconnect_service()
{
    if(sockFd > 0)
    {
        close(sockFd);
        sockFd = -1;
    }
    return 0;
}

int CSocketTcp::send_service(const char *p_data, int i_data)
{
    int send_data_save = i_data;
    fd_set fds_w, fds_e;
    int i_send;//本次发送数据
    int i_total = 0;//总发送数据
    int i_ret = 0;
    const int step_select_m = 500;//单次select 500毫秒
    const int select_count_total //总select次数
            = ((writeTimeOut.tv_sec * 1000) + (writeTimeOut.tv_usec / 1000)) / step_select_m;
    int select_count_now = 0;//当前select次数
    while( i_data > 0 )
    {
        struct timeval  time_out;
        while (select_count_now ++ < select_count_total)
        {
            FD_ZERO( &fds_w );
            FD_SET( sockFd, &fds_w );
            FD_ZERO( &fds_e );
            FD_SET( sockFd, &fds_e );
            time_out.tv_sec = step_select_m / 1000;
            time_out.tv_usec = step_select_m * 1000;
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
            gLog.Add("[%s]:SOCKET [%d] NetWrite SOCKET select error, error msg: %s\n",
                     __FUNCTION__, sockFd, strerror(errno) );
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
    return (i_total - send_data_save);
}

int CSocketTcp::recv_service(char *p_data, int i_data)
{
    int read_data_save = i_data;
    bool b_retry = true;
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
        while (select_count_now ++ < select_count_total)
        {
            FD_ZERO( &fds_r );
            FD_SET( sockFd, &fds_r );
            FD_ZERO( &fds_e );
            FD_SET( sockFd, &fds_e );
            time_out.tv_sec = step_select_m / 1000;
            time_out.tv_usec = (step_select_m % 1000) * 1000;
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
            gLog.Add("[%s]SOCKET [%d] select fail, error msg;%s,select_count_now=%d, select_count_total = %d\n",
                     __FUNCTION__,sockFd, strerror(errno), select_count_now, select_count_total );
            return i_total > 0 ? i_total : -1;
        }

        if( ( i_recv = recv( sockFd, (char *)p_data, i_data, 0) ) < 0 )
        {
            if( errno != EAGAIN )
            {
                gLog.Add("[%s]SOCKET [%d] recv fail, error msg;%s,select_count_now=%d, select_count_total = %d\n",
                         __FUNCTION__,sockFd, strerror(errno), select_count_now, select_count_total );
            }
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
    return (i_total - read_data_save);
}

int CSocketTcp::InitNet(int i_family, int i_socktype, int i_protocol)
{
    int i_val;
    int fd = socket( i_family, i_socktype, i_protocol );
    if( fd == -1 )
    {
        if( errno != EAFNOSUPPORT )
        {
            gLog.Add("[%s]can no create socket (%s)\n",__FUNCTION__, strerror(errno));
        }
        return -1;
    }

    if( ( ( i_val = fcntl( fd, F_GETFL, 0 ) ) < 0 ) ||
            ( fcntl( fd, F_SETFL, i_val | O_NONBLOCK ) < 0 ) )
        gLog.Add("[%s]:can not set non-block model (%s)\n",__FUNCTION__, strerror(errno));

    i_val = 1;
    setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&i_val, sizeof( i_val ) );
    return fd;
}

int CSocketTcp::ConnectSocket2Server(sockaddr *p_server, int len)
{
    int i_val = -1;
    bool b_unreach = false;

    int con_res = connect( sockFd, (struct sockaddr*)p_server, len);
    //非阻塞模式connect直接返回-1
    if( 1)
    {
        struct timeval tv;
        int select_cnt = 5;
        do{
            fd_set fds;
            FD_ZERO( &fds );
            FD_SET( sockFd, &fds );
            tv.tv_sec = 0;
            tv.tv_usec = 100000;
            i_val = select( sockFd + 1, NULL, &fds, NULL, &tv );
            if( select_cnt <= 0 )
            {
                gLog.Add("[%s]:connect to %d port of %s time out\n", __FUNCTION__, serverPort, serverIp);
                disconnect_service();
                sockFd = -1;
                break;
            }
            select_cnt--;
        }while( ( i_val == 0 ) || ( ( i_val < 0 ) &&
                                    ( errno == EINTR )
                                    ) );

        if( sockFd == -1 )
        {
            //connect time out
            return sockFd;
        }

        if( i_val <= 0 )
        {
            gLog.Add("[%s]: [%s-%d] select failed, abort connection\n", __FUNCTION__, serverIp, serverPort );
            disconnect_service();
            return -1;
        }

        int i_val_size = sizeof( i_val );
        if( getsockopt( sockFd, SOL_SOCKET, SO_ERROR, (char *)&i_val,(socklen_t *)&i_val_size ) == -1 || i_val != 0 )
        {
            if( i_val == ENETUNREACH )
            {
                b_unreach = true;
            }
            else
            {
                gLog.Add("[%s]:connect to port %d of host %s failed, error message:%s\n",
                         __FUNCTION__, serverPort, serverIp, strerror( i_val ));
            }
            disconnect_service();
            return -1;
        }
    }
    else
    {

    }

    if( sockFd == -1 )
    {
        if( b_unreach )
        {
            gLog.Add("[%s]:cann't identify router path while connect to port %d of hoat %s\n",
                     __FUNCTION__, serverPort, serverIp);
        }
        return -1;
    }
    return sockFd;
}

void CSocketTcp::SetServerIp(const char *ip)
{
    if (ip)
    {
        strcpy(serverIp,ip);
    }
}

void CSocketTcp::SetServerPort(int port)
{
    if (port> 0)
    {
        serverPort = port;
    }
}

void CSocketTcp::SetReadTimeOut(timeval &time_out)
{
    memcpy(&readTimeOut, &time_out, sizeof(time_out));
}

void CSocketTcp::SetWriteTimeOut(timeval &time_out)
{
    memcpy(&writeTimeOut, &time_out, sizeof(time_out));
}
