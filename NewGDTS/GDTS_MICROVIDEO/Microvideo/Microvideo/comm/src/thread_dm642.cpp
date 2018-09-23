//#include "stdafx.h"
#include "C2Htype.h"
#include "../include/thread.h"
#include <Windows.h>

MS_INT32 CreatLock(M_HANDLE *lockHandle)
{
	HANDLE			m_lockhandle;
	m_lockhandle	=	CreateMutex(NULL,TRUE,NULL);

	if (m_lockhandle == NULL)
		return MD_FAILED;

	ReleaseMutex(m_lockhandle);

	*lockHandle = m_lockhandle;

	return MD_OK;


}

MS_INT32 CloseLock(M_HANDLE lockHandle)
{

	CloseHandle(lockHandle);
	return MD_OK;

}

MS_INT32 lock(M_HANDLE lockHandle)
{

	WaitForSingleObject(lockHandle,INFINITE);


	return MD_OK;
}

MS_INT32 unlock(M_HANDLE lockHandle)
{
	ReleaseMutex(lockHandle);
	return MD_OK;

}

void ms_sleep(MU_INT32 nticks)
{

	Sleep(nticks);
	
}


