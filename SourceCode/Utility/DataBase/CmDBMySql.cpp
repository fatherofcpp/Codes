#include "stdafx.h"

#include "CmDBMySql.h"
#include "mysql/mysql.h"
#include "Log/CmLog.h"

#pragma comment(lib, "libmysql.lib")

static const int s_sql_val_max_len = 260;


int NSP_STP_CM::CMySqlDB::ConnectDataBase()
{
	int ret = 0;
	if (GetIsConnected())
	{
		return 0;
	}

	m_pMySql = mysql_init(NULL); 
	if (!m_pMySql)
	{
		gLog.Add("%s mysql_init err!\n", __FUNCTION__);
		return -1;
	}

	if (mysql_thread_init() != 0)
	{
		gLog.Add("%s mysql_thread_init err!\n", __FUNCTION__);
		return -2;
	}

	if (!mysql_real_connect(m_pMySql, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_dbName.c_str(), m_port, NULL, 0))
	{
		gLog.Add("%s mysql_real_connect err!\n", __FUNCTION__);
		return -3;
	}
	ret = mysql_query(m_pMySql, "SET NAMES GBK");

	SetIsConnected(true);
	return 0;
}

int NSP_STP_CM::CMySqlDB::DisConnectDataBase()
{
	if (m_pMySql && GetIsConnected())
	{
		mysql_close(m_pMySql);
		m_pMySql = NULL;
		SetIsConnected(false);
	}
	return 0;
}

NSP_STP_CM::CMySqlDB::CMySqlDB()
{
	m_pMySql = NULL;
	m_rg = 0x06;
	m_cg = 0x05;
	SetIsConnected(false);
}

NSP_STP_CM::CMySqlDB::~CMySqlDB()
{
	DisConnectDataBase();
}

void NSP_STP_CM::CMySqlDB::SetMySqlServerInf( const char* host, const char* user, const char* pass, const char* db_name, int port )
{
	m_host = host;
	m_user = user;
	m_passwd = pass;
	m_dbName = db_name;
	m_port = port;
}

int NSP_STP_CM::CMySqlDB::ReadDataBase(unsigned long long& row_count, unsigned int& col_count, char**** pppp_res)
{
	if (!GetIsConnected())
	{
		return -1;
	}

	if (!pppp_res)
	{
		return -2;
	}

	MYSQL_RES* res_ptr = mysql_store_result(m_pMySql);
	if (NULL == res_ptr)
	{
		return -3;
	}

	row_count = mysql_num_rows(res_ptr);
	col_count = mysql_num_fields(res_ptr);

	if (row_count == 0 || col_count == 0)
	{
		mysql_free_result(res_ptr);
		return -4;
	}

	char*** ppp_res = new char**[(int)row_count];
	*pppp_res = ppp_res;
	for (unsigned long long i = 0; i < row_count; i ++)
	{
		ppp_res[i] = new char*[col_count];
		for (unsigned int j = 0; j < col_count; j ++)
		{
			ppp_res[i][j] = new char[s_sql_val_max_len];
			memset(ppp_res[i][j], 0, s_sql_val_max_len);
		}
	}

	MYSQL_ROW result_row = NULL;
	int row_ind = 0;
	while(NULL != (result_row = mysql_fetch_row(res_ptr)))
	{
		for(unsigned int col_ind = 0; col_ind < col_count; col_ind++)
		{
			if (result_row[col_ind])
			{
				strcpy(ppp_res[row_ind][col_ind], result_row[col_ind]);
			}
			else
			{
				strcpy(ppp_res[row_ind][col_ind], "");
			}
		}
		row_ind ++;
	}
	mysql_free_result(res_ptr);
	return 0;
}

int NSP_STP_CM::CMySqlDB::WriteDataBase( const char* data_base_cmd )
{
	ConnectDataBase();

	if(mysql_query(m_pMySql, data_base_cmd) != 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

void NSP_STP_CM::CMySqlDB::ReleaseReadData( unsigned long long row_count, unsigned int col_count, char**** pppp_res )
{
	if (!pppp_res)
	{
		return;
	}

	char*** ppp_res = *pppp_res;
	for (unsigned long long i = 0; i < row_count; i ++)
	{
		for (unsigned int j = 0; j < col_count; j ++)
		{
			if (ppp_res[i][j])
			{
				delete[] ppp_res[i][j];
				ppp_res[i][j] = NULL;
			}
		}
		if (ppp_res[i])
		{
			delete[] ppp_res[i];
			ppp_res[i] = NULL;
		}
	}
	delete[] ppp_res;
	*pppp_res = NULL;
}

int NSP_STP_CM::CMySqlDB::CreateDB()
{
	int ret = 0;
	m_pMySql = mysql_init(NULL); 
	if (!m_pMySql)
	{
		gLog.Add("%s mysql_init err!\n", __FUNCTION__);
		return -1;
	}
	if (!mysql_real_connect(m_pMySql, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), NULL, m_port, NULL, 0))
	{
		gLog.Add("%s mysql_real_connect err!\n", __FUNCTION__);
		return -3;
	}
	else
	{
		SetIsConnected(true);
	}

	char sql[512];
	sprintf(sql, "create database if not exists %s DEFAULT CHARACTER SET 'utf8' COLLATE 'utf8_general_ci'", m_dbName.c_str());
	ret = WriteDataBase(sql);
	DisConnectDataBase();
	return ret;
}

int NSP_STP_CM::CMySqlDB::WriteAndReadDb( const char* db_cmd, unsigned long long& row_count, unsigned int& col_count, char**** pppp_res )
{
	int ret = 0;
	ret = WriteDataBase(db_cmd);
	if (ret == 0)
	{
		ret = ReadDataBase(row_count, col_count, pppp_res);
		if (ret == 0)
		{
			return 0;
		}
		else
		{
			return -2;
		}
	}
	else
	{
		return -1;
	}
}



