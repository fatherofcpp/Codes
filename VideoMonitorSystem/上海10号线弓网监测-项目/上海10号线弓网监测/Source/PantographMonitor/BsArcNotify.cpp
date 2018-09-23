#include "StdAfx.h"
#include "BsArcNotify.h"
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
#include "BsArcCollector.h"
#include "PantographMonitor.h"
#include "PantographMonitorDlg.h"
#include "BsTimeStamp.h"

/////////////
#define USE_ARC_NOTIFY_TEST 0
/////////////

const static unsigned int g_max_arcinf_count = 24 * 60 * 60 * 2;//保存24小时的燃弧信息,最快1秒2条
const static unsigned int g_max_currentinf_count = 24 * 60 * 60 * 1;//保存24小时的电流信息,最快1秒1条

const static double s_delt_collect_us = 0.1;//采样点间0.1微妙

CArcNotify::CArcNotify(void)
{
	m_pAbstractDeviceInfo = new Cam::CamDeviceInfo;
	m_pAbstractFormatInfo = new Cam::CamDeviceFormatInfo;

	m_arcInf.clear();
	m_currentInf.clear();

	m_udpCurrentPort = 0;
	m_arcRateCls1 = 0;
	m_arcRateCls2 = 0;
	m_arcRateCls3 = 0;
	m_currentRated = 0;
	m_arcBeginThreshold = 0;
	m_arcEndThreshold = 0;
	m_arcDeltTimeMsThreshold = 0;
	m_arcContinuTimeUsThreshold = 0;
	m_arcStrenthXsThreshold = 0;
	m_arcTimeLenGainThreshold = 0;

	m_pCArcCollector = NULL;
	m_pCStructAnalyse = NULL;

	memset(m_handleBuf, 0, sizeof(m_handleBuf));

	m_currentLastTimeMs = 0;
	m_arcLastTimeMs = 0;
	m_arcCbNum = 0;
}


CArcNotify::~CArcNotify(void)
{
	ReleaseRes();
}

void CArcNotify::SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo)
{
	if (!p_AbstractDeviceInfo || !p_AbstractFormatInfo)
	{
		return;
	}
	*(dynamic_cast<Cam::CamDeviceInfo*>(m_pAbstractDeviceInfo)) = *(dynamic_cast<Cam::CamDeviceInfo*>(p_AbstractDeviceInfo));
	*(dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo)) = *(dynamic_cast<Cam::CamDeviceFormatInfo*>(p_AbstractFormatInfo));

	m_pAbstractFormatInfo->pReceiveData = &CArcNotify::arc_inf_frame_cb;
	m_pAbstractFormatInfo->pParam = this;
	m_pAbstractFormatInfo->pHeartbeatConnectedCallback = &CArcNotify::thread_heart_beat_cb;
	m_pAbstractFormatInfo->pHeartbeatParam = this;
	(dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo))->pReceiveData2 = NULL;
	(dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo))->pParam2 = NULL;
}

void CALLBACK CArcNotify::arc_inf_frame_cb( const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader)
{
	CArcNotify* p_this = (CArcNotify*)pParam;
	if (p_this == NULL)
	{
		return;
	}

	p_this->m_arcCbNum = (p_this->m_arcCbNum + 1) % 0x0000FFFF;


	st_arc_camera arc_camera;
	memcpy(&arc_camera, pDataHeader, sizeof(arc_camera));

	bool state_well = p_this->m_pCMvb->GetMvbState();
	if (state_well)
	{
		const int c_point_num = arc_camera.eIndex - arc_camera.sIndex + 1;
		if (c_point_num <= 0)
		{
			gLog.Add("%s arc_camera.eIndex=%u,arc_camera.sIndex=%u\n", __FUNCTION__, arc_camera.eIndex, arc_camera.sIndex);
			return;
		}

		NSP_STP_CM::st_time tim_n;
		CTimeStamp::GetInstance()->parseTime(&tim_n, CTimeStamp::GetInstance()->GetTimeStampMs());

		time_t n_now_time_s = 0;
		NSP_STP_CM::GetStampByTime(&n_now_time_s, tim_n);
		time_t n_now_time_ms = n_now_time_s * 1000 + tim_n.ms;
		time_t last_time_us = (int)(c_point_num * s_delt_collect_us);
		if (last_time_us < 1000)
		{
			last_time_us %= 40;
			last_time_us *= 1000;
		}
		time_t last_time_ms = (int)(last_time_us / 1000);

		st_arc_inf* p_arc_inf = new st_arc_inf;
		if (1)
		{
			NSP_STP_CM::data_compress(pData, iDataLen, sizeof(p_arc_inf->arcPointVal), p_arc_inf->arcPointVal, p_arc_inf->arcPointNum);
		}

		p_arc_inf->packInd = arc_camera.aIndex;

		const int c_sasc = 1;
		const double xs_size = (double)p_this->m_arcStrenthXsThreshold;
		const double bd_size = xs_size * c_sasc;
		const double c_wps = 12.5;//12.5uw/cm2
		const double c_arc_size_cm2 = (double)(((double)arc_camera.aSize) / bd_size);
		const unsigned int arc_val = (unsigned int)(c_arc_size_cm2 * c_wps);

		p_arc_inf->arcVal = arc_val;
		p_arc_inf->beginTimeM = n_now_time_ms;
		p_arc_inf->endTimeM = n_now_time_ms + last_time_ms;

		p_this->AddArcInf(p_arc_inf);

		char show_msg[128];
		sprintf(show_msg, "强度=%u,时长=%lld毫秒,packInd=%u,aSize=%u,sInd=%u,eInd=%u;", 
			arc_val, last_time_ms, arc_camera.aIndex, arc_camera.aSize, arc_camera.sIndex, arc_camera.eIndex);
		CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
		if(p_CPantographMonitorDlg)
		{
			p_CPantographMonitorDlg->AddArcStrength(show_msg);
		}

		gLog.Add("%s sIndex=%u, eIndex=%u, aIndex=%u, aSize=%u\n", __FUNCTION__
			, arc_camera.sIndex, arc_camera.eIndex, arc_camera.aIndex, arc_camera.aSize
			);
	}

}

