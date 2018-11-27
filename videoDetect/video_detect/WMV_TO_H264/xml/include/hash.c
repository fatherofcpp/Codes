/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: hash.c 1699 2009-02-04 06:33:53Z dengxf $
 */
#include "mty.h"

#include "hash.h"

MU_INT32 HashOnlyForVCODE(MS_CHAR *_str, MU_INT32 length)
{
    MU_INT32 seed = 131, hash = 0, i;
    MS_INT8 *str = (MS_INT8 *)_str;

    for(i = 0; i < length; i++)
    {
        hash = (hash*seed)+str[i];
    }
    return (hash & 0x7FFFFFFF);
}

MU_INT32 HashForAid(MS_CHAR *_str, MU_INT32 length)
{
    MU_INT32 hash = 5381, i;
    MS_INT8 *str = (MS_INT8 *)_str;

    for(i = 0; i < length; i++)
    {
        hash = ((hash << 5) + hash) + str[i];
    }
    return (hash & 0x7FFFFFFF);
}

static MU_INT32 _HashAdd(MU_INT32 h, MU_CHAR c)
{
    h += (h << 5);
    return (h ^ c);
}

#define DEF_MDB_HASHSTART   5381

MU_INT32 Hash_Str(const void * M_RESTRICT value, MU_INT32 len)
{
    MU_INT32 h;
    const MS_INT8 *p = (const MS_INT8 *)value;

    h = DEF_MDB_HASHSTART;
    while (len != 0)
    {
        h = _HashAdd(h, *p++);
        --len;
    }
    return (h);
}

void Hash_Str_Ext(const void * M_RESTRICT value, MU_INT32 len, void * M_RESTRICT dst)
{
    MU_INT32 hid;
    MU_INT32 *p = (MU_INT32 *)dst;

    hid = Hash_Str(value, len);
    *p = hid;
}
