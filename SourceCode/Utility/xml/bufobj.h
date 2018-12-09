#pragma once
#include "mty.h"
namespace NSP_STP_CM
{
	typedef struct
	{
		M_BOOL      canfree;
		MS_INT32    size;
		MS_INT32    used;
		MS_INT32    align;
		MS_CHAR    *buf;
	} BUFOBJ;

	extern BUFOBJ     *f_bufobj_new(BUFOBJ *const bufobj, MS_INT32 size, MS_INT32 align);
	extern MS_INT32    f_bufobj_resize(BUFOBJ *const bufobj, MS_INT32 size);
	extern void        f_bufobj_del(BUFOBJ *bufobj);
}
