#include "StdAfx.h"
#include "BsGeoParamAnalyse.h"
#include "BsStructAnalyse.h"
#include "BsDefClsFrameInf.h"
#include "CamPicture/CameraObject.h"
#include "Socket/CmTransUdp.h"
#include "Log/CmLog.h"

#include "BsMvb.h"
#include "Mvb/ServerTrans.h"
#include "CmFunction.h"
#include "BsDefClsFrameInf.h"
#include "DataBase/CmDBMySql.h"
#include "BsDefStruct.h"
#include "BsGlobal.h"
#include "BsFunction.h"
#include "PantographMonitor.h"
#include "PantographMonitorDlg.h"
#include "BsTimeStamp.h"

const static unsigned int g_max_pull_guid_count = 1 * 60 * 5;//保存1分钟的几何参数信息,最快1秒5条

CGeoParamAnalyse::CGeoParamAnalyse(void)
{
	m_pDbMysqlParam = NULL;
	m_pCMvb = NULL;
	m_pCStructAnalyse = NULL;
	m_carriageNum = 0;
	m_lineId = 0;
	m_udpGeoPort = 0;
	memset(m_handleBuf, 0, sizeof(m_handleBuf));
}


CGeoParamAnalyse::~CGeoParamAnalyse(void)
{
	ReleaseRes();
}

int CGeoParamAnalyse::TaskStart()
{
	SetThreadCount(3);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_analyse", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_analyse, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_geoparam_recv", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_geoparam_recv, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 2;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_geoparam_handle", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_geoparam_handle, p_st_thread_param, CpuMaskRemain());
	}
	return 0;
}

