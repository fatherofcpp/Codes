#include "StdAfx.h"
#include "ParamIdMap.h"

#include "CommonFunction.h"
#include "dataConvert.h"
#include "include/mty.h"

CParamIdMap::CParamIdMap(void)
{
	devTypeNum = DEV_TYPE_NUM;
	paramNum = gAllDevTypeReportParamNum;

	offSet = new int* [devTypeNum];
	assert(offSet);
	for (int i = 0; i < devTypeNum; i++)
	{
		offSet[i] = new int[paramNum];
		assert(offSet[i]);
	}

}


CParamIdMap::~CParamIdMap(void)
{
	for (int i = 0; i < devTypeNum; i++)
	{
		delete[] offSet[i];
	}
	delete[] offSet;
}

int CParamIdMap::GetOffSet( int dev_type, int param_id )
{
	return offSet[dev_type][param_id];
}

void CParamIdMap::SetOffSet()
{
#if 1
	return;
#else
	/*
	for (int i = 0; i < devTypeNum; i++)
	{
	for (int j = 0; j < paramNum; j++)
	{
	offSet[i][j] = -1;//初始化为-1
	}
	}

	u_w_measure* measure = new u_w_measure;
	memset(measure, 0, sizeof(* measure));

	for (int dev_type_ind = 0; dev_type_ind < devTypeNum; dev_type_ind++)
	{
	for (int param_id_ind = 0; param_id_ind < paramNum; param_id_ind ++)
	{
	switch(gDevTypeValue[dev_type_ind])
	{
	case AM_3PHASE:
	{ 	
	switch(gAllDevTypeReportParamValue[param_id_ind])
	{
	case E_REPORT_ID_METER_STATE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.meter_state - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_VLOT_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.volt_change - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_CURR_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.current_change - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_FREQUENCY: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.frequency - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_A_VOLT   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phase_volt[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_B_VOLT   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phase_volt[1] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_C_VOLT   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phase_volt[2] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_AB_VOLT  :
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.line_volt[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_BC_VOLT  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.line_volt[1] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_CA_VOLT  :
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.line_volt[2] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_A_CURRENT:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_current[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_B_CURRENT:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_current[1] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_C_CURRENT: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_current[2] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_ACTIVE   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_active[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_ACTIVE_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_active[1] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_ACTIVE_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_active[2] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_ACTIVE_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_active[3] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_REACTIVE  :
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_reactive[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_REACTIVE_A:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_reactive[1] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_REACTIVE_B:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_reactive[2] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_REACTIVE_C:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_reactive[3] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_APPARENT  :
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_apparent[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_APPARENT_A:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_apparent[1] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_APPARENT_B:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_apparent[2] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_APPARENT_C:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.phrase_apparent[3] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_FACTOR   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.factor[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_FACTOR_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.factor[1] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_FACTOR_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.factor[2] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_FACTOR_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.factor[3] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_N_CURRENT: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.n_current - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_COMBINE  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.combine_active_energy[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_POSITIVE : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.positive_active_energy[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_REVERSE  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.reverse_active_energy[0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_COMBINE1 :
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.combine_reactive_energy[0][0] - (char*)&measure->am_3phase;
	break;
	case E_REPORT_ID_COMBINE2 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_3phase.combine_reactive_energy[1][0] - (char*)&measure->am_3phase;
	break;
	default :
	break;
	}
	}

	case AM_1PHASE:
	{ 
	switch(param_id_ind)
	{
	case E_REPORT_ID_METER_STATE:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.meter_state - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_VLOT_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.volt_change - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_CURR_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.current_change - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_FREQUENCY  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.frequency - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_VOLTAGE : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.volt - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_CURRENT : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.current - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_ACTIVE  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.active - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_REACTIVE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.reactive - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_APPARENT: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.apparent - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_FACTOR  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.factor - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_COMBINE : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.combine_active_energy[0] - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_POSITIVE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.positive_active_energy[0] - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_REVERSE : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.reverse_active_energy[0] - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_COMBINE1: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.combine_reactive_energy[0][0] - (char*)&measure->am_1phase;
	break;
	case E_REPORT_ID_COMBINE2: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_1phase.combine_reactive_energy[1][0] - (char*)&measure->am_1phase;
	break;
	default :  
	break;
	}
	}

	case AM_DC:
	{ 
	switch(param_id_ind)
	{
	case E_REPORT_ID_METER_STATE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_dc.meter_state - (char*)&measure->am_dc;
	break;
	case E_REPORT_ID_VLOT_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_dc.volt_change - (char*)&measure->am_dc;
	break;
	case E_REPORT_ID_CURR_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_dc.current_change - (char*)&measure->am_dc;
	break;
	case E_REPORT_ID_VOLTAGE : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_dc.volt - (char*)&measure->am_dc;
	break;
	case E_REPORT_ID_CURRENT : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_dc.current - (char*)&measure->am_dc;
	break;
	case E_REPORT_ID_ACTIVE  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_dc.active - (char*)&measure->am_dc;
	break;
	case E_REPORT_ID_COMBINE : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_dc.energy - (char*)&measure->am_dc;
	break;
	default : 
	break;
	}
	}

	case AM_HARM:
	{ 
	switch(param_id_ind)
	{
	case E_REPORT_ID_METER_STATE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.meter_state - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_VLOT_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.volt_change - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_CURR_CHANGE: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.current_change - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_FREQUENCY: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.frequency - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_A_VOLT   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phase_volt[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_B_VOLT   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phase_volt[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_C_VOLT   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phase_volt[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_AB_VOLT  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.line_volt[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_BC_VOLT  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.line_volt[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_CA_VOLT  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.line_volt[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_A_CURRENT: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_current[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_B_CURRENT: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_current[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_C_CURRENT: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_current[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ACTIVE   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_active[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ACTIVE_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_active[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ACTIVE_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_active[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ACTIVE_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_active[3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_REACTIVE  :
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_reactive[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_REACTIVE_A:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_reactive[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_REACTIVE_B:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_reactive[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_REACTIVE_C:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_reactive[3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_APPARENT  :
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_apparent[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_APPARENT_A:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_apparent[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_APPARENT_B:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_apparent[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_APPARENT_C:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_apparent[3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_FACTOR   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.factor[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_FACTOR_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.factor[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_FACTOR_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.factor[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_FACTOR_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.factor[3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_N_CURRENT: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.n_current - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_COMBINE  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.combine_active_energy[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_POSITIVE : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.positive_active_energy[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_REVERSE  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.reverse_active_energy[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_COMBINE1 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.combine_reactive_energy[0][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_COMBINE2 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.combine_reactive_energy[1][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_VOLTAGE_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_volt[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_VOLTAGE_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_volt[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_VOLTAGE_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_volt[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_CURRENT_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_current[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_CURRENT_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_current[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_CURRENT_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_current[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_ACTIVE_T : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_active[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_ACTIVE_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_active[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_ACTIVE_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_active[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_ORIGINAL_ACTIVE_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_original_active[3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_VOLTAGE_A: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_volt[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_VOLTAGE_B: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_volt[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_VOLTAGE_C: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_volt[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_CURRENT_A: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_current[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_CURRENT_B: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_current[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_CURRENT_C: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_current[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_ACTIVE_T : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_active[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_ACTIVE_A : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_active[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_ACTIVE_B : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_active[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_ACTIVE_C : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.phrase_harm_active[3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_UA : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_UB : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_UC : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_UT : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_IA : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[4] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_IB : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[5] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_IC : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[6] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_THD_IT : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.total_distor[7] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_2  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_3  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_4  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][2]  - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_5  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][3]  - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_6  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][4]  - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_7  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][5] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_8  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][6]  - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_9  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][7] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_10 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][8] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_11 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][9] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_12 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][10] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_13 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][11] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_14 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][12] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_15 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][13] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_16 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][14] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_17 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][15] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_18 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][16] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_19 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][17] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_20 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][18] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_21 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[0][19] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_ODD:  
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_parity[0][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_EVEN: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_parity[0][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_RIDGE:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_ridge[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UA_TETL: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_tele[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_2  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_3  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_4  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_5  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_6  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][4] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_7  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][5] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_8  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][6] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_9  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][7] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_10 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][8] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_11 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][9] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_12 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][10] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_13 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][11] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_14 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][12] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_15 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][13] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_16 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][14] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_17 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][15] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_18 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][16] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_19 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][17] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_20 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][18] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_21 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[1][19] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_ODD:  
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_parity[1][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_EVEN: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_parity[1][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_RIDGE:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_ridge[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UB_TETL: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_tele[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_2  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_3  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_4  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_5  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_6  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][4] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_7  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][5] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_8  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][6] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_9  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][7] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_10 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][8] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_11 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][9] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_12 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][10] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_13 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][11] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_14 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][12] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_15 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][13] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_16 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][14] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_17 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][15] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_18 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][16] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_19 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][17] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_20 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][18] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_21 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u[2][19] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_ODD:  
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_parity[2][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_EVEN: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_parity[2][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_RIDGE:
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_ridge[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_UC_TETL: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_u_tele[2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_2  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_3  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_4  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_5  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_6  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][4] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_7  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][5] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_8  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][6] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_9  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][7] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_10 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][8] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_11 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][9] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_12 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][10] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_13 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][11] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_14 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][12] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_15 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][13] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_16 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][14] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_17 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][15] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_18 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][16] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_19 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][17] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_20 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][18] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_21 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[0][19] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_ODD : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_parity[0][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_EVEN: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_parity[0][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IA_K   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_K[0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_2  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_3  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_4  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_5  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_6  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][4] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_7  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][5] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_8  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][6] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_9  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][7] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_10 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][8] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_11 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][9] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_12 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][10] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_13 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][11] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_14 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][12] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_15 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][13] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_16 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][14] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_17 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][15] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_18 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][16] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_19 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][17] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_20 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][18] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_21 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[1][19] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_ODD : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_parity[1][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_EVEN: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_parity[1][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IB_K   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_K[1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_2  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_3  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_4  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][2] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_5  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][3] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_6  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][4] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_7  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][5] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_8  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][6] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_9  : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][7] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_10 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][8] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_11 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][9] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_12 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][10] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_13 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][11] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_14 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][12] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_15 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][13] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_16 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][14] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_17 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][15] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_18 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][16] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_19 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][17] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_20 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][18] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_21 : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i[2][19] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_ODD : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_parity[2][0] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_EVEN: 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_parity[2][1] - (char*)&measure->am_harm;
	break;
	case E_REPORT_ID_HARMONIC_IC_K   : 
	offSet[dev_type_ind][param_id_ind] 
	= (char*)&measure->am_harm.harm_i_K[2] - (char*)&measure->am_harm;
	break;
	default : 
	break;
	}
	}

	default:
	{
	break;    
	}
	}
	}

	}

	delete measure;
	*/
#endif
}

