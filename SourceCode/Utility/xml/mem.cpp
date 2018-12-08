#include "stdafx.h"

#ifdef CHIP_DM642
#include <std.h>
#include <tsk.h>

#endif

#include "MMmem.h"
#include "malloc.h"

using namespace NSP_STP_CM;


void *NSP_STP_CM::MM_alloc(MU_INT32 size,MU_INT32 /*align*/)
{
	return (void *)malloc(size);
}

void *NSP_STP_CM::MM_calloc(MU_INT32 size,MS_INT8 value,MU_INT32 align)
{
	void *ptr;

	ptr = MM_alloc(size,align);

	if (ptr != NULL)
		memset(ptr,value,size);

	return ptr;

}

void NSP_STP_CM::MM_free(void *addr,MU_INT32 /*size*/)
{
	if (addr != NULL)
		free(addr);

}


void *NSP_STP_CM::my_calloc(MU_INT32 size)
{

	return MM_calloc(size,0,16);

}




void NSP_STP_CM::my_free(void *addr,MU_INT32 size)
{
	MM_free(addr,size);

}



void   *NSP_STP_CM::mmem_malloc(MU_INT32 alignment, MU_INT32 siz)
{
	return MM_alloc(siz,alignment);

}

void    NSP_STP_CM::mmem_free(void *real)
{
	free(real);

}