unsigned int WINAPI CArcNotify::thread_pull_arcinf( void* pParam)
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CArcNotify* p_this = (CArcNotify*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;
	while(1)
	{
		Sleep(sleep_m);

		int check_res = p_this->ThreadStateCheck(thread_ind);
		if (check_res == NSP_STP_CM::E_THREAD_STATE_STOPED)
		{
			p_this->StopCam();
			break;
		}
		else if (check_res == NSP_STP_CM::E_THREAD_STATE_PAUSED)
		{
			bool suc_pause = p_this->PauseCam();
			continue;
		}

		bool suc_play = p_this->PlayCam(NSP_STP_CM::E_CPU_MASK_1);

#if USE_ARC_NOTIFY_TEST
		Sleep(1000);
		static int ind = 0;
		ind ++;
		st_arc_camera arc_camera;
		const int c_arc_time_len_ms = 1000;
		const int c_point_num = c_arc_time_len_ms * 1000 / s_delt_collect_us;
		const int c_point_strength = ind;
		arc_camera.aIndex = ind;
		arc_camera.sIndex = 0;
		arc_camera.eIndex = c_point_num - 1;
		arc_camera.aSize = c_point_strength * c_point_num;
		int iDataLen = sizeof(arc_camera);
		arc_inf_frame_cb( NULL, iDataLen, p_this, &arc_camera);
#endif
	}

	return 0;
}

unsigned int WINAPI CArcNotify::thread_current_recv( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CArcNotify* p_this = (CArcNotify*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 5;
	const int no_data_delt_tm = p_this->m_currentTimeoutS;

	NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
	p_CUdp->SetLocalPort(p_this->m_udpCurrentPort);
	timeval r_time_out;
	r_time_out.tv_sec = 0;
	r_time_out.tv_usec = sleep_m * 1000;
	p_CUdp->SetReadTimeOut(r_time_out);
	int ret = p_CUdp->BindPort();
	if (ret != 0)
	{
		gLog.Add("%s p_CUdp->BindPort fail\n", __FUNCTION__);
		return 0;
	}
	char rcv_buf[256];
	memset(rcv_buf, 0, sizeof(rcv_buf));

	const int c_recv_len = 12;

	int nodata_pre_ms = ::GetTickCount();
	int nodata_now_ms = ::GetTickCount();
	while(1)
	{
		Sleep(sleep_m);

		int check_res = p_this->ThreadStateCheck(thread_ind);
		if (check_res == NSP_STP_CM::E_THREAD_STATE_STOPED)
		{
			break;
		}

		memset(rcv_buf, 0, sizeof(rcv_buf));
		ret = p_CUdp->RecvFrmService(rcv_buf, sizeof(rcv_buf));
		if (ret == c_recv_len)
		{
			if (0 == strlen(p_this->m_handleBuf))
			{
				memcpy(p_this->m_handleBuf, rcv_buf, sizeof(p_this->m_handleBuf));
			}
		}

		//"{i=01234567}"
		if (c_recv_len == strlen(p_this->m_handleBuf))
		{
			nodata_pre_ms = ::GetTickCount();

			unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_CURRENT_SENSOR;
			unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
			UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, false);
		}
		else
		{
			nodata_now_ms = ::GetTickCount();
			if (nodata_now_ms - nodata_pre_ms > no_data_delt_tm)
			{
				nodata_pre_ms = ::GetTickCount();

				unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_CURRENT_SENSOR;
				unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
				UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, true);
			}
		}

	}

	return 0;
}

