/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: massert.h 1699 2009-02-04 06:33:53Z dengxf $
 */
#include "mty.h"

#ifndef _MASSERT_H_
#define _MASSERT_H_

#include "assert.h"

#ifdef _ENV_OS_LINUX
#define _MASSERT(e, msg)        ((e) ? (void)0 : __assert_fail(__func__, __FILE__, __LINE__, #e))
#else
#define _MASSERT(e, msg)        ((e) ? (void)0 : __assert(__func__, __FILE__, __LINE__, #e))
#endif

#ifdef DEF_MASSERT_USED

#   define MASSERT              _MASSERT

#else

#   define MASSERT(...)

#endif

#endif
