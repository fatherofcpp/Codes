#ifndef _COMMAND_H_
#define _COMMAND_H_
/*
 * ͨѶЭ���Ϊ��������
 * 1. HDR, ����Ϊ4 * 8 = 32�ֽ�.
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
    MU_INT32    size;       /* �������ݳ���(��body) */
    
    MU_INT32	check;
    
    MU_INT8     dev;        /*
                             * ����Ӧ��ģ������
                             * 0x01 -- "����" �� "�ӿ�" ͨѶ
                             * 0x02 -- "RECV" �� "EXT" ͨѶ
							 * 0x03 -- "ת�������" �� "��������" ͨѶ
                             */

    MU_INT8     direct;     /*
                             * �������ķ���
                             * 0x01 --  ��ʾ "����" ==> "�ӿ�"; "RECV" ==> "ETX" ;"ת�������" ==> "��������"
                             * 0x02 --  ��ʾ "�ӿ�" ==> "����"; "EXT"  ==> "RECV" ��"��������" ==> "ת�������"
                             */

    MU_INT8     type;       /*
                             * ������������
                             *      ���λ:
                             *      0x80    --  RESP��, ��Ϊ��Ӧ����ʱ��, type = recved_type | 0x80
                             *
                             *      ��7λ:
                             *      1.  ���� BRD "����" �� DM64X "�ӿ�" ͨѶ
                             *      1.1 ����"����"����"�ӿ�" 
                             *          0x01    --  START   ֪ͨ��ʼ����, �����ǰ����ֹͣ״̬���򱨴���״̬����Ϊ "����" 
                             *          0x02    --  STOP    ֹͣ��ǰ������, ����״̬����Ϊ "ֹͣ" 
                             *          0x03    --  SET     ���õ�ǰ�Ĳ���, ֻ�е�ǰ״̬�� "ֹͣ"��ʱ�򣬲ſ�������
                             *          0x04    --  GET     ��ȡ��ǰ�Ĳ���
                             *          0x05    --  PING    ���Ƿ�������
                             *
                             *      1.2 ����"�ӿ�"����"����" 
                             *          0x06    --  WARN    ������Ϣ
                             *          0x07    --  VIDEO   �����������Ƶ, �ж����Ľṹ
                             *          0x08    --  AUDIO   �����������Ƶ, �ж����Ľṹ
                             *
                             *      2.  ����MSS / EXT ͨѶ
                             *      2.1 ����"MSS"����"EXT" 
                             *          0x01    --  INIT    ��ʼ������������
                             *          0x02    --  HDR     Header
                             *          0x03    --  PKT     Data Packet
                             *          0x04    --  END     ����
							 *
							 *      3.  ����ת������� / �������� ͨѶ
                             *      3.1 ����"��������"����"ת�������" 
                             *          0x01    --  INIT    �������
                             *          0x02    --  H264    h264������
                             *          0x03    --  YUV     ͼ���YUV����
                             *          0x04    --  PCM     PCM����
							 *      3.2 ����"ת�������"����"��������" 
                             *          0x0F    --  STOP    ֪ͨ���������˳�
                             */

    MU_INT8     devid;      /*
                             * �����豸ID
                             * Ŀǰ������������ķ�������ͨ����
                             */

    MU_INT32    body_len;   /*
                             * ����BODY�ĳ���
                             */

    MU_INT32    seq;        /*
                             * ����ͨѶ�����к�
                             * �� REQ/RESP����У������кű��ֲ���
                             */

    MU_INT32    ext_info;   /*
                             * ����������Ϣ, ���漸�����
                             * 1. ����RESP���еĴ�����Ϣ����;
                             * 2. ����AUDIO/VIDEO�е�STREAM ID������;
                             * 3. �������� "RECV" ==> "EXT" �� HASH(NAME);
                             * 4. �������Ϊ 0
                             */

    MU_INT32    ident;      /*
                             * �������ͷ������, Ψһ�ı��
                             * 0x00001001   -- Davanic Host
                             * 0x00001002   -- MSS RECV
                             * 0x00001003   -- MSS ADMIN
                             * 0x00001008   -- MSS 3GP RECV
                             *
                             * 0x00002001   -- DM642(��Ϊ�ӿ�)
							 * 0x00003001   -- �����ӳ���
                             */
    /*
     * ���Ϲ�20���ֽ�, ����12���ֽڱ����Ժ���չʹ��
     */
    MU_INT32    none1;		/*
                             * ��typeΪH264��YUVʱ����ʾͼ��Ŀ�
                             */
    MU_INT32    none2;		/*
                             * ��typeΪH264��YUVʱ����ʾͼ��ĸ�
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
    MS_INT32    none[4];    /* �����Ժ���չʹ�� */
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