int CArcNotify::TaskStart()
{
	int ret = CameraPictureInit();
	if (ret != 0)
	{
		return -1;
	}

	SetThreadCount(4);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_pull_arcinf", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_pull_arcinf, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_current_recv", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_current_recv, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 2;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_current_handle", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_current_handle, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 3;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_analyse_arc", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_analyse_arc, p_st_thread_param, CpuMaskRemain());
	}

	return 0;
}

void CArcNotify::AddArcInf( st_arc_inf* p_ac_inf )
{
	CCriticalSection* p_lock = &m_ArcInfLock;
	std::deque<tag_st_arc_inf*>* p_cont = &m_arcInf;

	p_lock->Lock();
	unsigned int size = (*p_cont).size();
	if (size > 0)
	{
		if (size >= g_max_arcinf_count)
		{
			st_arc_inf* ele_pop = (*p_cont)[0];
			gLog.Add("%s pop_front packInd=%d\n", __FUNCTION__, ele_pop->packInd);
			delete ele_pop;
			(*p_cont).pop_front();
		}

		int last_ind = size - 1;
		st_arc_inf* vec_ele = (*p_cont)[last_ind];
		const int c_max_ms = 1;
		if (vec_ele->packInd == p_ac_inf->packInd
			|| abs(vec_ele->endTimeM - p_ac_inf->beginTimeM) <= c_max_ms)
		{
			//合并
			(*p_cont)[last_ind]->endTimeM = p_ac_inf->endTimeM;
			(*p_cont)[last_ind]->arcVal += p_ac_inf->arcVal;

			{
				const int c_max_output_num = sizeof(p_ac_inf->arcPointVal);

				st_arc_inf arc_inf;
				char* p_output_data = NULL;

				//pre
				int output_len_pre = 0;
				p_output_data = arc_inf.arcPointVal;
				NSP_STP_CM::data_compress(
					(*p_cont)[last_ind]->arcPointVal,
					(*p_cont)[last_ind]->arcPointNum,
					c_max_output_num / 2,
					p_output_data,
					output_len_pre
					);

				//now
				int output_len_now = 0;
				p_output_data = arc_inf.arcPointVal + output_len_pre;
				NSP_STP_CM::data_compress(
					p_ac_inf->arcPointVal,
					p_ac_inf->arcPointNum,
					c_max_output_num / 2,
					p_output_data,
					output_len_now
					);
				arc_inf.arcPointNum = output_len_pre + output_len_now;
			}

			delete p_ac_inf;
		}
		else
		{
			//新增
			(*p_cont).push_back(p_ac_inf);
		}
	}
	else
	{
		(*p_cont).push_back(p_ac_inf);
	}

	p_lock->Unlock();
}

st_arc_inf* CArcNotify::PickArcInf()
{
	CCriticalSection* p_lock = &m_ArcInfLock;
	std::deque<tag_st_arc_inf*>* p_cont = &m_arcInf;

	st_arc_inf* ele_pop = NULL;
	p_lock->Lock();

	unsigned int size = (*p_cont).size();
	if (size > 0)
	{
		ele_pop = (*p_cont)[0];
		(*p_cont).pop_front();
	}
	p_lock->Unlock();

	return ele_pop;
}

