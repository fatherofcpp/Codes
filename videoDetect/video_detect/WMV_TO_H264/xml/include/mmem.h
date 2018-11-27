/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id$
 */
#include "mty.h"

#ifndef _MMEM_H_
#define _MMEM_H_
/*
#ifdef DEF_DEBUG_MMEM

void   *mmem_malloc(MU_INT32 alignment, MU_INT32 siz, const MS_CHAR *file, MS_INT32 line);
void    mmem_free(void *real, const MS_CHAR *file, MS_INT32 line);
//#define MMEM_MALLOC(mmalignment, mmsiz)     mmem_malloc(mmalignment, mmsiz, __FILE__, __LINE__)
//#define MMEM_FREE(mm)                       mmem_free(mm, __FILE__, __LINE__)

#else



#ifdef UNDER_CE
#define mmem_malloc(a,b) malloc(b)
#define mmem_free(b)     free(b)
#else

#endif

void   *mmem_malloc(MU_INT32 alignment, MU_INT32 siz);
void    mmem_free(void *real);

#define MMEM_MALLOC     					mmem_malloc
#define MMEM_FREE                           mmem_free

#endif
*/
#define mmem_malloc(a,b) malloc(b)
#define mmem_free(b)     free(b)

#define MMEM_MALLOC     					mmem_malloc
#define MMEM_FREE                           mmem_free
/*
 * END
 */
#define M_MALLOC(mmsiz)                     MMEM_MALLOC(8, mmsiz)

#define M_FREE(mm)                          \
do                                          \
{                                           \
    if (mm != NULL)                         \
    {                                       \
        MMEM_FREE(mm);                      \
        mm = NULL;                          \
    }                                       \
} while (0)

#define M_FREE_S(mm)                        \
do                                          \
{                                           \
    if (mm != NULL)                         \
    {                                       \
        MMEM_FREE(mm);                      \
    }                                       \
} while (0)

#endif

