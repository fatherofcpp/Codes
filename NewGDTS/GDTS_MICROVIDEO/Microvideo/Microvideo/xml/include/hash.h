/*
 * Copyright (c) 2003-2008 Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 * $Id: hash.h 1310 2008-04-06 02:02:59Z dengxf $
 */

#ifndef _HASH_H_
#define _HASH_H_

#ifdef __cplusplus 
extern "C" { 
#endif

#include "mty.h"

MU_INT32 HashOnlyForVCODE(MS_CHAR *str, MU_INT32 length);

MU_INT32 HashForAid(MS_CHAR *str, MU_INT32 length);

MU_INT32 Hash_Str(const void * M_RESTRICT value, MU_INT32 len);

MU_INT32 Hash_new(MS_CHAR *str, MS_INT32 len);

#ifdef __cplusplus 
} 
#endif

#endif