unsigned int WINAPI CArcNotify::thread_analyse_arc( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CArcNotify* p_this = (CArcNotify*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;

	int ret = 0;
	bool is_start = false;
	char* sql_cmd = new char[1024*8];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);
	while(1)
	{
		Sleep(sleep_m);

		int check_res = p_this->ThreadStateCheck(thread_ind);
		if (check_res == NSP_STP_CM::E_THREAD_STATE_STOPED)
		{
			break;
		}

		//确保分包燃弧合并完成
		unsigned int arc_cb_pre = p_this->m_arcCbNum;
		int pre_balance_ms = ::GetTickCount();
		while (1)
		{
			unsigned int arc_cb_nw = p_this->m_arcCbNum;
			if (arc_cb_nw != arc_cb_pre)
			{
				arc_cb_pre = arc_cb_nw;
				pre_balance_ms = ::GetTickCount();
			}
			Sleep(100);

			int nw_balance_ms = ::GetTickCount();
			if (nw_balance_ms - pre_balance_ms > 1000)
			{
				break;
			}
		}

		st_arc_inf* p_arc_inf = p_this->PickArcInf();
		if (p_arc_inf)
		{
			MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();
			const int station_ind = p_this->m_pCMvb->GetCurrentStationInd();
			MVB::tag_st_station_info* p_station_now = p_this->m_pCMvb->GetCurrentStationInf();

			//记录燃弧强度
			NSP_STP_CM::st_time tm_begin;
			time_t tm_ms_b = (p_arc_inf->beginTimeM);
			NSP_STP_CM::GetTimeByStamp(tm_ms_b / 1000, &tm_begin);
			tm_begin.ms = tm_ms_b % 1000;

			NSP_STP_CM::st_time tm_end;
			time_t tm_ms_e = (p_arc_inf->endTimeM);
			NSP_STP_CM::GetTimeByStamp(tm_ms_e / 1000, &tm_end);
			tm_end.ms = tm_ms_e % 1000;

			st_rt_data rt_data;
			rt_data.CarriageId = p_st_mvb_data->train_num;
			rt_data.CarriageNum = p_this->m_carriageNum;
			rt_data.StationInd = station_ind;
			rt_data.LineId = p_this->m_lineId;
			rt_data.Direction = p_st_mvb_data->direction;
			rt_data.DataType = E_RT_TYPE_ARC_STRENGTH;
			char rt_data_str[64];
			sprintf(rt_data_str, "%d", p_arc_inf->arcVal);
			rt_data.RtData = rt_data_str;
			char time_inf[256];
			sprintf(time_inf, "%04d-%02d-%02d %02d:%02d:%02d", tm_begin.year, tm_begin.mon, tm_begin.day, tm_begin.hour, tm_begin.min, tm_begin.sec);
			rt_data.BeginTime = time_inf;
			{
				NSP_STP_CM::st_time tim_n;
				NSP_STP_CM::GetNowTime(&tim_n);
				rt_data.BeginTimeMs = tim_n.ms;
			}
			rt_data.LastTimeMs = (int)(p_arc_inf->endTimeM - p_arc_inf->beginTimeM);

			//增加燃弧Y值数组
			char arc_gray[1024 * 4];
			int arc_gray_off = 0;
			for (int i = 0; i < p_arc_inf->arcPointNum; i ++)
			{
				arc_gray_off += sprintf(arc_gray + arc_gray_off, "%02x&", (unsigned char)p_arc_inf->arcPointVal[i]);
			}

			//插入数据
			sprintf(sql_cmd, "insert into  %s  (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s) \
							 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %d, %d, '%s', '%s');",
							 gTblRt.c_str(), 
							 gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(), gRtFLineId.c_str(), 
							 gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), gRtFBeginTime.c_str(), 
							 gRtFBeginTimeMs.c_str(), gRtFLastTime.c_str(), gRtFDataParam.c_str(), gRtFStationName.c_str(),
							 rt_data.CarriageId, rt_data.CarriageNum, rt_data.StationInd , rt_data.LineId, 
							 rt_data.Direction, rt_data.DataType, rt_data.RtData.c_str(), rt_data.BeginTime.c_str(), 
							 rt_data.BeginTimeMs, rt_data.LastTimeMs, arc_gray, p_station_now->station_alias
							 );
			ret = p_CMySqlDB->WriteDataBase(sql_cmd);
			if (ret != 0)
			{
				gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
			}

			//记录燃弧率到数据库
			//double arc_rate = p_this->ArcCalcOnce(p_arc_inf);
			double arc_rate = p_this->ArcCalcTotal(p_arc_inf);
			if (arc_rate > 0)
			{
				char show_msg[128];
				sprintf(show_msg, "燃弧率=%.3f,m_currentLastTimeMs=%lld,m_arcLastTimeMs=%lld", 
					arc_rate, p_this->m_currentLastTimeMs, p_this->m_arcLastTimeMs);
				CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
				if(p_CPantographMonitorDlg)
				{
					p_CPantographMonitorDlg->AddArcRate(show_msg);
				}

				std::string frame_path = p_this->m_pCArcCollector->GetFramePath();
				std::string structure_frame_path = p_this->m_pCStructAnalyse->GetFramePath();
				const bool is_alarm_when_happen = true;
				if (!frame_path.empty() 
					&& !structure_frame_path.empty() 
					&& (is_alarm_when_happen || arc_rate > p_this->m_arcRateCls1)
					)
				{
					//纪录到报警数据库
					unsigned int alarm_type = E_FRAME_WARN_TYPE_ARCRATE;
					unsigned int alarm_cls = E_FRAME_WARN_CLS_1;
					double alarm_val = arc_rate;

					if (arc_rate > p_this->m_arcRateCls1)
					{
						UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, false);
					}

					std::string alarm_message = "";
					FormatAlarmMessage(alarm_type, alarm_cls, alarm_val, alarm_message);

					long long alarm_id = -1;			
					unsigned int begin_time_ms = ::GetTickCount();
					while(1)
					{
						//确保该桢已经写入数据库
						sprintf(sql_cmd, "select %s from %s where  %s = '%s' and %s > %s LIMIT 1;", 
							gAlarmFKey.c_str(),
							gTblAlarm.c_str(), 
							gAlarmFFullName.c_str(), (NSP_STP_CM::str_sig_double(frame_path, '\\')).c_str(),
							gAlarmFBeginTime.c_str(), gDbTimeOneDayAgo.c_str()
							);
						unsigned long long row_count = 0;
						unsigned int col_count = 0;
						char*** ppp_res = NULL;
						ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
						if (ret == 0)
						{
							NSP_STP_CM::str_to_int(ppp_res[0][0], alarm_id);

							p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
						}
						else
						{
							//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
						}

						if (alarm_id > 0)
						{
							break;
						}
						unsigned int cur_time_ms = ::GetTickCount();
						unsigned int toltal_time_ms = cur_time_ms - begin_time_ms;
						const int total_wait_ms = 2000;
						if (toltal_time_ms > total_wait_ms)
						{
							break;
						}
						Sleep(500);
					}

					if (alarm_id > 0)
					{
						//修改图片名称
						char new_file_path[MAX_PATH];
						std::string old_file_name = NSP_STP_CM::str_left_cut_tag( frame_path, p_this->m_rootPath);
						std::string file_no_suf = NSP_STP_CM::str_right_cut_tag(old_file_name, p_this->m_saveFrameSuffix);
						std::string old_file_no_suf = p_this->m_rootPathAlarm + file_no_suf;
						sprintf(new_file_path, "%s_%d_%02d%s", old_file_no_suf.c_str(), alarm_type, alarm_cls, p_this->m_saveFrameSuffix.c_str());
						
						//copyfile
						st_db_alarm db_alarm;
						db_alarm.FullName = new_file_path;
						db_alarm.FileName = NSP_STP_CM::str_left_cut_tag( db_alarm.FullName, "\\");
						db_alarm.AlarmType = alarm_type;
						db_alarm.AlarmLevel = alarm_cls;
						db_alarm.AlarmMessage = alarm_message;
						//db_alarm.AlarmValue = arc_rate;
						db_alarm.AlarmValue = (double)p_arc_inf->arcVal;
						double milage = 0;

						//提取信息
						{
							sprintf(sql_cmd, "select %s, %s,  %s, %s, %s,  %s,  %s,  %s from %s where  %s = %lld LIMIT 1;", 
								gAlarmFCameraPos.c_str(), gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), 
								gAlarmFLineId.c_str(), gAlarmFDirection.c_str(), gAlarmFBeginTime.c_str(), gAlarmFMileage.c_str(),
								gTblAlarm.c_str(), 
								gAlarmFKey.c_str(), alarm_id
								);
							unsigned long long row_count = 0;
							unsigned int col_count = 0;
							char*** ppp_res = NULL;
							ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
							if (ret == 0)
							{
								NSP_STP_CM::str_to_int(ppp_res[0][0], db_alarm.CameraPos);
								NSP_STP_CM::str_to_int(ppp_res[0][1], db_alarm.CarriageId);
								NSP_STP_CM::str_to_int(ppp_res[0][2], db_alarm.CarriageNum);
								NSP_STP_CM::str_to_int(ppp_res[0][3], db_alarm.StationId);
								NSP_STP_CM::str_to_int(ppp_res[0][4], db_alarm.LineId);
								NSP_STP_CM::str_to_int(ppp_res[0][5], db_alarm.Direction);
								db_alarm.BeginTime = ppp_res[0][6];
								milage = atof(ppp_res[0][7]);

								p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
							}
							else
							{
								gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
							}
						}

						//故障拷贝
						{
							ret = NSP_STP_CM::copyFile(frame_path.c_str(), new_file_path, true);
							if (ret == 0)
							{
								//新增一条数据库
								//插入数据
								sprintf(sql_cmd, "insert into  %s  ( %s, %s,  %s, %s, %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s) \
												 values (%d, %d, %d, %d, %d, %d, '%s', '%s', '%s', %.03f, %d, %d, '%s', %.03f,  '%s', '%s');",
												 gTblAlarm.c_str(), 
												 gAlarmFCameraPos.c_str(), gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), 
												 gAlarmFLineId.c_str(), gAlarmFDirection.c_str(), gAlarmFFileName.c_str(), gAlarmFFullName.c_str(), 
												 gAlarmFBeginTime.c_str(), gAlarmFMileage.c_str(), gAlarmFAlarmType.c_str(), gAlarmFAlarmLevel.c_str(),  
												 gAlarmFAlarmMessage.c_str(), gAlarmFAlarmValue.c_str(), gAlarmFAlarmValueParam.c_str(), gAlarmFStuctureFullName.c_str(),
												 db_alarm.CameraPos, db_alarm.CarriageId, db_alarm.CarriageNum, db_alarm.StationId , 
												 db_alarm.LineId, db_alarm.Direction, db_alarm.FileName.c_str(), (NSP_STP_CM::str_sig_double(db_alarm.FullName, '\\')).c_str(), 
												 db_alarm.BeginTime.c_str(), milage, db_alarm.AlarmType, db_alarm.AlarmLevel, 
												 db_alarm.AlarmMessage.c_str(), db_alarm.AlarmValue, arc_gray, (NSP_STP_CM::str_sig_double(structure_frame_path, '\\')).c_str()             
												 );
								ret = p_CMySqlDB->WriteDataBase(sql_cmd);
								if (ret != 0)
								{
									gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
								}
							}
							else
							{
								gLog.Add("%s copyFile %s to %s fail,ret=%d\n", __FUNCTION__, frame_path.c_str(), new_file_path, ret);
							}
						}
					}
				}

				//纪录到实时数据库
				if (1)
				{
					st_rt_data rt_data;
					rt_data.CarriageId = p_st_mvb_data->train_num;
					rt_data.CarriageNum = p_this->m_carriageNum;
					rt_data.StationInd = station_ind;
					rt_data.LineId = p_this->m_lineId;
					rt_data.Direction = p_st_mvb_data->direction;
					rt_data.DataType = E_RT_TYPE_ARC_RATE;
					char rt_data_str[64];
					sprintf(rt_data_str, "%.03f", arc_rate);
					rt_data.RtData = rt_data_str;
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
									 rt_data.BeginTimeMs, arc_gray, p_station_now->station_alias
									 );
					ret = p_CMySqlDB->WriteDataBase(sql_cmd);
					if (ret != 0)
					{
						gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
				}
			}

			delete p_arc_inf;
		}

	}
	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

