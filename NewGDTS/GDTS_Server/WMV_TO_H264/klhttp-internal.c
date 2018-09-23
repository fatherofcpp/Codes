/**
  @file klhttp-internal.c
  @author Kevin Lynx
  @data 7.25.2008
  @brief for klhttpd internal use.
*/

#include "klhttp-config.h"
#include "klhttp-internal.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "evbuffer.h"

#include "klhttp.h"

/**
  parse http request initial line.
*/
static int _parse_init_line( struct http_request *request, char *line )
{
	char *token = strtok( line, " " );
	
	/* method */
	if( strcmp( token, "GET" ) == 0 )
	{
		request->type = HTTP_GET;
	}
	if( strcmp( token, "POST" ) == 0 )
	{
		request->type = HTTP_POST;
	}
	else if( strcmp( token, "HEAD" ) == 0 )
	{
		request->type = HTTP_HEAD;
	}
	else
	{
		request->type = HTTP_UNKNOWN;
		return 0;//hao modify because token is NULL
	}

	/* uri */
	token = strtok( 0, " " );
	request->uri = (char*) malloc( strlen( token ) + 1 );
	strcpy( request->uri, token );

	/* http protocol version */
	token = strtok( 0, " " );
	if( strcmp( token, "HTTP/1.0" ) == 0 )
	{
		request->ver.major = 1;
		request->ver.minor = 0;
	}
	else if( strcmp( token, "HTTP/1.1" ) == 0 )
	{
		request->ver.major = 1;
		request->ver.minor = 1;
	}
	else
	{
		request->ver.major = 1;
		request->ver.major = 0;
	}

	return 0;
}

/**
  add response headers
*/
static void _add_headers( struct http_header_head *headers, const char *name, const char *value )
{
	struct http_header *header = NEW( struct http_header );
	header->name = (char*) malloc( strlen( name ) + 1 );
	header->value = (char*) malloc( strlen( value ) + 1 );
	strcpy( header->name, name );
	strcpy( header->value, value );

	TAILQ_INSERT_TAIL( headers, header, next );
}

/**
  add time header
*/
static void _add_time_header( struct evbuffer *buf )
{
	char date[50];
#ifndef WIN32
	struct tm cur;
#endif
	struct tm *cur_p;
	time_t t = time( NULL );
#ifdef WIN32
	cur_p = gmtime( &t );
#else
	gmtime_r( &t, &cur );
	cur_p = &cur;
#endif
	strftime( date, sizeof( date ), "%a, %d %b %Y %H:%M:%S GMT", cur_p);
	evbuffer_add_printf( buf, "Date: %s\r\n", date );
}

/**
  get an uri's mime string.
*/
static const char *_get_mime_type( const char *uri, char *buf )
{
	const char *t;
	char type[64];

	if(uri == NULL)
		return NULL;

	t = uri + strlen( uri );
	
	for( ; t >= uri && *t != '.'; -- t )
	{
		/* until '.'*/
		;
	}

	if(strlen(t + 1)>63)//外网恶意攻击时会出现
	{
		sprintf( buf, "unknown" );
		return buf;

	}

	strcpy( type, t + 1 );

	/* append mime header */
	if( strcmp( type, "html" ) == 0 || 
		strcmp( type, "htm" ) == 0 )
	{
		sprintf( buf, "text/%s", type );
	}
	else if( strcmp( type, "gif" ) == 0 ||
		strcmp( type, "jpg" ) == 0 ||
		strcmp( type, "jpeg" ) == 0 ||
		strcmp( type, "png" ) == 0 )
	{
		sprintf( buf, "image/%s", type );
	}
	else if( strcmp( type, "/" ) == 0 )
	{
		/* the default value */
		sprintf( buf, "text/html" );
	}
	else
	{
		/* unknown error */
		sprintf( buf, "unknown" );
	}

	return buf;
}

/**
  check the whether the request is valid, if not, it will write the error response.

  @return if the request is valid return 0.
*/
static int _check_request_valid( struct evbuffer *buf, const struct http_request *request )
{
	if( request->type == HTTP_UNKNOWN )
	{
		/* not implemented method */
		http_response_error( buf, HTTP_NOTIMPLEMENT, CODE_STR( HTTP_NOTIMPLEMENT ), "Not Implemented Method" );
		return -1;
	}

	/* HTTP 1.1 needs Host header */
	if( request->ver.major == 1 && request->ver.minor == 1 )
	{
		const char *host = http_get_header_value( request->headers, "Host" );
		if( host == 0 )
		{
			/* bad request */
			http_response_error( buf, HTTP_BADREQUEST, CODE_STR( HTTP_BADREQUEST ), "Bad Request : needs Host header" );
			return -1;
		}
	}

	return 0;
}

