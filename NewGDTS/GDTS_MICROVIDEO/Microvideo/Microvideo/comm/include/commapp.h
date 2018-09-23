#ifndef _COMM_APP_H
#define _COMM_APP_H

#include "C2Htype.h"

MS_INT32 openNewSession(MS_INT32 *cmid,MS_CHAR *pServerAddr,MU_INT32 m_port);

MS_INT32 closeSession(MU_INT32 *ComId);

MS_INT32 initComToBoard(MU_INT32 chn,MU_INT32 *ComId);

MS_INT32 closeComToBoard(MU_INT32 *ComId);

MS_INT32 recvComFrBoard(MU_INT32 ComId,MS_CHAR *recvBuf,MU_INT32 bufLen);

MS_INT32 sendComToBoard(MU_INT32 ComId,MS_CHAR *sendBuf1,MU_INT32 bufLen1,MS_CHAR *sendBuf2,MU_INT32 bufLen2);

MS_INT32 sendComToHostW(MU_INT32 ComId,MS_CHAR *sendBuf1,MU_INT32 bufLen1,MS_CHAR *sendBuf2,MU_INT32 bufLen2);

MS_INT32 sendComToHostIm(MU_INT32 ComId,MS_CHAR *sendBuf1,MU_INT32 bufLen1,MS_CHAR *sendBuf2,MU_INT32 bufLen2);

MS_INT32 recvComFrHost(MU_INT32 ComId,MS_CHAR *recvBuf,MU_INT32 bufLen);
#endif

