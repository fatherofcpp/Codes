#include "StdAfx.h"
#include "BsAlarm.h"

#include "BsDefStruct.h"
#include "Socket/CmTransTcp.h"
#include "Socket/CmTransUdp.h"
#include "CmFunction.h"
#include "DataBase/CmDBMySql.h"
#include "BsGlobal.h"
#include "Log/CmLog.h"
#include "BsAbstractCamera.h"
#include "BsMainTask.h"
#include "BsMvb.h"
#include "BsFunction.h"
#include "Mvb/ServerTrans.h"
#include "CamVideo/VideoObjectDevSer.h"
#include "BsMonitor.h"

static const int g_cam_num = E_CAM_CLASS_TATOL;

CAlarm::CAlarm(void)
{
	m_pictureRootPath = "";
	m_pictureRootPathAlarm = "";
	m_videoRootPath = "";
	m_videoRootPathAlarm = "";
	m_saveFrameSuffix = "";
	m_diskNeedSpace = 0;
	m_pDbMysqlParam = NULL;
	m_pDataCenterService = new tag_st_data_center_service;
	m_pCMainTask = NULL;
	m_pCMvb = NULL;
	m_pCMonitor = NULL;
	m_videoMaxSec = 0;
}

CAlarm::~CAlarm(void)
{
	ReleaseRes();
}

unsigned int WINAPI CAlarm::thread_alarm_report( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAlarm* p_this = (CAlarm*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;
	const int heart_beat_delt_tm = 5 * 1000;
	const int no_data_delt_tm = p_this->m_dataCenterTimeoutS * 1000;

	const int recv_buf_len = 10240;
	char* p_recv_buf = new char[recv_buf_len];
	const int send_buf_len = 10240;
	char* p_send_buf = new char[send_buf_len];
	const int bj_ath_len = 1024;
	char* p_bj_ath = new char[bj_ath_len];
	NSP_STP_CM::CTcp* p_CTcp = new NSP_STP_CM::CTcp;
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	memset(p_recv_buf, 0, recv_buf_len);
	memset(p_send_buf, 0, send_buf_len);
	memset(p_bj_ath, 0, bj_ath_len);

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	p_CTcp->SetServerIp(p_this->m_pDataCenterService->alarm_service.ip.c_str());
	p_CTcp->SetServerPort(p_this->m_pDataCenterService->alarm_service.port);
	timeval r_time_out;
	r_time_out.tv_sec = 5;
	r_time_out.tv_usec = 0;
	p_CTcp->SetReadTimeOut(r_time_out);

	int ret = 0;

	int heart_beat_pre_ms = ::GetTickCount();
	int heart_beat_now_ms = ::GetTickCount();
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

		bool communicate_suc = false;//非心跳通信

		sprintf(sql_cmd, "select %s, %s, %s, %s from %s where %s > %s and %s = 0 and %s is not NULL and %s = 1 LIMIT 32;", 
			gAlarmFKey.c_str(), gAlarmFAlarmMessage.c_str(), gAlarmFFullName.c_str(), gAlarmFVideoFullName.c_str(),
			gTblAlarm.c_str(), 
			gAlarmFBeginTime.c_str(), gDbTimeTwoDayAgo.c_str(),
			gAlarmFSendFlag.c_str(), gAlarmFFullName.c_str(), gAlarmFVideoBuildFlag.c_str()
			);
		unsigned long long row_count = 0;
		unsigned int col_count = 0;
		char*** ppp_res = NULL;
		ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
		if (ret == 0)
		{
			for (unsigned long long i = 0; i < row_count; i ++)
			{
				long long alarm_id = -1;			
				std::string alarm_message = "";
				std::string alarm_full_name = "";
				std::string alarm_video_full_name = "";

				NSP_STP_CM::str_to_int(ppp_res[0][0], alarm_id);
				alarm_message =  ppp_res[0][1];
				alarm_full_name =  ppp_res[0][2];
				alarm_video_full_name =  ppp_res[0][3];

				if (alarm_id > 0)
				{
					const bool pic_exist = NSP_STP_CM::path_exist(alarm_full_name);
					const bool video_exist = NSP_STP_CM::path_exist(alarm_video_full_name);
					if (pic_exist && video_exist)
					{
						memset(p_bj_ath, 0, bj_ath_len);
						sprintf(p_bj_ath, "%lld;%s;%s;%s;\n", alarm_id, alarm_message.c_str(), alarm_full_name.c_str(), alarm_video_full_name.c_str());
						memset(p_send_buf, 0, send_buf_len);
						sprintf(p_send_buf, "{bj%010d}%s", strlen(p_bj_ath), p_bj_ath);

						p_CTcp->ConnectService();
						const int send_len = strlen(p_send_buf);
						bool net_err = false;
						ret = p_CTcp->Send2Service(p_send_buf, send_len);
						if (ret == send_len)
						{
							//pic
							const bool is_send_pic = true;
							if (is_send_pic)
							{
								const int file_size = NSP_STP_CM::file_len(alarm_full_name);
								sprintf(p_send_buf, "{tp%010d}", file_size);
								const int send_len = strlen(p_send_buf);
								ret = p_CTcp->Send2Service(p_send_buf, send_len);
								if (ret == send_len)
								{
									FILE* fp = NULL;
									fp = fopen(alarm_full_name.c_str(), "rb");
									if (fp != NULL)
									{
										while (1)
										{
											memset(p_send_buf, 0, send_buf_len);
											size_t read_size = fread(p_send_buf, 1, send_buf_len, fp);
											if (read_size > 0)
											{
												ret = p_CTcp->Send2Service(p_send_buf, read_size);
												if (ret != read_size)
												{
													gLog.Add("%s Send2Service pic err ,ret=%d\n", __FUNCTION__, ret);
													net_err = true;
													break;
												}
											}
											else
											{
												break;
											}
										}
										fclose(fp);
										fp = NULL;
									}

								}
								else
								{
									net_err = true;
								}

								if (net_err == true)
								{
									p_CTcp->DisconnectService();
									break;
								}
							}

							//video
							const bool is_send_video = true;
							if (is_send_video)
							{
								const int file_size = NSP_STP_CM::file_len(alarm_video_full_name);
								sprintf(p_send_buf, "{tv%010d}", file_size);
								const int send_len = strlen(p_send_buf);
								ret = p_CTcp->Send2Service(p_send_buf, send_len);
								if (ret == send_len)
								{
									FILE* fp = NULL;
									fp = fopen(alarm_video_full_name.c_str(), "rb");
									if (fp != NULL)
									{
										while (1)
										{
											memset(p_send_buf, 0, send_buf_len);
											size_t read_size = fread(p_send_buf, 1, send_buf_len, fp);
											if (read_size > 0)
											{
												ret = p_CTcp->Send2Service(p_send_buf, read_size);
												if (ret != read_size)
												{
													gLog.Add("%s Send2Service video err ,ret=%d\n", __FUNCTION__, ret);
													net_err = true;
													break;
												}
											}
											else
											{
												break;
											}
										}
										fclose(fp);
										fp = NULL;
									}

								}
								else
								{
									net_err = true;
								}

								if (net_err == true)
								{
									p_CTcp->DisconnectService();
									break;
								}
							}

							//receive
							int recv_len = 14;//"{bj0123456789}alarm_id\n"
							memset(p_recv_buf, 0, recv_buf_len);
							ret = p_CTcp->RecvFrmService(p_recv_buf, recv_len);
							if (ret == recv_len)
							{
								int body_len = 0;
								ret = sscanf(p_recv_buf, "{bj%10d}", &body_len);
								if (1 == ret)
								{
									memset(p_recv_buf, 0, recv_buf_len);
									ret = p_CTcp->RecvFrmService(p_recv_buf, body_len);
									if (ret == body_len)
									{
										long long recv_alarm_id = 0;
										NSP_STP_CM::str_to_int(p_recv_buf, recv_alarm_id);
										if (alarm_id == recv_alarm_id)
										{
											sprintf(sql_cmd, "update %s set %s = 1 where  %s = %lld;", 
												gTblAlarm.c_str(), 
												gAlarmFSendFlag.c_str(), 
												gAlarmFKey.c_str(), recv_alarm_id
												);
											ret = p_CMySqlDB->WriteDataBase(sql_cmd);
											if (ret != 0)
											{
												gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
											}
											else
											{
												ret = 0;
											}
										}
										else
										{
											//alarm_id not same
										}
									}
									else
									{
										net_err = true;
									}
								}
								else
								{
									net_err = true;
								}
							}
							else
							{
								net_err = true;
							}

							if (net_err == true)
							{
								p_CTcp->DisconnectService();
								break;
							}
						}
						else
						{
							net_err = true;
						}

						if (net_err == true)
						{
							p_CTcp->DisconnectService();
							break;
						}
						else
						{
							communicate_suc = true;
						}
					}
					else
					{
						//图片或视频不存在
					}
				}
				else
				{
					//continue
				}
			}

			p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
		}
		else
		{
			//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		}

		if (!communicate_suc)
		{
			heart_beat_now_ms = ::GetTickCount();
			if (heart_beat_now_ms - heart_beat_pre_ms >= heart_beat_delt_tm)
			{
				heart_beat_pre_ms = ::GetTickCount();

				//心跳
				strcpy(p_send_buf, "{bi}");
				const int send_len = strlen("{bi}");
				ret = p_CTcp->Send2Service(p_send_buf, send_len);
				if (ret == send_len)
				{
					const int recv_len = strlen("{bi}");
					memset(p_recv_buf, 0, recv_buf_len);
					ret = p_CTcp->RecvFrmService(p_recv_buf, recv_len);
					if (ret == recv_len && 0 == strcmp(p_recv_buf, "{bi}"))
					{
						nodata_pre_ms = ::GetTickCount();
						communicate_suc = true;
					}
					else
					{
						gLog.Add("%s RecvFrmService heartbeat err ,ret=%d\n", __FUNCTION__, ret);
						p_CTcp->DisconnectService();
					}
				}
				else
				{
					p_CTcp->DisconnectService();
				}
			}
		}
		else
		{
			heart_beat_pre_ms = ::GetTickCount();

			nodata_pre_ms = ::GetTickCount();
		}


		nodata_now_ms = ::GetTickCount();
		if (nodata_now_ms - nodata_pre_ms >= no_data_delt_tm)
		{
			nodata_pre_ms = ::GetTickCount();

			unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_DATA_ANALYSIS_SERVER_STATUS;
			unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
			UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, true);
		}

		if (communicate_suc)
		{
			unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_DATA_ANALYSIS_SERVER_STATUS;
			unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
			UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, false);
		}

	}

	delete[] p_recv_buf;
	delete[] p_send_buf;
	delete[] p_bj_ath;
	delete p_CTcp;
	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

