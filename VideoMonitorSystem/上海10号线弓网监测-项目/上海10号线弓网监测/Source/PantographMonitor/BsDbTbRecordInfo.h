#pragma once
#include "DataBase/CmDBMySql.h"

class CDbTbRecordInfo :
	public NSP_STP_CM::CMySqlDB
{
public:
	CDbTbRecordInfo(void);
	~CDbTbRecordInfo(void);

private:
	const std::string m_tblRecord;
	const std::string m_recordFKey;
	const std::string m_recordFCameraPos;
	const std::string m_recordFCarriageId;
	const std::string m_recordFCarriageNum;
	const std::string m_recordFStationId;
	const std::string m_recordFLineId;
	const std::string m_recordFDirection;
	const std::string m_recordFFileName;
	const std::string m_recordFFullName;
	const std::string m_recordFBeginTime;
	const std::string m_recordFEndTime;
};

