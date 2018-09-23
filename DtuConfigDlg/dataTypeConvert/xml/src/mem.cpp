#ifdef CHIP_DM642
#include <std.h>
#include <tsk.h>

#endif

#include "../include/MMmem.h"
#include "malloc.h"



void *MM_alloc(MU_INT32 size,MU_INT32 align)
{

	return (void *)malloc(size);


}

void *MM_calloc(MU_INT32 size,MS_INT8 value,MU_INT32 align)
{
	void *ptr;

	ptr = MM_alloc(size,align);

	if (ptr != NULL)
		memset(ptr,value,size);

	return ptr;

}

void MM_free(void *addr,MU_INT32 size)
{
	if (addr != NULL)
		free(addr);

}


void *my_calloc(MU_INT32 size)
{

	return MM_calloc(size,0,16);

}




void my_free(void *addr,MU_INT32 size)
{
	MM_free(addr,size);

}



void   *mmem_malloc(MU_INT32 alignment, MU_INT32 siz)
{
	return MM_alloc(siz,alignment);

}

void    mmem_free(void *real)
{
	free(real);

}





