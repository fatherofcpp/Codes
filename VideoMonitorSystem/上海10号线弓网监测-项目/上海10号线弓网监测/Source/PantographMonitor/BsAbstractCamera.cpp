#include "stdafx.h"
#include "BsAbstractCamera.h"
#include "BsAlarm.h"
#include "BsMvb.h"
#include "Mvb/ServerTrans.h"
#include "CmFunction.h"
#include "BsFunction.h"
#include "BsDefClsFrameInf.h"
#include "BsDefStruct.h"
#include "CamPicture/CameraObject.h"
#include "BsDllCamPicture.h"
#include "BsDllFrameAnalyze.h"
#include "BsDllCamVideo.h"
#include "CamVideo/VideoObjectDevSer.h"
#include "BsGlobal.h"
#include "Log/CmLog.h"
#include "CmOpenCvDll.h"
#include "BsTimeStamp.h"

CAbstractCamera::CAbstractCamera(void)
	: m_maxFrameCount(100)//结构4M*100=400M;
{
	m_pCMvb = NULL;
	m_pAbstractDeviceInfo = NULL;
	m_pAbstractFormatInfo = NULL;
	m_pCObjectService = NULL;
	m_pCObjectDevice = NULL;
	m_pDbMysqlParam = NULL;
	m_cameraInd = 0;
	m_frameAnalyzeType = E_FRAME_ANALYSE_TYPE_NONE;
	m_cameraHeartBeat = 0;
	m_cameraOnline = false;
	m_frameCount = 0;
	m_beginTimeStampMs = 0;
	m_useFrameAnalyze = false;
	m_pCameraOpt = new NSP_STP_CM::tagStCameraOpt;
	m_pJpg = new char[2048 * 2048 * 4];
}


CAbstractCamera::~CAbstractCamera(void)
{
	ReleaseRes();

	if (m_pAbstractDeviceInfo)
	{
		delete m_pAbstractDeviceInfo;
		m_pAbstractDeviceInfo = NULL;
	}
	if (m_pAbstractFormatInfo)
	{
		delete m_pAbstractFormatInfo;
		m_pAbstractFormatInfo = NULL;
	}
	delete m_pCameraOpt;
	delete[] m_pJpg;
}

void CAbstractCamera::AddStoreFrame( CFrameInf* p_frameCBInf )
{
	if (!p_frameCBInf)
	{
		return;
	}

	m_frameStoreLock.Lock();
	unsigned int size = m_frameStore.size();
	if (size >= m_maxFrameCount)
	{
		delete p_frameCBInf;
		gLog.Add("%s size=%d\n", __FUNCTION__, size);
	}
	else
	{
		m_frameStore.push_back(p_frameCBInf);
	}
	m_frameStoreLock.Unlock();
}

int CAbstractCamera::AddAnalyseFrame( CFrameInf* p_frameCBInf )
{
	int ret = 0;
	if (!p_frameCBInf)
	{
		return -1;
	}

	m_frameAnalyseLock.Lock();
	unsigned int size = m_frameAnalyse.size();
	if (size >= m_maxFrameCount)
	{
		delete p_frameCBInf;
		ret = -1;
		gLog.Add("%s size=%d\n", __FUNCTION__, size);
	}
	else
	{
		ret = 0;
		m_frameAnalyse.push_back(p_frameCBInf);
	}
	m_frameAnalyseLock.Unlock();
	return ret;
}

CFrameInf* CAbstractCamera::PickStoreFrame()
{
	CFrameInf* ele_pop = NULL;
	m_frameStoreLock.Lock();
	unsigned int size = m_frameStore.size();
	if (size > 0)
	{
		ele_pop = m_frameStore[0];
		m_frameStore.pop_front();
	}
	m_frameStoreLock.Unlock();
	return ele_pop;
}

CFrameInf* CAbstractCamera::PickAnalyzeFrame()
{
	CFrameInf* ele_pop = NULL;
	m_frameAnalyseLock.Lock();
	unsigned int size = m_frameAnalyse.size();
	if (size > 0)
	{
		ele_pop = m_frameAnalyse[0];
		m_frameAnalyse.pop_front();
	}
	m_frameAnalyseLock.Unlock();
	return ele_pop;
}

