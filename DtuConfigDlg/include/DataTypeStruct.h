#pragma once
/*
ȫ�ֽṹ��
*/
#include "include/mty.h"
#include <map>
#include <vector>
#include <deque>
#include <list>
using namespace std;

#define MAX_DEV_NUM (32)//ĳ�˿�����������
#define MAX_PORT_NUM (8)//�˿�������
#define MAX_SERVICE_NUM (4)//�ϱ�������������
#define MAX_NETLINK_NUM (1)//������·������
#define MESSAGE_BODY_IND (12)//��Ϣ����ʼλ��
#define MAX_NAME_LEN (50)//���ֳ���20utf8
#define MAX_WAYNAME_LEN (48)//���ֳ���48utf8
#define MAX_JZCODE_NUM (5)//����վ��Ÿ���
#define MAX_XML_LEN (1024 * 6)//���xml����
#define BUILD_NAME_MAX_LEN (50)
#define BUILD_POS_MAX_LEN (50)
#define MAX_WAY_NUM (12)//����·����
#define MAX_DEFINE_MODBUS_PROTOCOL (4 * 4)//����Զ���modbusЭ����
#define ALL_DEVTYPE_REPORT_PARAM_NUM (247)
#define DEV_ID_ERR (0xFFFFFFFFFFFFFFF)

#define HEAD_TAIL_LEN (4 + 4 + 4 + 2 + 4)

typedef map<int, int> MAP_INT_INT;
typedef map<int, string> MAP_INT_STRING;
typedef vector<string> VEC_STRING;
typedef vector<CString> VEC_CSTRING;
typedef vector<int> VEC_INT;


typedef struct
{
	int YY;//�豸ʱ��
	int MM;
	int DD;
	int HH;
	int MI;
	int SS;
}st_time;
typedef vector<st_time> VEC_TIME;

typedef enum 
{
	AM_3PHASE = 1,		//1:AM-3PHASE(3����);  
	AM_1PHASE,			//2:AM-1PHASE(1����); 
	AM_DC ,				//3:AM-DC(ֱ�����);   
	AM_HARM	,			//4:AM-HARM(г�����);  
	E_OP ,				//5:E-OP(���ܵ��);
	WM,					//6:WM(ˮ��);
	HM,					//7:HM(������)
	GM,					//8:GM(����);
}e_dev_type;
#define DEV_TYPE_NUM 8

typedef enum 
{
	AM_3PHASE_01 = 101,      //(DLT645-2007-3��-1·-����-���);      
	AM_3PHASE_02 = 102,     //(XHB-modbus.Vc-3��-4·-����-���);   
	AM_3PHASE_03 = 103,     //(XHB-modbus.Vb-3��-1·-����-���);        
	AM_3PHASE_04 = 104,     //(XHB-modbus.Va-3��-1·-����-���-����������);
	AM_3PHASE_05 = 105,     //(DLT645-1997-3��-1·-����-���);
	AM_3PHASE_DEF_01 = 111,     //�Զ���;
	AM_3PHASE_DEF_02,     //�Զ���;
	AM_3PHASE_DEF_03,     //�Զ���;
	AM_3PHASE_DEF_04,     //�Զ���;

	AM_1PHASE_01 = 201,     //(XHB-07-1��-1·-����-���);        
	AM_1PHASE_02 = 202,     //(XHB-modbus.Vc-1��-12·-����-���);       
	AM_1PHASE_03 = 203,     //(XHB-modbus.Vb-1��-3·-����-���);   
	AM_1PHASE_04 = 204,     //(XHB-modbus.Va-1��-3·-����-���);
	AM_1PHASE_DEF_01 = 211,		//�Զ���;
	AM_1PHASE_DEF_02,		//�Զ���;
	AM_1PHASE_DEF_03,		//�Զ���;
	AM_1PHASE_DEF_04,		//�Զ���;

	AM_DC_01 = 301,         //(XHB-07-12·-ֱ��-���);
	AM_DC_02 = 302,          //(XHB-modbus.vb-1·-ֱ��-���
	AM_DC_03 = 303,			//(XHB-modbus.vb-2·-ֱ��-���);
	AM_DC_DEF_01 = 311,			//�Զ���;
	AM_DC_DEF_02,			//�Զ���;
	AM_DC_DEF_03,			//�Զ���;
	AM_DC_DEF_04,			//�Զ���;

	AM_HARM_01 = 401,       //(XHB-modbus.Va-3��-1·-г��-���);
	AM_HARM_DEF_01 = 411,			//�Զ���;
	AM_HARM_DEF_02,			//�Զ���;
	AM_HARM_DEF_03,			//�Զ���;
	AM_HARM_DEF_04,			//�Զ���;

	E_OP_01 = 501,           //(XHB-1·-���ܵ��)  

	WM_01 = 601,			//(CJT188-2004-��ˮˮ��);
	WM_02,					//(CJT188-2004-������ˮˮ��) 
	WM_03,					//(CJT188-2004-ֱ��ˮˮ��) 
	WM_04,					//(CJT188-2004-��ˮˮ��) 

	HM_01 = 701,				//(CJT188-2004-������-������);  
	HM_02 = 702,				//(CJT188-2004-������-������)

	GM_01 = 801,				//CJT188-2004-ȼ����
}e_dev_sub_type;
#define SUB_TYPE_NUM (21 + 4*4)

typedef enum 
{
	DEFAUL_COMPANY	= 0,	//Ĭ�Ϲ�˾
	SHANG_HAI_YI_DONG, //�Ϻ��ƶ�
	SHAN_DONG_RU_YE,	//ɽ����Һ
	ZI_HENG_COMPANY,	//�Ϻ�
}e_company_type;

