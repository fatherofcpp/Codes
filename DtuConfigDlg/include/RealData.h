#pragma once
#include "include/mty.h"

typedef struct
{
	MU_INT64 meter_time;//���ʱ��
	MU_INT16 meter_state;//���״̬
	MU_INT16 threshold_state;//��ֵ�澯״̬
	MU_INT16 volt_change;//��ѹ���
	MU_INT16 current_change;//�������
	MU_INT16 frequency;//Ƶ��
	MU_INT16 phase_volt[3];//A,B,C���ѹ
	MU_INT16 line_volt[3];//ab,bc,ca�ߵ�ѹ
	MS_INT16 phrase_current[3];//A,B,C�����
	MS_INT16 phrase_active[4];//����,A��,B�࣬C���й�����
	MS_INT16 phrase_reactive[4];//����,A��,B�࣬C���޹�����
	MS_INT16 phrase_apparent[4];//����,A��,B�࣬C�����ڹ���
	MS_INT16 factor[4];//��,A,B,C��������
	MS_INT16 n_current;//���ߵ���
	MU_INT32 combine_active_energy[5];//����й��ܵ���;����й�����1-4����
	MU_INT32 positive_active_energy[5];//�����й��ܵ���;�����й�����1-4����
	MU_INT32 reverse_active_energy[5];//�����й��ܵ��ܣ������й�����1-4����
	MU_INT32 combine_reactive_energy[2][5];//����޹�1�ܵ���,����޹�1����1-4����;����޹�2...
} st_am_3phase;//������ͨ���

typedef struct 
{
	MU_INT64 meter_time;
	MU_INT16 meter_state;
	MU_INT16 threshold_state;//��ֵ�澯״̬
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
} st_am_1phase;//������ͨ���

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
} st_am_dc;//ֱ����ͨ���

typedef struct
{
	MU_INT64 meter_time;                      
	MU_INT16 meter_state;         
	MU_INT16 threshold_state;//��ֵ�澯״̬
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
	MU_INT16 phrase_original_volt[3];//A,B,C�������ѹ                           
	MS_INT16 phrase_original_current[3]; //A,B,C���������                       
	MS_INT16 phrase_original_active[4];//��,A,B,C������й�����                               
	MU_INT16 phrase_harm_volt[3];//A,B,C��г����ѹ                           
	MS_INT16 phrase_harm_current[3];//A,B,C��г������                        
	MS_INT16 phrase_harm_active[4];//��,A,B,C��г���й�����           
	MU_INT16 total_distor[8];//Ua,Ub,Uc,�����ѹ,Ia,Ib,Ic��������ܻ�����                             

	MU_INT16 harm_u[3][20];//Ua,Ub,Uc 2-21��г��                                                    
	MU_INT16 harm_u_parity[3][2];//Ua,Ub,Uc�棬ż��г��������                           
	MU_INT16 harm_u_ridge[3];// Ua,Ub,Uc����ϵ��                             
	MU_INT16 harm_u_tele[3]; //Ua,Ub,Uc�绰г����������                              

	MU_INT16 harm_i[3][20];//Ia,Ib,Ic 2-21��г��   
	MU_INT16 harm_i_parity[3][2];//Ia,Ib,Ic �棬ż��г�������� 
	MU_INT16 harm_i_K[3];//Ia,Ib,IcKϵ�� 


} st_am_harm;//����г�����

typedef struct
{
	MU_INT64 meter_time;
	MU_INT16 meter_state;
	MU_INT64 cur_cumula_flow;//��ǰ�ۼ�����
	MU_INT64 cumula_flow_settle_data;//�������ۼ�����
	MU_INT32 cumula_his_data[12];//��1-12�½������ۼ�����
	MU_INT16 price1;//�۸�һ
	MU_INT16 amount1;//����һ
	MU_INT16 price2;
	MU_INT16 amount2;
	MU_INT16 price3;
	MU_INT16 settle_day;//������
	MU_INT16 reading_day;//������
	MU_INT16 purchase_no;//���ι������
	MU_INT32 purchase_amount;//���ι�����
	MU_INT32 cumula_purchase_amount;//�ۼƹ�����
	MU_INT32 balance;//ʣ����
	MU_INT16 version;//��Կ�汾��
} st_water_gass_meter;//ˮ��

typedef struct
{
	MU_INT64 e_op_time;//ʱ��
	MU_INT16 e_op_fault;//��ٹ���
	MU_INT16 on_off_state;//�Ϸ�״̬     
	MU_INT16 on_count;//�غϴ���   
	MU_INT16 on_timer;//�غ�ʱ����    
	MU_INT16 on_think_time;//�ɹ���բ�ж�ʱ��    
} st_e_op;//���ܵ��

typedef struct
{
	MU_INT64 heat_time;//ʱ��
	MU_INT16 heat_state;//״̬
	MU_INT64 heat_settle_data;//����������     
	MU_INT64 cur_heat;//��ǰ����   
	MU_INT64 thermal_power;//�ȹ���    
	MU_INT64 flow;//���� 
	MU_INT64 cumula_flow;//�ۼ����� 
	MU_INT32 supplywater_temp;//��ˮ�¶� 
	MU_INT32 backwater_temp;//��ˮ�¶� 
	MU_INT32 cumula_time;//�ۼƹ���ʱ�� 
	MU_INT32 heat_his_data[12];//��1-12�½��������� 
	MU_INT16 price1;//�۸�һ    
	MU_INT16 amount1;//����һ    
	MU_INT16 price2;//�۸��        
	MU_INT16 amount2;//������    
	MU_INT16 price3;//�۸���    
	MU_INT16 settle_day;//������    
	MU_INT16 reading_day;//������    
	MU_INT16 purchase_no;//���ι������    
	MU_INT32 purchase_amount;//���ι�����   
	MU_INT32 cumula_purchase_amount;//�ۼƹ�����   
	MU_INT32 balance;//ʣ����  
	MU_INT16 version;//��Կ�汾��  
} st_heat_meter;//������

typedef union
{
	st_am_3phase am_3phase;//������ͨ���
	st_am_1phase am_1phase;//������ͨ���
	st_am_dc am_dc;//ֱ����ͨ���
	st_am_harm am_harm;//����г�����
	st_e_op e_op;//���ܵ��
	st_water_gass_meter water_meter;//ˮ��
	st_water_gass_meter gass_meter;//����
	st_heat_meter heat_meter;//������
} u_w_measure;

