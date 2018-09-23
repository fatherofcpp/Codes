
#ifndef _VNMP_DATAWORK_APP_H_
#define _VNMP_DATAWORK_APP_H_

#include "stdafx.h"
#include "C2Htype.h"
#include "command.h"
#include "inc.h"
#include "VNMP.h"
#include "VNMPDlg.h"

//MS_INT32 GetParamFrmHost(MS_INT32 comid);

//MS_INT32 SendUpdateToHost(MS_INT32 comid);

//MS_INT32 SendRespToHost(MS_INT32 comid);

MU_INT32 sendFrmBufToServer(MU_INT32 nChannel,MU_INT8 *send_buf);

MU_INT32 sendStateToServer(MU_INT8 *pBuf1,MS_INT32 len1,MU_INT32 *pBuf2,MS_INT32 len2,MS_CHAR *pIp,MS_INT32 port);

//MU_INT32 sendDataFrmBufW(MU_INT8 *send_buf);

//MU_INT32 sendStateInfo(MS_INT32 Comid);//,DSP_STAT *pdsp_stat

//MU_INT32 sendWarnFrmBuf(MS_INT32 Comid);

//MS_INT32 SendTimeRequest(MS_INT32 comid);
MS_INT32 checkHumanResponse(MS_INT32 s);

MU_INT32 sendDataToServer(MU_INT8 devId);
MS_INT32 FormatFrameHead(MS_CHAR* dst,MU_INT8 devid,MU_INT32 bodyLen);
MU_INT32 sendRequireToServer(MU_INT8 nDevId);

MS_INT32 checkGDServerResponse(MS_INT32 s);

MS_INT32 parseGDContent(MS_CHAR*pXmlBuf);



#endif

