#include "StdAfx.h"
#include "BsServiceSpare.h"

#include "CmFunction.h"
#include "Log/CmLog.h"
#include "Socket/CmTransTcp.h"
#include "BsDefStruct.h"
#include "BsFunction.h"
#include "BsMonitor.h"
#include "DataBase/CmDBMySql.h"
#include "BsGlobal.h"

const char c_requre_data[64] = "{spare_requre}";
const char c_reply_data[64] = "{spare_reply}";
const int c_requre_len = strlen(c_requre_data);
const int c_reply_len = strlen(c_reply_data);
const int c_com_delt_m = 1000;

CServiceSpare::CServiceSpare(void)
{
	m_pCMonitor = NULL;
	m_serverIp = "";
	m_serverPort = 0;
	m_pFuncThreadCommunicate = thread_spare_server_communicate;
}


CServiceSpare::~CServiceSpare(void)
{
	ReleaseRes();
}

int CServiceSpare::TaskStart()
{
	SetThreadCount(2);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_connect_accept", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_connect_accept, p_st_thread_param, CpuMaskRemain());
	}

	{
		int thread_ind = 1;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_spare_client_communicate", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_spare_client_communicate, p_st_thread_param, CpuMaskRemain());
	}

	
	return 0;
}

void CServiceSpare::ReleaseRes()
{

}

unsigned int WINAPI CServiceSpare::thread_spare_server_communicate( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CServiceSpare* p_this = (CServiceSpare*)p_st_thread_param->pParam;
	const int sock_fd = p_st_thread_param->pVoid;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;
	const int c_recv_count = c_com_delt_m / sleep_m;

	const int c_wait_time_m = 5 * 60 * 1000;
	const int c_wait_count = c_wait_time_m / c_com_delt_m;

	NSP_STP_CM::CTcp* p_CTcp = new NSP_STP_CM::CTcp;
	p_CTcp->SetSockFd(sock_fd);
	timeval r_time_out;
	r_time_out.tv_sec = 1;
	r_time_out.tv_usec = 500 * 1000;
	p_CTcp->SetReadTimeOut(r_time_out);

	int ret = 0;
	int wait_num = 0;
	int recv_num = 0;

	p_this->AddConnectCount();


	char rcv_buf[64];
	while(1)
	{
		Sleep(sleep_m);
		if (!p_this->m_isMainThreadRun)
		{
			break;
		}

		if (++recv_num >= c_recv_count)
		{
			recv_num = 0;
			//接收桢
			memset(rcv_buf, 0, sizeof(rcv_buf));
			int real_recv_len = p_CTcp->RecvFrmService(rcv_buf, c_requre_len);
			if(real_recv_len == c_requre_len && 0 == strcmp(rcv_buf, c_requre_data))  
			{
				//发送桢
				ret = p_CTcp->Send2Service(c_reply_data, c_reply_len);
				if (ret == c_reply_len)
				{
					wait_num = 0;
				}
				else
				{
					wait_num ++;
				}
			}
			else
			{
				wait_num ++;
			}
		}

		if (wait_num >= c_wait_count)
		{
			goto LEAVE;
		}

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}
	}

LEAVE:
	p_CTcp->CloseSockFd();
	delete p_CTcp;
	p_this->DelConnectCount();
	return 0;
}

unsigned int WINAPI CServiceSpare::thread_spare_client_communicate( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CServiceSpare* p_this = (CServiceSpare*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	const int sleep_m = 20;
	const int c_send_count = c_com_delt_m / sleep_m;

	const int c_time_out_m = 30 * 1000;
	const int c_time_out_count = c_time_out_m / c_com_delt_m;

	NSP_STP_CM::CTcp* p_CTcp = new NSP_STP_CM::CTcp;
	p_CTcp->SetServerIp(p_this->m_serverIp.c_str());
	p_CTcp->SetServerPort(p_this->m_serverPort);
	timeval r_time_out;
	r_time_out.tv_sec = 1;
	r_time_out.tv_usec = 500 * 1000;
	p_CTcp->SetReadTimeOut(r_time_out);

	int ret = 0;
	int send_num = 0;
	int time_out_num = 0;
	char rcv_buf[64];
	while(1)
	{
		Sleep(sleep_m);
		if (!p_this->m_isMainThreadRun)
		{
			break;
		}

		if (++ send_num >= c_send_count)
		{
			send_num = 0;
			p_CTcp->ConnectService();
			ret = p_CTcp->Send2Service(c_requre_data, c_requre_len);
			if (ret == c_requre_len)
			{
				memset(rcv_buf, 0, sizeof(rcv_buf));
				ret = p_CTcp->RecvFrmService(rcv_buf, c_reply_len);
				if (ret == c_reply_len && 0 == strcmp(rcv_buf, c_reply_data))
				{
					time_out_num = 0;
					//关闭备份
					p_this->m_pCMonitor->SetRunSpare(false);
				}
				else
				{
					time_out_num ++;
					p_CTcp->DisconnectService();
				}
			}
			else
			{
				time_out_num ++;
				p_CTcp->DisconnectService();
			}
		}

		if (time_out_num >= c_time_out_count)
		{
			time_out_num = 0;
			//打开备份
			p_this->m_pCMonitor->SetRunSpare(true);
		}

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}
	}

	p_CTcp->CloseSockFd();
	delete p_CTcp;
	return 0;
}

void CServiceSpare::SetServerInf( std::string ip, int port )
{
	m_serverIp = ip;
	m_serverPort = port;
}
