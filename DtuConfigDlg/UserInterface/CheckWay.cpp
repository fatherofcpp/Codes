// CheckWay.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "CheckWay.h"
#include "dataConvert.h"
#include "CommonFunction.h"
#include "xmlAnalyze.h"

// CCheckWay 对话框

IMPLEMENT_DYNAMIC(CShowDevAllWayData, CDialog)

	CShowDevAllWayData::CShowDevAllWayData(CWnd* pParent /*=NULL*/)
	: CDialog(CShowDevAllWayData::IDD, pParent)
{
	devAllWayData.clear();
	devType = AM_3PHASE;
	pReportFormat = new st_report_format;
	memset(pReportFormat, 0, sizeof(*pReportFormat));
}

CShowDevAllWayData::~CShowDevAllWayData()
{
	delete pReportFormat;
}

void CShowDevAllWayData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHECK, mList);
}


BEGIN_MESSAGE_MAP(CShowDevAllWayData, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_VAILID_DATA, &CShowDevAllWayData::OnBnClickedButtonVailidData)
	ON_BN_CLICKED(IDC_BUTTON_INVALID_DATA, &CShowDevAllWayData::OnBnClickedButtonInvalidData)
END_MESSAGE_MAP()

void CShowDevAllWayData::SetDevAllWayData(int dev_type, VEC_DEV_ALL_WAY_DATA_INF & way_check_inf, st_report_format* report_format)
{
	devAllWayData = way_check_inf;
	devType = (e_dev_type)dev_type;

	memcpy(pReportFormat, report_format, sizeof(*pReportFormat));
}

// CCheckWay 消息处理程序

