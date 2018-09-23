/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: c_defs.h 1699 2009-02-04 06:33:53Z dengxf $
 */
#include "mty.h"

#ifndef _C_DEFS_H_
#define _C_DEFS_H_

#include <stdio.h>
#include <stdlib.h>

#define DEF_PULL_PREFIX             "PULLING_"
#define DEF_VOD_ROOT                "vod"
#define DEF_EDITOR_DIR              "cut"
#define DEF_EDITOR_ROOT             DEF_VOD_ROOT"/"DEF_EDITOR_DIR

#define DEF_MAX_BACKLOG             64

#ifdef DEF_DEBUG_FD
#define M_CLOSE(mm)                 \
do                                  \
{                                   \
    printf("::::: CLOSE: %d\n", mm);\
    if (mm > 0)                     \
    {                               \
        close(mm);                  \
        mm = -1;                    \
    }                               \
} while (0)
#else
#define M_CLOSE(mm)                 \
do                                  \
{                                   \
    if (mm > 0)                     \
    {                               \
        close(mm);                  \
        mm = -1;                    \
    }                               \
} while (0)
#endif

typedef struct  iovec               IOVEC;

#define DEF_IP_LEN                  64

typedef struct
{
    MS_CHAR     ip[DEF_IP_LEN];
    MS_INT32    port;
} NETADDR;

typedef struct
{
    MS_CHAR        _protocol[DEF_STR_LEN_MIN];
    MS_INT32        protocol;
    MS_INT32        port;
    MS_INT32        hasdir;
    MS_INT32        uri_ft;
    MS_INT32        hosttype;
    MS_CHAR         host[DEF_STR_LEN_STD];
    MS_CHAR        _uri[DEF_STR_LEN_STD];
    MS_CHAR         uri[DEF_STR_LEN_STD];
    MS_CHAR         param[DEF_STR_LEN_MAX];
} RFC_URL;

typedef struct
{
    MS_INT32            count;
    MS_INT32            play_type;
    MS_INT32            sseq;
    MS_CHAR            *packet;
} SDATA;

#define DEF_HOSTTYPE_IP_ANY     0x80
#define DEF_HOSTTYPE_IPV6       0x06
#define DEF_HOSTTYPE_IPV4       0x04
#define DEF_HOSTTYPE_HOSTNAME   0x00

#endif
