#include "StdAfx.h"
#include "BsMonitor.h"
#include "CmFunction.h"
#include "Log/CmLog.h"
#include "BsMvb.h"
#include "Mvb/ServerTrans.h"
#include "DataBase/CmDBMySql.h"
#include "BsDefStruct.h"
#include "BsGlobal.h"
#include "CamVideo/NVCTRDefines.h"
#include "CamVideo/VideoObjectDevSer.h"
#include "BsFunction.h"
#include "BsDllCamVideo.h"

CMonitor::CMonitor(void)
{
	m_pAbstractDeviceInfo = new VideoNSP::VideoDeviceInfo;
	m_pAbstractFormatInfo = new VideoNSP::VideoDeviceFormatInfo;

	m_presetNum = 0;
	m_pantoPos = 0;
	m_presetReset = false;
	m_presetNo = 0;
	m_runSpare = false;
	m_firstRun = true;
}


CMonitor::~CMonitor(void)
{

}

void CMonitor::SetDevInf( tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo)
{
	if (!p_AbstractDeviceInfo || !p_AbstractFormatInfo)
	{
		return;
	}
	*(dynamic_cast<VideoNSP::VideoDeviceInfo*>(m_pAbstractDeviceInfo)) = *(dynamic_cast<VideoNSP::VideoDeviceInfo*>(p_AbstractDeviceInfo));
	*(dynamic_cast<VideoNSP::VideoDeviceFormatInfo*>(m_pAbstractFormatInfo)) = *(dynamic_cast<VideoNSP::VideoDeviceFormatInfo*>(p_AbstractFormatInfo));

	m_pAbstractFormatInfo->pReceiveData = NULL;
	m_pAbstractFormatInfo->pParam = NULL;
	m_pAbstractFormatInfo->pHeartbeatConnectedCallback = &CMonitor::heart_beat_cb;
	m_pAbstractFormatInfo->pHeartbeatParam = this;
	(dynamic_cast<VideoNSP::VideoDeviceFormatInfo*>(m_pAbstractFormatInfo))->pReceiveData2 = NULL;
	(dynamic_cast<VideoNSP::VideoDeviceFormatInfo*>(m_pAbstractFormatInfo))->pParam2 = NULL;
}

int CMonitor::Control(int cmd_type, const char* cmd)
{
	int ret = -1;
	m_videoCtrLock.Lock();
	ret = (dynamic_cast<VideoNSP::CVideoObjectDevice*>(m_pCObjectDevice))->setOption(cmd_type, cmd);
	m_videoCtrLock.Unlock();
	return ret;
}

