#include "StdAfx.h"
#include "BsMvb.h"
#include "Log/CmLog.h"
#include "BsMainTask.h"
#include "Mvb/ServerTrans.h"
#include "BsDllMvb.h"
#include "DataBase/CmDBMySql.h"
#include "CmFunction.h"
#include "BsDefStruct.h"
#include "BsGlobal.h"
#include "BsFunction.h"
#include "Serial/CmSerial.h"
#include "PantographMonitor.h"

#define USE_MVB_TEST 1

CMvb::CMvb(void)
{
	m_pCServerTrans = NULL;
	m_mvbState = false;
	m_pCMainTask = NULL;
	m_mvbPos = 1;
	m_pMvbRecvData = new MVB::tag_st_mvb_recv_data;
	memset(m_pMvbRecvData, 0, sizeof(*m_pMvbRecvData));
	m_pMvbAlarmData = new MVB::tag_st_mvb_alarm_data;
	memset(m_pMvbAlarmData, 0, sizeof(*m_pMvbAlarmData));
	m_pDbMysqlParam = NULL;
	m_comScmHand = NULL;
	m_trainNum = 0;
	m_videoMaxSec = 0;
}


CMvb::~CMvb(void)
{
	if (m_pMvbRecvData)
	{
		delete m_pMvbRecvData;
		m_pMvbRecvData = NULL;
	}
	if (m_pMvbAlarmData)
	{
		delete m_pMvbAlarmData;
		m_pMvbAlarmData = NULL;
	}

	ReleaseRes();
}

