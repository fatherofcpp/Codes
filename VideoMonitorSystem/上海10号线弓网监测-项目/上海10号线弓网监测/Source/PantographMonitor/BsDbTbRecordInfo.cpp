#include "StdAfx.h"
#include "BsDbTbRecordInfo.h"
#include "BsGlobal.h"


CDbTbRecordInfo::CDbTbRecordInfo(void)
	: m_tblRecord(gTblRecord),
	m_recordFKey(gRecordFKey),
	m_recordFCameraPos(gRecordFCameraPos),
	m_recordFCarriageId(gRecordFCarriageId),
	m_recordFCarriageNum(gRecordFCarriageNum),
	m_recordFStationId(gRecordFStationId),
	m_recordFLineId(gRecordFLineId),
	m_recordFDirection(gRecordFDirection),
	m_recordFFileName(gRecordFFileName),
	m_recordFFullName(gRecordFFullName),
	m_recordFBeginTime(gRecordFBeginTime),
	m_recordFEndTime(gRecordFEndTime)
{

}


CDbTbRecordInfo::~CDbTbRecordInfo(void)
{
}