void CALLBACK CArcNotify::thread_heart_beat_cb( const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader )
{
	CArcNotify* p_this = (CArcNotify*)pParam;
	if (p_this == NULL)
	{
		return;
	}

	p_this->m_cameraHeartBeat = (unsigned int)(p_this->m_cameraHeartBeat + 1);
}

void CArcNotify::AddCurrentInf( tag_st_current_inf* p_current_inf )
{
	CCriticalSection* p_lock = &m_CurrentInfLock;
	std::deque<tag_st_current_inf*>* p_cont = &m_currentInf;

	p_lock->Lock();

	unsigned int size = (*p_cont).size();
	if (size > 0)
	{
		if (size >= g_max_currentinf_count)
		{
			tag_st_current_inf* ele_pop = (*p_cont)[0];
			gLog.Add("%s pop_front currentVal=%d\n", __FUNCTION__, ele_pop->currentVal);
			delete ele_pop;
			(*p_cont).pop_front();
		}

		int last_ind = size - 1;
		tag_st_current_inf* vec_ele = (*p_cont)[last_ind];
		const int max_ms = 1 * 1000;
		if (abs(vec_ele->endTimeM - p_current_inf->beginTimeM) <= max_ms)
		{
			//合并
			(*p_cont)[last_ind]->endTimeM = p_current_inf->endTimeM;
			delete p_current_inf;
		}
		else
		{
			//新增
			(*p_cont).push_back(p_current_inf);
		}
	}
	else
	{
		(*p_cont).push_back(p_current_inf);
	}

	p_lock->Unlock();

}

