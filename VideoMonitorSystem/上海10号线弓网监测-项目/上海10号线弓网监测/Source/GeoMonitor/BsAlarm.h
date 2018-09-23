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
	//��ʼ��
	virtual int TaskStart();
	//��ջ���
	void ReleaseRes();
	void SetPictureRootPath(std::string root_path){m_pictureRootPath = root_path;}
	void SetFrameSuffix(std::string suffix){m_saveFrameSuffix = suffix;};
	void SetDiskNeedSpace(int need_space){m_diskNeedSpace = need_space;}
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* dbmysql_param){m_pDbMysqlParam = dbmysql_param;};
private:
	//������������߳�
	static unsigned int WINAPI thread_time_delete_db(void* pParam);
private:
	//�ļ��洢·��
	std::string m_pictureRootPath;
	//��洢��׺
	std::string m_saveFrameSuffix;
	//��������Ԥ���ռ�
	int m_diskNeedSpace;
	//���ݿ���Ϣ
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
};