void CAbstractCamera::ReleaseRes()
{
	unsigned int size = 0;
	m_frameAnalyseLock.Lock();
	size = m_frameAnalyse.size();
	for (unsigned int i = 0; i < size; i ++)
	{
		CFrameInf* p_CFrameInf = m_frameAnalyse[i];
		delete p_CFrameInf;
	}
	m_frameAnalyse.clear();
	m_frameAnalyseLock.Unlock();

	m_frameStoreLock.Lock();
	size = m_frameStore.size();
	for (unsigned int i = 0; i < size; i ++)
	{
		CFrameInf* p_CFrameInf = m_frameStore[i];
		delete p_CFrameInf;
	}
	m_frameStore.clear();
	m_frameStoreLock.Unlock();
}

int CAbstractCamera::Store( CFrameInf* p_frameCBInf)
{
	int ret = -1;
	//store
	if (m_pCMvb)
	{
		std::string file_path = p_frameCBInf->m_filePath;
		const NSP_STP_CM::tag_e_frame_format frame_type = p_frameCBInf->m_frameType;
		if (frame_type == NSP_STP_CM::E_FRAME_FORMAT_BMP)
		{
			bool is_convert_bmp2jpg = true;
			DWORD destfilelength = 0;
			unsigned char* rgb = NULL;
			if (is_convert_bmp2jpg)
			{
				const int jpe_len = p_frameCBInf->m_width * p_frameCBInf->m_height * 4;
				//memset(m_pJpg, 0, jpe_len);

				if (p_frameCBInf->m_isColor)
				{
					//彩色
					unsigned char* yuv = (unsigned char*)p_frameCBInf->m_pData;
					const int rgb_len = p_frameCBInf->m_width * p_frameCBInf->m_height * 3;
					rgb = new unsigned char[rgb_len];
					NSP_STP_CM::convert_yuv_to_rgb_buffer(yuv, rgb, p_frameCBInf->m_width, p_frameCBInf->m_height);
					ret = NSP_STP_CM::bmp2jpg(p_frameCBInf->m_width, p_frameCBInf->m_height, 
									(const char*)rgb, m_pJpg, jpe_len, destfilelength, true);
					delete[] rgb;
				}
				else
				{
					ret = NSP_STP_CM::bmp2jpg(p_frameCBInf->m_width, p_frameCBInf->m_height, 
									(char*)p_frameCBInf->m_pData, m_pJpg, jpe_len, destfilelength);
				}

				if (ret == 0)
				{
					p_frameCBInf->m_frameType = NSP_STP_CM::E_FRAME_FORMAT_JPEG;
					NSP_STP_CM::file_write(file_path, (unsigned char*)m_pJpg, destfilelength);
				}
			}
			else
			{
				BITMAPINFO* pBitmapInfo = (BITMAPINFO*)(p_frameCBInf->m_pDataHeader);
				ret = NSP_STP_CM::SaveBmp((BYTE*)p_frameCBInf->m_pData, p_frameCBInf->m_width, p_frameCBInf->m_height, p_frameCBInf->m_dataLen, pBitmapInfo, file_path);
			}
		}
		else if (frame_type == NSP_STP_CM::E_FRAME_FORMAT_JPEG)
		{
			NSP_STP_CM::file_write(file_path, p_frameCBInf->m_pData, p_frameCBInf->m_dataLen);
		}
	}
	else
	{
		return -4;
	}

	return ret;
}

static int frame_analyse( IN const unsigned char* frame_data, 
	IN const int frame_len, 
	IN const int width, 
	IN const int height, 
	IN const NSP_STP_CM::tag_e_frame_format frame_format, 
	IN const tag_e_frame_analyse_type analyze_type,
	OUT tag_e_frame_warn_type& alarm_type, 
	OUT tag_e_frame_warn_cls& alarm_cls, 
	OUT std::string& alarm_msg, 
	OUT double& alarm_val)
{
	alarm_type = E_FRAME_WARN_TYPE_NORMAL;
	alarm_cls = E_FRAME_WARN_CLS_0;
	alarm_msg = "";
	alarm_val = 0.0;
	return 0;
}

