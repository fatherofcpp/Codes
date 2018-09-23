#pragma once

#include "CmDefStruct.h"
#include <string>

typedef enum tag_e_cam_class
{
	E_CAM_CLASS_ARC_GEOUP=0,
	E_CAM_CLASS_ARC_GEODOWNLEFT,
	E_CAM_CLASS_ARC_GEODOWNRIGHT,
	E_CAM_CLASS_TATOL,
}e_cam_class;

typedef enum tag_e_cam_pos
{
	E_CAM_POS_STRUCT = 1,
	E_CAM_POS_TEMPRATE = 2,
	E_CAM_POS_ARC_NOTIFY = 3,
	E_CAM_POS_ARC_COLLECT = 4,
	E_CAM_POS_MONITOR = 8,
}e_cam_pos;//位置安排：车厢摄像机位置号预留 01~07 排序，受电弓摄像机位置号 08~09

typedef struct tag_st_mvb_com_param
{
	tag_st_mvb_com_param()
	{
		mvbSerialPort = "";
		mvbBuadRate = 0;
		mpCode = 0;
	}

	std::string mvbSerialPort;//MVB串口号
	int mvbBuadRate;//MVB波特率
	int mpCode;//1=MP1,2=MP2
}st_mvb_com_param;

typedef struct tag_st_scm_param
{
	tag_st_scm_param()
	{
		scmSerialPort = "";
		scmBuadRate = 0;
	}

	std::string scmSerialPort;
	int scmBuadRate;
}st_scm_param;

typedef struct tag_st_data_center_service
{
	NSP_STP_CM::st_report_service_inf alarm_service;
	NSP_STP_CM::st_report_service_inf rtdata_service;
}st_data_center_service;

typedef struct tag_st_local_conf
{
	tag_st_local_conf()
	{
		tcpPisPort = 0;
		tcpPtuPort = 0;
		udpPtuPort = 0;
		udpGeoRecvPort = 0;
		udpGeoSendPort = 0;
		udpGeoSendPortDown = 0;
		udpCurrentPort = 0;
		videoRootPath = "";
		videoRootPathAlarm = "";
		pictureRootPath = "";
		pictureRootPathAlarm = "";
		diskNeedSpace = 0;
		fps = 0;
		arcBeginThreshold = 0;
		arcEndThreshold = 0;
		saveFrameSuffix = "";
		saveVideoSuffix = "";
		carriageNum = 0;
		lineId = 0;
		presetNum = 0;
		PantoPos = 0;
		ratedCurrentThreshold = 0;
		arcRateThresholdCls1 = 0;
		arcRateThresholdCls2 = 0;
		arcRateThresholdCls3 = 0;
		tempThresholdCls1 = 0;
		tempThresholdCls2 = 0;
		tempThresholdCls3 = 0;
		waitSystemTs = 0;
		jgqPreReadyTms = 0;
	}

	st_mvb_com_param mvbComParam;//MVB参数
	st_scm_param scmComParam;//单片机参数

	st_data_center_service SrvdataCenter;//地面服务器
	int tcpPisPort;//pis监听端口
	int tcpPtuPort;//ptu监听端口
	int udpPtuPort;//ptu udp端口
	int udpGeoRecvPort;//几何参数监听端口
	int udpGeoSendPort;//几何参数发送端口
	int udpGeoSendPortDown;//几何参数车底发送端口
	int udpCurrentPort;//电流检测监听端口

	int carriageNum;// 车厢号
	int lineId;// 线路号(配置文件中读取)
	int presetNum;//预置点个数
	int PantoPos;// 受电弓位置号

	std::string videoRootPath;//视频保存根目录
	std::string videoRootPathAlarm;//报警视频保存根目录
	std::string pictureRootPath;//图片保存根目录
	std::string pictureRootPathAlarm;//报警图片保存根目录
	int diskNeedSpace;//磁盘预留空间
	float fps;//实际帧率
	std::string saveFrameSuffix;//图片后缀
	std::string saveVideoSuffix;//视频后缀
	NSP_STP_CM::st_dbmysql_param dbMySqlParam;//数据库参数

	int ratedCurrentThreshold;//额定电流,单位0.3
	int arcBeginThreshold;//燃弧开始阀值0-255;default:200
	int arcEndThreshold;//燃弧开始阀值0-255;default:100
	double arcRateThresholdCls1;//燃弧率阀值,单位0.01
	double arcRateThresholdCls2;//
	double arcRateThresholdCls3;//
	double tempThresholdCls1;//温度阀值,单位摄氏度
	double tempThresholdCls2;//
	double tempThresholdCls3;//
	int waitSystemTs;
	int jgqPreReadyTms;
}st_local_conf;