tag_st_current_inf* CArcNotify::PickCurrentInf()
{
	CCriticalSection* p_lock = &m_CurrentInfLock;
	std::deque<tag_st_current_inf*>* p_cont = &m_currentInf;

	tag_st_current_inf* ele_pop = NULL;
	p_lock->Lock();

	unsigned int size = (*p_cont).size();
	if (size > 0)
	{
		ele_pop = (*p_cont)[0];
		(*p_cont).pop_front();
	}
	p_lock->Unlock();

	return ele_pop;
}

void CArcNotify::ReleaseRes()
{
	for (unsigned int i = 0; i < m_arcInf.size(); i ++)
	{
		tag_st_arc_inf* p_arc_inf = m_arcInf[i];
		delete p_arc_inf;
	}
	m_arcInf.clear();

	for (unsigned int i = 0; i < m_currentInf.size(); i ++)
	{
		tag_st_current_inf* p_current_inf = m_currentInf[i];
		delete p_current_inf;
	}
	m_currentInf.clear();
}

void CArcNotify::SetCameraOpt()
{
	//设置参数
	int ret = 0;
	while (1)
	{
		ret = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_STM, "1\r\n");//1:Snap
		if (ret == 0)
		{
			break;
		}
		else
		{
			gLog.Add("%s Snap\n", __FUNCTION__);
			Sleep(1000);
		}
	}

	while (1)
	{
		ret = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_SAST, "%d\r\n", m_arcBeginThreshold);//设置燃弧开始阈值
		if (ret == 0)
		{
			break;
		}
		else
		{
			gLog.Add("%s 设置燃弧开始阈值\n", __FUNCTION__);
			Sleep(1000);
		}
	}

	while (1)
	{
		ret = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_SAET, "%d\r\n", m_arcEndThreshold);//设置燃弧结束阈值
		if (ret == 0)
		{
			break;
		}
		else
		{
			gLog.Add("%s 设置燃弧结束阈值\n", __FUNCTION__);
			Sleep(1000);
		}
	}

	while (1)
	{
		ret = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_SSPT, "%d\r\n", m_arcDeltTimeMsThreshold);//设置燃弧间隔时间
		if (ret == 0)
		{
			break;
		}
		else
		{
			gLog.Add("%s 设置燃弧间隔时间\n", __FUNCTION__);
			Sleep(1000);
		}
	}

	while (1)
	{
		ret = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_SDUT, "%d\r\n", m_arcContinuTimeUsThreshold);//设置燃弧持续时间
		if (ret == 0)
		{
			break;
		}
		else
		{
			gLog.Add("%s 设置燃弧持续时间\n", __FUNCTION__);
			Sleep(1000);
		}
	}

	while (1)
	{
		ret = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_SMUT, "500\r\n");//燃弧分包上传时最大单位时间
		if (ret == 0)
		{
			break;
		}
		else
		{
			gLog.Add("%s 燃弧分包上传时最大单位时间\n", __FUNCTION__);
			Sleep(1000);
		}
	}

}

