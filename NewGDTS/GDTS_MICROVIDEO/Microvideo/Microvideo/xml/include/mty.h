/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: mty.h 1803 2009-04-04 14:01:34Z dengxf $
 */

#ifndef _MTY_H_
#define _MTY_H_



/*
 * ONLY SUPPORTED ILP32 AND LP64 
 */

/*
 * Data type   | ILP32 size    | ILP32 alignment   | LP64 size | LP64 alignment
 * char        | 1 byte        | 1 byte            | 1 byte    | 1 byte
 * short       | 2 bytes       | 2 bytes           | 2 bytes   | 2 bytes
 * int         | 4 bytes       | 4 bytes           | 4 bytes   | 4 bytes
 * float       | 4 bytes       | 4 bytes           | 4 bytes   | 4 bytes
 * long        | 4 bytes       | 4 bytes           | 8 bytes   | 8 bytes
 * pointer     | 4 bytes       | 4 bytes           | 8 bytes   | 8 bytes
 * long long   | 8 bytes       | 4 bytes           | 8 bytes   | 8 bytes
 * double      | 8 bytes       | 4 bytes           | 8 bytes   | 8 bytes
 */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define _ENV_OS_WIN32
#elif defined(__FreeBSD__)
#   define _ENV_OS_FREEBSD
#elif defined(__linux__) || defined(__linux)
#   define _ENV_OS_LINUX
#else
#   error "OS NOT SUPPORT"
#endif

#if defined(_MSC_VER)
#   define _ENV_CC_VC
#elif defined(__GNUC__)
#   define _ENV_CC_GCC
#else
#   error "CC NOT SUPPORT"
#endif

#if defined(_ENV_CC_VC)
#   if defined(_M_IX86)
#       define _ENV_ARCH_i386
#   else
#//       error "ARCH NOT SUPPORT"
#   endif
#elif defined(_ENV_CC_GCC)
#   if defined(__i386__) || defined(__i386) 
#       define _ENV_ARCH_i386
#   elif defined(__arm__) || defined(__arm)
#       define _ENV_ARCH_arm
#   elif defined(__amd64__)
#       define _ENV_ARCH_amd64
#   else
#       error "ARCH NOT SUPPORT"
#   endif
#endif

#if defined(_ENV_OS_WIN32)
#   if !defined(_ENV_ARCH_i386)
//#       error "ARCH NOT SUPPORT"
#   endif
#endif

typedef char                    M_CHAR;
typedef int                     M_INT;
typedef float                   M_FLOAT;
typedef double                  M_DOUBLE;

#if defined(_ENV_OS_WIN32)
#   include <windows.h>
#   define M_RESTRICT
#   define M_VOLATILE           volatile

    typedef char                MS_CHAR;
    typedef __int8              MS_INT8;
    typedef __int16             MS_INT16;
    typedef __int32             MS_INT32;
    typedef __int64             MS_INT64;
    typedef __int32             MS_INT;
    typedef signed long         MS_LONG;

    typedef unsigned char       MU_CHAR;
    typedef unsigned __int8     MU_INT8;
    typedef unsigned __int16    MU_INT16;
    typedef unsigned __int32    MU_INT32;
    typedef unsigned __int64    MU_INT64;
    typedef unsigned __int32    MU_INT;
    typedef unsigned long       MU_LONG;

	typedef signed int          MS_PINT;
#endif

#if defined(_ENV_OS_FREEBSD)
#   include <stdint.h>
#   define M_RESTRICT           restrict
#   define M_VOLATILE           volatile

    typedef char                MS_CHAR;
    typedef int8_t              MS_INT8;
    typedef int16_t             MS_INT16;
    typedef int32_t             MS_INT32;
    typedef int64_t             MS_INT64;
    typedef int32_t             MS_INT;
    typedef signed long         MS_LONG;

    typedef unsigned char       MU_CHAR;
    typedef uint8_t             MU_INT8;
    typedef uint16_t            MU_INT16;
    typedef uint32_t            MU_INT32;
    typedef uint64_t            MU_INT64;
    typedef uint32_t            MU_INT;
    typedef unsigned long       MU_LONG;
