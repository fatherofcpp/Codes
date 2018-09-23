/**
  @file klhttp.c
  @author Kevin Lynx
  @date 7.29.2008
  @brief the interface of klhttpd ( an simple embeded http server )

*/

#include "klhttp-config.h"
#include "klhttp.h"
#include "evbuffer.h"
#include "klhttp-netbase.h"
#include "klhttp-internal.h"
#include <malloc.h>
#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#endif

/**
  info log
*/
static info_log_cb s_log;

/**
  helper function to support fmt.
*/
static void _log( const char *fmt, ... )
{
	if( s_log )
	{
		static char info[512];
		va_list list;
		va_start( list, fmt );
		vsprintf( info, fmt, list );
		va_end( list );
		s_log( info );
	}
}

/**
  get the connection object.
*/
static struct http_connection *_get_conn( struct http_connection_head *head, int fd )
{
	struct http_connection *conn;
	LIST_FOREACH( conn, head, next )
	{
		if( fd == conn->fd )
		{
			return conn;
		}
	}

	return 0;
}

/**
  accept new client.
*/
static void _accept_conn( struct tcp_server *server, struct http_connection_head *head, int fd )
{
	struct sockaddr_in addr;
	int len = sizeof( addr );
	int conn_fd = (int)accept( fd, (struct sockaddr*)&addr, &len );
	if( conn_fd < 0 )
	{
		return ;
	}
	_log( "accept a new connection : %s-%d", inet_ntoa( addr.sin_addr ), ntohs( addr.sin_port ) );
	{
		struct http_connection *conn = (struct http_connection*) malloc( sizeof( struct http_connection ) );
		conn->fd = conn_fd;
		conn->inbuf = evbuffer_new();
		conn->outbuf = evbuffer_new();
		conn->request = NEW( struct http_request );
		conn->request->type = HTTP_UNKNOWN;
		conn->request->body_size = 0;
		conn->request->headers = conn->request->uri = NULL;
		memset(&conn->request->ver, 0, sizeof(conn->request->ver));
		memset(&conn->request->body, 0, sizeof(conn->request->body));
	
		/* add the fd into the server */
		ts_server_add( server, conn_fd );
		/* insert into the list */
		LIST_INSERT_HEAD( head, conn, next );
	}
}

/** 
  disconnect a connection.
*/
static void _disconnect_conn( struct http_connection *conn )
{
	closesocket( conn->fd );
	evbuffer_free( conn->inbuf );
	evbuffer_free( conn->outbuf );
	if(conn->request)
		http_request_free( conn->request );
}

/**
  retrieve the socket name 
*/
static const char *_get_fd_name( int fd )
{
	static char buf[64];
	struct sockaddr_in addr;
	int len = sizeof( addr );
	int ret = getpeername( fd, (struct sockaddr*)&addr, &len );
	if( ret < 0 )
	{
		return "0.0.0.0:0";
	}
	sprintf( buf, "%s:%d", inet_ntoa( addr.sin_addr ), ntohs( addr.sin_port ) );
	return buf;
}

/**
  handle read.
*/
static void _read_cb( int fd, void *arg )
{
	//FILE *pFile2;
	struct http_server *server = (struct http_server*)arg;
	if( fd == server->server->fd )
	{
		/* accept new client */
		_accept_conn( server->server, &server->conns, fd );
	}
	else
	{
		struct http_request *request;
		struct http_connection *conn = _get_conn( &server->conns, fd );
		/* i suppose conn will not be invalid */
		int ret = evbuffer_read( conn->inbuf, fd, EVBUFFER_MAX_READ );
		
		if( ret <= 0 )
		{
			_log( "disconnect a connection : %s", _get_fd_name( fd ) );
			/* disconnect the connection */
			ts_server_remove( server->server, fd );
			_disconnect_conn( conn );
			/* remove from the list */
			LIST_REMOVE( conn, next );
			if(conn)
				free(conn);
		}
		else
		{
			/* parse the request */
			if (conn->request->type != HTTP_POST)
				request = http_request_parse( conn/*conn->inbuf*/ );
			else
				request = conn->request;
			/* call the user-level function */
			if (conn->request->type == HTTP_POST){
				char *pBuf = &conn->request->body.data[0]; 
				if (conn->request->body_size == conn->inbuf->off){
					memcpy( pBuf + conn->request->body.pos ,conn->inbuf->buffer, conn->inbuf->off );
					conn->request->body.pos += conn->inbuf->off;
					server->r_cb( conn, request, server->arg );
					//pFile2 = fopen("e:\\Write_File.dat","wb");
					//fwrite(pBuf,1,conn->request->body_size,pFile2);
					//fclose(pFile2);
				}
			}
			else
				server->r_cb( conn, request, server->arg );
			/* free the request */
			/*http_request_free( request );*///ÒÆµ½_disconnect_connº¯ÊýÊÍ·Å
		}
	}
}

/**
  handle write.
*/
static void _write_cb( int fd, void *arg )
{
	struct http_server *server = (struct http_server*)arg;

	struct http_connection *conn = _get_conn( &server->conns, fd );
	if( EVBUFFER_LENGTH( conn->outbuf ) > 0 )
	{
		evbuffer_write( conn->outbuf, fd );
	}
}

void http_set_info_log( info_log_cb log )
{
	s_log = log;
}

struct http_server *http_start( const char *ip, unsigned short port, int max_conn )
{
	int ret;
	struct http_server *server = (struct http_server*) malloc( sizeof( struct http_server ) );
	server->server = (struct tcp_server*) malloc( sizeof( struct tcp_server ) );
	LIST_INIT( &server->conns );
	server->r_cb = 0;
	server->arg = 0;

	ret = ts_server_startup( server->server, ip, port, max_conn, _read_cb, _write_cb, server );
	if( ret < 0 )
	{
		free( server->server );
		free( server );
		return 0;
	}

	return server;
}

void http_set_rcb( struct http_server *server, request_cb cb, void *arg )
{
	server->r_cb = cb;
	server->arg = arg;
}

void http_free( struct http_server *server )
{
	struct http_connection *conn, *prev = 0;
	ts_server_cleanup( server->server );
	LIST_FOREACH( conn, &server->conns, next )
	{
		if( prev != 0 )
		{
			free( prev );
		}
		_disconnect_conn( conn );
		prev = conn;
	}

	free( server->server );
	free( server );
}

int http_poll( struct http_server *server, struct timeval *timeout )
{
	return ts_server_poll( server->server, timeout );
}	