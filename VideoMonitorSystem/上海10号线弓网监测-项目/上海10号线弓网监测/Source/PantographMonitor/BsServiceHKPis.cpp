#include "StdAfx.h"
#include "BsServiceHKPis.h"

#include "CmFunction.h"
#include "Log/CmLog.h"
#include "Socket/CmTransTcp.h"
#include "BsDefStruct.h"
#include "BsFunction.h"
#include "BsMonitor.h"
#include "DataBase/CmDBMySql.h"
#include "BsGlobal.h"

#include <evhttp.h>
#pragma comment(lib,"libevent.lib")

CServiceHKPis::CServiceHKPis(void)
{
	m_pCMonitor = NULL;
}


CServiceHKPis::~CServiceHKPis(void)
{
	ReleaseRes();
}

int CServiceHKPis::TaskStart()
{
	SetThreadCount(1);

	{
		int thread_ind = 0;
		NSP_STP_CM::st_thread_param* p_st_thread_param = new NSP_STP_CM::st_thread_param;
		memset(p_st_thread_param, 0, sizeof(*p_st_thread_param));
		p_st_thread_param->pParam = this;
		p_st_thread_param->threadInd = thread_ind;
		sprintf(p_st_thread_param->threadName, "%s http_svr_tsk", __FUNCTION__);
		thread_run(&m_pCThreadArr[thread_ind], http_svr_tsk, p_st_thread_param, CpuMaskRemain());
	}
	return 0;
}

void CServiceHKPis::ReleaseRes()
{
}

unsigned int WINAPI CServiceHKPis::http_svr_tsk( void* pParam )
{
	NSP_STP_CM::st_thread_param* p_st_thread_param = (NSP_STP_CM::st_thread_param*)pParam;
	const int thread_ind = p_st_thread_param->threadInd;
	CServiceHKPis* p_this = (CServiceHKPis*)p_st_thread_param->pParam;
	delete p_st_thread_param;

	if (p_this == NULL)
	{
		return 0;
	}

	unsigned short http_local_port = p_this->m_httpLocalPort;
	char          *http_addr = "0.0.0.0";
	struct evhttp *http_server = NULL;
	struct evhttp *order_server = NULL;

	WORD wVersionRequested;
	WSADATA wsaData;
	int	err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if(err != 0)
	{
		gLog.Add("%s WSAStartup\n", __FUNCTION__);
		exit(0);
	}

	event_init();
	http_server  = evhttp_start(http_addr, http_local_port);

	gLog.Add("[%s] http svr start, listen port %d......\n", __FUNCTION__, http_local_port);

	evhttp_set_gencb(http_server, http_cb, p_this);
	evhttp_set_timeout(http_server, 10);

	event_dispatch();
	//event_free(http_server);

	return 0;
}

void CServiceHKPis::http_cb( struct evhttp_request *req, void *arg )
{
	CServiceHKPis* p_this = (CServiceHKPis*)arg;
	char* pic_data = NULL;

	struct evbuffer *returnbuffer = evbuffer_new();

	if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) 
	{
		goto LEAVE;
	}

	const char *host=evhttp_request_get_host(req);
	if(!host)
	{
		goto LEAVE;
	}

	const char *uri = evhttp_request_get_uri(req);
	bool req_jpg = false;
	if(strstr(uri, ".jpg"))
	{
		req_jpg = true;
	}else
	{
		req_jpg = false;
	}

	if (req_jpg)
	{
		//路径
		///E:/project/1.jpg
		char file_path[260];
		strcpy(file_path, uri+1);
		int ret = p_this->http_req_jpg(file_path, pic_data, returnbuffer, req);
		if (ret != 0)
		{
			goto LEAVE;
		}
	}
	else
	{
		int ret = p_this->http_req_alarm( uri, returnbuffer, req );
		if (ret != 0)
		{
			goto LEAVE;
		}
	}
	evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);

LEAVE:
	if (pic_data)
	{
		delete[] pic_data;
	}
	if (returnbuffer)
	{
		evbuffer_free(returnbuffer);
	}
	return;
}

void CServiceHKPis::SetHttpLocalPort( int http_port )
{
	m_httpLocalPort = http_port;
}

void CServiceHKPis::SetHttpOutPort( int http_port )
{
	m_httpOutPort = http_port;
}

void CServiceHKPis::SetHttpOutIp( std::string http_ip )
{
	m_httpOutIp = http_ip;
}

int CServiceHKPis::http_req_jpg( const char* path, char*& pic_data, struct evbuffer *returnbuffer, struct evhttp_request *req)
{
	int ret = 0;
	FILE* pf = fopen(path, "rb");
	if(pf != NULL)
	{
		fseek(pf,0,SEEK_END);
		int pic_len = 0;
		pic_len = ftell(pf);	
		fseek(pf, 0, SEEK_SET);

		//fread
		pic_data = new char[pic_len];
		memset(pic_data, 0, pic_len);
		int ret = fread(pic_data, 1, pic_len, pf);
		if(ret != pic_len)
		{
			ret = -1;
		}
		else
		{
			ret = 0;
			evbuffer_add(returnbuffer, pic_data, pic_len);
			evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "image/jpeg");
		}

		fclose(pf);
	}
	return ret;
}

