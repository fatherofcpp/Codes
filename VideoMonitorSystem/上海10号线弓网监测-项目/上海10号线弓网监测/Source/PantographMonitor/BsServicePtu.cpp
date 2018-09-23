#include "StdAfx.h"
#include "BsServicePtu.h"

#include "CmFunction.h"
#include "Log/CmLog.h"
#include "Socket/CmTransTcp.h"
#include "BsDefStruct.h"
#include "BsFunction.h"
#include "BsMonitor.h"
#include "DataBase/CmDBMySql.h"
#include "BsGlobal.h"

#include "CamVideo/NVCTRDefines.h"

CServicePtu::CServicePtu(void)
{
	m_udpPort = 0;
}

CServicePtu::~CServicePtu(void)
{
	ReleaseRes();
}

int CServicePtu::TaskStart()
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

void CServicePtu::ReleaseRes()
{

}
