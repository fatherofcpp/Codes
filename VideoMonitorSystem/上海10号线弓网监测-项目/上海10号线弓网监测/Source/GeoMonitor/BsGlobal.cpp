#include "StdAfx.h"
#include "BsGlobal.h"

//////////////////////////////////////////////////

const std::string gTblStation = "t_station_info";
const std::string gStationKey = "station_id";
const std::string gStationFCode = "station_code";
const std::string gStationFAlias = "station_alias";
const std::string gStationFName = "station_name";
const std::string gStationFSeq = "station_seq";
const std::string gStationFMileage = "mileage";
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string gTblRecord = "t_record_info";
const std::string gRecordFKey = "RecordKey";
const std::string gRecordFCameraPos = "CameraPos";
const std::string gRecordFCarriageId = "CarriageId";
const std::string gRecordFCarriageNum = "CarriageNum";
const std::string gRecordFStationId = "StationId";
const std::string gRecordFLineId = "LineId";
const std::string gRecordFDirection = "Direction";
const std::string gRecordFFileName = "FileName";
const std::string gRecordFFullName = "FullName";
const std::string gRecordFBeginTime = "BeginTime";
const std::string gRecordFEndTime = "EndTime";//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string gTblAlarm = "t_alarm_info";
const std::string gAlarmFKey = "AlarmKey";
const std::string gAlarmFCameraPos = gRecordFCameraPos;
const std::string gAlarmFCarriageId = gRecordFCarriageId;
const std::string gAlarmFCarriageNum = gRecordFCarriageNum;
const std::string gAlarmFStationId = gRecordFStationId;
const std::string gAlarmFLineId = gRecordFLineId;
const std::string gAlarmFDirection = gRecordFDirection;
const std::string gAlarmFFileName = gRecordFFileName;
const std::string gAlarmFFullName = gRecordFFullName;
const std::string gAlarmFBeginTime = gRecordFBeginTime;
const std::string gAlarmFAlarmType = "AlarmType";//
const std::string gAlarmFAlarmLevel = "AlarmLevel";//
const std::string gAlarmFAlarmMessage = "AlarmMessage";//
const std::string gAlarmFVideoFileName = "VideoFileName";//
const std::string gAlarmFVideoFullName = "VideoFullName";//
const std::string gAlarmFAlarmValue = "AlarmValue";//
const std::string gAlarmFSendFlag = "SendFlag";//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string gTblRt = "t_rt_info";
const std::string gRtFKey = "RtFKey";
const std::string gRtFCarriageId = gRecordFCarriageId;
const std::string gRtFCarriageNum = gRecordFCarriageNum;
const std::string gRtFStationInd = "StationInd";
const std::string gRtFLineId = gRecordFLineId;
const std::string gRtFDirection = gRecordFDirection;
const std::string gRtFSendFlag = gAlarmFSendFlag;
const std::string gRtFDataType = "RtDataType";
const std::string gRtFData = "RtData";
const std::string gRtFBeginTime = gRecordFBeginTime;
const std::string gRtFBeginTimeMs = "BeginTimeMs";
const std::string gRtFLastTime = "LastTime";
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string gDbVideoPicTimeMaxDayAgo = "date_format(date_add(NOW(), interval - 3 day),'%Y-%m-%d 00:00:00')";
const std::string gDbRtDataTimeMaxDayAgo = "date_format(date_add(NOW(), interval - 3 day),'%Y-%m-%d 00:00:00')";

const std::string gDbTimeOneDayAgo = "date_format(date_add(NOW(), interval - 1 day),'%Y-%m-%d 00:00:00')";
const std::string gDbTimeTwoDayAgo = "date_format(date_add(NOW(), interval - 2 day),'%Y-%m-%d 00:00:00')";
const std::string gDbTimeNow = "date_format(now(),'%Y-%m-%d %H:%i:%s')";
const std::string gDbTimeTodayBegin = "date_format(now(),'%Y-%m-%d 00:00:00')";
const std::string gDbTimeTodayEnd = "date_format(now(),'%Y-%m-%d 23:59:59')";


const int gAlarmVideoSecOff = 10;

const std::string gCameraBasler = "CAM_BASLER_CAMERA";
const std::string gCameraThermoGroup = "CAM_THERMOGROUP_CAMERA";
const std::string gCameraStarPmtUv = "CAM_NETWORK_STAR_PMT_UV_CAMERA";
const std::string gCameraGX23D = "CAM_NETWORK_GX2_3D_CAMERA";
const std::string gCameraA80I = "CAM_NETWORK_A80I_CAMERA";

const std::string gVersion = "Version(2018-4-28 1730)";
