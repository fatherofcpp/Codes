#pragma once

#include "CmDefStruct.h"
#include <string>

typedef enum tag_e_cam_class
{
	E_CAM_CLASS_MONITOR=0,
	E_CAM_CLASS_MONITOR_SPARE,
	E_CAM_CLASS_STRUCT,
	E_CAM_CLASS_TEMPRATE,
	E_CAM_CLASS_ARC_NOTIFY,
	E_CAM_CLASS_ARC_COLLECT,
	E_CAM_CLASS_TATOL,
	E_CAM_CLASS_PULL_GUID = 10
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
		udpCurrentPort = 0;
		tcpSparePort = 0;
		httpHkPisLocalPort = 0;
		httpHkPisOutPort = 0;
		httpHkPisOutIp = "";
		videoRootPath = "";
		videoRootPathAlarm = "";
		pictureRootPath = "";
		pictureRootPathAlarm = "";
		exportInfRootPath = "";
		diskNeedSpace = 0;
		fps = 0;
		arcBeginThreshold = 0;
		arcEndThreshold = 0;
		arcDeltTimeMsThreshold = 0;
		arcContinuTimeUsThreshold = 0;
		arcStrenthXsThreshold = 0;
		arcTimeLenGainThreshold = 0;
		saveFrameSuffix = "";
		tempMapFrameSuffix = "";
		saveVideoSuffix = "";
		carriageNum = 0;
		lineId = 0;
		trainNum = 0;
		presetNum = 0;
		PantoPos = 0;
		ratedCurrentThreshold = 0;
		arcRateThresholdCls1 = 0;
		arcRateThresholdCls2 = 0;
		arcRateThresholdCls3 = 0;
		tempThresholdCls1 = 0;
		tempThresholdCls2 = 0;
		tempThresholdCls3 = 0;
		pullThresholdCls1 = 0;
		pullThresholdCls2 = 0;
		pullThresholdCls3 = 0;
		guidThresholdCls1 = 0;
		guidThresholdCls2 = 0;
		guidThresholdCls3 = 0;
		dataCenterTimeoutS = 0;
		cameraTimeoutS = 0;
		currentTimeoutS = 0;
		geoTimeoutS = 0;
		waitSystemTs = 0;
		useQuickExit = 0;
		useFrameAnalyze = 0;
		videoMaxSec = 0;
	}

	st_mvb_com_param mvbComParam;//MVB参数
	st_scm_param scmComParam;//单片机参数

	st_data_center_service SrvdataCenter;//地面服务器
	NSP_STP_CM::st_report_service_inf SrvSpare;//备份服务器信息

	int tcpPisPort;//pis监听端口
	int tcpPtuPort;//ptu监听端口
	int udpPtuPort;//ptu udp端口
	int udpGeoRecvPort;//几何参数监听端口
	int udpGeoSendPort;//几何参数发送端口
	int udpCurrentPort;//电流检测监听端口
	int tcpSparePort;//备份监听端口
	int httpHkPisLocalPort;//华开Pis监听端口
	int httpHkPisOutPort;//
	std::string httpHkPisOutIp;//

	int carriageNum;// 车厢号
	int lineId;// 线路号(配置文件中读取)
	int trainNum;//列车编号(新增配置文件读取)
	int presetNum;//预置点个数
	int PantoPos;// 受电弓位置号

	std::string videoRootPath;//视频保存根目录
	std::string videoRootPathAlarm;//报警视频保存根目录
	std::string pictureRootPath;//图片保存根目录
	std::string pictureRootPathAlarm;//报警图片保存根目录
	std::string exportInfRootPath;//导出信息根目录
	int diskNeedSpace;//磁盘预留空间
	float fps;//实际帧率
	std::string saveFrameSuffix;//图片后缀
	std::string tempMapFrameSuffix;//温度映射后缀
	std::string saveVideoSuffix;//视频后缀
	NSP_STP_CM::st_dbmysql_param dbMySqlParam;//数据库参数

	int ratedCurrentThreshold;//额定电流,单位0.3
	int arcBeginThreshold;//燃弧开始阀值0-255;default:200
	int arcEndThreshold;//燃弧开始阀值0-255;default:100
	int arcDeltTimeMsThreshold;//燃弧间隔时间;default:500
	int arcContinuTimeUsThreshold;//燃弧持续时间:us;default:1
	int arcStrenthXsThreshold;//燃弧强度系数
	int arcTimeLenGainThreshold;
	double arcRateThresholdCls1;//燃弧率阀值,单位0.01
	double arcRateThresholdCls2;//
	double arcRateThresholdCls3;//
	double tempThresholdCls1;//温度阀值,单位摄氏度
	double tempThresholdCls2;//
	double tempThresholdCls3;//
	double pullThresholdCls1;
	double pullThresholdCls2;
	double pullThresholdCls3;
	double guidThresholdCls1;
	double guidThresholdCls2;
	double guidThresholdCls3;
	int dataCenterTimeoutS;
	int cameraTimeoutS;
	int currentTimeoutS;
	int geoTimeoutS;
	int waitSystemTs;
	int useQuickExit;
	int useFrameAnalyze;
	int videoMaxSec;//单个视频最大时长
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
	E_RT_TYPE_GEO,
	E_RT_TYPE_CURRENT
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

		arcPointNum = 0;
		memset(arcPointVal, 0, sizeof(arcPointVal));
	}
	time_t beginTimeM;//相机中时间戳
	time_t endTimeM;//相机中时间戳
	unsigned int arcVal;//本次发送的所有数据包（可能是1包或者多包）的燃弧值大小总和为本次燃弧的大小值
	unsigned int packInd;//一次燃弧的包编号，分包中该值相等

	char arcPointVal[1024];//燃弧产生期间的燃弧Y值数组,按最大燃弧时长60秒,每秒记录10个点。
	int arcPointNum;//实际Y值点数
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

