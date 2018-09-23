#include "StdAfx.h"
#include "BsDbTbRtInfo.h"
#include "BsGlobal.h"


CDbTbRtInfo::CDbTbRtInfo(void)
	: m_tblRt(gTblRt),
	m_rtFKey(gRtFKey),
	m_rtFCarriageId(gRtFCarriageId),
	m_rtFCarriageNum(gRtFCarriageNum),
	m_rtFStationInd(gRtFStationInd),
	m_rtFStationName(gRtFStationName),
	m_rtFLineId(gRtFLineId),
	m_rtFDirection(gRtFDirection),
	m_rtFSendFlag(gRtFSendFlag),
	m_rtFDataType(gRtFDataType),
	m_rtFData(gRtFData),
	m_rtFBeginTime(gRtFBeginTime),
	m_rtFBeginTimeMs(gRtFBeginTimeMs),
	m_rtFLastTime(gRtFLastTime),
	m_rtFFullName(gRtFFullName),
	m_rtFDataParam(gRtFDataParam)
{

}


CDbTbRtInfo::~CDbTbRtInfo(void)
{
}
