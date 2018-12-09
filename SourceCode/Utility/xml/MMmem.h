#pragma once
#include "mty.h"
namespace NSP_STP_CM
{
#define M_FREE(mm)                          \
	{                                           \
	if (mm != NULL)                         \
	{                                       \
	free(mm);                      \
	mm = NULL;                          \
	}                                       \
	}

	extern void   *mmem_malloc(MU_INT32 siz, MU_INT32 align = 8);
	extern void    mmem_free(void *real);
}

