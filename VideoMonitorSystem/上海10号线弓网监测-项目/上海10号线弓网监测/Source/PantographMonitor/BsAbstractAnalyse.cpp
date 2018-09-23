#include "StdAfx.h"
#include "BsAbstractAnalyse.h"
#include "BsTemprateAnalyse.h"
#include "BsStructAnalyse.h"
#include "BsAlarm.h"
#include "BsMvb.h"
#include "Mvb/ServerTrans.h"
#include "CmFunction.h"
#include "BsDefClsFrameInf.h"
#include "CamPicture/CameraObject.h"
#include "DataBase/CmDBMySql.h"
#include "BsDefStruct.h"
#include "BsGlobal.h"
#include "Log/CmLog.h"
#include "BsFunction.h"
#include "PantographMonitor.h"
#include "PantographMonitorDlg.h"

CAbstractAnalyse::CAbstractAnalyse(void)
{
	m_pAbstractDeviceInfo = new Cam::CamDeviceInfo;
	m_pAbstractFormatInfo = new Cam::CamDeviceFormatInfo;
	m_preAnalyzMs = ::GetTickCount();
}


CAbstractAnalyse::~CAbstractAnalyse(void)
{
}

int CAbstractAnalyse::TaskStart()
{
	int ret = CameraPictureInit();
	if (ret != 0)
	{
		return -1;
	}

	SetThreadCount(3);
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
		sprintf(p_st_thread_param->threadName, "%s thread_store", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_store, p_st_thread_param, CpuMaskRemain());
	}
	{
		int thread_ind = 2;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_analyse", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_analyse, p_st_thread_param, CpuMaskRemain());
	}
	return 0;
}

void CAbstractAnalyse::SetDevInf( tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo )
{
	if (!p_AbstractDeviceInfo || !p_AbstractFormatInfo)
	{
		return;
	}
	*(dynamic_cast<Cam::CamDeviceInfo*>(m_pAbstractDeviceInfo)) = *(dynamic_cast<Cam::CamDeviceInfo*>(p_AbstractDeviceInfo));
	*(dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo)) = *(dynamic_cast<Cam::CamDeviceFormatInfo*>(p_AbstractFormatInfo));

	m_pAbstractFormatInfo->pReceiveData = &CAbstractAnalyse::frame_cb;
	m_pAbstractFormatInfo->pParam = this;
	m_pAbstractFormatInfo->pHeartbeatConnectedCallback = &CAbstractAnalyse::heart_beat_cb;
	m_pAbstractFormatInfo->pHeartbeatParam = this;
	(dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo))->pReceiveData2 = NULL;
	(dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo))->pParam2 = NULL;
}

