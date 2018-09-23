#ifndef _IO_H_
#define _IO_H_


#include "H2Ctype.h"
#include <stdio.h>

typedef enum
{
	READ_ONLY = 0,

	WRITE_ONLY,

	READ_WRITE

}OPERATE_MODE;

MU_INT32 openIO(MS_CHAR *name,MU_INT32 mode);

MS_INT32 closeIO(MU_INT32 id);

MU_INT32 readIO(MU_INT32 id,MU_INT8 *buf,MU_INT32 size);

MU_INT32 writeIO(MU_INT32 id,MU_INT8 *buf,MU_INT32 size);

MU_INT32 getIOsize(MU_INT32 id);

MU_INT32 seekIO(MU_INT32 id,MU_INT32 offset,MU_INT32 flag);


void my_trace(const MS_CHAR *msg);


#endif

