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
}e_cam_pos;//λ�ð��ţ����������λ�ú�Ԥ�� 01~07 �����ܵ繭�����λ�ú� 08~09

typedef struct tag_st_mvb_com_param
{
	tag_st_mvb_com_param()
	{
		mvbSerialPort = "";
		mvbBuadRate = 0;
		mpCode = 0;
	}

	std::string mvbSerialPort;//MVB���ں�
	int mvbBuadRate;//MVB������
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

	st_mvb_com_param mvbComParam;//MVB����
	st_scm_param scmComParam;//��Ƭ������

	st_data_center_service SrvdataCenter;//���������
	NSP_STP_CM::st_report_service_inf SrvSpare;//���ݷ�������Ϣ

	int tcpPisPort;//pis�����˿�
	int tcpPtuPort;//ptu�����˿�
	int udpPtuPort;//ptu udp�˿�
	int udpGeoRecvPort;//���β��������˿�
	int udpGeoSendPort;//���β������Ͷ˿�
	int udpCurrentPort;//�����������˿�
	int tcpSparePort;//���ݼ����˿�
	int httpHkPisLocalPort;//����Pis�����˿�
	int httpHkPisOutPort;//
	std::string httpHkPisOutIp;//

	int carriageNum;// �����
	int lineId;// ��·��(�����ļ��ж�ȡ)
	int trainNum;//�г����(���������ļ���ȡ)
	int presetNum;//Ԥ�õ����
	int PantoPos;// �ܵ繭λ�ú�

	std::string videoRootPath;//��Ƶ�����Ŀ¼
	std::string videoRootPathAlarm;//������Ƶ�����Ŀ¼
	std::string pictureRootPath;//ͼƬ�����Ŀ¼
	std::string pictureRootPathAlarm;//����ͼƬ�����Ŀ¼
	std::string exportInfRootPath;//������Ϣ��Ŀ¼
	int diskNeedSpace;//����Ԥ���ռ�
	float fps;//ʵ��֡��
	std::string saveFrameSuffix;//ͼƬ��׺
	std::string tempMapFrameSuffix;//�¶�ӳ���׺
	std::string saveVideoSuffix;//��Ƶ��׺
	NSP_STP_CM::st_dbmysql_param dbMySqlParam;//���ݿ����

	int ratedCurrentThreshold;//�����,��λ0.3
	int arcBeginThreshold;//ȼ����ʼ��ֵ0-255;default:200
	int arcEndThreshold;//ȼ����ʼ��ֵ0-255;default:100
	int arcDeltTimeMsThreshold;//ȼ�����ʱ��;default:500
	int arcContinuTimeUsThreshold;//ȼ������ʱ��:us;default:1
	int arcStrenthXsThreshold;//ȼ��ǿ��ϵ��
	int arcTimeLenGainThreshold;
	double arcRateThresholdCls1;//ȼ���ʷ�ֵ,��λ0.01
	double arcRateThresholdCls2;//
	double arcRateThresholdCls3;//
	double tempThresholdCls1;//�¶ȷ�ֵ,��λ���϶�
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
	int videoMaxSec;//������Ƶ���ʱ��
}st_local_conf;

typedef enum tag_e_frame_warn_type
{
	//��������

	//�ޱ���
	E_FRAME_WARN_TYPE_NORMAL = 0,
	//"�����¶�
	E_FRAME_WARN_TYPE_PANTEM,
	//"ȼ����
	E_FRAME_WARN_TYPE_ARCRATE,
	//"����¶�
	E_FRAME_WARN_TYPE_SLOPE,
	//"����ֵ����
	E_FRAME_WARN_TYPE_GUIDE_HEIGHT,
	//"����ֵ����
	E_FRAME_WARN_TYPE_PULLOUT,
	//"��������
	E_FRAME_WARN_TYPE_FOREIGN_MATTER,
	//"�ܵ繭�ṹ�쳣
	E_FRAME_WARN_TYPE_STRUCTURE_ABNORMAL,
	//"�Ӵ��߸߶Ȳ�
	E_FRAME_WARN_TYPE_CONTACT_LINE_HEIGHT_DIFFERENCE,
	//"������б
	E_FRAME_WARN_TYPE_SLIDE_TILT,
	//"���嶪ʧ
	E_FRAME_WARN_TYPE_SKATEBOARD_MISSING,
	//"����쳣
	E_FRAME_WARN_TYPE_HORN_ANOMALY,
}e_frame_warn_type;

typedef enum tag_e_frame_warn_cls
{
	//�����ȼ�

	E_FRAME_WARN_CLS_0 = 0,//��
	E_FRAME_WARN_CLS_1,//1��
	E_FRAME_WARN_CLS_2,//2��
	E_FRAME_WARN_CLS_3//3��
}e_frame_warn_cls;