struct http_header_head *http_header_new()
{
	struct http_header_head *queue = NEW( struct http_header_head );
	if( queue == 0 )
	{
		return 0;
	}
	TAILQ_INIT( queue );
	return queue;
}

void http_header_free( struct http_header_head *header_queue )
{
	struct http_header *header, *prev = 0,*tmp=NULL;
	TAILQ_FOREACH( header, header_queue, next )
	{
		if( prev != 0 )
		{
			free( prev );
			prev = NULL;
		}
		free( header->name );
		free( header->value );
		tmp = prev = header;
	}
	if(tmp != NULL)
		free(tmp);
	free( header_queue );
}

int http_add_header( struct http_request *request, /*struct http_header_head *header_queue*/ char *line )
{
	char *value ;
	char *name = strchr( line, ':' );
	struct http_header *header = NEW( struct http_header );
	header->name = (char*) malloc( name - line + 1 );
	strncpy( header->name, line, name - line );
	header->name[name-line] = '\0';

	for( value = name + 1; *value == ' '; ++ value )
	{
		/* until a no-space character. */
		;
	}

	header->value = (char*) malloc( strlen( line ) - ( value - line ) + 1 );
	strcpy( header->value, value );
	if (strcmp(header->name, "Content-Length") == 0)
		request->body_size = atoi(header->value);
	TAILQ_INSERT_TAIL( request->headers/*header_queue*/, header, next );

	return 0;
}

const char *http_get_header_value( struct http_header_head *header_queue, const char *header_name )
{
	struct http_header *header;
	TAILQ_FOREACH( header, header_queue, next )
	{
		if( strcmp( header->name, header_name ) == 0 )
		{
			return header->value;
		}
	}
	return 0;
}

struct http_request *http_request_parse( struct http_connection *conn/*struct evbuffer *buf*/ )
{
	//struct http_request *request = NEW( struct http_request );
	struct http_request *request = conn->request ;
	if( request == 0 )
	{
		return 0;
	}

	if (request->type == HTTP_UNKNOWN)/* parse initial line */
	{
		char *line = evbuffer_readline( conn->inbuf );
		int ret ;
		if(NULL == line)
			return 0;
		ret = _parse_init_line( request, line );
		free( line );
		if( ret != 0 )
		{
			//free( request );
			return 0;
		}
	}

	/* create the header queue */
	request->headers = http_header_new();
	/* parse headers */
	{
		char *line = evbuffer_readline( conn->inbuf ) ;
		int ret = 0;
		while( line != 0 && *line != '\0' ) /* there is one null line between header and body */
		{
			ret = http_add_header( request, line );

			free( line );
			if( ret != 0 )
			{
				break;
			}
			line = evbuffer_readline( conn->inbuf );
		}
		free( line );
	}

	/* ignore the bodies because it does not support now */
	return request;
}

void http_request_free( struct http_request *request )
{
	if(request->uri)
		free( request->uri );
	if(request->headers)
		http_header_free( request->headers );
	free( request );
}