unsigned int WINAPI CMvb::thread_mvb( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CMvb* p_this = (CMvb*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	MVB::CServerTrans* p_CServerTrans = p_this->m_pCServerTrans;

	const int mvb_commu_delt_tm = 50;
	const int timer_delt_tm = p_this->m_videoMaxSec * 1000;//校时间隔
	const int c_timer_max_count = timer_delt_tm / mvb_commu_delt_tm;//校时间隔次数

	int suc_count = 0;
	while (1)
	{
		Sleep(mvb_commu_delt_tm);
		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		const MVB::st_mvb_recv_data* p_st_mvb_recv_data = p_CServerTrans->GetMvbRecvData();
#if USE_MVB_TEST
#else
		memcpy(p_this->m_pMvbRecvData, p_st_mvb_recv_data, sizeof(*p_this->m_pMvbRecvData));
#endif
		bool mvb_state = p_CServerTrans->GetMvbState();

#if USE_MVB_TEST
		mvb_state = true;
		p_this->m_pMvbRecvData->run_mode = true;
		p_this->m_pMvbRecvData->train_num = p_this->m_trainNum;
#endif

		p_this->SetMvbState(mvb_state);

		p_this->m_pCMainTask->TaskSetPause(!p_this->m_pMvbRecvData->run_mode);
		p_this->m_pCMainTask->TaskSetPause(p_this->m_pMvbRecvData->pantograph_sleep_mode);
		if (p_this->m_pMvbRecvData->shutdown_the_equipment)
		{
			const unsigned char close_send[5] = {0xff, 0x99, 0x00, 0x00, 0x66};
			unsigned char close_recv[5] = {0};
			const int c_send_len = sizeof(close_send);
			const int read_timeout_ms = 1000;
			if (!p_this->m_comScmHand)
			{
				p_this->m_comScmHand = NSP_STP_CM::OpenPort(p_this->m_serialPortScm.c_str(), p_this->m_baudRateScm, EVENPARITY, read_timeout_ms);
			}
			unsigned long send_len_real = 0;
			NSP_STP_CM::Write( p_this->m_comScmHand, close_send, c_send_len, send_len_real);
			if (send_len_real == c_send_len)
			{
				const int c_recv_len = sizeof(close_recv);
				unsigned long recv_len_real = 0;
				NSP_STP_CM::Read(p_this->m_comScmHand, close_recv, c_recv_len, recv_len_real);
				if (recv_len_real == c_recv_len)
				{
					if (0 == memcmp(close_recv, close_send, recv_len_real))
					{
						gLog.Add("%s shutdown_the_equipment MySystemShutdown\n", __FUNCTION__);
						NSP_STP_CM::MySystemShutdown();
					}
				}
			}
		}
		
		//校时
		if (++suc_count > c_timer_max_count)
		{
			suc_count = 0;
			if (p_this->m_pMvbRecvData->date_time_vaild)
			{
				NSP_STP_CM::st_time get_time;
				get_time.year = p_this->m_pMvbRecvData->current_time.tm_year;
				get_time.mon = p_this->m_pMvbRecvData->current_time.tm_mon;
				get_time.day = p_this->m_pMvbRecvData->current_time.tm_mday;
				get_time.hour = p_this->m_pMvbRecvData->current_time.tm_hour;
				get_time.min = p_this->m_pMvbRecvData->current_time.tm_min;
				get_time.sec = p_this->m_pMvbRecvData->current_time.tm_sec;
				NSP_STP_CM::set_local_time(get_time);
			}

			//检测维护模式，运行模式，模式选择提示
			if (p_this->m_pMvbRecvData->maintain_mode)
			{
				const int min_speed = 18 * 100;//18km/h;5m/s;
				if (p_this->m_pMvbRecvData->speed_vaild && p_this->m_pMvbRecvData->speed >= min_speed)
				{
					p_this->m_pMvbAlarmData->choose_mode = true;
				}
			}

#if USE_MVB_TEST
			p_this->m_pMvbRecvData->cur_station = (p_this->m_pMvbRecvData->cur_station + 1) % 100;
#endif
		}

		p_CServerTrans->SetMvbAlarmData(p_this->m_pMvbAlarmData);

	}

	return 0;
}

int CMvb::TaskStart()
{
	USES_CONVERSION;
	int ret = 0;
	m_pCServerTrans = g_pCMvbDll->m_pCServerTrans;

	const int read_timeout_ms = 150 * 5;//读取串口超时
	m_pCServerTrans->SetUartParam(m_serialPortMvb.c_str(), m_baudRateMvb, EVENPARITY, read_timeout_ms, m_mvbPos);

	bool load_frm_db = false;
	if (load_frm_db)
	{
		NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;
		p_CMySqlDB->SetMySqlServerInf(m_pDbMysqlParam->dbHost.c_str(), m_pDbMysqlParam->dbUser.c_str(), m_pDbMysqlParam->dbPass.c_str(), m_pDbMysqlParam->dbName.c_str(), m_pDbMysqlParam->dbPort);

		char sql_cmd[512];
		sprintf(sql_cmd, "select %s, %s, %s, %s, %s, %s from %s ORDER BY %s LIMIT 100", 
			gStationKey.c_str(), gStationFCode.c_str(), gStationFAlias.c_str(), gStationFName.c_str(), gStationFSeq.c_str(), gStationFMileage.c_str(),
			gTblStation.c_str(),
			gStationKey.c_str()
			);
		unsigned long long row_count = 0;
		unsigned int col_count = 0;
		char*** ppp_res = NULL;

		ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
		if (ret != 0)
		{
			delete p_CMySqlDB;
			return -11;
		}

		//id;code;      alias; name;    seq;  mileage;
		//1	 1314851457	DGZ	   窦官站	1	  34.444
		MVB::tag_st_station_info* station_inf_arr = new MVB::tag_st_station_info[(int)row_count];
		for (unsigned long long i = 0; i < row_count; i ++)
		{
			int station_key = 0;
			NSP_STP_CM::str_to_int(ppp_res[i][0], station_key);
			NSP_STP_CM::str_to_int(ppp_res[i][1], station_inf_arr[i].station_code);
			strcpy(station_inf_arr[i].station_alias, ppp_res[i][2]);
			strcpy(station_inf_arr[i].station_name, ppp_res[i][3]);
			NSP_STP_CM::str_to_int(ppp_res[i][4], station_inf_arr[i].station_seq);
			sscanf(ppp_res[i][5], "%lf", &station_inf_arr[i].mileage);
		}

		m_pCServerTrans->SetStationInfArr(station_inf_arr, (int)row_count);

		delete[] station_inf_arr;
		p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
		delete p_CMySqlDB;
	}
	else
	{
		/*
		id;code;alias;name;seq;mileage
		1;1314851457;JLLZ;基隆路站;1;34.444
		2;1314851458;GCLZ;港城路站;2;34.444
		*/

		CString strErrorInfo(_T(""));
		CString cur_path = theApp.GetCurrentPath();

		char file_path[MAX_PATH];
		sprintf(file_path, ("%s\\t_station_info.ini"), W2A(cur_path));

		FILE* pf = fopen(file_path, "rb");
		if (pf)
		{
			MVB::tag_st_station_info* station_inf_arr = new MVB::tag_st_station_info[MVB::MAX_STATION_COUNT];

			fseek(pf, 0, SEEK_END);
			const long c_file_len = ftell(pf);	
			fseek(pf, 0, SEEK_SET);

			const long c_new_len = c_file_len + 1;
			char* new_buf = new char[c_new_len];
			memset(new_buf, 0, sizeof(c_new_len));

			fread(new_buf, c_file_len, sizeof(new_buf[0]), pf);

			const int c_t = '\t';
			const int c_n = '\n';
			const int c_r = '\r';
			char line_data[1024];
			char* p_line_begin = new_buf;
			int arr_ind = 0;
			while(1)
			{
				memset(line_data, 0, sizeof(line_data));
				const char c_sig[] = "\r\n";
				char* p_line_end = strstr(p_line_begin, c_sig);
				if (p_line_end)
				{
					const int c_line_len = p_line_end - p_line_begin;
					memcpy(line_data, p_line_begin, c_line_len);

					/*
					id;code;alias;name;seq;mileage
					1;1314851457;JLLZ;基隆路站;1;34.444
					2;1314851458;GCLZ;港城路站;2;34.444
					*/
					int station_key = 0;

					int res = sscanf(line_data, "%d;%d;%[^;];%[^;];%d;%lf\r" 
						, &station_key
						, &station_inf_arr[arr_ind].station_code
						, station_inf_arr[arr_ind].station_alias
						, station_inf_arr[arr_ind].station_name
						, &station_inf_arr[arr_ind].station_seq
						, &station_inf_arr[arr_ind].mileage
						);
					if (res == 6)
					{
						arr_ind ++;
					}
					p_line_begin = p_line_end + strlen(c_sig);
				}
				else
				{
					break;
				}
			}

			delete[] new_buf;
			fclose(pf);

			if (arr_ind > 0)
			{
				m_pCServerTrans->SetStationInfArr(station_inf_arr, arr_ind);
			}
			delete[] station_inf_arr;
			if (arr_ind == 0)
			{
				return -21;
			}
		}
		else
		{
			return -31;
		}
	}

	m_pCServerTrans->MvbStart(NSP_STP_CM::E_CPU_MASK_3);


	//open scm
	const int read_timeout_scm_ms = 1000;
	if (!m_comScmHand)
	{
		m_comScmHand = NSP_STP_CM::OpenPort(m_serialPortScm.c_str(), m_baudRateScm, EVENPARITY, read_timeout_scm_ms);
	}

	SetThreadCount(3);
	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_mvb", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_mvb, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_scm", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_scm, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 2;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_clear_alarm", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_clear_alarm, p_st_thread_param, CpuMaskRemain());
	}
	return 0;
}

