/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: bufobj.h 1936 2009-06-10 22:58:08Z dengxf $
 */
#ifndef _H_BUFOBJ_H_
#define _H_BUFOBJ_H_

typedef struct
{
    M_BOOL      canfree;
    MS_INT32    size;
    MS_INT32    used;
    MS_INT32    align;
    MS_CHAR    *buf;
} BUFOBJ;

BUFOBJ     *f_bufobj_new(BUFOBJ *const bufobj, MS_INT32 size, MS_INT32 align);
MS_INT32    f_bufobj_resize(BUFOBJ *const bufobj, MS_INT32 size);
void        f_bufobj_del(BUFOBJ *bufobj);

#endif
