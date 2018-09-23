#ifndef _MM_MEM_H_
#define _MM_MEM_H_

#include "include/C2Htype.h"
#include <string.h>

void *MM_alloc(MU_INT32 size,MU_INT32 align);
void MM_free(void *addr,MU_INT32 size);
void *MM_calloc(MU_INT32 size,MS_INT8 value,MU_INT32 align);
void *my_calloc(MU_INT32 size);
void my_free(void *addr,MU_INT32 size);

void   *mmem_malloc(MU_INT32 alignment, MU_INT32 siz);
void    mmem_free(void *real);

#endif

