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

	st_mvb_com_param mvbComParam;//MVB����
	st_scm_param scmComParam;//��Ƭ������

	st_data_center_service SrvdataCenter;//���������
	int tcpPisPort;//pis�����˿�
	int tcpPtuPort;//ptu�����˿�
	int udpPtuPort;//ptu udp�˿�
	int udpGeoRecvPort;//���β��������˿�
	int udpGeoSendPort;//���β������Ͷ˿�
	int udpGeoSendPortDown;//���β������׷��Ͷ˿�
	int udpCurrentPort;//�����������˿�

	int carriageNum;// �����
	int lineId;// ��·��(�����ļ��ж�ȡ)
	int presetNum;//Ԥ�õ����
	int PantoPos;// �ܵ繭λ�ú�

	std::string videoRootPath;//��Ƶ�����Ŀ¼
	std::string videoRootPathAlarm;//������Ƶ�����Ŀ¼
	std::string pictureRootPath;//ͼƬ�����Ŀ¼
	std::string pictureRootPathAlarm;//����ͼƬ�����Ŀ¼
	int diskNeedSpace;//����Ԥ���ռ�
	float fps;//ʵ��֡��
	std::string saveFrameSuffix;//ͼƬ��׺
	std::string saveVideoSuffix;//��Ƶ��׺
	NSP_STP_CM::st_dbmysql_param dbMySqlParam;//���ݿ����

	int ratedCurrentThreshold;//�����,��λ0.3
	int arcBeginThreshold;//ȼ����ʼ��ֵ0-255;default:200
	int arcEndThreshold;//ȼ����ʼ��ֵ0-255;default:100
	double arcRateThresholdCls1;//ȼ���ʷ�ֵ,��λ0.01
	double arcRateThresholdCls2;//
	double arcRateThresholdCls3;//
	double tempThresholdCls1;//�¶ȷ�ֵ,��λ���϶�
	double tempThresholdCls2;//
	double tempThresholdCls3;//
	int waitSystemTs;
	int jgqPreReadyTms;
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
	}
	time_t beginTimeM;//�����ʱ���
	time_t endTimeM;//�����ʱ���
	unsigned int arcVal;//���η��͵��������ݰ���������1�����߶������ȼ��ֵ��С�ܺ�Ϊ����ȼ���Ĵ�Сֵ
	unsigned int packInd;//һ��ȼ���İ���ţ��ְ��и�ֵ���
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

///////////////////////////////////////////////////////////

