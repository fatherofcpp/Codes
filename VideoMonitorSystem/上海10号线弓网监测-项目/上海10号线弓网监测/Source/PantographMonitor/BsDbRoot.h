#pragma once
#include "DataBase/CmDBMySql.h"

namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}

class CDbTbAlarmInfo;
class CDbTbRecordInfo;
class CDbTbRtInfo;
class CDbTbStationInfo;

class CDbRoot : public NSP_STP_CM::CMySqlDB
{
public:
	CDbRoot(void);
	~CDbRoot(void);
public:
	int CreateDbTree(NSP_STP_CM::tag_st_dbmysql_param* p_tag_st_dbmysql_param);
private:
	NSP_STP_CM::tag_st_dbmysql_param* m_pStDbmysqlParam;

	CDbTbStationInfo* m_pCDbTbStationInfo;
	CDbTbAlarmInfo* m_pCDbTbAlarmInfo;
	CDbTbRtInfo* m_pCDbTbRtInfo;
	CDbTbRecordInfo* m_pCDbTbRecordInfo;
	CDbTbRecordInfo* m_pCDbTbRecordInfoSpare;
};

