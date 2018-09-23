#include "StdAfx.h"
#include "BsDbTbStationInfo.h"
#include "BsGlobal.h"


CDbTbStationInfo::CDbTbStationInfo(void)
	: m_tblStation(gTblStation),
	m_stationKey(gStationKey),
	m_stationFCode(gStationFCode),
	m_stationFAlias(gStationFAlias),
	m_stationFName(gStationFName),
	m_stationFSeq(gStationFSeq),
	m_stationFMileage(gStationFMileage)
{

}


CDbTbStationInfo::~CDbTbStationInfo(void)
{
}
