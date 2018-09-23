// Export.h: interface for the Export class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETAPI_H__087355E7_4B84_4A69_BCE8_B71632214D4A__INCLUDED_)
#define AFX_NETAPI_H__087355E7_4B84_4A69_BCE8_B71632214D4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef __int64				int64_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short        int16_t;
typedef unsigned int        uint32_t;
typedef signed int          int32_t;

#define ENETUNREACH WSAENETUNREACH
#define EINTR           4
#define vsnprintf _vsnprintf

typedef struct
{
    char *psz_protocol;
    char *psz_username;

    char *psz_password;
    char *psz_host;
    int  i_port;
    char *psz_path;
    char *psz_option;    
    char *psz_buffer; 
}tagURL;


#ifdef Porting
#pragma message( "Warning! Porting already defined!" )
#undef Porting
#endif

#ifdef COMPILING_DLL
#define Porting extern "C" __declspec( dllexport )
#pragma message( "     Exporting CRYPT" )
#else
#define Porting extern "C" __declspec( dllimport )
#pragma message( "     Importing CRYPT" )
#endif

Porting int	InitNet(int i_family, int i_socktype, int i_protocol);
Porting void NetClose( int fd );
Porting int ConnectTCP(const char *psz_host, int i_port);
Porting void URLParse(tagURL *url, const char *psz_url, char option );
Porting void URLFree(tagURL *url );
Porting int NetPrintf(int fd, const char *psz_fmt, ... );
Porting int NetVarPrintf(int fd, const char *psz_fmt, va_list args);
Porting int NetWrite(int fd, const uint8_t *p_data, int i_data );
Porting char *NetGets(int fd, int *iLen);
Porting int NetRead(int fd, uint8_t *p_data, int i_data, bool b_retry );
Porting void NetStop();
Porting void DLLMemFree(void *p);

#endif // !defined(AFX_NETAPI_H__087355E7_4B84_4A69_BCE8_B71632214D4A__INCLUDED_)