void CALLBACK CAbstractAnalyse::frame_cb( const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader )
{
	USES_CONVERSION;

	CAbstractAnalyse* p_this = (CAbstractAnalyse*)pParam;
	if (p_this == NULL)
	{
		return;
	}

	{
		const unsigned int size_anlyze = p_this->m_frameAnalyse.size();
		const unsigned int size_store = p_this->m_frameStore.size();
		if (size_anlyze >= p_this->m_maxFrameCount || size_store >= p_this->m_maxFrameCount)
		{
			gLog.Add("%s size_anlyze=%d, size_store=%d,p_this->m_cameraInd=%d,drop\n", 
				__FUNCTION__, size_anlyze, size_store, p_this->m_cameraInd);
			return;
		}
	}

	//fps
	if (p_this->m_beginTimeStampMs == 0)
	{
		p_this->m_beginTimeStampMs = ::GetTickCount();
	}

	unsigned int cur_time_ms = ::GetTickCount();
	unsigned int toltal_time_ms = cur_time_ms - p_this->m_beginTimeStampMs;
	if (toltal_time_ms <= 0)
	{
		toltal_time_ms = 1;
	}
	float fps = (float)((float)(p_this->m_frameCount * 1000) / (float)toltal_time_ms);
	float fps_max = 0;
	if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
	{
		fps_max = p_this->m_pCameraOpt->fps;
	}
	else
	{
		fps_max = p_this->m_pCameraOpt->fps;
	}

	if (fps > fps_max)
	{
		//gLog.Add("%s fps=%.3f,p_this->m_cameraInd=%d,drop\n", __FUNCTION__, fps, p_this->m_cameraInd);
		return;
	}
	else
	{
		p_this->m_frameCount ++;
	}

	char show_msg[64];
	CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
	if(p_CPantographMonitorDlg)
	{
		const unsigned int size_anlyze = p_this->m_frameAnalyse.size();
		const unsigned int size_store = p_this->m_frameStore.size();
		if (p_this->m_cameraInd == E_CAM_CLASS_STRUCT)
		{
			sprintf(show_msg, "(监测%.3f,分析%d,存储%d)", fps, size_anlyze, size_store);
			p_CPantographMonitorDlg->AddStructFps(show_msg);
		}
		else if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
		{
			sprintf(show_msg, "(红外%.3f,分析%d,存储%d)", fps, size_anlyze, size_store);
			p_CPantographMonitorDlg->AddTempFps(show_msg);
		}
		else if (p_this->m_cameraInd == E_CAM_CLASS_ARC_COLLECT)
		{
			sprintf(show_msg, "(紫外%.3f)", fps);
			//p_CPantographMonitorDlg->AddArcCollectorFps(show_msg);
		}
	}
	//fps

	int ret = 0;
	bool state_well = p_this->m_pCMvb->GetMvbState();
	if (state_well)
	{
		unsigned int width = ((Cam::tagDeviceFormatInfo*)p_this->m_pAbstractFormatInfo)->width;
		unsigned int height = ((Cam::tagDeviceFormatInfo*)p_this->m_pAbstractFormatInfo)->height;
		unsigned int iStreamType = ((Cam::tagDeviceFormatInfo*)p_this->m_pAbstractFormatInfo)->iStreamType;

		std::string file_path = p_this->GetDownFullPath(false, p_this->m_cameraInd);
		std::string folder_path = NSP_STP_CM::str_right_cut_tag(file_path, "\\");
		if (!folder_path.empty())
		{
			NSP_STP_CM::folder_create(folder_path);
		}

		NSP_STP_CM::tag_e_frame_format frame_type = NSP_STP_CM::E_FRAME_FORMAT_JPEG;
		if (p_this->m_pAbstractDeviceInfo->provider == gCameraThermoGroup)
		{
			frame_type = NSP_STP_CM::E_FRAME_FORMAT_BMP;
		}
		else if (p_this->m_pAbstractDeviceInfo->provider == gCameraBasler)
		{
			frame_type = NSP_STP_CM::E_FRAME_FORMAT_BMP;
		}
		else if (p_this->m_pAbstractDeviceInfo->provider == gCameraGX23D)
		{
			frame_type = NSP_STP_CM::E_FRAME_FORMAT_BMP;
		}
		else
		{
			frame_type = NSP_STP_CM::E_FRAME_FORMAT_JPEG;
		}

		CFrameInf* p_frame_store = new CFrameInf(pData, iDataLen, pDataHeader);
		p_frame_store->m_width = width;
		p_frame_store->m_height = height;
		p_frame_store->m_frameType = frame_type;
		p_frame_store->m_filePath = file_path;
		p_frame_store->m_isColor = (iStreamType == NSP_BS_CM::PixelFormat_YUV422_YUYV_Packed);
		if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
		{
			CThermoGroupDeviceEx* pDevice =  dynamic_cast<CThermoGroupDeviceEx*>(p_this->m_pCObjectDevice);
			p_frame_store->m_isTempSave = pDevice->saveBMP(0, A2W(file_path.c_str())); 

			const bool is_save_temp_map = true;
			if(is_save_temp_map)
			{
				std::string file_no_suf = NSP_STP_CM::str_right_cut_tag(file_path, p_this->m_saveFrameSuffix);
				std::string temp_map_file_path = file_no_suf + p_this->m_tempMapFrameSuffix;
				p_frame_store->m_isTempMapSave = pDevice->saveDDT(A2W(temp_map_file_path.c_str()));
			}

			CTemprateAnalyse* p_CTemprateAnalyse = dynamic_cast<CTemprateAnalyse*>(p_this);
			p_CTemprateAnalyse->GetTemp(p_frame_store);

		}
		p_this->AddStoreFrame(p_frame_store);
	}
	else
	{
		//丢弃
		return;
	}
}

void CALLBACK CAbstractAnalyse::heart_beat_cb( const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader )
{
	CAbstractAnalyse* p_this = (CAbstractAnalyse*)pParam;
	if (p_this == NULL)
	{
		return;
	}

	p_this->m_cameraHeartBeat = (unsigned int)(p_this->m_cameraHeartBeat + 1);
}

