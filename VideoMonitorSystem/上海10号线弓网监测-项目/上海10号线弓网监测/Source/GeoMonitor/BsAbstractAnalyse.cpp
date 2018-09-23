#include "StdAfx.h"
#include "BsAbstractAnalyse.h"
#include "BsAlarm.h"
#include "Mvb/ServerTrans.h"
#include "CmFunction.h"
#include "BsDefClsFrameInf.h"
#include "CamPicture/CameraObject.h"
#include "DataBase/CmDBMySql.h"
#include "BsDefStruct.h"
#include "BsGlobal.h"
#include "Log/CmLog.h"
#include "BsFunction.h"
#include "GeoMonitor.h"
#include "GeoMonitorDlg.h"

CAbstractAnalyse::CAbstractAnalyse(void)
{
	m_pAbstractDeviceInfo = new Cam::CamDeviceInfo;
	m_pAbstractFormatInfo = new Cam::CamDeviceFormatInfo;
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
		sprintf(p_st_thread_param->threadName, "%s thread_store", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_store, p_st_thread_param, CpuMaskRemain());
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
	CAbstractAnalyse* p_this = (CAbstractAnalyse*)pParam;
	if (p_this == NULL)
	{
		return;
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
	fps_max = p_this->m_fps;

	if (fps > fps_max)
	{
		return;
	}
	else
	{
		p_this->m_frameCount ++;
	}

	char show_msg[64];
	CGeoMonitorDlg* p_CGeoMonitorDlg = dynamic_cast<CGeoMonitorDlg*>(theApp.m_pMainWnd);
	if(p_CGeoMonitorDlg)
	{
		if (p_this->m_cameraInd == E_CAM_CLASS_ARC_GEOUP)
		{
			sprintf(show_msg, "几何UP帧率=%.3f", fps);
			p_CGeoMonitorDlg->AddGeoUpFps(show_msg);
		}
		else if (p_this->m_cameraInd == E_CAM_CLASS_ARC_GEODOWNLEFT)
		{
			sprintf(show_msg, "几何DownL帧率=%.3f", fps);
			p_CGeoMonitorDlg->AddGeoDownLFps(show_msg);
		}
		else if (p_this->m_cameraInd == E_CAM_CLASS_ARC_GEODOWNRIGHT)
		{
			sprintf(show_msg, "几何DownR帧率=%.3f", fps);
			p_CGeoMonitorDlg->AddGeoDownRFps(show_msg);
		}
	}
	//fps


	const unsigned int size_anlyze = p_this->m_frameAnalyse.size();
	const unsigned int size_store = p_this->m_frameStore.size();

	if (p_this->m_cameraInd == E_CAM_CLASS_ARC_GEOUP
		|| p_this->m_cameraInd == E_CAM_CLASS_ARC_GEODOWNLEFT
		|| p_this->m_cameraInd == E_CAM_CLASS_ARC_GEODOWNRIGHT
		)
	{
		//do nothing
		if (size_anlyze >= p_this->m_maxFrameCount || size_store >= p_this->m_maxFrameCount)
		{
			return;
		}
	}
	else
	{
		if (size_anlyze >= p_this->m_maxFrameCount || size_store >= p_this->m_maxFrameCount)
		{
			return;
		}
	}


	int ret = 0;
	bool state_well = true;
	if (state_well)
	{
		unsigned int width = ((Cam::tagDeviceFormatInfo*)p_this->m_pAbstractFormatInfo)->width;
		unsigned int height = ((Cam::tagDeviceFormatInfo*)p_this->m_pAbstractFormatInfo)->height;

		std::string file_path = p_this->GetDownFullPath(false);

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
		else if (p_this->m_pAbstractDeviceInfo->provider == gCameraA80I)
		{
			frame_type = NSP_STP_CM::E_FRAME_FORMAT_JPEG;
		}
		else
		{
			frame_type = NSP_STP_CM::E_FRAME_FORMAT_JPEG;
		}

		if (frame_type == NSP_STP_CM::E_FRAME_FORMAT_BMP)
		{
			DWORD destfilelength = 0;
			char* p_jpeg = NULL;
			const int jpe_len = width * height * 4;
			p_jpeg = new MS_CHAR[jpe_len];
			memset(p_jpeg, 0, jpe_len);
			ret = NSP_STP_CM::bmp2jpg(width, height, pData, p_jpeg, jpe_len, destfilelength);
			if(ret == 0)
			{
				CFrameInf* p_frame_store = new CFrameInf(p_jpeg, destfilelength, pDataHeader);
				p_frame_store->m_width = width;
				p_frame_store->m_height = height;
				p_frame_store->m_frameType = NSP_STP_CM::E_FRAME_FORMAT_JPEG;
				p_frame_store->m_filePath = file_path;
				p_this->AddStoreFrame(p_frame_store);
			}
			delete[] p_jpeg; 
		}
		else if (frame_type == NSP_STP_CM::E_FRAME_FORMAT_JPEG)
		{
			CFrameInf* p_frame_store = new CFrameInf(pData, iDataLen, pDataHeader);
			p_frame_store->m_width = width;
			p_frame_store->m_height = height;
			p_frame_store->m_frameType = frame_type;
			p_frame_store->m_filePath = file_path;
			p_this->AddStoreFrame(p_frame_store);
		}
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
	const int c_fps = 50;
	const int sleep_m = 1000 / c_fps;
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
		if (suc_play)
		{
			int iRet = 0;
			if (1 == ((Cam::tagDeviceFormatInfo*)(p_this->m_pAbstractFormatInfo))->triggerMode
				&& 1 == ((Cam::tagDeviceFormatInfo*)(p_this->m_pAbstractFormatInfo))->triggerType)
			{
				iRet = p_this->m_pCObjectDevice->setOption(Cam::CAM_COMMAND_TRIGGER_SOFTWARE,
					"\r\n");
			}
		}
	}

	return 0;
}

unsigned int WINAPI CAbstractAnalyse::thread_store( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CAbstractAnalyse* p_this = (CAbstractAnalyse*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 50;

	int ret = 0;

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
			ret = p_this->Store(p_frameCBInf);
			if (ret == 0)
			{
				{
					p_this->UpdataFramePath(p_frameCBInf->m_filePath);

					CFrameInf* p_frame_analyse = new CFrameInf((char*)p_frameCBInf->m_pData, p_frameCBInf->m_dataLen, p_frameCBInf->m_pDataHeader);
					p_frame_analyse->m_width = p_frameCBInf->m_width;
					p_frame_analyse->m_height = p_frameCBInf->m_height;
					p_frame_analyse->m_frameType = p_frameCBInf->m_frameType;
					p_frame_analyse->m_filePath = p_frameCBInf->m_filePath;
					p_this->AddAnalyseFrame(p_frame_analyse);
				}
			}
			else
			{
				gLog.Add("%s Store,ret=%d\n", __FUNCTION__, ret);
			}
			delete p_frameCBInf;
		}
	}

	return 0;
}

unsigned int WINAPI CAbstractAnalyse::thread_analyse( void* pParam )
{
	return 0;
}