int CServiceHKPis::http_req_alarm( const char* uri, struct evbuffer *returnbuffer, struct evhttp_request *req )
{
	char* sql_cmd = new char[1024*2];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

	//开始时间&结束时间&第几页数据&每页故障个数
	//2017-10-22 11:12:13&2017-10-23 11:12:13&1&8
	int begin_yy = 0;
	int begin_mo = 0;
	int begin_dd = 0;
	int begin_hh = 0;
	int begin_mi = 0;
	int begin_ss = 0;
	int end_yy = 0;
	int end_mo = 0;
	int end_dd = 0;
	int end_hh = 0;
	int end_mi = 0;
	int end_ss = 0;

	int page_ind = 1;
	int alarm_num_per_page = 1;
	int ret = sscanf(uri, "/%d-%d-%d&%d:%d:%d&%d-%d-%d&%d:%d:%d&%d&%d", 
		&begin_yy, &begin_mo, &begin_dd,
		&begin_hh, &begin_mi, &begin_ss,
		&end_yy, &end_mo, &end_dd,
		&end_hh, &end_mi, &end_ss,
		&page_ind, &alarm_num_per_page);
	if (ret == 14)
	{
		bool is_param_valid = true;

		//check date
		char begin_time[64];
		char end_time[64];
		sprintf(begin_time, "%04d-%02d-%02d %02d:%02d:%02d", begin_yy, begin_mo, begin_dd, begin_hh, begin_mi, begin_ss);
		sprintf(end_time, "%04d-%02d-%02d %02d:%02d:%02d", end_yy, end_mo, end_dd, end_hh, end_mi, end_ss);

		if (alarm_num_per_page <= 0 || page_ind <= 0)
		{
			ret = -1;
			goto LEAVE;
		}

		//find db
		int alarm_count_total = 0;
		{
			const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = m_pDbMysqlParam;
			p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
				p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

			sprintf(sql_cmd, "select %s, %s, %s, %s, %s from %s where  %s >= '%s' and %s < '%s' and %s is not NULL LIMIT 1024;", 
				gAlarmFBeginTime.c_str(), gAlarmFAlarmMessage.c_str(), gAlarmFStationId.c_str(), gAlarmFFullName.c_str(),
				gAlarmFMileage.c_str(),
				gTblAlarm.c_str(), 
				gAlarmFBeginTime.c_str(), begin_time,
				gAlarmFBeginTime.c_str(), end_time,
				gAlarmFFullName.c_str()
				);
			unsigned long long row_count = 0;
			unsigned int col_count = 0;
			char*** ppp_res = NULL;
			ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
			if (ret == 0)
			{
				alarm_count_total = (int)row_count;
				const int alarm_ind = (page_ind - 1) * alarm_num_per_page;
				if (alarm_ind <= alarm_count_total)
				{
					evbuffer_add_printf(returnbuffer, "<ArrayOfFaultInfo>\r\n");
					int show_num = 0;
					for (unsigned long long i = alarm_ind; i < row_count; i ++)
					{
						if (show_num++ >= alarm_num_per_page)
						{
							break;
						}
						std::string alarm_time;
						std::string alarm_name;
						int alarm_station;
						std::string alarm_file_path;
						double alarm_milage;

						alarm_time =  ppp_res[0][0];
						alarm_name =  ppp_res[0][1];
						NSP_STP_CM::str_to_int(ppp_res[0][2], alarm_station);
						alarm_file_path =  ppp_res[0][3];
						alarm_milage = atof(ppp_res[0][4]);

						const int http_out_port = m_httpOutPort;
						const std::string http_out_ip = m_httpOutIp;
						evbuffer_add_printf(returnbuffer, "	<FaultInfo>\r\n");
						evbuffer_add_printf(returnbuffer, "		<Time>%s</Time>\r\n", alarm_time.c_str());
						evbuffer_add_printf(returnbuffer, "		<Name>%s</Name>\r\n", alarm_name.c_str());
						evbuffer_add_printf(returnbuffer, "		<Station>%d</Station>\r\n", alarm_station);
						evbuffer_add_printf(returnbuffer, "		<Url>http://%s:%d/%s</Url>\r\n", http_out_ip.c_str(), http_out_port, alarm_file_path.c_str());
						evbuffer_add_printf(returnbuffer, "		<KilometerPost>%.03f</KilometerPost>\r\n", alarm_milage);
						evbuffer_add_printf(returnbuffer, "	</FaultInfo>\r\n");
					}
					evbuffer_add_printf(returnbuffer, "</ArrayOfFaultInfo>\r\n");
				}
				else
				{
					ret = -1;
				}
				p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
			}
		}

		if (ret != 0)
		{
			goto LEAVE;
		}

		evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/xml");
		ret = 0;
	}
	else
	{
		ret = -1;
		goto LEAVE;
	}

LEAVE:
	delete[] sql_cmd;
	delete p_CMySqlDB;
	return ret;
}

