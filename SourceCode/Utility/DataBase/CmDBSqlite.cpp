
#include "stdafx.h"
#include "CmDBSqlite.h"
#include "sqlite/sqlite3.h"
#include "Log/CmLog.h"

#pragma comment(lib, "sqlite3.lib")

static const int s_sql_val_max_len = 260;

NSP_STP_CM::CSqliteDB::CSqliteDB()
{
	m_pSqlite = NULL;
	m_filePath = "";
	SetIsConnected(false);
}

NSP_STP_CM::CSqliteDB::~CSqliteDB()
{
	DisConnectDataBase();
}

int NSP_STP_CM::CSqliteDB::CreateDB()
{
	int ret = 0;
	ret = ConnectDataBase();
	if(ret != 0)
	{
		return -1;
	}

	char sql[512];
	sprintf(sql, "create database if not exists %s DEFAULT CHARACTER SET 'utf8' COLLATE 'utf8_general_ci'", m_dbName.c_str());
	ret = WriteDataBase(sql);
	return ret;
}

int NSP_STP_CM::CSqliteDB::ConnectDataBase()
{
	int ret = 0;
	if (GetIsConnected())
	{
		return 0;
	}

	if (sqlite3_open(m_filePath.c_str(), &m_pSqlite) != SQLITE_OK)
	{
		m_pSqlite = NULL;
		gLog.Add("%s sqlite3_open err!\n", __FUNCTION__);
		return -1;
	}

	SetIsConnected(true);

	return 0;
}

int NSP_STP_CM::CSqliteDB::DisConnectDataBase()
{
	if (m_pSqlite && GetIsConnected())
	{
		sqlite3_close(m_pSqlite);
		m_pSqlite = NULL;
		SetIsConnected(false);
	}
	return 0;
}

int NSP_STP_CM::CSqliteDB::ReadDataBase(unsigned long long& row_count, unsigned int& col_count, char**** pppp_res)
{
	if (!GetIsConnected())
	{
		return -1;
	}

	if (!pppp_res)
	{
		return -2;
	}

	row_count = 0;
	col_count = 1;

	if (row_count == 0 || col_count == 0)
	{
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

	for(unsigned int row_ind = 0; row_ind < col_count; row_ind ++)
	{
		for(unsigned int col_ind = 0; col_ind < col_count; col_ind++)
		{
			char result_row[2][260] = {0};
			if (result_row[col_ind])
			{
				strcpy(ppp_res[row_ind][col_ind], result_row[col_ind]);
			}
			else
			{
				strcpy(ppp_res[row_ind][col_ind], "");
			}
		}
	}
	return 0;
}

int NSP_STP_CM::CSqliteDB::WriteDataBase(const char* data_base_cmd)
{
	ConnectDataBase();
	int ret = 0;
	if(m_isRead)
	{
		ret = sqlite3_exec(m_pSqlite, data_base_cmd, 
			print_result, NULL, NULL);
	}
	else
	{
		ret = sqlite3_exec(m_pSqlite, data_base_cmd, 
			NULL, NULL, NULL);
	}
	if(ret != SQLITE_OK)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

static void print_row(int n_values, char** values)
{
	for (int i = 0; i < n_values; ++i) 
	{
		printf("%10s", values[i]);
	}
}

int NSP_STP_CM::CSqliteDB::print_result(void* data, int n_columns, char** column_values, char** column_names)
{
	//每行触发一次?
	print_row(n_columns, column_names);

	print_row(n_columns, column_values);

	return 0;
}
