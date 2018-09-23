#pragma once
#include "Thread/CmThreadPool.h"

#include <string>
#include <map>
#include <deque>

namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}

class CAlarm: public NSP_STP_CM::CThreadPool
{
public:
	CAlarm(void);
	~CAlarm(void);
public:
	//初始化
	virtual int TaskStart();
	//清空缓存
	void ReleaseRes();
	void SetPictureRootPath(std::string root_path){m_pictureRootPath = root_path;}
	void SetFrameSuffix(std::string suffix){m_saveFrameSuffix = suffix;};
	void SetDiskNeedSpace(int need_space){m_diskNeedSpace = need_space;}
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* dbmysql_param){m_pDbMysqlParam = dbmysql_param;};
private:
	//磁盘容量检测线程
	static unsigned int WINAPI thread_time_delete_db(void* pParam);
private:
	//文件存储路径
	std::string m_pictureRootPath;
	//桢存储后缀
	std::string m_saveFrameSuffix;
	//磁盘最少预留空间
	int m_diskNeedSpace;
	//数据库信息
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
};