typedef enum 
{
	SQLITE_REPORT = 1,		//
	CSV_REPORT,			// 
}e_report_type;
#define REPORT_TYPE_NUM 2

typedef struct
{
	int province;//ʡ,����������
	int city;//��
	int county;//��(����id)  
	char pos_name[MAX_NAME_LEN];// ��������
}st_position;

typedef struct
{
	bool is_modify;//�Ƿ��޸�
	int  build_code;//�������
	int build_type;                         // ��������          
	char build_name[BUILD_NAME_MAX_LEN];	//��������
	st_position build_pos;//����������Ϣ
} st_build_conf;//������Ϣ

typedef struct
{
	bool is_modify;//�Ƿ��޸�
	char name[MAX_NAME_LEN];//�ɼ�������
	int id;// �ɼ������
	int type;
	int sample_m;//��������
	int heart_s;//��������
	int report_m;//�ϱ�����
	char pos_name[MAX_NAME_LEN];// ��������
	//�����ڽ�����ʾ,����Ҫ�ж�ֵ�Ƿ��޸�
	int age;//����
	char sw_ver[64];//�汾
	char hw_ver[64];
}st_col_conf;//�ɼ�����Ϣ

typedef struct
{
	bool is_modify;//�Ƿ��޸�ʱ��
	st_time dev_time;//���յ����豸ʱ��
	DWORD local_time; //����ʱ�ı���ʱ�� 
}st_dev_time;

typedef struct
{
	e_dev_type dev_type; 
	int param_num;
	int report_param_id[ALL_DEVTYPE_REPORT_PARAM_NUM];//�ϱ�������
} st_dev_type_report_format;//ĳ�������豸���ϱ�������ʽ

typedef struct
{
	bool is_modify;//�Ƿ��޸�
	int dev_type_num;
	st_dev_type_report_format dev_type_report[DEV_TYPE_NUM];
} st_report_format;//�ϱ���Ϣ

typedef struct
{
	bool is_modify;//�Ƿ��޸�
	bool show_used;
	bool report_used;
	e_report_type report_type;
} st_store_inf;//�洢��ʽ��Ϣ

typedef struct
{
	st_report_format report_format;
	st_store_inf store_inf;
} st_report_store_inf;//�ϱ��洢��ʽ��Ϣ

typedef struct
{
	st_col_conf col_conf;
	st_dev_time dev_time;//�豸ʱ��
	st_build_conf build_conf;//������Ϣ
}st_coltor_inf;//�ɼ�����Ϣ

typedef struct
{
	bool valid;
	int port;
	char domain[64];
}st_ip_port_domain;


typedef struct
{
	bool is_modify;//�Ƿ��޸�
	st_ip_port_domain ip_port_domain[MAX_SERVICE_NUM];
	char secret[32];
}st_service_net;

typedef enum 
{
	STATIC = 0,
	DYNAMIC,
}e_net_type;

typedef enum 
{
	RTOS = 1,
	LINUX,
}e_sys_type;

typedef struct
{
	bool is_modify;//�Ƿ��޸�
	e_sys_type sysType;//1:Ŀǰ����free rtos��(���ζ�̬ip); 2:�Ժ����linux��(��Ҫ��̬ip)��
	char ip[32];//�ɼ���ip
	char mask[32];//�ɼ�������
	char gateWay[32];//�ɼ�������
	e_net_type netType;//��̬��̬DHCP
	char mac[64];//mac��ַ
	bool used;
	char dns[64];
}st_eth;

typedef struct
{
	char ssid[64];
	char psk[64];
	char proto[64];
} st_wpa;

typedef struct
{
	bool is_modify;//�Ƿ��޸�
	char ssid[64];
	char passphrase[64];
} st_hotspot;

typedef struct
{
	st_eth eth;
	st_hotspot hotspot;
}st_coltor_net;

typedef struct
{
	st_service_net service_net;//������ip��port
	st_coltor_net coltor_net;//�ɼ���ip
}st_net_conf;//��������Ϣ


//1:2400; 2:4800; 3:9600; 4:19200; 5:38400; 6:115200;
typedef enum 
{
	B1200 = 1200,
	B2400 = 2400,
	B4800 = 4800,
	B9600 = 9600,
	B19200 = 19200,
	B38400 = 38400,
	B115200 = 115200,
}e_baudrate;
#define BAUD_NUM 7

//У�� 0~4=no, odd, even, mark, space
//0:��; 1:��; 2:ż;
typedef enum 
{
	NONE = 0,
	ODD,//��
	EVEN,//ż
}e_parity;
#define PARITY_NUM 3

typedef struct
{
	MU_INT64 id;//�豸��ַ64λ
}st_dev_addr;

typedef enum 
{
	E_WAY_TYPE_UNKNOW          = -1,
	E_WAY_TYPE_TV              = 1
} e_way_type;

typedef struct
{
	char way_name[MAX_WAYNAME_LEN];//��·����
	int way_id;                           // gb_node_id �ڵ���
	int class_id;                     // gb_class_id	������
	int item_id;                      // gb_item_id		������
	char way_addr;       //��·��ַ
	e_way_type way_type;//��·����
	char pos_name[MAX_NAME_LEN];// λ����Ϣ
	char detail[128];//��·����
}st_way_data;

typedef struct
{
	int way_mask;//��·����
	st_way_data way_data[MAX_WAY_NUM];
}st_way_inf;