MVB::tag_st_mvb_recv_data* CMvb::GetMvbRecvData()
{
	return m_pMvbRecvData;
}

void CMvb::SetSerialInf(std::string serial_port_mvb, int baud_rate_mvb, std::string serial_port_scm, int baud_rate_scm)
{
	if (serial_port_mvb.empty() || baud_rate_mvb <= 0)
	{
		return;
	}
	if (serial_port_scm.empty() || baud_rate_scm <= 0)
	{
		return;
	}
	m_serialPortMvb = serial_port_mvb;
	m_baudRateMvb = baud_rate_mvb;
	m_serialPortScm = serial_port_scm;
	m_baudRateScm = baud_rate_scm;
}

void CMvb::ReleaseRes()
{

}

void CMvb::SetMvbState( bool state )
{
	m_mvbState = state;
	m_pCMainTask->TaskSetPause(!state);
}

MVB::tag_st_station_info* CMvb::GetCurrentStationInf()
{
	if (!m_pMvbRecvData || !m_pCServerTrans)
	{
		return NULL;
	}

	MVB::tag_st_station_info* p_station_current = GetStationInfByInd(m_pMvbRecvData->cur_station);

	return p_station_current;
}

MVB::tag_st_station_info* CMvb::GetStationInfByInd(int station_ind)
{
	if (!m_pCServerTrans)
	{
		return NULL;
	}

	if (station_ind < 0 || station_ind > MVB::MAX_STATION_COUNT)
	{
		return NULL;
	}

	int station_num = 0;
	MVB::tag_st_station_info* p_STATION_INFO_Arr = m_pCServerTrans->GetStationInfArr(&station_num);
	MVB::tag_st_station_info* p_station_current = &p_STATION_INFO_Arr[station_ind];
	return p_STATION_INFO_Arr;
}

