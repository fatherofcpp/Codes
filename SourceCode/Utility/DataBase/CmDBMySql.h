#pragma once

#include "CmAbstractDataBase.h"
#include <string>

struct st_mysql;
namespace NSP_STP_CM
{
	class CMySqlDB : public AbstractDataBase
	{
	public:
		CMySqlDB();
		~CMySqlDB();
	public:
		virtual int CreateDB();
		virtual int ConnectDataBase();
		virtual int DisConnectDataBase();
		virtual int ReadDataBase(unsigned long long& row_count, unsigned int& col_count, char**** pppp_res);
		virtual int WriteDataBase(const char* data_base_cmd);
		void SetMySqlServerInf(const char* host, const char* user, const char* pass, const char* db_name, int port);
		void ReleaseReadData(unsigned long long row_count, unsigned int col_count, char**** pppp_res);
		int WriteAndReadDb(const char* db_cmd, unsigned long long& row_count, unsigned int& col_count, char**** pppp_res);
	private:
		bool GetIsConnected(){return m_isConnected;};
		void SetIsConnected(bool is_connected){m_isConnected = is_connected;};
	private:
		bool m_isConnected;
		st_mysql* m_pMySql;//数据库连接句柄  
		std::string m_host;//= "192.168.1.76";
		std::string m_user;//"root";
		std::string m_passwd;//"root";
		std::string m_dbName;//"test";
		int m_port;//3306
		char m_rg;//行隔开
		char m_cg;//字段隔开
	};
}