void CParamIdMap::SetValueByParamId( u_w_measure* measure, e_dev_type dev_type, char* data, int param_id )
{
	MU_INT8 flag = 0;
	CString unit_name = "";
	MU_INT64 lval = 0;
	switch(dev_type)
	{
	case AM_3PHASE:
		{ 	
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->am_3phase.meter_time, gHexType);
				break;
			case E_REPORT_ID_METER_STATE: 
				str_to_int(data, measure->am_3phase.meter_state, gHexType);
				break;
			case E_REPORT_ID_THRESHOLD_STATE:
				str_to_int(data, measure->am_3phase.threshold_state, gHexType);
				break;
			case E_REPORT_ID_VOLT_CHANGE: 
				str_to_int(data, measure->am_3phase.volt_change, gHexType);
				break;
			case E_REPORT_ID_CURR_CHANGE: 
				str_to_int(data, measure->am_3phase.current_change, gHexType);
				break;
			case E_REPORT_ID_FREQUENCY: 
				str_to_int(data, measure->am_3phase.frequency , gHexType);
				break;
			case E_REPORT_ID_A_VOLT   : 
				str_to_int(data, measure->am_3phase.phase_volt[0] , gHexType);
				break;
			case E_REPORT_ID_B_VOLT   : 
				str_to_int(data, measure->am_3phase.phase_volt[1] , gHexType);
				break;
			case E_REPORT_ID_C_VOLT   : 
				str_to_int(data, measure->am_3phase.phase_volt[2] , gHexType);
				break;
			case E_REPORT_ID_AB_VOLT  :
				str_to_int(data, measure->am_3phase.line_volt[0] , gHexType);
				break;
			case E_REPORT_ID_BC_VOLT  : 
				str_to_int(data, measure->am_3phase.line_volt[1] , gHexType);
				break;
			case E_REPORT_ID_CA_VOLT  :
				str_to_int(data, measure->am_3phase.line_volt[2] , gHexType);
				break;
			case E_REPORT_ID_A_CURRENT:
				str_to_int(data, measure->am_3phase.phrase_current[0] , gHexType);
				break;
			case E_REPORT_ID_B_CURRENT:
				str_to_int(data, measure->am_3phase.phrase_current[1] , gHexType);
				break;
			case E_REPORT_ID_C_CURRENT: 
				str_to_int(data, measure->am_3phase.phrase_current[2] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE   : 
				str_to_int(data, measure->am_3phase.phrase_active[0] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE_A : 
				str_to_int(data, measure->am_3phase.phrase_active[1] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE_B : 
				str_to_int(data, measure->am_3phase.phrase_active[2] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE_C : 
				str_to_int(data, measure->am_3phase.phrase_active[3] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE  :
				str_to_int(data, measure->am_3phase.phrase_reactive[0] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE_A:
				str_to_int(data, measure->am_3phase.phrase_reactive[1] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE_B:
				str_to_int(data, measure->am_3phase.phrase_reactive[2] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE_C:
				str_to_int(data, measure->am_3phase.phrase_reactive[3] , gHexType);
				break;
			case E_REPORT_ID_APPARENT  :
				str_to_int(data, measure->am_3phase.phrase_apparent[0] , gHexType);
				break;
			case E_REPORT_ID_APPARENT_A:
				str_to_int(data, measure->am_3phase.phrase_apparent[1] , gHexType);
				break;
			case E_REPORT_ID_APPARENT_B:
				str_to_int(data, measure->am_3phase.phrase_apparent[2] , gHexType);
				break;
			case E_REPORT_ID_APPARENT_C:
				str_to_int(data, measure->am_3phase.phrase_apparent[3] , gHexType);
				break;
			case E_REPORT_ID_FACTOR   : 
				str_to_int(data, measure->am_3phase.factor[0] , gHexType);
				break;
			case E_REPORT_ID_FACTOR_A : 
				str_to_int(data, measure->am_3phase.factor[1] , gHexType);
				break;
			case E_REPORT_ID_FACTOR_B : 
				str_to_int(data, measure->am_3phase.factor[2] , gHexType);
				break;
			case E_REPORT_ID_FACTOR_C : 
				str_to_int(data, measure->am_3phase.factor[3] , gHexType);
				break;
			case E_REPORT_ID_N_CURRENT: 
				str_to_int(data, measure->am_3phase.n_current , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE  : 
				str_to_int(data, measure->am_3phase.combine_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE1:
				str_to_int(data, measure->am_3phase.combine_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE2:
				str_to_int(data, measure->am_3phase.combine_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE3:
				str_to_int(data, measure->am_3phase.combine_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE4:
				str_to_int(data, measure->am_3phase.combine_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE : 
				str_to_int(data, measure->am_3phase.positive_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE1:
				str_to_int(data, measure->am_3phase.positive_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE2:
				str_to_int(data, measure->am_3phase.positive_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE3:
				str_to_int(data, measure->am_3phase.positive_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE4:
				str_to_int(data, measure->am_3phase.positive_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE: 
				str_to_int(data, measure->am_3phase.reverse_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE1:
				str_to_int(data, measure->am_3phase.reverse_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE2:
				str_to_int(data, measure->am_3phase.reverse_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE3:
				str_to_int(data, measure->am_3phase.reverse_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE4:
				str_to_int(data, measure->am_3phase.reverse_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_COMBINE1 :
				str_to_int(data, measure->am_3phase.combine_reactive_energy[0][0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE1:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[0][1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE2:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[0][2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE3:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[0][3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE4:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[0][4] , gHexType);
				break;
			case E_REPORT_ID_COMBINE2 : 
				str_to_int(data, measure->am_3phase.combine_reactive_energy[1][0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE1:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[1][1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE2:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[1][2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE3:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[1][3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE4:
				str_to_int(data, measure->am_3phase.combine_reactive_energy[1][4] , gHexType);
				break;

			default :
				break;
			}
			break;
		}

	case AM_1PHASE:
		{ 
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->am_1phase.meter_time, gHexType);
				break;
			case E_REPORT_ID_METER_STATE: 
				str_to_int(data, measure->am_1phase.meter_state, gHexType);
				break;
			case E_REPORT_ID_THRESHOLD_STATE:
				str_to_int(data, measure->am_1phase.threshold_state, gHexType);
				break;
			case E_REPORT_ID_VOLT_CHANGE: 
				str_to_int(data, measure->am_1phase.volt_change , gHexType);
				break;
			case E_REPORT_ID_CURR_CHANGE: 
				str_to_int(data, measure->am_1phase.current_change , gHexType);
				break;
			case E_REPORT_ID_FREQUENCY  : 
				str_to_int(data, measure->am_1phase.frequency , gHexType);
				break;
			case E_REPORT_ID_VOLT : 
				str_to_int(data, measure->am_1phase.volt , gHexType);
				break;
			case E_REPORT_ID_CURRENT : 
				str_to_int(data, measure->am_1phase.current , gHexType);
				break;
			case E_REPORT_ID_ACTIVE  : 
				str_to_int(data, measure->am_1phase.active , gHexType);
				break;
			case E_REPORT_ID_REACTIVE: 
				str_to_int(data, measure->am_1phase.reactive , gHexType);
				break;
			case E_REPORT_ID_APPARENT: 
				str_to_int(data, measure->am_1phase.apparent , gHexType);
				break;
			case E_REPORT_ID_FACTOR  : 
				str_to_int(data, measure->am_1phase.factor , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE : 
				str_to_int(data, measure->am_1phase.combine_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE1:
				str_to_int(data, measure->am_1phase.combine_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE2:
				str_to_int(data, measure->am_1phase.combine_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE3:
				str_to_int(data, measure->am_1phase.combine_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE4:
				str_to_int(data, measure->am_1phase.combine_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE: 
				str_to_int(data, measure->am_1phase.positive_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE1:
				str_to_int(data, measure->am_1phase.positive_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE2:
				str_to_int(data, measure->am_1phase.positive_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE3:
				str_to_int(data, measure->am_1phase.positive_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE4:
				str_to_int(data, measure->am_1phase.positive_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE : 
				str_to_int(data, measure->am_1phase.reverse_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE1:
				str_to_int(data, measure->am_1phase.reverse_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE2:
				str_to_int(data, measure->am_1phase.reverse_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE3:
				str_to_int(data, measure->am_1phase.reverse_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE4:
				str_to_int(data, measure->am_1phase.reverse_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_COMBINE1: 
				str_to_int(data, measure->am_1phase.combine_reactive_energy[0][0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE1:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[0][1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE2:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[0][2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE3:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[0][3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE4:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[0][4] , gHexType);
				break;
			case E_REPORT_ID_COMBINE2: 
				str_to_int(data, measure->am_1phase.combine_reactive_energy[1][0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE1:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[1][1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE2:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[1][2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE3:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[1][3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE4:
				str_to_int(data, measure->am_1phase.combine_reactive_energy[1][4] , gHexType);
				break;
			default :  
				break;
			}
			break;
		}

	case AM_DC:
		{ 
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->am_dc.meter_time, gHexType);
				break;
			case E_REPORT_ID_METER_STATE: 
				str_to_int(data, measure->am_dc.meter_state, gHexType);
				break;
			case E_REPORT_ID_VOLT_CHANGE: 
				str_to_int(data, measure->am_dc.volt_change , gHexType);
				break;
			case E_REPORT_ID_CURR_CHANGE: 
				str_to_int(data, measure->am_dc.current_change , gHexType);
				break;
			case E_REPORT_ID_VOLT : 
				str_to_int(data, measure->am_dc.volt , gHexType);
				break;
			case E_REPORT_ID_CURRENT : 
				str_to_int(data, measure->am_dc.current , gHexType);
				break;
			case E_REPORT_ID_ACTIVE  : 
				str_to_int(data, measure->am_dc.active , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE : 
				str_to_int(data, measure->am_dc.energy , gHexType);
				break;
			default : 
				break;
			}
			break;
		}

	case AM_HARM:
		{ 
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->am_harm.meter_time, gHexType);
				break;
			case E_REPORT_ID_METER_STATE: 
				str_to_int(data, measure->am_harm.meter_state, gHexType);
				break;
			case E_REPORT_ID_THRESHOLD_STATE:
				str_to_int(data, measure->am_harm.threshold_state, gHexType);
				break;
			case E_REPORT_ID_VOLT_CHANGE: 
				str_to_int(data, measure->am_harm.volt_change , gHexType);
				break;
			case E_REPORT_ID_CURR_CHANGE: 
				str_to_int(data, measure->am_harm.current_change , gHexType);
				break;
			case E_REPORT_ID_FREQUENCY: 
				str_to_int(data, measure->am_harm.frequency , gHexType);
				break;
			case E_REPORT_ID_A_VOLT   : 
				str_to_int(data, measure->am_harm.phase_volt[0] , gHexType);
				break;
			case E_REPORT_ID_B_VOLT   : 
				str_to_int(data, measure->am_harm.phase_volt[1] , gHexType);
				break;
			case E_REPORT_ID_C_VOLT   : 
				str_to_int(data, measure->am_harm.phase_volt[2] , gHexType);
				break;
			case E_REPORT_ID_AB_VOLT  : 
				str_to_int(data, measure->am_harm.line_volt[0] , gHexType);
				break;
			case E_REPORT_ID_BC_VOLT  : 
				str_to_int(data, measure->am_harm.line_volt[1] , gHexType);
				break;
			case E_REPORT_ID_CA_VOLT  : 
				str_to_int(data, measure->am_harm.line_volt[2] , gHexType);
				break;
			case E_REPORT_ID_A_CURRENT: 
				str_to_int(data, measure->am_harm.phrase_current[0] , gHexType);
				break;
			case E_REPORT_ID_B_CURRENT: 
				str_to_int(data, measure->am_harm.phrase_current[1] , gHexType);
				break;
			case E_REPORT_ID_C_CURRENT: 
				str_to_int(data, measure->am_harm.phrase_current[2] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE   : 
				str_to_int(data, measure->am_harm.phrase_active[0] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE_A : 
				str_to_int(data, measure->am_harm.phrase_active[1] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE_B : 
				str_to_int(data, measure->am_harm.phrase_active[2] , gHexType);
				break;
			case E_REPORT_ID_ACTIVE_C : 
				str_to_int(data, measure->am_harm.phrase_active[3] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE  :
				str_to_int(data, measure->am_harm.phrase_reactive[0] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE_A:
				str_to_int(data, measure->am_harm.phrase_reactive[1] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE_B:
				str_to_int(data, measure->am_harm.phrase_reactive[2] , gHexType);
				break;
			case E_REPORT_ID_REACTIVE_C:
				str_to_int(data, measure->am_harm.phrase_reactive[3] , gHexType);
				break;
			case E_REPORT_ID_APPARENT  :
				str_to_int(data, measure->am_harm.phrase_apparent[0] , gHexType);
				break;
			case E_REPORT_ID_APPARENT_A:
				str_to_int(data, measure->am_harm.phrase_apparent[1] , gHexType);
				break;
			case E_REPORT_ID_APPARENT_B:
				str_to_int(data, measure->am_harm.phrase_apparent[2] , gHexType);
				break;
			case E_REPORT_ID_APPARENT_C:
				str_to_int(data, measure->am_harm.phrase_apparent[3] , gHexType);
				break;
			case E_REPORT_ID_FACTOR   : 
				str_to_int(data, measure->am_harm.factor[0] , gHexType);
				break;
			case E_REPORT_ID_FACTOR_A : 
				str_to_int(data, measure->am_harm.factor[1] , gHexType);
				break;
			case E_REPORT_ID_FACTOR_B : 
				str_to_int(data, measure->am_harm.factor[2] , gHexType);
				break;
			case E_REPORT_ID_FACTOR_C : 
				str_to_int(data, measure->am_harm.factor[3] , gHexType);
				break;
			case E_REPORT_ID_N_CURRENT: 
				str_to_int(data, measure->am_harm.n_current , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE  : 
				str_to_int(data, measure->am_harm.combine_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE1:
				str_to_int(data, measure->am_harm.combine_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE2:
				str_to_int(data, measure->am_harm.combine_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE3:
				str_to_int(data, measure->am_harm.combine_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE_RATE4:
				str_to_int(data, measure->am_harm.combine_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE : 
				str_to_int(data, measure->am_harm.positive_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE1:
				str_to_int(data, measure->am_harm.positive_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE2:
				str_to_int(data, measure->am_harm.positive_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE3:
				str_to_int(data, measure->am_harm.positive_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_POSITIVE_RATE4:
				str_to_int(data, measure->am_harm.positive_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE  : 
				str_to_int(data, measure->am_harm.reverse_active_energy[0] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE1:
				str_to_int(data, measure->am_harm.reverse_active_energy[1] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE2:
				str_to_int(data, measure->am_harm.reverse_active_energy[2] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE3:
				str_to_int(data, measure->am_harm.reverse_active_energy[3] , gHexType);
				break;
			case E_REPORT_ID_T_REVERSE_RATE4:
				str_to_int(data, measure->am_harm.reverse_active_energy[4] , gHexType);
				break;
			case E_REPORT_ID_COMBINE1 : 
				str_to_int(data, measure->am_harm.combine_reactive_energy[0][0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE1:
				str_to_int(data, measure->am_harm.combine_reactive_energy[0][1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE2:
				str_to_int(data, measure->am_harm.combine_reactive_energy[0][2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE3:
				str_to_int(data, measure->am_harm.combine_reactive_energy[0][3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE1_RATE4:
				str_to_int(data, measure->am_harm.combine_reactive_energy[0][4] , gHexType);
				break;
			case E_REPORT_ID_COMBINE2 : 
				str_to_int(data, measure->am_harm.combine_reactive_energy[1][0] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE1:
				str_to_int(data, measure->am_harm.combine_reactive_energy[1][1] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE2:
				str_to_int(data, measure->am_harm.combine_reactive_energy[1][2] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE3:
				str_to_int(data, measure->am_harm.combine_reactive_energy[1][3] , gHexType);
				break;
			case E_REPORT_ID_T_COMBINE2_RATE4:
				str_to_int(data, measure->am_harm.combine_reactive_energy[1][4] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_VOLTAGE_A : 
				str_to_int(data, measure->am_harm.phrase_original_volt[0] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_VOLTAGE_B : 
				str_to_int(data, measure->am_harm.phrase_original_volt[1] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_VOLTAGE_C : 
				str_to_int(data, measure->am_harm.phrase_original_volt[2] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_CURRENT_A : 
				str_to_int(data, measure->am_harm.phrase_original_current[0] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_CURRENT_B : 
				str_to_int(data, measure->am_harm.phrase_original_current[1] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_CURRENT_C : 
				str_to_int(data, measure->am_harm.phrase_original_current[2] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_ACTIVE_T : 
				str_to_int(data, measure->am_harm.phrase_original_active[0] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_ACTIVE_A : 
				str_to_int(data, measure->am_harm.phrase_original_active[1] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_ACTIVE_B : 
				str_to_int(data, measure->am_harm.phrase_original_active[2] , gHexType);
				break;
			case E_REPORT_ID_ORIGINAL_ACTIVE_C : 
				str_to_int(data, measure->am_harm.phrase_original_active[3] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_VOLTAGE_A: 
				str_to_int(data, measure->am_harm.phrase_harm_volt[0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_VOLTAGE_B: 
				str_to_int(data, measure->am_harm.phrase_harm_volt[1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_VOLTAGE_C: 
				str_to_int(data, measure->am_harm.phrase_harm_volt[2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_CURRENT_A: 
				str_to_int(data, measure->am_harm.phrase_harm_current[0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_CURRENT_B: 
				str_to_int(data, measure->am_harm.phrase_harm_current[1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_CURRENT_C: 
				str_to_int(data, measure->am_harm.phrase_harm_current[2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_ACTIVE_T : 
				str_to_int(data, measure->am_harm.phrase_harm_active[0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_ACTIVE_A : 
				str_to_int(data, measure->am_harm.phrase_harm_active[1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_ACTIVE_B : 
				str_to_int(data, measure->am_harm.phrase_harm_active[2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_ACTIVE_C : 
				str_to_int(data, measure->am_harm.phrase_harm_active[3] , gHexType);
				break;
			case E_REPORT_ID_THD_UA : 
				str_to_int(data, measure->am_harm.total_distor[0] , gHexType);
				break;
			case E_REPORT_ID_THD_UB : 
				str_to_int(data, measure->am_harm.total_distor[1] , gHexType);
				break;
			case E_REPORT_ID_THD_UC : 
				str_to_int(data, measure->am_harm.total_distor[2] , gHexType);
				break;
			case E_REPORT_ID_THD_UT : 
				str_to_int(data, measure->am_harm.total_distor[3] , gHexType);
				break;
			case E_REPORT_ID_THD_IA : 
				str_to_int(data, measure->am_harm.total_distor[4] , gHexType);
				break;
			case E_REPORT_ID_THD_IB : 
				str_to_int(data, measure->am_harm.total_distor[5] , gHexType);
				break;
			case E_REPORT_ID_THD_IC : 
				str_to_int(data, measure->am_harm.total_distor[6] , gHexType);
				break;
			case E_REPORT_ID_THD_IT : 
				str_to_int(data, measure->am_harm.total_distor[7] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_2  : 
				str_to_int(data, measure->am_harm.harm_u[0][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_3  : 
				str_to_int(data, measure->am_harm.harm_u[0][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_4  : 
				str_to_int(data, measure->am_harm.harm_u[0][2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_5  : 
				str_to_int(data, measure->am_harm.harm_u[0][3] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_6  : 
				str_to_int(data, measure->am_harm.harm_u[0][4]  , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_7  : 
				str_to_int(data, measure->am_harm.harm_u[0][5] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_8  : 
				str_to_int(data, measure->am_harm.harm_u[0][6] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_9  : 
				str_to_int(data, measure->am_harm.harm_u[0][7] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_10 : 
				str_to_int(data, measure->am_harm.harm_u[0][8] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_11 : 
				str_to_int(data, measure->am_harm.harm_u[0][9] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_12 : 
				str_to_int(data, measure->am_harm.harm_u[0][10] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_13 : 
				str_to_int(data, measure->am_harm.harm_u[0][11] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_14 : 
				str_to_int(data, measure->am_harm.harm_u[0][12] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_15 : 
				str_to_int(data, measure->am_harm.harm_u[0][13] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_16 : 
				str_to_int(data, measure->am_harm.harm_u[0][14] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_17 : 
				str_to_int(data, measure->am_harm.harm_u[0][15] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_18 : 
				str_to_int(data, measure->am_harm.harm_u[0][16] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_19 : 
				str_to_int(data, measure->am_harm.harm_u[0][17] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_20 : 
				str_to_int(data, measure->am_harm.harm_u[0][18] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_21 : 
				str_to_int(data, measure->am_harm.harm_u[0][19] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_ODD:  
				str_to_int(data, measure->am_harm.harm_u_parity[0][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_EVEN: 
				str_to_int(data, measure->am_harm.harm_u_parity[0][1], gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_RIDGE:
				str_to_int(data, measure->am_harm.harm_u_ridge[0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UA_TETL: 
				str_to_int(data, measure->am_harm.harm_u_tele[0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_2  : 
				str_to_int(data, measure->am_harm.harm_u[1][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_3  : 
				str_to_int(data, measure->am_harm.harm_u[1][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_4  : 
				str_to_int(data, measure->am_harm.harm_u[1][2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_5  : 
				str_to_int(data, measure->am_harm.harm_u[1][3] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_6  : 
				str_to_int(data, measure->am_harm.harm_u[1][4] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_7  : 
				str_to_int(data, measure->am_harm.harm_u[1][5] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_8  : 
				str_to_int(data, measure->am_harm.harm_u[1][6] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_9  : 
				str_to_int(data, measure->am_harm.harm_u[1][7] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_10 : 
				str_to_int(data, measure->am_harm.harm_u[1][8] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_11 : 
				str_to_int(data, measure->am_harm.harm_u[1][9] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_12 : 
				str_to_int(data, measure->am_harm.harm_u[1][10] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_13 : 
				str_to_int(data, measure->am_harm.harm_u[1][11] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_14 : 
				str_to_int(data, measure->am_harm.harm_u[1][12] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_15 : 
				str_to_int(data, measure->am_harm.harm_u[1][13] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_16 : 
				str_to_int(data, measure->am_harm.harm_u[1][14] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_17 : 
				str_to_int(data, measure->am_harm.harm_u[1][15] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_18 : 
				str_to_int(data, measure->am_harm.harm_u[1][16] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_19 : 
				str_to_int(data, measure->am_harm.harm_u[1][17] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_20 : 
				str_to_int(data, measure->am_harm.harm_u[1][18] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_21 : 
				str_to_int(data, measure->am_harm.harm_u[1][19] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_ODD:  
				str_to_int(data, measure->am_harm.harm_u_parity[1][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_EVEN: 
				str_to_int(data, measure->am_harm.harm_u_parity[1][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_RIDGE:
				str_to_int(data, measure->am_harm.harm_u_ridge[1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UB_TETL: 
				str_to_int(data, measure->am_harm.harm_u_tele[1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_2  : 
				str_to_int(data, measure->am_harm.harm_u[2][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_3  : 
				str_to_int(data, measure->am_harm.harm_u[2][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_4  : 
				str_to_int(data, measure->am_harm.harm_u[2][2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_5  : 
				str_to_int(data, measure->am_harm.harm_u[2][3] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_6  : 
				str_to_int(data, measure->am_harm.harm_u[2][4] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_7  : 
				str_to_int(data, measure->am_harm.harm_u[2][5] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_8  : 
				str_to_int(data, measure->am_harm.harm_u[2][6] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_9  : 
				str_to_int(data, measure->am_harm.harm_u[2][7] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_10 : 
				str_to_int(data, measure->am_harm.harm_u[2][8] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_11 : 
				str_to_int(data, measure->am_harm.harm_u[2][9] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_12 : 
				str_to_int(data, measure->am_harm.harm_u[2][10] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_13 : 
				str_to_int(data, measure->am_harm.harm_u[2][11] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_14 : 
				str_to_int(data, measure->am_harm.harm_u[2][12] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_15 : 
				str_to_int(data, measure->am_harm.harm_u[2][13] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_16 : 
				str_to_int(data, measure->am_harm.harm_u[2][14] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_17 : 
				str_to_int(data, measure->am_harm.harm_u[2][15] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_18 : 
				str_to_int(data, measure->am_harm.harm_u[2][16] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_19 : 
				str_to_int(data, measure->am_harm.harm_u[2][17] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_20 : 
				str_to_int(data, measure->am_harm.harm_u[2][18] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_21 : 
				str_to_int(data, measure->am_harm.harm_u[2][19] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_ODD:  
				str_to_int(data, measure->am_harm.harm_u_parity[2][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_EVEN: 
				str_to_int(data, measure->am_harm.harm_u_parity[2][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_RIDGE:
				str_to_int(data, measure->am_harm.harm_u_ridge[2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_UC_TETL: 
				str_to_int(data, measure->am_harm.harm_u_tele[2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_2  : 
				str_to_int(data, measure->am_harm.harm_i[0][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_3  : 
				str_to_int(data, measure->am_harm.harm_i[0][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_4  : 
				str_to_int(data, measure->am_harm.harm_i[0][2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_5  : 
				str_to_int(data, measure->am_harm.harm_i[0][3] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_6  : 
				str_to_int(data, measure->am_harm.harm_i[0][4] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_7  : 
				str_to_int(data, measure->am_harm.harm_i[0][5] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_8  : 
				str_to_int(data, measure->am_harm.harm_i[0][6] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_9  : 
				str_to_int(data, measure->am_harm.harm_i[0][7] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_10 : 
				str_to_int(data, measure->am_harm.harm_i[0][8] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_11 : 
				str_to_int(data, measure->am_harm.harm_i[0][9] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_12 : 
				str_to_int(data, measure->am_harm.harm_i[0][10] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_13 : 
				str_to_int(data, measure->am_harm.harm_i[0][11] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_14 : 
				str_to_int(data, measure->am_harm.harm_i[0][12] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_15 : 
				str_to_int(data, measure->am_harm.harm_i[0][13] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_16 : 
				str_to_int(data, measure->am_harm.harm_i[0][14] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_17 : 
				str_to_int(data, measure->am_harm.harm_i[0][15] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_18 : 
				str_to_int(data, measure->am_harm.harm_i[0][16] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_19 : 
				str_to_int(data, measure->am_harm.harm_i[0][17] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_20 : 
				str_to_int(data, measure->am_harm.harm_i[0][18] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_21 : 
				str_to_int(data, measure->am_harm.harm_i[0][19] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_ODD : 
				str_to_int(data, measure->am_harm.harm_i_parity[0][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_EVEN: 
				str_to_int(data, measure->am_harm.harm_i_parity[0][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IA_K   : 
				str_to_int(data, measure->am_harm.harm_i_K[0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_2  : 
				str_to_int(data, measure->am_harm.harm_i[1][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_3  : 
				str_to_int(data, measure->am_harm.harm_i[1][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_4  : 
				str_to_int(data, measure->am_harm.harm_i[1][2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_5  : 
				str_to_int(data, measure->am_harm.harm_i[1][3] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_6  : 
				str_to_int(data, measure->am_harm.harm_i[1][4] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_7  : 
				str_to_int(data, measure->am_harm.harm_i[1][5] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_8  : 
				str_to_int(data, measure->am_harm.harm_i[1][6] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_9  : 
				str_to_int(data, measure->am_harm.harm_i[1][7] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_10 : 
				str_to_int(data, measure->am_harm.harm_i[1][8] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_11 : 
				str_to_int(data, measure->am_harm.harm_i[1][9] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_12 : 
				str_to_int(data, measure->am_harm.harm_i[1][10] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_13 : 
				str_to_int(data, measure->am_harm.harm_i[1][11] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_14 : 
				str_to_int(data, measure->am_harm.harm_i[1][12] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_15 : 
				str_to_int(data, measure->am_harm.harm_i[1][13] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_16 : 
				str_to_int(data, measure->am_harm.harm_i[1][14] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_17 : 
				str_to_int(data, measure->am_harm.harm_i[1][15] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_18 : 
				str_to_int(data, measure->am_harm.harm_i[1][16] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_19 : 
				str_to_int(data, measure->am_harm.harm_i[1][17] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_20 : 
				str_to_int(data, measure->am_harm.harm_i[1][18] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_21 : 
				str_to_int(data, measure->am_harm.harm_i[1][19] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_ODD : 
				str_to_int(data, measure->am_harm.harm_i_parity[1][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_EVEN: 
				str_to_int(data, measure->am_harm.harm_i_parity[1][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IB_K   : 
				str_to_int(data, measure->am_harm.harm_i_K[1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_2  : 
				str_to_int(data, measure->am_harm.harm_i[2][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_3  : 
				str_to_int(data, measure->am_harm.harm_i[2][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_4  : 
				str_to_int(data, measure->am_harm.harm_i[2][2] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_5  : 
				str_to_int(data, measure->am_harm.harm_i[2][3] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_6  : 
				str_to_int(data, measure->am_harm.harm_i[2][4] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_7  : 
				str_to_int(data, measure->am_harm.harm_i[2][5] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_8  : 
				str_to_int(data, measure->am_harm.harm_i[2][6] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_9  : 
				str_to_int(data, measure->am_harm.harm_i[2][7] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_10 : 
				str_to_int(data, measure->am_harm.harm_i[2][8] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_11 : 
				str_to_int(data, measure->am_harm.harm_i[2][9] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_12 : 
				str_to_int(data, measure->am_harm.harm_i[2][10] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_13 : 
				str_to_int(data, measure->am_harm.harm_i[2][11] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_14 : 
				str_to_int(data, measure->am_harm.harm_i[2][12] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_15 : 
				str_to_int(data, measure->am_harm.harm_i[2][13] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_16 : 
				str_to_int(data, measure->am_harm.harm_i[2][14] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_17 : 
				str_to_int(data, measure->am_harm.harm_i[2][15] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_18 : 
				str_to_int(data, measure->am_harm.harm_i[2][16] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_19 : 
				str_to_int(data, measure->am_harm.harm_i[2][17] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_20 : 
				str_to_int(data, measure->am_harm.harm_i[2][18] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_21 : 
				str_to_int(data, measure->am_harm.harm_i[2][19] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_ODD : 
				str_to_int(data, measure->am_harm.harm_i_parity[2][0] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_EVEN: 
				str_to_int(data, measure->am_harm.harm_i_parity[2][1] , gHexType);
				break;
			case E_REPORT_ID_HARMONIC_IC_K   : 
				str_to_int(data, measure->am_harm.harm_i_K[2] , gHexType);
				break;
			default : 
				break;
			}
			break;
		}

	case WM:
		{ 
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->water_meter.meter_time, gHexType);
				break;
			case E_REPORT_ID_METER_STATE://状态
				str_to_int(data, measure->water_meter.meter_state);
				break;
			case E_REPORT_ID_CUR_CUMULA_FLOW://当前累计流量
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->water_meter.cur_cumula_flow = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			case E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA://结算日累计流量
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->water_meter.cumula_flow_settle_data = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			default:
				break;
			}
			break;
		}
	case GM:
		{ 
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->gass_meter.meter_time, gHexType);
				break;
			case E_REPORT_ID_METER_STATE://状态
				str_to_int(data, measure->gass_meter.meter_state);
				break;
			case E_REPORT_ID_CUR_CUMULA_FLOW://当前累计流量
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->gass_meter.cur_cumula_flow = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			case E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA://结算日累计流量
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->gass_meter.cumula_flow_settle_data = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			default:
				break;
			}
			break;
		}
	case E_OP:
		{ 
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->e_op.e_op_time, gHexType);
				break;
			case E_REPORT_ID_E_OP_FAULT: //e_op_fault
				str_to_int(data, measure->e_op.e_op_fault, gHexType);
				break;
			case E_REPORT_ID_ON_OFF_STATE: //on_off_state
				str_to_int(data, measure->e_op.on_off_state, gHexType);
				break;
			case E_REPORT_ID_ON_COUNT: //on_count
				str_to_int(data, measure->e_op.on_count, gHexType);
				break;
			case E_REPORT_ID_ON_TIMER: //on_timer
				str_to_int(data, measure->e_op.e_op_time, gHexType);
				break;
			case E_REPORT_ID_ON_THINK_TIME: //on_think_time
				str_to_int(data, measure->e_op.on_think_time, gHexType);
				break;
			default : 
				break;
			}
			break;
		}
	case HM:
		{ 
			switch(param_id)
			{
			case E_REPORT_ID_METER_TIME:
				str_to_int(data, measure->heat_meter.heat_time, gHexType);
				break;
			case E_REPORT_ID_METER_STATE: //heat_state;//状态
				str_to_int(data, measure->heat_meter.heat_state, gHexType);
				break;
			case E_REPORT_ID_HEAT_SETTLE_DATA://结算日热量    
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->heat_meter.heat_settle_data = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			case E_REPORT_ID_CUR_HEAT://当前热量 
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->heat_meter.cur_heat = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			case E_REPORT_ID_THERMAL_POWER://热功率 
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->heat_meter.thermal_power = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			case E_REPORT_ID_FLOW://流量 
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->heat_meter.flow = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);

				break;
			case E_REPORT_ID_CUMULA_FLOW://累计流量
				str_to_int(data, lval);
				//修改单位
				flag = (lval >> (4*8) ) & 0xFF;
				measure->heat_meter.cumula_flow = lval & 0xFFFFFFFF;
				unit_name = ParseUnitByValue(flag);
				ModifyReportParamUnitName(unit_name, param_id);
				break;
			case E_REPORT_ID_SUPPLYWATER_TEMP://供水温度
				str_to_int(data, measure->heat_meter.supplywater_temp, gHexType);
				break;
			case E_REPORT_ID_BACKWATER_TEMP://回水温度
				str_to_int(data, measure->heat_meter.backwater_temp, gHexType);
				break;
			case E_REPORT_ID_CUMULA_TIME://累计工作时间
				str_to_int(data, measure->heat_meter.cumula_time, gHexType);
				break;
			default : 
				break;
			}
			break;
		}
	default:
		{
			break;    
		}
	}
}