typedef struct
{
	unsigned char port;
	unsigned char databits;
	e_parity pari;
	unsigned char stop;
	unsigned int baud;
} st_rs485;

typedef struct
{
	st_dev_addr dev_addr;
	char name[MAX_NAME_LEN];//�豸����
	e_dev_sub_type sub_type;//�豸������
	st_way_inf way_inf;//��·��Ϣ
	e_dev_type dev_type;
	char pos_name[MAX_NAME_LEN];// �豸��������
	st_rs485 rs485_conf;
	char hw_ver[64];
	char sw_ver[64];
}st_dev_inf;

typedef struct
{
	bool is_modify;//�Ƿ��޸�
	int dev_num;
	st_dev_inf dev_inf[MAX_DEV_NUM];
}st_port_dev;

typedef struct
{
	int port_num;
	st_port_dev port_dev[MAX_PORT_NUM];
}str_outer_dev;//������Ϣ

typedef enum 
{
	COMMON_TIMEOUT = 0, //ͨ����ʱ
	CLEAN_HISTORY_DATA_TIMEOUT,//�����ʷ���ݳ�ʱ
	REALTIME_DATA_TIMEOUT,//��ʵʱ���ݳ�ʱ
	TEST_TIMEOUT,//���Գ�ʱ
	COPY_HIS_DATA_UDISK_TIMEOUT,//������ʷ���ݵ�u�̳�ʱ
	COPY_LOG_UDISK_TIMEOUT,//������־��u�̳�ʱ
	BACK_FACTORY_TIMEOUT,//�ָ��������ó�ʱ
	ADAPT_DEV_TIMEOUT,//��������Ӧ��ʱ
	RESET_SDCARD_TIMEOUT,//��ʽ��SD����ʱ
}e_read_timeout_type;

typedef struct
{
	/*���ݰ�ͷ (4�ֽ� : 0x68 0x68 0x16 0x16)
	���ݳ��� (4�ֽ� : �����򳤶�)
	������   (4�ֽ� : ָ����� + M�ֽ�:ָ������[����AES���ܺ��XML�ı�])
	CRCУ��  (2�ֽ� : ֻ�Դ�"���ݰ�ͷ"��"������"����CRCУ�飬CRCУ�����CRC-16-CCITT)
	���ݰ�β (4�ֽ� : 0x55 0xAA 0x55 0xAA)*/

	MU_CHAR head[4];//���ݰ�ͷ
	MU_CHAR data_len[4];//���ݳ���
	MU_CHAR sequence[4];//ָ�����
	MS_CHAR xml_data[MAX_XML_LEN];//xml
	MU_INT32 xml_len;
	MU_INT16 parity_crc;//CRCУ��
	MU_CHAR tail[4];//���ݰ�β
} str_communicate_data;

typedef struct  
{
	MU_INT64 id;
	e_dev_sub_type sub_type;
}st_devid_subtype;

typedef enum 
{
	LOAD_COLTOR_THREAD = 1,//���زɼ�����Ϣ�߳�
	SAVE_COLTOR_THREAD,//����ɼ�����Ϣ�߳�
	UPGRAD_THREAD,//���������߳�
	HISTORY_DATA_THREAD,//����ʷ�����߳�
	SAVE_HISTORY_DATA_THREAD,//������ʷ���ݵ������߳�
	REALTIME_DATA_THREAD,//ʵʱ���ݶ�ȡ�߳�
	CLEAN_HISDATA_THREAD,//�����ʷ�����߳�
	COPY_HISDATA_UDISK_THREAD,//������ʷ���ݵ�u���߳�
	COPY_LOG_UDISK_THREAD,//������־��u���߳�
	BACK_FACTORY_THREAD,//�ָ����������߳�
	WAIT_TIME_THREAD,//��ʱ�ȴ�
	RESET_SDCARD_THREAD,//��ʽ��SD��
}e_thread_type;

typedef struct
{
	int	way_ind;
	e_dev_type dev_type;
	u_w_measure measure;
}st_way_data_inf;
typedef vector<st_way_data_inf> VEC_DEV_ALL_WAY_DATA_INF;
typedef struct  
{
	e_thread_type type_thread;//�߳�����

	char* file_data;//����
	int data_len;//���ݳ���

	int pre_result;//�ϴβ���ֵ

	st_time date_tsk;//ʱ��

	st_dev_inf* dev_inf;//�豸��Ϣ
	VEC_DEV_ALL_WAY_DATA_INF* way_check_inf;//��·��Ϣ

	int subType;//�豸������
	MU_INT64 devAddr;//�豸��ַ
	int portInd;//�˿ں�
}st_thread_tsk_param;

typedef enum 
{
	LOCAL_TIME_ID = 1,
	LOAD_STATE_ID,
}e_timer_id;

typedef struct
{
	st_devid_subtype dev_id_subtype;
	st_way_inf way_inf;//�˽ṹ��ֻ�����е�name
}st_way_inf_id_subtype;
typedef vector<st_way_inf_id_subtype> VEC_WAY_NAME_ID_SUBTYPE;

typedef vector<st_devid_subtype> VEC_DEVID_SUBTYPE;

typedef struct
{
	int port_ind;//�˿ں�
	int dev_ind;//�豸��
}st_dev_coordinate;