unsigned int WINAPI CGeoParamAnalyse::thread_analyse( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CGeoParamAnalyse* p_this = (CGeoParamAnalyse*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;

	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	int ret = 0;

	int pre_alarm_ms = ::GetTickCount();
	int alarm_delt_ms = 5 * 1000;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		tag_st_alarm* p_alarm = p_this->PickPullGuid();
		if (p_alarm != NULL)
		{
			bool is_analyze = false;

			int nw_ms = ::GetTickCount();
			if (nw_ms - pre_alarm_ms > alarm_delt_ms)
			{
				pre_alarm_ms = nw_ms;
				is_analyze = true;
			}
			else
			{
				is_analyze = false;
			}

			if (is_analyze)
			{
				unsigned int alarm_type = p_alarm->AlarmType;
				unsigned int alarm_cls = p_alarm->AlarmLevel;

				UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, false);

				std::string alarm_message = "";
				ret = FormatAlarmMessage(p_alarm->AlarmType, p_alarm->AlarmLevel, p_alarm->AlarmValue, alarm_message);
				if (ret == 0)
				{
					MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();
					MVB::tag_st_station_info* p_station_now = p_this->m_pCMvb->GetCurrentStationInf();
					st_db_alarm db_alarm;
					db_alarm.CameraPos = E_CAM_CLASS_TATOL + 1;
					db_alarm.CarriageId = p_st_mvb_data->train_num;
					db_alarm.CarriageNum = p_this->m_carriageNum;
					db_alarm.StationId = p_station_now->station_seq;
					db_alarm.LineId = p_this->m_lineId;
					db_alarm.Direction = p_st_mvb_data->direction;
					db_alarm.BeginTime = gDbTimeNow;
					double milage = (double)(p_st_mvb_data->mileage) * 0.01;
					db_alarm.AlarmType = p_alarm->AlarmType;
					db_alarm.AlarmLevel = p_alarm->AlarmLevel;
					db_alarm.AlarmMessage = alarm_message;
					db_alarm.AlarmValue = p_alarm->AlarmValue;
					std::string alarm_val_param = "";

					std::string dst_file_path =  "";
					//D:\pantoMonitorPath\pictureDownPath\20180320\1600\02\20180320_160051541_02_10_10019_05_JLLZ_0_0.000KM.jpg
					std::string file_path_structure = p_this->m_pCStructAnalyse->GetFramePath();
					if (!file_path_structure.empty())
					{
						NSP_STP_CM::tag_st_time cur_time;
						NSP_STP_CM::GetNowTime(&cur_time);
						int tm_year = cur_time.year;
						int tm_mon = cur_time.mon;
						int tm_mday = cur_time.day;
						int tm_hour = cur_time.hour;
						int tm_min = cur_time.min;
						int tm_sec = cur_time.sec;
						int tm_ms = cur_time.ms;
						char ymd[16];
						sprintf(ymd, "%04d%02d%02d", tm_year, tm_mon, tm_mday);
						char hmsm[16];
						sprintf(hmsm, "%02d%02d%02d%03d", tm_hour, tm_min, tm_sec, tm_ms);
						char date_msg[32];
						sprintf(date_msg, "%s_%s", ymd, hmsm);

						std::string suff = "";
						char _tmp_file_name[MAX_PATH] = {'\0'};
						char tmp_msg[MAX_PATH] = {'\0'};
						suff = p_this->m_saveFrameSuffix;

						int camera_Ind = E_CAM_CLASS_PULL_GUID;
						sprintf(_tmp_file_name, "%s_%02d_%02d_%04d_%02d_%s_%d_%.03fKM_%d_%02d",
							date_msg, camera_Ind, db_alarm.LineId, db_alarm.CarriageId, db_alarm.CarriageNum, 
							p_station_now->station_alias, db_alarm.Direction, p_st_mvb_data->mileage * 0.01, db_alarm.AlarmType, db_alarm.AlarmLevel);
						sprintf(tmp_msg, "%s\\%02d%02d\\%02d", ymd, tm_hour, tm_min / 10 * 10, camera_Ind);

						std::string folder_path = p_this->m_pictureRootPathAlarm + tmp_msg + "\\";
						std::string file_name = _tmp_file_name;

						dst_file_path = folder_path + file_name + suff;

						ret = NSP_STP_CM::copyFile(file_path_structure.c_str(), dst_file_path.c_str(), true);
						if (0 == ret)
						{
							db_alarm.FileName = NSP_STP_CM::str_left_cut_tag( dst_file_path, "\\");
							db_alarm.FullName = dst_file_path;
							//插入数据
							sprintf(sql_cmd, "insert into  %s  ( %s, %s,  %s, %s, %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s) \
											 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %s, %.03f, %d, %d, '%s', %.03f,  '%s',  '%s');",
											 gTblAlarm.c_str(), 
											 gAlarmFCameraPos.c_str(), gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), 
											 gAlarmFLineId.c_str(), gAlarmFDirection.c_str(), gAlarmFFileName.c_str(), gAlarmFFullName.c_str(), 
											 gAlarmFBeginTime.c_str(), gAlarmFMileage.c_str(), gAlarmFAlarmType.c_str(), gAlarmFAlarmLevel.c_str(),  
											 gAlarmFAlarmMessage.c_str(), gAlarmFAlarmValue.c_str(), gAlarmFAlarmValueParam.c_str(), gAlarmFStuctureFullName.c_str(),
											 db_alarm.CameraPos, db_alarm.CarriageId, db_alarm.CarriageNum, db_alarm.StationId , 
											 db_alarm.LineId, db_alarm.Direction, db_alarm.FileName.c_str(), (NSP_STP_CM::str_sig_double(db_alarm.FullName, '\\')).c_str(), 
											 db_alarm.BeginTime.c_str(), milage, db_alarm.AlarmType, db_alarm.AlarmLevel, 
											 db_alarm.AlarmMessage.c_str(), db_alarm.AlarmValue, alarm_val_param.c_str(), (NSP_STP_CM::str_sig_double(file_path_structure, '\\')).c_str()          
											 );
							ret = p_CMySqlDB->WriteDataBase(sql_cmd);
							if (ret != 0)
							{
								gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
							}
						}
					}
				}
				else
				{
					gLog.Add("%s FormatAlarmMessage fail,ret=%d\n", __FUNCTION__, ret);
				}
			}
			delete p_alarm;
		}
		else
		{


		}
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

