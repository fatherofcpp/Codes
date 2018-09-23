#include "stdafx.h"
#include "../include/mysocket.h"
//#include "msginfo.h"
#include "../../include/dev_init.h"
#include <WinSock.h>
#pragma comment(lib,"WS2_32.LIB")
#define myLOGprint LogPrintf

#define MSG_NOSIGNAL	0

#define PACK_SIZE		(1024*1024)//1440
#define DEF_MAX_MSG_LENGHT 256

//#include <sys/ioctls.h>

//#include "../../slib/src/msysio.h"
//#include "../../slib/src/mnetio.h"
//#include <netdb.h>
#define SAFECLOSE(p)		{if (p!=INVALID_SOCKET) {closesocket(p);p=INVALID_SOCKET;}}

MS_INT32 ConnectSocket(SOCKET *s,MS_CHAR *svr_name,MU_INT32 svr_port,MU_INT32 connectout)
{


    struct timeval timeout;
    struct sockaddr_in srv;
	fd_set obits;
	int ret;
	unsigned long bDontLinger;
		int nZero=64*1024;		//不设置发送缓冲区	
	int nNetTimeout=10000; //设置接收超时限制
	int nRecvBuf=64*1024;//设置接收缓冲区为32K

	SAFECLOSE(*s)


	*s=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//IPPROTO_IP);////IPPROTO_IP);//);//IPPROTO_TCP);

	if (*s ==INVALID_SOCKET)
		return MD_FAILED;

  	ZeroMemory(&srv,sizeof(struct sockaddr_in) );
    srv.sin_family = AF_INET;
    //srv.sin_len    = sizeof(srv);
    srv.sin_addr.s_addr = inet_addr(svr_name);
    srv.sin_port   = htons(svr_port);

	bDontLinger = 1;
	//#define SO_CONNECT_TIME 0x700C
    //setsockopt( m_socket, SOL_SOCKET, SO_DONTLINGER,(const char *)&bDontLinger, sizeof( BOOL ));

    setsockopt( *s, SOL_SOCKET, SO_REUSEADDR,(const char *)&bDontLinger, sizeof( BOOL ));

	//bDontLinger = FALSE;

	bDontLinger = 1;

	ioctlsocket(*s, FIONBIO, (unsigned   long*)&bDontLinger);

	


	//setsockopt( m_socket, SOL_SOCKET, SO_CONNECT_TIME,(const char *)&bDontLinger, sizeof( BOOL ));

	{

	setsockopt(*s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));


	setsockopt(*s,SOL_SOCKET,SO_SNDBUF,(char *)&nZero,sizeof(nZero));


	setsockopt(*s,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
	}
   //tmp = 0;
    //setsockopt( s, SOL_SOCKET, SO_BLOCKING, &tmp, sizeof( tmp ) );

#if 1
	connect( *s, (struct sockaddr*)&srv, sizeof(srv) );

	//if ( connect( m_socket, (sockaddr*)&srv, sizeof(srv) ) == SOCKET_ERROR )
	//{
	//	return FALSE;
	//}

    timeout.tv_sec  = 5;//2;
	timeout.tv_usec = 0;//500000;
	
	FD_ZERO(&obits);
	FD_SET(*s,&obits);

	ret=WSAGetLastError();

	if (ret == WSAEISCONN)
		return MD_OK;

	//ret = select(0, NULL, &obits, NULL, &timeout );
	ret = select(*s+1, NULL, &obits, NULL, &timeout );

	if( !ret ) {

		//TRACE("Connect to Server timeout!\n");

		SAFECLOSE(*s)

		return MD_FAILED;


	   }
	//else
		//TRACE("Connectted to Server\n");

	bDontLinger = 0;

	ioctlsocket(*s, FIONBIO, (unsigned   long*)&bDontLinger); 

	{
	//int nRecvBuf=64*1024;//设置接收缓冲区为32K
	//setsockopt(*s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));

	//int nZero=64*1024;		//不设置发送缓冲区
	//setsockopt(*s,SOL_SOCKET,SO_SNDBUF,(char *)&nZero,sizeof(nZero));

	int nNetTimeout=3000; //设置接收超时限制
	setsockopt(*s,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
	}

	return MD_OK;

	
#else    
	if ( connect( m_socket, (sockaddr*)&srv, sizeof(srv) ) == SOCKET_ERROR )
	{
	    
	    timeout.tv_sec  = 0;//2;
		timeout.tv_usec = 500000;
		
		FD_ZERO(&obits);
		FD_SET(m_socket,&obits);

		ret=WSAGetLastError();

		if (ret == WSAEISCONN)
			return TRUE;

		ret = select((int)m_socket, NULL, &obits, NULL, &timeout );
		if( !ret ) {
			if (my_TRACE != NULL)
				my_TRACE("Connect to Server timeout!\n");
			else
				TRACE("Connect to Server timeout!\n");

		   }
		SAFECLOSE(m_socket)
	}
	//else
		;//ioctlsocket(m_socket, FIONBIO, (unsigned   long*)&bDontLinger); 

	return TRUE;

#endif
           
}


void CloseSocket(SOCKET *s)
{
	if (*s != (MS_INT32)INVALID_SOCKET)
		closesocket(*s);

	*s=INVALID_SOCKET;
}

void CreateSocket(SOCKET *s,MS_INT32 domain, MS_INT32 type, MS_INT32 protocol)
{

	SOCKET sock;

	sock = socket(domain, type, protocol);//socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//
	
	*s = sock;


}

MS_INT32 CreateListenSockUDP(SOCKET *sock,MU_INT32 port)
{


    return MD_OK;
    
}

MS_INT32 CreateListenSock(SOCKET *sock, MS_INT32 domain, MS_INT32 type, MS_INT32 protocol,MU_INT32 port )
{


	return MD_OK;

}

MS_INT32 ListenSock(SOCKET sock,SOCKET *subsock)
{

	return MD_OK;

}


//#define DEF_RETRY_NUMS	1000


MS_INT32 RecvSocketData(SOCKET sock,MS_CHAR *buf,MS_INT32 len,MS_INT32 timeOut,MS_INT32 times)
{

#if 0

	MS_INT32 retlen;
	
	//retlen = MSI_LNread((MS_INT32)sock, (void * M_RESTRICT)buf,len);
	//retlen = MSI_Oread((MS_INT32)sock, (void * M_RESTRICT)buf,len);
	retlen = MSI_WLread((MS_INT32)sock, (void * M_RESTRICT)buf,len,100,10);//(MS_INT32 fd, void * M_RESTRICT d, MU_INT32 len, MU_INT32 wloop, MU_INT32 wtime);
	 		
	//if (retlen<0)
	if (retlen!=len)
	{
		//myLOGprint("Recv dat to socket err \n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}	
	return retlen;


#else

 	struct  timeval timeout;    
	fd_set ibits, obits, xbits;
	MS_INT32 ircv = 0, cnt, err,retryNums=times;//DEF_RETRY_NUMS;
	MS_CHAR msg[DEF_MAX_MSG_LENGHT];
	//retryNums = times/1000;
	
	if (retryNums == 0)
		retryNums = 1;		

	while(len > 0 && retryNums >0)
	{	
		FD_ZERO(&ibits);
		FD_ZERO(&obits);
		FD_ZERO(&xbits);    
		FD_SET(sock, &ibits);
		timeout.tv_sec  = timeOut/1000;//3;//0;
        timeout.tv_usec = (timeOut%1000)*1000;//1000;//500000; 	  
 
		err = select(sock+1, &ibits, NULL, NULL, &timeout );
		
		if (err == 0)
		{
		
			retryNums --;//=DEF_RETRY_NUMS	
			
			continue;	
		
		}
		if(err <= 0 ) 				
		{
			sprintf(msg,"Receive dat from socket err \n");//fdError());

		//	//myLOGprint(msg,DEF_MSG_PRINT|DEF_MSG_SAVE,0);

			return MD_FAILED;//err;//
		}
		else 
		{	
		    if( FD_ISSET(sock, &ibits))
		    {
				cnt = (int)recv( sock, buf + ircv, len, MSG_NOSIGNAL );
				if( cnt <= 0 )
				{	

					sprintf(msg,"Receive socket err %d\n",errno);//fdError());

		//			//myLOGprint(msg,DEF_MSG_PRINT|DEF_MSG_SAVE,0);

					return MD_FAILED;
	
				}		
				len		-=	cnt;
				ircv	+=	cnt;
			}
		}

	}

	if (len != 0)
	{
	    sprintf(msg,"Recv dat timeout %d\n",errno);//fdError());

	//	//myLOGprint(msg,DEF_MSG_PRINT|DEF_MSG_SAVE,0);
		
		return MD_FAILED;
	}
		
	return ircv;
	
#endif


}


MS_INT32 RecvSocketReady(SOCKET sock)
{
 	struct  timeval timeout;    
	fd_set ibits, obits, xbits;
	MS_INT32 err;
		
	MS_CHAR msg[DEF_MAX_MSG_LENGHT];
	
	FD_ZERO(&ibits);
	FD_ZERO(&obits);
	FD_ZERO(&xbits);    
	FD_SET(sock, &ibits);
	timeout.tv_sec  = 0;
    timeout.tv_usec = 0;//500000; 	  

	err = select(sock+1, &ibits, NULL, NULL, &timeout );
	if(err < 0 ) 				
	{
		sprintf(msg,"RecvSocketReady err \n");//fdError());
	
	//	//myLOGprint(msg,DEF_MSG_PRINT,0);
		return MD_FAILED;//err;//
	}
	else 
	{	
	    if( FD_ISSET(sock, &ibits))
	    {
			//cnt = (int)recv( sock, buf, 1, 0 );
			//if( cnt < 0 )
			//{	
			//	//myLOGprint("Receive dat from socket err \n",0);

			//	return MD_FAILED;

			//}
			//else
			return MD_OK;		
		}
		else
			return MD_BUSY;
	}


	//return MD_OK;
}




MS_INT32 SendSocketData(SOCKET sock, MS_CHAR *buf1, MS_INT32 len1,MS_CHAR *buf2, MS_INT32 len2,MS_INT32 timeOut,MS_INT32 times)
{

#if 0

	MS_CHAR msg[DEF_MAX_MSG_LENGHT];
	IOVEC iov[2];
	
	MS_INT32 retlen;
	
	iov[0].iov_base = buf1;
	iov[0].iov_len 	= len1;
	
	iov[1].iov_base = buf2;
	iov[1].iov_len	= len2;
	
	//retlen = MSI_LNwritev((MS_INT32)sock, &iov[0], 2);
	//retlen = MSI_WLAwritev((MS_INT32)sock, &iov[0], 2, 50, 10);
	retlen = MSI_WLwritev((MS_INT32)sock, &iov[0], 2, timeOut, times);
	
	
	if (retlen<0)// || retLen != len1+len2)
	{
		sprintf(msg,"Send data socket err \n");//fdError());
	
		//myLOGprint(msg,DEF_MSG_PRINT,0);

		return MD_FAILED;
	}
	
	return retlen;


#else

	struct  timeval timeout;    
	fd_set ibits, obits, xbits;
	MS_INT32 sent = 0, cnt, err,len,totalnums=0,retryNums=times;//DEF_RETRY_NUMS;
	MS_CHAR msg[DEF_MAX_MSG_LENGHT],*buf;
	
	len = len1;	
	buf = buf1;

repeat:
	retryNums = times;///1000;
	
	if (retryNums == 0)
		retryNums = 1;


	while(len > 0 && buf !=NULL && retryNums>0)
	{	
		FD_ZERO(&ibits);
		FD_ZERO(&obits);
		FD_ZERO(&xbits);    
		FD_SET(sock, &obits);
		timeout.tv_sec  = timeOut/1000;//10;//3;//0;//3;//5;//1;// modify
		timeout.tv_usec = (timeOut%1000)*1000;//1000;//500000;//0; 
	   

			  
		err = select(sock+1, NULL, &obits, NULL, &timeout );
		
		if (err == 0) //time out
		{
			retryNums -- ;
			
			continue;
		}
		
		if(err <= 0 ) 				
		{
			////myLOGprint("Send dat to socket err \n",DEF_MSG_PRINT,0);
			TRACE("Send dat to socket err \n");
            //myLOGprint(msg,DEF_MSG_PRINT,0);

			return MD_FAILED;//err;//
		}

		if( FD_ISSET(sock, &obits))
		{
			if(len > PACK_SIZE)
			{	
				cnt = send( sock, buf + sent, PACK_SIZE, MSG_NOSIGNAL );
				if( cnt <= 0 )
				{
					TRACE("SendToNetBuffer err %d\n",errno);//fdError());
	
					//myLOGprint(msg,DEF_MSG_PRINT,0);
	
					return MD_FAILED;
						
				}		
				sent += cnt;//PACK_SIZE;
				len  -= cnt;//PACK_SIZE;
			}
			else
			{
				cnt = send( sock, buf + sent, len, MSG_NOSIGNAL );
				if( cnt <= 0 )
				{	
					TRACE("SendToNetBuffer err %d\n",errno);//fdError());
	
					//myLOGprint(msg,DEF_MSG_PRINT,0);
	
					return MD_FAILED;
				}		
				sent += cnt;//len;
				len -= cnt;//0;
			}
		}
	}

	if (len != 0)
	{
	
		////myLOGprint("Send dat timeout \n",DEF_MSG_PRINT,0);
        TRACE("Send dat timeout \n");
		//myLOGprint(msg,DEF_MSG_PRINT,0);

		
		return MD_FAILED;
		
	}
		
	totalnums +=sent;
	
	if (buf2 != NULL && len2 >0)
	{
		len = len2;	
		buf = buf2;

		len2 =	0 ;
		buf2 = NULL;
		sent = 0;
		
		retryNums=times;//DEF_RETRY_NUMS;
		
		goto repeat;

	}

	return totalnums;
	
#endif


}




MS_INT32 SendSocketReady(SOCKET sock)
{
 	struct  timeval timeout;    
	fd_set ibits, obits, xbits;
	MS_INT32 err;
		
	MS_CHAR msg[DEF_MAX_MSG_LENGHT];
	
	FD_ZERO(&ibits);
	FD_ZERO(&obits);
	FD_ZERO(&xbits);    
	FD_SET(sock, &obits);
	timeout.tv_sec  = 0;
    timeout.tv_usec = 0;//500000; 	  
  
	err = select(sock+1, NULL, &obits, NULL, &timeout );
	if(err < 0 ) 				
	{
		sprintf(msg,"Send data socket err \n");//fdError());
	
		//myLOGprint(msg,DEF_MSG_PRINT,0);

		return MD_FAILED;//err;//
	}
	else 
	{	
	    if( FD_ISSET(sock, &obits))
	    {
			//cnt = (int)recv( sock, buf, 1, 0 );
			//if( cnt < 0 )
			//{	
			//	//myLOGprint("Receive dat from socket err \n",0);

			//	return MD_FAILED;

			//}
			//else
			return MD_OK;		
		}
		else
			return MD_BUSY;
	}

}


MS_INT32 SendSocketDataUDP(SOCKET sock, MS_CHAR *buf, MS_INT32 len,MS_CHAR *svr_ip,MU_INT32 svr_port)
{

   
	
	return MD_OK;   
}

MS_INT32 RecvSocketDataUDP(SOCKET sock,MS_CHAR *buf,MS_INT32 len)
{

	
	return MD_OK;   

}


