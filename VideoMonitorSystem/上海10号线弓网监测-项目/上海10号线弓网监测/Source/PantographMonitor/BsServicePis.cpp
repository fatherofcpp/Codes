#include "StdAfx.h"
#include "BsServicePis.h"
#include "PantographMonitor.h"

#include "CmFunction.h"
#include "Log/CmLog.h"
#include "Socket/CmTransTcp.h"
#include "BsDefStruct.h"
#include "BsFunction.h"
#include "BsMonitor.h"
#include "DataBase/CmDBMySql.h"
#include "BsGlobal.h"

#include "CamVideo/NVCTRDefines.h"

CServicePis::CServicePis(void)
{
	m_pCMonitor = NULL;
	m_pantoPos = 0;
	m_pDbMysqlParam = NULL;

	m_pFuncThreadCommunicate = thread_pis_communicate;
}


CServicePis::~CServicePis(void)
{
	ReleaseRes();
}

int CServicePis::TaskStart()
{
	SetThreadCount(1);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_connect_accept", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_connect_accept, p_st_thread_param, CpuMaskRemain());
	}
	return 0;
}

void CServicePis::ReleaseRes()
{
}

unsigned int WINAPI CServicePis::thread_pis_communicate( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CServicePis* p_this = (CServicePis*)p_st_thread_param->pParam;
	const int sock_fd = p_st_thread_param->pVoid;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;
	const int heartbeat_delay_m = 5 * 1000;
	const int rcv_buf_len = 1024 * 10;
	const int send_buf_len = 1024 * 10;

	char* rcv_buf = new char[rcv_buf_len];
	char* send_buf = new char[send_buf_len];
	NSP_STP_CM::CTcp* p_CTcp = new NSP_STP_CM::CTcp;

	p_CTcp->SetSockFd(sock_fd);
	timeval r_time_out;
	r_time_out.tv_sec = 1;
	r_time_out.tv_usec = 100 * 1000;
	p_CTcp->SetReadTimeOut(r_time_out);

	std::vector<st_pis_pack_inf> vec_recv_pack;
	std::vector<st_pis_pack_inf> vec_frame_pack;
	std::vector<st_pis_pack_inf> vec_send_pack;
	vec_recv_pack.clear();
	vec_frame_pack.clear();
	vec_send_pack.clear();

	int ret = 0;
	int pre_ms = ::GetTickCount();

	bool is_wait_heart_beat = false;

	p_this->AddConnectCount();

	while(1)
	{
		Sleep(sleep_m);
		if (!p_this->m_isMainThreadRun)
		{
			break;
		}

		//发送桢
		for (unsigned int i = 0; i < vec_send_pack.size(); i ++)
		{
			st_pis_pack_inf pack_inf = vec_send_pack[i];
			ret = p_CTcp->Send2Service(pack_inf.pData, pack_inf.iDataLength);
			if (ret != pack_inf.iDataLength)
			{
				goto LEAVE;
			}
			pre_ms = ::GetTickCount();
			delete[] pack_inf.pData;

			//gLog.Add("%s Send2Service ind=%d, sendLen=%d\n", __FUNCTION__, i, ret);
		}
		vec_send_pack.clear();

		//接收桢
		memset(rcv_buf, 0, rcv_buf_len);
		int real_recv_len = p_CTcp->RecvFrmService(rcv_buf, rcv_buf_len);
		if(real_recv_len > 0)  
		{
			pre_ms = ::GetTickCount();

			//缓存接收数据
			st_pis_pack_inf pack_inf;
			pack_inf.iDataLength = real_recv_len;
			pack_inf.pData = new char[pack_inf.iDataLength];
			memcpy(pack_inf.pData, rcv_buf, pack_inf.iDataLength);
			vec_recv_pack.push_back(pack_inf);

			//gLog.Add("%s RecvFrmService recvLen=%d\n", __FUNCTION__, real_recv_len);
		}
		
		if (1)
		{
			//缓存数据提取完整桢
			HandlePisPacket(&vec_recv_pack, &vec_frame_pack);
			vec_recv_pack.clear();

			const unsigned int frame_size = vec_frame_pack.size();
			if (frame_size > 0)
			{
				for (unsigned int i = 0; i < vec_frame_pack.size(); i ++)
				{
					st_pis_pack_inf pis_pack_inf = vec_frame_pack[i];

					p_this->m_pCMonitor->HandlePisCmd(&pis_pack_inf, is_wait_heart_beat, &vec_send_pack);

					delete[] pis_pack_inf.pData;
				}
				vec_frame_pack.clear();
			}
			else
			{
				int now_ms = ::GetTickCount();
				if (now_ms - pre_ms > heartbeat_delay_m)
				{
					pre_ms = ::GetTickCount();
					if (is_wait_heart_beat)
					{
						goto LEAVE;
					}
					//发送心跳
					TCP_HEADER pis_head;
					pis_head.HEARD = 0xFF;
					pis_head.FrameType = TX_MSG_HEARTBEAT;
					const int body_len = 0;
					NSP_STP_CM::Dec2Hex(pis_head.DataLenth, 2, body_len);
					char* data_body = NULL;
					memset(send_buf, 0, send_buf_len);
					const int frame_len = FormatPisFrame(send_buf, pis_head, data_body);
					if(frame_len > 0)
					{
						st_pis_pack_inf pack_inf;
						pack_inf.iDataLength = frame_len;
						pack_inf.pData = new char[pack_inf.iDataLength];
						memcpy(pack_inf.pData, send_buf, pack_inf.iDataLength);
						vec_send_pack.push_back(pack_inf);
					}
					is_wait_heart_beat = true;
				}
			}
		}

		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}
	}

LEAVE:
	for (unsigned int i = 0; i < vec_send_pack.size(); i ++)
	{
		if(vec_send_pack[i].pData)
		{
			delete[] vec_send_pack[i].pData;
		}
	}
	vec_send_pack.clear();

	for (unsigned int i = 0; i < vec_recv_pack.size(); i ++)
	{
		if(vec_recv_pack[i].pData)
		{
			delete[] vec_recv_pack[i].pData;
		}
	}
	vec_recv_pack.clear();

	for (unsigned int i = 0; i < vec_frame_pack.size(); i ++)
	{
		if(vec_frame_pack[i].pData)
		{
			delete[] vec_frame_pack[i].pData;
		}
	}
	vec_frame_pack.clear();

	p_CTcp->CloseSockFd();
	delete p_CTcp;
	delete[] rcv_buf;
	delete[] send_buf;
	p_this->DelConnectCount();

	//gLog.Add("%s leave\n", __FUNCTION__);

	return 0;
}


