#include "StdAfx.h"
#include "BsTemprateAnalyse.h"
#include "BsStructAnalyse.h"
#include "DataBase/CmDBMySql.h"
#include "Mvb/ServerTrans.h"
#include "BsMvb.h"
#include "BsDefClsFrameInf.h"
#include "Log/CmLog.h"
#include "CmFunction.h"
#include "BsDefClsFrameInf.h"
#include "BsDefStruct.h"
#include "BsGlobal.h"
#include "BsFunction.h"
#include "CamPicture/CameraObject.h"
#include "CmOpenCvDll.h"
#include "PantographMonitor.h"
#include "PantographMonitorDlg.h"
#include "BsTimeStamp.h"

CTemprateAnalyse::CTemprateAnalyse(void)
{
	m_pCStructAnalyse = NULL;
}


CTemprateAnalyse::~CTemprateAnalyse(void)
{
}

int CTemprateAnalyse::AnalyzeFrameTemp( NSP_STP_CM::CMySqlDB* p_CMySqlDB, CFrameInf* p_frameCBInf )
{
	char sql_cmd[512];
	int ret = 0;

	MVB::tag_st_mvb_recv_data* p_st_mvb_data = m_pCMvb->GetMvbRecvData();
	const int station_ind = m_pCMvb->GetCurrentStationInd();
	MVB::tag_st_station_info* p_station_now = m_pCMvb->GetCurrentStationInf();

	const double temp_high = p_frameCBInf->m_tempHigh;
	const int left_up_x = p_frameCBInf->m_tempHignLeftUpX;
	const int left_up_y = p_frameCBInf->m_tempHignLeftUpY;

	st_rt_data rt_data;
	rt_data.CarriageId = p_st_mvb_data->train_num;
	rt_data.CarriageNum = m_carriageNum;
	rt_data.StationInd = station_ind;
	rt_data.LineId = m_lineId;
	rt_data.Direction = p_st_mvb_data->direction;
	rt_data.DataType = E_RT_TYPE_TEMP;
	char rt_val_param[128];
	sprintf(rt_val_param, "%d;%d", left_up_x, left_up_y);
	char rt_data_msg[128];
	sprintf(rt_data_msg, "%.03f", temp_high);
	rt_data.RtData = rt_data_msg;
	rt_data.BeginTime = CTimeStamp::GetInstance()->parseTime(CTimeStamp::GetInstance()->GetTimeStampMs());

	{
		NSP_STP_CM::tag_st_time cur_time;
		CTimeStamp::GetInstance()->parseTime(&cur_time, CTimeStamp::GetInstance()->GetTimeStampMs());
		rt_data.BeginTimeMs = cur_time.ms;
	}
	//插入数据
	sprintf(sql_cmd, "insert into  %s  (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s) \
					 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %d, '%s', '%s');",
					 gTblRt.c_str(), 
					 gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(), gRtFLineId.c_str(), 
					 gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), gRtFBeginTime.c_str(), 
					 gRtFBeginTimeMs.c_str(), gRtFDataParam.c_str(), gRtFStationName.c_str(),
					 rt_data.CarriageId, rt_data.CarriageNum, rt_data.StationInd , rt_data.LineId, 
					 rt_data.Direction, rt_data.DataType, rt_data.RtData.c_str(), rt_data.BeginTime.c_str(),
					 rt_data.BeginTimeMs, rt_val_param, p_station_now->station_alias
					 );
	int res_sql = p_CMySqlDB->WriteDataBase(sql_cmd);
	if (res_sql != 0)
	{
		gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, res_sql);
	}

	if (temp_high > m_tempThresholdCls1)
	{
		ret = 1;
		p_frameCBInf->m_pAnalyzeResArr->resNum = 1;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_type = E_FRAME_WARN_TYPE_PANTEM;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_cls = E_FRAME_WARN_CLS_1; 
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_msg = "弓网温度1级报警";
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_val = temp_high;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_val_param = rt_val_param;
	}
	else if (temp_high > m_tempThresholdCls2)
	{
		ret = 1;
		p_frameCBInf->m_pAnalyzeResArr->resNum = 1;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_type = E_FRAME_WARN_TYPE_PANTEM;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_cls = E_FRAME_WARN_CLS_2; 
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_msg = "弓网温度2级报警";
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_val = temp_high;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_val_param = rt_val_param;
	}
	else if (temp_high > m_tempThresholdCls3)
	{
		ret = 1;
		p_frameCBInf->m_pAnalyzeResArr->resNum = 1;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_type = E_FRAME_WARN_TYPE_PANTEM;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_cls = E_FRAME_WARN_CLS_3; 
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_msg = "弓网温度3级报警";
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_val = temp_high;
		p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_val_param = rt_val_param;
	}
	return ret;
}

