/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: mty.h 1924 2009-05-31 22:03:41Z dengxf $
 */

#ifndef _GD_TYPE_H_
#define _GD_TYPE_H_

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

#if defined(_WIN32_WCE)
#   define _ENV_OS_WINCE
#   include <windows.h>
#elif defined(_WIN64)
#   define _ENV_OS_WIN64
#   include <windows.h>
#elif defined(_WIN32)
#   define _ENV_OS_WIN32
#   include <windows.h>
#elif defined(__FreeBSD__)
#   define _ENV_OS_FREEBSD
#elif defined(linux) || defined(__linux__)
#   define _ENV_OS_LINUX
#elif defined(__TI_COMPILER_VERSION__) && defined(_TMS320C6X)
#   define _ENV_OS_TIDSPBIOS
#else
#   error "OS NOT SUPPORT"
#endif

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

#if defined(_MSC_VER)
#   define _ENV_CC_MSVC
#   define _ENV_ASM_SYNTAX_INTEL
#elif defined(__ICC) || defined(__INTEL_COMPILER)
#   define _ENV_CC_INTEL
#   define _ENV_ASM_SYNTAX_INTEL
#elif defined(__GNUC__)
#   define _ENV_CC_GNU
#   define _ENV_ASM_SYNTAX_ATT
#elif defined(__TI_COMPILER_VERSION__) && defined(_TMS320C6X)
#   define _ENV_CC_TICCS
#   define _ENV_ASM_SYNTAX_TI
#else
#   error "COMPILER NOT SUPPORT"
#endif

#if defined(__STDC_VERSION__)
#   if (__STDC_VERSION__ - 0 >= 199901L)
#       define _ENV_STANDARD_C99
#   else
#       define _ENV_STANDARD_C89
#   endif
#else
#   if defined(__STDC__) && defined(__TI_COMPILER_VERSION__) && defined(_TMS320C6X)
#       define _ENV_STANDARD_C99
#   else
#       define _ENV_STANDARD_C89
#   endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#   define _ENV_ARCH_X86
#elif defined(__x86_64__) || defined(__amd64__) || defined(_M_X64) || defined(_M_AMD64)
#   define _ENV_ARCH_X86_64
#elif defined(__arm__) || defined(_ARM) || defined(ARM)
#   define _ENV_ARCH_ARM
#elif defined(__TI_COMPILER_VERSION__) && defined(_TMS320C6X)
#   define _ENV_ARCH_TIDSP
#else
#   error "ARCH NOT SUPPORT"
#endif

#if defined(_ENV_OS_WINCE) && defined(_ENV_ARCH_ARM)
#   define _ENV_DATA_TYPE_ILP32
#elif defined(_ENV_OS_WIN32) && defined(_ENV_ARCH_X86)
#   define _ENV_DATA_TYPE_ILP32
#elif defined(_ENV_OS_WIN64) && defined(_ENV_ARCH_X86_64)
#   define _ENV_DATA_TYPE_LLP64
#elif defined(_ENV_OS_FREEBSD) && defined(_ENV_ARCH_X86)
#   define _ENV_DATA_TYPE_ILP32
#elif defined(_ENV_OS_FREEBSD) && defined(_ENV_ARCH_X86_64)
#   define _ENV_DATA_TYPE_LP64
#elif defined(_ENV_OS_LINUX) && defined(_ENV_ARCH_X86)
#   define _ENV_DATA_TYPE_ILP32
#elif defined(_ENV_OS_LINUX) && defined(_ENV_ARCH_X86_64)
#   define _ENV_DATA_TYPE_LP64
#elif defined(_ENV_OS_LINUX) && defined(_ENV_ARCH_ARM)
#   define _ENV_DATA_TYPE_ILP32
#elif defined(_ENV_OS_TIDSPBIOS) && defined(_LITTLE_ENDIAN)
#   define _ENV_DATA_TYPE_ILP32
#else
#   error "DATA TYPE NOT SUPPORT"
#endif

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

#if defined(_ENV_DATA_TYPE_ILP32)
typedef signed int          MS_PINT;
#elif defined(_ENV_DATA_TYPE_LP64)
typedef signed long         MS_PINT;
#elif defined(_ENV_DATA_TYPE_LLP64)
typedef signed long long    MS_PINT;
#endif

/*
 * unsigned
 */

