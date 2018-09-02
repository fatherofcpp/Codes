/*
* All rights reserved.
*
* $Id$
*/

#pragma once

#include "mty.h"

namespace NSP_STP_CM
{
	void   *mmem_malloc(MU_INT32 alignment, MU_INT32 siz);
	void    mmem_free(void *real);

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

}