typedef enum 
{
	READ_TIME_CMD = 0x11,
	SET_TIME_CMD,//дʱ��
	READ_AGE_CMD = 0x21,
	SET_AGE_CMD,//д����
	READ_COLLECTOR_CMD = 0x23,
	SET_COLLECTOR_CMD,//д�ɼ�����Ϣ
	CLEAR_DEV_CMD,//д�������豸
	SET_DEV_CMD,//д�豸��Ϣ
	DELETE_DEV_CMD,//ɾ�豸
	READ_NET_CMD = 0x2a,
	SET_NET_CMD,//д������Ϣ
	READ_SERVICE_CMD = 0x2c,
	SET_SERVICE_CMD,//д��������Ϣ
	SET_REBOOT_CMD = 0x31,//����
	READ_HISTORYDATA_CMD = 0x51,//����ʷ����
	SET_START_SAVE_HISTORYDATA_CMD = 0x52,//��ʼ������ʷ����
	SAVE_HISTORYDATA_CMD = 0x53,
	SET_END_SAVE_HISTORYDATA_CMD = 0x54,
	SET_CLEAN_HISTORYDATA_CMD = 0x55,//�����ʷ����
	READ_REALTIME_DATA_CMD = 0x61,//��ʵʱ����
	READ_SOFTWARE_VERSION_CMD = 0x71,//������汾��
	SET_START_UPGRADE_CMD = 0x72,//��ʼ����
	SET_UPGRADE_DATA_CMD,
	SET_END_UPGRADE_CMD,
	SET_START_REPORT_CMD = 0x81,//��ʼ�ϱ�
	SET_END_REPORT_CMD,
	READ_BUILD_CMD = 0x91,
	SET_BUILD_CMD,//д������Ϣ
	READ_ETH_CMD,
	SET_ETH_CMD,//д��̫����Ϣ
	READ_WLAN_CMD,
	SET_WLAN_CMD,//дwlan��Ϣ
	READ_GNET_CMD,
	SET_GNET_CMD,//дgnet
	READ_HOTSPOT_CMD,
	SET_HOTSPOT_CMD,//д�ȵ�
	READ_PORT_DEV_NUM_CMD,//���˿��豸��
	SET_START_SET_PORT_DEV_CMD,//��ʼд�豸
	SET_END_SET_PORT_DEV_CMD,
	READ_PORT_DEV_CMD,
	SET_PORT_DEV_CMD,
	READ_DEV_FACTORY_CMD,//���豸������Ϣ
	READ_REPORT_FORMAT_CMD,
	SET_REPORT_FORMAT_CMD,//д�ϱ���ʽ
	READ_STORE_INF_CMD,
	SET_STORE_INF_CMD,//д�洢��Ϣ
	SET_EFFECT_NOW_CMD,//������Чĳ��
	READ_DEF_SUBTYPE_NUM_CMD,//���Զ��������͸���
	READ_DEF_SUBTYPE_CONF_CMD,//���Զ�����Ϣ
	READ_DEF_SUBTYPE_WAY_CMD,//���Զ����·��Ϣ
	SET_START_SET_DEF_SUBTYPE_CMD,//��ʼ�Զ���
	SET_DEF_SUBTYPE_CONF_CMD,//д�Զ�������
	SET_DEF_SUBTYPE_WAY_CMD,//д�Զ����·
	SET_END_SET_DEF_SUBTYPE_CMD,//�����Զ���
	GET_MONTH_HISDATA_INF_CMD,//��ȡ����ʷ�����б�
	SET_COPY_HISDATA_UDISK_CMD,//������ʷ����U��
	SET_COPY_LOG_UDISK_CMD,//������־u��
	SET_BACK_FACTORY_CMD,//�ָ�����
	SET_ADAPT_DEV_CMD,//����Ӧ
	SET_HEART_BEAT_CMD,//����
	SET_RESET_SDCARD_CMD,//��ʽ��SD��
}e_require_type;

typedef enum 
{
	ADD_DEV_COMP = 0,
	DELETE_DEV_COMP,
	MODIFY_DEV_COMP,
	CLEAR_DEV_COMP,
}e_dev_Comp_state;

typedef struct
{
	e_dev_Comp_state state;//��ɾ��
	st_dev_inf dev_inf;//�豸��Ϣ
	st_dev_inf old_dev_inf;//��ɾ��֮ǰ���豸��Ϣ,��ʱֻ�õ���ַ��������
	st_dev_coordinate dev_coord;//�豸���ڶ˿ں��豸��
}st_dev_change_inf;

typedef vector<st_dev_change_inf> VEC_DEV_CHANGE;

typedef struct
{
	u_w_measure w_measure;
	e_dev_type dev_type;
	int way_ind;
	int port_ind;
	MU_INT64 dev_ind;
	char data[1024];
}st_history_xml;
typedef vector<st_history_xml> VEC_HISTORY_XML;

typedef struct
{
	bool is_run;
	bool is_kill;
}st_thread_run_state;

