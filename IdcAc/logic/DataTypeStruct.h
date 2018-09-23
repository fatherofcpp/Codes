#pragma once
/*
全局结构体
*/
#include "include/mty.h"
#include "RealData.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include <map>
#include <vector>
#include <deque>
#include <list>
using namespace std;

#define MAX_DEV_NUM (4)//某端口最大外设个数
#define MAX_PORT_NUM (1)//端口最大个数
#define MAX_SERVICE_NUM (4)//上报服务器最大个数
#define MAX_NETLINK_NUM (1)//数据链路最大个数
#define MESSAGE_BODY_IND (12)//消息体起始位置
#define MAX_NAME_LEN (50)//名字长度20utf8
#define MAX_WAYNAME_LEN (48)//名字长度48utf8
#define MAX_JZCODE_NUM (5)//最大基站编号个数
#define MAX_XML_LEN (1024 * 32)//最大xml长度
#define BUILD_NAME_MAX_LEN (50)
#define BUILD_POS_MAX_LEN (50)
#define SUB_WAY_PARAM_NUM (30)
#define MAX_WAY_NUM (2*(SUB_WAY_PARAM_NUM+1))//最大回路个数
#define ELECTRIC_TOTAL_WAY_NUM (84)
#define MAIN_B_OFF_ELECTRIC (ELECTRIC_TOTAL_WAY_NUM/2)
#define MAX_DEFINE_MODBUS_PROTOCOL (4 * 4)//最大自定义modbus协议数
#define ALL_DEVTYPE_REPORT_PARAM_NUM (247)
#define DEV_ID_ERR (0xFFFFFFFFFFFFFFF)
#define MAIN_A_CODE (10)
#define MAIN_B_CODE (110)
#define RS_DTU_TIMEOUT (6)
#define FRAME_DTU_DELAY_M (200)
#define DEVID_ELECTRIC_M (1)
#define DEVID_SWITCH_M_A (2)
#define DEVID_SWITCH_M_B (3)
#define REPORT_SUB_NUM (30)

#define HEAD_TAIL_LEN (4 + 4 + 4 + 2 + 4)

typedef map<int, int> MAP_INT_INT;
typedef map<int, string> MAP_INT_STRING;
typedef vector<string> VEC_STRING;
//typedef vector<CString> VEC_CSTRING;
typedef vector<int> VEC_INT;
typedef vector<MU_INT16> VEC_INT16;
typedef vector<MU_INT8> VEC_INT8;

typedef struct
{
    int YY;//设备时间
    int MM;
    int DD;
    int HH;
    int MI;
    int SS;
}st_time;
typedef vector<st_time> VEC_TIME;

typedef enum
{
    //0:电表设备; 1:绝缘设备; 2:开关设备
    E_DEV_ELECTRIC = 0,
    E_DEV_INSULATION,
    E_DEV_SWITCH,
    E_DEV_TYPE_NUM
}e_dev_type;

typedef enum
{
    AM_3PHASE_01 = 101,      //(DLT645-2007-3相-1路-交流-电表);
    AM_3PHASE_02 = 102,     //(XHB-modbus.Vc-3相-4路-交流-电表);
    AM_3PHASE_03 = 103,     //(XHB-modbus.Vb-3相-1路-交流-电表);
    AM_3PHASE_04 = 104,     //(XHB-modbus.Va-3相-1路-交流-电表-电力分析仪);
    AM_3PHASE_05 = 105,     //(DLT645-1997-3相-1路-交流-电表);
    AM_3PHASE_DEF_01 = 111,     //自定义;
    AM_3PHASE_DEF_02,     //自定义;
    AM_3PHASE_DEF_03,     //自定义;
    AM_3PHASE_DEF_04,     //自定义;

    AM_1PHASE_01 = 201,     //(XHB-07-1相-1路-交流-电表);
    AM_1PHASE_02 = 202,     //(XHB-modbus.Vc-1相-12路-交流-电表);
    AM_1PHASE_03 = 203,     //(XHB-modbus.Vb-1相-3路-交流-电表);
    AM_1PHASE_04 = 204,     //(XHB-modbus.Va-1相-3路-交流-电表);
    AM_1PHASE_DEF_01 = 211,		//自定义;
    AM_1PHASE_DEF_02,		//自定义;
    AM_1PHASE_DEF_03,		//自定义;
    AM_1PHASE_DEF_04,		//自定义;

    AM_DC_01 = 301,         //(XHB-07-12路-直流-电表);
    AM_DC_02 = 302,          //(XHB-modbus.vb-1路-直流-电表）
    AM_DC_03 = 303,			//(XHB-modbus.vb-2路-直流-电表);
    AM_DC_DEF_01 = 311,			//自定义;
    AM_DC_DEF_02,			//自定义;
    AM_DC_DEF_03,			//自定义;
    AM_DC_DEF_04,			//自定义;

    AM_HARM_01 = 401,       //(XHB-modbus.Va-3相-1路-谐波-电表);
    AM_HARM_DEF_01 = 411,			//自定义;
    AM_HARM_DEF_02,			//自定义;
    AM_HARM_DEF_03,			//自定义;
    AM_HARM_DEF_04,			//自定义;

    E_OP_01 = 501,           //(XHB-1路-智能电操)

    WM_01 = 601,			//(CJT188-2004-冷水水表);
    WM_02,					//(CJT188-2004-生活热水水表)
    WM_03,					//(CJT188-2004-直饮水水表)
    WM_04,					//(CJT188-2004-中水水表)

    HM_01 = 701,				//(CJT188-2004-热气表-计热量);
    HM_02 = 702,				//(CJT188-2004-热气表-计冷量)

    GM_01 = 801				//CJT188-2004-燃气表
}e_dev_sub_type;
#define SUB_TYPE_NUM (21 + 4*4)

