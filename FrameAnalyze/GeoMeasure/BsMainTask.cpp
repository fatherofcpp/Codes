#include "StdAfx.h"
#include "BsMainTask.h"

#include "Socket/CmTransUdp.h"
#include "GeoMeasure.h"
#include "GeoMeasureDlg.h"
#include "CmOpenCvDll.h"
#include "Log/CmLog.h"
#include "CmFunction/CmFunction.h"
#include<math.h>

std::string gVersion = "2017-11-3";

int BsMainTask::m_s_areaWith = 0;
int BsMainTask::m_s_areaHeight = 0;
int BsMainTask::m_s_top = 0;
int BsMainTask::m_s_bottom = 0;
int BsMainTask::m_s_left = 0;
int BsMainTask::m_s_rigth = 0;
int BsMainTask::m_s_threPoint = 0;

BsMainTask::BsMainTask(void)
{
	USES_CONVERSION;

	int ret = 0;
	//log init
	CString current_path = theApp.GetCurrentPath();
	current_path += "\\GeoMeasure.log";
	gLog.InitLog(W2A(current_path), "GeoMeasure");
	gLog.Add("%s ################Program Start################\n", __FUNCTION__);

	//version
#ifdef _DEBUG
	gLog.Add("%s %s Debug\n", __FUNCTION__, gVersion.c_str());
#else
	gLog.Add("%s %s Release\n", __FUNCTION__, gVersion.c_str());
#endif

	//cpu num
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	DWORD processors_num = system_info.dwNumberOfProcessors;
	int max_mask = (int)(0x01 << processors_num) - 1;
	gLog.Add("%s processors_num=%d\n", __FUNCTION__, processors_num);

	//net init
	ret = NSP_STP_CM::SocketInit();
	if(ret != 0)
	{
		gLog.Add("%s SocketInit\n", __FUNCTION__);
		exit(0);
	}

	//config
	ret = LoadConfig();
	if (ret != 0)
	{
		gLog.Add("%s LoadConfig,ret=%d\n", __FUNCTION__, ret);
		exit(0);
	}
}


BsMainTask::~BsMainTask(void)
{
}

unsigned int WINAPI BsMainTask::thread_recv_geo( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	BsMainTask* p_this = (BsMainTask*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}
	const int sleep_m = 20;
	int ret = 0;

	NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
	const int c_udp_recv = p_this->m_udpPortRecv;
	p_CUdp->SetLocalPort(c_udp_recv);
	timeval r_time_out;
	r_time_out.tv_sec = 0;
	r_time_out.tv_usec = 5 * 1000;
	p_CUdp->SetReadTimeOut(r_time_out);
	ret = p_CUdp->BindPort();
	if (ret != 0)
	{
		TRACE("%s p_CUdp->BindPort fail\n", __FUNCTION__);
		delete p_CUdp;
		return 0;
	}

	char rcv_buf[256];
	memset(rcv_buf, 0, sizeof(rcv_buf));
	memset(p_this->m_handleBuf, 0, sizeof(p_this->m_handleBuf));

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
	}

	return 0;
}

