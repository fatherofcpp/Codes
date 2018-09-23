#pragma once
#include "include/mty.h"
//实时数据
typedef struct
{
    MU_INT64 meter_time;//电表时间
    MU_INT16 meter_state;//电表状态
    MU_INT16 threshold_state;//阈值告警状态
    MU_INT16 volt_change;//电压变比
    MU_INT16 current_change;//电流变比
    MU_INT16 frequency;//频率
    MU_INT16 phase_volt[3];//A,B,C相电压
    MU_INT16 line_volt[3];//ab,bc,ca线电压
    MS_INT16 phrase_current[3];//A,B,C相电流
    MS_INT16 phrase_active[4];//总相,A相,B相，C相有功功率
    MS_INT16 phrase_reactive[4];//总相,A相,B相，C相无功功率
    MS_INT16 phrase_apparent[4];//总相,A相,B相，C相视在功率
    MS_INT16 factor[4];//总,A,B,C功率因数
    MS_INT16 n_current;//零线电流
    MU_INT32 combine_active_energy[5];//组合有功总电能;组合有功费率1-4电能
    MU_INT32 positive_active_energy[5];//正向有功总电能;正向有功费率1-4电能
    MU_INT32 reverse_active_energy[5];//反向有功总电能：反向有功费率1-4电能
    MU_INT32 combine_reactive_energy[2][5];//组合无功1总电能,组合无功1费率1-4电能;组合无功2...
} st_am_3phase;//三相普通电表

typedef struct
{
    MU_INT64 meter_time;
    MU_INT16 meter_state;
    MU_INT16 threshold_state;//阈值告警状态
    MU_INT16 volt_change;
    MU_INT16 current_change;
    MU_INT16 frequency;
    MU_INT16 volt;
    MS_INT16 current;
    MS_INT16 active;
    MS_INT16 reactive;
    MS_INT16 apparent;
    MS_INT16 factor;
    MU_INT32 combine_active_energy[5];
    MU_INT32 positive_active_energy[5];
    MU_INT32 reverse_active_energy[5];
    MU_INT32 combine_reactive_energy[2][5];
} st_am_1phase;//单相普通电表

typedef struct
{
    MU_INT64 meter_time;
    MU_INT16 meter_state;
    MU_INT16 volt_change;
    MU_INT16 current_change;
    MU_INT16 volt;
    MS_INT16 current;
    MS_INT16 active;
    MU_INT32 energy;
} st_am_dc;//直流普通电表

typedef struct
{
    MU_INT64 meter_time;
    MU_INT16 meter_state;
    MU_INT16 threshold_state;//阈值告警状态
    MU_INT16 volt_change;
    MU_INT16 current_change;
    MU_INT16 frequency;
    MU_INT16 phase_volt[3];
    MU_INT16 line_volt[3];
    MS_INT16 phrase_current[3];
    MS_INT16 phrase_active[4];
    MS_INT16 phrase_reactive[4];
    MS_INT16 phrase_apparent[4];
    MS_INT16 factor[4];
    MS_INT16 n_current;
    MU_INT32 combine_active_energy[5];
    MU_INT32 positive_active_energy[5];
    MU_INT32 reverse_active_energy[5];
    MU_INT32 combine_reactive_energy[2][5];
    MU_INT16 phrase_original_volt[3];//A,B,C相基波电压
    MS_INT16 phrase_original_current[3]; //A,B,C相基波电流
    MS_INT16 phrase_original_active[4];//总,A,B,C相基波有功功率
    MU_INT16 phrase_harm_volt[3];//A,B,C相谐波电压
    MS_INT16 phrase_harm_current[3];//A,B,C相谐波电流
    MS_INT16 phrase_harm_active[4];//总,A,B,C相谐波有功功率
    MU_INT16 total_distor[8];//Ua,Ub,Uc,合相电压,Ia,Ib,Ic合相电流总畸变率

    MU_INT16 harm_u[3][20];//Ua,Ub,Uc 2-21次谐波
    MU_INT16 harm_u_parity[3][2];//Ua,Ub,Uc奇，偶次谐波畸变率
    MU_INT16 harm_u_ridge[3];// Ua,Ub,Uc波峰系数
    MU_INT16 harm_u_tele[3]; //Ua,Ub,Uc电话谐波波形因数

    MU_INT16 harm_i[3][20];//Ia,Ib,Ic 2-21次谐波
    MU_INT16 harm_i_parity[3][2];//Ia,Ib,Ic 奇，偶次谐波畸变率
    MU_INT16 harm_i_K[3];//Ia,Ib,IcK系数


} st_am_harm;//三相谐波电表

