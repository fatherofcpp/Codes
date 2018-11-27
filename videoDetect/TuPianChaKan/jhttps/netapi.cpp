#include "stdafx.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>




#include "netapi.h"
#include<windows.h>
#include<winSock.h>


/*
static void printf(const char* fmt, ...)
{
	char pBuff[500];
#ifdef WIN32	
	if (!AfxIsValidString(fmt, -1))
		return ;
	try{
#endif
		va_list argptr;          //�����ַ�ĸ��?
		va_start(argptr, fmt);
		vsnprintf(pBuff, 500, fmt, argptr);
		va_end(argptr);
		Error("%s\n", pBuff);
#ifdef WIN32	
	}
	catch (...){
		pBuff[0] = 0;
	}	
#endif
}
*/

int vasprintf(char **strp, const char *fmt, va_list ap)
{
    size_t     i_size = 100;
    char    *p = (char *)malloc( i_size );
    int     n;

    if( p == NULL ){
        *strp = NULL;
        return -1;
    }

    for( ;; ){
        n = vsnprintf( p, i_size, fmt, ap );

        if (n > -1 && n < i_size){
            *strp = p;
            return strlen( p );
        }
        if (n > -1)    
           i_size = n+1; 
        else           
           i_size *= 2;  
        if( (p = (char *)realloc( p, i_size ) ) == NULL){
            *strp = NULL;
            return -1;
        }
    }
}

int InitNet(int i_family, int i_socktype, int i_protocol)
{
	int i_val;
	int fd = socket( i_family, i_socktype, i_protocol );
    if( fd == -1 ){
#ifdef WIN32 
        if( WSAGetLastError ( ) != WSAEAFNOSUPPORT )
            printf("InitNet:can not create socket (%i)\n", WSAGetLastError());
#else
        if( errno != EAFNOSUPPORT )
            printf("InitNet:can no create socket (%s)\n", strerror(errno));
#endif
        return -1;
    }

#ifdef WIN32 
    unsigned long i_dummy = 1;
    if( ioctlsocket( fd, FIONBIO, &i_dummy ) != 0 )
        printf("InitNet:can not set non-block model\n");
#else
    if( ( ( i_val = fcntl( fd, F_GETFL, 0 ) ) < 0 ) ||
        ( fcntl( fd, F_SETFL, i_val | O_NONBLOCK ) < 0 ) )
			printf("InitNet:can not set non-block model (%s)\n", strerror(errno));
#endif

    i_val = 1;
    setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&i_val, sizeof( i_val ) );

#ifdef IPV6_V6ONLY
    if( i_family == AF_INET6 )
        setsockopt( fd, IPPROTO_IPV6, IPV6_V6ONLY, (const char *)&i_val, sizeof( i_val ) );
#endif

#ifdef WIN32 
	#ifndef IPV6_PROTECTION_LEVEL
		#define IPV6_PROTECTION_LEVEL 23
	#endif
//    if( i_family == AF_INET6 )
	{
 //       i_val = 30 ;
 //       setsockopt( fd, IPPROTO_IPV6, IPV6_PROTECTION_LEVEL, (const char*)&i_val, sizeof( i_val ) );
    }
#endif
    return fd;
}

void NetClose( int fd )
{
#ifdef WIN32
    shutdown(fd, 2);
    closesocket(fd);
#else
    close(fd);
#endif
}

