#include "csocketunix.h"
#include "include_all_qt.h"
#include "Log.h"
#define TRACE qDebug

CSocketUnix::CSocketUnix()
{
    sockFd = -1;
    readTimeOut.tv_sec = 0;
    readTimeOut.tv_usec = 800 * 1000;

    writeTimeOut.tv_sec = 0;
    writeTimeOut.tv_usec = 800 * 1000;
}

CSocketUnix::~CSocketUnix()
{

}

int CSocketUnix::connect_service()
{
    int i_family = AF_UNIX;
    int i_protocol = IPPROTO_IP;
    int i_sock_type = SOCK_STREAM;
    sockFd = InitNet(i_family, i_sock_type, i_protocol);
    if( sockFd == -1 )
    {
        gLog.Add("[%s]:InitNet failed\n", __FUNCTION__);
        return sockFd;
    }

    struct sockaddr_un server;
    memset(&server, 0, sizeof(server));
    server.sun_family = i_family;
    strcpy(server.sun_path, serverIp);

    ConnectSocket2Server((struct sockaddr*)&server, sizeof(server));
    return sockFd;
}
