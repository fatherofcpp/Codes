#include "StdAfx.h"
#include "BsGeoSender.h"
#include "Bsgeocoltor.h"
#include "BsFunction.h"
#include "BsDefClsFrameInf.h"
#include "Socket/CmTransUdp.h"

/*
图片udp发送:{JH}&图片路径1&图片路径2&图片路径3

几何参数值返回:{JH_JG}&BY1&BY2&导高值&拉出值

图片路径1代表29通道，图片路径2代表30通道，图片路径3代表23通道
*/

CGeoSender::CGeoSender(void)
{
	m_pFrameUp = NULL;
	m_pFrameDownLeft = NULL;
	m_pFrameDownRight = NULL;
}


CGeoSender::~CGeoSender(void)
{

}

void CGeoSender::SetGeoCamera( CGeoColtor* geo_up, CGeoColtor* geo_down_left, CGeoColtor* geo_down_right )
{
	m_pCGeoColtorUp = geo_up;
	m_pCGeoColtorDownLeft = geo_down_left;
	m_pCGeoColtorDownRight = geo_down_right;
}

unsigned int WINAPI CGeoSender::thread_send_geo( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CGeoSender* p_this = (CGeoSender*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int c_fps = 15;//采集帧率15
	const int c_freq_m = 1000 / c_fps;
	const int sleep_m = c_freq_m / 2;

	const int c_check_t_ms = c_freq_m / 2;
	const int c_check_sleep_ms = c_check_t_ms / 5;
	const int c_check_count = c_check_t_ms / c_check_sleep_ms;

	NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
	p_CUdp->SetServerPort(p_this->m_udpGeoPort);
	p_CUdp->SetServerIp("127.0.0.1");

	NSP_STP_CM::CUdp* p_CUdp_down = new NSP_STP_CM::CUdp;
	p_CUdp_down->SetServerPort(p_this->m_udpGeoPortDown);
	p_CUdp_down->SetServerIp("127.0.0.1");

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		p_this->m_pFrameUp = p_this->m_pCGeoColtorUp->PickAnalyzeFrame();

		if (p_this->m_pFrameUp)
		{
			int count = 0;

			//find left
			while(1)
			{
				p_this->m_pFrameDownLeft = p_this->m_pCGeoColtorDownLeft->PickAnalyzeFrame();
				if (p_this->m_pFrameDownLeft)
				{
					break;
				}

				if (++count > c_check_count)
				{
					break;
				}

				Sleep(c_check_sleep_ms);
			}

			//find right
			while(1)
			{
				p_this->m_pFrameDownRight = p_this->m_pCGeoColtorDownRight->PickAnalyzeFrame();
				if (p_this->m_pFrameDownRight)
				{
					break;
				}

				if (++count > c_check_count)
				{
					break;
				}

				Sleep(c_check_sleep_ms);
			}
		}
		else
		{
			p_this->m_pFrameDownLeft = p_this->m_pCGeoColtorDownLeft->PickAnalyzeFrame();
			p_this->m_pFrameDownRight = p_this->m_pCGeoColtorDownRight->PickAnalyzeFrame();
		}

		if (p_this->m_pFrameUp)
		{
			std::string file_up = p_this->m_pFrameUp->m_filePath;
			std::string file_down_left = "spare";
			std::string file_down_right = "spare";
			if (p_this->m_pFrameDownLeft)
			{
				file_down_left = p_this->m_pFrameDownLeft->m_filePath;
			}
			if (p_this->m_pFrameDownRight)
			{
				file_down_right = p_this->m_pFrameDownRight->m_filePath;
			}
			//图片udp发送:{JH}&图片路径1&图片路径2&图片路径3
			//图片路径1代表29通道left，图片路径2代表30通道right，图片路径3代表23通道up
			char send_msg[256];
			sprintf(send_msg, "{JH}&%s&%s&%s", file_down_left.c_str(), file_down_right.c_str(), file_up.c_str());
			const int c_send_len = strlen(send_msg);
			int real_send_len = 0;
			real_send_len = p_CUdp->Send2Service(send_msg, c_send_len);
			real_send_len = p_CUdp_down->Send2Service(send_msg, c_send_len);
			real_send_len = 0;
		}


		if (p_this->m_pFrameUp)
		{
			delete p_this->m_pFrameUp;
			p_this->m_pFrameUp = NULL;
		}

		if (p_this->m_pFrameDownLeft)
		{
			delete p_this->m_pFrameDownLeft;
			p_this->m_pFrameDownLeft = NULL;
		}

		if (p_this->m_pFrameDownRight)
		{
			delete p_this->m_pFrameDownRight;
			p_this->m_pFrameDownRight = NULL;
		}
	}

	delete p_CUdp;
	delete p_CUdp_down;
	return 0;
}

int CGeoSender::TaskStart()
{
	SetThreadCount(1);
	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_send_geo", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_send_geo, p_st_thread_param, CpuMaskRemain());
	}
	return 0;
}