typedef struct
{
    MU_INT64 meter_time;
    MU_INT16 meter_state;
    MU_INT64 cur_cumula_flow;//当前累计流量
    MU_INT64 cumula_flow_settle_data;//结算日累计流量
    MU_INT32 cumula_his_data[12];//上1-12月结算日累计流量
    MU_INT16 price1;//价格一
    MU_INT16 amount1;//用量一
    MU_INT16 price2;
    MU_INT16 amount2;
    MU_INT16 price3;
    MU_INT16 settle_day;//结算日
    MU_INT16 reading_day;//抄表日
    MU_INT16 purchase_no;//本次购买序号
    MU_INT32 purchase_amount;//本次购入金额
    MU_INT32 cumula_purchase_amount;//累计购入金额
    MU_INT32 balance;//剩余金额
    MU_INT16 version;//密钥版本号
} st_water_gass_meter;//水表

typedef struct
{
    MU_INT64 e_op_time;//时间
    MU_INT16 e_op_fault;//电操故障
    MU_INT16 on_off_state;//合分状态
    MU_INT16 on_count;//重合次数
    MU_INT16 on_timer;//重合时间间隔
    MU_INT16 on_think_time;//成功合闸判断时间
} st_e_op;//智能电操

typedef struct
{
    MU_INT64 heat_time;//时间
    MU_INT16 heat_state;//状态
    MU_INT64 heat_settle_data;//结算日热量
    MU_INT64 cur_heat;//当前热量
    MU_INT64 thermal_power;//热功率
    MU_INT64 flow;//流量
    MU_INT64 cumula_flow;//累计流量
    MU_INT32 supplywater_temp;//供水温度
    MU_INT32 backwater_temp;//回水温度
    MU_INT32 cumula_time;//累计工作时间
    MU_INT32 heat_his_data[12];//上1-12月结算日热量
    MU_INT16 price1;//价格一
    MU_INT16 amount1;//用量一
    MU_INT16 price2;//价格二
    MU_INT16 amount2;//用量二
    MU_INT16 price3;//价格三
    MU_INT16 settle_day;//结算日
    MU_INT16 reading_day;//抄表日
    MU_INT16 purchase_no;//本次购买序号
    MU_INT32 purchase_amount;//本次购入金额
    MU_INT32 cumula_purchase_amount;//累计购入金额
    MU_INT32 balance;//剩余金额
    MU_INT16 version;//密钥版本号
} st_heat_meter;//热量表

typedef union
{
    st_am_3phase am_3phase;//三相普通电表
    st_am_1phase am_1phase;//单相普通电表
    st_am_dc am_dc;//直流普通电表
    st_am_harm am_harm;//三相谐波电表
    st_e_op e_op;//智能电操
    st_water_gass_meter water_meter;//水表
    st_water_gass_meter gass_meter;//气表
    st_heat_meter heat_meter;//热量表
} u_w_measure;

//------------------------------------------------------
typedef struct
{
    MU_INT16 u_phase[3];//A,B,C相电压(0.1V)
    MU_INT16 u_line[3];//AB,BC,CA线电压(0.1V)
    MU_INT16 frequence;//频率(0.01Hz)
    MU_INT16 i_phase[4];//进线A,B,C相,零序电流(0.01A)
    MS_INT16 power_total[2];//进线总有功,无功功率(0.01)
    MU_INT16 power_apparent;//视在功率(0.01)
    MS_INT16 power_factor_total;//进线总功率因数(0.01)
    MU_INT32 energy_total[2];//进线总有，无功电能(0.01)
    MS_INT16 power_phase[3][2];//进线A,B,C相有功,无功功率(0.01)
    MU_INT16 power_phase_apparent[3];//视在功率(0.01)
    MS_INT16 power_factor_phase[3];//进线A,B,C相功率因数(0.01)
    MU_INT32 energy_phase[3][2];//进线A,B,C相有功,无功电能(0.01)
    MU_INT16 unbanlance_phase[2];//输入主进线的三相电压,电流不平衡度(0.01%)
}st_inline_m_param;//主进线电参数数据