unsigned int WINAPI CMonitor::thread_pull( void* pParam)
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CMonitor* p_this = (CMonitor*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;

	const int mvb_err_wait_m = 10 * 1000;
	int ret = 0;
	int pre_station = -1;
	int retry_count = 0;
	std::string current_down_path;

	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	std::string tbl_record;
	int carriage_num = 0;
	if (p_this->m_cameraInd == E_CAM_CLASS_MONITOR_SPARE)
	{
		tbl_record = gTblRecordSpare;
		carriage_num = GetSpareCarrigeNumByCurrent(p_this->m_carriageNum);
	}
	else
	{
		tbl_record = gTblRecord;
		carriage_num = p_this->m_carriageNum;
	}

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

		if (p_this->m_cameraInd == E_CAM_CLASS_MONITOR_SPARE)
		{
			//判断是否暂停
			if (!p_this->m_runSpare)
			{
				pre_station = -1;
				continue;
			}
		}

		bool state_well = p_this->m_pCMvb->GetMvbState();
		if (state_well)
		{
			MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();
			MVB::tag_st_station_info* p_station_now = p_this->m_pCMvb->GetCurrentStationInf();

			int cur_station = p_st_mvb_data->cur_station;
			if (cur_station != pre_station)
			{
				if (pre_station != -1)
				{
					//pre down suc
					std::string pre_down_path = current_down_path;
					if (!pre_down_path.empty())
					{
						//更新视频结束时间
						long long record_id = -1;
						sprintf(sql_cmd, "select %s from %s where  %s = '%s' and %s > %s LIMIT 1;", 
							gRecordFKey.c_str(),
							tbl_record.c_str(), 
							gRecordFFullName.c_str(), (NSP_STP_CM::str_sig_double(pre_down_path, '\\')).c_str(),
							gRecordFBeginTime.c_str(), gDbTimeOneDayAgo.c_str()
							);
						unsigned long long row_count = 0;
						unsigned int col_count = 0;
						char*** ppp_res = NULL;
						ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
						if (ret == 0)
						{
							NSP_STP_CM::str_to_int(ppp_res[0][0], record_id);

							p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
						}
						else
						{
							gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
						}

						if (record_id > 0)
						{
							sprintf(sql_cmd, "update  %s  set %s = %s where %s = %lld;"
								, tbl_record.c_str(), 
								gRecordFEndTime.c_str(), gDbTimeNow.c_str(),
								gRecordFKey.c_str(), record_id
								);
							ret = p_CMySqlDB->WriteDataBase(sql_cmd);
							if (ret != 0)
							{
								gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
							}
						}
					}
				}


				//next video
				current_down_path = p_this->GetDownFullPath(true, p_this->m_cameraInd);
				std::string folder_path = NSP_STP_CM::str_right_cut_tag(current_down_path, "\\");
				if (!folder_path.empty())
				{
					NSP_STP_CM::folder_create(folder_path);
				}

				bool suc_play = p_this->PlayNextCam(current_down_path);
				if (suc_play)
				{
					pre_station = cur_station;
					gLog.Add("%s current_down_path=%s\n", __FUNCTION__, current_down_path.c_str());

					st_db_record db_record;
					db_record.CameraPos = p_this->m_pantoPos;
					db_record.CarriageId = p_st_mvb_data->train_num;
					db_record.CarriageNum = carriage_num;
					db_record.LineId = p_this->m_lineId;
					db_record.StationId = p_station_now->station_seq;
					db_record.Direction = p_st_mvb_data->direction;
					db_record.FileName = NSP_STP_CM::str_left_cut_tag( current_down_path, "\\");
					db_record.FullName = current_down_path;
					db_record.BeginTime = gDbTimeNow;

					//插入视频
					sprintf(sql_cmd, "insert into  %s  ( %s, %s,  %s, %s, %s,  %s,  %s,  %s,  %s) \
									 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %s);",
									 tbl_record.c_str(), 
									 gRecordFCameraPos.c_str(), gRecordFCarriageId.c_str(), gRecordFCarriageNum.c_str(), gRecordFLineId.c_str(), 
									 gRecordFStationId.c_str(), gRecordFDirection.c_str(),  gRecordFFileName.c_str(), gRecordFFullName.c_str(),
									 gRecordFBeginTime.c_str(),
									 db_record.CameraPos, db_record.CarriageId, db_record.CarriageNum, db_record.LineId, 
									 db_record.StationId , db_record.Direction, db_record.FileName.c_str(), (NSP_STP_CM::str_sig_double(db_record.FullName, '\\')).c_str(),
									 db_record.BeginTime.c_str());
					ret = p_CMySqlDB->WriteDataBase(sql_cmd);
					if (ret != 0)
					{
						gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
				}
				else
				{
					//continue play
					pre_station = -1;
				}
			}
			else
			{
				//keep playing
				p_this->PlayCam(NSP_STP_CM::E_CPU_MASK_2);
			}
			retry_count = 0;
		}
		else
		{
			//mvb err
			retry_count ++;
			if (retry_count > mvb_err_wait_m / sleep_m)
			{
				retry_count = 0;
				p_this->StopCam();
				pre_station = -1;
			}
		}

	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

unsigned int WINAPI CMonitor::thread_control( void* pParam)
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CMonitor* p_this = (CMonitor*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;
	const int preset_delay_m = 2 * 60 * 1000;//2min
	const int video_text_delay_m = 5 * 1000;
	const int c_preset_wait_count = preset_delay_m / sleep_m;
	const int c_video_text_count = video_text_delay_m / sleep_m;
	int preset_wait_times = 0;
	int video_text_wait_times = 0;
	while(1)
	{
		preset_wait_times ++;
		video_text_wait_times ++;
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

		bool state_well = p_this->m_pCMvb->GetMvbState();
		if (state_well)
		{
			MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();

			if(video_text_wait_times >= c_video_text_count)
			{
				video_text_wait_times = 0;

				//校时			
				NSP_STP_CM::tag_st_time cur_time;
				NSP_STP_CM::GetNowTime(&cur_time);
				int tm_year = cur_time.year;
				int tm_mon = cur_time.mon;
				int tm_mday = cur_time.day;
				int tm_hour = cur_time.hour;
				int tm_min = cur_time.min;
				int tm_sec = cur_time.sec;
				int tm_ms = cur_time.ms;
				Msg_TimingInfo msg_TimingInfo;
				TimeInfo TimingInfo;
				NSP_STP_CM::Dec2Hex(TimingInfo.Year, 2, (int)tm_year);
				TimingInfo.Month = (int)tm_mon;
				TimingInfo.Day = (int)tm_mday;
				TimingInfo.Hour = (int)tm_hour;
				TimingInfo.Minute = (int)tm_min;
				TimingInfo.Second = (int)tm_sec;
				memcpy(&msg_TimingInfo.dateTime, &TimingInfo, sizeof(TimingInfo));
				if (p_this->m_firstRun)
				{
					p_this->m_firstRun = false;
					//p_this->OptionTimer(msg_TimingInfo);
				}

				//字符叠加
				Video_ADDTextInfo add_TextInfo;
				memset(&add_TextInfo, 0x00, sizeof(add_TextInfo));
				char train_number[8] = {0};
				sprintf(train_number, "%d", p_st_mvb_data->train_num);
				memcpy(add_TextInfo.TrainNumber, train_number, 7);
				//
				char carriage_num[16] = {0};
				sprintf(carriage_num, "%d%d", p_st_mvb_data->train_num, p_this->m_carriageNum);
				char CarriageNumber[8];//replace with struct member
				memcpy(CarriageNumber, carriage_num, 8);
				//
				add_TextInfo.CarriageNumber = p_this->m_carriageNum;
				unsigned int speed = (p_st_mvb_data->speed) * 100;//1km/h * 100
				NSP_STP_CM::Dec2Hex(add_TextInfo.Speed, 2, speed);
				add_TextInfo.PantoPos = p_this->m_pantoPos;
				p_this->OptionAddText(add_TextInfo);
			}

			//预置位设置
			if(p_this->m_presetNum > 0)
			{
				if(preset_wait_times >= c_preset_wait_count)
				{
					preset_wait_times = 0;
					if (p_this->m_presetReset)
					{
						if (p_this->m_presetNo != 0)
						{
							p_this->m_presetNo = 0;

							Msg_PresetInfo PresetInfo;
							PresetInfo.ControlType = 0x02;
							PresetInfo.PresetNum = 1 + (p_this->m_presetNo % p_this->m_presetNum);
							PresetInfo.PantoPos = p_this->m_pantoPos;
							p_this->OptionPreset(PresetInfo);
						}
						else
						{
							//do nothing
						}
					}
					else
					{
						p_this->m_presetNo ++;

						//add
						p_this->m_presetNo = 0;
						//add

						Msg_PresetInfo PresetInfo;
						PresetInfo.ControlType = 0x02;
						PresetInfo.PresetNum = 1 + (p_this->m_presetNo % p_this->m_presetNum);
						PresetInfo.PantoPos = p_this->m_pantoPos;
						p_this->OptionPreset(PresetInfo);
					}
				}
			}

			if (0)
			{
				//相机开关
				Msg_CameraSwitchInfo CameraSwitchInfo;
				CameraSwitchInfo.ControlType = 1;	
				CameraSwitchInfo.PantoPos = p_this->m_pantoPos;
				p_this->OptionCameraSwitch(CameraSwitchInfo);

				//补光灯开关
				Msg_PantoLedSwitchInfo PantoLedSwitchInfo;
				PantoLedSwitchInfo.ControlType = 1;
				PantoLedSwitchInfo.PantoPos = p_this->m_pantoPos;
				p_this->OptionLedSwitch(PantoLedSwitchInfo);

				//云台控制
				Msg_PtzControlInfo PtzControlInfo;
				PtzControlInfo.ControlType = 0x01;
				PtzControlInfo.MoveType = 1;
				PtzControlInfo.PantoPos = p_this->m_pantoPos;
				p_this->OptionPtz(PtzControlInfo);
			}

		}
	}
	return 0;
}

int CMonitor::TaskStart()
{
	int ret = CameraVideoInit();
	if (ret != 0)
	{
		return -1;
	}

	if (m_cameraInd == E_CAM_CLASS_MONITOR_SPARE)
	{
		SetThreadCount(1);
		{
			int thread_ind = 0;
			NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
			memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
			p_st_thread_param->pParam = this;
			p_st_thread_param->threadInd = thread_ind;
			sprintf(p_st_thread_param->threadName, "%s thread_pull", __FUNCTION__);
			thread_run(&m_pCThreadArr[thread_ind], thread_pull, p_st_thread_param, CpuMaskRemain());
		}
	}
	else
	{
		SetThreadCount(2);
		{
			int thread_ind = 0;
			NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
			memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
			p_st_thread_param->pParam = this;
			p_st_thread_param->threadInd = thread_ind;
			sprintf(p_st_thread_param->threadName, "%s thread_pull", __FUNCTION__);
			thread_run(&m_pCThreadArr[thread_ind], thread_pull, p_st_thread_param, CpuMaskRemain());
		}

		{
			int thread_ind = 1;
			NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
			memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
			p_st_thread_param->pParam = this;
			p_st_thread_param->threadInd = thread_ind;
			sprintf(p_st_thread_param->threadName, "%s thread_control", __FUNCTION__);
			thread_run(&m_pCThreadArr[thread_ind], thread_control, p_st_thread_param, CpuMaskRemain());
		}
	}

	return 0;
}

bool CMonitor::PlayNextCam(std::string download_path)
{
	bool b_ret = false;
	bool is_playing = false;
	int ret = -1;

	if (!m_pCObjectDevice)
	{
		return false;
	}

	ret = (dynamic_cast<VideoNSP::CVideoObjectDevice*>(m_pCObjectDevice))->playNext(NSP_STP_CM::E_CPU_MASK_2, download_path);


	b_ret = (0 == ret);
	return b_ret;
}

void CALLBACK CMonitor::heart_beat_cb( const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader )
{
	CMonitor* p_this = (CMonitor*)pParam;
	if (p_this == NULL)
	{
		return;
	}

	p_this->m_cameraHeartBeat = (unsigned int)(p_this->m_cameraHeartBeat + 1);
}

void CMonitor::OptionTimer( Msg_TimingInfo timing_Info )
{
	Control(VideoNSP::VIDEO_COMMAND_TIMER, (const char*)&timing_Info);
}

void CMonitor::OptionAddText( Video_ADDTextInfo video_ADDTextInfo )
{
	Control(VideoNSP::VIDEO_COMMAND_TEXT, (const char*)&video_ADDTextInfo);
}

void CMonitor::OptionPreset( Msg_PresetInfo preset_Info )
{
	Control(VideoNSP::VIDEO_COMMAND_PRESET, (const char*)&preset_Info);
}

void CMonitor::OptionCameraSwitch( Msg_CameraSwitchInfo camera_SwitchInfo )
{
	Control(VideoNSP::VIDEO_COMMAND_CAMERA, (const char*)&camera_SwitchInfo);
}

void CMonitor::OptionLedSwitch( Msg_PantoLedSwitchInfo led_switchInfo )
{
	Control(VideoNSP::VIDEO_COMMAND_LIGHT, (const char*)&led_switchInfo);
}

void CMonitor::OptionPtz( Msg_PtzControlInfo ptz_ControlInfo )
{
	Control(VideoNSP::VIDEO_COMMAND_PLZ, (const char*)&ptz_ControlInfo);
}

int CMonitor::HandlePisCmd( tag_st_pis_pack_inf* p_pis_pack_inf, bool& is_wait_heart_beat, std::vector<tag_st_pis_pack_inf>* p_vec_send_pack )
{
	TCP_HEADER pis_head;
	const int c_head_len = sizeof(pis_head);
	memcpy((char*)&pis_head, p_pis_pack_inf->pData, c_head_len);

	const int c_data_size = NSP_STP_CM::Hex2Dec(pis_head.DataLenth, 2);
	const int c_frame_len = c_data_size + 5;
	const BYTE c_frame_type = pis_head.FrameType;

	if (c_frame_len != p_pis_pack_inf->iDataLength || pis_head.HEARD != 0xff)
	{
		return -1;
	}

	const int c_send_buf_len = 1024 * 2;
	char* send_buf = new char[c_send_buf_len];

	int ret = 0;
	switch(c_frame_type)
	{
		////////common
	case TX_MSG_HEARTBEAT_FEEDBACK://客户端反馈心跳
		{
			is_wait_heart_beat = false;
		}
		break;
	case TX_MSG_PTZ_CONTROL://PTZ控制报文
		{
			Msg_PtzControlInfo PtzControlInfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &PtzControlInfo);
			if (PtzControlInfo.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				OptionPtz(PtzControlInfo);
				ret = 1;
			}

			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_PTZ_CONTROL_FEEDBACK;		
			const int body_len = 1;
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
			char data_body[2] = {0};
			data_body[0] = ret;
			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
		////////end common
		////////pis
	case TX_MSG_PANTOLED_SWITCH://受电弓闪光灯开关
		{
			Msg_PantoLedSwitchInfo PantoLedSwitchInfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &PantoLedSwitchInfo);
			if (PantoLedSwitchInfo.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				OptionLedSwitch(PantoLedSwitchInfo);
				ret = 1;
			}
			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_PANTOLED_SWITCH_FEEDBACK;
			const int body_len = 1;
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
			char data_body[2] = {0};
			data_body[0] = ret;
			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_PTZ_CRUISE_DATA://预置位轮询		
		{
			Msg_PresetPos preset_inf;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &preset_inf);
			if (preset_inf.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				SetPresetReset(false);
				ret = 1;
			}

			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_PTZ_CRUISE_RTDATA;
			const int body_len = 1;
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
			char data_body[2] = {0};
			data_body[0] = ret;
			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_PTZ_REST_DATA://预置位复位	
		{
			Msg_PresetPos preset_inf;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &preset_inf);
			if (preset_inf.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				SetPresetReset(true);
				ret = 1;
			}

			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_PTZ_REST_RTDATA;
			const int body_len = 1;
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
			char data_body[2] = {0};
			data_body[0] = ret;
			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_ALARM_LIST_DATA://故障列表查询 		
		{
			Msg_PresetPos preset_inf;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &preset_inf);
			if (preset_inf.PantoPos == m_pantoPos)
			{
				std::vector<st_pis_pack_inf> vec_alarm;
				vec_alarm.clear();
				GetAlarmMessageList(&vec_alarm);
				for (unsigned int i = 0; i < vec_alarm.size(); i ++)
				{
					st_pis_pack_inf alarm_in_vec = vec_alarm[i];
					const int body_len = alarm_in_vec.iDataLength;
					char* data_body = alarm_in_vec.pData;

					TCP_HEADER pis_head;
					pis_head.HEARD = 0xFF;
					pis_head.FrameType = TX_MSG_ALARM_LIST_RTDATA;
					NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
					memset(send_buf, 0, c_send_buf_len);
					const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
					if(frame_len > 0)
					{
						st_pis_pack_inf pack_inf;
						pack_inf.iDataLength = frame_len;
						pack_inf.pData = new char[pack_inf.iDataLength];
						memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
						(*p_vec_send_pack).push_back(pack_inf);
					}

					delete[] alarm_in_vec.pData;
				}
				vec_alarm.clear();
			}
		}
		break;
		////////end pis
		////////ptu
	case TX_MSG_ADDTEXT:				//视频叠加字符报文
		{
			_Video_ADDTextInfo video_addtextinfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &video_addtextinfo);
			if (video_addtextinfo.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				OptionAddText(video_addtextinfo);
				ret = 1;
			}
		}
		break;
	case TX_MSG_TIMING:					//校时报文
		{
			_Msg_TimingInfo msg_timinginfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &msg_timinginfo);
			OptionTimer(msg_timinginfo);
			ret = 1;

			NSP_STP_CM::st_time s_time;
			s_time.year = NSP_STP_CM::Hex2Dec(msg_timinginfo.dateTime.Year, 2);
			s_time.mon = msg_timinginfo.dateTime.Month;
			s_time.day = msg_timinginfo.dateTime.Day;
			s_time.hour = msg_timinginfo.dateTime.Hour;
			s_time.min = msg_timinginfo.dateTime.Minute;
			s_time.sec = msg_timinginfo.dateTime.Second;
			NSP_STP_CM::set_local_time(s_time);
		}
		break;
	case TX_MSG_STATE_QUERY:			//NVR状态查询报文
		{
			_Msg_StateQueryFeedBackInfo msg_statequeryfeedbackinfo;
			msg_statequeryfeedbackinfo.CarriageNumber = m_carriageNum;
			msg_statequeryfeedbackinfo.DerviceType = 0x01;//受电弓视频监控服务器为 01；智能分析主机为 02
			strcpy((char*)msg_statequeryfeedbackinfo.Factory, "NEWVISION");
			NSP_STP_CM::Dec2Hex(msg_statequeryfeedbackinfo.Version, 2, 110);

			LPCSTR pszDrive = m_rootPath.c_str();
			unsigned int total_g;
			unsigned int used_g;
			NSP_STP_CM::MyGetDiskFreeSpaceEx( pszDrive, total_g, used_g);
			NSP_STP_CM::Dec2Hex(msg_statequeryfeedbackinfo.Harddiskcapacity, 2, total_g);
			NSP_STP_CM::Dec2Hex(msg_statequeryfeedbackinfo.HarddiskcapacityUsed, 2, used_g);

			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_STATE_FEEDBACK;
			const int body_len = sizeof(msg_statequeryfeedbackinfo);
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);

			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, (char*)&msg_statequeryfeedbackinfo);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_IPC_STATE_QUERY:       //IPC状态查询报文
		{
			_Msg_IpcStateQueryFeedBackInfo msg_ipcstatequeryfeedbackinfo;
			msg_ipcstatequeryfeedbackinfo.CarriageNumber = m_carriageNum;
			msg_ipcstatequeryfeedbackinfo.DervicePosition = m_pantoPos;
			if (m_cameraOnline)
			{
				msg_ipcstatequeryfeedbackinfo.DerviceOnline = 1;
			}
			else
			{
				msg_ipcstatequeryfeedbackinfo.DerviceOnline = 0;
			}
			msg_ipcstatequeryfeedbackinfo.DerviceType = 0x03;//受电弓摄像机：03
			strcpy((char*)msg_ipcstatequeryfeedbackinfo.Factory, "NEWVISION");
			NSP_STP_CM::Dec2Hex(msg_ipcstatequeryfeedbackinfo.Version, 2, 110);

			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_IPC_STATE_QUERY_FEEDBACK;
			const int body_len = sizeof(msg_ipcstatequeryfeedbackinfo);
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);

			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, (char*)&msg_ipcstatequeryfeedbackinfo);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_SEARCH_VIDEO:			//远程搜索视频报文
		{
			_Msg_SearchVideoInfo msg_searchvideoinfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &msg_searchvideoinfo);
			if (msg_searchvideoinfo.DervicePosition == m_pantoPos)
			{
				std::vector<st_pis_pack_inf> vec_video_full_name;
				vec_video_full_name.clear();
				GetVideoRecordList(msg_searchvideoinfo, &vec_video_full_name);
				for (unsigned int i = 0; i < vec_video_full_name.size(); i ++)
				{
					st_pis_pack_inf video_in_vec = vec_video_full_name[i];
					const int body_len = video_in_vec.iDataLength;
					char* data_body = video_in_vec.pData;

					TCP_HEADER pis_head;
					pis_head.HEARD = 0xFF;
					pis_head.FrameType = TX_MSG_SEARCH_VIDEO_FEEDBACK;
					NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
					memset(send_buf, 0, c_send_buf_len);
					const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
					if(frame_len > 0)
					{
						st_pis_pack_inf pack_inf;
						pack_inf.iDataLength = frame_len;
						pack_inf.pData = new char[pack_inf.iDataLength];
						memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
						(*p_vec_send_pack).push_back(pack_inf);
					}

					delete[] video_in_vec.pData;
				}
				vec_video_full_name.clear();
			}
		}
		break;
	case TX_MSG_PRESET:					//预置位
		{
			_Msg_PresetInfo msg_presetinfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &msg_presetinfo);
			if (msg_presetinfo.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				OptionPreset(msg_presetinfo);
				ret = 1;
			}

			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_PRESET_FEEDBACK;
			const int body_len = 1;
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
			char data_body[2] = {0};
			data_body[0] = ret;
			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_CAMERA_SWITCH:			//受电弓摄像机开关
		{
			_Msg_CameraSwitchInfo msg_cameraswitchinfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &msg_cameraswitchinfo);
			if (msg_cameraswitchinfo.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				OptionCameraSwitch(msg_cameraswitchinfo);
				ret = 1;
			}


			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_CAMERA_SWITCH_FEEDBACK;
			const int body_len = 1;
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
			char data_body[2] = {0};
			data_body[0] = ret;
			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_IPC_LIGHT_DATA:			//受电弓摄像机补光灯报文
		{
			_Msg_PantoLedSwitchInfo msg_pantoledswitchinfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &msg_pantoledswitchinfo);
			if (msg_pantoledswitchinfo.PantoPos != m_pantoPos)
			{
				ret = 0;
			}
			else
			{
				OptionLedSwitch(msg_pantoledswitchinfo);
				ret = 1;
			}

			TCP_HEADER pis_head;
			pis_head.HEARD = 0xFF;
			pis_head.FrameType = TX_MSG_IPC_LIGHT_RTDATA;
			const int body_len = 1;
			NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
			char data_body[2] = {0};
			data_body[0] = ret;
			memset(send_buf, 0, c_send_buf_len);
			const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
			if(frame_len > 0)
			{
				st_pis_pack_inf pack_inf;
				pack_inf.iDataLength = frame_len;
				pack_inf.pData = new char[pack_inf.iDataLength];
				memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
				(*p_vec_send_pack).push_back(pack_inf);
			}
		}
		break;
	case TX_MSG_SEARCH_JPG:				//远程搜索图片
		{
			_Msg_SearchJpgInfo msg_searchjpginfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &msg_searchjpginfo);
			if (msg_searchjpginfo.DervicePosition == m_pantoPos)
			{
				std::vector<st_pis_pack_inf> pic_full_name;
				pic_full_name.clear();
				GetAlarmPictureList(msg_searchjpginfo, &pic_full_name);
				for (unsigned int i = 0; i < pic_full_name.size(); i ++)
				{
					st_pis_pack_inf pic_in_vec = pic_full_name[i];
					const int body_len = pic_in_vec.iDataLength;
					char* data_body = pic_in_vec.pData;

					TCP_HEADER pis_head;
					pis_head.HEARD = 0xFF;
					pis_head.FrameType = TX_MSG_SEARCH_JPG_FEEDBACK;
					NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
					memset(send_buf, 0, c_send_buf_len);
					const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
					if(frame_len > 0)
					{
						st_pis_pack_inf pack_inf;
						pack_inf.iDataLength = frame_len;
						pack_inf.pData = new char[pack_inf.iDataLength];
						memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
						(*p_vec_send_pack).push_back(pack_inf);
					}

					delete[] pic_in_vec.pData;
				}
				pic_full_name.clear();
			}
		}
		break;
	case TX_MSG_ALARM_VIDEO_DATA:		//远程搜索视频报文(故障视频)
		{
			_Msg_SearchVideoInfo msg_searchvideoinfo;
			ParsePisFrame(p_pis_pack_inf->pData, p_pis_pack_inf->iDataLength, &msg_searchvideoinfo);
			if (msg_searchvideoinfo.DervicePosition == m_pantoPos)
			{
				std::vector<st_pis_pack_inf> pic_video_full_name;
				pic_video_full_name.clear();
				GetAlarmVideoList(msg_searchvideoinfo, &pic_video_full_name);
				for (unsigned int i = 0; i < pic_video_full_name.size(); i ++)
				{
					st_pis_pack_inf pic_video_in_vec = pic_video_full_name[i];
					const int body_len = pic_video_in_vec.iDataLength;
					char* data_body = pic_video_in_vec.pData;

					TCP_HEADER pis_head;
					pis_head.HEARD = 0xFF;
					pis_head.FrameType = TX_MSG_ALARM_VIDEO_RTDATA;
					NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
					memset(send_buf, 0, c_send_buf_len);
					const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
					if(frame_len > 0)
					{
						st_pis_pack_inf pack_inf;
						pack_inf.iDataLength = frame_len;
						pack_inf.pData = new char[pack_inf.iDataLength];
						memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
						(*p_vec_send_pack).push_back(pack_inf);
					}

					delete[] pic_video_in_vec.pData;
				}
				pic_video_full_name.clear();
			}
		}
		break;
		///////end ptu
	default:
		break;
	}

	delete[] send_buf;
	return 0;
}