typedef enum
{
	// ����                     ��������ID     ��������           
	E_REPORT_ID_COMBINE_DIF  = 1    ,     //��ֵ�ܵ���          
	E_REPORT_ID_T_COMBINE             ,     //����й��ܵ���      
	E_REPORT_ID_T_POSITIVE            ,     //�����й��ܵ���      
	E_REPORT_ID_T_REVERSE             ,     //�����й��ܵ���      
	E_REPORT_ID_VOLT             ,     //��ѹ                
	E_REPORT_ID_CURRENT             ,     //����                
	E_REPORT_ID_VOLT_CHANGE         ,     //��ѹ���            
	E_REPORT_ID_CURR_CHANGE         ,     //�������            
	E_REPORT_ID_FREQUENCY           ,     //Ƶ��                
	E_REPORT_ID_ACTIVE              ,     //���й�����          
	E_REPORT_ID_REACTIVE            ,     //���޹�����          
	E_REPORT_ID_APPARENT            ,     //�����ڹ���          
	E_REPORT_ID_FACTOR              ,     //�ܹ�������          
	E_REPORT_ID_COMBINE1            ,     //����޹�1�ܵ���     
	E_REPORT_ID_COMBINE2            ,     //����޹�2�ܵ���     
	E_REPORT_ID_N_CURRENT           ,     //���ߵ���            
	E_REPORT_ID_A_VOLT              ,     //A���ѹ             
	E_REPORT_ID_B_VOLT              ,     //B���ѹ             
	E_REPORT_ID_C_VOLT              ,     //C���ѹ             
	E_REPORT_ID_AB_VOLT             ,     //AB�ߵ�ѹ            
	E_REPORT_ID_BC_VOLT             ,     //BC�ߵ�ѹ            
	E_REPORT_ID_CA_VOLT             ,     //CA�ߵ�ѹ            
	E_REPORT_ID_A_CURRENT           ,     //A�����             
	E_REPORT_ID_B_CURRENT           ,     //B�����             
	E_REPORT_ID_C_CURRENT           ,     //C�����             
	E_REPORT_ID_ACTIVE_A            ,     //A���й�����         
	E_REPORT_ID_ACTIVE_B            ,     //B���й�����         
	E_REPORT_ID_ACTIVE_C            ,     //C���й�����         
	E_REPORT_ID_REACTIVE_A          ,     //A���޹�����         
	E_REPORT_ID_REACTIVE_B          ,     //B���޹�����         
	E_REPORT_ID_REACTIVE_C          ,     //C���޹�����         
	E_REPORT_ID_APPARENT_A          ,     //A�����ڹ���         
	E_REPORT_ID_APPARENT_B          ,     //B�����ڹ���         
	E_REPORT_ID_APPARENT_C          ,     //C�����ڹ���         
	E_REPORT_ID_FACTOR_A            ,     //A�๦������         
	E_REPORT_ID_FACTOR_B            ,     //B�๦������         
	E_REPORT_ID_FACTOR_C            ,     //C�๦������         
	E_REPORT_ID_A_COMBINE           ,     //A������й��ܵ���   
	E_REPORT_ID_B_COMBINE           ,     //B������й��ܵ���   
	E_REPORT_ID_C_COMBINE           ,     //C������й��ܵ���   
	E_REPORT_ID_METER_STATE         ,     //���״̬            
	E_REPORT_ID_ORIGINAL_VOLTAGE_A  ,     //A�������ѹ         
	E_REPORT_ID_ORIGINAL_VOLTAGE_B  ,     //B�������ѹ         
	E_REPORT_ID_ORIGINAL_VOLTAGE_C  ,     //C�������ѹ         
	E_REPORT_ID_ORIGINAL_CURRENT_A  ,     //A���������         
	E_REPORT_ID_ORIGINAL_CURRENT_B  ,     //B���������         
	E_REPORT_ID_ORIGINAL_CURRENT_C  ,     //C���������         
	E_REPORT_ID_ORIGINAL_ACTIVE_T   ,     //�ܻ����й�����      
	E_REPORT_ID_ORIGINAL_ACTIVE_A   ,     //A������й�����     
	E_REPORT_ID_ORIGINAL_ACTIVE_B   ,     //B������й�����     
	E_REPORT_ID_ORIGINAL_ACTIVE_C   ,     //C������й�����     
	E_REPORT_ID_HARMONIC_VOLTAGE_A  ,     //A��г����ѹ         
	E_REPORT_ID_HARMONIC_VOLTAGE_B  ,     //B��г����ѹ         
	E_REPORT_ID_HARMONIC_VOLTAGE_C  ,     //C��г����ѹ         
	E_REPORT_ID_HARMONIC_CURRENT_A  ,     //A��г������         
	E_REPORT_ID_HARMONIC_CURRENT_B  ,     //B��г������         
	E_REPORT_ID_HARMONIC_CURRENT_C  ,     //C��г������         
	E_REPORT_ID_HARMONIC_ACTIVE_T   ,     //��г���й�����      
	E_REPORT_ID_HARMONIC_ACTIVE_A   ,     //A��г���й�����     
	E_REPORT_ID_HARMONIC_ACTIVE_B   ,     //B��г���й�����     
	E_REPORT_ID_HARMONIC_ACTIVE_C   ,     //C��г���й�����     
	E_REPORT_ID_THD_UA              ,     //Ua�ܻ�����          
	E_REPORT_ID_THD_UB              ,     //Ub�ܻ�����          
	E_REPORT_ID_THD_UC              ,     //Uc�ܻ�����          
	E_REPORT_ID_THD_UT              ,     //�����ѹ�ܻ�����    
	E_REPORT_ID_THD_IA              ,     //Ia�ܻ�����          
	E_REPORT_ID_THD_IB              ,     //Ib�ܻ�����          
	E_REPORT_ID_THD_IC              ,     //Ic�ܻ�����          
	E_REPORT_ID_THD_IT              ,     //��������ܻ�����    
	E_REPORT_ID_HARMONIC_UA_2       ,     //UA 2��г��          
	E_REPORT_ID_HARMONIC_UA_3       ,     //UA 3��г��          
	E_REPORT_ID_HARMONIC_UA_4       ,     //UA 4��г��          
	E_REPORT_ID_HARMONIC_UA_5       ,     //UA 5��г��          
	E_REPORT_ID_HARMONIC_UA_6       ,     //UA 6��г��          
	E_REPORT_ID_HARMONIC_UA_7       ,     //UA 7��г��          
	E_REPORT_ID_HARMONIC_UA_8       ,     //UA 8��г��          
	E_REPORT_ID_HARMONIC_UA_9       ,     //UA 9��г��          
	E_REPORT_ID_HARMONIC_UA_10      ,     //UA 10��г��         
	E_REPORT_ID_HARMONIC_UA_11      ,     //UA 11��г��         
	E_REPORT_ID_HARMONIC_UA_12      ,     //UA 12��г��         
	E_REPORT_ID_HARMONIC_UA_13      ,     //UA 13��г��         
	E_REPORT_ID_HARMONIC_UA_14      ,     //UA 14��г��         
	E_REPORT_ID_HARMONIC_UA_15      ,     //UA 15��г��         
	E_REPORT_ID_HARMONIC_UA_16      ,     //UA 16��г��         
	E_REPORT_ID_HARMONIC_UA_17      ,     //UA 17��г��         
	E_REPORT_ID_HARMONIC_UA_18      ,     //UA 18��г��         
	E_REPORT_ID_HARMONIC_UA_19      ,     //UA 19��г��         
	E_REPORT_ID_HARMONIC_UA_20      ,     //UA 20��г��         
	E_REPORT_ID_HARMONIC_UA_21      ,     //UA 21��г��         
	E_REPORT_ID_HARMONIC_UA_ODD     ,     //UA ���г��������   
	E_REPORT_ID_HARMONIC_UA_EVEN    ,     //UA ż��г��������   
	E_REPORT_ID_HARMONIC_UA_RIDGE   ,     //UA ����ϵ��         
	E_REPORT_ID_HARMONIC_UA_TETL    ,     //UA �绰г���������� 
	E_REPORT_ID_HARMONIC_UB_2       ,     //UB 2��г��          
	E_REPORT_ID_HARMONIC_UB_3       ,     //UB 3��г��          
	E_REPORT_ID_HARMONIC_UB_4       ,     //UB 4��г��          
	E_REPORT_ID_HARMONIC_UB_5       ,     //UB 5��г��          
	E_REPORT_ID_HARMONIC_UB_6       ,     //UB 6��г��          
	E_REPORT_ID_HARMONIC_UB_7       ,     //UB 7��г��          
	E_REPORT_ID_HARMONIC_UB_8       ,     //UB 8��г��          
	E_REPORT_ID_HARMONIC_UB_9       ,     //UB 9��г��          
	E_REPORT_ID_HARMONIC_UB_10      ,     //UB 10��г��         
	E_REPORT_ID_HARMONIC_UB_11      ,     //UB 11��г��         
	E_REPORT_ID_HARMONIC_UB_12      ,     //UB 12��г��         
	E_REPORT_ID_HARMONIC_UB_13      ,     //UB 13��г��        
	E_REPORT_ID_HARMONIC_UB_14      ,     //UB 14��г��        
	E_REPORT_ID_HARMONIC_UB_15      ,     //UB 15��г��        
	E_REPORT_ID_HARMONIC_UB_16      ,     //UB 16��г��        
	E_REPORT_ID_HARMONIC_UB_17      ,     //UB 17��г��        
	E_REPORT_ID_HARMONIC_UB_18      ,     //UB 18��г��        
	E_REPORT_ID_HARMONIC_UB_19      ,     //UB 19��г��        
	E_REPORT_ID_HARMONIC_UB_20      ,     //UB 20��г��        
	E_REPORT_ID_HARMONIC_UB_21      ,     //UB 21��г��        
	E_REPORT_ID_HARMONIC_UB_ODD     ,     //UB ���г��������  
	E_REPORT_ID_HARMONIC_UB_EVEN    ,     //UB ż��г��������  
	E_REPORT_ID_HARMONIC_UB_RIDGE   ,     //UB ����ϵ��        
	E_REPORT_ID_HARMONIC_UB_TETL    ,     //UB �绰г����������
	E_REPORT_ID_HARMONIC_UC_2       ,     //UC 2��г��         
	E_REPORT_ID_HARMONIC_UC_3       ,     //UC 3��г��         
	E_REPORT_ID_HARMONIC_UC_4       ,     //UC 4��г��         
	E_REPORT_ID_HARMONIC_UC_5       ,     //UC 5��г��         
	E_REPORT_ID_HARMONIC_UC_6       ,     //UC 6��г��         
	E_REPORT_ID_HARMONIC_UC_7       ,     //UC 7��г��         
	E_REPORT_ID_HARMONIC_UC_8       ,     //UC 8��г��         
	E_REPORT_ID_HARMONIC_UC_9       ,     //UC 9��г��         
	E_REPORT_ID_HARMONIC_UC_10      ,     //UC 10��г��        
	E_REPORT_ID_HARMONIC_UC_11      ,     //UC 11��г��        
	E_REPORT_ID_HARMONIC_UC_12      ,     //UC 12��г��        
	E_REPORT_ID_HARMONIC_UC_13      ,     //UC 13��г��        
	E_REPORT_ID_HARMONIC_UC_14      ,     //UC 14��г��        
	E_REPORT_ID_HARMONIC_UC_15      ,     //UC 15��г��        
	E_REPORT_ID_HARMONIC_UC_16      ,     //UC 16��г��        
	E_REPORT_ID_HARMONIC_UC_17      ,     //UC 17��г��        
	E_REPORT_ID_HARMONIC_UC_18      ,     //UC 18��г��        
	E_REPORT_ID_HARMONIC_UC_19      ,     //UC 19��г��        
	E_REPORT_ID_HARMONIC_UC_20      ,     //UC 20��г��        
	E_REPORT_ID_HARMONIC_UC_21      ,     //UC21��г��         
	E_REPORT_ID_HARMONIC_UC_ODD     ,     //UC ���г��������  
	E_REPORT_ID_HARMONIC_UC_EVEN    ,     //UC ż��г��������  
	E_REPORT_ID_HARMONIC_UC_RIDGE   ,     //UC ����ϵ��        
	E_REPORT_ID_HARMONIC_UC_TETL    ,     //UC �绰г����������
	E_REPORT_ID_HARMONIC_IA_2       ,     //IA 2��г��         
	E_REPORT_ID_HARMONIC_IA_3       ,     //IA 3��г��         
	E_REPORT_ID_HARMONIC_IA_4       ,     //IA 4��г��         
	E_REPORT_ID_HARMONIC_IA_5       ,     //IA 5��г��         
	E_REPORT_ID_HARMONIC_IA_6       ,     //IA 6��г��         
	E_REPORT_ID_HARMONIC_IA_7       ,     //IA 7��г��         
	E_REPORT_ID_HARMONIC_IA_8       ,     //IA 8��г��         
	E_REPORT_ID_HARMONIC_IA_9       ,     //IA 9��г��         
	E_REPORT_ID_HARMONIC_IA_10      ,     //IA 10��г��        
	E_REPORT_ID_HARMONIC_IA_11      ,     //IA 11��г��        
	E_REPORT_ID_HARMONIC_IA_12      ,     //IA 12��г��        
	E_REPORT_ID_HARMONIC_IA_13      ,     //IA 13��г��        
	E_REPORT_ID_HARMONIC_IA_14      ,     //IA 14��г��        
	E_REPORT_ID_HARMONIC_IA_15      ,     //IA 15��г��        
	E_REPORT_ID_HARMONIC_IA_16      ,     //IA 16��г��        
	E_REPORT_ID_HARMONIC_IA_17      ,     //IA 17��г��        
	E_REPORT_ID_HARMONIC_IA_18      ,     //IA 18��г��        
	E_REPORT_ID_HARMONIC_IA_19      ,     //IA 19��г��        
	E_REPORT_ID_HARMONIC_IA_20      ,     //IA 20��г��        
	E_REPORT_ID_HARMONIC_IA_21      ,     //IA21��г��         
	E_REPORT_ID_HARMONIC_IA_ODD     ,     //IA ���г��������  
	E_REPORT_ID_HARMONIC_IA_EVEN    ,     //IA ż��г��������  
	E_REPORT_ID_HARMONIC_IA_K       ,     //IA  Kϵ��          
	E_REPORT_ID_HARMONIC_IB_2       ,     //IB 2��г��         
	E_REPORT_ID_HARMONIC_IB_3       ,     //IB 3��г��         
	E_REPORT_ID_HARMONIC_IB_4       ,     //IB 4��г��         
	E_REPORT_ID_HARMONIC_IB_5       ,     //IB 5��г��         
	E_REPORT_ID_HARMONIC_IB_6       ,     //IB 6��г��         
	E_REPORT_ID_HARMONIC_IB_7       ,     //IB 7��г��         
	E_REPORT_ID_HARMONIC_IB_8       ,     //IB 8��г��         
	E_REPORT_ID_HARMONIC_IB_9       ,     //IB 9��г��         
	E_REPORT_ID_HARMONIC_IB_10      ,     //IB 10��г��        
	E_REPORT_ID_HARMONIC_IB_11      ,     //IB 11��г��        
	E_REPORT_ID_HARMONIC_IB_12      ,     //IB 12��г��        
	E_REPORT_ID_HARMONIC_IB_13      ,     //IB 13��г��        
	E_REPORT_ID_HARMONIC_IB_14      ,     //IB 14��г��        
	E_REPORT_ID_HARMONIC_IB_15      ,     //IB 15��г��        
	E_REPORT_ID_HARMONIC_IB_16      ,     //IB 16��г��        
	E_REPORT_ID_HARMONIC_IB_17      ,     //IB 17��г��        
	E_REPORT_ID_HARMONIC_IB_18      ,     //IB 18��г��        
	E_REPORT_ID_HARMONIC_IB_19      ,     //IB 19��г��        
	E_REPORT_ID_HARMONIC_IB_20      ,     //IB 20��г��        
	E_REPORT_ID_HARMONIC_IB_21      ,     //IB 21��г��        
	E_REPORT_ID_HARMONIC_IB_ODD     ,     //IB ���г��������  
	E_REPORT_ID_HARMONIC_IB_EVEN    ,     //IB ż��г��������  
	E_REPORT_ID_HARMONIC_IB_K       ,     //IB  Kϵ��          
	E_REPORT_ID_HARMONIC_IC_2       ,     //IC 2��г��         
	E_REPORT_ID_HARMONIC_IC_3       ,     //IC 3��г��         
	E_REPORT_ID_HARMONIC_IC_4       ,     //IC 4��г��         
	E_REPORT_ID_HARMONIC_IC_5       ,     //IC 5��г��         
	E_REPORT_ID_HARMONIC_IC_6       ,     //IC 6��г��         
	E_REPORT_ID_HARMONIC_IC_7       ,     //IC 7��г��         
	E_REPORT_ID_HARMONIC_IC_8       ,     //IC 8��г��         
	E_REPORT_ID_HARMONIC_IC_9       ,     //IC 9��г��         
	E_REPORT_ID_HARMONIC_IC_10      ,     //IC 10��г��        
	E_REPORT_ID_HARMONIC_IC_11      ,     //IC 11��г��        
	E_REPORT_ID_HARMONIC_IC_12      ,     //IC 12��г��        
	E_REPORT_ID_HARMONIC_IC_13      ,     //IC 13��г��        
	E_REPORT_ID_HARMONIC_IC_14      ,     //IC 14��г��        
	E_REPORT_ID_HARMONIC_IC_15      ,     //IC 15��г��        
	E_REPORT_ID_HARMONIC_IC_16      ,     //IC 16��г��        
	E_REPORT_ID_HARMONIC_IC_17      ,     //IC 17��г��        
	E_REPORT_ID_HARMONIC_IC_18      ,     //IC 18��г��        
	E_REPORT_ID_HARMONIC_IC_19      ,     //IC 19��г��        
	E_REPORT_ID_HARMONIC_IC_20      ,     //IC 20��г��        
	E_REPORT_ID_HARMONIC_IC_21      ,     //IC 21��г��        
	E_REPORT_ID_HARMONIC_IC_ODD     ,     //IC ���г��������  
	E_REPORT_ID_HARMONIC_IC_EVEN    ,     //IC ż��г��������  
	E_REPORT_ID_HARMONIC_IC_K       ,//IC  Kϵ��  
	E_REPORT_ID_HEAT_SETTLE_DATA,//����������
	E_REPORT_ID_CUR_HEAT,//��ǰ����
	E_REPORT_ID_THERMAL_POWER,//�ȹ���
	E_REPORT_ID_FLOW,//����
	E_REPORT_ID_CUMULA_FLOW,//�ۼ�����
	E_REPORT_ID_SUPPLYWATER_TEMP,//��ˮ�¶�
	E_REPORT_ID_BACKWATER_TEMP,//��ˮ�¶�
	E_REPORT_ID_CUMULA_TIME,//�ۼƹ���ʱ��
	E_REPORT_ID_CUR_CUMULA_FLOW = 231,//��ǰ�ۼ�����
	E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA,//�������ۼ�����
	E_REPORT_ID_METER_TIME = 245,//ʱ��
	E_REPORT_ID_THRESHOLD_STATE,//��ֵ�澯״̬
	E_REPORT_ID_T_COMBINE_RATE1,//����й�����1����
	E_REPORT_ID_T_COMBINE_RATE2,//����й�����2����
	E_REPORT_ID_T_COMBINE_RATE3,//����й�����3����
	E_REPORT_ID_T_COMBINE_RATE4,//����й�����4����
	E_REPORT_ID_T_POSITIVE_RATE1,//�����й�����1����
	E_REPORT_ID_T_POSITIVE_RATE2,//�����й�����2����
	E_REPORT_ID_T_POSITIVE_RATE3,//�����й�����3����
	E_REPORT_ID_T_POSITIVE_RATE4,//�����й�����4����
	E_REPORT_ID_T_REVERSE_RATE1,//�����й�����1����
	E_REPORT_ID_T_REVERSE_RATE2,//�����й�����2����
	E_REPORT_ID_T_REVERSE_RATE3,//�����й�����3����
	E_REPORT_ID_T_REVERSE_RATE4,//�����й�����4����
	E_REPORT_ID_T_COMBINE1_RATE1,//����޹�1����1����
	E_REPORT_ID_T_COMBINE1_RATE2,//����޹�1����2����
	E_REPORT_ID_T_COMBINE1_RATE3,//����޹�1����3����
	E_REPORT_ID_T_COMBINE1_RATE4,//����޹�1����4����
	E_REPORT_ID_T_COMBINE2_RATE1,//����޹�2����1����
	E_REPORT_ID_T_COMBINE2_RATE2,//����޹�2����2����
	E_REPORT_ID_T_COMBINE2_RATE3,//����޹�2����3����
	E_REPORT_ID_T_COMBINE2_RATE4,//����޹�2����4����
	E_REPORT_ID_E_OP_FAULT,//��ٹ���
	E_REPORT_ID_ON_OFF_STATE,//�Ϸ�״̬
	E_REPORT_ID_ON_COUNT,//�غϴ���
	E_REPORT_ID_ON_TIMER,//�غ�ʱ����
	E_REPORT_ID_ON_THINK_TIME,//�ɹ���բ�ж�ʱ��
} e_report_param_id;

