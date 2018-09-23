#ifndef _THREAD_H_
#define _THREAD_H_

#include "C2Htype.h"

#ifndef WIN32
typedef struct
{
	MU_INT8		*funcExtent;
	MU_INT32	channelnum;
	
	M_HANDLE	m_lockHandle;
	
}THREAD_PARAM_BOARD;


typedef void (*THREAD_FUNC)(void *);

#define FOR_EVER 0xFFFFFFFF

M_HANDLE CreateThread(void *(*func)(void *),M_HANDLE params);
MS_INT32 CloseThread(M_HANDLE thrdID);
MS_INT32 WaitThreadEnd(M_HANDLE thrdID,MU_INT32 timeOut);
void ExitThread(void *rtn);


MS_INT32 CreatLock(M_HANDLE *lockHandle);
MS_INT32 CloseLock(M_HANDLE lockHandle);
MS_INT32 lock(M_HANDLE lockHandle);
MS_INT32 unlock(M_HANDLE lockHandle);


#endif



#endif

