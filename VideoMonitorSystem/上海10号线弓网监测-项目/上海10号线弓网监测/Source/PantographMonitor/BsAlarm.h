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
	//��ʼ��
	virtual int TaskStart();
	//�����ϱ���������Ϣ
	void SetReportService(tag_st_data_center_service* p_st_data_center_service);
	//��ջ���
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
	//�����ϱ��߳�
	static unsigned int WINAPI thread_alarm_report(void* pParam);
	//ʵʱ�����ϱ��߳�
	static unsigned int WINAPI thread_rtdata_report(void* pParam);
	//�豸���߼���߳�
	static unsigned int WINAPI thread_device_state(void* pParam);
	//������������߳�
	static unsigned int WINAPI thread_time_delete_db(void* pParam);
	//����ͼƬ���ö���¼����Ƶ
	static unsigned int WINAPI thread_alarm_frame_match_video(void* pParam);
	//������Ϣ�����̣߳����ݿ�תtxt
	static unsigned int WINAPI thread_export_info(void* pParam);
	//���̿��Ź�ι��
	static unsigned int WINAPI thread_watchdog(void* pParam);
private:
	//�ϱ���������Ϣ
	const tag_st_data_center_service* m_pDataCenterService;
	//�ļ��洢·��
	std::string m_pictureRootPath;
	std::string m_pictureRootPathAlarm;
	std::string m_videoRootPath;
	std::string m_videoRootPathAlarm;
	std::string m_exportInfRootPath;
	//��洢��׺
	std::string m_saveFrameSuffix;
	//��Ƶ�洢��׺
	std::string m_saveVideoSuffix;
	//��������Ԥ���ռ�
	int m_diskNeedSpace;
	//���ݿ���Ϣ
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
	CMainTask* m_pCMainTask;
	//Mvbͨ����
	CMvb* m_pCMvb;
	CMonitor* m_pCMonitor;
	int m_dataCenterTimeoutS;
	int m_cameraTimeoutS;
	int m_videoMaxSec;
};