BOOL CShowDevAllWayData::OnInitDialog()
{
	CDialog::OnInitDialog();
	CreateList();

	int n_row_ind = 1;//行号
	int n_row = 1;
	int ret = 0;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));
	char item[64];
	memset(item, 0, sizeof(item));
	CString str = "";

	if (devAllWayData.size() == 0)
	{
		return TRUE;
	}

	int report_ind = GetReportIndByDevType(devType,pReportFormat);
	if (report_ind < 0)
	{
		return TRUE;
	}

	switch(devType)
	{
	case AM_3PHASE:
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->am_3phase.meter_time);
				case E_REPORT_ID_METER_STATE: 
					str = AnalyzeMeterState(measure->am_3phase.meter_state);
					sprintf(item, "%04x(%s)", measure->am_3phase.meter_state,str.GetString());
					break;
				case E_REPORT_ID_THRESHOLD_STATE:
					str = AnalyzeThresholdState(measure->am_3phase.threshold_state);
					sprintf(item, "%04x(%s)", measure->am_3phase.threshold_state,str.GetString());
					break;
				case E_REPORT_ID_VOLT_CHANGE: 
					sprintf(item, "%d", measure->am_3phase.volt_change);
					break;
				case E_REPORT_ID_CURR_CHANGE: 
					sprintf(item, "%d", measure->am_3phase.current_change);
					break;
				case E_REPORT_ID_FREQUENCY: 
					sprintf(item, "%.2f", measure->am_3phase.frequency * gTwoUnit);
					break;
				case E_REPORT_ID_A_VOLT   : 
					sprintf(item, "%.1f", measure->am_3phase.phase_volt[0] * gOneUnit);
					break;
				case E_REPORT_ID_B_VOLT   : 
					sprintf(item, "%.1f", measure->am_3phase.phase_volt[1] * gOneUnit);
					break;
				case E_REPORT_ID_C_VOLT   : 
					sprintf(item, "%.1f", measure->am_3phase.phase_volt[2] * gOneUnit);
					break;
				case E_REPORT_ID_AB_VOLT  :
					sprintf(item, "%.1f", measure->am_3phase.line_volt[0] * gOneUnit);
					break;
				case E_REPORT_ID_BC_VOLT  : 
					sprintf(item, "%.1f", measure->am_3phase.line_volt[1] * gOneUnit);
					break;
				case E_REPORT_ID_CA_VOLT  :
					sprintf(item, "%.1f", measure->am_3phase.line_volt[2] * gOneUnit);
					break;
				case E_REPORT_ID_A_CURRENT:
					sprintf(item, "%.2f", measure->am_3phase.phrase_current[0] * gTwoUnit);
					break;
				case E_REPORT_ID_B_CURRENT:
					sprintf(item, "%.2f", measure->am_3phase.phrase_current[1] * gTwoUnit);
					break;
				case E_REPORT_ID_C_CURRENT: 
					sprintf(item, "%.2f", measure->am_3phase.phrase_current[2] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE   : 
					sprintf(item, "%.2f", measure->am_3phase.phrase_active[0] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE_A : 
					sprintf(item, "%.2f", measure->am_3phase.phrase_active[1] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE_B : 
					sprintf(item, "%.2f", measure->am_3phase.phrase_active[2] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE_C : 
					sprintf(item, "%.2f", measure->am_3phase.phrase_active[3] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE  :
					sprintf(item, "%.2f", measure->am_3phase.phrase_reactive[0] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE_A:
					sprintf(item, "%.2f", measure->am_3phase.phrase_reactive[1] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE_B:
					sprintf(item, "%.2f", measure->am_3phase.phrase_reactive[2] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE_C:
					sprintf(item, "%.2f", measure->am_3phase.phrase_reactive[3] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT  :
					sprintf(item, "%.2f", measure->am_3phase.phrase_apparent[0] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT_A:
					sprintf(item, "%.2f", measure->am_3phase.phrase_apparent[1] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT_B:
					sprintf(item, "%.2f", measure->am_3phase.phrase_apparent[2] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT_C:
					sprintf(item, "%.2f", measure->am_3phase.phrase_apparent[3] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR   : 
					sprintf(item, "%.2f", measure->am_3phase.factor[0] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR_A : 
					sprintf(item, "%.2f", measure->am_3phase.factor[1] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR_B : 
					sprintf(item, "%.2f", measure->am_3phase.factor[2] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR_C : 
					sprintf(item, "%.2f", measure->am_3phase.factor[3] * gTwoUnit);
					break;
				case E_REPORT_ID_N_CURRENT: 
					sprintf(item, "%.2f", measure->am_3phase.n_current * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE  : 
					sprintf(item, "%.2f", measure->am_3phase.combine_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE1:
					sprintf(item, "%.2f", measure->am_3phase.combine_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE2:
					sprintf(item, "%.2f", measure->am_3phase.combine_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE3:
					sprintf(item, "%.2f", measure->am_3phase.combine_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE4:
					sprintf(item, "%.2f", measure->am_3phase.combine_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE : 
					sprintf(item, "%.2f", measure->am_3phase.positive_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE1:
					sprintf(item, "%.2f", measure->am_3phase.positive_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE2:
					sprintf(item, "%.2f", measure->am_3phase.positive_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE3:
					sprintf(item, "%.2f", measure->am_3phase.positive_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE4:
					sprintf(item, "%.2f", measure->am_3phase.positive_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE: 
					sprintf(item, "%.2f", measure->am_3phase.reverse_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE1:
					sprintf(item, "%.2f", measure->am_3phase.reverse_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE2:
					sprintf(item, "%.2f", measure->am_3phase.reverse_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE3:
					sprintf(item, "%.2f", measure->am_3phase.reverse_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE4:
					sprintf(item, "%.2f", measure->am_3phase.reverse_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_COMBINE1 :
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[0][0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE1:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[0][1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE2:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[0][2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE3:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[0][3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE4:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[0][4] * gTwoUnit);
					break;
				case E_REPORT_ID_COMBINE2 : 
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[1][0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE1:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[1][1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE2:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[1][2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE3:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[1][3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE4:
					sprintf(item, "%.2f", measure->am_3phase.combine_reactive_energy[1][4] * gTwoUnit);
					break;
				default :
					is_patch = false;
					break;
				}
				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;

	case AM_1PHASE:  
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->am_1phase.meter_time);
				case E_REPORT_ID_METER_STATE: 
					str = AnalyzeMeterState(measure->am_1phase.meter_state);
					sprintf(item, "%04x(%s)", measure->am_1phase.meter_state,str.GetString());
					break;
				case E_REPORT_ID_THRESHOLD_STATE:
					str = AnalyzeThresholdState(measure->am_1phase.threshold_state);
					sprintf(item, "%04x(%s)", measure->am_1phase.threshold_state,str.GetString());
					break;
				case E_REPORT_ID_VOLT_CHANGE: 
					sprintf(item, "%d", measure->am_1phase.volt_change);
					break;
				case E_REPORT_ID_CURR_CHANGE: 
					sprintf(item, "%d", measure->am_1phase.current_change);
					break;
				case E_REPORT_ID_FREQUENCY  : 
					sprintf(item, "%.2f", measure->am_1phase.frequency * gTwoUnit);
					break;
				case E_REPORT_ID_VOLT : 
					sprintf(item, "%.1f", measure->am_1phase.volt * gOneUnit);
					break;
				case E_REPORT_ID_CURRENT : 
					sprintf(item, "%.2f", measure->am_1phase.current * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE  : 
					sprintf(item, "%.2f", measure->am_1phase.active * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE: 
					sprintf(item, "%.2f", measure->am_1phase.reactive * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT: 
					sprintf(item, "%.2f", measure->am_1phase.apparent * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR  : 
					sprintf(item, "%.2f", measure->am_1phase.factor * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE : 
					sprintf(item, "%.2f", measure->am_1phase.combine_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE1:
					sprintf(item, "%.2f", measure->am_1phase.combine_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE2:
					sprintf(item, "%.2f", measure->am_1phase.combine_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE3:
					sprintf(item, "%.2f", measure->am_1phase.combine_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE4:
					sprintf(item, "%.2f", measure->am_1phase.combine_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE: 
					sprintf(item, "%.2f", measure->am_1phase.positive_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE1:
					sprintf(item, "%.2f", measure->am_1phase.positive_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE2:
					sprintf(item, "%.2f", measure->am_1phase.positive_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE3:
					sprintf(item, "%.2f", measure->am_1phase.positive_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE4:
					sprintf(item, "%.2f", measure->am_1phase.positive_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE : 
					sprintf(item, "%.2f", measure->am_1phase.reverse_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE1:
					sprintf(item, "%.2f", measure->am_1phase.reverse_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE2:
					sprintf(item, "%.2f", measure->am_1phase.reverse_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE3:
					sprintf(item, "%.2f", measure->am_1phase.reverse_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE4:
					sprintf(item, "%.2f", measure->am_1phase.reverse_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_COMBINE1: 
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[0][0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE1:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[0][1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE2:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[0][2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE3:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[0][3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE4:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[0][4] * gTwoUnit);
					break;
				case E_REPORT_ID_COMBINE2: 
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[1][0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE1:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[1][1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE2:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[1][2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE3:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[1][3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE4:
					sprintf(item, "%.2f", measure->am_1phase.combine_reactive_energy[1][4] * gTwoUnit);
					break;
				default :  
					is_patch = false;
					break;
				}
				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;

	case AM_DC:   
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->am_dc.meter_time);
				case E_REPORT_ID_METER_STATE: 
					str = AnalyzeMeterState(measure->am_dc.meter_state);
					sprintf(item, "%04x(%s)", measure->am_dc.meter_state,str.GetString());
					break;
				case E_REPORT_ID_VOLT_CHANGE: 
					sprintf(item, "%d", measure->am_dc.volt_change);
					break;
				case E_REPORT_ID_CURR_CHANGE: 
					sprintf(item, "%d", measure->am_dc.current_change);
					break;
				case E_REPORT_ID_VOLT : 
					sprintf(item, "%.1f", measure->am_dc.volt * gOneUnit);
					break;
				case E_REPORT_ID_CURRENT : 
					sprintf(item, "%.2f", measure->am_dc.current * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE  : 
					sprintf(item, "%.2f", measure->am_dc.active * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE : 
					sprintf(item, "%.2f", measure->am_dc.energy * gTwoUnit);
					break;
				default : 
					is_patch = false;
					break;
				}
				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;

	case AM_HARM:
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->am_harm.meter_time);
				case E_REPORT_ID_METER_STATE: 
					str = AnalyzeMeterState(measure->am_harm.meter_state);
					sprintf(item, "%04x(%s)", measure->am_harm.meter_state,str.GetString());
					break;
				case E_REPORT_ID_THRESHOLD_STATE:
					str = AnalyzeThresholdState(measure->am_harm.threshold_state);
					sprintf(item, "%04x(%s)", measure->am_harm.threshold_state,str.GetString());
					break;
				case E_REPORT_ID_VOLT_CHANGE: 
					sprintf(item, "%d", measure->am_harm.volt_change);
					break;
				case E_REPORT_ID_CURR_CHANGE: 
					sprintf(item, "%d", measure->am_harm.current_change);
					break;
				case E_REPORT_ID_FREQUENCY: 
					sprintf(item, "%.2f", measure->am_harm.frequency * gTwoUnit);
					break;
				case E_REPORT_ID_A_VOLT   : 
					sprintf(item, "%.1f", measure->am_harm.phase_volt[0] * gOneUnit);
					break;
				case E_REPORT_ID_B_VOLT   : 
					sprintf(item, "%.1f", measure->am_harm.phase_volt[1] * gOneUnit);
					break;
				case E_REPORT_ID_C_VOLT   : 
					sprintf(item, "%.1f", measure->am_harm.phase_volt[2] * gOneUnit);
					break;
				case E_REPORT_ID_AB_VOLT  : 
					sprintf(item, "%.1f", measure->am_harm.line_volt[0] * gOneUnit);
					break;
				case E_REPORT_ID_BC_VOLT  : 
					sprintf(item, "%.1f", measure->am_harm.line_volt[1] * gOneUnit);
					break;
				case E_REPORT_ID_CA_VOLT  : 
					sprintf(item, "%.1f", measure->am_harm.line_volt[2] * gOneUnit);
					break;
				case E_REPORT_ID_A_CURRENT: 
					sprintf(item, "%.2f", measure->am_harm.phrase_current[0] * gTwoUnit);
					break;
				case E_REPORT_ID_B_CURRENT: 
					sprintf(item, "%.2f", measure->am_harm.phrase_current[1] * gTwoUnit);
					break;
				case E_REPORT_ID_C_CURRENT: 
					sprintf(item, "%.2f", measure->am_harm.phrase_current[2] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE   : 
					sprintf(item, "%.2f", measure->am_harm.phrase_active[0] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE_A : 
					sprintf(item, "%.2f", measure->am_harm.phrase_active[1] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE_B : 
					sprintf(item, "%.2f", measure->am_harm.phrase_active[2] * gTwoUnit);
					break;
				case E_REPORT_ID_ACTIVE_C : 
					sprintf(item, "%.2f", measure->am_harm.phrase_active[3] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE  :
					sprintf(item, "%.2f", measure->am_harm.phrase_reactive[0] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE_A:
					sprintf(item, "%.2f", measure->am_harm.phrase_reactive[1] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE_B:
					sprintf(item, "%.2f", measure->am_harm.phrase_reactive[2] * gTwoUnit);
					break;
				case E_REPORT_ID_REACTIVE_C:
					sprintf(item, "%.2f", measure->am_harm.phrase_reactive[3] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT  :
					sprintf(item, "%.2f", measure->am_harm.phrase_apparent[0] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT_A:
					sprintf(item, "%.2f", measure->am_harm.phrase_apparent[1] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT_B:
					sprintf(item, "%.2f", measure->am_harm.phrase_apparent[2] * gTwoUnit);
					break;
				case E_REPORT_ID_APPARENT_C:
					sprintf(item, "%.2f", measure->am_harm.phrase_apparent[3] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR   : 
					sprintf(item, "%.2f", measure->am_harm.factor[0] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR_A : 
					sprintf(item, "%.2f", measure->am_harm.factor[1] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR_B : 
					sprintf(item, "%.2f", measure->am_harm.factor[2] * gTwoUnit);
					break;
				case E_REPORT_ID_FACTOR_C : 
					sprintf(item, "%.2f", measure->am_harm.factor[3] * gTwoUnit);
					break;
				case E_REPORT_ID_N_CURRENT: 
					sprintf(item, "%.2f", measure->am_harm.n_current * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE  : 
					sprintf(item, "%.2f", measure->am_harm.combine_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE1:
					sprintf(item, "%.2f", measure->am_harm.combine_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE2:
					sprintf(item, "%.2f", measure->am_harm.combine_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE3:
					sprintf(item, "%.2f", measure->am_harm.combine_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE_RATE4:
					sprintf(item, "%.2f", measure->am_harm.combine_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE : 
					sprintf(item, "%.2f", measure->am_harm.positive_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE1:
					sprintf(item, "%.2f", measure->am_harm.positive_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE2:
					sprintf(item, "%.2f", measure->am_harm.positive_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE3:
					sprintf(item, "%.2f", measure->am_harm.positive_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_POSITIVE_RATE4:
					sprintf(item, "%.2f", measure->am_harm.positive_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE  : 
					sprintf(item, "%.2f", measure->am_harm.reverse_active_energy[0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE1:
					sprintf(item, "%.2f", measure->am_harm.reverse_active_energy[1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE2:
					sprintf(item, "%.2f", measure->am_harm.reverse_active_energy[2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE3:
					sprintf(item, "%.2f", measure->am_harm.reverse_active_energy[3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_REVERSE_RATE4:
					sprintf(item, "%.2f", measure->am_harm.reverse_active_energy[4] * gTwoUnit);
					break;
				case E_REPORT_ID_COMBINE1 : 
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[0][0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE1:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[0][1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE2:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[0][2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE3:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[0][3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE1_RATE4:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[0][4] * gTwoUnit);
					break;
				case E_REPORT_ID_COMBINE2 : 
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[1][0] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE1:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[1][1] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE2:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[1][2] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE3:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[1][3] * gTwoUnit);
					break;
				case E_REPORT_ID_T_COMBINE2_RATE4:
					sprintf(item, "%.2f", measure->am_harm.combine_reactive_energy[1][4] * gTwoUnit);
					break;
				case E_REPORT_ID_ORIGINAL_VOLTAGE_A : 
					sprintf(item, "%.1f", measure->am_harm.phrase_original_volt[0] * gOneUnit);
					break;
				case E_REPORT_ID_ORIGINAL_VOLTAGE_B : 
					sprintf(item, "%.1f", measure->am_harm.phrase_original_volt[1] * gOneUnit);
					break;
				case E_REPORT_ID_ORIGINAL_VOLTAGE_C : 
					sprintf(item, "%.1f", measure->am_harm.phrase_original_volt[2] * gOneUnit);
					break;
				case E_REPORT_ID_ORIGINAL_CURRENT_A : 
					sprintf(item, "%.2f", measure->am_harm.phrase_original_current[0] * gTwoUnit);
					break;
				case E_REPORT_ID_ORIGINAL_CURRENT_B : 
					sprintf(item, "%.2f", measure->am_harm.phrase_original_current[1] * gTwoUnit);
					break;
				case E_REPORT_ID_ORIGINAL_CURRENT_C : 
					sprintf(item, "%.2f", measure->am_harm.phrase_original_current[2] * gTwoUnit);
					break;
				case E_REPORT_ID_ORIGINAL_ACTIVE_T : 
					sprintf(item, "%.2f", measure->am_harm.phrase_original_active[0] * gTwoUnit);
					break;
				case E_REPORT_ID_ORIGINAL_ACTIVE_A : 
					sprintf(item, "%.2f", measure->am_harm.phrase_original_active[1] * gTwoUnit);
					break;
				case E_REPORT_ID_ORIGINAL_ACTIVE_B : 
					sprintf(item, "%.2f", measure->am_harm.phrase_original_active[2] * gTwoUnit);
					break;
				case E_REPORT_ID_ORIGINAL_ACTIVE_C : 
					sprintf(item, "%.2f", measure->am_harm.phrase_original_active[3] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_VOLTAGE_A: 
					sprintf(item, "%.1f", measure->am_harm.phrase_harm_volt[0] * gOneUnit);
					break;
				case E_REPORT_ID_HARMONIC_VOLTAGE_B: 
					sprintf(item, "%.1f", measure->am_harm.phrase_harm_volt[1] * gOneUnit);
					break;
				case E_REPORT_ID_HARMONIC_VOLTAGE_C: 
					sprintf(item, "%.1f", measure->am_harm.phrase_harm_volt[2] * gOneUnit);
					break;
				case E_REPORT_ID_HARMONIC_CURRENT_A: 
					sprintf(item, "%.2f", measure->am_harm.phrase_harm_current[0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_CURRENT_B: 
					sprintf(item, "%.2f", measure->am_harm.phrase_harm_current[1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_CURRENT_C: 
					sprintf(item, "%.2f", measure->am_harm.phrase_harm_current[2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_ACTIVE_T : 
					sprintf(item, "%.2f", measure->am_harm.phrase_harm_active[0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_ACTIVE_A : 
					sprintf(item, "%.2f", measure->am_harm.phrase_harm_active[1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_ACTIVE_B : 
					sprintf(item, "%.2f", measure->am_harm.phrase_harm_active[2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_ACTIVE_C : 
					sprintf(item, "%.2f", measure->am_harm.phrase_harm_active[3] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_UA : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[0] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_UB : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[1] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_UC : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[2] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_UT : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[3] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_IA : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[4] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_IB : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[5] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_IC : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[6] * gTwoUnit);
					break;
				case E_REPORT_ID_THD_IT : 
					sprintf(item, "%.2f", measure->am_harm.total_distor[7] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_2  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_3  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_4  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_5  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][3] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_6  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][4] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_7  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][5] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_8  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][6] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_9  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][7] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_10 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][8] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_11 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][9] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_12 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][10] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_13 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][11] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_14 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][12] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_15 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][13] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_16 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][14] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_17 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][15] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_18 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][16] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_19 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][17] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_20 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][18] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_21 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[0][19] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_ODD:  
					sprintf(item, "%.2f", measure->am_harm.harm_u_parity[0][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_EVEN: 
					sprintf(item, "%.2f", measure->am_harm.harm_u_parity[0][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_RIDGE:
					sprintf(item, "%.3f", measure->am_harm.harm_u_ridge[0] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_UA_TETL: 
					sprintf(item, "%.3f", measure->am_harm.harm_u_tele[0] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_2  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_3  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_4  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_5  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][3] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_6  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][4] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_7  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][5] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_8  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][6] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_9  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][7] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_10 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][8] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_11 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][9] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_12 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][10] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_13 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][11] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_14 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][12] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_15 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][13] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_16 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][14] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_17 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][15] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_18 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][16] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_19 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][17] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_20 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][18] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_21 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[1][19] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_ODD:  
					sprintf(item, "%.2f", measure->am_harm.harm_u_parity[1][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_EVEN: 
					sprintf(item, "%.2f", measure->am_harm.harm_u_parity[1][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_RIDGE:
					sprintf(item, "%.3f", measure->am_harm.harm_u_ridge[1] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_UB_TETL: 
					sprintf(item, "%.3f", measure->am_harm.harm_u_tele[1] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_2  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_3  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_4  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_5  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][3] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_6  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][4] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_7  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][5] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_8  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][6] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_9  : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][7] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_10 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][8] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_11 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][9] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_12 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][10] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_13 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][11] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_14 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][12] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_15 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][13] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_16 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][14] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_17 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][15] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_18 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][16] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_19 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][17] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_20 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][18] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_21 : 
					sprintf(item, "%.2f", measure->am_harm.harm_u[2][19] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_ODD:  
					sprintf(item, "%.2f", measure->am_harm.harm_u_parity[2][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_EVEN: 
					sprintf(item, "%.2f", measure->am_harm.harm_u_parity[2][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_RIDGE:
					sprintf(item, "%.3f", measure->am_harm.harm_u_ridge[2] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_UC_TETL: 
					sprintf(item, "%.3f", measure->am_harm.harm_u_tele[2] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_2  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_3  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_4  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_5  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][3] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_6  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][4] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_7  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][5] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_8  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][6] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_9  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][7] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_10 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][8] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_11 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][9] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_12 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][10] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_13 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][11] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_14 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][12] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_15 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][13] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_16 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][14] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_17 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][15] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_18 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][16] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_19 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][17] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_20 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][18] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_21 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[0][19] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_ODD : 
					sprintf(item, "%.2f", measure->am_harm.harm_i_parity[0][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_EVEN: 
					sprintf(item, "%.2f", measure->am_harm.harm_i_parity[0][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IA_K   : 
					sprintf(item, "%.3f", measure->am_harm.harm_i_K[0] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_2  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_3  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_4  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_5  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][3] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_6  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][4] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_7  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][5] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_8  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][6] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_9  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][7] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_10 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][8] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_11 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][9] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_12 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][10] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_13 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][11] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_14 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][12] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_15 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][13] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_16 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][14] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_17 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][15] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_18 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][16] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_19 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][17] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_20 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][18] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_21 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[1][19] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_ODD : 
					sprintf(item, "%.2f", measure->am_harm.harm_i_parity[1][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_EVEN: 
					sprintf(item, "%.2f", measure->am_harm.harm_i_parity[1][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IB_K   : 
					sprintf(item, "%.3f", measure->am_harm.harm_i_K[1] * gThreeUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_2  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_3  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_4  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][2] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_5  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][3] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_6  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][4] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_7  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][5] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_8  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][6] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_9  : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][7] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_10 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][8] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_11 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][9] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_12 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][10] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_13 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][11] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_14 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][12] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_15 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][13] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_16 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][14] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_17 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][15] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_18 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][16] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_19 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][17] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_20 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][18] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_21 : 
					sprintf(item, "%.2f", measure->am_harm.harm_i[2][19] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_ODD : 
					sprintf(item, "%.2f", measure->am_harm.harm_i_parity[2][0] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_EVEN: 
					sprintf(item, "%.2f", measure->am_harm.harm_i_parity[2][1] * gTwoUnit);
					break;
				case E_REPORT_ID_HARMONIC_IC_K   : 
					sprintf(item, "%.3f", measure->am_harm.harm_i_K[2] * gThreeUnit);
					break;
				default : 
					is_patch = false;
					break;
				}

				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;

	case WM:   
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->water_meter.meter_time);
				case E_REPORT_ID_METER_STATE://state
					str = AnalyzeMeterState(measure->water_meter.meter_state);
					sprintf(item, "%04x(%s)", measure->water_meter.meter_state,str.GetString());
					break;
				case E_REPORT_ID_CUR_CUMULA_FLOW: //cur_cumula_flow
					sprintf(item, "%.2f", measure->water_meter.cur_cumula_flow * gTwoUnit);
					break;
				case E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA: //cumula_flow_settle_data 
					sprintf(item, "%.2f", measure->water_meter.cumula_flow_settle_data * gTwoUnit);
					break;
				default : 
					is_patch = false;
					break;
				}
				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;

	case GM:   
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->gass_meter.meter_time);
				case E_REPORT_ID_METER_STATE://state
					str = AnalyzeMeterState(measure->gass_meter.meter_state);
					sprintf(item, "%04x(%s)", measure->gass_meter.meter_state,str.GetString());
					break;
				case E_REPORT_ID_CUR_CUMULA_FLOW: //cur_cumula_flow
					sprintf(item, "%.2f", measure->gass_meter.cur_cumula_flow * gTwoUnit);
					break;
				case E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA: //cumula_flow_settle_data 
					sprintf(item, "%.2f", measure->gass_meter.cumula_flow_settle_data * gTwoUnit);
					break;
				default : 
					is_patch = false;
					break;
				}
				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;

	case E_OP:
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->e_op.e_op_time);
					break;
				case E_REPORT_ID_E_OP_FAULT: //e_op_fault
					//str = AnalyzeThresholdState(measure->e_op.e_op_fault);
					sprintf(item, "%04x", measure->e_op.e_op_fault);
					break;
				case E_REPORT_ID_ON_OFF_STATE: //on_off_state
					sprintf(item, "%d", measure->e_op.on_off_state);
					break;
				case E_REPORT_ID_ON_COUNT: //on_count
					sprintf(item, "%d", measure->e_op.on_count);
					break;
				case E_REPORT_ID_ON_TIMER: //on_timer
					sprintf(item, "%d", measure->e_op.on_timer);
					break;
				case E_REPORT_ID_ON_THINK_TIME: //on_think_time
					sprintf(item, "%d", measure->e_op.on_think_time);
					break;
				default :
					is_patch = false;
					break;
				}
				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;

	case HM:
		for(unsigned int i = 0; i < devAllWayData.size(); i++)
		{
			sprintf(item, "%d", n_row_ind);
			n_row = mList.InsertItem(n_row_ind++, item);//插入行

			int column_ind = 1;
			sprintf(item, "%d", devAllWayData[i].way_ind);
			mList.SetItemText(n_row, column_ind++, item);

			u_w_measure* measure = &devAllWayData[i].measure;
			for(int j = 0; j < pReportFormat->dev_type_report[report_ind].param_num; j++)
			{
				int param_id = pReportFormat->dev_type_report[report_ind].report_param_id[j];
				bool is_patch = true;//param_id匹配
				switch(param_id)
				{
				case E_REPORT_ID_METER_TIME:
					sprintf(item, "%lld", measure->heat_meter.heat_time);
					break;
				case E_REPORT_ID_METER_STATE: //heat_state;//状态
					str = AnalyzeMeterState(measure->heat_meter.heat_state);
					sprintf(item, "%04x(%s)", measure->heat_meter.heat_state,str.GetString());
					break;
				case E_REPORT_ID_HEAT_SETTLE_DATA: //heat_settle_data;//结算日热量     
					sprintf(item, "%.2f", measure->heat_meter.heat_settle_data * gTwoUnit);
					break;
				case E_REPORT_ID_CUR_HEAT: //cur_heat;//当前热量   
					sprintf(item, "%.2f", measure->heat_meter.cur_heat * gTwoUnit);
					break;
				case E_REPORT_ID_THERMAL_POWER: //thermal_power;//热功率    
					sprintf(item, "%.2f", measure->heat_meter.thermal_power * gTwoUnit);
					break;
				case E_REPORT_ID_FLOW: //flow;//流量 
					sprintf(item, "%.4f", measure->heat_meter.flow * gFourUnit);
					break;
				case E_REPORT_ID_CUMULA_FLOW: //cumula_flow;//累计流量 
					sprintf(item, "%.3f", measure->heat_meter.cumula_flow * gThreeUnit);
					break;
				case E_REPORT_ID_SUPPLYWATER_TEMP: //supplywater_temp;//供水温度 
					sprintf(item, "%.2f", measure->heat_meter.supplywater_temp * gTwoUnit);
					break;
				case E_REPORT_ID_BACKWATER_TEMP: //backwater_temp;//回水温度 
					sprintf(item, "%.2f", measure->heat_meter.backwater_temp * gTwoUnit);
					break;
				case E_REPORT_ID_CUMULA_TIME: //cumula_time;//累计工作时间 
					sprintf(item, "%d", measure->heat_meter.cumula_time);
					break;
				default :
					is_patch = false;
					break;
				}
				if (is_patch)
				{
					mList.SetItemText(n_row, column_ind++, item);
				}
			}
		}
		break;
	default:
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CShowDevAllWayData::CreateList()
{
	LONG l_style;
	l_style = GetWindowLong(mList.m_hWnd, GWL_STYLE);//获取当前窗口style
	l_style &= ~LVS_TYPEMASK; //清除显示方式位
	l_style |= LVS_REPORT; //设置style
	SetWindowLong(mList.m_hWnd, GWL_STYLE, l_style);//设置style

	DWORD dw_style = mList.GetExtendedStyle();
	dw_style |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dw_style |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	mList.SetExtendedStyle(dw_style); //设置扩展风格

	int n_index = 0;
	int com_width = 160;
	int width = com_width;
	if (devAllWayData.size() == 0)
	{
		return;
	}

	int report_ind = GetReportIndByDevType(devType,pReportFormat);
	if (report_ind < 0)
	{
		return;
	}

	switch(devType)
	{
	case AM_3PHASE:
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_METER_STATE)
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;

	case AM_1PHASE:            
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_METER_STATE)
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;

	case AM_DC:     	
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_METER_STATE)
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;

	case AM_HARM:
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_METER_STATE)
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;

	case WM:
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_METER_STATE)
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;

	case GM:
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_METER_STATE)
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;
	case E_OP:
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_E_OP_FAULT)//e_op_fault
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;
	case HM:
		mList.InsertColumn(n_index++, "", LVCFMT_LEFT, 20 );
		mList.InsertColumn(n_index++, "回路下标", LVCFMT_LEFT, width );
		for(int i = 0; i < pReportFormat->dev_type_report[report_ind].param_num; i++)
		{
			if (pReportFormat->dev_type_report[report_ind].report_param_id[i] == E_REPORT_ID_METER_STATE)//heat_state
			{
				width = com_width * 2;
			}
			else
			{
				width = com_width;
			}
			mList.InsertColumn(n_index++, 
				GetAllDevTypeReportParamNameByValue(
				pReportFormat->dev_type_report[report_ind].report_param_id[i]), LVCFMT_LEFT, width );
		}
		break;
	default:
		break;
	}

}

void CShowDevAllWayData::OnBnClickedButtonVailidData()
{
	CDialog::OnOK();
}


void CShowDevAllWayData::OnBnClickedButtonInvalidData()
{
	CDialog::OnCancel();
}