typedef unsigned char       MU_CHAR;
typedef unsigned char       MU_INT8;
typedef unsigned short int  MU_INT16;
typedef unsigned int        MU_INT32;
typedef unsigned long long  MU_INT64;

#if defined(_ENV_DATA_TYPE_ILP32)
typedef unsigned int        MU_PINT;
#elif defined(_ENV_DATA_TYPE_LP64)
typedef unsigned long       MU_PINT;
#elif defined(_ENV_DATA_TYPE_LLP64)
typedef unsigned long long  MU_PINT;
#endif

typedef float               M_FLOAT;
typedef double              M_DOUBLE;
typedef int                 M_BOOL;

typedef time_t              M_TIME;

#define M_FALSE             0
#define M_TRUE              1

/*
 * Some Definetions
 */
typedef MS_INT32                (*I_FT_II)(MS_INT32, MS_INT32);
typedef MS_INT32                (*I_FT_I)(MS_INT32);
typedef MS_INT32                (*I_FT_V)(void);
typedef MS_INT32                (*I_FT_VCP)(void *const);
typedef MS_INT32                (*I_FT_CCP)(MS_CHAR *const);
typedef void                    (*V_FT_I)(MS_INT32);
typedef void                    (*V_FT_V)(void);
typedef void                    (*V_FT_VCP)(void *const);
typedef void                    (*V_FT_CCP)(MS_CHAR *const);

typedef void                    (*FT_THRM)(void *const);
typedef MS_INT32                (*FUNC_CMP)(const void *, const void *);

/* MACRO DEFINITION */
#define DEF_FLAG_FREE           0
#define DEF_FLAG_USED           0x55
#define DEF_FLAG_SET_FREE       0x53

#define DEF_STATUS_NONE         DEF_FLAG_FREE
#define DEF_STATUS_READY        DEF_FLAG_USED
#define DEF_STATUS_END          DEF_FLAG_SET_FREE

#define DEF_STR_LEN_MIN         64
#define DEF_STR_LEN_STD         256
#define DEF_STR_LEN_MAX         1024

#define DEF_BUF_LEN_MIN         (4 * 1024)
#define DEF_BUF_LEN_STD         (64 * 1024)
#define DEF_BUF_LEN_MAX         (256 * 1024)

#define DEF_PAGE_SIZE           4096
#define DEF_PAGE_MASK           (~(DEF_PAGE_SIZE - 1))

#define M_OFTOFM(TYPE, MEMBER)  ((MU_INT32) &((TYPE *)0)->MEMBER)

#define M_LND(mod,x)            ((x) & ~(mod-1))
#define M_RND(mod,x)            (((x) + (mod-1)) & ~(mod-1))
#define M_POT(x)                ((((x) & ((x) - 1)) == 0) && (x != 0))

#define M_MOVECONST(x)          ((MU_PINT)x)

#define M_MKLETAG(a,b,c,d)      ((((d) & 0xFF) << 24) | (((c) & 0xFF) << 16) | (((b) & 0xFF) << 8) | ((a) & 0xFF))
#define M_MKBETAG(a,b,c,d)      ((((a) & 0xFF) << 24) | (((b) & 0xFF) << 16) | (((c) & 0xFF) << 8) | ((d) & 0xFF))

#define M_HTONS(x)              ((((x) >> 8) & 0xFF) | (((x) << 8) & 0xFF00))
#define M_HTONL(x)              (((x) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | ((x) << 24))

#define _FUNC_X(name,method)    f_##name##method
#define FUNC_X(name,method)     _FUNC_X(name,method)

//#define M_SIZE(mm)              sizeof(mm)/sizeof(mm[0])

#define DEF_ECODE_EACCESS       400
#define DEF_ECODE_EINVAL        401
#define DEF_ECODE_ESYS          402
#define DEF_ECODE_ENOFILE       403

/* addr get/set */
#define M_GET_MS_INT8(to,pfrom)             \
do                                          \
{                                           \
    MS_INT8 t_s8;                           \
    memcpy(&t_s8, pfrom, 1);                \
    to = t_s8;                              \
} while (0)

#define M_GET_MU_INT8(to,pfrom)             \
do                                          \
{                                           \
    MU_INT8 t_u8;                           \
    memcpy(&t_u8, pfrom, 1);                \
    to = t_u8;                              \
} while (0)

