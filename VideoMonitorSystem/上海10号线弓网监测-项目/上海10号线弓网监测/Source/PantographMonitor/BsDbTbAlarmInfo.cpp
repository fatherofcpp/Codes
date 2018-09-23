#include "StdAfx.h"
#include "BsDbTbAlarmInfo.h"
#include "BsGlobal.h"


CDbTbAlarmInfo::CDbTbAlarmInfo(void)
	: m_tblAlarm(gTblAlarm),
	m_alarmFKey(gAlarmFKey),
	m_alarmFCameraPos(gAlarmFCameraPos),
	m_alarmFCarriageId(gAlarmFCarriageId),
	m_alarmFCarriageNum(gAlarmFCarriageNum),
	m_alarmFStationId(gAlarmFStationId),
	m_alarmFLineId(gAlarmFLineId),
	m_alarmFDirection(gAlarmFDirection),
	m_alarmFFileName(gAlarmFFileName),
	m_alarmFFullName(gAlarmFFullName),
	m_alarmFBeginTime(gAlarmFBeginTime),
	m_alarmFAlarmType(gAlarmFAlarmType),
	m_alarmFAlarmLevel(gAlarmFAlarmLevel),
	m_alarmFAlarmMessage(gAlarmFAlarmMessage),
	m_alarmFVideoFileName(gAlarmFVideoFileName),
	m_alarmFVideoFullName(gAlarmFVideoFullName),
	m_alarmFAlarmValue(gAlarmFAlarmValue),
	m_alarmFSendFlag(gAlarmFSendFlag),
	m_alarmFMileage(gAlarmFMileage)
{

}


CDbTbAlarmInfo::~CDbTbAlarmInfo(void)
{

}
