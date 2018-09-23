#pragma once
#include "DataBase/CmDBMySql.h"
class CDbTbStationInfo :
	public NSP_STP_CM::CMySqlDB
{
public:
	CDbTbStationInfo(void);
	~CDbTbStationInfo(void);

private:
	const std::string m_tblStation;
	const std::string m_stationKey;
	const std::string m_stationFCode;
	const std::string m_stationFAlias;
	const std::string m_stationFName;
	const std::string m_stationFSeq;
	const std::string m_stationFMileage;
};

