#ifndef _CHECK_SUM_H
#define _CHECK_SUM_H


#include "H2Ctype.h"

MS_INT32 CheckCommFlag(MU_INT32 flag);

MU_INT32 SetCommFlag(MU_INT32 flag);

MS_INT32 FillCheckSum(MU_CHAR *buf1, MS_INT32 len1, MU_CHAR *buf2, MS_INT32 len2);

MS_INT32 VerifyCheckSum(MU_CHAR *buf, MS_INT32 len);

#endif
