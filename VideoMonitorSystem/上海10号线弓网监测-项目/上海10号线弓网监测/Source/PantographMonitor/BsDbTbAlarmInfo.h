#pragma once
#include "DataBase/CmDBMySql.h"

class CDbTbAlarmInfo : public NSP_STP_CM::CMySqlDB
{
public:
	CDbTbAlarmInfo(void);
	~CDbTbAlarmInfo(void);

private:
	const std::string m_tblAlarm;
	const std::string m_alarmFKey;
	const std::string m_alarmFCameraPos;
	const std::string m_alarmFCarriageId;
	const std::string m_alarmFCarriageNum;
	const std::string m_alarmFStationId;
	const std::string m_alarmFLineId;
	const std::string m_alarmFDirection;
	const std::string m_alarmFFileName;
	const std::string m_alarmFFullName;
	const std::string m_alarmFBeginTime;
	const std::string m_alarmFAlarmType;
	const std::string m_alarmFAlarmLevel;
	const std::string m_alarmFAlarmMessage;
	const std::string m_alarmFVideoFileName;
	const std::string m_alarmFVideoFullName;
	const std::string m_alarmFAlarmValue;
	const std::string m_alarmFSendFlag;
	const std::string m_alarmFMileage;
};