/**
  currently, this function mainly to handle the http request, until now it only 
  load the resource file and send them to the client.
*/
//返回值
int http_handle_request( struct evbuffer *buf, const struct http_request *request, res_exist_cb cb_exist, res_load_cb cb_load ,int *rt ,const char*str )
{
	//const char*sz=str;
	//char *tmp="200abcdefg";
	//int ret=strlen(str);//123;
	//int ret1=strlen(sz);
	if(*rt==400)
	{
#if 0
		char more_info[256];
		sprintf( more_info, "The resource : %s is not found on this sever!", request->uri );
		//request->uri以字符串形式保存在more_info，并打印
		http_response_error( buf, HTTP_BADREQUEST, CODE_STR( HTTP_BADREQUEST ), more_info );
		//参数二为状态描述，三为状态信息字符串，四为只读路径
		return -1;
#else
		int ret=0;
		char tmp400[]="400";
		//if(NULL != str)
			ret = strlen(tmp400)+1;
		//200 OK 
		//将“/%d.%d %d %s\r\n”按前格式输出到buf
		evbuffer_add_printf( buf, "HTTP/%d.%d %d %s\r\n", request->ver.major, request->ver.minor, HTTP_BADREQUEST, CODE_STR( HTTP_BADREQUEST ) );

		// add server info header 
		evbuffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
		// add time header 
		_add_time_header( buf );
		// add Content-Type header 
		{
			char mime[32];
			evbuffer_add_printf( buf, "Content-Type: %s\r\n", _get_mime_type( request->uri, mime ) );
		}
		//add Content-Length header 
		evbuffer_add_printf( buf, "Content-Length: %d\r\n", ret );

		// end of headers 
		evbuffer_add( buf, "\r\n", 2 );

		if(NULL != tmp400)
			evbuffer_add( buf, tmp400, ret );
		return 0;


#endif
	}
	else if(*rt==200)
	{
		int ret=0;
		if(NULL != str)
			ret = strlen(str);
		//200 OK 
		//将“/%d.%d %d %s\r\n”按前格式输出到buf
		evbuffer_add_printf( buf, "HTTP/%d.%d %d %s\r\n", request->ver.major, request->ver.minor, HTTP_OK, CODE_STR( HTTP_OK ) );

		// add server info header 
		evbuffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
		// add time header 
		_add_time_header( buf );
		// add Content-Type header 
		{
			char mime[32];
			evbuffer_add_printf( buf, "Content-Type: %s\r\n", _get_mime_type( request->uri, mime ) );
		}
		//add Content-Length header 
		evbuffer_add_printf( buf, "Content-Length: %d\r\n", ret );

		// end of headers 
		evbuffer_add( buf, "\r\n", 2 );

		if(NULL != str)
			evbuffer_add( buf, str, ret );
		return 0;
	}
	else if(*rt==503)
	{
#if 0
		//char more_info[256];
		//sprintf( more_info, "The resource : %s is not found on this sever!", request->uri );
		//request->uri以字符串形式保存在more_info，并打印
		http_response_error( buf, HTTP_SERVUNAVAIL, CODE_STR( HTTP_SERVUNAVAIL ), NULL );
		//参数二为状态描述，三为状态信息字符串，四为只读路径
		return -1;
#else
		int ret=0;
		char tmp503[]="503";
		//if(NULL != str)
			ret = strlen(tmp503);
		//200 OK 
		//将“/%d.%d %d %s\r\n”按前格式输出到buf
		evbuffer_add_printf( buf, "HTTP/%d.%d %d %s\r\n", request->ver.major, request->ver.minor, HTTP_SERVUNAVAIL, CODE_STR( HTTP_SERVUNAVAIL ) );

		// add server info header 
		evbuffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
		// add time header 
		_add_time_header( buf );
		// add Content-Type header 
		{
			char mime[32];
			evbuffer_add_printf( buf, "Content-Type: %s\r\n", _get_mime_type( request->uri, mime ) );
		}
		//add Content-Length header 
		evbuffer_add_printf( buf, "Content-Length: %d\r\n", ret );

		// end of headers 
		evbuffer_add( buf, "\r\n", 2 );

		if(NULL != tmp503)
			evbuffer_add( buf, tmp503, ret );
		return 0;


#endif
	}
	else if(*rt==404)
	{
#if 0
		//char more_info[256];
		//sprintf( more_info, "The resource : %s is not found on this sever!", request->uri );
		//request->uri以字符串形式保存在more_info，并打印
		http_response_error( buf, HTTP_NOTFOUND, CODE_STR( HTTP_NOTFOUND ), NULL );
		//参数二为状态描述，三为状态信息字符串，四为只读路径
		return -1;
#else
		int ret=0;
		char tmp404[]="404";
		//if(NULL != str)
			ret = strlen(tmp404);
		//200 OK 
		//将“/%d.%d %d %s\r\n”按前格式输出到buf
		evbuffer_add_printf( buf, "HTTP/%d.%d %d %s\r\n", request->ver.major, request->ver.minor, HTTP_NOTFOUND, CODE_STR( HTTP_NOTFOUND ) );

		// add server info header 
		evbuffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
		// add time header 
		_add_time_header( buf );
		// add Content-Type header 
		{
			char mime[32];
			evbuffer_add_printf( buf, "Content-Type: %s\r\n", _get_mime_type( request->uri, mime ) );
		}
		//add Content-Length header 
		evbuffer_add_printf( buf, "Content-Length: %d\r\n", ret );

		// end of headers 
		evbuffer_add( buf, "\r\n", 2 );

		if(NULL != tmp404)
			evbuffer_add( buf, tmp404, ret );
		return 0;


#endif
	}
	//*********

	//*********
	/*	
	char more_info[256];
	sprintf( more_info, "The resource : %s is not found on this sever!", request->uri );
	//request->uri以字符串形式保存在more_info，并打印
	http_response_error( buf, HTTP_NOTFOUND, CODE_STR( HTTP_NOTFOUND ), more_info );
	//参数二为状态描述，三为状态信息字符串，四为只读路径
	return -1;



	if ((cb_exist == NULL) || (cb_load == NULL))
	return 0;
	// check whether the request is valid 
	ret = _check_request_valid( buf, request );
	if( ret == -1 )
	{
	return -1;
	}

	ret = cb_exist( request->uri );//
	if( ret < 0 )
	{
	// file not found 
	char more_info[256];
	sprintf( more_info, "The resource : %s is not found on this sever!", request->uri );
	//request->uri以字符串形式保存在more_info，并打印
	http_response_error( buf, HTTP_NOTFOUND, CODE_STR( HTTP_NOTFOUND ), more_info );
	//参数二为状态描述，三为状态信息字符串，四为只读路径
	return -1;
	}


	//200 OK 
	//将“/%d.%d %d %s\r\n”按前格式输出到buf
	evbuffer_add_printf( buf, "HTTP/%d.%d %d %s\r\n", request->ver.major, request->ver.minor, HTTP_OK, CODE_STR( HTTP_OK ) );

	// add server info header 
	evbuffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
	// add time header 
	_add_time_header( buf );
	// add Content-Type header 
	{
	char mime[32];
	evbuffer_add_printf( buf, "Content-Type: %s\r\n", _get_mime_type( request->uri, mime ) );
	}
	//add Content-Length header 
	evbuffer_add_printf( buf, "Content-Length: %d\r\n", ret );

	// end of headers 
	evbuffer_add( buf, "\r\n", 2 );


	if( request->type == HTTP_GET )
	{
	// load the resource 
	ret = cb_load( request->uri, buf );
	if( ret < 0 )
	{
	// server unknown error 

	}
	}
	*/

	return 0;
}

