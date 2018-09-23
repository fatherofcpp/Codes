/**
  @file klhttp-netbase.c
  @author Kevin Lynx
  @date 7.29.2008
  @brief wrap the net module.
*/

#include "klhttp-netbase.h"
#ifdef WIN32
#include <winsock2.h>
#endif

/**
  malloc a fd_set on the heap, to make it dependent from FD_SETSIZE.
  make sure fd_count > FD_SETSIZE.
*/
#define NEW_FD_SET( fd_count ) \
	(struct fd_set*) malloc( sizeof( struct fd_set ) + sizeof( int ) * ( \
	fd_count - FD_SETSIZE > 0 ? fd_count - FD_SETSIZE : 0 ) )

/**
  bind address and listen.
*/
static int _server_listen( int fd, const char *ip, unsigned short port )
{
	int ret;
	struct sockaddr_in addr;
	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = inet_addr( ip );
	
	ret = bind( fd, (struct sockaddr*) &addr, sizeof( addr ) );//分配的套接口绑定ip 和端口
	if( ret < 0 )
	{
		return -1;
	}

	/* the backlog */
	ret = listen( fd, 1024 );//一个套接字处于监听到来的连接请求的状态


	return ret;
}

/**
  set fd set
*/
static void _server_set_fd( struct tcp_server *server )
{
	memcpy( server->fd_readset->fd_array, server->fd_set->fd_array, sizeof( server->fd_set->fd_array[0] ) * server->fd_set->fd_count );
	server->fd_readset->fd_count = server->fd_set->fd_count ;
	memcpy( server->fd_writeset->fd_array, server->fd_set->fd_array, sizeof( server->fd_set->fd_array[0] ) * server->fd_set->fd_count );
	server->fd_writeset->fd_count = server->fd_set->fd_count ;
	FD_SET( server->fd, server->fd_readset );
}

int ts_server_startup( struct tcp_server *server, const char *ip, unsigned short port, int max_con, read_write_cb read_cb, read_write_cb write_cb, void *arg )
{
	int ret;

	server->fd = (int) socket( AF_INET, SOCK_STREAM, 0 );//分配一个套接口的描述字
	server->max_con = max_con;
	server->read_cb = read_cb;
	server->write_cb = write_cb;
	server->arg = arg;

	ret = _server_listen( server->fd, ip, port );
	if( ret < 0 )
	{
		closesocket( server->fd );
		return -1;
	}

	/* malloc the fd set */
	server->fd_readset = NEW_FD_SET( max_con );
	server->fd_writeset = NEW_FD_SET( max_con );
	server->fd_set = NEW_FD_SET( max_con );
	FD_ZERO( server->fd_readset );
	FD_ZERO( server->fd_writeset );
	FD_ZERO( server->fd_set );
	
	return 0;
}

void ts_server_cleanup( struct tcp_server *server )
{
	closesocket( server->fd );
	/* free memory then */
	free( server->fd_readset );
	free( server->fd_writeset );
	free( server->fd_set );
}

int ts_server_poll( struct tcp_server *server, struct timeval *timeout )
{
	unsigned int i;
	int ret ;

	/* reset the fd set */
	_server_set_fd( server );

	ret = select( 0, server->fd_readset, server->fd_writeset, 0, timeout );
	if( ret <= 0 )
	{
		return ret;
	}

	/* write first and read later can avoid some bugs */
	for( i = 0; i < server->fd_writeset->fd_count; ++ i )
	{
		server->write_cb( (int)server->fd_writeset->fd_array[i], server->arg );
	}

	for( i = 0; i < server->fd_readset->fd_count; ++ i )
	{
		server->read_cb( (int)server->fd_readset->fd_array[i], server->arg );
	}

	return ret;
}
