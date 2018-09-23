#ifndef CSOCKETTCP_H
#define CSOCKETTCP_H

#include "itransf.h"

class CSocketTcp :
        public ITransf
{
public:
    CSocketTcp();
    virtual ~CSocketTcp();

    virtual int connect_service();
    virtual int disconnect_service();
    virtual int send_service(const char *p_data, int i_data);
    virtual int recv_service(char *p_data, int i_data);
private:
    int	InitNet(int i_family, int i_socktype, int i_protocol);
    int ConnectSocket2Server(sockaddr *p_server, int len);
public:
    friend class CSocketUnix;
public:
    int GetSockFd(){return sockFd;}
    void SetServerIp(const char* ip);
    void SetServerPort(int port);
    void SetReadTimeOut(struct timeval& time_out);
    void SetWriteTimeOut(struct timeval& time_out);
private:
    int sockFd;
    char serverIp[32];
    int serverPort;
    struct timeval  readTimeOut;//读超时
    struct timeval  writeTimeOut;//写超时
};

#endif // CSOCKETTCP_H