unsigned int WINAPI CAbstractAnalyse::thread_pull( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAbstractAnalyse* p_this = (CAbstractAnalyse*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;
	bool is_temp_inited = false;
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

		unsigned int cpu_mask = 0xF;
		if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
		{
			//cpu_mask = 0x03;
		}
		else if (p_this->m_cameraInd == E_CAM_CLASS_STRUCT)
		{
			//cpu_mask = 0x0C;
		}

		bool suc_play = p_this->PlayCam(cpu_mask);
		if (suc_play)
		{
			if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
			{
				if (!is_temp_inited)
				{
					is_temp_inited = true;

					struct_FixPara param;
					param.fDistance = 20.0f;
					param.fEmissivity = 1.0f;						//最关键参数
					param.fTemp = 20.0f;
					param.fRH = 0.4f;
					param.fVisDistance = 20.0f;
					param.fRain = 0.0f;
					param.fSnow = 0.0f;
					param.fExtrapara1 = 0.0f;
					param.fExtrapara2 = 0.0f;
					param.fTaoAtm = 0.99724f;
					param.fTaoFilter = 1.0f;

					CThermoGroupDeviceEx* pDevice =  dynamic_cast<CThermoGroupDeviceEx*>(p_this->m_pCObjectDevice);
					//pDevice->setFixPara(&param, false);//设置修正参数
				}
			}
		}
	}

	return 0;
}