void CTemprateAnalyse::SetCameraOpt()
{
	int iRet = 0;
	unsigned int fps = (unsigned int)((dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo))->fps);
	unsigned int uMicoSeconds = (unsigned int)((float)1 / fps * 1000 * 1000);

	//iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_FRAME_CYCLE, 
	//	"%u\r\n", uMicoSeconds);
}

void CTemprateAnalyse::SetTempThreshold( double temp_threshold_cls1, double temp_threshold_cls2, double temp_threshold_cls3 )
{
	m_tempThresholdCls1 = temp_threshold_cls1;
	m_tempThresholdCls2 = temp_threshold_cls2;
	m_tempThresholdCls3 = temp_threshold_cls3;
}

int CTemprateAnalyse::GetTemp( CFrameInf* p_frameCBInf )
{
	int ret = 0;

	//get temp
	const unsigned int width = ((Cam::tagDeviceFormatInfo*)m_pAbstractFormatInfo)->width;
	const unsigned int height = ((Cam::tagDeviceFormatInfo*)m_pAbstractFormatInfo)->height;
	int info[5] = {0};
	CThermoGroupDeviceEx* pDevice =  dynamic_cast<CThermoGroupDeviceEx*>(m_pCObjectDevice);
	const UINT x0 = 0;
	const UINT y0 = 0;
	const UINT x1 = width - 1;
	const UINT y1 = height - 1;
	bool is_get_temp = pDevice->getRectTemperatureInfo(x0, y0, x1, y1, info); 
	if (!is_get_temp)		
	{
		return -1;
	}
	const double temp_low = info[0] * 0.001f;
	const double temp_high = info[1] * 0.001f;
	const double temp_avarage = info[2] * 0.001f;
	//char msg[64];
	//sprintf(msg, "temp_low=%.1f, temp_high=%.1f, temp_avarage=%.1f\n", temp_low, temp_high, temp_avarage);
	//TRACE(msg);

	//modify jpg
	const int temp_pos_low = info[3];
	const int temp_pos_hign = info[4];
	const int temp_pos_hign_x = temp_pos_hign % width;
	const int temp_pos_hign_y = height - 1 - temp_pos_hign / width;//红外坐标原点是左下角,转换为opencv中左上角的坐标系
	const int left_up_x = temp_pos_hign_x;
	const int left_up_y = temp_pos_hign_y;
	if (0)
	{
		ret = NSP_STP_CM::ImgRectAdd(p_frameCBInf->m_filePath, left_up_x, left_up_y, width, height);

		char str_temp_high[64];
		sprintf(str_temp_high, "%.1f", temp_high);
		ret = NSP_STP_CM::ImgTextAdd(p_frameCBInf->m_filePath, left_up_x, left_up_y, width, height, str_temp_high);
	}

	char show_msg[64];
	sprintf(show_msg, "温度值=%.3f", temp_high);
	CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
	if(p_CPantographMonitorDlg)
	{
		p_CPantographMonitorDlg->AddTemp(show_msg);
	}

	p_frameCBInf->m_tempLow = temp_low;
	p_frameCBInf->m_tempHigh = temp_high;
	p_frameCBInf->m_tempAvarage = temp_avarage;
	p_frameCBInf->m_tempHignLeftUpX = left_up_x;
	p_frameCBInf->m_tempHignLeftUpY = left_up_y;

	return 0;
}

