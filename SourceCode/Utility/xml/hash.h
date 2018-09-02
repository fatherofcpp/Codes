/*
* All rights reserved.
*/

#pragma once


#include "mty.h"

namespace NSP_STP_CM
{
	MU_INT32 HashOnlyForVCODE(MS_CHAR *str, MU_INT32 length);

	MU_INT32 HashForAid(MS_CHAR *str, MU_INT32 length);

	MU_INT32 Hash_Str(const void * M_RESTRICT value, MU_INT32 len);

	MU_INT32 Hash_new(MS_CHAR *str, MS_INT32 len);
}

