#ifndef _COMM_BASE_H
#define _COMM_BASE_H

#include "C2Htype.h"
//#include "../include/mysocket.h"


#define DEF_COMM_READ_WAITTIME_W		3000
#define DEF_COMM_READ_WAITTIME_IM		1000

#define DEF_COMM_WRITE_WAITTIME_W		3000
#define DEF_COMM_WRITE_WAITTIME_IM		1000	


typedef struct
{
	MS_CHAR		*m_serverAdd;
	MU_INT32	m_port;	

}SOCKET_PARAM;

MS_INT32 closeComm(MS_INT32 *cmid,COMM_MODE mode);
MS_INT32 openNewComm(MS_INT32 *cmid,M_HANDLE params,COMM_MODE mode,MU_INT32 timeout);
MS_INT32 CreateListenComm(MS_INT32 *scomId, MU_INT32 port,COMM_MODE mode);
MS_INT32 listenComm(MS_INT32 scomId,MS_INT32 *cmid,COMM_MODE mode);
MS_INT32 readDataFrmCommIm(MS_INT32 cmid,MU_INT8 *recvBuf,MU_INT32 recvLen,COMM_MODE mode);
MS_INT32 readDataFrmCommW(MS_INT32 cmid,MU_INT8 *recvBuf,MU_INT32 recvLen,COMM_MODE mode,MS_INT32 timeOut,MS_INT32 rtyTimes);
MS_INT32 sendDataToCommIm(MS_INT32 cmid,MU_INT8 *sendBuf1,MU_INT32 sendLen1,MU_INT8 *sendBuf2,MU_INT32 sendLen2,COMM_MODE mode);
MS_INT32 sendDataToCommW(MS_INT32 cmid,MU_INT8 *sendBuf1,MU_INT32 sendLen1,MU_INT8 *sendBuf2,MU_INT32 sendLen2,COMM_MODE mode,MS_INT32 timeOut,MS_INT32 rtyTimes);


#endif

