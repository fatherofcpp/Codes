#pragma once

#include "CmAbstractDataBase.h"

#include <string>

struct sqlite3;
namespace NSP_STP_CM
{
	class CSqliteDB : public AbstractDataBase
	{
	public: 
		CSqliteDB();    
		~CSqliteDB();
	public:
		virtual int CreateDB();
		virtual int ConnectDataBase();
		virtual int DisConnectDataBase();
		virtual int ReadDataBase(unsigned long long& row_count, unsigned int& col_count, char**** pppp_res);
		virtual int WriteDataBase(const char* data_base_cmd);

		void SetSqliteInf(const char* db_name){m_dbName = db_name;}
		void SetIsRead(bool is_read){m_isRead = is_read;}
		void ReleaseReadData(unsigned long long row_count, unsigned int col_count, char**** pppp_res);
		int WriteAndReadDb(const char* db_cmd, unsigned long long& row_count, unsigned int& col_count, char**** pppp_res);
	private:
		bool GetIsConnected(){return m_isConnected;};
		void SetIsConnected(bool is_connected){m_isConnected = is_connected;};
		static int print_result(void* data, int n_columns, char** column_values, char** column_names);
	private:
		bool m_isConnected;
		bool m_isRead;
		struct sqlite3* m_pSqlite;//数据库连接句柄  
		std::string m_filePath;
		std::string m_dbName;//"test";
	};
}
