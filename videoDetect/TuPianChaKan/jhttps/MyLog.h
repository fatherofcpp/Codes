#ifndef _MYLOG_H_
#define _MYLOG_H_

#include "stdafx.h"
#include <time.h>

#define MS_CHAR char
#define MS_INT32 int
#define MU_INT32 unsigned int
#define DEF_MAX_CHARSTR_LENGHT2		128
#define DEF_MSG_PRINT		0x00000001
#define DEF_MSG_SEND		0x00000002
#define DEF_MSG_SAVE		0x00000004
#define DEF_BUFFER_LENGTH_1M        (1024*1024)
#define MD_OK			0x00000000



void getTimeBuf(MS_CHAR *pBuf);
MS_INT32 LogPrintf(MS_CHAR *msg,MU_INT32 flag,MU_INT32 type);
static MS_INT32 write_log(MS_CHAR* pbuf,MS_INT32 len);


#endif