int CMonitor::GetAlarmMessageList( std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf )
{
	const unsigned int alarm_count_max = 100;
	const int frame_max_len = 1024;
	const int frame_body_max_len = frame_max_len - 7;
	int ret = 0;
	int pack_ind = 0;//分包序号

	int frame_body_len = 0;//当前帧数据长度，不含头尾
	char frame_body[frame_body_max_len];
	memset(frame_body, 0, sizeof(frame_body));

	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	sprintf(sql_cmd, "select %s from %s where  %s >= %s and %s < %s LIMIT 32;", 
		gAlarmFAlarmMessage.c_str(),
		gTblAlarm.c_str(), 
		gAlarmFBeginTime.c_str(), gDbTimeTodayBegin.c_str(),
		gAlarmFBeginTime.c_str(), gDbTimeTodayEnd.c_str()
		);
	unsigned long long row_count = 0;
	unsigned int col_count = 0;
	char*** ppp_res = NULL;
	ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
	if (ret == 0)
	{
		//第一帧
		frame_body_len = 0;
		memset(frame_body, 0, sizeof(frame_body));
		frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
		frame_body[frame_body_len++] = pack_ind++;

		for (unsigned long long i = 0; i < row_count; i ++)
		{
			char* alarm_msg =  ppp_res[i][0];
			const int alarm_msg_len = strlen(alarm_msg);

			if (frame_body_len + alarm_msg_len <= frame_body_max_len)
			{
				//一帧数据
				memcpy(frame_body + frame_body_len, alarm_msg, alarm_msg_len);
				frame_body_len += alarm_msg_len;
			}
			else
			{
				//超过一帧
				const int body_remain_len = frame_body_max_len - frame_body_len;
				memcpy(frame_body + frame_body_len, alarm_msg, body_remain_len);
				frame_body_len += body_remain_len;

				unsigned int alarm_num = (*p_vec_pack_inf).size();
				if(alarm_num <= alarm_count_max)
				{
					//存入容器
					st_pis_pack_inf alarm_pack;
					alarm_pack.iDataLength = frame_body_max_len;
					alarm_pack.pData = new char[alarm_pack.iDataLength];
					memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
					(*p_vec_pack_inf).push_back(alarm_pack);
				}
				else
				{
					break;
				}

				//下一帧
				frame_body_len = 0;
				memset(frame_body, 0, sizeof(frame_body));
				frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
				frame_body[frame_body_len++] = pack_ind++;
				const int alarm_msg_remain_len = alarm_msg_len - body_remain_len;
				memcpy(frame_body + frame_body_len, alarm_msg + body_remain_len, alarm_msg_remain_len);
				frame_body_len += alarm_msg_remain_len;
			}
		}

		unsigned int alarm_num = (*p_vec_pack_inf).size();
		if(alarm_num <= alarm_count_max)
		{
			//有未满一帧
			if(frame_body_len > 0)
			{
				//存入容器
				st_pis_pack_inf alarm_pack;
				alarm_pack.iDataLength = frame_body_len;
				alarm_pack.pData = new char[alarm_pack.iDataLength];
				memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
				(*p_vec_pack_inf).push_back(alarm_pack);
			}
		}

		p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
	}
	else
	{
		//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

int CMonitor::GetVideoRecordList( _Msg_SearchVideoInfo SearchInfo, std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf )
{
	const unsigned int vec_count_max = 1024;
	const int frame_max_len = 1024;
	const int frame_body_max_len = frame_max_len - 7;
	int ret = 0;
	int pack_ind = 0;//分包序号

	int frame_body_len = 0;//当前帧数据长度，不含头尾
	char frame_body[frame_body_max_len];
	memset(frame_body, 0, sizeof(frame_body));

	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	char begin_time[20] = {'\0'};
	char end_time[20] = {'\0'};
	sprintf(begin_time, "%04d-%02d-%02d %02d:%02d:%02d",
		NSP_STP_CM::Hex2Dec(SearchInfo.dateTimeBegin.Year, 2), SearchInfo.dateTimeBegin.Month, SearchInfo.dateTimeBegin.Day, 
		SearchInfo.dateTimeBegin.Hour, SearchInfo.dateTimeBegin.Minute,SearchInfo.dateTimeBegin.Second);
	sprintf(end_time, "%04d-%02d-%02d %02d:%02d:%02d",
		NSP_STP_CM::Hex2Dec(SearchInfo.dateTimeEnd.Year, 2), SearchInfo.dateTimeEnd.Month, SearchInfo.dateTimeEnd.Day, 
		SearchInfo.dateTimeEnd.Hour, SearchInfo.dateTimeEnd.Minute,SearchInfo.dateTimeEnd.Second );
	
	sprintf(sql_cmd, "select %s from %s where  %s >= '%s' and %s < '%s' and %s = %d and %s = %d and %s is not NULL \
					 and %s is not NULL LIMIT 1024;", 
		gRecordFFullName.c_str(),
		gTblRecord.c_str(), 
		gRecordFBeginTime.c_str(), begin_time, 
		gRecordFBeginTime.c_str(), end_time,
		gRecordFCarriageNum.c_str(), SearchInfo.CarriageNumber,
		gRecordFCameraPos.c_str(), SearchInfo.DervicePosition,
		gRecordFEndTime.c_str(),
		gRecordFFullName.c_str()
		);
	unsigned long long row_count = 0;
	unsigned int col_count = 0;
	char*** ppp_res = NULL;
	ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
	if (ret == 0)
	{
		//第一帧
		frame_body_len = 0;
		memset(frame_body, 0, sizeof(frame_body));
		frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
		frame_body[frame_body_len++] = pack_ind++;

		for (unsigned long long i = 0; i < row_count; i ++)
		{
			char* video_full_name =  ppp_res[i][0];
			const int video_full_name_len = strlen(video_full_name);

			if (frame_body_len + video_full_name_len <= frame_body_max_len)
			{
				//一帧数据
				memcpy(frame_body + frame_body_len, video_full_name, video_full_name_len);
				frame_body_len += video_full_name_len;
			}
			else
			{
				//超过一帧
				const int body_remain_len = frame_body_max_len - frame_body_len;
				memcpy(frame_body + frame_body_len, video_full_name, body_remain_len);
				frame_body_len += body_remain_len;

				unsigned int vec_num = (*p_vec_pack_inf).size();
				if(vec_num <= vec_count_max)
				{
					//存入容器
					st_pis_pack_inf alarm_pack;
					alarm_pack.iDataLength = frame_body_max_len;
					alarm_pack.pData = new char[alarm_pack.iDataLength];
					memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
					(*p_vec_pack_inf).push_back(alarm_pack);
				}
				else
				{
					break;
				}

				//下一帧
				frame_body_len = 0;
				memset(frame_body, 0, sizeof(frame_body));
				frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
				frame_body[frame_body_len++] = pack_ind++;
				const int alarm_msg_remain_len = video_full_name_len - body_remain_len;
				memcpy(frame_body + frame_body_len, video_full_name + body_remain_len, alarm_msg_remain_len);
				frame_body_len += alarm_msg_remain_len;
			}
		}

		unsigned int alarm_num = (*p_vec_pack_inf).size();
		if(alarm_num <= vec_count_max)
		{
			//有未满一帧
			if(frame_body_len > 0)
			{
				//存入容器
				st_pis_pack_inf alarm_pack;
				alarm_pack.iDataLength = frame_body_len;
				alarm_pack.pData = new char[alarm_pack.iDataLength];
				memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
				(*p_vec_pack_inf).push_back(alarm_pack);
			}
		}

		p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
	}
	else
	{
		//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

int CMonitor::GetAlarmPictureList( _Msg_SearchJpgInfo SearchJpgInfo, std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf )
{
	const unsigned int vec_count_max = 1024;
	const int frame_max_len = 1024;
	const int frame_body_max_len = frame_max_len - 7;
	int ret = 0;
	int pack_ind = 0;//分包序号

	int frame_body_len = 0;//当前帧数据长度，不含头尾
	char frame_body[frame_body_max_len];
	memset(frame_body, 0, sizeof(frame_body));

	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	char begin_time[20] = {'\0'};
	char end_time[20] = {'\0'};
	sprintf(begin_time, "%04d-%02d-%02d %02d:%02d:%02d",
		NSP_STP_CM::Hex2Dec(SearchJpgInfo.dateTimeBegin.Year, 2), SearchJpgInfo.dateTimeBegin.Month, SearchJpgInfo.dateTimeBegin.Day, 
		SearchJpgInfo.dateTimeBegin.Hour, SearchJpgInfo.dateTimeBegin.Minute, SearchJpgInfo.dateTimeBegin.Second);
	sprintf(end_time, "%04d-%02d-%02d %02d:%02d:%02d",
		NSP_STP_CM::Hex2Dec(SearchJpgInfo.dateTimeEnd.Year, 2), SearchJpgInfo.dateTimeEnd.Month, SearchJpgInfo.dateTimeEnd.Day, 
		SearchJpgInfo.dateTimeEnd.Hour, SearchJpgInfo.dateTimeEnd.Minute, SearchJpgInfo.dateTimeEnd.Second );

	sprintf(sql_cmd, "select %s from %s where  %s >= '%s' and %s < '%s' and %s = %d and %s = %d \
					 and %s is not NULL LIMIT 1024;", 
					 gAlarmFFullName.c_str(),
					 gTblAlarm.c_str(), 
					 gAlarmFBeginTime.c_str(), begin_time, 
					 gAlarmFBeginTime.c_str(), end_time,
					 gAlarmFCarriageNum.c_str(), SearchJpgInfo.CarriageNumber,
					 gAlarmFCameraPos.c_str(), SearchJpgInfo.DervicePosition,
					 gAlarmFFullName.c_str()
					 );
	unsigned long long row_count = 0;
	unsigned int col_count = 0;
	char*** ppp_res = NULL;
	ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
	if (ret == 0)
	{
		//第一帧
		frame_body_len = 0;
		memset(frame_body, 0, sizeof(frame_body));
		frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
		frame_body[frame_body_len++] = pack_ind++;

		for (unsigned long long i = 0; i < row_count; i ++)
		{
			char* video_full_name =  ppp_res[i][0];
			const int video_full_name_len = strlen(video_full_name);

			if (frame_body_len + video_full_name_len <= frame_body_max_len)
			{
				//一帧数据
				memcpy(frame_body + frame_body_len, video_full_name, video_full_name_len);
				frame_body_len += video_full_name_len;
			}
			else
			{
				//超过一帧
				const int body_remain_len = frame_body_max_len - frame_body_len;
				memcpy(frame_body + frame_body_len, video_full_name, body_remain_len);
				frame_body_len += body_remain_len;

				unsigned int vec_num = (*p_vec_pack_inf).size();
				if(vec_num <= vec_count_max)
				{
					//存入容器
					st_pis_pack_inf alarm_pack;
					alarm_pack.iDataLength = frame_body_max_len;
					alarm_pack.pData = new char[alarm_pack.iDataLength];
					memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
					(*p_vec_pack_inf).push_back(alarm_pack);
				}
				else
				{
					break;
				}

				//下一帧
				frame_body_len = 0;
				memset(frame_body, 0, sizeof(frame_body));
				frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
				frame_body[frame_body_len++] = pack_ind++;
				const int alarm_msg_remain_len = video_full_name_len - body_remain_len;
				memcpy(frame_body + frame_body_len, video_full_name + body_remain_len, alarm_msg_remain_len);
				frame_body_len += alarm_msg_remain_len;
			}
		}

		unsigned int alarm_num = (*p_vec_pack_inf).size();
		if(alarm_num <= vec_count_max)
		{
			//有未满一帧
			if(frame_body_len > 0)
			{
				//存入容器
				st_pis_pack_inf alarm_pack;
				alarm_pack.iDataLength = frame_body_len;
				alarm_pack.pData = new char[alarm_pack.iDataLength];
				memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
				(*p_vec_pack_inf).push_back(alarm_pack);
			}
		}

		p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
	}
	else
	{
		//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

int CMonitor::GetAlarmVideoList( _Msg_SearchVideoInfo SearchVideoInfo, std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf )
{
	const unsigned int vec_count_max = 1024;
	const int frame_max_len = 1024;
	const int frame_body_max_len = frame_max_len - 7;
	int ret = 0;
	int pack_ind = 0;//分包序号

	int frame_body_len = 0;//当前帧数据长度，不含头尾
	char frame_body[frame_body_max_len];
	memset(frame_body, 0, sizeof(frame_body));

	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	char begin_time[20] = {'\0'};
	char end_time[20] = {'\0'};
	sprintf(begin_time, "%04d-%02d-%02d %02d:%02d:%02d",
		NSP_STP_CM::Hex2Dec(SearchVideoInfo.dateTimeBegin.Year, 2), SearchVideoInfo.dateTimeBegin.Month, SearchVideoInfo.dateTimeBegin.Day, 
		SearchVideoInfo.dateTimeBegin.Hour, SearchVideoInfo.dateTimeBegin.Minute, SearchVideoInfo.dateTimeBegin.Second);
	sprintf(end_time, "%04d-%02d-%02d %02d:%02d:%02d",
		NSP_STP_CM::Hex2Dec(SearchVideoInfo.dateTimeEnd.Year, 2), SearchVideoInfo.dateTimeEnd.Month, SearchVideoInfo.dateTimeEnd.Day, 
		SearchVideoInfo.dateTimeEnd.Hour, SearchVideoInfo.dateTimeEnd.Minute, SearchVideoInfo.dateTimeEnd.Second );

	sprintf(sql_cmd, "select %s from %s where  %s >= '%s' and %s < '%s' and %s = %d and %s = %d \
					 and %s = 1 LIMIT 1024;", 
					 gAlarmFVideoFullName.c_str(),
					 gTblAlarm.c_str(), 
					 gAlarmFBeginTime.c_str(), begin_time, 
					 gAlarmFBeginTime.c_str(), end_time,
					 gAlarmFCarriageNum.c_str(), SearchVideoInfo.CarriageNumber,
					 gAlarmFCameraPos.c_str(), SearchVideoInfo.DervicePosition,
					 gAlarmFVideoBuildFlag.c_str()
					 );
	unsigned long long row_count = 0;
	unsigned int col_count = 0;
	char*** ppp_res = NULL;
	ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
	if (ret == 0)
	{
		//第一帧
		frame_body_len = 0;
		memset(frame_body, 0, sizeof(frame_body));
		frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
		frame_body[frame_body_len++] = pack_ind++;

		for (unsigned long long i = 0; i < row_count; i ++)
		{
			char* video_full_name =  ppp_res[i][0];
			const int video_full_name_len = strlen(video_full_name);

			if (frame_body_len + video_full_name_len <= frame_body_max_len)
			{
				//一帧数据
				memcpy(frame_body + frame_body_len, video_full_name, video_full_name_len);
				frame_body_len += video_full_name_len;
			}
			else
			{
				//超过一帧
				const int body_remain_len = frame_body_max_len - frame_body_len;
				memcpy(frame_body + frame_body_len, video_full_name, body_remain_len);
				frame_body_len += body_remain_len;

				unsigned int vec_num = (*p_vec_pack_inf).size();
				if(vec_num <= vec_count_max)
				{
					//存入容器
					st_pis_pack_inf alarm_pack;
					alarm_pack.iDataLength = frame_body_max_len;
					alarm_pack.pData = new char[alarm_pack.iDataLength];
					memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
					(*p_vec_pack_inf).push_back(alarm_pack);
				}
				else
				{
					break;
				}

				//下一帧
				frame_body_len = 0;
				memset(frame_body, 0, sizeof(frame_body));
				frame_body[frame_body_len++] = pack_ind * frame_body_max_len;
				frame_body[frame_body_len++] = pack_ind++;
				const int alarm_msg_remain_len = video_full_name_len - body_remain_len;
				memcpy(frame_body + frame_body_len, video_full_name + body_remain_len, alarm_msg_remain_len);
				frame_body_len += alarm_msg_remain_len;
			}
		}

		unsigned int alarm_num = (*p_vec_pack_inf).size();
		if(alarm_num <= vec_count_max)
		{
			//有未满一帧
			if(frame_body_len > 0)
			{
				//存入容器
				st_pis_pack_inf alarm_pack;
				alarm_pack.iDataLength = frame_body_len;
				alarm_pack.pData = new char[alarm_pack.iDataLength];
				memcpy(alarm_pack.pData, frame_body, alarm_pack.iDataLength);
				(*p_vec_pack_inf).push_back(alarm_pack);
			}
		}

		p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
	}
	else
	{
		//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

void CMonitor::OptionBuildVideo( VideoNSP::tag_st_video_copy video_copy)
{
	Control(VideoNSP::VIDEO_COMMAND_COPY, (const char*)&video_copy);
}

void CMonitor::SetRunSpare( bool is_run_spare )
{
	m_runSpare = is_run_spare;
}



