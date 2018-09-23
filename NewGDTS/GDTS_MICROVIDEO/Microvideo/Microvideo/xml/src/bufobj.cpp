/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: bufobj.c 1939 2009-06-12 02:45:48Z dengxf $
 */
#include "../../stdafx.h"
#include "../include/mty.h"

#include <string.h>

#include "../include/mmem.h"
#include "../include/bufobj.h"

#define DEF_BUFOBJ_SIZE_MAX     (256 * 1024)
#define DEF_BUFOBJ_SIZE_STEP    4096

BUFOBJ *f_bufobj_new(BUFOBJ *const _bufobj, MS_INT32 size, MS_INT32 align)
{
    BUFOBJ *bufobj = _bufobj;

    if ((size < 0) || (size > DEF_BUFOBJ_SIZE_MAX))
    {
        return (NULL);
    }

    switch (align)
    {
        case 4:
        case 8:
        case 16:
        case 32:
        case 64:
        case 128:
        case 256:
        case 512:
        case 1024:
        case 2048:
        case 4096:
            break;

        default:
            return (NULL);
            break;
    }

    if (bufobj == NULL)
    {
        bufobj = (BUFOBJ *)M_MALLOC(sizeof(BUFOBJ));
        if (bufobj == NULL)
        {
            return (NULL);
        }
        memset(bufobj, 0, sizeof(BUFOBJ));
        bufobj->canfree = M_TRUE;
    }
    else
    {
        memset(bufobj, 0, sizeof(BUFOBJ));
        bufobj->canfree = M_FALSE;
    }
    bufobj->align = align;

    if (size > 0)
    {
        bufobj->size = size + bufobj->align - (size % bufobj->align);
        bufobj->buf = (MS_CHAR *)M_MALLOC(bufobj->size);
        if (bufobj->buf == NULL)
        {
            if (_bufobj == NULL)
            {
                M_FREE(bufobj);
            }
            return (NULL);
        }
    }

    return (bufobj);
}

MS_INT32 f_bufobj_resize(BUFOBJ *const bufobj, MS_INT32 size)
{
    MS_CHAR *buf;

    if ((size < 0) || (size > DEF_BUFOBJ_SIZE_MAX))
    {
        return (-1);
    }

    size += bufobj->align - (size % bufobj->align);
    if (size <= bufobj->size)
    {
        return (0);
    }

    buf = (MS_CHAR *)M_MALLOC(size);
    if (buf == NULL)
    {
        return (-1);
    }

    if (size > bufobj->size)
    {
        memcpy(buf, bufobj->buf, bufobj->size);
        memset(buf + bufobj->size, 0, size - bufobj->size);
    }

    M_FREE(bufobj->buf);
    bufobj->buf = buf;
    bufobj->size = size;

    return (0);
}

void f_bufobj_del(BUFOBJ *bufobj)
{
    M_FREE(bufobj->buf);
    if (bufobj->canfree == M_TRUE)
    {
        M_FREE(bufobj);
    }
}