void CGeoParamAnalyse::ReleaseRes()
{
	for (unsigned int i = 0; i < m_pullGuid.size(); i++ )
	{
		tag_st_alarm* p_alarm = m_pullGuid[i];
		delete p_alarm;
	}
	m_pullGuid.clear();
}

unsigned int WINAPI CGeoParamAnalyse::thread_geoparam_recv( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CGeoParamAnalyse* p_this = (CGeoParamAnalyse*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 5;
	const int no_data_delt_tm = p_this->m_geoTimeoutS;
	int ret = 0;

	NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
	p_CUdp->SetLocalPort(p_this->m_udpGeoPort);
	timeval r_time_out;
	r_time_out.tv_sec = 0;
	r_time_out.tv_usec = 5 * 1000;
	p_CUdp->SetReadTimeOut(r_time_out);
	ret = p_CUdp->BindPort();
	if (ret != 0)
	{
		gLog.Add("%s p_CUdp->BindPort fail\n", __FUNCTION__);
		delete p_CUdp;
		return 0;
	}

	char rcv_buf[256];
	memset(rcv_buf, 0, sizeof(rcv_buf));

	int nodata_pre_ms = ::GetTickCount();
	int nodata_now_ms = ::GetTickCount();
	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		memset(rcv_buf, 0, sizeof(rcv_buf));
		ret = p_CUdp->RecvFrmService(rcv_buf, sizeof(rcv_buf));
		if (ret > 0)
		{
			if (0 == strlen(p_this->m_handleBuf))
			{
				memcpy(p_this->m_handleBuf, rcv_buf, sizeof(p_this->m_handleBuf));
			}
		}

		if (strlen(p_this->m_handleBuf) > 0)
		{
			nodata_pre_ms = ::GetTickCount();

			unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_GEOMETRIC_PARAMETER_MONITORING;
			unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
			UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, false);
		}
		else
		{
			nodata_now_ms = ::GetTickCount();
			if (nodata_now_ms - nodata_pre_ms > no_data_delt_tm)
			{
				nodata_pre_ms = ::GetTickCount();

				unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_GEOMETRIC_PARAMETER_MONITORING;
				unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
				UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, true);
			}
		}

	}

	return 0;
}

void CGeoParamAnalyse::AddPullGuid(tag_st_alarm* p_alarm)
{
	m_pullGuidLock.Lock();
	unsigned int size = m_pullGuid.size();
	if (size >= g_max_pull_guid_count)
	{
		gLog.Add("%s pop_front\n", __FUNCTION__);
		m_pullGuid.pop_front();
	}

	m_pullGuid.push_back(p_alarm);

	m_pullGuidLock.Unlock();
}

tag_st_alarm* CGeoParamAnalyse::PickPullGuid()
{
	tag_st_alarm* p_alarm = NULL;
	m_pullGuidLock.Lock();
	unsigned int size = m_pullGuid.size();
	if (size > 0)
	{
		p_alarm = m_pullGuid[0];
		m_pullGuid.pop_front();
	}
	m_pullGuidLock.Unlock();
	return p_alarm;
}

