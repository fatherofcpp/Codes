/**
*/
#include "klhttp.h"
#include "klhttp-internal.h"
#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#endif

/**
  get file size
*/
int get_file_size( FILE *fp )
{
	int file_size ;
	fseek( fp, 0, SEEK_END );
	file_size = ftell( fp );
	fseek( fp, 0, SEEK_SET );
	return file_size;
}

/**
  resource file load.
*/
int load_file( const char *uri, struct evbuffer *buf )
{
	char file[260];
	FILE *fp;
	int file_size;
	char *data;

	if( strcmp( uri, "/" ) == 0 )
	{
		strcpy( file, "./www/index.html" );
	}
	else
	{
		sprintf( file, "./www%s", uri );
	}

	fp = fopen( file, "rb" );
	if( fp == 0 )
	{
		return -1;
	}
	
	file_size = get_file_size( fp );
	data = (char*) malloc( file_size );
	fread( data, file_size, 1, fp );
	evbuffer_add( buf, data, file_size );
	free( data );

	fclose( fp );

	return file_size;
}

int file_exist( const char *uri )
{
	char file[256];
	FILE *fp;
	int file_size;
	
	if( strcmp( uri, "/" ) == 0 )
	{
		strcpy( file, "./www/index.html" );
	}
	else
	{
		sprintf( file, "./www%s", uri );
	}

	fp = fopen( file, "rb" );
	if( fp == 0 )
	{
		return -1;
	}

	file_size = get_file_size( fp );
	fclose( fp );
	return file_size;
}

void handle_request( struct http_connection *conn, const struct http_request *request, void *arg )
{
	if (arg == NULL)
		http_handle_request( conn->outbuf, request, NULL, NULL );
	else
		http_handle_request( conn->outbuf, request, file_exist, load_file );
}

void info_log( const char *msg )
{
	printf( "%s\n", msg );
}

volatile int is_done = 0;  

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
	struct http_server *httpd;
	struct timeval timeout = { 2, 0 };	
#ifdef WIN32
	{
		WSADATA wd;
		WSAStartup( MAKEWORD( 2, 0 ), &wd );
	}
#endif
	
#ifdef WIN32
	if( !SetConsoleCtrlHandler( Handler, TRUE ) )
	{
		fprintf( stderr, "error setting event handler.\n" );
		return -1;
	}
#endif

	http_set_info_log( info_log );
	httpd = http_start( "0.0.0.0", 8080, 1024 );
	http_set_rcb( httpd, handle_request, 0 );

	printf( "server startup\n" );
	while( is_done == 0 )
	{
		http_poll( httpd, &timeout );
	}

	http_free( httpd );
	
#ifdef WIN32
	WSACleanup();
#endif
	printf( "server shutdown\n" );
	return 0;
}