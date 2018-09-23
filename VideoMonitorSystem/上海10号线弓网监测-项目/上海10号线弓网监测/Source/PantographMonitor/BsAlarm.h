#pragma once
#include "Thread/CmThreadPool.h"

#include <string>
#include <map>
#include <deque>

namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}

struct tag_st_data_center_service;
class CMainTask;
class CMvb;
class CMonitor;

class CAlarm: public NSP_STP_CM::CThreadPool
{
public:
	CAlarm(void);
	~CAlarm(void);
public:
	//初始化
	virtual int TaskStart();
	//设置上报服务器信息
	void SetReportService(tag_st_data_center_service* p_st_data_center_service);
	//清空缓存
	void ReleaseRes();
	void SetPictureRootPath(std::string root_path){m_pictureRootPath = root_path;}
	void SetPictureRootPathAlarm(std::string alarm_path){m_pictureRootPathAlarm = alarm_path;}
	void SetVideoRootPath(std::string root_path){m_videoRootPath = root_path;}
	void SetVideoRootPathAlarm(std::string alarm_path){m_videoRootPathAlarm = alarm_path;}
	void SetExportInfRootPath(std::string export_path){m_exportInfRootPath = export_path;}
	void SetFrameSuffix(std::string suffix){m_saveFrameSuffix = suffix;};
	void SetVideoSuffix(std::string suffix){m_saveVideoSuffix = suffix;};
	void SetDiskNeedSpace(int need_space){m_diskNeedSpace = need_space;}
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* dbmysql_param){m_pDbMysqlParam = dbmysql_param;};
	void SetMainTask(CMainTask* p_CMainTask){m_pCMainTask = p_CMainTask;};
	void SetMvb(CMvb* p_Mvb){m_pCMvb = p_Mvb;}
	void SetMonitor(CMonitor* p_CMonitor){m_pCMonitor = p_CMonitor;}
	void SetDataCenterTimeoutS(int tm_outs){m_dataCenterTimeoutS = tm_outs;}
	void SetCameraTimeoutS(int tm_outs){m_cameraTimeoutS = tm_outs;}
	void SetVideoMaxSec(int video_max_sec){m_videoMaxSec = video_max_sec;}
private:
	std::string WarnType2Str(int warn_type);
private:
	//报警上报线程
	static unsigned int WINAPI thread_alarm_report(void* pParam);
	//实时数据上报线程
	static unsigned int WINAPI thread_rtdata_report(void* pParam);
	//设备在线检测线程
	static unsigned int WINAPI thread_device_state(void* pParam);
	//磁盘容量检测线程
	static unsigned int WINAPI thread_time_delete_db(void* pParam);
	//报警图片设置对象录像视频
	static unsigned int WINAPI thread_alarm_frame_match_video(void* pParam);
	//导出信息生成线程，数据库转txt
	static unsigned int WINAPI thread_export_info(void* pParam);
	//进程看门狗喂狗
	static unsigned int WINAPI thread_watchdog(void* pParam);
private:
	//上报服务器信息
	const tag_st_data_center_service* m_pDataCenterService;
	//文件存储路径
	std::string m_pictureRootPath;
	std::string m_pictureRootPathAlarm;
	std::string m_videoRootPath;
	std::string m_videoRootPathAlarm;
	std::string m_exportInfRootPath;
	//桢存储后缀
	std::string m_saveFrameSuffix;
	//视频存储后缀
	std::string m_saveVideoSuffix;
	//磁盘最少预留空间
	int m_diskNeedSpace;
	//数据库信息
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
	CMainTask* m_pCMainTask;
	//Mvb通信类
	CMvb* m_pCMvb;
	CMonitor* m_pCMonitor;
	int m_dataCenterTimeoutS;
	int m_cameraTimeoutS;
	int m_videoMaxSec;
};

