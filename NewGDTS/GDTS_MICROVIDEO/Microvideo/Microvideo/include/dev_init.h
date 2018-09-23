#ifndef _DEV_INTT_H_
#define _DEV_INTT_H_

#include "C2Htype.h"
#ifndef WIN32
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#endif
//#include "../mem/include/MMmem.h"
//#include "../msg/include/msginfo.h"

#define DEF_DEV_DEVID				"devid"
#define DEF_DEV_TEMP				"devtemp"
#define DEF_DEV_HUMID				"devhumidity"
#define DEF_DEV_VISIB				"devvisibility"
#define DEF_DEV_WINDRECTION			"devwinddirection"
#define DEF_DEV_WINDSPEED			"devwindspeed"    
#define DEF_DEV_HOST_ADDR			"host_addr"
#define DEF_DEV_HOST_PORT			"host_port"

#define DEF_CLIENT_ADDR         "client_addr"
#define DEF_CLIENT_PORT         "client_port"

#define DEF_DEV_INDEX_TEMP				0x0001
#define DEF_DEV_INDEX_HUMID				0x0002
#define DEF_DEV_INDEX_VISIB				0x0004
#define DEF_DEV_INDEX_WINDRECTION		0x0008
#define DEF_DEV_INDEX_WINDSPEED			0x0010
/*
typedef enum
{
     TRAFFICE_DETECT_MODE = 0,
     ENVIRON_DETECT_MODE,
     VIDEO_DETECT_MODE

}DEV_MODE;
*/
typedef struct strSCOMMPARAMS{
    MS_CHAR m_com[32];
    MS_INT32 m_speed;
    MS_INT32 m_databits;
    MS_CHAR  m_parity;
    MS_INT32 m_stopbits;

}SCOMMPARAMS;

typedef struct strDEV_INFOR{
    MS_CHAR ip[16];
    MS_INT32 port;
}DEV_INFOR;

MS_INT32 initFileBuffer(void);
MS_INT32 pushFileBuffer(MS_CHAR *pBuf,MU_INT32 len);
MS_INT32 getFileBufferContent(MU_INT32 **pBuf);
MS_INT32 deleteFileBuffer(void);

MS_INT32 parseDevListXml(const MS_CHAR *const ubuf,DEV_MODE mode);

MU_INT32 initDevListXml(const MS_CHAR *pfile,DEV_MODE mode);

MS_INT32 initDevParams(void);

MS_INT32 LogPrintf(MS_CHAR *msg,MU_INT32 flag,MU_INT32 type);

void getTimeBuf(MS_CHAR *pBuf);

MS_INT32 parseRecvData(const MS_CHAR *const ubuf,MS_INT32 nLen );

#endif //_DEV_INTT_H_