typedef enum
{
    DEFAUL_COMPANY	= 0,	//默认公司
    SHANG_HAI_YI_DONG, //上海移动
    SHAN_DONG_RU_YE,	//山东乳液
    ZI_HENG_COMPANY	//紫衡
}e_company_type;

typedef enum
{
    SQLITE_REPORT = 1,		//
    CSV_REPORT		//
}e_report_type;
#define REPORT_TYPE_NUM 2

typedef struct
{
    int province;//省,建筑区域编号
    int city;//市
    int county;//县(区域id)
    char pos_name[MAX_NAME_LEN];// 区域名称
}st_position;

typedef struct
{
    bool is_modify;//是否修改
    int  build_code;//建筑编号
    int build_type;                         // 建筑类型
    char build_name[BUILD_NAME_MAX_LEN];	//建筑名称
    st_position build_pos;//建筑区域信息
} st_build_conf;//建筑信息

typedef struct
{
    bool is_modify;//是否修改
    char name[MAX_NAME_LEN];//采集器名称
    int id;// 采集器编号
    int type;
    int sample_m;//采样周期
    int heart_s;//心跳周期
    int report_m;//上报周期
    char pos_name[MAX_NAME_LEN];// 区域名称
    //不用于界面显示,不需要判断值是否修改
    int age;//年龄
    char sw_ver[64];//版本
    char hw_ver[64];
}st_col_conf;//采集器信息

typedef struct
{
    bool is_modify;//是否修改时间
    st_time dev_time;//接收到的设备时间
}st_dev_time;

typedef struct
{
    e_dev_type dev_type;
    int param_num;
    int report_param_id[ALL_DEVTYPE_REPORT_PARAM_NUM];//上报最大参量
} st_dev_type_report_format;//某种类型设备的上报参数格式

typedef struct
{
    bool is_modify;//是否修改
    int dev_type_num;
    st_dev_type_report_format dev_type_report[E_DEV_TYPE_NUM];
} st_report_format;//上报信息

typedef struct
{
    bool is_modify;//是否修改
    bool show_used;
    bool report_used;
    e_report_type report_type;
} st_store_inf;//存储格式信息

typedef struct
{
    st_report_format report_format;
    st_store_inf store_inf;
} st_report_store_inf;//上报存储格式信息

typedef struct
{
    st_col_conf col_conf;
    st_dev_time dev_time;//设备时间
    st_build_conf build_conf;//建筑信息
}st_coltor_inf;//采集器信息

typedef struct
{
    bool valid;
    int port;
    char domain[64];
}st_ip_port_domain;


typedef struct
{
    bool is_modify;//是否修改
    st_ip_port_domain ip_port_domain[MAX_SERVICE_NUM];
    char secret[32];
}st_service_net;

typedef enum
{
    STATIC = 0,
    DYNAMIC
}e_net_type;

typedef enum
{
    RTOS = 1,
    LINUX
}e_sys_type;

typedef struct
{
    bool is_modify;//是否修改
    e_sys_type sysType;//1:目前基于free rtos的(屏蔽动态ip); 2:以后基于linux的(需要动态ip)。
    char ip[32];//采集器ip
    char mask[32];//采集器掩码
    char gateWay[32];//采集器网关
    e_net_type netType;//静态或动态DHCP
    char mac[64];//mac地址
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
    bool is_modify;//是否修改
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
    st_service_net service_net;//服务器ip，port
    st_coltor_net coltor_net;//采集器ip
}st_net_conf;//服务器信息


//1:2400; 2:4800; 3:9600; 4:19200; 5:38400; 6:115200;
typedef enum
{
    BAUD1200 = 1200,
    BAUD2400 = 2400,
    BAUD4800 = 4800,
    BAUD9600 = 9600,
    BAUD19200 = 19200,
    BAUD38400 = 38400,
    BAUD115200 = 115200
}e_baudrate;
#define BAUD_NUM 7

//校验 0~4=no, odd, even, mark, space
//0:无; 1:奇; 2:偶;
typedef enum
{
    NONE = 0,
    ODD,//奇
    EVEN//偶
}e_parity;
#define PARITY_NUM 3

typedef struct
{
    MU_INT8 id;//设备地址64位
}st_dev_addr;

typedef enum
{
    E_WAY_TYPE_UNKNOW          = -1,
    E_WAY_TYPE_TV              = 1
} e_way_type;

typedef struct
{
    //0表示阀值，1表示是否使能
    MU_INT32 u_down_limit[2];//电压下限阀值      (主路);
    MU_INT32 u_up_limit[2];//电压上限阀值      (主路);
    MU_INT32 u_harm_percent_up_limit[2];//电压谐波百分比上限(主路);
    MU_INT32 i_zero_up_limit[2];//零线电流上限阀值(主路);
    MU_INT32 i_unbalance_up_limit[2];//电流不平衡上限阀值(主路);
    MU_INT32 i_harm_percent_limit[2];//电流谐波百分比(主路);
    MU_INT32 f_up_limit[2];//频率上限(主路);
    MU_INT32 f_down_limit[2];//频率下限(主路);
    MU_INT32 lose_phase_limit[2];//缺相下限          (主路);
    MU_INT32 i_down_limit[2];//电流下限阀值      (主路、支路);
    MU_INT32 i_up_limit[2];//电流上限阀值      (主路、支路);
    MU_INT32 i_down2_limit[2];//电流下下限阀值    (主路、支路);
    MU_INT32 i_up2_limit[2];//电流上上限阀值    (主路、支路);
    MU_INT32 fuse_limit[2];//熔断器开关

    MU_INT32 resistance_threshold[2];//电阻阀值
}st_threshold;