int CMvb::GetCurrentStationInd()
{
	if (!m_pMvbRecvData)
	{
		return -1;
	}

	return m_pMvbRecvData->cur_station;
}

unsigned int WINAPI CMvb::thread_scm( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CMvb* p_this = (CMvb*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int scm_commu_delt_tm = 50;

	int suc_count = 0;
	while (1)
	{
		Sleep(scm_commu_delt_tm);
		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		const unsigned char c_close_cmd[5] = {0xff, 0x49, 0x00, 0x00, 0xB6};
		unsigned char close_recv[5] = {0};
		const int c_recv_len = sizeof(close_recv);

		const int read_timeout_ms = 1000;
		if (!p_this->m_comScmHand)
		{
			p_this->m_comScmHand = NSP_STP_CM::OpenPort(p_this->m_serialPortScm.c_str(), p_this->m_baudRateScm, EVENPARITY, read_timeout_ms);
		}

		unsigned long recv_len_real = 0;
		NSP_STP_CM::Read(p_this->m_comScmHand, close_recv, c_recv_len, recv_len_real);
		if (recv_len_real == c_recv_len)
		{
			if (0 == memcmp(close_recv, c_close_cmd, recv_len_real))
			{
				gLog.Add("%s recv close cmd\n", __FUNCTION__);
				NSP_STP_CM::MySystemShutdown();
			}
		}

	}

	return 0;
}

unsigned int WINAPI CMvb::thread_clear_alarm( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CMvb* p_this = (CMvb*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int c_delay_tm = 50;
	const int c_clear_delt_tm = 5 * 1000;
	const int c_clear_count = c_clear_delt_tm / c_delay_tm;

	int clear_num = 0;
	while (1)
	{
		Sleep(c_delay_tm);
		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		if (++ clear_num > c_clear_count)
		{
			clear_num = 0;

			p_this->m_MvbAlarmDataLock.Lock();

			unsigned short alive_seq = p_this->m_pMvbAlarmData->alive_seq;
			bool selftest_fault = p_this->m_pMvbAlarmData->selftest_fault;
			bool current_sensor = p_this->m_pMvbAlarmData->current_sensor;
			bool car_bottom_compensator = p_this->m_pMvbAlarmData->car_bottom_compensator;
			bool roof_monitoring_equipment = p_this->m_pMvbAlarmData->roof_monitoring_equipment;
			bool monitor_camera_status = p_this->m_pMvbAlarmData->monitor_camera_status;
			bool holder_fault = p_this->m_pMvbAlarmData->holder_fault;
			bool arc_monitoring_module = p_this->m_pMvbAlarmData->arc_monitoring_module;
			bool infrared_monitoring_module = p_this->m_pMvbAlarmData->infrared_monitoring_module;
			bool geometric_parameter_monitoring = p_this->m_pMvbAlarmData->geometric_parameter_monitoring;
			bool data_analysis_server_status = p_this->m_pMvbAlarmData->data_analysis_server_status;
			bool rigid_disk_fault = p_this->m_pMvbAlarmData->rigid_disk_fault;
			bool ipc_fault = p_this->m_pMvbAlarmData->ipc_fault;

			memset(p_this->m_pMvbAlarmData, 0, sizeof(*p_this->m_pMvbAlarmData));

			p_this->m_pMvbAlarmData->alive_seq = alive_seq;
			p_this->m_pMvbAlarmData->selftest_fault = selftest_fault;
			p_this->m_pMvbAlarmData->current_sensor = current_sensor;
			p_this->m_pMvbAlarmData->car_bottom_compensator = car_bottom_compensator;
			p_this->m_pMvbAlarmData->roof_monitoring_equipment = roof_monitoring_equipment;
			p_this->m_pMvbAlarmData->monitor_camera_status = monitor_camera_status;
			p_this->m_pMvbAlarmData->holder_fault = holder_fault;
			p_this->m_pMvbAlarmData->arc_monitoring_module = arc_monitoring_module;
			p_this->m_pMvbAlarmData->infrared_monitoring_module = infrared_monitoring_module;
			p_this->m_pMvbAlarmData->geometric_parameter_monitoring = geometric_parameter_monitoring;
			p_this->m_pMvbAlarmData->data_analysis_server_status = data_analysis_server_status;
			p_this->m_pMvbAlarmData->rigid_disk_fault = rigid_disk_fault;
			p_this->m_pMvbAlarmData->ipc_fault = ipc_fault;

			p_this->m_MvbAlarmDataLock.Unlock();
		}
	}

	return 0;
}

