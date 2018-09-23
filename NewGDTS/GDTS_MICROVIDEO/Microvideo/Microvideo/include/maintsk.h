#ifndef _MAINTSK_H_
#define _MAINTSK_H_

#include "C2Htype.h"
#include "parse_http.h"

#define DEF_STARTED_FLAG_SEND2SERVER    0x00000001
#define DEF_STARTED_FLAG_SEND2CLIENT    0x00000002
#define DEF_STARTED_FLAG_RESERVER       0x00000004

#define DEF_STARTED_FLAG_COMPLETE       0x00000003

#define DEF_MAX_NO_SUPPORT  20

typedef struct S_HTTP_URL
{
	MS_CHAR uri[64];
	MS_CHAR param[64];
    MS_CHAR uid[32];
    MS_CHAR gid[32];
    MS_CHAR username[32];
    MS_CHAR password[32];
	MU_INT32 number;
    MS_CHAR host[16];
	MU_INT32 port;

    MU_INT32 m_hour;
	MU_INT32 m_flag;//今天是否已经重启

    REALTIME_METRICS m_rtMetrics;
}HTTP_URL;



MS_INT32 createDetectData(MS_CHAR *pBuf);

MS_INT32 createXml(MS_CHAR *pBuf,MS_INT32 *pLen);

MS_INT32 sendToServer(MS_CHAR *pBuf,MS_INT32 nLen,void *pUrl);

void *sendStateTsk(void *p);

void startMainTsk( void );

MS_INT32 parse_http_head(MS_CHAR *const buf,MS_INT32 *pLen);

MS_INT32 changeDevState(MU_INT32 flag);

//MS_INT32 parse_xml_content(MS_CHAR *const buf,MS_INT32 nLen);

MS_INT32 getDevIp(MS_CHAR* pIpBuf,const MS_CHAR *pEth);

MS_INT32  pushState2Queues(MS_CHAR *pSrc,MS_CHAR srcLen);



#endif 