typedef enum
{
    E_U_DOWN_LIMIT = 0,//电压下限阀值      (主路);
    E_U_UP_LIMIT,//电压上限阀值      (主路);
    E_U_HARM_PERCENT_UP_LIMIT,//电压谐波百分比上限(主路);
    E_I_ZERO_UP_LIMIT,//零线电流上限阀值(主路);
    E_I_UNBALANCE_UP_LIMIT,//电流不平衡上限阀值(主路);
    E_I_HARM_PERCENT_LIMIT,//电流谐波百分比(主路);
    E_F_UP_LIMIT,//频率上限(主路);
    E_F_DOWN_LIMIT,//频率下限(主路);
    E_LOSE_PHASE_LIMIT,//缺相下限          (主路);
    E_I_DOWN_LIMIT,//电流下限阀值      (主路、支路);
    E_I_UP_LIMIT,//电流上限阀值      (主路、支路);
    E_I_DOWN2_LIMIT,//电流下下限阀值    (主路、支路);
    E_I_UP2_LIMIT,//电流上上限阀值    (主路、支路);
    E_FUSE_LIMIT,//熔断器开关
    E_WARN_EVENT_NUM//总数
}e_warn_type;//读阀值时，对边界值做限制

typedef struct
{
    char way_name[MAX_WAYNAME_LEN];//回路名称
    int way_id;                           // gb_node_id 节点编号
    int class_id;                     // gb_class_id	分类编号
    int item_id;                      // gb_item_id		分项编号
    char way_addr;       //回路地址
    MU_INT32 way_type;//回路类型
    char pos_name[MAX_NAME_LEN];// 位置信息
    char detail[128];//回路详情

    //回路阀值
    st_threshold way_threshold;
}st_way_data;

typedef struct
{
    MU_INT64 way_mask;//回路掩码
    st_way_data way_data[MAX_WAY_NUM];
}st_way_inf;

typedef struct
{
    MU_INT8 port;
    MU_INT8 databits;
    MU_INT32 pari;
    MU_INT8 stop;
    MU_INT32 baud;
} st_rs485;

typedef struct
{
    st_dev_addr dev_addr;
    char name[MAX_NAME_LEN];//设备名称
    MU_INT32 sub_type;//设备子类型
    st_way_inf way_inf;//回路信息
    int dev_type;
    char pos_name[MAX_NAME_LEN];// 设备区域名称
    st_rs485 rs485_conf;
    char hw_ver[64];
    char sw_ver[64];

    //设备实时数据
    bool comm_state;
    u_dev_measure dev_measure;
}st_dev_inf;

typedef struct
{
    bool is_modify;//是否修改
    int dev_num;
    st_dev_inf dev_inf[MAX_DEV_NUM];
}st_port_dev;

typedef struct
{
    int port_num;
    st_port_dev port_dev[MAX_PORT_NUM];
}str_outer_dev;//外设信息

typedef enum
{
    COMMON_TIMEOUT = 0, //通常超时
    CLEAN_HISTORY_DATA_TIMEOUT,//清除历史数据超时
    REALTIME_DATA_TIMEOUT,//读实时数据超时
    TEST_TIMEOUT,//测试超时
    COPY_HIS_DATA_UDISK_TIMEOUT,//拷贝历史数据到u盘超时
    COPY_LOG_UDISK_TIMEOUT,//拷贝日志到u盘超时
    BACK_FACTORY_TIMEOUT,//恢复出厂设置超时
    ADAPT_DEV_TIMEOUT,//下行自适应超时
    RESET_SDCARD_TIMEOUT//格式化SD卡超时
}e_read_timeout_type;

typedef struct
{
    /*
    数据包头 (4字节 : 0x68 0x68 0x16 0x16)
    数据长度 (4字节 : 数据域长度)
    数据域   (4字节 : 指令序号 + M字节:指令内容[经过AES加密后的XML文本])
    CRC校验  (2字节 : 只对从"数据包头"到"数据域"进行CRC校验，CRC校验采用CRC-16-CCITT)
    数据包尾 (4字节 : 0x55 0xAA 0x55 0xAA)
    */

    MU_CHAR head[4];//数据包头
    MU_CHAR data_len[4];//数据长度
    MU_CHAR sequence[4];//指令序号
    MS_CHAR xml_data[MAX_XML_LEN];//xml
    MU_INT32 xml_len;
    MU_INT16 parity_crc;//CRC校验
    MU_CHAR tail[4];//数据包尾
} str_communicate_data;

typedef struct
{
    MU_INT64 id;
    e_dev_sub_type sub_type;
}st_devid_subtype;

typedef enum
{
    LOAD_COLTOR_THREAD = 1,//加载采集器信息线程
    SAVE_COLTOR_THREAD,//保存采集器信息线程
    UPGRAD_THREAD,//程序升级线程
    HISTORY_DATA_THREAD,//读历史数据线程
    SAVE_HISTORY_DATA_THREAD,//保存历史数据到本地线程
    CLEAN_HISDATA_THREAD,//清除历史数据线程
    COPY_HISDATA_UDISK_THREAD,//拷贝历史数据到u盘线程
    COPY_LOG_UDISK_THREAD,//拷贝日志到u盘线程
    BACK_FACTORY_THREAD,//恢复出厂设置线程
    WAIT_TIME_THREAD,//定时等待
    RESET_SDCARD_THREAD//格式化SD卡
}e_thread_type;

