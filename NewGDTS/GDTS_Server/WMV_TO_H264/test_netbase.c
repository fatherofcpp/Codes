/**
  @file test_netbase.c
  @brief to test the http-netbase.c
*/
#include "klhttp-netbase.h"
#ifdef WIN32
#include <winsock2.h>
#endif
#include <stdio.h>

volatile int is_done = 0;  

void read_cb( int fd, void *arg )
{
	struct tcp_server *server = (struct tcp_server*)arg;

	if( fd == server->fd )
	{
		/* accept connection */
		int s = (int)accept( fd, 0, 0 );
		ts_server_add( server, s );
		printf( "accept new conn\n" );
	}
	else
	{
		char buf[4096];
		int ret = recv( fd, buf, sizeof( buf ), 0 );
		if( ret == 0 )
		{
			closesocket( fd );
			ts_server_remove( server, fd );
			printf( "disconnect a conn\n" );
		}
	}
}

void write_cb( int fd, void *arg )
{
	struct tcp_server *server = (struct tcp_server*)arg;

}

/**
  handle the ctrl+c event.
*/
#ifdef WIN32
BOOL WINAPI Handler( DWORD ctrl_event )
{
	switch( ctrl_event )
	{
	case CTRL_CLOSE_EVENT:
	case CTRL_C_EVENT:
		{
			/* shutdown the server */
			is_done = 1;
		}

		return TRUE;
	}

	return FALSE;
}
#endif

int main()
{
	struct tcp_server server;
	int ret ;
	struct timeval timeout = { 0, 0 };

#ifdef WIN32
	if( !SetConsoleCtrlHandler( Handler, TRUE ) )
	{
		fprintf( stderr, "error setting event handler.\n" );
		return -1;
	}
#endif

#ifdef WIN32
	{
		WSADATA wd;
		ret = WSAStartup( MAKEWORD( 2, 0 ), &wd );
		if( ret < 0 )
		{
			fprintf( stderr, "winsock startup failed\n" );
			exit( -1 );
		}
	}
#endif
	ret = ts_server_startup( &server, "0.0.0.0", 8080, 1024, read_cb, write_cb, &server );
	if( ret < 0 )
	{
		fprintf( stderr, "server starup failed\n" );
		exit( -1 );
	}
	printf( "server startup\n" );

	while( is_done == 0 )
	{
		ts_server_poll( &server, &timeout );
	}

	ts_server_cleanup( &server );
	printf( "server shutdown\n" );
#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}
