#include "StdAfx.h"
#include "BsDbRoot.h"
#include "BsGlobal.h"
#include "Log/CmLog.h"
#include "CmDefStruct.h"

#include "BsDbTbAlarmInfo.h"
#include "BsDbTbRecordInfo.h"
#include "BsDbTbRtInfo.h"
#include "BsDbTbStationInfo.h"

CDbRoot::CDbRoot(void)
{
	m_pStDbmysqlParam = new NSP_STP_CM::tag_st_dbmysql_param;
}


CDbRoot::~CDbRoot(void)
{
	delete m_pStDbmysqlParam;
}

int CDbRoot::CreateDbTree(NSP_STP_CM::tag_st_dbmysql_param* p_tag_st_dbmysql_param)
{
	memcpy(m_pStDbmysqlParam, p_tag_st_dbmysql_param, sizeof(*m_pStDbmysqlParam));

	int ret = 0;
	//init db
	char sql_cmd[1024];
	SetMySqlServerInf(m_pStDbmysqlParam->dbHost.c_str(), m_pStDbmysqlParam->dbUser.c_str(), 
		m_pStDbmysqlParam->dbPass.c_str(), m_pStDbmysqlParam->dbName.c_str(), m_pStDbmysqlParam->dbPort);

	//create db
	while(1)
	{
		ret = CreateDB();
		if (ret == 0)
		{
			break;
		}
		else
		{
			Sleep(500);
		}
	}

	if (ret != 0)
	{
		gLog.Add("%s CreateDB,ret=%d\n", __FUNCTION__, ret);
		return ret;
	}

	ret = ConnectDataBase();
	if (ret != 0)
	{
		gLog.Add("%s ConnectDataBase,ret=%d\n", __FUNCTION__, ret);
		return ret;
	}

	//create table
	sprintf(sql_cmd, "create table if not exists %s"
		"("
		"%s INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(20) DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s double DEFAULT NULL,"
		"%s tinyint(1) DEFAULT '0',"
		"%s double DEFAULT NULL"
		")", gTblAlarm.c_str(),
		gAlarmFKey.c_str(), gAlarmFCameraPos.c_str(),
		gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), gAlarmFLineId.c_str(), 
		gAlarmFDirection.c_str(), gAlarmFFileName.c_str(), gAlarmFFullName.c_str(), gAlarmFBeginTime.c_str(), 
		gAlarmFAlarmType.c_str(), gAlarmFAlarmLevel.c_str(), gAlarmFAlarmMessage.c_str(), 
		gAlarmFVideoFileName.c_str(), gAlarmFVideoFullName.c_str(), gAlarmFAlarmValue.c_str(), gAlarmFSendFlag.c_str(),
		gAlarmFMileage.c_str()
		);
	ret = WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		gLog.Add("%s  WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		return ret;
	}

	//create table
	sprintf(sql_cmd, "create table if not exists %s"
		"("
		"%s INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(20) DEFAULT NULL,"
		"%s VARCHAR(20) DEFAULT NULL"
		")", gTblRecord.c_str()
		,gRecordFKey.c_str(), gRecordFCameraPos.c_str()
		,gRecordFCarriageId.c_str(), gRecordFCarriageNum.c_str(), gRecordFStationId.c_str(),gRecordFLineId.c_str(), 
		gRecordFDirection.c_str(), gRecordFFileName.c_str(), gRecordFFullName.c_str(), gRecordFBeginTime.c_str(),
		gRecordFEndTime.c_str()
		);
	ret = WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		return ret;
	}

	//create table
	sprintf(sql_cmd, "create table if not exists %s"
		"("
		"%s INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(20) DEFAULT NULL,"
		"%s VARCHAR(20) DEFAULT NULL"
		")", gTblRecordSpare.c_str()
		,gRecordFKey.c_str(), gRecordFCameraPos.c_str()
		,gRecordFCarriageId.c_str(), gRecordFCarriageNum.c_str(), gRecordFStationId.c_str(),gRecordFLineId.c_str(), 
		gRecordFDirection.c_str(), gRecordFFileName.c_str(), gRecordFFullName.c_str(), gRecordFBeginTime.c_str(),
		gRecordFEndTime.c_str()
		);
	ret = WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		return ret;
	}

	//create table
	sprintf(sql_cmd, "create table if not exists %s"
		"("
		"%s INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s INTEGER DEFAULT '0',"
		"%s INTEGER DEFAULT NULL,"
		"%s VARCHAR(64) DEFAULT NULL,"
		"%s VARCHAR(32) DEFAULT NULL,"
		"%s INTEGER DEFAULT NULL,"
		"%s VARCHAR(32) DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s VARCHAR(4096) DEFAULT NULL"
		")", gTblRt.c_str(),
		gRtFKey.c_str(),
		gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(), gRtFLineId.c_str(), 
		gRtFDirection.c_str(), gRtFSendFlag.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), 
		gRtFBeginTime.c_str(), gRtFBeginTimeMs.c_str(), gRtFLastTime.c_str(), gRtFFullName.c_str(),
		gRtFDataParam.c_str()
		);
	ret = WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		gLog.Add("%s  WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		return ret;
	}

	DisConnectDataBase();
	//end init db

	return ret;
}