typedef struct
{
    MU_INT16 u_harm[4];//,A,B,C,合相 电压谐波分量(0.01%)
    MU_INT16 i_harm[4];//,A,B,C,合相 电流谐波分量(0.01%)
    MU_INT16 u_harm_percent[3][30];//UA,UB,UC 2-31次谐波含量百分比(0.01%)
    MU_INT16 u_harm_ratio[3][4];//UA,UB,UC 奇次，偶次，波峰系数,电话谐波波形因数(0.01%)
    MU_INT16 i_harm_percent[3][30];//IA,IB,IC 2-31次谐波含量百分比(0.01%)
    MU_INT16 i_harm_ratio[3][3];//IA,IB,IC 奇次，偶次， K系数(0.01%)
}st_harm_param;

typedef struct
{
    //reg 0x0000-0x0067(len:104)
    //基本参数数据
    MU_INT16 meter_addr;//表地址
    MU_INT16 cur_time[3];//当前时间，YYMM,DDHH,MMSS
    MU_INT16 rs_485[3][2];//上行通讯口1波特率
    MU_INT16 relay[2];//进线继电器1-2输出状态
    MU_INT16 switch_on_ele[2];//进线开关量输入状态
    MU_INT16 v_range[2];//电压量程1-2(0.1v)
    MU_INT16 i_m_range[2];//进线1-2电流量程(0.01A)
    MU_INT16 i_range[84];//出线1-84电流量程(0.01A)
    MU_INT16 v_ratio[2];//电压变比1-2
    MU_INT16 i_ratio[2];//进线1-2电流变比
    MU_INT16 version;//版本号

    //reg 0x1000-0x1061(len:98)
    //主进线电参数数据
    st_inline_m_param inline_m_param[2];

    //reg 0x2000-0x22F3(len:756)
    //输出支路电参数数据（单相）
    MU_INT16 i_out_line[84];//出线第1-84路电流(0.01)
    MS_INT16 power_out_line[3][84];//出线第1-84路有功,无功，视在功率(0.01)
    MS_INT16 power_factor_out_line[84];//出线第1-84路功率因数(0.01)
    MU_INT32 energy_out_line[2][84];//出线第1-84路有功,无功电能(0.01)

    //reg 0x3000-0x30DF(len:224)
    //输出支路电参数数据（三相）
    MS_INT16 power_total_3_phase_out_line[2][28];//出线第1-28路三相总有功,无功功率(0.01)
    MU_INT16 power_total_3_phase_apparent[28];//出线第1-28路三相总视在功率(0.01)
    MS_INT16 power_factor_total_3_phase_out_line[28];//出线第1-28路三相总功率因数(0.01)
    MU_INT32 energy_total_3_phase_out_line[2][28];//出线第1-28路三相总有功,无功电能(0.01)

    //reg 0x4000-0x41A1(len:418)
    //谐波参数数据
    st_harm_param harm_param[2];

    //reg 0x6000-0x6006(len:7)
    //事件记录
    MU_INT16 u_err_power_negative;//进线功率是否为负;
    MU_INT16 power_negative[6];//出线 1~16,17~32,33~48,49~64,65~80,81~84路功率是否为负
}st_ac_meter;//交流电表

typedef struct
{
    MU_INT16 meter_addr;//表地址
    MU_INT16 baud_parity[4];
    MU_INT16 version;//版本号

    /*
1~16路开关量输入
Bit8~bit15代表9~16路
Bit0~bit7代表1~8路
1：有开关量输入
0：无开关量输入
17~32路开关量输入
Bit8~bit15代表25~32路
Bit0~bit7代表17~24路
1：有开关量输入
0：无开关量输入
33~42路开关量输入
Bit8~bit9代表41~42路
Bit0~bit7代表33~40路
1：有开关量输入
0：无开关量输入
      */
    MU_INT64 switch_state;//开关状态
}st_switch_meter;//开关量

typedef union
{
    st_ac_meter ac_meter;//交流电表
    st_switch_meter switch_meter;//开关量
}u_dev_measure;