#endif

#if defined(_ENV_OS_LINUX)
#   include <stdint.h>
#   define M_RESTRICT
#   define M_VOLATILE           volatile

    typedef char                MS_CHAR;
    typedef int8_t              MS_INT8;
    typedef int16_t             MS_INT16;
    typedef int32_t             MS_INT32;
    typedef int64_t             MS_INT64;
    typedef int32_t             MS_INT;
    typedef signed long         MS_LONG;

    typedef unsigned char       MU_CHAR;
    typedef uint8_t             MU_INT8;
    typedef uint16_t            MU_INT16;
    typedef uint32_t            MU_INT32;
    typedef uint64_t            MU_INT64;
    typedef uint32_t            MU_INT;
    typedef unsigned long       MU_LONG;
#endif

typedef MS_INT32                M_BOOL;
#define M_FALSE                 0
#define M_TRUE                  1

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

typedef M_VOLATILE MU_LONG      M_ATOMIC;

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

#define M_OFTOFM(TYPE, MEMBER)  ((MU_INT32) &((TYPE *)0)->MEMBER)

#define M_LND(mod,x)            ((x) & ~(mod-1))
#define M_RND(mod,x)            (((x) + (mod-1)) & ~(mod-1))
#define M_POT(x)                ((((x) & ((x) - 1)) == 0) && (x != 0))

#define M_MOVECONST(x)          ((MU_LONG)x)

#define M_MKLETAG(a,b,c,d)      ((((d) & 0xFF) << 24) | (((c) & 0xFF) << 16) | (((b) & 0xFF) << 8) | ((a) & 0xFF))
#define M_MKBETAG(a,b,c,d)      ((((a) & 0xFF) << 24) | (((b) & 0xFF) << 16) | (((c) & 0xFF) << 8) | ((d) & 0xFF))

#define M_HTONS(x)              ((((x) >> 8) & 0xFF) | (((x) << 8) & 0xFF00))
#define M_HTONL(x)              (((x) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | ((x) << 24))

#define M_PAGE_SIZE             4096
#define M_PAGE_MASK             (~(M_PAGE_SIZE - 1))

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

#define M_GET_MS_LONG(to,pfrom)             \
do                                          \
{                                           \
    MS_LONG t_sl;                           \
    memcpy(&t_sl, pfrom, sizeof(MS_LONG));  \
    to = t_sl;                              \
} while (0)

#define M_GET_MU_LONG(to,pfrom)             \
do                                          \
{                                           \
    MS_LONG t_ul;                           \
    memcpy(&t_ul, pfrom, sizeof(MU_LONG));  \
    to = t_ul;                              \
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

#define M_SET_MS_LONG(pto,from)             \
do                                          \
{                                           \
    MS_LONG t_sl = from;                    \
    memcpy(pto, &t_sl, sizeof(MS_LONG));    \
} while (0)

#define M_SET_MU_LONG(pto,from)             \
do                                          \
{                                           \
    MS_LONG t_ul = from;                    \
    memcpy(pto, &t_ul, sizeof(MU_LONG));    \
} while (0)

#define M_SET_BUF M_GET_BUF

/* addr/long convert */
#define M_ADDR_2_ADDR(pto,pfrom)            \
do                                          \
{                                           \
    void *addr = pfrom;                     \
    memcpy(&pto, &addr, sizeof(void *));    \
} while (0)

#define M_ADDR_2_LONG(to,pfrom)             \
do                                          \
{                                           \
    void *addr = pfrom;                     \
    memcpy(&to, addr, sizeof(void *));      \
} while (0)

#define M_LONG_2_ADDR(pto,from)             \
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