typedef enum tag_e_frame_warn_type
{
	//报警故障

	//无报警
	E_FRAME_WARN_TYPE_NORMAL = 0,
	//"弓网温度
	E_FRAME_WARN_TYPE_PANTEM,
	//"燃弧率
	E_FRAME_WARN_TYPE_ARCRATE,
	//"跨距坡度
	E_FRAME_WARN_TYPE_SLOPE,
	//"导高值超限
	E_FRAME_WARN_TYPE_GUIDE_HEIGHT,
	//"拉出值超限
	E_FRAME_WARN_TYPE_PULLOUT,
	//"异物侵入
	E_FRAME_WARN_TYPE_FOREIGN_MATTER,
	//"受电弓结构异常
	E_FRAME_WARN_TYPE_STRUCTURE_ABNORMAL,
	//"接触线高度差
	E_FRAME_WARN_TYPE_CONTACT_LINE_HEIGHT_DIFFERENCE,
	//"滑板倾斜
	E_FRAME_WARN_TYPE_SLIDE_TILT,
	//"滑板丢失
	E_FRAME_WARN_TYPE_SKATEBOARD_MISSING,
	//"羊角异常
	E_FRAME_WARN_TYPE_HORN_ANOMALY,
}e_frame_warn_type;

typedef enum tag_e_frame_warn_cls
{
	//报警等级

	E_FRAME_WARN_CLS_0 = 0,//无
	E_FRAME_WARN_CLS_1,//1级
	E_FRAME_WARN_CLS_2,//2级
	E_FRAME_WARN_CLS_3//3级
}e_frame_warn_cls;

typedef enum tag_e_selfcheck_fault_type
{
	//自检

	//无故障
	E_SELFCHECK_FAULT_TYPE_NORMAL = 0,
	//"红外监测模块
	E_SELFCHECK_FAULT_TYPE_INFRARED_MONITORING_MODULE,
	//"燃弧监测模块
	E_SELFCHECK_FAULT_TYPE_ARC_MONITORING_MODULE,
	//"云台故障
	E_SELFCHECK_FAULT_TYPE_HOLDER_FAULT,
	//"监控相机故障
	E_SELFCHECK_FAULT_TYPE_MONITOR_CAMERA_STATUS,
	//"车顶监测设备
	E_SELFCHECK_FAULT_TYPE_ROOF_MONITORING_EQUIPMENT,
	//"车底补偿装置
	E_SELFCHECK_FAULT_TYPE_CAR_BOTTOM_COMPENSATOR,
	//"电流传感器
	E_SELFCHECK_FAULT_TYPE_CURRENT_SENSOR,
	//"工控机故障
	E_SELFCHECK_FAULT_TYPE_IPC_FAULT,
	//"硬盘故障
	E_SELFCHECK_FAULT_TYPE_RIGID_DISK_FAULT,
	//"数据分析服务器故障
	E_SELFCHECK_FAULT_TYPE_DATA_ANALYSIS_SERVER_STATUS,
	//"几何参数监测
	E_SELFCHECK_FAULT_TYPE_GEOMETRIC_PARAMETER_MONITORING,
}e_selfcheck_fault_type;

typedef struct tag_st_db_record  
{
	tag_st_db_record()
	{
		CameraPos = 0;
		CarriageId = 0;
		CarriageNum = 0;
		StationId = 0;
		LineId = 0;
		Direction = 0;
		FileName = "";
		FullName = "";
		BeginTime = "";
		DeleteFlag = 0;
		AlarmFlag = 0;
		EndTime = "";
		RecordType = 0;
		VideoType = 0;
	}
	int CameraPos;
	int CarriageId;//列车编号（TCMS获取）
	int CarriageNum;// 车厢号(配置文件中读取)
	int StationId;//站点seq
	int LineId;// 线路号(配置文件中读取)
	int Direction;//行驶方向
	std::string FileName;
	std::string FullName;
	std::string BeginTime;
	int DeleteFlag;
	int AlarmFlag;
	int RecordType;//reversed
	int VideoType;//reversed
	std::string EndTime;//
}st_db_record;

typedef struct tag_st_db_alarm  
{
	tag_st_db_alarm()
	{
		CameraPos = 0;
		CarriageId = 0;
		CarriageNum = 0;
		StationId = 0;
		LineId = 0;
		Direction = 0;
		FileName = "";
		FullName = "";
		BeginTime = "";
		DeleteFlag = 0;
		AlarmType = 0;
		AlarmLevel = 0;
		AlarmMessage = "";
		VideoFileName = "";
		VideoFullName = "";
		AlarmValue = 0.0;
		SendFlag = 0;
	}
	int CameraPos;
	int CarriageId;//列车编号（TCMS获取）
	int CarriageNum;// 车厢号(配置文件中读取)
	int StationId;//站点seq
	int LineId;// 线路号(配置文件中读取)
	int Direction;//行驶方向
	std::string FileName;
	std::string FullName;
	std::string BeginTime;
	int DeleteFlag;
	int AlarmType;// 报警类型1,拉出值；2火花；3
	int AlarmLevel;// 报警级别
	std::string AlarmMessage;//报警内容
	double AlarmValue;//报警值
	std::string VideoFileName;//
	std::string VideoFullName;//
	int SendFlag;//
}st_db_alarm;

