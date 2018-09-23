/**
  @file klhttp-netbase.h
  @author Kevin Lynx
  @date 7.29.2008
  @brief wrap the net module.
*/
#ifndef ___KL_NETBASE_H_
#define ___KL_NETBASE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
  handle read/write callback.
*/
typedef void (*read_write_cb)( int fd, void *arg );

struct fd_set;
struct timeval;

/**
  a tcp server.
*/
struct tcp_server
{
	/** socket : file descriptor */
	int fd;
	/** fd set */
	struct fd_set *fd_readset;
	struct fd_set *fd_writeset;
	struct fd_set *fd_set;
	/** max connection */
	int max_con;
	/** read/write callback */
	read_write_cb read_cb;
	read_write_cb write_cb;
	/** argument for read/write callback */
	void *arg;
};

/**
  create a tcp server, this function will create the socket resource, and listen on some port.
*/
int ts_server_startup( struct tcp_server *server, const char *ip, unsigned short port, int max_con, read_write_cb read_cb, read_write_cb write_cb, void *arg );

/**
  shutdown the tcp server and free resources.
*/
void ts_server_cleanup( struct tcp_server *server );

/**
  poll server events.
*/
int ts_server_poll( struct tcp_server *server, struct timeval *timeout );

/**
  add a fd in the server.
*/
#define ts_server_add( server_ptr, fd ) \
	FD_SET( fd, server_ptr->fd_set )
	

/**
  remove a fd from the server.
*/
#define ts_server_remove( server_ptr, fd ) \
	FD_CLR( fd, server_ptr->fd_set )
	

#ifdef __cplusplus
}
#endif

#endif /* ___KL_NETBASE_H_ */