typedef enum tag_e_selfcheck_fault_type
{
	//�Լ�

	//�޹���
	E_SELFCHECK_FAULT_TYPE_NORMAL = 0,
	//"������ģ��
	E_SELFCHECK_FAULT_TYPE_INFRARED_MONITORING_MODULE,
	//"ȼ�����ģ��
	E_SELFCHECK_FAULT_TYPE_ARC_MONITORING_MODULE,
	//"��̨����
	E_SELFCHECK_FAULT_TYPE_HOLDER_FAULT,
	//"����������
	E_SELFCHECK_FAULT_TYPE_MONITOR_CAMERA_STATUS,
	//"��������豸
	E_SELFCHECK_FAULT_TYPE_ROOF_MONITORING_EQUIPMENT,
	//"���ײ���װ��
	E_SELFCHECK_FAULT_TYPE_CAR_BOTTOM_COMPENSATOR,
	//"����������
	E_SELFCHECK_FAULT_TYPE_CURRENT_SENSOR,
	//"���ػ�����
	E_SELFCHECK_FAULT_TYPE_IPC_FAULT,
	//"Ӳ�̹���
	E_SELFCHECK_FAULT_TYPE_RIGID_DISK_FAULT,
	//"���ݷ�������������
	E_SELFCHECK_FAULT_TYPE_DATA_ANALYSIS_SERVER_STATUS,
	//"���β������
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
	int CarriageId;//�г���ţ�TCMS��ȡ��
	int CarriageNum;// �����(�����ļ��ж�ȡ)
	int StationId;//վ��seq
	int LineId;// ��·��(�����ļ��ж�ȡ)
	int Direction;//��ʻ����
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
	int CarriageId;//�г���ţ�TCMS��ȡ��
	int CarriageNum;// �����(�����ļ��ж�ȡ)
	int StationId;//վ��seq
	int LineId;// ��·��(�����ļ��ж�ȡ)
	int Direction;//��ʻ����
	std::string FileName;
	std::string FullName;
	std::string BeginTime;
	int DeleteFlag;
	int AlarmType;// ��������1,����ֵ��2�𻨣�3
	int AlarmLevel;// ��������
	std::string AlarmMessage;//��������
	double AlarmValue;//����ֵ
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
	int CarriageId;//�г���ţ�TCMS��ȡ��
	int CarriageNum;// �����(�����ļ��ж�ȡ)
	int StationInd;//վ���±�
	int LineId;// ��·��(�����ļ��ж�ȡ)
	int Direction;//��ʻ����
	int DataType;//��������
	std::string RtData;//ֵ;"%.03f";"%.03f;%.03f;%.03f;%.03f";"%.03f;%.03f";
	std::string BeginTime;
	int BeginTimeMs;
	int LastTimeMs;
}st_rt_data;

typedef enum tag_e_frame_analyse_type
{
	//�������
	E_FRAME_ANALYSE_TYPE_NONE = 0,
	//"�ṹ
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
	unsigned int AlarmLevel;// ��������
	double AlarmValue;//����ֵ
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
	time_t beginTimeM;//�����ʱ���
	time_t endTimeM;//�����ʱ���
	unsigned int arcVal;//���η��͵��������ݰ���������1�����߶������ȼ��ֵ��С�ܺ�Ϊ����ȼ���Ĵ�Сֵ
	unsigned int packInd;//һ��ȼ���İ���ţ��ְ��и�ֵ���

	char arcPointVal[1024];//ȼ�������ڼ��ȼ��Yֵ����,�����ȼ��ʱ��60��,ÿ���¼10���㡣
	int arcPointNum;//ʵ��Yֵ����
}st_arc_inf;

typedef struct tag_st_current_inf
{
	tag_st_current_inf()
	{
		beginTimeM = 0;
		endTimeM = 0;
		currentVal = 0;
	}
	time_t beginTimeM;//��ʼʱ���,����
	time_t endTimeM;//����ʱ���
	unsigned int currentVal;
}st_current_inf;

typedef struct tag_st_pis_pack_inf
{
	tag_st_pis_pack_inf()
	{
		pData = NULL;
		iDataLength = 0;
	}
	char* pData;//����
	int iDataLength;//���ݳ���
}st_pis_pack_inf;

typedef struct tag_st_arc_camera
{
	UINT32     flag1;  
	UINT32     flag2; 
	UINT16     nWidth;                //Normalģʽ�£�Ϊʵ��ֵ��Snapģʽ�£��̶�Ϊ0
	UINT16     nHeight;                //Normalģʽ�£�Ϊʵ��ֵ��Snapģʽ�£��̶�Ϊ1
	UINT32     nImagePackageLen;    //Snapģʽ�£���ֵ��Ϊ���ݿ��
	UINT32     nCounter;  // �������ݵ�һ��������ı�ţ����ϵ翪ʼһֱ���0-0xffffffff��
	UINT16     nImageInfo;            //normalģʽ����0x3001��Snapģʽ��0x4000
	UINT16     nCalcResultPrecion;
	UINT32     sIndex;                //���������ڵĵ�һ�������ı�ţ�һ��ȼ���У���һ����һ��Ϊ0��
	UINT32     eIndex;                //���������ڵģ����һ��������ı��
	UINT32     aIndex;                 //����ȼ�����������ݰ������ı��
	UINT32     aSize;                //�����ڵ�ȼ����С��������ȼ���ۼ�ֵ*ϵ��
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
	tag_e_frame_warn_type alarm_type; //ͼ�񱨾�����
	tag_e_frame_warn_cls alarm_cls; //ͼ�񱨾��ȼ�
	std::string alarm_msg; //ͼ�񱨾�����;
	double alarm_val;// ͼ�񱨾���ֵ
	std::string alarm_val_param;//ͼ�񱨾���ֵ������Ϣ�����¶����ꡣ
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
	float fDistance;//Ŀ�����, ��λm, ��Χ(2*fFocalLength, ��)
	float fEmissivity;//ȫ��ȱʡ�����ʣ���Χ(0,1]
	float fTemp;//����, ��λC
	float fRH;//���ʪ�ȣ���Χ(0~1)
	float fVisDistance;//�ܼ���, ��λkm, ��Χ(0, ��)
	float fRain;//����ǿ��, ��λmm/h, ��Χ(0, ��)
	float fSnow;//��ѩǿ��, ��λmm/h, ��Χ(0, ��)
	float fExtrapara1;//��������1 ��ͷ������
	float fExtrapara2;//��������2 ��ͷ������
	float fTaoAtm;//����͸����, ֻ��
	float fTaoFilter;//(�������ⲿ��)����/�˹�Ƭ͸����
}struct_FixPara;

///////////////////////////////////////////////////////////