int CAlarm::TaskStart()
{
	SetThreadCount(7);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_alarm_report", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_alarm_report, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_rtdata_report", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_rtdata_report, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 2;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_device_state", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_device_state, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 3;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_time_delete_db", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_time_delete_db, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 4;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_alarm_frame_match_video", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_alarm_frame_match_video, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 5;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_export_info", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_export_info, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 6;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_watchdog", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_watchdog, p_st_thread_param, CpuMaskRemain());
	}
	return 0;
}


void CAlarm::SetReportService( tag_st_data_center_service* p_st_data_center_service )
{
	m_pDataCenterService = p_st_data_center_service;
}

void CAlarm::ReleaseRes()
{

}

std::string CAlarm::WarnType2Str( int warn_type )
{
	std::string warn_str = "";
	return warn_str;
}

unsigned int WINAPI CAlarm::thread_device_state( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAlarm* p_this = (CAlarm*)p_st_thread_param->pParam;
	delete p_st_thread_param;


	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;
	const int state_err_total_tm = p_this->m_cameraTimeoutS;
	const unsigned int c_rerty_max_count = state_err_total_tm / sleep_m;
	st_dev_state dev_state[g_cam_num];

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		for (int i = 0; i < g_cam_num; i ++)
		{
			unsigned int alarm_type = 0;
			unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
			if ( E_CAM_CLASS_MONITOR == i)
			{
				alarm_type = E_SELFCHECK_FAULT_TYPE_MONITOR_CAMERA_STATUS;
			}
			else if (E_CAM_CLASS_STRUCT == i)
			{
				alarm_type = E_SELFCHECK_FAULT_TYPE_ROOF_MONITORING_EQUIPMENT;
			}
			else if (E_CAM_CLASS_TEMPRATE == i)
			{
				alarm_type = E_SELFCHECK_FAULT_TYPE_INFRARED_MONITORING_MODULE;
			}
			else if (E_CAM_CLASS_ARC_NOTIFY == i)
			{
				alarm_type = E_SELFCHECK_FAULT_TYPE_ARC_MONITORING_MODULE;
			}
			else if (E_CAM_CLASS_ARC_COLLECT == i)
			{
				alarm_type = E_SELFCHECK_FAULT_TYPE_ARC_MONITORING_MODULE;
			}
			else
			{
			}

			CAbstractCamera* p_camera = p_this->m_pCMainTask->GetCamera(i);
			if (p_camera)
			{
				unsigned int now_heartb = p_camera->m_cameraHeartBeat;
				if (now_heartb != dev_state[i].heartHeat)
				{
					dev_state[i].faultCount = 0;
					UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, false);
				}
				else
				{
					dev_state[i].faultCount ++;
				}

				dev_state[i].heartHeat = p_camera->m_cameraHeartBeat;

				if (c_rerty_max_count < dev_state[i].faultCount)
				{
					dev_state[i].faultCount = 0;
					p_camera->m_cameraOnline = false;
					//设备故障报警
					UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, true);
				}
				else
				{
					p_camera->m_cameraOnline = true;
				}


			}
		}
	}

	return 0;
}