int CAbstractCamera::AnalyseFrame( CFrameInf* p_frameCBInf)
{
	int ret = -1;
	const bool is_use_frame_dll = m_useFrameAnalyze;
	if (!is_use_frame_dll)
	{
		ret = frame_analyse(p_frameCBInf->m_pData, 
			p_frameCBInf->m_dataLen, 
			p_frameCBInf->m_width, 
			p_frameCBInf->m_height, 
			p_frameCBInf->m_frameType, 
			m_frameAnalyzeType, 
			p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_type,  
			p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_cls, 
			p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_msg, 
			p_frameCBInf->m_pAnalyzeResArr->resArr[0].alarm_val);
	}
	else
	{
		MVB::tag_st_mvb_recv_data* p_st_mvb_data = m_pCMvb->GetMvbRecvData();
		MVB::tag_st_station_info* p_station_now = m_pCMvb->GetCurrentStationInf();

		/*
		5. TrainStation:车辆站点信息
		TrainStation [0] 行车速度
		TrainStation [1] 行车方向 0为首站至末站 1为末站至首站
		TrainStation [2] 当前站点编号 记录当前区间的起点站编号（具体形式可根据实际情况定义）
		*/
		int train_station[3];
		train_station[0] = p_st_mvb_data->speed / 100;
		/*
		0x1 = upgoing，
		0x2 = downgoing,
		0x3 = upgoing2,
		0x4 = downgoing2,
		0x0 = notgoing
		*/
		if (p_st_mvb_data->direction == 0x1 || p_st_mvb_data->direction == 0x3)
		{
			train_station[1] = 0;
		}
		else if (p_st_mvb_data->direction == 0x2 || p_st_mvb_data->direction == 0x4)
		{
			train_station[1] = 1;
		}
		else
		{
			train_station[1] = 0;
		}
		train_station[2] = p_station_now->station_code;

		//step 2
		const bool load_local = false;
		if (load_local)
		{
			std::string file_path = "";
			unsigned char* pData = NULL;
			int width = 0;
			int height = 0;
			int frame_size = 0;
			bool is_load = NSP_STP_CM::LoadImgFrame(file_path, false, pData, width, height, frame_size);
			if (is_load)
			{
				ret = g_pCFrameAnalyzeDll->m_imgdetect_deal(
					pData, 
					frame_size, 
					p_frameCBInf->m_frameType, 
					file_path.c_str(),
					train_station, 
					p_frameCBInf->m_pAnalyzeResArr);

				delete[] pData;
				pData = NULL;
			}
		}
		else
		{
			ret = g_pCFrameAnalyzeDll->m_imgdetect_deal(
				p_frameCBInf->m_pData, 
				p_frameCBInf->m_dataLen, 
				p_frameCBInf->m_frameType, 
				p_frameCBInf->m_filePath.c_str(),
				train_station, 
				p_frameCBInf->m_pAnalyzeResArr);
		}

		if (ret == 1)
		{
			if (p_frameCBInf->m_pAnalyzeResArr->resNum == 0)
			{
				ret = 0;
			}
			else
			{
				ret = 1;
			}
		}
		else
		{
			ret = 0;
		}
	}

	return ret;
}

int CAbstractCamera::ThreadStateCheck( int thread_ind )
{
	bool is_thread_run = m_pCThreadArr[thread_ind].GetThreadStatus();
	if (!is_thread_run)
	{
		return NSP_STP_CM::E_THREAD_STATE_STOPED;
	}
	bool is_thread_pause = GetThreadPause();
	if (is_thread_pause)
	{
		return NSP_STP_CM::E_THREAD_STATE_PAUSED;
	}
	return NSP_STP_CM::E_THREAD_STATE_RUNNING;
}

bool CAbstractCamera::CheckWorkState()
{
	bool is_work = false;

	return is_work;
}

bool CAbstractCamera::PlayCam(unsigned int uUseCPU)
{
	bool b_ret = false;
	bool is_playing = false;
	int ret = -1;

	if (!m_pCObjectDevice)
	{
		return false;
	}

	is_playing = m_pCObjectDevice->isPlaying();
	if (!is_playing)
	{
		ret = m_pCObjectDevice->play(uUseCPU);
	}
	else
	{
		ret = 0;
	}

	b_ret = (0 == ret);
	return b_ret;
}

bool CAbstractCamera::StopCam()
{
	bool b_ret = false;
	bool is_stopped = false;
	int ret = -1;

	if (!m_pCObjectDevice)
	{
		return false;
	}

	is_stopped = m_pCObjectDevice->isStopped();
	if (!is_stopped)
	{
		ret = m_pCObjectDevice->stop();
	}
	else
	{
		ret = 0;
	}

	b_ret = (0 == ret);
	return b_ret;
}