typedef struct
{
    int port_ind;
    MU_INT64 dev_id;
    e_dev_sub_type dev_sub_type;
    int way_id;
    int	way_ind;
    e_dev_type dev_type;
    u_w_measure measure;
}st_way_data_inf;
typedef vector<st_way_data_inf> VEC_DEV_ALL_WAY_DATA_INF;

typedef struct
{
    MU_INT16 reg_val;
    MU_INT16 reg_addr;
    MU_INT8 dev_addr;
}st_thread_param_i_range_one;

typedef struct
{
    VEC_INT16 vec_val;
    MU_INT16 reg_addr;
    MU_INT8 dev_addr;
}st_thread_param_i_range_all;

typedef struct
{
    int dev_type;
    int way_code;
    int warn_type;
    bool is_enable;
    int thre_val;
}st_threshold_item;
typedef vector<st_threshold_item> VEC_THRESHOLD_ITEM;

typedef struct
{
    char start_time[32];
    char stop_time[32];
    char obj_name[64];
    char warn_type[64];

    //用于报警判断
    bool is_warn;
    int way_code;
    int dev_type;

    //dtu
    MU_INT8 dev_addr;
}st_warn_history_inf;
typedef deque<st_warn_history_inf*> DEQ_ST_WARN_HISTORY_INF;


typedef struct
{
    int off_set;
    DEQ_ST_WARN_HISTORY_INF deq_st_warn_history;
}st_thread_param_warn_history_get;

typedef struct
{
    VEC_THRESHOLD_ITEM vec_threshold_item;
}st_thread_param_threshold_set;

typedef struct
{
    MU_INT16 reg_val;
    MU_INT8 dev_addr;
}st_thread_param_zero_energy_set;

typedef struct
{
    MU_INT32 type_cmd;//线程类型

    st_thread_param_i_range_one param_i_range_one;//单个电流量程写
    st_thread_param_i_range_all param_i_range_all;//多个电流量程写
    st_thread_param_threshold_set param_threshold_set;//写阀值
    st_thread_param_warn_history_get param_warn_history_get;//读历史数据
    st_thread_param_zero_energy_set param_zero_energy_set;//写清零电能
}st_thread_tsk_param;

typedef enum
{
    LOCAL_TIME_ID = 1,
    LOAD_STATE_ID
}e_timer_id;

typedef struct
{
    st_devid_subtype dev_id_subtype;
    st_way_inf way_inf;//此结构体只用其中的name
}st_way_inf_id_subtype;
typedef vector<st_way_inf_id_subtype> VEC_WAY_NAME_ID_SUBTYPE;

typedef vector<st_devid_subtype> VEC_DEVID_SUBTYPE;

typedef struct
{
    int port_ind;//端口号
    int dev_ind;//设备号
}st_dev_coordinate;

typedef enum
{
    E_DTU_CMD = 0,//dtu
    E_READ_CURRENT_RANGE_CMD,//读电流量程
    E_WRITE_CURRENT_RANGE_ONE_CMD,//写电流量程
    E_WRITE_CURRENT_RANGE_ALL_CMD,//写电流量程
    E_READ_THRESHOLD_CMD,//读报警阀值
    E_WRITE_THRESHOLD_CMD,//
    E_READ_WARN_HISTORY_CMD,//读历史报警
    E_WRITE_WARN_HISTORY_CMD,//写历史报警
    E_WRITE_RELAY_CMD,//写继电器
    E_WRITE_ZERO_ENERGY_CMD,//清空电能
    E_READ_COLTOR_CMD,//读采集器信息
    E_WRITE_COLTOR_CMD,//
    E_WRITE_COLTOR_TIME_CMD,//写采集器时间
    E_WRITE_UPGRADE_CMD,//升级
    E_THREAD_TYPE_NUM
}e_require_type;

typedef enum
{
    ADD_DEV_COMP = 0,
    DELETE_DEV_COMP,
    MODIFY_DEV_COMP,
    CLEAR_DEV_COMP
}e_dev_Comp_state;

