#ifndef _COMMAND_H_
#define _COMMAND_H_
/*
 * 通讯协议分为两个部分
 * 1. HDR, 长度为4 * 8 = 32字节.
 * 2. BODY
 */
//#include "C2Htype.h"
/* dev */
#define DEF_MICP_DEV_HOST                   0x01
#define DEF_MICP_DEV_BRD                   	0x02

/* direct for davinci */
#define DEF_MICP_DIRECT_HOST_TO_BRD	        0x01
#define DEF_MICP_DIRECT_BRD_TO_HOST	        0x02

/* direct for recv */
#define DEF_MICP_DIRECT_TO_RECV             0x01
#define DEF_MICP_DIRECT_TO_EXT              0x02

#define DEF_MICP_TYPE_RESP                  0x80
#define DEF_MICP_TYPE_BRD_START             0x01
#define DEF_MICP_TYPE_BRD_STOP              0x02
#define DEF_MICP_TYPE_BRD_SET               0x03
#define DEF_MICP_TYPE_BRD_GET               0x04
#define DEF_MICP_TYPE_BRD_PING              0x05
#define DEF_MICP_TYPE_BRD_WARN              0x06
#define DEF_MICP_TYPE_BRD_VIDEO             0x07
#define DEF_MICP_TYPE_BRD_AUDIO             0x08
#define DEF_MICP_TYPE_BRD_ADJUST            0x09
//#define DEF_MICP_TYPE_BRD_UPDATA			0x0A

#define DEF_MICP_TYPE_BRD_UPDATEINFO		0x0A
#define DEF_MICP_TYPE_BRD_UPDATE			0x0B

#define DEF_MICP_TYPE_MSS_HDR               0x01
#define DEF_MICP_TYPE_MSS_PKT               0x02
#define DEF_MICP_TYPE_MSS_END               0x03
#define DEF_MICP_TYPE_MSS_CHK               0x04


#define DEF_MICP_IDENT_BRD_DM642			0x00002001
#define DEF_MICP_IDENT_HOST_DAVANIC			0x00001001

typedef struct
{
    MU_INT32    size;       /* 整个数据长度(含body) */
    
    MU_INT32	check;
    
    MU_INT8     dev;        /*
                             * 描述应用模块类型
                             * 0x01 -- "主卡" 和 "子卡" 通讯
                             * 0x02 -- "RECV" 和 "EXT" 通讯
							 * 0x03 -- "转码服务器" 和 "拉流程序" 通讯
                             */

    MU_INT8     direct;     /*
                             * 描述包的方向
                             * 0x01 --  表示 "主卡" ==> "子卡"; "RECV" ==> "ETX" ;"转码服务器" ==> "拉流程序"
                             * 0x02 --  表示 "子卡" ==> "主卡"; "EXT"  ==> "RECV" ；"拉流程序" ==> "转码服务器"
                             */

    MU_INT8     type;       /*
                             * 描述包的类型
                             *      最高位:
                             *      0x80    --  RESP包, 当为回应包的时候, type = recved_type | 0x80
                             *
                             *      低7位:
                             *      1.  用于 BRD "主卡" 和 DM64X "子卡" 通讯
                             *      1.1 用于"主卡"发给"子卡" 
                             *          0x01    --  START   通知开始运行, 如果当前不是停止状态，则报错。把状态设置为 "运行" 
                             *          0x02    --  STOP    停止当前的任务, 并把状态设置为 "停止" 
                             *          0x03    --  SET     设置当前的参数, 只有当前状态是 "停止"的时候，才可以设置
                             *          0x04    --  GET     获取当前的参数
                             *          0x05    --  PING    看是否还在运行
                             *
                             *      1.2 用于"子卡"发给"主卡" 
                             *          0x06    --  WARN    报警信息
                             *          0x07    --  VIDEO   编码出来的视频, 有独立的结构
                             *          0x08    --  AUDIO   编码出来的音频, 有独立的结构
                             *
                             *      2.  用于MSS / EXT 通讯
                             *      2.1 用于"MSS"发给"EXT" 
                             *          0x01    --  INIT    初始化，包含参数
                             *          0x02    --  HDR     Header
                             *          0x03    --  PKT     Data Packet
                             *          0x04    --  END     结束
							 *
							 *      3.  用于转码服务器 / 拉流程序 通讯
                             *      3.1 用于"拉流程序"发给"转码服务器" 
                             *          0x01    --  INIT    请求参数
                             *          0x02    --  H264    h264纯数据
                             *          0x03    --  YUV     图像的YUV数据
                             *          0x04    --  PCM     PCM数据
							 *      3.2 用于"转码服务器"发给"拉流程序" 
                             *          0x0F    --  STOP    通知拉流程序退出
                             */

    MU_INT8     devid;      /*
                             * 描述设备ID
                             * 目前描述拉流程序的发送流的通道号
                             */

    MU_INT32    body_len;   /*
                             * 描述BODY的长度
                             */

    MU_INT32    seq;        /*
                             * 描述通讯的序列号
                             * 在 REQ/RESP组合中，此序列号保持不变
                             */

    MU_INT32    ext_info;   /*
                             * 描述附加信息, 下面几种情况
                             * 1. 用于RESP包中的错误信息描述;
                             * 2. 用于AUDIO/VIDEO中的STREAM ID的描述;
                             * 3. 用于描述 "RECV" ==> "EXT" 的 HASH(NAME);
                             * 4. 其他情况为 0
                             */

    MU_INT32    ident;      /*
                             * 描述发送方的身份, 唯一的编号
                             * 0x00001001   -- Davanic Host
                             * 0x00001002   -- MSS RECV
                             * 0x00001003   -- MSS ADMIN
                             * 0x00001008   -- MSS 3GP RECV
                             *
                             * 0x00002001   -- DM642(作为子卡)
							 * 0x00003001   -- 拉流子程序
                             */
    /*
     * 以上共20个字节, 下面12个字节保留以后扩展使用
     */
    MU_INT32    none1;		/*
                             * 当type为H264和YUV时：表示图像的宽
                             */
    MU_INT32    none2;		/*
                             * 当type为H264和YUV时：表示图像的高
                             */

    MU_INT32    none3;
} MICP_HDR;
/*
typedef struct
{
    MS_INT32    timstamp;
    MS_INT32    duration;
    MS_CHAR     data[];
} MICP_BODY_BRD_VIDEO;
*/
typedef struct tagSCCP_VIDEO
{
    MS_INT32    timstamp;
    MS_INT32    keyframe;//duration;
    MS_INT32    channel;
    MS_INT32    none[4];    /* 保留以后扩展使用 */
    MS_CHAR     *data;
} MICP_BODY_BRD_VIDEO; /* MICP_BODY_BRD_VIDEO */

typedef MICP_BODY_BRD_VIDEO MICP_BODY_BRD_AUDIO;
    

typedef struct tagSCCP_PARAMS
{
   MU_INT32    size;
   MU_INT32    type;
   MU_INT32    channel;
   MS_CHAR 	   flags[32];
   MU_CHAR     *parms;
}MICP_BODY_BRD_PARAMS;

#endif