namespace NSP_BS_CM
{
	enum PixelFormatEnums
	{
		PixelFormat_Mono8 = 0,  //!<Sets the pixel format to Mono 8
		PixelFormat_Mono10,  //!<Sets the pixel format to Mono 10
		PixelFormat_Mono10Packed,  //!<Sets the pixel format to Mono 10 Packed
		PixelFormat_Mono12,  //!<Sets the pixel format to Mono 12
		PixelFormat_Mono12Packed,  //!<Sets the pixel format to Mono 12 Packed
		PixelFormat_Mono16,  //!<Sets the pixel format to Mono 16
		PixelFormat_BayerGR8,  //!<Sets the pixel format to Bayer GR 8
		PixelFormat_BayerRG8,  //!<Sets the pixel format to Bayer RG 8
		PixelFormat_BayerGB8,  //!<Sets the pixel format to Bayer GB 8
		PixelFormat_BayerBG8,  //!<Sets the pixel format to Bayer BG 8
		PixelFormat_BayerGR10,  //!<Sets the pixel format to Bayer GR 10
		PixelFormat_BayerRG10,  //!<Sets the pixel format to Bayer RG 10
		PixelFormat_BayerGB10,  //!<Sets the pixel format to Bayer GB 10
		PixelFormat_BayerBG10,  //!<Sets the pixel format to Bayer BG 10
		PixelFormat_BayerGR12,  //!<Sets the pixel format to Bayer GR 12
		PixelFormat_BayerRG12,  //!<Sets the pixel format to Bayer RG 12
		PixelFormat_BayerGB12,  //!<Sets the pixel format to Bayer GB 12
		PixelFormat_BayerBG12,  //!<Sets the pixel format to Bayer BG 12
		PixelFormat_RGB8Packed,  //!<Sets the pixel format to RGB 8 Packed
		PixelFormat_BGR8Packed,  //!<Sets the pixel format to BGR 8 Packed
		PixelFormat_RGBA8Packed,  //!<Sets the pixel format to RGBA 8 Packed
		PixelFormat_BGRA8Packed,  //!<Sets the pixel format to BGRA 8 Packed
		PixelFormat_RGB10Packed,  //!<Sets the pixel format to RGB 10 Packed
		PixelFormat_BGR10Packed,  //!<Sets the pixel format to BGR 10 Packed
		PixelFormat_RGB12Packed,  //!<Sets the pixel format to RGB 12 Packed
		PixelFormat_BGR12Packed,  //!<Sets the pixel format to BGR 12 Packed
		PixelFormat_RGB10V1Packed,  //!<Sets the pixel format to RGB 10V1 Packed
		PixelFormat_RGB10V2Packed,  //!<Sets the pixel format to RGB 10V2 Packed
		PixelFormat_YUV411Packed,  //!<Sets the pixel format to YUV 411 Packed
		PixelFormat_YUV422Packed,  //!<Sets the pixel format to YUV 422 Packed
		PixelFormat_YUV444Packed,  //!<Sets the pixel format to YUV 444 Packed
		PixelFormat_RGB8Planar,  //!<Sets the pixel format to RGB 8 Planar
		PixelFormat_RGB10Planar,  //!<Sets the pixel format to RGB 10 Planar
		PixelFormat_RGB12Planar,  //!<Sets the pixel format to RGB 12 Planar
		PixelFormat_RGB16Planar,  //!<Sets the pixel format to RGB 16 Planar
		PixelFormat_YUV422_YUYV_Packed,  //!<Sets the pixel format to YUV 422 (YUYV) Packed
		PixelFormat_BayerGB12Packed,  //!<Sets the pixel format to Bayer GB 12 Packed
		PixelFormat_BayerGR12Packed,  //!<Sets the pixel format to Bayer GR 12 Packed
		PixelFormat_BayerRG12Packed,  //!<Sets the pixel format to Bayer RG 12 Packed
		PixelFormat_BayerBG12Packed,  //!<Sets the pixel format to Bayer BG 12 Packed
		PixelFormat_BayerGR16,  //!<Sets the pixel format to Bayer GR 16
		PixelFormat_BayerRG16,  //!<Sets the pixel format to Bayer RG 16
		PixelFormat_BayerGB16,  //!<Sets the pixel format to Bayer GB 16
		PixelFormat_BayerBG16,  //!<Sets the pixel format to Bayer BG 16
		PixelFormat_RGB12V1Packed,  //!<Sets the pixel format to RGB 12 Packed 
		PixelFormat_Mono8Signed   //!<Sets the pixel format to Mono 8 Signed
	};
}

