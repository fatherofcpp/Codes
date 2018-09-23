#ifndef _MY_SOCKET_H
#define _MY_SOCKET_H


#include "C2Htype.h"

#include "winsock.h"

#ifdef CHIP_DM642
#include <netmain.h>
#else

#ifdef __linux
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#endif

//#define INVALID_SOCKET 0xFFFFFFFF

//#define IPPROTO_IP      0               // IP Placeholder
//#define IPPROTO_ICMP    1               // ICMP
//#define IPPROTO_IGMP    2               // IGMP
//#define IPPROTO_TCP     6               // TCP
//#define IPPROTO_UDP     17              // UDP

//typedef MS_INT32 SOCKET;

#endif



MS_INT32 ConnectSocket(SOCKET *s,MS_CHAR *svr_ip,MU_INT32 svr_port,MU_INT32 timeout);
void CloseSocket(SOCKET *s);
void CreateSocket(SOCKET *s,MS_INT32 domain, MS_INT32 type, MS_INT32 protocol);

MS_INT32 CreateListenSock(SOCKET *sock, MS_INT32 domain, MS_INT32 type, MS_INT32 protocol,MU_INT32 port);
MS_INT32 ListenSock(SOCKET sock,SOCKET *subsock);
MS_INT32 RecvSocketData(SOCKET sock,MS_CHAR *buf,MS_INT32 len,MS_INT32 timeOut,MS_INT32 rtyTimes);
MS_INT32 SendSocketData(SOCKET sock, MS_CHAR *buf1, MS_INT32 len1,MS_CHAR *buf2, MS_INT32 len2,MS_INT32 timeOut,MS_INT32 rtyTimes);
MS_INT32 RecvSocketReady(SOCKET sock);
MS_INT32 SendSocketReady(SOCKET sock);
MS_INT32 CreateListenSockUDP(SOCKET *sock,MU_INT32 port);

MS_INT32 RecvSocketDataUDP(SOCKET sock,MS_CHAR *buf,MS_INT32 len);
MS_INT32 SendSocketDataUDP(SOCKET sock, MS_CHAR *buf, MS_INT32 len,MS_CHAR *svr_ip,MU_INT32 svr_port);

#endif