void CArcNotify::SetArcRateThreshold( double arc_rate_cls1, double arc_rate_cls2, double arc_rate_cls3 )
{
	m_arcRateCls1 = arc_rate_cls1;
	m_arcRateCls2 = arc_rate_cls2;
	m_arcRateCls3 = arc_rate_cls3;
}

void CArcNotify::SetArcThreshold( int arcBeginThreshold, int arcEndThreshold , int arcDeltTimeMsThreshold, 
	int arcContinuTimeUsThreshold, int arcStrenthXsThreshold, int arcTimeLenGainThreshold)
{
	m_arcBeginThreshold = arcBeginThreshold;
	m_arcEndThreshold = arcEndThreshold;
	m_arcDeltTimeMsThreshold = arcDeltTimeMsThreshold;
	m_arcContinuTimeUsThreshold = arcContinuTimeUsThreshold;
	m_arcStrenthXsThreshold = arcStrenthXsThreshold;
	m_arcTimeLenGainThreshold = arcTimeLenGainThreshold;
}

unsigned int WINAPI CArcNotify::thread_current_handle( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CArcNotify* p_this = (CArcNotify*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;
	const int handle_delt_tm = 1 * 1000;
	const int handle_total_count = handle_delt_tm / sleep_m;

	int ret = 0;
	int handle_count = 0;

	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;
	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);
	while(1)
	{
		Sleep(sleep_m);

		int check_res = p_this->ThreadStateCheck(thread_ind);
		if (check_res == NSP_STP_CM::E_THREAD_STATE_STOPED)
		{
			break;
		}

		if (++ handle_count > handle_total_count)
		{
			handle_count = 0;

			if (strlen(p_this->m_handleBuf) > 0)
			{
				//"{i=01234567}"
				int current = 0;
				ret = sscanf(p_this->m_handleBuf, "{i=%08X}", &current);
				memset(p_this->m_handleBuf, 0, sizeof(p_this->m_handleBuf));
				if (1 == ret)
				{
					bool state_well = p_this->m_pCMvb->GetMvbState();
					if (state_well)
					{
						bool is_db_current = true;
						if (is_db_current)
						{
							char sql_cmd[512];
							int ret = 0;

							MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();
							const int station_ind = p_this->m_pCMvb->GetCurrentStationInd();
							MVB::tag_st_station_info* p_station_now = p_this->m_pCMvb->GetCurrentStationInf();
							st_rt_data rt_data;
							rt_data.CarriageId = p_st_mvb_data->train_num;
							rt_data.CarriageNum = p_this->m_carriageNum;
							rt_data.StationInd = station_ind;
							rt_data.LineId = p_this->m_lineId;
							rt_data.Direction = p_st_mvb_data->direction;
							rt_data.DataType = E_RT_TYPE_CURRENT;
							char rt_data_msg[128];
							sprintf(rt_data_msg, "%d", current);
							rt_data.RtData = rt_data_msg;
							rt_data.BeginTime = CTimeStamp::GetInstance()->parseTime(CTimeStamp::GetInstance()->GetTimeStampMs());
							{
								NSP_STP_CM::tag_st_time cur_time;
								CTimeStamp::GetInstance()->parseTime(&cur_time, CTimeStamp::GetInstance()->GetTimeStampMs());
								rt_data.BeginTimeMs = cur_time.ms;
							}
							//插入数据
							sprintf(sql_cmd, "insert into  %s  (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s) \
											 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %d, '%s');",
											 gTblRt.c_str(), 
											 gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(), gRtFLineId.c_str(), 
											 gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), gRtFBeginTime.c_str(), 
											 gRtFBeginTimeMs.c_str(), gRtFStationName.c_str(),
											 rt_data.CarriageId, rt_data.CarriageNum, rt_data.StationInd , rt_data.LineId, 
											 rt_data.Direction, rt_data.DataType, rt_data.RtData.c_str(), rt_data.BeginTime.c_str(),
											 rt_data.BeginTimeMs, p_station_now->station_alias
											 );
							int res_sql = p_CMySqlDB->WriteDataBase(sql_cmd);
							if (res_sql != 0)
							{
								gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, res_sql);
							}

						}
					}

					if (current > p_this->m_currentRated)
					{
						if (state_well)
						{		
							tag_st_current_inf* p_current_inf = new tag_st_current_inf;
							NSP_STP_CM::st_time tim_n;
							NSP_STP_CM::GetNowTime(&tim_n);
							time_t n_time_stamp_s = time(NULL);
							time_t n_time_stamp_begin_ms = n_time_stamp_s * 1000 + tim_n.ms;
							p_current_inf->beginTimeM = n_time_stamp_begin_ms;
							p_current_inf->endTimeM = n_time_stamp_begin_ms + handle_delt_tm;
							p_current_inf->currentVal = current;

							p_this->AddCurrentInf(p_current_inf);
						}
					}

					char show_msg[64];
					sprintf(show_msg, "电流值=%d", current);
					CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
					if(p_CPantographMonitorDlg)
					{
						p_CPantographMonitorDlg->AddCurrent(show_msg);
					}

					//gLog.Add("%s ret=%d, rcv_buf=%s\n", __FUNCTION__, ret, rcv_buf);
				}
				else
				{
					//丢弃
				}
			}
		}
	}

	delete p_CMySqlDB;
	return 0;
}