#define M_GET_MS_INT16(to,pfrom)            \
do                                          \
{                                           \
    MS_INT16 t_s16;                         \
    memcpy(&t_s16, pfrom, 2);               \
    to = t_s16;                             \
} while (0)

#define M_GET_MU_INT16(to,pfrom)            \
do                                          \
{                                           \
    MU_INT16 t_u16;                         \
    memcpy(&t_u16, pfrom, 2);               \
    to = t_u16;                             \
} while (0)

#define M_GET_MS_INT32(to,pfrom)            \
do                                          \
{                                           \
    MS_INT32 t_s32;                         \
    memcpy(&t_s32, pfrom, 4);               \
    to = t_s32;                             \
} while (0)

#define M_GET_MU_INT32(to,pfrom)            \
do                                          \
{                                           \
    MU_INT32 t_u32;                         \
    memcpy(&t_u32, pfrom, 4);               \
    to = t_u32;                             \
} while (0)

#define M_GET_MS_INT64(to,pfrom)            \
do                                          \
{                                           \
    MS_INT32 t_s64;                         \
    memcpy(&t_s64, pfrom, 8);               \
    to = t_s64;                             \
} while (0)

#define M_GET_MU_INT64(to,pfrom)            \
do                                          \
{                                           \
    MU_INT32 t_u64;                         \
    memcpy(&t_u64, pfrom, 8);               \
    to = t_u64;                             \
} while (0)

#define M_GET_BUF(pto,pfrom,len)            \
do                                          \
{                                           \
    memcpy(pto, pfrom, len);                \
} while (0)

#define M_SET_MS_INT8(pto,from)             \
do                                          \
{                                           \
    MS_INT8 t_s8 = from;                    \
    memcpy(pto, &t_s8, 1);                  \
} while (0)

#define M_SET_MU_INT8(pto,from)             \
do                                          \
{                                           \
    MU_INT8 t_u8 = from;                    \
    memcpy(pto, &t_u8, 1);                  \
} while (0)

#define M_SET_MS_INT16(pto,from)            \
do                                          \
{                                           \
    MS_INT16 t_s16 = from;                  \
    memcpy(pto, &t_s16, 2);                 \
} while (0)

#define M_SET_MU_INT16(pto,from)            \
do                                          \
{                                           \
    MU_INT16 t_u16 = from;                  \
    memcpy(pto, &t_u16, 2);                 \
} while (0)

#define M_SET_MS_INT32(pto,from)            \
do                                          \
{                                           \
    MS_INT32 t_s32 = from;                  \
    memcpy(pto, &t_s32, 4);                 \
} while (0)

#define M_SET_MU_INT32(pto,from)            \
do                                          \
{                                           \
    MU_INT32 t_u32 = from;                  \
    memcpy(pto, &t_u32, 4);                 \
} while (0)

#define M_SET_MS_INT64(pto,from)            \
do                                          \
{                                           \
    MS_INT32 t_s64 = from;                  \
    memcpy(pto, &t_s64, 8);                 \
} while (0)

#define M_SET_MU_INT64(pto,from)            \
do                                          \
{                                           \
    MU_INT32 t_u64 = from;                  \
    memcpy(pto, &t_u64, 8);                 \
} while (0)

#define M_SET_BUF M_GET_BUF

#define M_ADDR_2_ADDR(pto,pfrom)            \
do                                          \
{                                           \
    void *addr = pfrom;                     \
    memcpy(&pto, &addr, sizeof(void *));    \
} while (0)

#define M_ADDR_2_PINT(to,pfrom)             \
do                                          \
{                                           \
    void *addr = pfrom;                     \
    memcpy(&to, addr, sizeof(void *));      \
} while (0)

#define M_PINT_2_ADDR(pto,from)             \
do                                          \
{                                           \
    memcpy(&pto, &from, sizeof(void *));    \
} while (0)


#define M_MAX(x, y)             (((x) > (y)) ? (x) : (y))
#define M_MIN(x, y)             (((x) < (y)) ? (x) : (y))
#define M_MIX(x,min,max)                    \
do                                          \
{                                           \
    if (x <= min)                           \
    {                                       \
        x = min;                            \
    }                                       \
    if (x >= max)                           \
    {                                       \
        x = max;                            \
    }                                       \
} while (0)

#endif