unsigned int WINAPI CAbstractAnalyse::thread_store( void* pParam )
{
	USES_CONVERSION;
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAbstractAnalyse* p_this = (CAbstractAnalyse*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 1;

	int ret = 0;
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	int analyze_struct_ind = 0;
	while(1)
	{
		Sleep(sleep_m);

		int check_res = p_this->ThreadStateCheck(thread_ind);
		if (check_res == NSP_STP_CM::E_THREAD_STATE_STOPED)
		{
			break;
		}

		CFrameInf* p_frameCBInf = p_this->PickStoreFrame();
		if (p_frameCBInf)
		{
			bool is_store = false;
			const unsigned int c_size_store = p_this->m_frameStore.size();
			if (c_size_store >= p_this->m_maxFrameCount)
			{
				is_store = false;
				gLog.Add("%s c_size_store=%d,drop\n", __FUNCTION__, c_size_store);
			}
			else
			{
				is_store = true;
			}

			if (is_store)
			{
				if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
				{
					if (p_frameCBInf->m_isTempSave)
					{
						ret = 0;
					}
					else
					{
						ret = -1;
					}
				}
				else
				{
					ret = p_this->Store(p_frameCBInf);
				}
				if (ret == 0)
				{
					p_this->UpdataFramePath(p_frameCBInf->m_filePath);

					bool is_add_analyze = true;
					if (p_this->m_cameraInd == E_CAM_CLASS_STRUCT)
					{
						analyze_struct_ind ++;
						unsigned int size = p_this->m_frameAnalyse.size();
						if (size < 5)
						{
							if (analyze_struct_ind >= 5)
							{
								analyze_struct_ind = 0;
								is_add_analyze = true;
							}
							else
							{
								is_add_analyze = false;
							}
						}
						else
						{
							is_add_analyze = false;
						}
					}
					else
					{
						is_add_analyze = true;
					}

					if (is_add_analyze)
					{
						CFrameInf* p_frame_analyse = new CFrameInf((char*)p_frameCBInf->m_pData, p_frameCBInf->m_dataLen, p_frameCBInf->m_pDataHeader);
						p_frame_analyse->m_width = p_frameCBInf->m_width;
						p_frame_analyse->m_height = p_frameCBInf->m_height;
						p_frame_analyse->m_frameType = p_frameCBInf->m_frameType;
						p_frame_analyse->m_filePath = p_frameCBInf->m_filePath;
						p_frame_analyse->m_isTempSave = p_frameCBInf->m_isTempSave;
						p_frame_analyse->m_isTempMapSave = p_frameCBInf->m_isTempMapSave;
						p_frame_analyse->m_isTempAlarm = p_frameCBInf->m_isTempAlarm;
						p_frame_analyse->m_isColor = p_frameCBInf->m_isColor;
						p_frame_analyse->m_tempLow = p_frameCBInf->m_tempLow;
						p_frame_analyse->m_tempHigh = p_frameCBInf->m_tempHigh;
						p_frame_analyse->m_tempAvarage = p_frameCBInf->m_tempAvarage;
						p_frame_analyse->m_tempHignLeftUpX = p_frameCBInf->m_tempHignLeftUpX;
						p_frame_analyse->m_tempHignLeftUpY = p_frameCBInf->m_tempHignLeftUpY;
						p_this->AddAnalyseFrame(p_frame_analyse);
					}
				}
				else
				{
					gLog.Add("%s Store,ret=%d\n", __FUNCTION__, ret);
				}
			}
			
			delete p_frameCBInf;
		}
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

static void inset_normal(CAbstractAnalyse* p_this, const CFrameInf* p_frameCBInf, NSP_STP_CM::CMySqlDB* p_CMySqlDB)
{
	char sql_cmd[1024*2];
	MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();
	MVB::tag_st_station_info* p_station_now = p_this->m_pCMvb->GetCurrentStationInf();
	st_db_alarm db_alarm;
	db_alarm.CameraPos = p_this->m_cameraInd;
	db_alarm.CarriageId = p_st_mvb_data->train_num;
	db_alarm.CarriageNum = p_this->m_carriageNum;
	db_alarm.StationId = p_station_now->station_seq;
	db_alarm.LineId = p_this->m_lineId;
	db_alarm.Direction = p_st_mvb_data->direction;
	db_alarm.FileName = NSP_STP_CM::str_left_cut_tag( p_frameCBInf->m_filePath, "\\");
	db_alarm.FullName = p_frameCBInf->m_filePath;
	db_alarm.BeginTime = gDbTimeNow;
	double milage = (double)(p_st_mvb_data->mileage) * 0.01;

	//插入数据
	sprintf(sql_cmd, "insert into  %s  ( %s, %s,  %s, %s, %s,  %s,  %s,  %s,  %s,  %s) \
					 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %s, %.03f);",
					 gTblAlarm.c_str(), 
					 gAlarmFCameraPos.c_str(), gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), 
					 gAlarmFLineId.c_str(), gAlarmFDirection.c_str(), gAlarmFFileName.c_str(), gAlarmFFullName.c_str(), 
					 gAlarmFBeginTime.c_str(), gAlarmFMileage.c_str(),
					 db_alarm.CameraPos, db_alarm.CarriageId, db_alarm.CarriageNum, db_alarm.StationId , 
					 db_alarm.LineId, db_alarm.Direction, db_alarm.FileName.c_str(), (NSP_STP_CM::str_sig_double(db_alarm.FullName, '\\')).c_str(), 
					 db_alarm.BeginTime.c_str(), milage                    
					 );
	int ret = p_CMySqlDB->WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
	}
}

unsigned int WINAPI CAbstractAnalyse::thread_analyse( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAbstractAnalyse* p_this = (CAbstractAnalyse*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 5;

	int ret = 0;
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = p_this->m_pDbMysqlParam;
	p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
		p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

	int frame_ind = 0;
	const int c_frame_delt_num = 20;//存储间隔帧数
	int alarm_delt_ms = 5*1000;
	int pre_alarm_ms = ::GetTickCount();
	pre_alarm_ms -= (alarm_delt_ms*2);
	while(1)
	{
		Sleep(sleep_m);

		int check_res = p_this->ThreadStateCheck(thread_ind);
		if (check_res == NSP_STP_CM::E_THREAD_STATE_STOPED)
		{
			break;
		}

		if (p_this->m_pAbstractDeviceInfo->provider == gCameraGX23D)
		{
			continue;
		}

		CFrameInf* p_frameCBInf = p_this->PickAnalyzeFrame();
		if (p_frameCBInf)
		{
			const unsigned int c_size_anlyze = p_this->m_frameAnalyse.size();
			bool is_analyze = false;
			int analyze_ret = 0;

			//filter
			if (p_this->m_cameraInd == E_CAM_CLASS_STRUCT)
			{
				if (c_size_anlyze <= 1)
				{
					is_analyze = true;
				}
				else
				{
					is_analyze = false;
				}
			}
			else if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
			{
				frame_ind ++;
				if (frame_ind >= c_frame_delt_num)
				{
					frame_ind = 0;
					is_analyze = true;

					analyze_ret = p_this->AnalyzeFrameTemp(p_CMySqlDB, p_frameCBInf);
					if (analyze_ret == 0)
					{
						//删除温度映射文件
						std::string tmp_old_file_no_suf = NSP_STP_CM::str_right_cut_tag(p_frameCBInf->m_filePath, p_this->m_saveFrameSuffix);
						std::string temp_old_map_file_path = tmp_old_file_no_suf + p_this->m_tempMapFrameSuffix;
						NSP_STP_CM::file_delete(temp_old_map_file_path);
					}

					int nw_ms = ::GetTickCount();
					if (nw_ms - pre_alarm_ms > alarm_delt_ms)
					{
						is_analyze = true;
					}
				}
				else
				{
					is_analyze = false;
				}
			}

			std::string structure_frame_path = "";
			if (is_analyze)
			{
				bool is_file_stored = NSP_STP_CM::path_exist(p_frameCBInf->m_filePath);
				if (is_file_stored)
				{
					if (p_this->m_cameraInd == E_CAM_CLASS_STRUCT)
					{
						analyze_ret = p_this->AnalyseFrame(p_frameCBInf);
						CStructAnalyse* p_CStructAnalyse = dynamic_cast<CStructAnalyse*>(p_this);
						structure_frame_path = p_CStructAnalyse->GetFramePath();
					}
					else if (p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
					{
						CTemprateAnalyse* p_CTemprateAnalyse = dynamic_cast<CTemprateAnalyse*>(p_this);
						structure_frame_path = p_CTemprateAnalyse->GetStructAnalyse()->GetFramePath();
						if (analyze_ret != 0 && !structure_frame_path.empty())
						{
							pre_alarm_ms = ::GetTickCount();
						}
					}

					if (analyze_ret != 0
						&& !structure_frame_path.empty())
					{
						for (int frame_anlyze_ind = 0; frame_anlyze_ind < p_frameCBInf->m_pAnalyzeResArr->resNum; frame_anlyze_ind ++)
						{
							AnalyzeRes* p_AnalyzeRes = &p_frameCBInf->m_pAnalyzeResArr->resArr[frame_anlyze_ind];

							UpdataMvbAlarmByFrame(p_this->m_pCMvb->GetMvbAlarmData(), p_this->m_pCMvb->GetMvbAlarmLock(), 
								p_AnalyzeRes->alarm_type, p_AnalyzeRes->alarm_cls, false);

							ret = FormatAlarmMessage(p_AnalyzeRes->alarm_type, p_AnalyzeRes->alarm_cls, 
								p_AnalyzeRes->alarm_val, p_AnalyzeRes->alarm_msg);

							if (ret == 0)
							{
								MVB::tag_st_mvb_recv_data* p_st_mvb_data = p_this->m_pCMvb->GetMvbRecvData();
								MVB::tag_st_station_info* p_station_now = p_this->m_pCMvb->GetCurrentStationInf();
								st_db_alarm db_alarm;
								db_alarm.CameraPos = p_this->m_cameraInd;
								db_alarm.CarriageId = p_st_mvb_data->train_num;
								db_alarm.CarriageNum = p_this->m_carriageNum;
								db_alarm.StationId = p_station_now->station_seq;
								db_alarm.LineId = p_this->m_lineId;
								db_alarm.Direction = p_st_mvb_data->direction;
								db_alarm.FileName = NSP_STP_CM::str_left_cut_tag( p_frameCBInf->m_filePath, "\\");
								db_alarm.FullName = p_frameCBInf->m_filePath;
								db_alarm.BeginTime = gDbTimeNow;
								double milage = (double)(p_st_mvb_data->mileage) * 0.01;

								db_alarm.AlarmType = p_AnalyzeRes->alarm_type;
								db_alarm.AlarmLevel = p_AnalyzeRes->alarm_cls;
								db_alarm.AlarmMessage = p_AnalyzeRes->alarm_msg;
								db_alarm.AlarmValue = p_AnalyzeRes->alarm_val;

								//修改图片名称
								char new_file_path[MAX_PATH];
								std::string old_file_name = NSP_STP_CM::str_left_cut_tag( p_frameCBInf->m_filePath, p_this->m_rootPath);
								std::string file_no_suf = NSP_STP_CM::str_right_cut_tag(old_file_name, p_this->m_saveFrameSuffix);
								std::string old_file_no_suf = p_this->m_rootPathAlarm + file_no_suf;
								sprintf(new_file_path, "%s_%d_%02d%s", old_file_no_suf.c_str(), db_alarm.AlarmType, db_alarm.AlarmLevel, p_this->m_saveFrameSuffix.c_str());
								db_alarm.FullName = new_file_path;
								db_alarm.FileName = NSP_STP_CM::str_left_cut_tag( db_alarm.FullName, "\\");

								//故障拷贝
								{
									//剪切温度映射文件
									if(p_this->m_cameraInd == E_CAM_CLASS_TEMPRATE)
									{
										std::string tmp_old_file_no_suf = NSP_STP_CM::str_right_cut_tag(p_frameCBInf->m_filePath, p_this->m_saveFrameSuffix);
										std::string temp_old_map_file_path = tmp_old_file_no_suf + p_this->m_tempMapFrameSuffix;
										std::string tmp_new_file_no_suf = NSP_STP_CM::str_right_cut_tag(new_file_path, p_this->m_saveFrameSuffix);
										std::string temp_new_map_file_path = tmp_new_file_no_suf + p_this->m_tempMapFrameSuffix;
										ret = NSP_STP_CM::renameFile(temp_old_map_file_path.c_str(), temp_new_map_file_path.c_str());
									}

									ret = NSP_STP_CM::copyFile(p_frameCBInf->m_filePath.c_str(), new_file_path, true);
									if (ret == 0)
									{
										//新增一条数据库
										char alarm_video_full_name[MAX_PATH] = "";
										char alarm_video_file_name[MAX_PATH] = "";
										{
											//std::string alarm_file_name_no_suf = NSP_STP_CM::str_right_cut_tag(db_alarm.FileName, p_this->m_saveFrameSuffix);
											//sprintf(alarm_video_file_name, "%s%s", alarm_file_name_no_suf.c_str(), p_this->m_saveVideoSuffix.c_str());
											//std::string videoRootAlarmPath = "D:\\pantoMonitorPath\\videoAlarmPath\\";
											//std::string videoRootPath = "D:\\pantoMonitorPath\\videoDownPath\\";
											//std::string video_folder = videoRootAlarmPath;
											//std::string date_inf = NSP_STP_CM::str_right_cut_tag(video_full_name, "\\");
											//date_inf = NSP_STP_CM::str_left_cut_tag(date_inf, videoRootPath);
											//sprintf(alarm_video_full_name, "%s%s\\%s", video_folder.c_str(), date_inf.c_str(), alarm_video_file_name);
										}
										//插入数据
										sprintf(sql_cmd, "insert into  %s  ( %s, %s,  %s, %s, %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s,  %s, %s) \
														 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %s, %.03f, %d, %d, '%s', %.03f,  '%s',  '%s',  '%s',  '%s');",
														 gTblAlarm.c_str(), 
														 gAlarmFCameraPos.c_str(), gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), 
														 gAlarmFLineId.c_str(), gAlarmFDirection.c_str(), gAlarmFFileName.c_str(), gAlarmFFullName.c_str(), 
														 gAlarmFBeginTime.c_str(), gAlarmFMileage.c_str(), gAlarmFAlarmType.c_str(), gAlarmFAlarmLevel.c_str(),  
														 gAlarmFAlarmMessage.c_str(), gAlarmFAlarmValue.c_str(), gAlarmFAlarmValueParam.c_str(), gAlarmFStuctureFullName.c_str(),
														 gAlarmFVideoFullName.c_str(), gAlarmFVideoFileName.c_str(),
														 db_alarm.CameraPos, db_alarm.CarriageId, db_alarm.CarriageNum, db_alarm.StationId , 
														 db_alarm.LineId, db_alarm.Direction, db_alarm.FileName.c_str(), (NSP_STP_CM::str_sig_double(db_alarm.FullName, '\\')).c_str(), 
														 db_alarm.BeginTime.c_str(), milage, db_alarm.AlarmType, db_alarm.AlarmLevel, 
														 db_alarm.AlarmMessage.c_str(), db_alarm.AlarmValue, p_AnalyzeRes->alarm_val_param.c_str(), (NSP_STP_CM::str_sig_double(structure_frame_path, '\\')).c_str(),
														 (NSP_STP_CM::str_sig_double(alarm_video_full_name, '\\')).c_str(), alarm_video_file_name
														 );
										ret = p_CMySqlDB->WriteDataBase(sql_cmd);
										if (ret != 0)
										{
											gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
										}
									}
									else
									{
										gLog.Add("%s copyFile %s to %s fail,ret=%d\n", __FUNCTION__, p_frameCBInf->m_filePath.c_str(), new_file_path, ret);
									}
								}
							}
							else
							{
								gLog.Add("%s FormatAlarmMessage fail,ret=%d\n", __FUNCTION__, ret);
							}
						}
					}
					else
					{
						//insert normal
						inset_normal(p_this, p_frameCBInf, p_CMySqlDB);
					}
				}
			}
			else
			{
				//insert normal
				inset_normal(p_this, p_frameCBInf, p_CMySqlDB);
			}
			delete p_frameCBInf;
		}
	}

	delete p_CMySqlDB;
	delete[] sql_cmd;
	return 0;
}

void CAbstractAnalyse::SetCameraCmOpt()
{
	int iRet = 0;

	unsigned int fps = (unsigned int)((dynamic_cast<Cam::CamDeviceFormatInfo*>(m_pAbstractFormatInfo))->fps);
	//int iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_FRAME_CYCLE, 
	//	"%u\r\n", fps);

	//return;

	if (m_pCameraOpt->isUseExposeCmd)
	{
		if (m_pCameraOpt->isAutoExpose)
		{
			while (1)
			{
				//设置自动曝光
				iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_AUTO_EXPOSURE, 
					"%d\r\n", 1);
				if (iRet == 0)
				{
					break;
				}
				else
				{
					gLog.Add("%s CAM_COMMAND_AUTO_EXPOSURE 1, fail\n", __FUNCTION__);
					Sleep(1000);
				}
			}
		}
		else
		{
			while (1)
			{
				//设置自动曝光
				iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_AUTO_EXPOSURE, 
					"%d\r\n", 0);
				if (iRet == 0)
				{
					break;
				}
				else
				{
					gLog.Add("%s CAM_COMMAND_AUTO_EXPOSURE 0, fail\n", __FUNCTION__);
					Sleep(1000);
				}
			}

			while (1)
			{
				//设置曝光时间
				const int exposure_time_us = m_pCameraOpt->exposeTimeUs;
				iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_EXPOSURE_TIME, 
					"%d\r\n", exposure_time_us);
				if (iRet == 0)
				{
					break;
				}
				else
				{
					gLog.Add("%s CAM_COMMAND_EXPOSURE_TIME exposure_time_us=%d, fail\n", __FUNCTION__, exposure_time_us);
					Sleep(1000);
				}
			}
		}
	}


	if (m_pCameraOpt->isUseGainCmd)
	{
		if (m_pCameraOpt->isAutoGain)
		{
			while (1)
			{
				iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_AUTO_GAIN, "%d\r\n", 2);
				if (iRet == 0)
				{
					break;
				}
				else
				{
					gLog.Add("%s CAM_COMMAND_AUTO_GAIN 2, fail\n", __FUNCTION__);
					Sleep(1000);
				}
			}
		}
		else
		{
			while (1)
			{
				iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_AUTO_GAIN, "%d\r\n", 0);
				if (iRet == 0)
				{
					break;
				}
				else
				{
					gLog.Add("%s CAM_COMMAND_AUTO_GAIN 0, fail\n", __FUNCTION__);
					Sleep(1000);
				}
			}

			while (1)
			{
				//设置自动增益范围
				//iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_AUTO_GAIN_RANGE, "0 10\r\n");
				iRet = 0;
				if (iRet == 0)
				{
					break;
				}
				else
				{
					gLog.Add("%s CAM_COMMAND_AUTO_GAIN_RANGE, fail\n", __FUNCTION__);
					Sleep(1000);
				}
			}

			while (1)
			{
				//设置益值
				const int gain_val = m_pCameraOpt->gainVal;
				iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_GAIN_VALUE, 
					"%d\r\n", gain_val);
				if (iRet == 0)
				{
					break;
				}
				else
				{
					gLog.Add("%s CAM_COMMAND_GAIN_VALUE gain_val=%d, fail\n", __FUNCTION__, gain_val);
					Sleep(1000);
				}
			}
		}
	}
}
