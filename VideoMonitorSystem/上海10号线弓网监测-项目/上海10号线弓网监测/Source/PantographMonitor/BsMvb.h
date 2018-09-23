#pragma once
#include "Thread/CmThreadPool.h"
#include <string>

class CMainTask;

namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}


namespace MVB
{
	class CServerTrans;
	struct tag_st_mvb_recv_data;
	struct tag_st_mvb_alarm_data;
	struct tag_st_station_info;
}

class CMvb :
	public NSP_STP_CM::CThreadPool
{
public:
	CMvb(void);
	~CMvb(void);
public:
	//初始化
	virtual int TaskStart();

	//设置串口信息
	void SetSerialInf(std::string serial_port_mvb, int baud_rate_mvb, std::string serial_port_scm, int baud_rate_scm);
	//设置主线程指针
	void SetMainTask(CMainTask* p_CMainTask){m_pCMainTask = p_CMainTask;};
	//设置MVB位置编号
	void SetMvbPos(int mp_code){m_mvbPos = mp_code;};
	//设置数据库信息
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* p_st_dbmysql_param){m_pDbMysqlParam = p_st_dbmysql_param;};

	//获取接收的MVB数据
	MVB::tag_st_mvb_recv_data* GetMvbRecvData();
	//获取MVB运行状态
	bool GetMvbState(){return m_mvbState;}
	//获取当前站点信息
	MVB::tag_st_station_info* GetCurrentStationInf();
	int GetCurrentStationInd();
	//获取站点信息
	MVB::tag_st_station_info* GetStationInfByInd(int station_ind);
	//设置相应的报警,仅更新true的报警，不是完全复制
	MVB::tag_st_mvb_alarm_data* GetMvbAlarmData(){return m_pMvbAlarmData;};
	CCriticalSection* GetMvbAlarmLock(){return &m_MvbAlarmDataLock;}
	//清空缓存
	void ReleaseRes();
	void SetTrainNum(int train_num){m_trainNum = train_num;}
	void SetVideoMaxSec(int video_max_sec){m_videoMaxSec = video_max_sec;}
private:
	//设置MVB状态
	void SetMvbState(bool state);
private:
	//mvb通信线程
	static unsigned int WINAPI thread_mvb(void* pParam);
	static unsigned int WINAPI thread_scm(void* pParam);
	static unsigned int WINAPI thread_clear_alarm(void* pParam);
private:
	CMainTask* m_pCMainTask;
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;//数据库

	MVB::CServerTrans* m_pCServerTrans;//服务接口
	std::string m_serialPortMvb;//"COM1"
	int m_baudRateMvb;//38400
	int m_mvbPos;//1=MP1,2=MP2

	MVB::tag_st_mvb_recv_data* m_pMvbRecvData;//接收的数据
	MVB::tag_st_mvb_alarm_data* m_pMvbAlarmData;//发送的报警
	CCriticalSection m_MvbAlarmDataLock;

	bool m_mvbState;//通信状态

	HANDLE m_comScmHand;
	std::string m_serialPortScm;//"COM2"
	int m_baudRateScm;//38400
	int m_trainNum;

	int m_videoMaxSec;
};

