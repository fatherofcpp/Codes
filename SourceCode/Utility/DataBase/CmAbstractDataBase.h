#pragma once

namespace NSP_STP_CM
{
	class AbstractDataBase
	{
	public:
		AbstractDataBase();
		virtual ~AbstractDataBase();
	public:
		virtual int CreateDB() = 0;
		virtual int ConnectDataBase() = 0;
		virtual int DisConnectDataBase() = 0;
		virtual int ReadDataBase(unsigned long long& row_count, unsigned int& col_count, char**** pppp_res) = 0;
		virtual int WriteDataBase(const char* data_base_cmd) = 0;
	};
}