unsigned int WINAPI CGeoParamAnalyse::thread_geoparam_handle( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CGeoParamAnalyse* p_this = (CGeoParamAnalyse*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 5;
	const int handle_delt_tm = 10;
	int ret = 0;

	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	int pre_ms = ::GetTickCount();

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		int nw_ms = ::GetTickCount();
		if (nw_ms - pre_ms >= handle_delt_tm)
		{
			pre_ms = nw_ms;

			std::string file_path = p_this->m_pCStructAnalyse->GetFramePath();
			if (file_path.empty())
			{
				continue;
			}

			//"{JH_JG}&BY&BY&导高值1&拉出值1"
			//"{JH_JG}&BY&BY&导高值1#导高值2&拉出值1#拉出值2"

			if (strlen(p_this->m_handleBuf) > 0)
			{
				const int c_lcdg_one = 4;
				const int c_lcdg_two = 6;
				double lc1, dg1, lc2, dg2 = 0;
				char spare1[260];
				char spare2[260];
				tag_st_pull_guid* p_pull_guid = new tag_st_pull_guid;
				int geo_count = sscanf(p_this->m_handleBuf, "{JH_JG}&%260[^&]&%260[^&]&%lf&%lf", spare1, spare2, &dg1, &lc1);
				if (geo_count != c_lcdg_one)
				{
					geo_count = sscanf(p_this->m_handleBuf, "{JH_JG}&%260[^&]&%260[^&]&%lf#%lf&%lf#%lf", spare1, spare2, &dg1, &dg2, &lc1, &lc2);
				}
				memset(p_this->m_handleBuf, 0, sizeof(p_this->m_handleBuf));
				if (c_lcdg_one == geo_count)
				{
					//lc1 dg1
					p_pull_guid->isTwoValue = false;
					p_pull_guid->pull1 = lc1 * 1.000;
					p_pull_guid->guid1 = dg1 * 1.000;

					if (1)
					{
						if (p_pull_guid->pull1 > p_this->m_pullThresholdCls1)
						{
							tag_st_alarm* p_alarm = new tag_st_alarm;
							p_alarm->AlarmType = E_FRAME_WARN_TYPE_PULLOUT;
							p_alarm->AlarmValue = p_pull_guid->pull1;
							p_alarm->AlarmLevel = E_FRAME_WARN_CLS_1;
							p_this->AddPullGuid(p_alarm);
						}
						else if (p_pull_guid->pull1 > p_this->m_pullThresholdCls2)
						{
							tag_st_alarm* p_alarm = new tag_st_alarm;
							p_alarm->AlarmType = E_FRAME_WARN_TYPE_PULLOUT;
							p_alarm->AlarmValue = p_pull_guid->pull1;
							p_alarm->AlarmLevel = E_FRAME_WARN_CLS_2;
							p_this->AddPullGuid(p_alarm);
						}
						else if (p_pull_guid->pull1 > p_this->m_pullThresholdCls3)
						{
							tag_st_alarm* p_alarm = new tag_st_alarm;
							p_alarm->AlarmType = E_FRAME_WARN_TYPE_PULLOUT;
							p_alarm->AlarmValue = p_pull_guid->pull1;
							p_alarm->AlarmLevel = E_FRAME_WARN_CLS_3;
							p_this->AddPullGuid(p_alarm);
						}

						if (p_pull_guid->guid1 > p_this->m_guidThresholdCls1)
						{
							tag_st_alarm* p_alarm = new tag_st_alarm;
							p_alarm->AlarmType = E_FRAME_WARN_TYPE_GUIDE_HEIGHT;
							p_alarm->AlarmValue = p_pull_guid->guid1;
							p_alarm->AlarmLevel = E_FRAME_WARN_CLS_1;
							p_this->AddPullGuid(p_alarm);
						}
						else if (p_pull_guid->guid1 > p_this->m_guidThresholdCls2)
						{
							tag_st_alarm* p_alarm = new tag_st_alarm;
							p_alarm->AlarmType = E_FRAME_WARN_TYPE_GUIDE_HEIGHT;
							p_alarm->AlarmValue = p_pull_guid->guid1;
							p_alarm->AlarmLevel = E_FRAME_WARN_CLS_2;
							p_this->AddPullGuid(p_alarm);
						}
						else if (p_pull_guid->guid1 > p_this->m_guidThresholdCls3)
						{
							tag_st_alarm* p_alarm = new tag_st_alarm;
							p_alarm->AlarmType = E_FRAME_WARN_TYPE_GUIDE_HEIGHT;
							p_alarm->AlarmValue = p_pull_guid->guid1;
							p_alarm->AlarmLevel = E_FRAME_WARN_CLS_3;
							p_this->AddPullGuid(p_alarm);
						}
					}

					char show_msg[128];
					sprintf(show_msg, "拉出值1=%.3f;导高值1=%.3f;", p_pull_guid->pull1, p_pull_guid->guid1);
					CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
					if(p_CPantographMonitorDlg)
					{
						p_CPantographMonitorDlg->AddGeoParam(show_msg);
					}

				}
				else if (c_lcdg_two == geo_count)
				{
					//lc1 dg1 lc2 dg2
					p_pull_guid->isTwoValue = true;
					p_pull_guid->pull1 = lc1 * 1.000;
					p_pull_guid->guid1 = dg1 * 1.000;
					p_pull_guid->pull2 = lc2 * 1.000;
					p_pull_guid->guid2 = dg2 * 1.000;

					char show_msg[128];
					sprintf(show_msg, "拉出值1=%.3f;导高值1=%.3f;拉出值2=%.3f;导高值2=%.3f;", 
						p_pull_guid->pull1, p_pull_guid->guid1, p_pull_guid->pull2, p_pull_guid->guid2);
					CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
					if(p_CPantographMonitorDlg)
					{
						p_CPantographMonitorDlg->AddGeoParam(show_msg);
					}
				}
				else
				{
					//丢弃
				}

				//纪录到实时数据库
				if(c_lcdg_two == geo_count || c_lcdg_one == geo_count)
				{
					bool state_well = p_this->m_pCMvb->GetMvbState();
					if (state_well)
					{				
						MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();
						const int station_ind = p_this->m_pCMvb->GetCurrentStationInd();
						MVB::tag_st_station_info* p_station_now = p_this->m_pCMvb->GetCurrentStationInf();

						st_rt_data rt_data;
						rt_data.CarriageId = p_st_mvb_data->train_num;
						rt_data.CarriageNum = p_this->m_carriageNum;
						rt_data.StationInd = station_ind;
						rt_data.LineId = p_this->m_lineId;
						rt_data.Direction = p_st_mvb_data->direction;
						rt_data.DataType = E_RT_TYPE_GEO;
						char rt_data_msg[128];
						if(c_lcdg_one == geo_count)
						{
							sprintf(rt_data_msg, "%.03f;%.03f", p_pull_guid->pull1, p_pull_guid->guid1);
						}
						else if(c_lcdg_two == geo_count)
						{
							sprintf(rt_data_msg, "%.03f;%.03f;%.03f;%.03f", p_pull_guid->pull1, p_pull_guid->guid1, p_pull_guid->pull2, p_pull_guid->guid2);
						}
						rt_data.RtData = rt_data_msg;
						rt_data.BeginTime = CTimeStamp::GetInstance()->parseTime(CTimeStamp::GetInstance()->GetTimeStampMs());
						{
							NSP_STP_CM::tag_st_time cur_time;
							CTimeStamp::GetInstance()->parseTime(&cur_time, CTimeStamp::GetInstance()->GetTimeStampMs());
							rt_data.BeginTimeMs = cur_time.ms;
						}

						//file_path struct
						//插入数据
						sprintf(sql_cmd, "insert into  %s  (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s) \
										 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %d, '%s', '%s');",
										 gTblRt.c_str(), 
										 gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(), gRtFLineId.c_str(), 
										 gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), gRtFBeginTime.c_str(), 
										 gRtFBeginTimeMs.c_str(), gRtFFullName.c_str(), gRtFStationName.c_str(),
										 rt_data.CarriageId, rt_data.CarriageNum, rt_data.StationInd , rt_data.LineId, 
										 rt_data.Direction, rt_data.DataType, rt_data.RtData.c_str(), rt_data.BeginTime.c_str(),              
										 rt_data.BeginTimeMs, (NSP_STP_CM::str_sig_double(file_path, '\\')).c_str(), p_station_now->station_alias
										 );
						ret = p_CMySqlDB->WriteDataBase(sql_cmd);
						if (ret != 0)
						{
							gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
						}
					}
				}

				delete p_pull_guid;
			}
		}
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

void CGeoParamAnalyse::SetGeoThreshold( double pull_thre_cls1, double pull_thre_cls2, double pull_thre_cls3, double guid_thre_cls1, double guid_thre_cls2, double guid_thre_cls3 )
{
	m_pullThresholdCls1 = pull_thre_cls1;
	m_pullThresholdCls2 = pull_thre_cls2;
	m_pullThresholdCls3 = pull_thre_cls3;
	m_guidThresholdCls1 = guid_thre_cls1;
	m_guidThresholdCls2 = guid_thre_cls2;
	m_guidThresholdCls3 = guid_thre_cls3;
}



