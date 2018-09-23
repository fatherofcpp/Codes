/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: base64.c 2365 2010-02-10 07:15:49Z dengxf $
 */
#include "mty.h"

#include <string.h>

#include "../include/base64.h"

static MS_CHAR encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static MS_CHAR decode_table[] = {
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77, 77, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
    77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 77,
    77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
};

MS_INT32 f_base64_encode(MS_CHAR *const dst, const void *src, MU_INT32 src_len)
{
    const MU_CHAR  *s;
    MU_CHAR        *d;
    MU_INT32        len;

    len = src_len;
    s = (const MU_CHAR *)src;
    d = (MU_CHAR *)dst;

    while (len > 2) {
        *d++ = encode_table[(s[0] >> 2) & 0x3F];
        *d++ = encode_table[((s[0] & 3) << 4) | (s[1] >> 4)];
        *d++ = encode_table[((s[1] & 0x0F) << 2) | (s[2] >> 6)];
        *d++ = encode_table[s[2] & 0x3F];

        s += 3;
        len -= 3;
    }

    if (len > 0)
    {
        *d++ = encode_table[(s[0] >> 2) & 0x3F];
        if (len == 1) {
            *d++ = encode_table[(s[0] & 3) << 4];
            *d++ = '=';

        } else {
            *d++ = encode_table[((s[0] & 3) << 4) | (s[1] >> 4)];
            *d++ = encode_table[(s[1] & 0x0F) << 2];
        }
        *d++ = '=';
    }
    len = d - (MU_CHAR *)dst;

    return (len);
}

MS_INT32 f_base64_decode(void *const dst, MS_CHAR *const src)
{
    MU_INT32    slen;
    MU_INT32    len;
    MU_CHAR    *d;
    MU_CHAR    *s;

    slen = strlen(src);
    for (len = 0; len < slen; len++)
    {
        if (src[len] == '=')
        {
            break;
        }
        if (decode_table[(MU_INT32)src[len]] == 77)
        {
            return (-1);
        }
    }
    if (len % 4 == 1)
    {
        return (-1);
    }
    s = (MU_CHAR *)src;
    d = (MU_CHAR *)dst;
    while (len > 3)
    {
        *d++ = (MU_CHAR) (decode_table[s[0]] << 2 | decode_table[s[1]] >> 4);
        *d++ = (MU_CHAR) (decode_table[s[1]] << 4 | decode_table[s[2]] >> 2);
        *d++ = (MU_CHAR) (decode_table[s[2]] << 6 | decode_table[s[3]]);
        s += 4;
        len -= 4;
    }
    if (len > 1)
    {
        *d++ = (MU_CHAR) (decode_table[s[0]] << 2 | decode_table[s[1]] >> 4);
    }
    if (len > 2)
    {
        *d++ = (MU_CHAR) (decode_table[s[1]] << 4 | decode_table[s[2]] >> 2);
    }
    len = d - (MU_CHAR *)dst;
    return (len);
}