unsigned int WINAPI BsMainTask::thread_recv_handle_geo( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	BsMainTask* p_this = (BsMainTask*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;

	int ret = 0;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		//图片udp发送:{JH}&图片路径1&图片路径2&图片路径3
		//图片路径1代表29通道left，图片路径2代表30通道right，图片路径3代表23通道up

		if (strlen(p_this->m_handleBuf) > 0)
		{
			char file_path[E_CAMERA_COUNT][260];
			int geo_count = sscanf(p_this->m_handleBuf, "{JH}&%260[^&]&%260[^&]&%260[^&]", file_path[E_CAMERA_LEFT], file_path[E_CAMERA_RIGHT], file_path[E_CAMERA_UP]);
			memset(p_this->m_handleBuf, 0, sizeof(p_this->m_handleBuf));
			if (E_CAMERA_COUNT == geo_count)
			{
				memset(&p_this->m_geoArr, 0, sizeof(p_this->m_geoArr));

				double dg[E_CAMERA_COUNT] = {0};
				double lc[E_CAMERA_COUNT] = {0};

				for(int camera_ind = 0; camera_ind < E_CAMERA_COUNT; camera_ind ++)
				{
					bool file_exist = NSP_STP_CM::path_exist(file_path[camera_ind]);
					if (file_exist)
					{
						CAnalyzeGeo* p_geo_ana = &p_this->m_anaGeoArr[camera_ind];
						p_geo_ana->analyze_geo(file_path[camera_ind]);
						memcpy(&p_this->m_geoArr[camera_ind], &p_geo_ana->m_geo, sizeof(p_geo_ana->m_geo));

						CGeoMeasureDlg* p_CGeoMeasureDlg = (CGeoMeasureDlg*)theApp.m_pMainWnd;
						if (p_CGeoMeasureDlg && p_CGeoMeasureDlg->m_isContinueShow)
						{
							const int fps = 15;
							const int delay_tms = 1000 / fps;
							char msg[32];
							strcpy(msg, "DEFAULT");
							if (camera_ind == E_CAMERA_UP)
							{
								strcpy(msg, "导高相机");
							}
							else if (camera_ind == E_CAMERA_LEFT)
							{
								strcpy(msg, "补偿左相机");
							}
							else if (camera_ind == E_CAMERA_RIGHT)
							{
								strcpy(msg, "补偿右相机");
							}
							NSP_STP_CM::COpenCvDll cv_dll;
							cv_dll.ContinueImg(file_path[camera_ind], camera_ind * p_geo_ana->m_picHeight, 0.5, delay_tms, msg);
						}
					}
				}

				if(p_this->m_geoArr[E_CAMERA_UP].dg != 0 && p_this->m_geoArr[E_CAMERA_UP].lc != 0)
				{
					tag_geo geo_para;
					memcpy(&geo_para, &p_this->m_geoArr[E_CAMERA_UP], sizeof(geo_para));

					if(
						p_this->m_geoArr[E_CAMERA_LEFT].dg != 0 && p_this->m_geoArr[E_CAMERA_LEFT].lc != 0
						&& p_this->m_geoArr[E_CAMERA_RIGHT].dg != 0 && p_this->m_geoArr[E_CAMERA_RIGHT].lc != 0
						)
					{
						//启动补偿
						double delt_dg = abs(p_this->m_geoArr[E_CAMERA_LEFT].dg - p_this->m_geoArr[E_CAMERA_RIGHT].dg);
						double delt_lc = abs(p_this->m_geoArr[E_CAMERA_LEFT].lc - p_this->m_geoArr[E_CAMERA_RIGHT].lc);
						double third_leng = sqrt(delt_dg*delt_dg + delt_lc*delt_lc);
						double cos_a = 1;
						if ( 
							(delt_dg > -0.000001 && delt_dg < 0.000001)
							||
							(delt_lc > -0.000001 && delt_lc < 0.000001)
							||
							(third_leng > -0.000001 && third_leng < 0.000001)
							)
						{
							cos_a = 1;
						}
						else
						{
							cos_a = delt_lc / third_leng;
						}
						geo_para.dg *= cos_a;
						geo_para.lc *= cos_a;
					}
					else
					{
						//不启动补偿
					}

					p_this->m_geoContLock.Lock();
					unsigned int size = p_this->m_geoCont.size();
					if (size >= 15 * 60)
					{
						gLog.Add("%s size=%d\n", __FUNCTION__, size);
					}
					else
					{
						p_this->m_geoCont.push_back(geo_para);
					}
					p_this->m_geoContLock.Unlock();

					{
						char show_msg[128];
						sprintf(show_msg, "导高值=%.3f毫米;", geo_para.dg);
						CGeoMeasureDlg* p_CGeoMeasureDlg = dynamic_cast<CGeoMeasureDlg*>(theApp.m_pMainWnd);
						if(p_CGeoMeasureDlg)
						{
							p_CGeoMeasureDlg->AddDg(show_msg);
						}

						sprintf(show_msg, "拉出值=%.3f毫米;", geo_para.lc);
						if(p_CGeoMeasureDlg)
						{
							p_CGeoMeasureDlg->AddLc(show_msg);
						}

					}
				}
				else
				{
					//丢弃
				}
			}
			else
			{
				//丢弃
			}
		}
	}

	return 0;
}

unsigned int WINAPI BsMainTask::thread_send_geo( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	BsMainTask* p_this = (BsMainTask*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int c_fps = 15;//采集帧率15
	const int c_freq_m = 1000 / c_fps;
	const int sleep_m = c_freq_m / 2;

	NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
	const int c_udp_send_port = p_this->m_udpPortSend;
	p_CUdp->SetServerPort(c_udp_send_port);
	p_CUdp->SetServerIp(p_this->m_udpIpSend.c_str());

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		p_this->m_geoContLock.Lock();
		unsigned int size = p_this->m_geoCont.size();
		if (size > 0)
		{
			//"{JH_JG}&BY&BY&导高值1&拉出值1"
			st_geo geo = p_this->m_geoCont[0];
			p_this->m_geoCont.pop_front();

			double dg = geo.dg;
			double lc = geo.lc;
			char send_msg[256];
			sprintf(send_msg, "{JH_JG}&%s&%s&%.3f&%.3f", "BY", "BY", dg, lc);
			const int c_send_len = strlen(send_msg);
			int real_send_len = 0;
			real_send_len = p_CUdp->Send2Service(send_msg, c_send_len);
		}
		p_this->m_geoContLock.Unlock();


	}

	delete p_CUdp;
	return 0;
}

