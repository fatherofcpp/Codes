#pragma once
#include "DataBase/CmDBMySql.h"
class CDbTbRtInfo :
	public NSP_STP_CM::CMySqlDB
{
public:
	CDbTbRtInfo(void);
	~CDbTbRtInfo(void);

private:
	const std::string m_tblRt;
	const std::string m_rtFKey;
	const std::string m_rtFCarriageId;
	const std::string m_rtFCarriageNum;
	const std::string m_rtFStationInd;
	const std::string m_rtFStationName;
	const std::string m_rtFLineId;
	const std::string m_rtFDirection;
	const std::string m_rtFSendFlag;
	const std::string m_rtFDataType;
	const std::string m_rtFData;
	const std::string m_rtFBeginTime;
	const std::string m_rtFBeginTimeMs;
	const std::string m_rtFLastTime;
	const std::string m_rtFFullName;
	const std::string m_rtFDataParam;
};

