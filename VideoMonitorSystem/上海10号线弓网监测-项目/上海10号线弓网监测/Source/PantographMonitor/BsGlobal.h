#pragma once
#include <string>

//const 常量

extern const std::string gTblStation;
extern const std::string gStationKey;
extern const std::string gStationFCode;
extern const std::string gStationFAlias;
extern const std::string gStationFName;
extern const std::string gStationFSeq;
extern const std::string gStationFMileage;
//////////////////////////////////////////////////
extern const std::string gTblRecord;
extern const std::string gRecordFKey;
extern const std::string gRecordFCameraPos;//相机位置
extern const std::string gRecordFCarriageId;//车辆编号
extern const std::string gRecordFCarriageNum;//车厢编号
extern const std::string gRecordFStationId;//站点下标
extern const std::string gRecordFLineId;//线路号
extern const std::string gRecordFDirection;//方向
extern const std::string gRecordFFileName;//文件名
extern const std::string gRecordFFullName;//文件路径
extern const std::string gRecordFBeginTime;//开始时间
extern const std::string gRecordFEndTime;//结束时间
///////////////////////////////////////////////////
extern const std::string gTblRecordSpare;
///////////////////////////////////////////////////
extern const std::string gTblAlarm;
extern const std::string gAlarmFKey;
extern const std::string gAlarmFCameraPos;
extern const std::string gAlarmFCarriageId;
extern const std::string gAlarmFCarriageNum;
extern const std::string gAlarmFStationId;
extern const std::string gAlarmFLineId;
extern const std::string gAlarmFDirection;
extern const std::string gAlarmFFileName;
extern const std::string gAlarmFFullName;
extern const std::string gAlarmFBeginTime;
extern const std::string gAlarmFAlarmType;//
extern const std::string gAlarmFAlarmLevel;//
extern const std::string gAlarmFAlarmMessage;//
extern const std::string gAlarmFVideoFileName;//
extern const std::string gAlarmFVideoFullName;//
extern const std::string gAlarmFVideoBuildFlag;//
extern const std::string gAlarmFAlarmValue;//
extern const std::string gAlarmFSendFlag;//
extern const std::string gAlarmFMileage;//
extern const std::string gAlarmFAlarmValueParam;//
extern const std::string gAlarmFStuctureFullName;//
///////////////////////////////////////////////////////
extern const std::string gTblRt;
extern const std::string gRtFKey;
extern const std::string gRtFCarriageId;
extern const std::string gRtFCarriageNum;
extern const std::string gRtFStationInd;
extern const std::string gRtFStationName;
extern const std::string gRtFLineId;
extern const std::string gRtFDirection;
extern const std::string gRtFSendFlag;
extern const std::string gRtFDataType;
extern const std::string gRtFData;
extern const std::string gRtFBeginTime;
extern const std::string gRtFBeginTimeMs;
extern const std::string gRtFLastTime;
extern const std::string gRtFFullName;
extern const std::string gRtFDataParam;
///////////////////////////////////////////////////////
extern const unsigned int gDiskSaveDayNum;//磁盘历史数据天数
extern const unsigned int gDbSaveDayNum;//数据库历史数据天数
extern const std::string gDbVideoPicAlarmTimeMaxDayAgo;//故障图片与对应故障视频保存天数
extern const std::string gDbVideoNormalTimeMaxDayAgo;//(非故障)视频保存天数
extern const std::string gDbPicNormalTimeMaxDayAgo;//非故障图片保存天数
extern const std::string gDbRtDataTimeMaxDayAgo;//实时数据保存天数

extern const std::string gDbTimeOneDayAgo;
extern const std::string gDbTimeTwoDayAgo;
extern const std::string gDbTimeNow;
extern const std::string gDbTimeTodayBegin;
extern const std::string gDbTimeTodayEnd;

///////////////////////////////////////////////////////
extern const std::string gCameraBasler;
extern const std::string gCameraThermoGroup;
extern const std::string gCameraStarPmtUv;
extern const std::string gCameraGX23D;
///////////////////////////////////////////////////////
extern const int gAlarmVideoSecOff;//告警视频距报警时间的时间差
///////////////////////////////////////////////////////
extern const std::string gVersion;
///////////////////////////////////////////////////////