typedef struct tagAnalyzeRes
{
	tagAnalyzeRes()
	{
		alarm_type = E_FRAME_WARN_TYPE_NORMAL;
		alarm_cls = E_FRAME_WARN_CLS_0;
		alarm_msg = "";
		alarm_val = 0.0;
		alarm_val_param = "";
	}
	tag_e_frame_warn_type alarm_type; //图像报警类型
	tag_e_frame_warn_cls alarm_cls; //图像报警等级
	std::string alarm_msg; //图像报警内容;
	double alarm_val;// 图像报警数值
	std::string alarm_val_param;//图像报警数值附带信息，如温度坐标。
}AnalyzeRes;

typedef struct tagAnalyzeResArr
{
	tagAnalyzeResArr()
	{
		resNum = 0;
	}
	int resNum;
	AnalyzeRes resArr[64];
}AnalyzeResArr;

typedef struct 
{
	float fDistance;//目标距离, 单位m, 范围(2*fFocalLength, ∞)
	float fEmissivity;//全局缺省发射率，范围(0,1]
	float fTemp;//气温, 单位C
	float fRH;//相对湿度，范围(0~1)
	float fVisDistance;//能见度, 单位km, 范围(0, ∞)
	float fRain;//降雨强度, 单位mm/h, 范围(0, ∞)
	float fSnow;//降雪强度, 单位mm/h, 范围(0, ∞)
	float fExtrapara1;//修正参数1 镜头相机相关
	float fExtrapara2;//修正参数2 镜头相机相关
	float fTaoAtm;//大气透过率, 只读
	float fTaoFilter;//(热像仪外部的)窗口/滤光片透过率
}struct_FixPara;

///////////////////////////////////////////////////////////

