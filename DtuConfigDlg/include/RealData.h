#pragma once
#include "include/mty.h"

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

