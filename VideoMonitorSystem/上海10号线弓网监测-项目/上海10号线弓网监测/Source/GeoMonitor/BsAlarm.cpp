#include "StdAfx.h"
#include "BsAlarm.h"

#include "BsDefStruct.h"
#include "Socket/CmTransTcp.h"
#include "CmFunction.h"
#include "DataBase/CmDBMySql.h"
#include "BsGlobal.h"
#include "Log/CmLog.h"
#include "BsAbstractCamera.h"
#include "BsFunction.h"

static const int g_cam_num = E_CAM_CLASS_TATOL;

CAlarm::CAlarm(void)
{
	m_pictureRootPath = "";
	m_saveFrameSuffix = "";
	m_diskNeedSpace = 0;
	m_pDbMysqlParam = NULL;
}

CAlarm::~CAlarm(void)
{
	ReleaseRes();
}

int CAlarm::TaskStart()
{
	SetThreadCount(1);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s thread_time_delete_db", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], thread_time_delete_db, p_st_thread_param, CpuMaskRemain());
	}

	return 0;

}


void CAlarm::ReleaseRes()
{

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

	const int sleep_m = 50;

	const std::string root_path = p_this->m_pictureRootPath;
	const int min_space_g = p_this->m_diskNeedSpace;

	int ret = -1;

	while(1)
	{
		Sleep(sleep_m);

		bool is_thread_run = p_this->m_pCThreadArr[thread_ind].GetThreadStatus();
		if (!is_thread_run)
		{
			break;
		}

		const bool disk_full = NSP_STP_CM::DiskIsFull(root_path, min_space_g);
		const bool is_rm_old_folder = true;
		if (is_rm_old_folder)
		{
			delete_folder_before_days(p_this->m_pictureRootPath, 3, disk_full);
		}

	}

	return 0;
}


