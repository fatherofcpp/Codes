

#ifndef DEF_TSLIB_H_
#define DEF_TSLIB_H_

#if 1

#include "ts2frame.h"
//ts
typedef void (__stdcall*INITTSFUNC)(void *pParams,void *CALLFUNC);
typedef void (__stdcall*PROCESSTSDATA)(void *pParams,unsigned char *data, int len);
typedef void (__stdcall*PARSEPES)(void *hHandle,unsigned char* buf, int len, void* pES);

extern INITTSFUNC InitTsFunc;
extern PROCESSTSDATA ProcessTSData;
extern PARSEPES ParsePES;

int loadTSLib();
#endif
#endif