static int delete_folder_before_days(std::string disk_path, unsigned int days_keep, bool is_delete_oldest)
{
	std::vector<M_STRING> vec_folder_path;
	NSP_STP_CM::folderpath_list_in_folder(disk_path, vec_folder_path);
	MU_INT32 folder_num = vec_folder_path.size();
	if (folder_num > days_keep || is_delete_oldest)
	{
		NSP_STP_CM::folder_oldest_delete(disk_path);
	}
	return 0;
}

unsigned int WINAPI CAlarm::thread_time_delete_db( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAlarm* p_this = (CAlarm*)p_st_thread_param->pParam;
	delete p_st_thread_param;


	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;

	const std::string root_path = NSP_STP_CM::str_match_head2sig_first(p_this->m_pictureRootPath, "\\", true);
	const std::string file_suffix = p_this->m_saveFrameSuffix;
	const int min_space_g = p_this->m_diskNeedSpace;

	int ret = -1;
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		bool disk_full = NSP_STP_CM::DiskIsFull(root_path, min_space_g);
		if (disk_full)
		{
			//磁盘报警
			unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_RIGID_DISK_FAULT;
			unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
			UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, true);
			gLog.Add("%s disk_full\n", __FUNCTION__);
		}
		else
		{
			unsigned int alarm_type = E_SELFCHECK_FAULT_TYPE_RIGID_DISK_FAULT;
			unsigned int alarm_cls = E_FRAME_WARN_CLS_0;
			UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), alarm_type, alarm_cls, true, false);
		}

		//删除数据库有效期之前的视频与图像,并删除数据对应记录
		bool is_delete_old_db = true;
		if (is_delete_old_db)
		{
			//删除备份视频
			const bool is_delete_spare_old_normal_video = true;
			if (is_delete_spare_old_normal_video)
			{
				ret = 0;
				while (0 == ret)
				{
					Sleep(sleep_m);

					bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
					if (!is_thread_run)
					{
						break;
					}

					long long record_id = -1;			
					std::string record_full_name = "";
					sprintf(sql_cmd, "select %s, %s from %s where %s < %s LIMIT 32;", 
						gRecordFKey.c_str(), gRecordFFullName.c_str(),
						gTblRecordSpare.c_str(), 
						gRecordFBeginTime.c_str(), gDbVideoNormalTimeMaxDayAgo.c_str()
						);
					unsigned long long row_count = 0;
					unsigned int col_count = 0;
					char*** ppp_res = NULL;
					ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
					if (ret == 0)
					{
						for (unsigned long long i = 0; i < col_count; i ++)
						{
							NSP_STP_CM::str_to_int(ppp_res[i][0], record_id);
							record_full_name = ppp_res[i][1];
							//file_delete(record_full_name);
							if (record_id > 0)
							{
								//删除记录
								sprintf(sql_cmd, "delete from %s where %s = %lld;", 
									gTblRecordSpare.c_str(), 
									gRecordFKey.c_str(), record_id
									);
								ret = p_CMySqlDB->WriteDataBase(sql_cmd);
								if (ret != 0)
								{
									gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
								}
							}
						}

						p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
					}
					else
					{
						//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
				}
			}

			//删除视频
			const bool is_delete_old_normal_video = true;
			if (is_delete_old_normal_video)
			{
				ret = 0;
				while (0 == ret)
				{
					Sleep(sleep_m);

					bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
					if (!is_thread_run)
					{
						break;
					}

					long long record_id = -1;			
					std::string record_full_name = "";
					sprintf(sql_cmd, "select %s, %s from %s where %s < %s LIMIT 32;", 
						gRecordFKey.c_str(), gRecordFFullName.c_str(),
						gTblRecord.c_str(), 
						gRecordFBeginTime.c_str(), gDbVideoNormalTimeMaxDayAgo.c_str()
						);
					unsigned long long row_count = 0;
					unsigned int col_count = 0;
					char*** ppp_res = NULL;
					ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
					if (ret == 0)
					{
						for (unsigned long long i = 0; i < col_count; i ++)
						{
							NSP_STP_CM::str_to_int(ppp_res[i][0], record_id);
							record_full_name = ppp_res[i][1];
							//file_delete(record_full_name);
							if (record_id > 0)
							{
								//删除记录
								sprintf(sql_cmd, "delete from %s where %s = %lld;", 
									gTblRecord.c_str(), 
									gRecordFKey.c_str(), record_id
									);
								ret = p_CMySqlDB->WriteDataBase(sql_cmd);
								if (ret != 0)
								{
									gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
								}
							}
						}

						p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
					}
					else
					{
						//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
				}
			}

			//删除报警图片及报警视频
			const bool is_delete_alarm_pic_video = true;
			if (is_delete_alarm_pic_video)
			{
				ret = 0;
				while (0 == ret)
				{
					Sleep(sleep_m);

					bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
					if (!is_thread_run)
					{
						break;
					}

					long long alarm_id = -1;			
					std::string alarm_full_name = "";
					std::string alarm_video_full_name = "";
					sprintf(sql_cmd, "select %s, %s, %s from %s where %s < %s LIMIT 32;", 
						gAlarmFKey.c_str(), gAlarmFFullName.c_str(), gAlarmFVideoFullName.c_str(),
						gTblAlarm.c_str(), 
						gAlarmFBeginTime.c_str(), gDbVideoPicAlarmTimeMaxDayAgo.c_str()
						);
					unsigned long long row_count = 0;
					unsigned int col_count = 0;
					char*** ppp_res = NULL;
					ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
					if (ret == 0)
					{
						for (unsigned long long i = 0; i < col_count; i ++)
						{
							NSP_STP_CM::str_to_int(ppp_res[0][0], alarm_id);
							alarm_full_name =  ppp_res[0][1];
							alarm_video_full_name =  ppp_res[0][2];

							//file_delete(alarm_full_name);
							//file_delete(alarm_video_full_name);

							if (alarm_id > 0)
							{
								//删除记录
								sprintf(sql_cmd, "delete from %s where %s = %lld;", 
									gTblAlarm.c_str(), 
									gAlarmFKey.c_str(), alarm_id
									);
								ret = p_CMySqlDB->WriteDataBase(sql_cmd);
								if (ret != 0)
								{
									gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
								}
							}
						}

						p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
					}
					else
					{
						//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
				}
			}

			//删除过期实时数据
			const bool is_delete_rtdata = true;
			if (is_delete_rtdata)
			{
				ret = 0;
				while (0 == ret)
				{
					Sleep(sleep_m);

					bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
					if (!is_thread_run)
					{
						break;
					}

					long long rt_id = -1;			
					sprintf(sql_cmd, "select %s from %s where %s < %s LIMIT 32;", 
						gRtFKey.c_str(), 
						gTblRt.c_str(), 
						gRtFBeginTime.c_str(), gDbRtDataTimeMaxDayAgo.c_str()
						);
					unsigned long long row_count = 0;
					unsigned int col_count = 0;
					char*** ppp_res = NULL;
					ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
					if (ret == 0)
					{
						for (unsigned long long i = 0; i < col_count; i ++)
						{
							NSP_STP_CM::str_to_int(ppp_res[0][0], rt_id);
							if (rt_id > 0)
							{
								//删除记录
								sprintf(sql_cmd, "delete from %s where %s = %lld;", 
									gTblRt.c_str(), 
									gRtFKey.c_str(), rt_id
									);
								ret = p_CMySqlDB->WriteDataBase(sql_cmd);
								if (ret != 0)
								{
									gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
								}
							}
						}

						p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
					}
					else
					{
						//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
				}
			}

			// 删除之前的非报警图片
			const bool is_delete_normal_pic = true;
			if (is_delete_normal_pic)
			{
				ret = 0;
				while (0 == ret)
				{
					Sleep(sleep_m);

					bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
					if (!is_thread_run)
					{
						break;
					}

					long long alarm_id = -1;			
					std::string alarm_full_name = "";

					sprintf(sql_cmd, "select %s, %s from %s where  %s = 0 and %s < %s LIMIT 32;", 
						gAlarmFKey.c_str(), gAlarmFFullName.c_str(),
						gTblAlarm.c_str(), 
						gAlarmFAlarmType.c_str(),
						gAlarmFBeginTime.c_str(), gDbPicNormalTimeMaxDayAgo.c_str()
						);

					unsigned long long row_count = 0;
					unsigned int col_count = 0;
					char*** ppp_res = NULL;
					ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
					if (ret == 0)
					{
						for (unsigned long long i = 0; i < row_count; i ++)
						{
							NSP_STP_CM::str_to_int(ppp_res[i][0], alarm_id);
							alarm_full_name =  ppp_res[i][1];

							//file_delete(alarm_full_name);

							sprintf(sql_cmd, "delete from %s where %s = %lld;", 
								gTblAlarm.c_str(), 
								gAlarmFKey.c_str(), alarm_id
								);
							ret = p_CMySqlDB->WriteDataBase(sql_cmd);
							if (ret != 0)
							{
								gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
							}
						}
						p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
					}
					else
					{
						//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
				}
			}

			const bool is_rm_old_export = true;
			if (is_rm_old_export)
			{
				delete_folder_before_days(p_this->m_exportInfRootPath, gDiskSaveDayNum, disk_full);
				delete_folder_before_days(p_this->m_pictureRootPath, gDiskSaveDayNum, disk_full);
				delete_folder_before_days(p_this->m_pictureRootPathAlarm, gDiskSaveDayNum, disk_full);
				delete_folder_before_days(p_this->m_videoRootPath, gDiskSaveDayNum, disk_full);
				delete_folder_before_days(p_this->m_videoRootPathAlarm, gDiskSaveDayNum, disk_full);
			}

		}
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

unsigned int WINAPI CAlarm::thread_alarm_frame_match_video( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAlarm* p_this = (CAlarm*)p_st_thread_param->pParam;
	delete p_st_thread_param;


	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;

	int ret = -1;
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);
	
	const int c_total_wait_video_src_s = p_this->m_videoMaxSec * 3 / 2;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		// 对最近一天报警图片匹配视频,此处LIMIT 必须为1
		sprintf(sql_cmd, "select %s, %s, %s from %s where  %s > %s and %s != 0 and %s = 0 LIMIT 1;", 
			gAlarmFKey.c_str(), gAlarmFFileName.c_str(), gAlarmFBeginTime.c_str(),
			gTblAlarm.c_str(), 
			gAlarmFBeginTime.c_str(), gDbTimeOneDayAgo.c_str(), 
			gAlarmFAlarmType.c_str(), gAlarmFVideoBuildFlag.c_str()
			);
		unsigned long long row_count = 0;
		unsigned int col_count = 0;
		char*** ppp_res = NULL;
		ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
		if (ret == 0)
		{
			for (unsigned long long i = 0; i < row_count; i ++)
			{
				long long alarm_id = -1;	
				NSP_STP_CM::st_time alarm_begin_time;

				NSP_STP_CM::str_to_int(ppp_res[i][0], alarm_id);
				std::string alarm_file_name = ppp_res[i][1];
				sscanf(ppp_res[i][2], "%04d-%02d-%02d %02d:%02d:%02d", &alarm_begin_time.year, &alarm_begin_time.mon, &alarm_begin_time.day,
					&alarm_begin_time.hour, &alarm_begin_time.min, &alarm_begin_time.sec);

				//匹配原始视频
				char time_inf[256];
				sprintf(time_inf, "%04d-%02d-%02d %02d:%02d:%02d", 
					alarm_begin_time.year, alarm_begin_time.mon, alarm_begin_time.day, alarm_begin_time.hour, alarm_begin_time.min, alarm_begin_time.sec);

				sprintf(sql_cmd, "select %s, %s, %s, %s, %s from %s where  %s < '%s' and %s > '%s' and %s is not NULL LIMIT 1;", 
					gRecordFKey.c_str(), gRecordFBeginTime.c_str(), gRecordFFullName.c_str(), gRecordFFileName.c_str(),
					gRecordFEndTime.c_str(),
					gTblRecord.c_str(), 
					gRecordFBeginTime.c_str(), time_inf,
					gRecordFEndTime.c_str(),   time_inf,
					gRecordFEndTime.c_str()
					);
				unsigned long long row_count = 0;
				unsigned int col_count = 0;
				char*** ppp_res = NULL;
				ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
				if (ret == 0)
				{
					long long record_id = -1;			
					NSP_STP_CM::str_to_int(ppp_res[0][0], record_id); 
					NSP_STP_CM::st_time record_begin_time;
					sscanf(ppp_res[0][1], "%04d-%02d-%02d %02d:%02d:%02d", &record_begin_time.year, &record_begin_time.mon, &record_begin_time.day,
						&record_begin_time.hour, &record_begin_time.min, &record_begin_time.sec);
					std::string video_full_name = ppp_res[0][2];
					std::string video_file_name = ppp_res[0][3];
					NSP_STP_CM::st_time record_end_time;
					sscanf(ppp_res[0][4], "%04d-%02d-%02d %02d:%02d:%02d", &record_end_time.year, &record_end_time.mon, &record_end_time.day,
						&record_end_time.hour, &record_end_time.min, &record_end_time.sec);

					char alarm_video_full_name[MAX_PATH];
					char alarm_video_file_name[MAX_PATH];
					int build_flag = 0;
					const bool is_src_exist = NSP_STP_CM::path_exist(video_full_name);
					if (is_src_exist)
					{
						//拷贝故障视频
						VideoNSP::st_video_copy video_copy;
						time_t time_stamp_alarm;
						GetStampByTime(&time_stamp_alarm, alarm_begin_time);
						time_t time_stamp_record_begin;
						GetStampByTime(&time_stamp_record_begin, record_begin_time);
						time_t time_stamp_record_end;
						GetStampByTime(&time_stamp_record_end, record_end_time);
						const long long alarm_2_head = time_stamp_alarm - time_stamp_record_begin;
						int alarm_left_sec = 0;
						if (alarm_2_head > gAlarmVideoSecOff)
						{
							alarm_left_sec = gAlarmVideoSecOff;
						}
						else
						{
							alarm_left_sec = (int)alarm_2_head;
						}
						video_copy.TimeOffSec = alarm_2_head - alarm_left_sec;
						if (video_copy.TimeOffSec < 0)
						{
							video_copy.TimeOffSec = 0;
						}
						const long long alarm_2_tail = time_stamp_record_end - time_stamp_alarm;
						int alarm_right_sec = 0;
						if (alarm_2_tail > gAlarmVideoSecOff)
						{
							alarm_right_sec = gAlarmVideoSecOff;
						}
						else
						{
							alarm_right_sec = (int)alarm_2_tail;
						}
						video_copy.TimeLenSec = alarm_left_sec + alarm_right_sec;

						std::string alarm_file_name_no_suf = NSP_STP_CM::str_right_cut_tag(alarm_file_name, p_this->m_saveFrameSuffix);
						sprintf(alarm_video_file_name, "%s%s", alarm_file_name_no_suf.c_str(), p_this->m_saveVideoSuffix.c_str());
						std::string video_folder = p_this->m_videoRootPathAlarm;
						std::string date_inf = NSP_STP_CM::str_right_cut_tag(video_full_name, "\\");
						date_inf = NSP_STP_CM::str_left_cut_tag(date_inf, p_this->m_videoRootPath);
						sprintf(alarm_video_full_name, "%s%s\\%s", video_folder.c_str(), date_inf.c_str(), alarm_video_file_name);

						strcpy(video_copy.SourceUrl, video_full_name.c_str());
						strcpy(video_copy.SavePath, alarm_video_full_name);
						if (p_this->m_pCMonitor)
						{
							gLog.Add("%s SourceUrl=%s,SavePath=%s,TimeOffSec=%lld,TimeLenSec=%d\n", 
								__FUNCTION__, video_copy.SourceUrl, video_copy.SavePath,video_copy.TimeOffSec,video_copy.TimeLenSec);
							p_this->m_pCMonitor->OptionBuildVideo(video_copy);
							build_flag = 1;
						}
					}
					else
					{
						sprintf(alarm_video_full_name, "%s", "drop");
						sprintf(alarm_video_file_name, "%s", "drop");
						build_flag = 2;
					}

					//设置图片对应视频
					sprintf(sql_cmd, "update %s set %s = '%s', %s = '%s', %s = %d where  %s = %lld;", 
						gTblAlarm.c_str(), 
						gAlarmFVideoFullName.c_str(), (NSP_STP_CM::str_sig_double(alarm_video_full_name, '\\')).c_str(),
						gAlarmFVideoFileName.c_str(), alarm_video_file_name,
						gAlarmFVideoBuildFlag.c_str(), build_flag,
						gAlarmFKey.c_str(), alarm_id
						);
					ret = p_CMySqlDB->WriteDataBase(sql_cmd);
					if (ret != 0)
					{
						gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}

					p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
				}
				else
				{
					//匹配视频失败,原始视频可能没有录制完成。
					time_t time_stamp_alarm;
					GetStampByTime(&time_stamp_alarm, alarm_begin_time);
					time_t time_now;
					time(&time_now);
					int time_off_s = (int)(time_now - time_stamp_alarm);

					if (time_off_s > c_total_wait_video_src_s)
					{
						//设置图片对应视频
						std::string alarm_video_full_name = "drop";
						std::string alarm_video_file_name = "drop";
						int build_flag = 2;
						sprintf(sql_cmd, "update %s set %s = '%s', %s = '%s', %s = %d where  %s = %lld;", 
							gTblAlarm.c_str(), 
							gAlarmFVideoFullName.c_str(), alarm_video_full_name.c_str(),
							gAlarmFVideoFileName.c_str(), alarm_video_file_name.c_str(),
							gAlarmFVideoBuildFlag.c_str(), build_flag,
							gAlarmFKey.c_str(), alarm_id
							);
						ret = p_CMySqlDB->WriteDataBase(sql_cmd);
						if (ret != 0)
						{
							gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
						}
					}
					else
					{
						//减少数据库访问频率
						Sleep(1000);
					}
					//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
				}
			}
			p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
		}
		else
		{
			//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		}
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

unsigned int WINAPI CAlarm::thread_rtdata_report( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAlarm* p_this = (CAlarm*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;
	const int heart_beat_delt_tm = 5 * 1000;
	const int heart_beat_total_count = heart_beat_delt_tm / sleep_m;

	const int recv_buf_len = 10240;
	char* p_recv_buf = new char[recv_buf_len];
	const int send_buf_len = 10240;
	char* p_send_buf = new char[send_buf_len];
	const int rt_ath_len = 10240;
	char* p_rt_ath = new char[rt_ath_len];
	NSP_STP_CM::CTcp* p_CTcp = new NSP_STP_CM::CTcp;
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	memset(p_recv_buf, 0, recv_buf_len);
	memset(p_send_buf, 0, send_buf_len);
	memset(p_rt_ath, 0, rt_ath_len);

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	p_CTcp->SetServerIp(p_this->m_pDataCenterService->rtdata_service.ip.c_str());
	p_CTcp->SetServerPort(p_this->m_pDataCenterService->rtdata_service.port);
	timeval r_time_out;
	r_time_out.tv_sec = 5;
	r_time_out.tv_usec = 0;
	p_CTcp->SetReadTimeOut(r_time_out);

	int ret = 0;
	int heart_beat_count = 0;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		bool communicate_suc = false;//非心跳通信

		sprintf(sql_cmd, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s > %s and %s = 0 LIMIT 32;", 
			gRtFKey.c_str(), gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(),
			gRtFLineId.c_str(), gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), 
			gRtFBeginTime.c_str(), gRtFBeginTimeMs.c_str(), gRtFLastTime.c_str(), 
			gTblRt.c_str(),
			gRtFBeginTime.c_str(), gDbTimeOneDayAgo.c_str(),
			gRtFSendFlag.c_str()
			);
		unsigned long long row_count = 0;
		unsigned int col_count = 0;
		char*** ppp_res = NULL;
		ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
		if (ret == 0)
		{
			for (unsigned long long i = 0; i < row_count; i ++)
			{
				long long rt_id = -1;	
				NSP_STP_CM::str_to_int(ppp_res[0][0], rt_id);

				st_rt_data rt_data;
				NSP_STP_CM::str_to_int(ppp_res[0][1], rt_data.CarriageId);
				NSP_STP_CM::str_to_int(ppp_res[0][2], rt_data.CarriageNum);
				NSP_STP_CM::str_to_int(ppp_res[0][3], rt_data.StationInd);
				MVB::tag_st_station_info* p_station = p_this->m_pCMvb->GetStationInfByInd(rt_data.StationInd);
				std::string station_alis = p_station->station_alias;
				NSP_STP_CM::str_to_int(ppp_res[0][4], rt_data.LineId);
				NSP_STP_CM::str_to_int(ppp_res[0][5], rt_data.Direction);
				NSP_STP_CM::str_to_int(ppp_res[0][6], rt_data.DataType);
				rt_data.RtData = ppp_res[0][7];
				rt_data.BeginTime = ppp_res[0][8];
				NSP_STP_CM::str_to_int(ppp_res[0][9], rt_data.BeginTimeMs);
				NSP_STP_CM::str_to_int(ppp_res[0][10], rt_data.LastTimeMs);

				if (rt_id > 0)
				{
					memset(p_rt_ath, 0, rt_ath_len);
					//列车编号&车厢编号&站点名称&几号线&行驶方向&开始时间&持续时间
					sprintf(p_rt_ath, "%d;%lld;%s;%d&%d&%s&%d&%d&%s.%03d&%d", 
						rt_data.DataType, rt_id, rt_data.RtData.c_str(), 
						rt_data.CarriageId, rt_data.CarriageNum, station_alis.c_str(), rt_data.LineId,
						rt_data.Direction, rt_data.BeginTime.c_str(), rt_data.BeginTimeMs, rt_data.LastTimeMs
						);
					memset(p_send_buf, 0, send_buf_len);
					sprintf(p_send_buf, "{rt%010d}%s", strlen(p_rt_ath), p_rt_ath);

					p_CTcp->ConnectService();
					const int send_len = strlen(p_send_buf);
					bool net_err = false;
					ret = p_CTcp->Send2Service(p_send_buf, send_len);
					if (ret == send_len)
					{
						//receive
						int recv_len = 14;//"{bj0123456789}alarm_id\n"
						memset(p_recv_buf, 0, recv_buf_len);
						ret = p_CTcp->RecvFrmService(p_recv_buf, recv_len);
						if (ret == recv_len)
						{
							int body_len = 0;
							ret = sscanf(p_recv_buf, "{rt%10d}", &body_len);
							if (1 == ret)
							{
								memset(p_recv_buf, 0, recv_buf_len);
								ret = p_CTcp->RecvFrmService(p_recv_buf, body_len);
								if (ret == body_len)
								{
									long long recv_rt_id = 0;
									NSP_STP_CM::str_to_int(p_recv_buf, recv_rt_id);
									if (rt_id == recv_rt_id)
									{
										sprintf(sql_cmd, "update %s set %s = 1 where  %s = %lld;", 
											gTblRt.c_str(), 
											gRtFSendFlag.c_str(), 
											gRtFKey.c_str(), recv_rt_id
											);
										ret = p_CMySqlDB->WriteDataBase(sql_cmd);
										if (ret != 0)
										{
											gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
										}
									}
									else
									{
										//alarm_id not same
									}
								}
								else
								{
									net_err = true;
								}
							}
							else
							{
								net_err = true;
							}
						}
						else
						{
							net_err = true;
						}

						if (net_err == true)
						{
							p_CTcp->DisconnectService();
							continue;
						}
					}
					else
					{
						net_err = true;
					}

					if (net_err == true)
					{
						p_CTcp->DisconnectService();
					}
					else
					{
						communicate_suc = true;
					}
				}
				else
				{
					//continue
				}
			}

			p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
		}
		else
		{
			//gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		}

		if (!communicate_suc)
		{
			if (++ heart_beat_count > heart_beat_total_count)
			{
				heart_beat_count = 0;

				//心跳
				strcpy(p_send_buf, "{bi}");
				const int send_len = strlen("{bi}");
				ret = p_CTcp->Send2Service(p_send_buf, send_len);
				if (ret == send_len)
				{
					const int recv_len = strlen("{bi}");
					memset(p_recv_buf, 0, recv_buf_len);
					ret = p_CTcp->RecvFrmService(p_recv_buf, recv_len);
					if (ret == recv_len && 0 == strcmp(p_recv_buf, "{bi}"))
					{
					}
					else
					{
						gLog.Add("%s RecvFrmService heartbeat,ret=%d\n", __FUNCTION__, ret);
						p_CTcp->DisconnectService();
					}
				}
				else
				{
					p_CTcp->DisconnectService();
				}
			}
		}
		else
		{
			heart_beat_count = 0;

		}

	}

	delete[] p_recv_buf;
	delete[] p_send_buf;
	delete[] p_rt_ath;
	delete p_CTcp;
	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

unsigned int WINAPI CAlarm::thread_export_info( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAlarm* p_this = (CAlarm*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;

	int ret = -1;
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	int pre_ms = ::GetTickCount();
	NSP_STP_CM::tag_st_time st_time_pre_alarm;
	NSP_STP_CM::tag_st_time st_time_pre_rt;
	NSP_STP_CM::tag_st_time st_time_pre_record;
	NSP_STP_CM::GetNowTime(&st_time_pre_alarm);
	NSP_STP_CM::GetNowTime(&st_time_pre_rt);
	NSP_STP_CM::GetNowTime(&st_time_pre_record);

	const int c_wait_video_down_s = p_this->m_videoMaxSec  * 3 / 2;//导出该时间之前的数据
	const int c_delay_ms = 1 * 60 * 1000;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		int nw_ms = ::GetTickCount();
		if (nw_ms - pre_ms > c_delay_ms)
		{
			pre_ms = ::GetTickCount();

			const std::string c_cmd_param = " FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' LINES STARTING BY '>' TERMINATED BY '\r\n'";

			NSP_STP_CM::tag_st_time st_time_cur;
			NSP_STP_CM::GetNowTime(&st_time_cur);
			char date_inf[64];
			sprintf(date_inf, "%04d%02d%02d\\%02d%02d", 
				st_time_cur.year, st_time_cur.mon, st_time_cur.day,
				st_time_cur.hour, st_time_cur.min / 30 * 30);

			std::string export_path = p_this->m_exportInfRootPath;
			export_path += date_inf;
			export_path += "\\";

			bool is_folder_create_suc = NSP_STP_CM::folder_create(export_path);
			if (is_folder_create_suc)
			{
				NSP_STP_CM::tag_st_time st_time_export_end;
				const int off_s = 0 - c_wait_video_down_s;
				NSP_STP_CM::TimeAjustByMs(st_time_cur, &st_time_export_end, off_s);

				char time_export_end[256];
				sprintf(time_export_end, "%04d-%02d-%02d %02d:%02d:%02d", 
					st_time_export_end.year, st_time_export_end.mon, st_time_export_end.day, 
					st_time_export_end.hour, st_time_export_end.min, st_time_export_end.sec);
				
				ret = TimeCompare(st_time_export_end, st_time_pre_alarm);
				if (ret > 0)
				{
					//t_alarm_info.txt为图像信息，格式为
					//">主键,图像路径,图像保存时间,对应报警视频路径,数据值，数据值参数,结构相机图像路径”。
					const std::string c_alarm_file = "f_alarm_info.txt";
					const std::string c_alarm_file_rt = "f_alarm_info_rt.txt";
					const std::string c_out_file = export_path + c_alarm_file_rt;
					const std::string c_alarm_file_dst = export_path + c_alarm_file;
					char c_cmd[256];
					sprintf(c_cmd, "into outfile '%s' %s", 
						(NSP_STP_CM::str_sig_double(c_out_file, '\\')).c_str(), c_cmd_param.c_str());
					remove(c_out_file.c_str());

					char time_export_beg[256];
					sprintf(time_export_beg, "%04d-%02d-%02d %02d:%02d:%02d", 
						st_time_pre_alarm.year, st_time_pre_alarm.mon, st_time_pre_alarm.day, 
						st_time_pre_alarm.hour, st_time_pre_alarm.min, st_time_pre_alarm.sec);

					sprintf(sql_cmd, "select %s, %s, %s, %s, %s, %s, %s from %s where  %s >= '%s' and %s < '%s' %s;", 
						gAlarmFKey.c_str(), gAlarmFFullName.c_str(), gAlarmFBeginTime.c_str(), gAlarmFVideoFullName.c_str(),
						gAlarmFAlarmValue.c_str(), gAlarmFAlarmValueParam.c_str(), gAlarmFStuctureFullName.c_str(),
						gTblAlarm.c_str(), 
						gAlarmFBeginTime.c_str(), time_export_beg, 
						gAlarmFBeginTime.c_str(), time_export_end, 
						c_cmd
						);
					ret = p_CMySqlDB->WriteDataBase(sql_cmd);
					if (ret != 0)
					{
						gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
					else
					{
						const int file_len_rt = NSP_STP_CM::file_len(c_out_file);
						if (file_len_rt > 0)
						{
							ret = NSP_STP_CM::copyFile(c_out_file.c_str(), c_alarm_file_dst.c_str(), false);
							if (ret != 0)
							{
								gLog.Add("%s copyFile %s to %s,ret=%d\n", __FUNCTION__, c_out_file.c_str(), c_alarm_file_dst.c_str(), ret);
							}
							else
							{
								memcpy(&st_time_pre_alarm, &st_time_export_end, sizeof(st_time_export_end));
							}
						}
					}
				}

				{
					NSP_STP_CM::tag_st_time st_time_now;
					NSP_STP_CM::GetNowTime(&st_time_now);
					char time_now[256];
					sprintf(time_now, "%04d-%02d-%02d %02d:%02d:%02d", 
						st_time_now.year, st_time_now.mon, st_time_now.day, 
						st_time_now.hour, st_time_now.min, st_time_now.sec);

					//t_rt_info.txt为实时数据信息，格式为
					//”>主键,数据类型，数据值，数据产生时间，数据持续时间（毫秒），车辆编号,车厢号, 数据值参数，站点名称,线路号,方向”。
					const std::string c_rt_file = "f_rt_info.txt";
					const std::string c_rt_file_rt = "f_rt_info_rt.txt";
					const std::string c_out_file = export_path + c_rt_file_rt;
					const std::string c_rt_file_dst = export_path + c_rt_file;
					char c_cmd[256];
					sprintf(c_cmd, "into outfile '%s' %s", 
						(NSP_STP_CM::str_sig_double(c_out_file, '\\')).c_str(), c_cmd_param.c_str());
					remove(c_out_file.c_str());

					char time_inf_beg[256];
					sprintf(time_inf_beg, "%04d-%02d-%02d %02d:%02d:%02d", 
						st_time_pre_rt.year, st_time_pre_rt.mon, st_time_pre_rt.day, st_time_pre_rt.hour, st_time_pre_rt.min, st_time_pre_rt.sec);
					sprintf(sql_cmd, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where  %s >= '%s' and %s < '%s' %s;", 
						gRtFKey.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), gRtFBeginTime.c_str(), 
						gRtFLastTime.c_str(), gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFDataParam.c_str(),
						gRtFStationName.c_str(), gRtFLineId.c_str(), gRtFDirection.c_str(),
						gTblRt.c_str(), 
						gRtFBeginTime.c_str(), time_inf_beg, 
						gRtFBeginTime.c_str(), time_now, 
						c_cmd
						);
					ret = p_CMySqlDB->WriteDataBase(sql_cmd);
					if (ret != 0)
					{
						gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
					else
					{
						const int file_len_rt = NSP_STP_CM::file_len(c_out_file);
						if (file_len_rt > 0)
						{
							ret = NSP_STP_CM::copyFile(c_out_file.c_str(), c_rt_file_dst.c_str(), false);
							if (ret != 0)
							{
								gLog.Add("%s copyFile %s to %s,ret=%d\n", __FUNCTION__, c_out_file.c_str(), c_rt_file_dst.c_str(), ret);
							}
							else
							{
								memcpy(&st_time_pre_rt, &st_time_now, sizeof(st_time_now));
							}
						}
					}
				}

				ret = TimeCompare(st_time_export_end, st_time_pre_record);
				if (ret > 0)
				{
					//t_record_info.txt为视频信息，格式为”>主键,视频路径，视频开始时间，视频结束时间”。
					const std::string c_record_file = "f_record_info.txt";
					const std::string c_record_file_rt = "f_record_info_rt.txt";
					const std::string c_out_file = export_path + c_record_file_rt;
					const std::string c_record_file_dst = export_path + c_record_file;
					char c_cmd[256];
					sprintf(c_cmd, "into outfile '%s' %s", 
						(NSP_STP_CM::str_sig_double(c_out_file, '\\')).c_str(), c_cmd_param.c_str());
					remove(c_out_file.c_str());

					char time_inf_beg[256];
					sprintf(time_inf_beg, "%04d-%02d-%02d %02d:%02d:%02d", 
						st_time_pre_record.year, st_time_pre_record.mon, st_time_pre_record.day, st_time_pre_record.hour, st_time_pre_record.min, st_time_pre_record.sec);
					sprintf(sql_cmd, "select %s, %s, %s, %s from %s where  %s >= '%s' and %s < '%s' %s;", 
						gRecordFKey.c_str(), gRecordFFullName.c_str(), gRecordFBeginTime.c_str(), gRecordFEndTime.c_str(),
						gTblRecord.c_str(), 
						gRecordFBeginTime.c_str(), time_inf_beg, 
						gRecordFBeginTime.c_str(), time_export_end, 
						c_cmd
						);
					ret = p_CMySqlDB->WriteDataBase(sql_cmd);
					if (ret != 0)
					{
						gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
					}
					else
					{
						const int file_len_rt = NSP_STP_CM::file_len(c_out_file);
						if (file_len_rt > 0)
						{
							ret = NSP_STP_CM::copyFile(c_out_file.c_str(), c_record_file_dst.c_str(), false);
							if (ret != 0)
							{
								gLog.Add("%s copyFile %s to %s,ret=%d\n", __FUNCTION__, c_out_file.c_str(), c_record_file_dst.c_str(), ret);
							}
							else
							{
								memcpy(&st_time_pre_record, &st_time_export_end, sizeof(st_time_export_end));
							}
						}
					}
				}
			}
		}


	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

unsigned int WINAPI CAlarm::thread_watchdog( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAlarm* p_this = (CAlarm*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;
	const int delt_m = 5 * 1000;
	NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
	p_CUdp->SetServerPort(9876);
	p_CUdp->SetServerIp("127.0.0.1");

	int pre_ms = ::GetTickCount();
	char send_msg[256];

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}
		
		const int nw_ms = ::GetTickCount();
		if (nw_ms - pre_ms > delt_m)
		{
			pre_ms = nw_ms;
			int send_len = sprintf(send_msg, "%d", nw_ms);
			p_CUdp->Send2Service(send_msg, send_len);
		}
	}

	delete p_CUdp;
	return 0;
}


