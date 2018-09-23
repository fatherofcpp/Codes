#pragma once

typedef unsigned char       uint8_t;
#define EINTR           4

class CNetApi
{
public:
	CNetApi();
	~CNetApi();
private:
	int	InitNet(int i_family, int i_socktype, int i_protocol);
public:
	int ConnectTCP();
	void NetClose();
	int OnSend(char *p_data, int i_data );
	int OnReceive(char *p_data, int i_data, bool b_retry = true );
public:
	int GetSockFd(){return sockFd;};
	void SetServerIp(char* ip);
	void SetServerPort(int port);
	void SetReadTimeOut(struct timeval& time_out);
	void SetWriteTimeOut(struct timeval& time_out);

private:
	int sockFd;
	char serverIp[32];
	int serverPort;
	struct timeval  readTimeOut;//¶Á³¬Ê±
	struct timeval  writeTimeOut;//Ð´³¬Ê±

};