int BsMainTask::TaskStart()
{
	SetThreadCount(3);
	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_recv_geo", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_recv_geo, p_st_thread_param, 0xF);
	}
	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_recv_handle_geo", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_recv_handle_geo, p_st_thread_param, 0xF);
	}
	{
		int thread_ind = 2;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_send_geo", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_send_geo, p_st_thread_param, 0xF);
	}
	return 0;
}

int BsMainTask::LoadConfig()
{
	USES_CONVERSION;

	int ret = -1;

	const int SIZE = 1024;
	TCHAR Buffer[SIZE];
	memset(Buffer, 0, sizeof(Buffer));

	CString current_path = theApp.GetCurrentPath();
	CString strIniFile(current_path + _T("\\configGeoMeasure.ini"));

	CString strSection;

	strSection.Format(_T("system"));


	m_udpPortSend = GetPrivateProfileInt(strSection, _T("udpGeoSendPort"), -1, strIniFile);
	if (m_udpPortSend <= 0)
	{
		goto LEAVE;
	}

	GetPrivateProfileString(strSection, _T("udpGeoSendIp"), _T(""), Buffer, SIZE, strIniFile);
	m_udpIpSend = W2A(Buffer);
	if (m_udpIpSend.empty())
	{
		goto LEAVE;
	}

	m_udpPortRecv = GetPrivateProfileInt(strSection, _T("udpGeoRecvPort"), -1, strIniFile);
	if (m_udpPortRecv <= 0)
	{
		goto LEAVE;
	}

	int areaWith = GetPrivateProfileInt(strSection, _T("threAreaWith"), -1, strIniFile);
	if (areaWith <= 0)
	{
		goto LEAVE;
	}

	int areaHeight = GetPrivateProfileInt(strSection, _T("threAreaHeight"), -1, strIniFile);
	if (areaHeight <= 0)
	{
		goto LEAVE;
	}

	int err_threshold = -99999;
	int top = GetPrivateProfileInt(strSection, _T("threTop"), err_threshold, strIniFile);
	if (top == err_threshold)
	{
		goto LEAVE;
	}

	int bottom = GetPrivateProfileInt(strSection, _T("threBottom"), err_threshold, strIniFile);
	if (bottom == err_threshold)
	{
		goto LEAVE;
	}

	int left = GetPrivateProfileInt(strSection, _T("threLeft"), err_threshold, strIniFile);
	if (left == err_threshold)
	{
		goto LEAVE;
	}

	int right = GetPrivateProfileInt(strSection, _T("threRight"), err_threshold, strIniFile);
	if (right == err_threshold)
	{
		goto LEAVE;
	}

	int thre_point_gray = GetPrivateProfileInt(strSection, _T("threPointGray"), err_threshold, strIniFile);
	if (thre_point_gray == err_threshold)
	{
		goto LEAVE;
	}

	for(int camera_ind = 0; camera_ind < E_CAMERA_COUNT; camera_ind ++)
	{
		m_anaGeoArr[camera_ind].m_areaWith = areaWith;
		m_anaGeoArr[camera_ind].m_areaHeight = areaHeight;
		m_anaGeoArr[camera_ind].m_top = top;
		m_anaGeoArr[camera_ind].m_bottom = bottom;
		m_anaGeoArr[camera_ind].m_left = left;
		m_anaGeoArr[camera_ind].m_rigth = right;
		m_anaGeoArr[camera_ind].m_threPoint = thre_point_gray;
		m_anaGeoArr[camera_ind].m_cameraInd = camera_ind;
	}

	{
		m_s_areaWith = areaWith;
		m_s_areaHeight = areaHeight;
		m_s_top = top;
		m_s_bottom = bottom;
		m_s_left = left;
		m_s_rigth = right;
		m_s_threPoint = thre_point_gray;
	}

	ret = 0;
LEAVE:
	return ret;
}

void BsMainTask::TaskStop()
{
	gLog.Add("%s begin\n", __FUNCTION__);
	ThreadsStop();
	gLog.Add("%s end\n", __FUNCTION__);
}