typedef struct
{
    e_dev_Comp_state state;//增删改
    st_dev_inf dev_inf;//设备信息
    st_dev_inf old_dev_inf;//增删改之前的设备信息,暂时只用到地址和子类型
    st_dev_coordinate dev_coord;//设备所在端口和设备号
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
    // 库名                     参数类型ID     参数类型
    E_REPORT_ID_COMBINE_DIF  = 1    ,     //差值总电能
    E_REPORT_ID_T_COMBINE             ,     //组合有功总电能
    E_REPORT_ID_T_POSITIVE            ,     //正向有功总电能
    E_REPORT_ID_T_REVERSE             ,     //反向有功总电能
    E_REPORT_ID_VOLT             ,     //电压
    E_REPORT_ID_CURRENT             ,     //电流
    E_REPORT_ID_VOLT_CHANGE         ,     //电压变比
    E_REPORT_ID_CURR_CHANGE         ,     //电流变比
    E_REPORT_ID_FREQUENCY           ,     //频率
    E_REPORT_ID_ACTIVE              ,     //总有功功率
    E_REPORT_ID_REACTIVE            ,     //总无功功率
    E_REPORT_ID_APPARENT            ,     //总视在功率
    E_REPORT_ID_FACTOR              ,     //总功率因数
    E_REPORT_ID_COMBINE1            ,     //组合无功1总电能
    E_REPORT_ID_COMBINE2            ,     //组合无功2总电能
    E_REPORT_ID_N_CURRENT           ,     //零线电流
    E_REPORT_ID_A_VOLT              ,     //A相电压
    E_REPORT_ID_B_VOLT              ,     //B相电压
    E_REPORT_ID_C_VOLT              ,     //C相电压
    E_REPORT_ID_AB_VOLT             ,     //AB线电压
    E_REPORT_ID_BC_VOLT             ,     //BC线电压
    E_REPORT_ID_CA_VOLT             ,     //CA线电压
    E_REPORT_ID_A_CURRENT           ,     //A相电流
    E_REPORT_ID_B_CURRENT           ,     //B相电流
    E_REPORT_ID_C_CURRENT           ,     //C相电流
    E_REPORT_ID_ACTIVE_A            ,     //A相有功功率
    E_REPORT_ID_ACTIVE_B            ,     //B相有功功率
    E_REPORT_ID_ACTIVE_C            ,     //C相有功功率
    E_REPORT_ID_REACTIVE_A          ,     //A相无功功率
    E_REPORT_ID_REACTIVE_B          ,     //B相无功功率
    E_REPORT_ID_REACTIVE_C          ,     //C相无功功率
    E_REPORT_ID_APPARENT_A          ,     //A相视在功率
    E_REPORT_ID_APPARENT_B          ,     //B相视在功率
    E_REPORT_ID_APPARENT_C          ,     //C相视在功率
    E_REPORT_ID_FACTOR_A            ,     //A相功率因数
    E_REPORT_ID_FACTOR_B            ,     //B相功率因数
    E_REPORT_ID_FACTOR_C            ,     //C相功率因数
    E_REPORT_ID_A_COMBINE           ,     //A相组合有功总电能
    E_REPORT_ID_B_COMBINE           ,     //B相组合有功总电能
    E_REPORT_ID_C_COMBINE           ,     //C相组合有功总电能
    E_REPORT_ID_METER_STATE         ,     //电表状态
    E_REPORT_ID_ORIGINAL_VOLTAGE_A  ,     //A相基波电压
    E_REPORT_ID_ORIGINAL_VOLTAGE_B  ,     //B相基波电压
    E_REPORT_ID_ORIGINAL_VOLTAGE_C  ,     //C相基波电压
    E_REPORT_ID_ORIGINAL_CURRENT_A  ,     //A相基波电流
    E_REPORT_ID_ORIGINAL_CURRENT_B  ,     //B相基波电流
    E_REPORT_ID_ORIGINAL_CURRENT_C  ,     //C相基波电流
    E_REPORT_ID_ORIGINAL_ACTIVE_T   ,     //总基波有功功率
    E_REPORT_ID_ORIGINAL_ACTIVE_A   ,     //A相基波有功功率
    E_REPORT_ID_ORIGINAL_ACTIVE_B   ,     //B相基波有功功率
    E_REPORT_ID_ORIGINAL_ACTIVE_C   ,     //C相基波有功功率
    E_REPORT_ID_HARMONIC_VOLTAGE_A  ,     //A相谐波电压
    E_REPORT_ID_HARMONIC_VOLTAGE_B  ,     //B相谐波电压
    E_REPORT_ID_HARMONIC_VOLTAGE_C  ,     //C相谐波电压
    E_REPORT_ID_HARMONIC_CURRENT_A  ,     //A相谐波电流
    E_REPORT_ID_HARMONIC_CURRENT_B  ,     //B相谐波电流
    E_REPORT_ID_HARMONIC_CURRENT_C  ,     //C相谐波电流
    E_REPORT_ID_HARMONIC_ACTIVE_T   ,     //总谐波有功功率
    E_REPORT_ID_HARMONIC_ACTIVE_A   ,     //A相谐波有功功率
    E_REPORT_ID_HARMONIC_ACTIVE_B   ,     //B相谐波有功功率
    E_REPORT_ID_HARMONIC_ACTIVE_C   ,     //C相谐波有功功率
    E_REPORT_ID_THD_UA              ,     //Ua总畸变率
    E_REPORT_ID_THD_UB              ,     //Ub总畸变率
    E_REPORT_ID_THD_UC              ,     //Uc总畸变率
    E_REPORT_ID_THD_UT              ,     //合相电压总畸变率
    E_REPORT_ID_THD_IA              ,     //Ia总畸变率
    E_REPORT_ID_THD_IB              ,     //Ib总畸变率
    E_REPORT_ID_THD_IC              ,     //Ic总畸变率
    E_REPORT_ID_THD_IT              ,     //合相电流总畸变率
    E_REPORT_ID_HARMONIC_UA_2       ,     //UA 2次谐波
    E_REPORT_ID_HARMONIC_UA_3       ,     //UA 3次谐波
    E_REPORT_ID_HARMONIC_UA_4       ,     //UA 4次谐波
    E_REPORT_ID_HARMONIC_UA_5       ,     //UA 5次谐波
    E_REPORT_ID_HARMONIC_UA_6       ,     //UA 6次谐波
    E_REPORT_ID_HARMONIC_UA_7       ,     //UA 7次谐波
    E_REPORT_ID_HARMONIC_UA_8       ,     //UA 8次谐波
    E_REPORT_ID_HARMONIC_UA_9       ,     //UA 9次谐波
    E_REPORT_ID_HARMONIC_UA_10      ,     //UA 10次谐波
    E_REPORT_ID_HARMONIC_UA_11      ,     //UA 11次谐波
    E_REPORT_ID_HARMONIC_UA_12      ,     //UA 12次谐波
    E_REPORT_ID_HARMONIC_UA_13      ,     //UA 13次谐波
    E_REPORT_ID_HARMONIC_UA_14      ,     //UA 14次谐波
    E_REPORT_ID_HARMONIC_UA_15      ,     //UA 15次谐波
    E_REPORT_ID_HARMONIC_UA_16      ,     //UA 16次谐波
    E_REPORT_ID_HARMONIC_UA_17      ,     //UA 17次谐波
    E_REPORT_ID_HARMONIC_UA_18      ,     //UA 18次谐波
    E_REPORT_ID_HARMONIC_UA_19      ,     //UA 19次谐波
    E_REPORT_ID_HARMONIC_UA_20      ,     //UA 20次谐波
    E_REPORT_ID_HARMONIC_UA_21      ,     //UA 21次谐波
    E_REPORT_ID_HARMONIC_UA_ODD     ,     //UA 奇次谐波畸变率
    E_REPORT_ID_HARMONIC_UA_EVEN    ,     //UA 偶次谐波畸变率
    E_REPORT_ID_HARMONIC_UA_RIDGE   ,     //UA 波峰系数
    E_REPORT_ID_HARMONIC_UA_TETL    ,     //UA 电话谐波波形因数
    E_REPORT_ID_HARMONIC_UB_2       ,     //UB 2次谐波
    E_REPORT_ID_HARMONIC_UB_3       ,     //UB 3次谐波
    E_REPORT_ID_HARMONIC_UB_4       ,     //UB 4次谐波
    E_REPORT_ID_HARMONIC_UB_5       ,     //UB 5次谐波
    E_REPORT_ID_HARMONIC_UB_6       ,     //UB 6次谐波
    E_REPORT_ID_HARMONIC_UB_7       ,     //UB 7次谐波
    E_REPORT_ID_HARMONIC_UB_8       ,     //UB 8次谐波
    E_REPORT_ID_HARMONIC_UB_9       ,     //UB 9次谐波
    E_REPORT_ID_HARMONIC_UB_10      ,     //UB 10次谐波
    E_REPORT_ID_HARMONIC_UB_11      ,     //UB 11次谐波
    E_REPORT_ID_HARMONIC_UB_12      ,     //UB 12次谐波
    E_REPORT_ID_HARMONIC_UB_13      ,     //UB 13次谐波
    E_REPORT_ID_HARMONIC_UB_14      ,     //UB 14次谐波
    E_REPORT_ID_HARMONIC_UB_15      ,     //UB 15次谐波
    E_REPORT_ID_HARMONIC_UB_16      ,     //UB 16次谐波
    E_REPORT_ID_HARMONIC_UB_17      ,     //UB 17次谐波
    E_REPORT_ID_HARMONIC_UB_18      ,     //UB 18次谐波
    E_REPORT_ID_HARMONIC_UB_19      ,     //UB 19次谐波
    E_REPORT_ID_HARMONIC_UB_20      ,     //UB 20次谐波
    E_REPORT_ID_HARMONIC_UB_21      ,     //UB 21次谐波
    E_REPORT_ID_HARMONIC_UB_ODD     ,     //UB 奇次谐波畸变率
    E_REPORT_ID_HARMONIC_UB_EVEN    ,     //UB 偶次谐波畸变率
    E_REPORT_ID_HARMONIC_UB_RIDGE   ,     //UB 波峰系数
    E_REPORT_ID_HARMONIC_UB_TETL    ,     //UB 电话谐波波形因数
    E_REPORT_ID_HARMONIC_UC_2       ,     //UC 2次谐波
    E_REPORT_ID_HARMONIC_UC_3       ,     //UC 3次谐波
    E_REPORT_ID_HARMONIC_UC_4       ,     //UC 4次谐波
    E_REPORT_ID_HARMONIC_UC_5       ,     //UC 5次谐波
    E_REPORT_ID_HARMONIC_UC_6       ,     //UC 6次谐波
    E_REPORT_ID_HARMONIC_UC_7       ,     //UC 7次谐波
    E_REPORT_ID_HARMONIC_UC_8       ,     //UC 8次谐波
    E_REPORT_ID_HARMONIC_UC_9       ,     //UC 9次谐波
    E_REPORT_ID_HARMONIC_UC_10      ,     //UC 10次谐波
    E_REPORT_ID_HARMONIC_UC_11      ,     //UC 11次谐波
    E_REPORT_ID_HARMONIC_UC_12      ,     //UC 12次谐波
    E_REPORT_ID_HARMONIC_UC_13      ,     //UC 13次谐波
    E_REPORT_ID_HARMONIC_UC_14      ,     //UC 14次谐波
    E_REPORT_ID_HARMONIC_UC_15      ,     //UC 15次谐波
    E_REPORT_ID_HARMONIC_UC_16      ,     //UC 16次谐波
    E_REPORT_ID_HARMONIC_UC_17      ,     //UC 17次谐波
    E_REPORT_ID_HARMONIC_UC_18      ,     //UC 18次谐波
    E_REPORT_ID_HARMONIC_UC_19      ,     //UC 19次谐波
    E_REPORT_ID_HARMONIC_UC_20      ,     //UC 20次谐波
    E_REPORT_ID_HARMONIC_UC_21      ,     //UC21次谐波
    E_REPORT_ID_HARMONIC_UC_ODD     ,     //UC 奇次谐波畸变率
    E_REPORT_ID_HARMONIC_UC_EVEN    ,     //UC 偶次谐波畸变率
    E_REPORT_ID_HARMONIC_UC_RIDGE   ,     //UC 波峰系数
    E_REPORT_ID_HARMONIC_UC_TETL    ,     //UC 电话谐波波形因数
    E_REPORT_ID_HARMONIC_IA_2       ,     //IA 2次谐波
    E_REPORT_ID_HARMONIC_IA_3       ,     //IA 3次谐波
    E_REPORT_ID_HARMONIC_IA_4       ,     //IA 4次谐波
    E_REPORT_ID_HARMONIC_IA_5       ,     //IA 5次谐波
    E_REPORT_ID_HARMONIC_IA_6       ,     //IA 6次谐波
    E_REPORT_ID_HARMONIC_IA_7       ,     //IA 7次谐波
    E_REPORT_ID_HARMONIC_IA_8       ,     //IA 8次谐波
    E_REPORT_ID_HARMONIC_IA_9       ,     //IA 9次谐波
    E_REPORT_ID_HARMONIC_IA_10      ,     //IA 10次谐波
    E_REPORT_ID_HARMONIC_IA_11      ,     //IA 11次谐波
    E_REPORT_ID_HARMONIC_IA_12      ,     //IA 12次谐波
    E_REPORT_ID_HARMONIC_IA_13      ,     //IA 13次谐波
    E_REPORT_ID_HARMONIC_IA_14      ,     //IA 14次谐波
    E_REPORT_ID_HARMONIC_IA_15      ,     //IA 15次谐波
    E_REPORT_ID_HARMONIC_IA_16      ,     //IA 16次谐波
    E_REPORT_ID_HARMONIC_IA_17      ,     //IA 17次谐波
    E_REPORT_ID_HARMONIC_IA_18      ,     //IA 18次谐波
    E_REPORT_ID_HARMONIC_IA_19      ,     //IA 19次谐波
    E_REPORT_ID_HARMONIC_IA_20      ,     //IA 20次谐波
    E_REPORT_ID_HARMONIC_IA_21      ,     //IA21次谐波
    E_REPORT_ID_HARMONIC_IA_ODD     ,     //IA 奇次谐波畸变率
    E_REPORT_ID_HARMONIC_IA_EVEN    ,     //IA 偶次谐波畸变率
    E_REPORT_ID_HARMONIC_IA_K       ,     //IA  K系数
    E_REPORT_ID_HARMONIC_IB_2       ,     //IB 2次谐波
    E_REPORT_ID_HARMONIC_IB_3       ,     //IB 3次谐波
    E_REPORT_ID_HARMONIC_IB_4       ,     //IB 4次谐波
    E_REPORT_ID_HARMONIC_IB_5       ,     //IB 5次谐波
    E_REPORT_ID_HARMONIC_IB_6       ,     //IB 6次谐波
    E_REPORT_ID_HARMONIC_IB_7       ,     //IB 7次谐波
    E_REPORT_ID_HARMONIC_IB_8       ,     //IB 8次谐波
    E_REPORT_ID_HARMONIC_IB_9       ,     //IB 9次谐波
    E_REPORT_ID_HARMONIC_IB_10      ,     //IB 10次谐波
    E_REPORT_ID_HARMONIC_IB_11      ,     //IB 11次谐波
    E_REPORT_ID_HARMONIC_IB_12      ,     //IB 12次谐波
    E_REPORT_ID_HARMONIC_IB_13      ,     //IB 13次谐波
    E_REPORT_ID_HARMONIC_IB_14      ,     //IB 14次谐波
    E_REPORT_ID_HARMONIC_IB_15      ,     //IB 15次谐波
    E_REPORT_ID_HARMONIC_IB_16      ,     //IB 16次谐波
    E_REPORT_ID_HARMONIC_IB_17      ,     //IB 17次谐波
    E_REPORT_ID_HARMONIC_IB_18      ,     //IB 18次谐波
    E_REPORT_ID_HARMONIC_IB_19      ,     //IB 19次谐波
    E_REPORT_ID_HARMONIC_IB_20      ,     //IB 20次谐波
    E_REPORT_ID_HARMONIC_IB_21      ,     //IB 21次谐波
    E_REPORT_ID_HARMONIC_IB_ODD     ,     //IB 奇次谐波畸变率
    E_REPORT_ID_HARMONIC_IB_EVEN    ,     //IB 偶次谐波畸变率
    E_REPORT_ID_HARMONIC_IB_K       ,     //IB  K系数
    E_REPORT_ID_HARMONIC_IC_2       ,     //IC 2次谐波
    E_REPORT_ID_HARMONIC_IC_3       ,     //IC 3次谐波
    E_REPORT_ID_HARMONIC_IC_4       ,     //IC 4次谐波
    E_REPORT_ID_HARMONIC_IC_5       ,     //IC 5次谐波
    E_REPORT_ID_HARMONIC_IC_6       ,     //IC 6次谐波
    E_REPORT_ID_HARMONIC_IC_7       ,     //IC 7次谐波
    E_REPORT_ID_HARMONIC_IC_8       ,     //IC 8次谐波
    E_REPORT_ID_HARMONIC_IC_9       ,     //IC 9次谐波
    E_REPORT_ID_HARMONIC_IC_10      ,     //IC 10次谐波
    E_REPORT_ID_HARMONIC_IC_11      ,     //IC 11次谐波
    E_REPORT_ID_HARMONIC_IC_12      ,     //IC 12次谐波
    E_REPORT_ID_HARMONIC_IC_13      ,     //IC 13次谐波
    E_REPORT_ID_HARMONIC_IC_14      ,     //IC 14次谐波
    E_REPORT_ID_HARMONIC_IC_15      ,     //IC 15次谐波
    E_REPORT_ID_HARMONIC_IC_16      ,     //IC 16次谐波
    E_REPORT_ID_HARMONIC_IC_17      ,     //IC 17次谐波
    E_REPORT_ID_HARMONIC_IC_18      ,     //IC 18次谐波
    E_REPORT_ID_HARMONIC_IC_19      ,     //IC 19次谐波
    E_REPORT_ID_HARMONIC_IC_20      ,     //IC 20次谐波
    E_REPORT_ID_HARMONIC_IC_21      ,     //IC 21次谐波
    E_REPORT_ID_HARMONIC_IC_ODD     ,     //IC 奇次谐波畸变率
    E_REPORT_ID_HARMONIC_IC_EVEN    ,     //IC 偶次谐波畸变率
    E_REPORT_ID_HARMONIC_IC_K       ,//IC  K系数
    E_REPORT_ID_HEAT_SETTLE_DATA,//结算日热量
    E_REPORT_ID_CUR_HEAT,//当前热量
    E_REPORT_ID_THERMAL_POWER,//热功率
    E_REPORT_ID_FLOW,//流量
    E_REPORT_ID_CUMULA_FLOW,//累计流量
    E_REPORT_ID_SUPPLYWATER_TEMP,//供水温度
    E_REPORT_ID_BACKWATER_TEMP,//回水温度
    E_REPORT_ID_CUMULA_TIME,//累计工作时间
    E_REPORT_ID_CUR_CUMULA_FLOW = 231,//当前累计流量
    E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA,//结算日累计流量
    E_REPORT_ID_METER_TIME = 245,//时间
    E_REPORT_ID_THRESHOLD_STATE,//阀值告警状态
    E_REPORT_ID_T_COMBINE_RATE1,//组合有功费率1电能
    E_REPORT_ID_T_COMBINE_RATE2,//组合有功费率2电能
    E_REPORT_ID_T_COMBINE_RATE3,//组合有功费率3电能
    E_REPORT_ID_T_COMBINE_RATE4,//组合有功费率4电能
    E_REPORT_ID_T_POSITIVE_RATE1,//正向有功费率1电能
    E_REPORT_ID_T_POSITIVE_RATE2,//正向有功费率2电能
    E_REPORT_ID_T_POSITIVE_RATE3,//正向有功费率3电能
    E_REPORT_ID_T_POSITIVE_RATE4,//正向有功费率4电能
    E_REPORT_ID_T_REVERSE_RATE1,//反向有功费率1电能
    E_REPORT_ID_T_REVERSE_RATE2,//反向有功费率2电能
    E_REPORT_ID_T_REVERSE_RATE3,//反向有功费率3电能
    E_REPORT_ID_T_REVERSE_RATE4,//反向有功费率4电能
    E_REPORT_ID_T_COMBINE1_RATE1,//组合无功1费率1电能
    E_REPORT_ID_T_COMBINE1_RATE2,//组合无功1费率2电能
    E_REPORT_ID_T_COMBINE1_RATE3,//组合无功1费率3电能
    E_REPORT_ID_T_COMBINE1_RATE4,//组合无功1费率4电能
    E_REPORT_ID_T_COMBINE2_RATE1,//组合无功2费率1电能
    E_REPORT_ID_T_COMBINE2_RATE2,//组合无功2费率2电能
    E_REPORT_ID_T_COMBINE2_RATE3,//组合无功2费率3电能
    E_REPORT_ID_T_COMBINE2_RATE4,//组合无功2费率4电能
    E_REPORT_ID_E_OP_FAULT,//电操故障
    E_REPORT_ID_ON_OFF_STATE,//合分状态
    E_REPORT_ID_ON_COUNT,//重合次数
    E_REPORT_ID_ON_TIMER,//重合时间间隔
    E_REPORT_ID_ON_THINK_TIME//成功合闸判断时间
} e_report_param_id;