int ConnectTCP(const char *psz_host, int i_port)
{
	int fd;
	int i_val, i_handle = -1;
	bool b_unreach = false;
/*        
   struct hostent *hp;
*/ 
       
  fd = InitNet(AF_INET, SOCK_STREAM, 0);
  if( fd == -1 )
      return fd;

/*	
	hp = gethostbyname(psz_host);
	if (hp == NULL){
		NetClose(fd);
		return -1;
	}
*/
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(psz_host) /* *((unsigned long*)hp->h_addr)*/;
	server.sin_port = htons(i_port);	

   if( connect( fd, (struct sockaddr*)&server, sizeof(server)))
	{
        int i_val_size = sizeof( i_val );
        div_t d;
        struct timeval tv;

#if defined( WIN32 ) 
        if( WSAGetLastError() != WSAEWOULDBLOCK )
			{
            if( WSAGetLastError () == WSAENETUNREACH )
                b_unreach = true;
            else
                printf("[ConnectTCP]:connect %d port of %s failed, (%d)\n", psz_host, i_port, WSAGetLastError( ) );
            NetClose( fd );
        }
#else
			if( errno != EINPROGRESS )
			{
            if( errno == ENETUNREACH )
                b_unreach = true;
            else
                printf("[ConnectTCP]:connect %d port of %s failed, (%s)\n", i_port,psz_host, strerror( errno ) );
            NetClose( fd );
      	}
#endif
        d = div( 5*1000, 100 );
        printf("[ConnectTCP]:beginning to connect to %d port of %s ......\n", i_port, psz_host);
        do{
            fd_set fds;
            FD_ZERO( &fds );
            FD_SET( fd, &fds );
            tv.tv_sec = 0;
            tv.tv_usec = (d.quot > 0) ? 100000 : (1000 * d.rem);
            i_val = select( fd + 1, NULL, &fds, NULL, &tv );
            if( d.quot <= 0 )
					{
                printf("[ConnectTCP]:connect to %d port of %s time out\n", i_port, psz_host);
                NetClose( fd );
                fd = -1;
                break;
            }
            d.quot--;
				}while( ( i_val == 0 ) || ( ( i_val < 0 ) &&
#if defined( WIN32 )
	        ( WSAGetLastError() == WSAEWOULDBLOCK )
#else
            ( errno == EINTR )
#endif
            ) );

        if( fd == -1 )
            return fd; //���ӳ�ʱ

        if( i_val < 0 )
			{
            printf("[ConnectTCP]: [%s-%d] select failed, abort connection\n", psz_host, i_port );
            NetClose( fd );
            return -1;
        }
#if !defined( SYS_BEOS ) && !defined( UNDER_CE )
        if( getsockopt( fd, SOL_SOCKET, SO_ERROR, (char *)&i_val,(int *)&i_val_size ) == -1 || i_val != 0 )
			{
            if( i_val == ENETUNREACH )
                b_unreach = true;
            else
            {
#ifdef WIN32
                printf("[ConnectTCP]:connect port %d of hoat %s failed, error code:%d \n", psz_host, i_port, WSAGetLastError( ));
#else
                printf("[ConnectTCP]:connect to port %d of host %s failed, error message:%s\n", i_port, psz_host, strerror( i_val ));
#endif
            }
            NetClose( fd );
            return -1;
        }
#endif
    }
	i_handle = fd; 
	if( i_handle == -1 )
	{
	  if( b_unreach )
            printf("[ConnectTCP]:cann't identify router path while connect to port %d of hoat %s\n", i_port, psz_host);
	  return -1;
	}
	return i_handle;
}
#if 0
int NetPrintf(int fd, const char *psz_fmt, ... )
{
    int i_ret;
    va_list args;
    va_start( args, psz_fmt );
    i_ret = NetVarPrintf(fd, psz_fmt, args );
    va_end( args );

    return i_ret;
}

int NetVarPrintf(int fd, const char *psz_fmt, va_list args)
{
    char    *psz;
    int     i_size, i_ret;

    i_size = vasprintf( &psz, psz_fmt, args );
    i_ret = NetWrite(fd, (uint8_t *)psz, i_size ) < i_size ? -1 : i_size;
    free( psz );

    return i_ret;
}
#endif
int NetWrite(int fd, const uint8_t *p_data, int i_data )
{
	struct timeval  timeout;
	fd_set          fds_w, fds_e;
	int             i_send;
	int             i_total = 0;
	int             i_ret;
   while( i_data > 0 )
	{
        do{
            FD_ZERO( &fds_w );
            FD_SET( fd, &fds_w );
            FD_ZERO( &fds_e );
            FD_SET( fd, &fds_e );

            timeout.tv_sec = 0;
            timeout.tv_usec = 500000;

        } while((i_ret = select(fd + 1, NULL, &fds_w, &fds_e, &timeout)) == 0 || ( i_ret < 0 && errno == EINTR ));
        if( i_ret < 0 )
			{
#if defined(WIN32)
            printf("[NetWrite]:SOCKET [%d] NetWrite SOCKET select error, error code: %d\n", fd, WSAGetLastError() );
#else
            printf("[NetWrite]:SOCKET [%d] NetWrite SOCKET select error, error message: %s\n", fd, strerror(errno) );
#endif
            return i_total > 0 ? i_total : -1;
        }
        if((i_send = send( fd, (const char *)p_data, i_data, 0)) < 0 )
			{
            return i_total > 0 ? i_total : -1;
			}
        p_data += i_send;
        i_data -= i_send;
        i_total+= i_send;
    }
    return i_total;
}