#if 0
int http_handle_request( struct evbuffer *buf, const struct http_request *request, res_exist_cb cb_exist, res_load_cb cb_load   )
{
	int ret;

	if ((cb_exist == NULL) || (cb_load == NULL))
		return 0;
	/* check whether the request is valid */
	ret = _check_request_valid( buf, request );
	if( ret == -1 )
	{
		return -1;
	}

	ret = cb_exist( request->uri );
	if( ret < 0 )
	{
		/* file not found */
		char more_info[256];
		sprintf( more_info, "The resource : %s is not found on this sever!", request->uri );
		http_response_error( buf, HTTP_NOTFOUND, CODE_STR( HTTP_NOTFOUND ), more_info );
		return -1;
	}
	
	/* 200 OK */
	evbuffer_add_printf( buf, "HTTP/%d.%d %d %s\r\n", request->ver.major, request->ver.minor, HTTP_OK, CODE_STR( HTTP_OK ) );

	/* add server info header */
	evbuffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
	/* add time header */
	_add_time_header( buf );
	/* add Content-Type header */
	{
		char mime[32];
		evbuffer_add_printf( buf, "Content-Type: %s\r\n", _get_mime_type( request->uri, mime ) );
	}
	/* add Content-Length header */
	evbuffer_add_printf( buf, "Content-Length: %d\r\n", ret );

	/* end of headers */
	evbuffer_add( buf, "\r\n", 2 );

	if( request->type == HTTP_GET )
	{
		/* load the resource */
		ret = cb_load( request->uri, buf );
		if( ret < 0 )
		{
			/* server unknown error */
			
		}
	}

	return 0;
}
#endif

void http_response_error( struct evbuffer *buf, int status, const char *status_str, const char *more_info )
{
#define ERR_FORMAT "<HTML><HEAD>\n" \
	    "<TITLE>%d %s</TITLE>\n" \
	    "</HEAD><BODY>\n" \
	    "<H1>%d %s</H1>\n" \
	    "%s<P>\n" \
	    "</BODY></HTML>\n"
	char content[512];
	size_t len;

	sprintf( content, ERR_FORMAT, status, status_str, status, status_str, more_info );
	len = strlen( content );

	/* initial line */
	evbuffer_add_printf( buf, "HTTP/1.1 %d %s\r\n", status, status_str );
	/* add server info header */
	evbuffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
	/* add time header */
	_add_time_header( buf );
	/* content type */
	evbuffer_add_printf( buf, "Content-Type: text/html\r\n" );
	/* content length */
	evbuffer_add_printf( buf, "Content-Length: %d\r\n", len );
	/* end of headers */
	evbuffer_add( buf, "\r\n", 2 );
	/* body */
	evbuffer_add( buf, content, len );
	
#undef ERR_FORMAT
}