typedef struct
{
	int param_id;//�����id
	int param_unit;//��λ,���׼������λ�ı�ֵ��
	//unit��ӳ���ϵ {0->0.001; 1->0.01; 2->0.1; 3->1; 4->10; 5->100; 6->1000}
	int param_register_addr;//�����Ĵ�����ַ
	int param_register_len;//�Ĵ�������
}st_param_info;//������Ϣ

typedef struct
{
	int dev_type;//�豸������
	int sub_type;//������

	//����������,���������޸ģ������Զ����޸ģ�ӳ�䵽ȫ������gSubTypeValue
	//ע�⣺��ֵΪ�գ�ʹ��ȫ������gSubTypeName��Ӧֵ
	char sub_type_name[128];
	
	//�����ͻ�·����.��ʼֵΪ0.���������޸ģ������Զ����޸ģ�ӳ�䵽ȫ������gWayCount
	int way_num;
	bool is_reg_same;//����·�Ĵ�����Ϣ�Ƿ�һ��
}st_def_sub_type_info;//�Զ���modbusЭ����Ϣ

typedef enum 
{
	UART_TRANSF = 0,			//���ڴ���
	NET_TRANSF,					//���紫��
}e_transfer_type;

typedef enum 
{
	SEND_FAIL = 1111,		//����ʧ��  
	RECEIVE_FAIL,//����ʧ��
}e_communicate_err;