//������������ͷ��ڴ�?
char *NetGets(int fd, int *iLen)
{
    char *psz_line = NULL, *ptr = NULL;
    size_t  i_line = 0, i_max = 0;
		char r='a';

    for( ;; )
	 {
        if( i_line == i_max )
			{
            i_max += 1024;
            psz_line = (char *)realloc( psz_line, i_max );
            ptr = psz_line + i_line;
        }

        if( NetRead(fd, (uint8_t *)ptr, 1, true ) != 1 )
			{
            if( i_line == 0 )
					{
                free( psz_line );
                return NULL;
            }
            break;
        }
        if ((r == '\r')&&(*ptr == '\n'))
				break;

			r = *ptr;
        i_line++;
        ptr++;
    }
    *ptr-- = '\0';
    if( ( ptr >= psz_line ) && ( *ptr == '\r' ) )
        *ptr = '\0';

	 *iLen = i_line;
    return psz_line;
}

int NetRead(int fd, uint8_t *p_data, int i_data, bool b_retry )
{
    struct timeval  timeout;
    fd_set          fds_r, fds_e;
    int             i_recv;
    int             i_total = 0;
    int             i_ret;

    while( i_data > 0 )
	 {
        do{
            FD_ZERO( &fds_r );
            FD_SET( fd, &fds_r );
            FD_ZERO( &fds_e );
            FD_SET( fd, &fds_e );

            timeout.tv_sec = 0;
            timeout.tv_usec = 500000;

        } while( (i_ret = select(fd + 1, &fds_r, NULL, &fds_e, &timeout)) == 0 || ( i_ret < 0 && errno == EINTR ) );

        if( i_ret < 0 ){
#if defined(WIN32)
            printf("SOCKET [%d] NetRead:SOCKET select error, error code;%d\n", fd, WSAGetLastError() );
#else
            printf("SOCKET [%d] NetRead:SOCKET select error, error message;%s\n", fd, strerror(errno) );
#endif
            return i_total > 0 ? i_total : -1;
        }

        if( ( i_recv = recv( fd, (char *)p_data, i_data, 0) ) < 0 )
        {
#if defined(WIN32) || defined(UNDER_CE)
            if( WSAGetLastError() == WSAEWOULDBLOCK )
            {
                /* only happens with p_vs (SSL) - not really an error */
            }
            else
            if( WSAGetLastError() == WSAEMSGSIZE ){
//                PrintfInfo("NetRead:����ʧ��. ����mtu ֵ\n" );
                i_total += i_data;
            }
            else if( WSAGetLastError() == WSAEINTR ) continue;
//            else PrintfInfo("SOCKET���?%d,NetRead:����ʧ�� (%i)\n", fd, WSAGetLastError() );
#else
            /* EAGAIN only happens with p_vs (TLS) and it's not an error */
            if( errno != EAGAIN )
                printf("SOCKET [%d] NetRead failed, error message:%s\n", fd, strerror(errno) );
#endif
            return i_total > 0 ? i_total : -1;
        }
        else if( i_recv == 0 )
			 {
            b_retry = false;
         }

        p_data += i_recv;
        i_data -= i_recv;
        i_total+= i_recv;
        if( !b_retry )
        	break;
    }
    return i_total;
}


