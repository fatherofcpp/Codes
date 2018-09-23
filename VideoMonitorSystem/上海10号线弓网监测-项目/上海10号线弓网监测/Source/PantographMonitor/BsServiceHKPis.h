#pragma once
#include "Thread/CmThreadPool.h"

#include <string>

namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}

class CMonitor;

class CServiceHKPis :
	public NSP_STP_CM::CThreadPool
{
public:
	CServiceHKPis(void);
	~CServiceHKPis(void);
public:
	//初始化
	virtual int TaskStart();

	//清空缓存
	virtual void ReleaseRes();

	//设置本地端口
	void SetHttpLocalPort(int http_port);
	void SetHttpOutPort(int http_port);
	void SetHttpOutIp(std::string http_ip);
	void SetMonitor(CMonitor* p_CMonitor){m_pCMonitor = p_CMonitor;};
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* dbmysql_param){m_pDbMysqlParam = dbmysql_param;};

private:
	static unsigned int WINAPI http_svr_tsk(void* pParam);
	static void http_cb(struct evhttp_request *req, void *arg);
private:
	int http_req_jpg(const char* path, char*& p_buf, struct evbuffer *returnbuffer, struct evhttp_request *req);
	int http_req_alarm(const char* uri, struct evbuffer *returnbuffer, struct evhttp_request *req);
private:
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
	CMonitor* m_pCMonitor;
	int m_httpLocalPort;
	int m_httpOutPort;
	std::string m_httpOutIp;
};

