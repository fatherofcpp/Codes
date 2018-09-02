#pragma once

#include <string>

//type define
#define M_IN
#define M_OUT
//

//to fit win32 && linux
#ifndef WIN32

typedef unsigned char		BYTE;
typedef const char*			LPCSTR;
typedef unsigned long		DWORD;
typedef int                 BOOL;
typedef unsigned short		WCHAR;
typedef void*				LPVOID;
typedef void*				HANDLE;

#else

#endif
//

//rename
typedef void	M_VOID;

typedef char                MS_CHAR;
typedef char				MS_INT8;
typedef const char          MCS_CHAR;
typedef unsigned char       MU_CHAR;
typedef unsigned char       MU_INT8;
typedef const unsigned char MCU_CHAR;

typedef signed short int		 MS_INT16;
typedef const signed short int	 MCS_INT16;
typedef unsigned short int		 MU_INT16;
typedef const unsigned short int MCU_INT16;


typedef signed int          MS_INT32;
typedef const signed int    MCS_INT32;
typedef unsigned int        MU_INT32;
typedef const unsigned int  MCU_INT32;

typedef signed long long		 MS_INT64;
typedef const signed long long	 MCS_INT64;
typedef unsigned long long		 MU_INT64;
typedef const unsigned long long MCU_INT64;

typedef float               M_FLOAT;
typedef const float         MC_FLOAT;
typedef double              M_DOUBLE;
typedef const double        MC_DOUBLE;
typedef bool                M_BOOL;
typedef const bool          MC_BOOL;
typedef time_t              M_TIME_T;
typedef const time_t        MC_TIME_T;
typedef size_t				M_SIZE_T;
typedef const size_t		MC_SIZE_T;

typedef std::string			M_STRING;
typedef const std::string   MC_STRING;

//

///////////////////////////////////////////
