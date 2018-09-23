/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: mty.h 1924 2009-05-31 22:03:41Z dengxf $
 */

#ifndef _MTY_H_
#define _MTY_H_

/*
 * Data type    ILP32   LLP64   LP64    
 * char         1       1       1       
 * short        2       2       2       
 * int          4       4       4       
 * long         4       4       8       
 * long long    8       8       8       
 *                                      
 * pointer      4       8       8       
 */

#include <time.h>

/*
 * Visual Studio C/C++ Compiler : _MSC_VER
 *                              :
 *                              : _M_IX86
 *                              : _M_X64, _M_AMD64,
 *                              : _ARM, ARM
 *
 * Intel C/C++ Compiler         : __ICC, __INTEL_COMPILER
 *                              :
 *                              : _M_IX86
 *                              : _M_X64
 *
 * GNU C/C++ Compiler           : __GNUC__
 *                              :
 *                              : __i386__
 *                              : __amd64__, __x86_64__
 *                              : __arm__
 *
 * TI Compiler                  : __TI_COMPILER_VERSION__
 *                              : _TMS320C6X, _LITTLE_ENDIAN
 *                              :
 *
 */



/*
 * Some 
 */
#define M_INLINE             inline
#define M_CONST              const
#define M_RESTRICT           //restrict
#define M_VOLATILE           volatile

/*
 * DATA TYPE
 * sizeof(void *) == sizeof (MS_PINT)
 */

/*
 * signed
 */
typedef char                MS_CHAR;
typedef signed char         MS_INT8;
typedef signed short int    MS_INT16;
typedef signed int          MS_INT32;
typedef signed long long    MS_INT64;



/*
 * unsigned
 */

typedef unsigned char       MU_CHAR;
typedef unsigned char       MU_INT8;
typedef unsigned short int  MU_INT16;
typedef unsigned int        MU_INT32;
typedef unsigned long long  MU_INT64;



typedef float               M_FLOAT;
typedef double              M_DOUBLE;
typedef int                 M_BOOL;

typedef time_t              M_TIME;
typedef unsigned int        MU_PINT;
#define M_FALSE             0
#define M_TRUE              1



#define DEF_STR_LEN_MIN         64
#define DEF_STR_LEN_STD         256
#define DEF_STR_LEN_MAX         1024

#define DEF_BUF_LEN_MIN         (4 * 1024)
#define DEF_BUF_LEN_STD         (64 * 1024)
#define DEF_BUF_LEN_MAX         (256 * 1024)


#define M_MOVECONST(x)          ((MU_PINT)x)
#define M_HTONL(x)              (((x) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | ((x) << 24))

#define BOOL bool
#endif
