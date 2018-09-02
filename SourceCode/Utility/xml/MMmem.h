#pragma once


#include <string.h>

//

#ifndef WIN32
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
#include "xr.h"

namespace NSP_STP_CM
{
	//

#define MD_OK			0x00000000

#define MD_FAILED		0xFFFFFFFF
#define MD_INVALID		0xFFFFFFFF
	//

	//

#define XR_DEV 			(0x1000<<16)//0x1fff  #define XR_DEV 0x1000 - 0x10FF

#define DEF_LEVEL_1		(0x01<<13)//0x0100
#define DEF_LEVEL_2		(0x02<<13)
#define DEF_LEVEL_3		(0x03<<13)
#define DEF_LEVEL_4		(0x04<<13)
#define DEF_LEVEL_5		(0x05<<13)
#define DEF_LEVEL_6		(0x06<<13)
#define DEF_LEVEL_7		(0x07<<13)


#define DEF_BOARD_FAILED_CLASS					 (XR_DEV|XR_F | (XR_WINF<<29))
#define DEF_BOARD_NORMAL_CLASS				   	 (XR_DEV|XR_S | (XR_GINF<<29))

#define DEF_BOARD_SUCCESS_CLASS					 (DEF_BOARD_NORMAL_CLASS|0x1000)	
#define DEF_BOARD_UPDATE_SUCCESS				 (DEF_BOARD_SUCCESS_CLASS+0x01)	//升级成功


	//#define DEF_VOARD_DETECT_CLASS					 (DEF_BOARD_NORMAL_CLASS|0x1010)					//音视频检测信息
	//#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_1		 (DEF_VOARD_DETECT_CLASS+0x01)	//第一路无视频信号
	//#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_2		 (DEF_VOARD_DETECT_VIDEO_NOSIGNAL_1+0x01)	//第二路无视频信号
	//#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_3		 (DEF_VOARD_DETECT_VIDEO_NOSIGNAL_2+0x01)	//第三路无视频信号
	//#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_4		 (DEF_VOARD_DETECT_VIDEO_NOSIGNAL_3+0x01)	//第四路无视频信号

#define DEF_BOARD_STATUS_MASK					 0xFFFFF800
#define DEF_BOARD_STATUS_CLASS					 (DEF_BOARD_NORMAL_CLASS|0x1800)
#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_1		 (DEF_BOARD_STATUS_CLASS+0x01)	//第一路无视频信号
#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_2		 (DEF_BOARD_STATUS_CLASS+0x02)	//第二路无视频信号
#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_3		 (DEF_BOARD_STATUS_CLASS+0x04)	//第三路无视频信号
#define DEF_VOARD_DETECT_VIDEO_NOSIGNAL_4		 (DEF_BOARD_STATUS_CLASS+0x08)	//第四路无视频信号

#define DEF_BOARD_UPDATING						 (DEF_BOARD_STATUS_CLASS+0x10)//0x0002//系统升级中



#define DEF_BOARD_SYSTEM_CLASS			  		 (DEF_BOARD_FAILED_CLASS|0x0000)					//系统信息
#define	DEF_BOARD_POINTER_IS_NULL				 (DEF_BOARD_SYSTEM_CLASS+0x01)	//指针为空
#define	DEF_BOARD_MEMORY_NOT_ENOUGH				 (DEF_BOARD_SYSTEM_CLASS+0x02)	//内存不足
#define	DEF_BOARD_UPDATE_FAILED					 (DEF_BOARD_SYSTEM_CLASS+0x03)	//升级失败

#define	DEF_BOARD_SOCKET_CLASS					 (DEF_BOARD_FAILED_CLASS|0x0010)					// "网络信息"
#define	DEF_BOARD_SOCKET_CONNECT_TIMEOUT		 (DEF_BOARD_SOCKET_CLASS+0x01)	// "TCP连接超时"},
#define	DEF_BOARD_SOCKET_RECV_TIMEOUT			 (DEF_BOARD_SOCKET_CLASS+0x02)	// "接收超时"},
#define	DEF_BOARD_SOCKET_RECV_ERROR				 (DEF_BOARD_SOCKET_CLASS+0x03)	// "接收错误"},
#define	DEF_BOARD_SOCKET_SEND_TIMEOUT			 (DEF_BOARD_SOCKET_CLASS+0x04)	// "发送超时"},
#define	DEF_BOARD_SOCKET_SEND_ERROR				 (DEF_BOARD_SOCKET_CLASS+0x05)	// "发送错误"},
#define DEF_BOARD_HEAD_PACKAGE_LAWLESS			 (DEF_BOARD_SOCKET_CLASS+0x06)	// "数据包头非法"

#define	DEF_BOARD_XML_INCORRECT_FORMAT			 (DEF_BOARD_FAILED_CLASS|0x0020)					// "XML内容格式不正确"
#define DEF_BOARD_XML_SOFTWARE_VGA				 (DEF_BOARD_XML_INCORRECT_FORMAT+0x01)			//VGA
#define DEF_BOARD_XML_SOFTWARE_CVBS				 (DEF_BOARD_XML_INCORRECT_FORMAT+0x02)			//CVBS

#define DEF_BOARD_VIDEO_REGISTER_ERROR			 (DEF_BOARD_XML_INCORRECT_FORMAT+0x03)			//视频编码类型不支持
	//

	//

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
	//

	void *MM_alloc(MU_INT32 size,MU_INT32 align);
	void MM_free(void *addr,MU_INT32 size);
	void *MM_calloc(MU_INT32 size,MS_INT8 value,MU_INT32 align);
	void *my_calloc(MU_INT32 size);
	void my_free(void *addr,MU_INT32 size);

	void   *mmem_malloc(MU_INT32 alignment, MU_INT32 siz);
	void    mmem_free(void *real);

}

