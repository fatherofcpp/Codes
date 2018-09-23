#pragma once
#include "Thread/CmThreadPool.h"
#include <deque>
namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}
struct tag_st_alarm;
struct tag_st_pull_guid;
class CMvb;
class CStructAnalyse;

class CGeoParamAnalyse: public NSP_STP_CM::CThreadPool
{
public:
	CGeoParamAnalyse(void);
	~CGeoParamAnalyse(void);
public:
	//初始化
	virtual int TaskStart();

	//清空缓存
	void ReleaseRes();
public:
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* dbmysql_param){m_pDbMysqlParam = dbmysql_param;};
	void SetMvb(CMvb* p_mvb){m_pCMvb = p_mvb;}
	void SetCStructAnalyse(CStructAnalyse* p_CStructAnalyse){m_pCStructAnalyse = p_CStructAnalyse;}
	void SetCarrigeNum(int carrige_num){m_carriageNum = carrige_num;}
	void SetLineId(int line_id){m_lineId = line_id;}
	void SetGeoPort(int port){m_udpGeoPort = port;}
	void SetGeoThreshold(double pull_thre_cls1, double pull_thre_cls2, double pull_thre_cls3,
						double guid_thre_cls1, double guid_thre_cls2, double guid_thre_cls3);
	void SetGeoTimeoutS(int tm_outs){m_geoTimeoutS = tm_outs;}
	void SetPictureRootPathAlarm(std::string alarm_path){m_pictureRootPathAlarm = alarm_path;}
	void SetFrameSuffix(std::string suffix){m_saveFrameSuffix = suffix;};
private:
	void AddPullGuid(tag_st_alarm* p_alarm);
	tag_st_alarm* PickPullGuid();
private:
	//分析线程
	static unsigned int WINAPI thread_analyse(void* pParam);
	//几何参数获取线程
	static unsigned int WINAPI thread_geoparam_recv(void* pParam);
	static unsigned int WINAPI thread_geoparam_handle(void* pParam);
private:
	std::deque<tag_st_alarm*> m_pullGuid;//报警的拉出
	CCriticalSection m_pullGuidLock;

	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
	//Mvb通信类
	CMvb* m_pCMvb;

	CStructAnalyse* m_pCStructAnalyse;

	// 车厢号
	int m_carriageNum;
	// 线路号
	int m_lineId;
	//监听端口
	int m_udpGeoPort;
	//待处理数据
	char m_handleBuf[256];
	std::string m_pictureRootPathAlarm;
	std::string m_saveFrameSuffix;

	double m_pullThresholdCls1;
	double m_pullThresholdCls2;
	double m_pullThresholdCls3;
	double m_guidThresholdCls1;
	double m_guidThresholdCls2;
	double m_guidThresholdCls3;

	int m_geoTimeoutS;
};