typedef struct
{
    int param_id;//电参量id
    int param_unit;//单位,与标准参量单位的比值，
    //unit的映射关系 {0->0.001; 1->0.01; 2->0.1; 3->1; 4->10; 5->100; 6->1000}
    int param_register_addr;//参数寄存器地址
    int param_register_len;//寄存器长度
}st_param_info;//参数信息

typedef struct
{
    int dev_type;//设备主类型
    int sub_type;//子类型

    //子类型名称,本地配置修改，界面自定义修改，映射到全局数组gSubTypeValue
    //注意：当值为空，使用全局数组gSubTypeName对应值
    char sub_type_name[128];

    //子类型回路个数.初始值为0.本地配置修改，界面自定义修改，映射到全局数组gWayCount
    int way_num;
    bool is_reg_same;//各回路寄存器信息是否一致
}st_def_sub_type_info;//自定义modbus协议信息

typedef enum
{
    UART_TRANSF = 0,			//串口传输
    NET_TRANSF					//网络传输
}e_transfer_type;

typedef enum
{
    SEND_FAIL = 1111,		//发送失败
    RECEIVE_FAIL//接收失败
}e_communicate_err;

typedef struct
{
    MU_INT16 relay_val;
    MU_INT8 dev_addr;
    MU_INT16 reg_addr;
}st_write_relay_param;

typedef struct
{
    MU_INT32 is_suc;
    MU_INT32 frm_sig;
}st_signal_parm;

typedef struct
{
    char user_name[128];
    char user_pwd[8];
    char user_descript[128];
}st_user_inf;
typedef deque<st_user_inf> DEQ_ST_USER_INF;
