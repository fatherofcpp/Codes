#ifndef		_C2H_TYPE_H_
#define		_C2H_TYPE_H_


#ifdef __linux
#include <stdlib.h>
#include <stdio.h>
//#include <xdc/std.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#endif

#include <string.h>

#include "mty.h"
#include "rtntype.h"
//#include "../msg/include/msginfo.h"
#include "error_code.h"
#define DEF_MAX_NAMESTRING_LENGHT2            64
#define DEF_RECV_BUFFER_LEN			128*1024
#define DEF_SEND_BUGGER_LEN			48*1024
#define MD_BUSY               0x00000001
#define DEF_MSG_DEBUG                 0x00000004

#define DEF_BUFFER_LENGTH_1K        1024
#define DEF_BUFFER_LENGTH_1M        (1024*1024)

#define DEF_MAX_CHARSTR_LENGHT1		256
#define DEF_MAX_CHARSTR_LENGHT2		128
#define DEF_MAX_CHARSTR_LENGHT3		64
#define DEF_MAX_CHARSTR_LENGHT4		32

#define DEF_XML_HEAD "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"

#define DEF_MSG_PRINT		0x00000001
#define DEF_MSG_SEND		0x00000002
#define DEF_MSG_SAVE		0x00000004

#define DEF_ENV_DEV_FREQUENCY		10

#define DEF_MAX_LENGTH  (64*1024)

#define DEF_MAX_PER_PACKET_SIZE_SOCKET		32*1024//每次放入队列的最大数据量
#define DEF_MAX_PER_PACKET_SIZE_HPI			4*1024//每次放入队列的最大数据量

#define DEF_MICP_BODY_MEDIA_VIDEO      0x01
#define DEF_MICP_BODY_MEDIA_AUDIO      0x02

typedef void*              M_HANDLE;

#ifndef NULL
#define NULL      0
#endif

#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

#define M_ATTRIBUTE_PACK(n)         __attribute__ ((packed, aligned(n)))
/*
 *  constants
 */
#define SYS_FOREVER	-1	/* used by SEM_pend(), ... */
#define SYS_POLL	0	/* used by SEM_pend(), ... */

typedef enum
{
	SOCKET_TCP_MODE = 0,
	SOCKET_UDP_MODE,
	HPIC_MODE,
	MEM_MODE,
	FILE_MODE

}COMM_MODE;

typedef enum
{
     TRAFFIC_DETECT_MODE = 0,
     ENVIRON_DETECT_MODE,
     VIDEO_DETECT_MODE,
     GPS_BD_DETECT_MODE,
     CXYJ_DETECT_MODE,
     HUMANCOUNT_MODE

}DEV_MODE;





#endif

