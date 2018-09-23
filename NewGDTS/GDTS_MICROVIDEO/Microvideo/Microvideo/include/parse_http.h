

#ifndef _DEF_PARSE_HTTP_H_
#define _DEF_PARSE_HTTP_H_

#include "C2Htype.h"

typedef struct strRealTimeMetrics{

    MS_CHAR ip[64];
    MS_CHAR devid[64];
    MS_CHAR devname[64];

    MU_INT32 pretime;
    MU_INT32 curtime;

    MS_INT32 TotalEnters;
    MS_INT32 TotalExits;

}REALTIME_METRICS;



void httpSvrTsk(void);



#endif// 