bool CAbstractCamera::PauseCam()
{
	bool b_ret = false;
	bool is_paused = false;
	int ret = -1;

	if (!m_pCObjectDevice)
	{
		return false;
	}

	is_paused = m_pCObjectDevice->isPaused();
	if (!is_paused)
	{
		ret = m_pCObjectDevice->pause();
	}
	else
	{
		ret = 0;
	}

	b_ret = (0 == ret);
	return b_ret;
}

std::string CAbstractCamera::GetDownFullPath(bool is_video, int camera_ind)
{
	std::string file_path =  "";

	NSP_STP_CM::tag_st_time cur_time;
	//NSP_STP_CM::GetNowTime(&cur_time);
	CTimeStamp::GetInstance()->parseTime(&cur_time, CTimeStamp::GetInstance()->GetTimeStampMs());
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

	MVB::tag_st_mvb_recv_data* p_st_mvb_data = m_pCMvb->GetMvbRecvData();
	MVB::tag_st_station_info* p_station_now = m_pCMvb->GetCurrentStationInf();
	st_db_alarm db_alarm;
	db_alarm.CarriageId = p_st_mvb_data->train_num;
	db_alarm.CarriageNum = m_carriageNum;
	db_alarm.LineId = m_lineId;
	db_alarm.StationId = p_station_now->station_seq;
	db_alarm.Direction = p_st_mvb_data->direction;

	std::string suff = "";
	char _tmp_file_name[MAX_PATH] = {'\0'};
	char tmp_msg[MAX_PATH] = {'\0'};
	if (is_video)
	{
		suff = m_saveVideoSuffix;
	}
	else
	{
		suff = m_saveFrameSuffix;
	}

	sprintf(_tmp_file_name, "%s_%02d_%02d_%04d_%02d_%s_%d_%.03fKM",
		date_msg, m_cameraInd, db_alarm.LineId, db_alarm.CarriageId, db_alarm.CarriageNum, 
		p_station_now->station_alias, db_alarm.Direction, p_st_mvb_data->mileage * 0.01);
	sprintf(tmp_msg, "%s\\%02d%02d\\%02d", ymd, tm_hour, tm_min / 10 * 10, camera_ind);

	std::string folder_path = m_rootPath + tmp_msg + "\\";
	std::string file_name = _tmp_file_name;

	file_path = folder_path + file_name + suff;
	return file_path;
}

void CAbstractCamera::SetCameraInd( int camera_ind )
{
	m_cameraInd = camera_ind;
}

int CAbstractCamera::CameraPictureInit()
{
	////获取句柄
	Cam::CCamObjectService* p_service = g_pCCamPictureDll->m_pCCamObjectService;

	//得到所有相机
	char info[1024];
	memset(info, 0, sizeof(info));
	int iRet = ((CObjectService*)p_service)->getAvailableDevices(info);

	if (iRet != CAM_OK ||
		!findDevice(m_pAbstractDeviceInfo->provider.c_str(), info))
	{
		return -2;
	}

	Cam::CCamObjectDevice* p_device = dynamic_cast<Cam::CCamObjectDevice*>(p_service->createDevice(*m_pAbstractDeviceInfo, *m_pAbstractFormatInfo));
	if (p_device == NULL)
	{
		return -3;
	}

	Cam::CamDeviceInfo cam_deviceInfo;
	cam_deviceInfo = p_device->getDeviceInfo();

	m_pCObjectService = p_service;
	m_pCObjectDevice = p_device;

	SetCameraOpt();

	return 0;
}

int CAbstractCamera::CameraVideoInit()
{
	VideoNSP::CVideoObjectService* p_service = g_pCCamVideoDll->m_pCVideoObjectService;

	//得到所有相机
	char info[1024];
	memset(info, 0, sizeof(info));
	int iRet = ((CObjectService*)p_service)->getAvailableDevices(info);

	if (iRet != 0 ||
		!findDevice(m_pAbstractDeviceInfo->provider.c_str(), info))
	{
		return -2;
	}

	VideoNSP::CVideoObjectDevice* p_device = dynamic_cast<VideoNSP::CVideoObjectDevice*>(p_service->createDevice(*m_pAbstractDeviceInfo, *m_pAbstractFormatInfo));
	if (p_device == NULL)
	{
		return -3;
	}

	m_pCObjectService = p_service;
	m_pCObjectDevice = p_device;

	return 0;
}