double CArcNotify::ArcCalcOnce(st_arc_inf* p_arc_inf)
{
	double arc_rate = -1;
	const time_t arc_begin_m = p_arc_inf->beginTimeM;
	const time_t arc_end_m = p_arc_inf->endTimeM;
	const time_t arc_last_time_m = arc_end_m - arc_begin_m;
	std::deque<tag_st_current_inf*>::reverse_iterator iter_begin_current = m_currentInf.rbegin();
	std::deque<tag_st_current_inf*>::reverse_iterator iter_end_current = m_currentInf.rbegin();

	const int c_balance_count = 2;
	//最近一个电流时间起点iter_begin_current
	bool is_find_begin = false;
	if (1)
	{
		int balance_count = 0;
		std::deque<tag_st_current_inf*>::reverse_iterator iter_pre = iter_begin_current;
		for (iter_begin_current = m_currentInf.rbegin(); iter_begin_current != m_currentInf.rend(); iter_begin_current++)
		{
			tag_st_current_inf* p_current = *iter_begin_current;
			if (p_current->beginTimeM < arc_begin_m)
			{
				is_find_begin = true;

				tag_st_current_inf* p_current_pre = *iter_pre;

				if (abs((int)(p_current->currentVal - p_current_pre->currentVal)) <= (int)p_current->currentVal / 10)
				{
					//找到平衡点
					if (++balance_count >= c_balance_count)
					{
						break;
					}
				}
				iter_pre = iter_begin_current;
			}
		}
	}

	//最近一个电流时间终点iter_end_current
	bool is_find_end = false;
	if (is_find_begin)
	{
		int balance_count = 0;
		std::deque<tag_st_current_inf*>::reverse_iterator iter_pre = iter_end_current;
		std::deque<tag_st_current_inf*>::reverse_iterator iter_balance = iter_end_current;
		for (iter_end_current = m_currentInf.rbegin(); iter_end_current != iter_begin_current; iter_end_current++)
		{
			tag_st_current_inf* p_current = *iter_end_current;
			if (p_current->endTimeM >= arc_end_m)
			{
				tag_st_current_inf* p_current_pre = *iter_pre;
				if (abs((int)(p_current->currentVal - p_current_pre->currentVal)) <= (int)p_current->currentVal / 10)
				{
					//找到平衡点
					if (++balance_count >= c_balance_count)
					{
						iter_balance = iter_end_current;
					}
				}
				iter_pre = iter_end_current;
			}
			else
			{
				iter_end_current = iter_balance;
				is_find_end = true;
				break;
			}
		}
	}

	if (is_find_begin && is_find_end)
	{
		//iter_begin_current ... iter_end_current
		time_t current_last_time_m = 0;
		for (std::deque<tag_st_current_inf*>::reverse_iterator iter = iter_end_current; (iter != iter_begin_current) && (iter != m_currentInf.rend()); iter++)
		{
			tag_st_current_inf* p_current = *iter;
			current_last_time_m += (p_current->endTimeM - p_current->beginTimeM);
		}

		if (current_last_time_m != 0)
		{
			arc_rate = (double)((arc_last_time_m * 1.0) / (current_last_time_m * 1.0));
		}
	}

	return arc_rate;
}

double CArcNotify::ArcCalcTotal(st_arc_inf* p_arc_inf)
{
	double arc_rate = -1;

	m_arcLastTimeMs += (p_arc_inf->endTimeM - p_arc_inf->beginTimeM);

	for (std::deque<tag_st_current_inf*>::iterator iter = m_currentInf.begin(); iter != m_currentInf.end(); iter++)
	{
		tag_st_current_inf* p_current = *iter;
		m_currentLastTimeMs += (p_current->endTimeM - p_current->beginTimeM);
	}

	if (m_currentLastTimeMs != 0)
	{
		arc_rate = (double)((m_arcLastTimeMs * 1.0) / (m_currentLastTimeMs * 1.0));
	}
	else
	{
		arc_rate = 0;
	}

	return arc_rate;
}