typedef struct tag_st_rt_data  
{
	tag_st_rt_data()
	{
		CameraPos = 0;
		CarriageId = 0;
		CarriageNum = 0;
		StationInd = 0;
		LineId = 0;
		Direction = 0;
		DataType = 0;
		RtData = "";
		BeginTime = "";
		BeginTimeMs = 0;
		LastTimeMs = 0;
	}
	int CameraPos;
	int CarriageId;//列车编号（TCMS获取）
	int CarriageNum;// 车厢号(配置文件中读取)
	int StationInd;//站点下标
	int LineId;// 线路号(配置文件中读取)
	int Direction;//行驶方向
	int DataType;//数据类型
	std::string RtData;//值;"%.03f";"%.03f;%.03f;%.03f;%.03f";"%.03f;%.03f";
	std::string BeginTime;
	int BeginTimeMs;
	int LastTimeMs;
}st_rt_data;

typedef enum tag_e_frame_analyse_type
{
	//无需分析
	E_FRAME_ANALYSE_TYPE_NONE = 0,
	//"结构
	E_FRAME_ANALYSE_TYPE_STRUCT,
}e_frame_analyse_type;

typedef enum tag_e_rt_type
{
	E_RT_TYPE_TEMP = 0,
	E_RT_TYPE_ARC_RATE,
	E_RT_TYPE_ARC_STRENGTH,
	E_RT_TYPE_GEO
}e_rt_type;

typedef struct tag_st_dev_state
{
	tag_st_dev_state()
	{
		heartHeat = 0;
		faultCount = 0;
	}
	unsigned int heartHeat;
	unsigned int faultCount;
}st_dev_state;

typedef struct tag_st_alarm
{
	tag_st_alarm()
	{
		AlarmLevel = E_FRAME_WARN_CLS_0;
		AlarmValue = 0;
		AlarmType = E_FRAME_WARN_TYPE_NORMAL;
	}
	unsigned int AlarmLevel;// 报警级别
	double AlarmValue;//报警值
	unsigned int AlarmType;
}st_alarm;

typedef struct tag_st_pull_guid
{
	tag_st_pull_guid()
	{
		isTwoValue = false;
		pull1 = 0;
		guid1 = 0;
		pull2 = 0;
		guid2 = 0;
	}

	bool isTwoValue;
	double pull1;
	double guid1;
	double pull2;
	double guid2;
}st_pull_guid;

typedef struct tag_st_arc_inf
{
	tag_st_arc_inf()
	{
		beginTimeM = 0;
		endTimeM = 0;
		arcVal = 0;
		packInd = 0;
	}
	time_t beginTimeM;//相机中时间戳
	time_t endTimeM;//相机中时间戳
	unsigned int arcVal;//本次发送的所有数据包（可能是1包或者多包）的燃弧值大小总和为本次燃弧的大小值
	unsigned int packInd;//一次燃弧的包编号，分包中该值相等
}st_arc_inf;

typedef struct tag_st_current_inf
{
	tag_st_current_inf()
	{
		beginTimeM = 0;
		endTimeM = 0;
		currentVal = 0;
	}
	time_t beginTimeM;//开始时间戳,毫秒
	time_t endTimeM;//结束时间戳
	unsigned int currentVal;
}st_current_inf;

typedef struct tag_st_pis_pack_inf
{
	tag_st_pis_pack_inf()
	{
		pData = NULL;
		iDataLength = 0;
	}
	char* pData;//数据
	int iDataLength;//数据长度
}st_pis_pack_inf;

typedef struct tag_st_arc_camera
{
	UINT32     flag1;  
	UINT32     flag2; 
	UINT16     nWidth;                //Normal模式下，为实际值；Snap模式下，固定为0
	UINT16     nHeight;                //Normal模式下，为实际值；Snap模式下，固定为1
	UINT32     nImagePackageLen;    //Snap模式下，该值即为数据宽度
	UINT32     nCounter;  // 本包数据第一个采样点的编号，从上电开始一直编号0-0xffffffff。
	UINT16     nImageInfo;            //normal模式下是0x3001，Snap模式下0x4000
	UINT16     nCalcResultPrecion;
	UINT32     sIndex;                //本包数据内的第一个采样的编号（一次燃弧中，第一包第一个为0）
	UINT32     eIndex;                //本包数据内的，最后一个采样点的编号
	UINT32     aIndex;                 //本次燃弧，长传数据包所属的编号
	UINT32     aSize;                //本包内的燃弧大小：本包的燃弧累加值*系数
	UINT32     nReserved[6];
}st_arc_camera;

///////////////////////////////////////////////////////////

