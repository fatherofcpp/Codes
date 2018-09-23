/**
  @file klhttp.h
  @author Kevin Lynx
  @date 7.29.2008
  @brief the interface of klhttpd ( an simple embeded http server )

*/
#ifndef ___KL_HTTP_H_
#define ___KL_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sys/queue.h"


struct evbuffer;
struct http_request;

#define NEW( type ) (type*) malloc( sizeof( type ) )

/**
  information logger.
*/
typedef void (*info_log_cb)( const char *info );

/**
  set info logger.
*/
void http_set_info_log( info_log_cb log );

/**
  http connection structure.
*/
struct http_connection
{
	int fd;
	struct evbuffer *inbuf;
	struct evbuffer *outbuf;
	struct http_request *request;
	LIST_ENTRY( http_connection ) next;
};

/**
  http connection list head.
*/
LIST_HEAD( http_connection_head, http_connection );

/** request handler */
typedef void (*request_cb)( struct http_connection *conn, const struct http_request *request, void *arg );

/**
  http server structure.
*/
struct http_server
{
	struct tcp_server *server;
	struct http_connection_head conns;
	request_cb r_cb;
	void *arg;
};

/**
  startup the server.
*/
struct http_server *http_start( const char *ip, unsigned short port, int max_conn );

/**
  set request handler.
*/
void http_set_rcb( struct http_server *server, request_cb cb, void *arg );

/**
  run the server.
*/
int http_poll( struct http_server *server, struct timeval *timeout );

/**
  cleanup the server.
*/
void http_free( struct http_server *server );

#ifdef __cplusplus
}
#endif

#endif /* ___KL_HTTP_H_ */