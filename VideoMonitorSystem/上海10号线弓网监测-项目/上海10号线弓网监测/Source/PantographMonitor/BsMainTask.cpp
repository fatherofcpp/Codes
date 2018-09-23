#include "StdAfx.h"
#include "BsMainTask.h"

#include "Log/CmLog.h"

#include "BsAbstractCamera.h"
#include "BsGeoParamAnalyse.h"
#include "BsAlarm.h"
#include "BsServicePis.h"
#include "BsServicePtu.h"
#include "BsServiceSpare.h"
#include "BsServiceHKPis.h"
#include "BsMvb.h"

#include "Bsmonitor.h"
#include "BsStructAnalyse.h"
#include "BsTemprateAnalyse.h"
#include "BsArcNotify.h"
#include "BsArcCollector.h"

#include "CmFunction.h"
#include "BsDllCamPicture.h"
#include "BsDllCamVideo.h"
#include "BsDllMvb.h"
#include "BsDllFrameAnalyze.h"
#include "PantographMonitor.h"

#include "DataBase/CmDBMySql.h"
#include "Mvb/ServerTrans.h"
#include "BsGlobal.h"
#include "CamPicture/CameraObject.h"
#include "CamVideo/VideoObjectDevSer.h"
#include "BsDefStruct.h"
#include "BsFunction.h"
#include "BsTimeStamp.h"

#include "Socket/CmTransUdp.h"

#include "CmOpenCvDll.h"

/////////////
#define MAINTASK_TEST 1
#define MAINTASK_TEST_USE_DAS 0	//仅测试帧率使用

#if MAINTASK_TEST_USE_DAS
#include "Basler/DAS.h"
#endif
/////////////

static const int g_cam_num = E_CAM_CLASS_TATOL;

CMainTask::CMainTask(void)
{
	USES_CONVERSION;

	int ret = 0;

	//new
	m_pLocalConf = new tag_st_local_conf;
	m_CAbstractModuleArr[E_CAM_CLASS_MONITOR] = (CAbstractCamera*)new CMonitor;
	m_CAbstractModuleArr[E_CAM_CLASS_MONITOR_SPARE] = (CAbstractCamera*)new CMonitor;
	m_CAbstractModuleArr[E_CAM_CLASS_STRUCT] = (CAbstractCamera*)new CStructAnalyse;
	m_CAbstractModuleArr[E_CAM_CLASS_TEMPRATE] = (CAbstractCamera*)new CTemprateAnalyse;
	m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY] = (CAbstractCamera*)new CArcNotify;
	m_CAbstractModuleArr[E_CAM_CLASS_ARC_COLLECT] = (CAbstractCamera*)new CArcCollector;

#if MAINTASK_TEST_USE_DAS
	m_pCDAS = HXZY::TEDS::CDAS::GetInstance();
#endif

	m_pCAnalyseGeoParam = new CGeoParamAnalyse;
	m_pCAlarm = new CAlarm;
	m_pCServicePis = new CServicePis;
	m_pCServicePtu = new CServicePtu;
	m_pCServiceSpare = new CServiceSpare;
	m_pCServiceHKPis = new CServiceHKPis;
	m_pCMvb = new CMvb;

	//log init
	CString current_path = theApp.GetCurrentPath();
	current_path += "\\pan_log.log";
	gLog.InitLog(W2A(current_path), "PanLog");
	gLog.Add("%s ################Program Start################\n", __FUNCTION__);

	//version
#ifdef _DEBUG
	gLog.Add("%s %s Debug\n", __FUNCTION__, gVersion.c_str());
#else
	gLog.Add("%s %s Release\n", __FUNCTION__, gVersion.c_str());
#endif

	//cpu num
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	DWORD processors_num = system_info.dwNumberOfProcessors;
	int max_mask = (int)(0x01 << processors_num) - 1;
	gLog.Add("%s processors_num=%d\n", __FUNCTION__, processors_num);

	//config
	ret = LoadConfig();
	if (ret != 0)
	{
		gLog.Add("%s LoadConfig,ret=%d\n", __FUNCTION__, ret);
		exit(0);
	}

	//net init
	ret = NSP_STP_CM::SocketInit();
	if(ret != 0)
	{
		gLog.Add("%s SocketInit\n", __FUNCTION__);
		exit(0);
	}

	//dll
	g_pCMvbDll = new CMvbDll;
	ret = g_pCMvbDll->LoadDll();
	if (ret != 0)
	{
		gLog.Add("%s gCMvbDll.LoadDll\n", __FUNCTION__);
		exit(0);
	}

	g_pCCamPictureDll = new CCamPictureDll;
	ret = g_pCCamPictureDll->LoadDll();
	if (ret != 0)
	{
		gLog.Add("%s gCCamPictureDll.LoadDll\n", __FUNCTION__);
		exit(0);
	}

	g_pCCamVideoDll = new CCamVideoDll;
	ret = g_pCCamVideoDll->LoadDll();
	if (ret != 0)
	{
		gLog.Add("%s gCCamVideoDll.LoadDll\n", __FUNCTION__);
		exit(0);
	}

	g_pCFrameAnalyzeDll = new CFrameAnalyzeDll;
	g_pCFrameAnalyzeDll->m_height = (dynamic_cast<Cam::CamDeviceFormatInfo*>((m_CAbstractModuleArr[E_CAM_CLASS_STRUCT])->m_pAbstractFormatInfo))->height;
	g_pCFrameAnalyzeDll->m_width = (dynamic_cast<Cam::CamDeviceFormatInfo*>((m_CAbstractModuleArr[E_CAM_CLASS_STRUCT])->m_pAbstractFormatInfo))->width;
	ret = g_pCFrameAnalyzeDll->LoadDll();
	if (ret != 0)
	{
		gLog.Add("%s g_pCFrameAnalyzeDll.LoadDll\n", __FUNCTION__);
		exit(0);
	}

	m_pCAnalyseGeoParam->SetDbMySqlParam(&m_pLocalConf->dbMySqlParam);
	m_pCAnalyseGeoParam->SetCarrigeNum(m_pLocalConf->carriageNum);
	m_pCAnalyseGeoParam->SetLineId(m_pLocalConf->lineId);
	m_pCAnalyseGeoParam->SetGeoPort(m_pLocalConf->udpGeoRecvPort);
	m_pCAnalyseGeoParam->SetGeoThreshold(m_pLocalConf->pullThresholdCls1, m_pLocalConf->pullThresholdCls2, m_pLocalConf->pullThresholdCls3,
		m_pLocalConf->guidThresholdCls1, m_pLocalConf->guidThresholdCls2, m_pLocalConf->guidThresholdCls3);
	m_pCAnalyseGeoParam->SetMvb(m_pCMvb);
	m_pCAnalyseGeoParam->SetCStructAnalyse(dynamic_cast<CStructAnalyse*>(m_CAbstractModuleArr[E_CAM_CLASS_STRUCT]));
	m_pCAnalyseGeoParam->SetGeoTimeoutS(m_pLocalConf->geoTimeoutS);
	m_pCAnalyseGeoParam->SetPictureRootPathAlarm(m_pLocalConf->pictureRootPathAlarm);
	m_pCAnalyseGeoParam->SetFrameSuffix(m_pLocalConf->saveFrameSuffix);

	m_pCAlarm->SetMainTask(this);
	m_pCAlarm->SetMvb(m_pCMvb);
	m_pCAlarm->SetMonitor(dynamic_cast<CMonitor*>(m_CAbstractModuleArr[E_CAM_CLASS_MONITOR]));
	m_pCAlarm->SetReportService(&m_pLocalConf->SrvdataCenter);
	m_pCAlarm->SetPictureRootPath(m_pLocalConf->pictureRootPath);
	m_pCAlarm->SetPictureRootPathAlarm(m_pLocalConf->pictureRootPathAlarm);
	m_pCAlarm->SetVideoRootPath(m_pLocalConf->videoRootPath);
	m_pCAlarm->SetVideoRootPathAlarm(m_pLocalConf->videoRootPathAlarm);
	m_pCAlarm->SetExportInfRootPath(m_pLocalConf->exportInfRootPath);
	m_pCAlarm->SetFrameSuffix(m_pLocalConf->saveFrameSuffix);
	m_pCAlarm->SetVideoSuffix(m_pLocalConf->saveVideoSuffix);
	m_pCAlarm->SetDiskNeedSpace(m_pLocalConf->diskNeedSpace);
	m_pCAlarm->SetDbMySqlParam(&m_pLocalConf->dbMySqlParam);
	m_pCAlarm->SetCameraTimeoutS(m_pLocalConf->cameraTimeoutS);
	m_pCAlarm->SetDataCenterTimeoutS(m_pLocalConf->dataCenterTimeoutS);
	m_pCAlarm->SetVideoMaxSec(m_pLocalConf->videoMaxSec);

	m_pCServicePis->SetTcpPort(m_pLocalConf->tcpPisPort);
	m_pCServicePis->SetPanToPos(m_pLocalConf->PantoPos);
	m_pCServicePis->SetDbMySqlParam(&m_pLocalConf->dbMySqlParam);
	m_pCServicePis->SetMonitor(dynamic_cast<CMonitor*>(m_CAbstractModuleArr[E_CAM_CLASS_MONITOR]));

	m_pCServicePtu->SetTcpPort(m_pLocalConf->tcpPtuPort);
	m_pCServicePtu->SetUdpPtuPort(m_pLocalConf->udpPtuPort);
	m_pCServicePtu->SetPanToPos(m_pLocalConf->PantoPos);
	m_pCServicePtu->SetDbMySqlParam(&m_pLocalConf->dbMySqlParam);
	m_pCServicePtu->SetMonitor(dynamic_cast<CMonitor*>(m_CAbstractModuleArr[E_CAM_CLASS_MONITOR]));

	m_pCServiceSpare->SetTcpPort(m_pLocalConf->tcpSparePort);
	m_pCServiceSpare->SetMonitor(dynamic_cast<CMonitor*>(m_CAbstractModuleArr[E_CAM_CLASS_MONITOR_SPARE]));
	m_pCServiceSpare->SetServerInf(m_pLocalConf->SrvSpare.ip, m_pLocalConf->SrvSpare.port);

	m_pCServiceHKPis->SetHttpLocalPort(m_pLocalConf->httpHkPisLocalPort);
	m_pCServiceHKPis->SetHttpOutPort(m_pLocalConf->httpHkPisOutPort);
	m_pCServiceHKPis->SetHttpOutIp(m_pLocalConf->httpHkPisOutIp);
	m_pCServiceHKPis->SetMonitor(dynamic_cast<CMonitor*>(m_CAbstractModuleArr[E_CAM_CLASS_MONITOR]));
	m_pCServiceHKPis->SetDbMySqlParam(&m_pLocalConf->dbMySqlParam);

	m_pCMvb->SetMainTask(this);
	m_pCMvb->SetSerialInf(m_pLocalConf->mvbComParam.mvbSerialPort, m_pLocalConf->mvbComParam.mvbBuadRate,
		m_pLocalConf->scmComParam.scmSerialPort, m_pLocalConf->scmComParam.scmBuadRate);
	m_pCMvb->SetDbMySqlParam(&m_pLocalConf->dbMySqlParam);
	m_pCMvb->SetMvbPos(m_pLocalConf->mvbComParam.mpCode);
	m_pCMvb->SetTrainNum(m_pLocalConf->trainNum);
	m_pCMvb->SetVideoMaxSec(m_pLocalConf->videoMaxSec);

	(dynamic_cast<CTemprateAnalyse*>(m_CAbstractModuleArr[E_CAM_CLASS_TEMPRATE]))->SetTempThreshold(m_pLocalConf->tempThresholdCls1, 
		m_pLocalConf->tempThresholdCls2, m_pLocalConf->tempThresholdCls3);
	(dynamic_cast<CTemprateAnalyse*>(m_CAbstractModuleArr[E_CAM_CLASS_TEMPRATE]))->SetStructAnalyse((dynamic_cast<CStructAnalyse*>(m_CAbstractModuleArr[E_CAM_CLASS_STRUCT])));

	(dynamic_cast<CArcNotify*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY]))->SetUdpCurrentPort(m_pLocalConf->udpCurrentPort);
	(dynamic_cast<CArcNotify*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY]))->SetArcCollector((dynamic_cast<CArcCollector*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_COLLECT])));
	(dynamic_cast<CArcNotify*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY]))->SetStructAnalyse((dynamic_cast<CStructAnalyse*>(m_CAbstractModuleArr[E_CAM_CLASS_STRUCT])));
	(dynamic_cast<CArcNotify*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY]))->SetArcRateThreshold(m_pLocalConf->arcRateThresholdCls1, 
		m_pLocalConf->arcRateThresholdCls2, m_pLocalConf->arcRateThresholdCls3);
	(dynamic_cast<CArcNotify*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY]))->SetCurrentRated(m_pLocalConf->ratedCurrentThreshold);
	(dynamic_cast<CArcNotify*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY]))->SetArcThreshold(m_pLocalConf->arcBeginThreshold, m_pLocalConf->arcEndThreshold,
		m_pLocalConf->arcDeltTimeMsThreshold, m_pLocalConf->arcContinuTimeUsThreshold,
		m_pLocalConf->arcStrenthXsThreshold, m_pLocalConf->arcTimeLenGainThreshold);
	(dynamic_cast<CArcNotify*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_NOTIFY]))->SetCurrentTimeoutS(m_pLocalConf->currentTimeoutS);

	for (int i = 0; i < g_cam_num; i ++)
	{
		m_CAbstractModuleArr[i]->m_saveFrameSuffix = m_pLocalConf->saveFrameSuffix;
		m_CAbstractModuleArr[i]->m_tempMapFrameSuffix = m_pLocalConf->tempMapFrameSuffix;
		m_CAbstractModuleArr[i]->m_saveVideoSuffix = m_pLocalConf->saveVideoSuffix;
		m_CAbstractModuleArr[i]->m_diskNeedSpace = m_pLocalConf->diskNeedSpace;
		m_CAbstractModuleArr[i]->m_pCameraOpt->fps = m_pLocalConf->fps;
		m_CAbstractModuleArr[i]->m_carriageNum = m_pLocalConf->carriageNum;
		m_CAbstractModuleArr[i]->m_lineId = m_pLocalConf->lineId;
		m_CAbstractModuleArr[i]->m_useFrameAnalyze = (m_pLocalConf->useFrameAnalyze != 0);

		m_CAbstractModuleArr[i]->SetCameraInd(i);
		m_CAbstractModuleArr[i]->m_pCMvb = m_pCMvb;
		m_CAbstractModuleArr[i]->m_pDbMysqlParam = &m_pLocalConf->dbMySqlParam;
		if (i == E_CAM_CLASS_MONITOR || i == E_CAM_CLASS_MONITOR_SPARE)
		{
			m_CAbstractModuleArr[i]->m_rootPath = m_pLocalConf->videoRootPath;
			m_CAbstractModuleArr[i]->m_rootPathAlarm = m_pLocalConf->videoRootPathAlarm;
		}
		else
		{
			m_CAbstractModuleArr[i]->m_rootPath = m_pLocalConf->pictureRootPath;
			m_CAbstractModuleArr[i]->m_rootPathAlarm = m_pLocalConf->pictureRootPathAlarm;
		}
	}
	m_CAbstractModuleArr[E_CAM_CLASS_STRUCT]->m_frameAnalyzeType = E_FRAME_ANALYSE_TYPE_STRUCT;

	//init db
	char sql_cmd[1024];
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;
	p_CMySqlDB->SetMySqlServerInf(m_pLocalConf->dbMySqlParam.dbHost.c_str(), m_pLocalConf->dbMySqlParam.dbUser.c_str(), 
		m_pLocalConf->dbMySqlParam.dbPass.c_str(), m_pLocalConf->dbMySqlParam.dbName.c_str(), m_pLocalConf->dbMySqlParam.dbPort);

	//create db
	while(1)
	{
		ret = p_CMySqlDB->CreateDB();
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
		delete p_CMySqlDB;
		gLog.Add("%s CreateDB,ret=%d\n", __FUNCTION__, ret);
		exit(0);
	}

	ret = p_CMySqlDB->ConnectDataBase();
	if (ret != 0)
	{
		delete p_CMySqlDB;
		gLog.Add("%s ConnectDataBase,ret=%d\n", __FUNCTION__, ret);
		exit(0);
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
		"%s double DEFAULT NULL,"
		"%s VARCHAR(4096) DEFAULT NULL,"
		"%s VARCHAR(260) DEFAULT NULL,"
		"%s tinyint(1) DEFAULT '0'"
		")", gTblAlarm.c_str(),
		gAlarmFKey.c_str(), gAlarmFCameraPos.c_str(),
		gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), gAlarmFLineId.c_str(), 
		gAlarmFDirection.c_str(), gAlarmFFileName.c_str(), gAlarmFFullName.c_str(), gAlarmFBeginTime.c_str(), 
		gAlarmFAlarmType.c_str(), gAlarmFAlarmLevel.c_str(), gAlarmFAlarmMessage.c_str(), 
		gAlarmFVideoFileName.c_str(), gAlarmFVideoFullName.c_str(), gAlarmFAlarmValue.c_str(), gAlarmFSendFlag.c_str(),
		gAlarmFMileage.c_str(), gAlarmFAlarmValueParam.c_str(), gAlarmFStuctureFullName.c_str(), gAlarmFVideoBuildFlag.c_str()
		);
	ret = p_CMySqlDB->WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		delete p_CMySqlDB;
		gLog.Add("%s  WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		exit(0);
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
	ret = p_CMySqlDB->WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		delete p_CMySqlDB;
		gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		exit(0);
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
	ret = p_CMySqlDB->WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		delete p_CMySqlDB;
		gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		exit(0);
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
		"%s VARCHAR(4096) DEFAULT NULL,"
		"%s VARCHAR(64) DEFAULT NULL"
		")", gTblRt.c_str(),
		gRtFKey.c_str(),
		gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(), gRtFLineId.c_str(), 
		gRtFDirection.c_str(), gRtFSendFlag.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), 
		gRtFBeginTime.c_str(), gRtFBeginTimeMs.c_str(), gRtFLastTime.c_str(), gRtFFullName.c_str(),
		gRtFDataParam.c_str(), gRtFStationName.c_str()
		);
	ret = p_CMySqlDB->WriteDataBase(sql_cmd);
	if (ret != 0)
	{
		delete p_CMySqlDB;
		gLog.Add("%s  WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		exit(0);
	}

	delete p_CMySqlDB;
	//end init db

	//test
	Test();
}


CMainTask::~CMainTask(void)
{
	gLog.Add("%s begin\n", __FUNCTION__);

	delete g_pCMvbDll;
	delete g_pCCamPictureDll;
	delete g_pCCamVideoDll;
	delete g_pCFrameAnalyzeDll;

	for (int i = 0; i < g_cam_num; i ++)
	{
		if (m_CAbstractModuleArr[i])
		{
			delete m_CAbstractModuleArr[i];
			m_CAbstractModuleArr[i] = NULL;
		}
	}

	delete m_pCAnalyseGeoParam;
	m_pCAnalyseGeoParam = NULL;

	delete m_pCAlarm;
	m_pCAlarm = NULL;

	delete m_pCServicePis;
	m_pCServicePis = NULL;

	delete m_pCServicePtu;
	m_pCServicePtu = NULL;

	delete m_pCMvb;
	m_pCMvb = NULL;

	delete m_pLocalConf;
	m_pLocalConf = NULL;

#ifdef _DEBUG
	gLog.Add("%s Program Exit %s Debug\n", __FUNCTION__, gVersion.c_str());
#else
	gLog.Add("%s Program Exit %s Release\n", __FUNCTION__, gVersion.c_str());
#endif

	gLog.Add("%s ################Program Stop################\n", __FUNCTION__);
}

int CMainTask::TaskStart()
{
	int ret = 0;
#if MAINTASK_TEST
	ret = CTimeStamp::GetInstance()->TaskStart();
	return 0;
#endif
	ret = CTimeStamp::GetInstance()->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s CTimeStamp->TaskStart, ret=%d\n", __FUNCTION__, ret);
		return -1;
	}

	ret = m_pCMvb->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCMvb->TaskStart, ret=%d\n", __FUNCTION__, ret);
		return -1;
	}

	//wait system
	Sleep(1000 * m_pLocalConf->waitSystemTs);

	ret = m_pCAlarm->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCAlarm->TaskStart\n", __FUNCTION__);
		return -2;
	}
	ret = m_pCServicePis->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCServicePis->TaskStart\n", __FUNCTION__);
		return -3;
	}
	ret = m_pCServicePtu->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCServicePtu->TaskStart\n", __FUNCTION__);
		return -4;
	}
	ret = m_pCServiceSpare->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCServiceSpare->TaskStart\n", __FUNCTION__);
		return -4;
	}
	ret = m_pCServiceHKPis->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCServiceHKPis->TaskStart\n", __FUNCTION__);
		return -4;
	}
	ret = m_pCAnalyseGeoParam->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCAnalyseGeoParam->TaskStart\n", __FUNCTION__);
		return -5;
	}

	//等待磁盘删除
	while (1)
	{
		const std::string disk_root = NSP_STP_CM::str_match_head2sig_first(m_pLocalConf->pictureRootPath, "\\", true);
		bool disk_full = NSP_STP_CM::DiskIsFull(disk_root, m_pLocalConf->diskNeedSpace);
		if (!disk_full)
		{
			break;
		}
		gLog.Add("%s waitting for disk clear\n", __FUNCTION__);
		Sleep(1000);
	}

	for (int i = 0; i < g_cam_num; i ++)
	{
#if MAINTASK_TEST_USE_DAS
		if (i == E_CAM_CLASS_STRUCT)
		{
			m_pCDAS->Start("");
			m_pCDAS->StartDAS();
			continue;
		}
#endif
		ret = m_CAbstractModuleArr[i]->TaskStart();
		if (ret != 0)
		{
			gLog.Add("%s m_pCAbstractModule[%d]->TaskStart\n", __FUNCTION__, i);
			return -6;
		}
	}

	return 0;
}

void CMainTask::TaskStop()
{
	gLog.Add("%s begin\n", __FUNCTION__);
	for (int i = 0; i < g_cam_num; i ++)
	{
#if MAINTASK_TEST_USE_DAS
		if (i == E_CAM_CLASS_STRUCT)
		{
			m_pCDAS->StopDAS();
			m_pCDAS->Destory();
			continue;
		}
#endif
		if(m_CAbstractModuleArr[i] != NULL)
		{
			m_CAbstractModuleArr[i]->ThreadsStop();
			m_CAbstractModuleArr[i]->ReleaseRes();
		}
	}
	m_pCAlarm->ThreadsStop();
	m_pCServicePis->ThreadsStop();
	m_pCServicePtu->ThreadsStop();
	m_pCServiceSpare->ThreadsStop();
	m_pCServiceHKPis->ThreadsStop();
	m_pCMvb->ThreadsStop();
	m_pCAnalyseGeoParam->ThreadsStop();
	gLog.Add("%s end\n", __FUNCTION__);
}

int CMainTask::LoadConfig()
{
	USES_CONVERSION;

	int ret = -1;

	const int SIZE = 1024;
	TCHAR Buffer[SIZE];
	memset(Buffer, 0, sizeof(Buffer));

	CString current_path = theApp.GetCurrentPath();
	CString strIniFile(current_path + _T("\\configPan.ini"));
	CString strDasConf(current_path + _T("\\configBasler.ini"));

	CString strSection;

	strSection.Format(_T("system"));
	m_pLocalConf->tcpPisPort = GetPrivateProfileInt(strSection, _T("tcpPisPort"), -1, strIniFile);
	if (m_pLocalConf->tcpPisPort <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->tcpPtuPort = GetPrivateProfileInt(strSection, _T("tcpPtuPort"), -1, strIniFile);
	if (m_pLocalConf->tcpPtuPort <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->udpPtuPort = GetPrivateProfileInt(strSection, _T("udpPtuPort"), -1, strIniFile);
	if (m_pLocalConf->udpPtuPort <= 0)
	{
		goto LEAVE;
	}

	GetPrivateProfileString(strSection, _T("DataCenterSvrIp"), _T(""), Buffer, SIZE, strIniFile);
	m_pLocalConf->SrvdataCenter.alarm_service.ip = W2A(Buffer);
	m_pLocalConf->SrvdataCenter.rtdata_service.ip = W2A(Buffer);
	if (m_pLocalConf->SrvdataCenter.alarm_service.ip.empty())
	{
		goto LEAVE;
	}

	m_pLocalConf->SrvdataCenter.alarm_service.port = GetPrivateProfileInt(strSection, _T("DataCenterSvrAlarmPort"), -1, strIniFile);
	if (m_pLocalConf->SrvdataCenter.alarm_service.port <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->SrvdataCenter.rtdata_service.port = GetPrivateProfileInt(strSection, _T("DataCenterSvrRtDataPort"), -1, strIniFile);
	if (m_pLocalConf->SrvdataCenter.rtdata_service.port <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->udpGeoRecvPort = GetPrivateProfileInt(strSection, _T("udpGeoRecvPort"), -1, strIniFile);
	if (m_pLocalConf->udpGeoRecvPort <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->udpCurrentPort = GetPrivateProfileInt(strSection, _T("udpCurrentPort"), -1, strIniFile);
	if (m_pLocalConf->udpCurrentPort <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->tcpSparePort = GetPrivateProfileInt(strSection, _T("tcpSparePort"), -1, strIniFile);
	if (m_pLocalConf->tcpSparePort <= 0)
	{
		goto LEAVE;
	}

	GetPrivateProfileString(strSection, _T("SpareSvrIp"), _T(""), Buffer, SIZE, strIniFile);
	m_pLocalConf->SrvSpare.ip = W2A(Buffer);
	if (m_pLocalConf->SrvSpare.ip.empty())
	{
		goto LEAVE;
	}

	m_pLocalConf->SrvSpare.port = GetPrivateProfileInt(strSection, _T("SpareSvrPort"), -1, strIniFile);
	if (m_pLocalConf->SrvSpare.port <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->httpHkPisLocalPort = GetPrivateProfileInt(strSection, _T("httpHkPisLocalPort"), -1, strIniFile);
	if (m_pLocalConf->httpHkPisLocalPort <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->httpHkPisOutPort = GetPrivateProfileInt(strSection, _T("httpHkPisOutPort"), -1, strIniFile);
	if (m_pLocalConf->httpHkPisOutPort <= 0)
	{
		goto LEAVE;
	}

	GetPrivateProfileString(strSection, _T("httpHkPisOutIp"), _T(""), Buffer, SIZE, strIniFile);
	m_pLocalConf->httpHkPisOutIp = W2A(Buffer);
	if (m_pLocalConf->httpHkPisOutIp.empty())
	{
		goto LEAVE;
	}

	GetPrivateProfileString(strSection, _T("videoDownPath"), _T("D:\\pantoMonitorPath\\videoDownPath\\"), Buffer, SIZE, strIniFile);
	m_pLocalConf->videoRootPath = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("videoAlarmPath"), _T("D:\\pantoMonitorPath\\videoAlarmPath\\"), Buffer, SIZE, strIniFile);
	m_pLocalConf->videoRootPathAlarm = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("pictureDownPath"), _T("D:\\pantoMonitorPath\\pictureDownPath\\"), Buffer, SIZE, strIniFile);
	m_pLocalConf->pictureRootPath = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("pictureAlarmPath"), _T("D:\\pantoMonitorPath\\pictureAlarmPath\\"), Buffer, SIZE, strIniFile);
	m_pLocalConf->pictureRootPathAlarm = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("exportInfRootPath"), _T("D:\\pantoMonitorPath\\exportInf\\"), Buffer, SIZE, strIniFile);
	m_pLocalConf->exportInfRootPath = W2A(Buffer);

	m_pLocalConf->mvbComParam.mvbBuadRate = GetPrivateProfileInt(strSection, _T("mvbBuadRate"), 9600, strIniFile);

	GetPrivateProfileString(strSection, _T("mvbSerialPort"), _T(""), Buffer, SIZE, strIniFile);
	m_pLocalConf->mvbComParam.mvbSerialPort = W2A(Buffer);
	if (m_pLocalConf->mvbComParam.mvbSerialPort.empty())
	{
		goto LEAVE;
	}

	m_pLocalConf->scmComParam.scmBuadRate = GetPrivateProfileInt(strSection, _T("scmBuadRate"), 9600, strIniFile);

	GetPrivateProfileString(strSection, _T("scmSerialPort"), _T(""), Buffer, SIZE, strIniFile);
	m_pLocalConf->scmComParam.scmSerialPort = W2A(Buffer);
	if (m_pLocalConf->scmComParam.scmSerialPort.empty())
	{
		goto LEAVE;
	}

	m_pLocalConf->diskNeedSpace = GetPrivateProfileInt(strSection, _T("DiskNeedSpaceG"), -1, strIniFile);
	if (m_pLocalConf->diskNeedSpace <= 0)
	{
		goto LEAVE;
	}

	int fps = GetPrivateProfileInt(strSection, _T("fps"), 10, strIniFile);
	m_pLocalConf->fps = (float)(fps * 1.0);

	m_pLocalConf->arcBeginThreshold = GetPrivateProfileInt(strSection, _T("arcBeginThreshold"), 200, strIniFile);
	m_pLocalConf->arcEndThreshold = GetPrivateProfileInt(strSection, _T("arcEndThreshold"), 100, strIniFile);
	m_pLocalConf->arcDeltTimeMsThreshold = GetPrivateProfileInt(strSection, _T("arcDeltTimeMsThreshold"), 1, strIniFile);
	m_pLocalConf->arcContinuTimeUsThreshold = GetPrivateProfileInt(strSection, _T("arcContinuTimeUsThreshold"), 2, strIniFile);
	m_pLocalConf->arcStrenthXsThreshold = GetPrivateProfileInt(strSection, _T("arcStrenthXsThreshold"), 512000, strIniFile);
	m_pLocalConf->arcTimeLenGainThreshold = GetPrivateProfileInt(strSection, _T("arcTimeLenGainThreshold"), 1000, strIniFile);

	GetPrivateProfileString(strSection, _T("SaveFrameSuffix"), _T(".jpg"), Buffer, SIZE, strIniFile);
	m_pLocalConf->saveFrameSuffix = W2A(Buffer);
	GetPrivateProfileString(strSection, _T("tempMapFrameSuffix"), _T(".tempMap"), Buffer, SIZE, strIniFile);
	m_pLocalConf->tempMapFrameSuffix = W2A(Buffer);
	GetPrivateProfileString(strSection, _T("SaveVideoSuffix"), _T(".mp4"), Buffer, SIZE, strIniFile);
	m_pLocalConf->saveVideoSuffix = W2A(Buffer);

	m_pLocalConf->carriageNum = GetPrivateProfileInt(strSection, _T("carriageNum"), -1, strIniFile);
	if (m_pLocalConf->carriageNum <= 0)
	{
		goto LEAVE;
	}

	if (m_pLocalConf->carriageNum == 2)
	{
		m_pLocalConf->mvbComParam.mpCode = 1;
	}
	else
	{
		m_pLocalConf->mvbComParam.mpCode = 2;
	}

	m_pLocalConf->lineId = GetPrivateProfileInt(strSection, _T("lineId"), 10, strIniFile);
	m_pLocalConf->trainNum = GetPrivateProfileInt(strSection, _T("trainNum"), 10019, strIniFile);

	GetPrivateProfileString(strSection, _T("dbHost"), _T("127.0.0.1"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbHost = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("dbUser"), _T("root"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbUser = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("dbPass"), _T("pass"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbPass = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("dbName"), _T("dbPanMonitor"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbName = W2A(Buffer);

	m_pLocalConf->dbMySqlParam.dbPort = GetPrivateProfileInt(strSection, _T("dbPort"), 3306, strIniFile);

	m_pLocalConf->presetNum = GetPrivateProfileInt(strSection, _T("presetNum"), 8, strIniFile);
	m_pLocalConf->PantoPos = GetPrivateProfileInt(strSection, _T("PantoPos"), 8, strIniFile);

	m_pLocalConf->ratedCurrentThreshold = GetPrivateProfileInt(strSection, _T("ratedCurrent"), -1, strIniFile);
	m_pLocalConf->ratedCurrentThreshold = m_pLocalConf->ratedCurrentThreshold * 30 / 100;
	if (m_pLocalConf->ratedCurrentThreshold <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->arcRateThresholdCls1 = 0.01 * GetPrivateProfileInt(strSection, _T("arcRateCls1"), -1, strIniFile);
	if (m_pLocalConf->arcRateThresholdCls1 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->arcRateThresholdCls2 = 0.01 * GetPrivateProfileInt(strSection, _T("arcRateCls2"), -1, strIniFile);
	if (m_pLocalConf->arcRateThresholdCls2 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->arcRateThresholdCls3 = 0.01 * GetPrivateProfileInt(strSection, _T("arcRateCls3"), -1, strIniFile);
	if (m_pLocalConf->arcRateThresholdCls3 <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->tempThresholdCls1 = GetPrivateProfileInt(strSection, _T("tempCls1"), -1, strIniFile);
	if (m_pLocalConf->tempThresholdCls1 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->tempThresholdCls2 = GetPrivateProfileInt(strSection, _T("tempCls2"), -1, strIniFile);
	if (m_pLocalConf->tempThresholdCls2 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->tempThresholdCls3 = GetPrivateProfileInt(strSection, _T("tempCls3"), -1, strIniFile);
	if (m_pLocalConf->tempThresholdCls3 <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->pullThresholdCls1 = GetPrivateProfileInt(strSection, _T("pullThresholdCls1"), -1, strIniFile);
	if (m_pLocalConf->pullThresholdCls1 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->pullThresholdCls2 = GetPrivateProfileInt(strSection, _T("pullThresholdCls2"), -1, strIniFile);
	if (m_pLocalConf->pullThresholdCls2 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->pullThresholdCls3 = GetPrivateProfileInt(strSection, _T("pullThresholdCls3"), -1, strIniFile);
	if (m_pLocalConf->pullThresholdCls3 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->guidThresholdCls1 = GetPrivateProfileInt(strSection, _T("guidThresholdCls1"), -1, strIniFile);
	if (m_pLocalConf->guidThresholdCls1 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->guidThresholdCls2 = GetPrivateProfileInt(strSection, _T("guidThresholdCls2"), -1, strIniFile);
	if (m_pLocalConf->guidThresholdCls2 <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->guidThresholdCls3 = GetPrivateProfileInt(strSection, _T("guidThresholdCls3"), -1, strIniFile);
	if (m_pLocalConf->guidThresholdCls3 <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->dataCenterTimeoutS = GetPrivateProfileInt(strSection, _T("dataCenterTimeoutS"), 30, strIniFile);
	if (m_pLocalConf->dataCenterTimeoutS <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->cameraTimeoutS = GetPrivateProfileInt(strSection, _T("cameraTimeoutS"), 30, strIniFile);
	if (m_pLocalConf->cameraTimeoutS <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->currentTimeoutS = GetPrivateProfileInt(strSection, _T("currentTimeoutS"), 30, strIniFile);
	if (m_pLocalConf->currentTimeoutS <= 0)
	{
		goto LEAVE;
	}
	m_pLocalConf->geoTimeoutS = GetPrivateProfileInt(strSection, _T("geoTimeoutS"), 30, strIniFile);
	if (m_pLocalConf->geoTimeoutS <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->waitSystemTs = GetPrivateProfileInt(strSection, _T("waitSystemTs"), 30, strIniFile);
	if (m_pLocalConf->waitSystemTs <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->useQuickExit = GetPrivateProfileInt(strSection, _T("useQuickExit"), 0, strIniFile);
	if (m_pLocalConf->useQuickExit <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->useFrameAnalyze = GetPrivateProfileInt(strSection, _T("useFrameAnalyze"), 1, strIniFile);
	m_pLocalConf->videoMaxSec = GetPrivateProfileInt(strSection, _T("videoMaxSec"), 10*60, strIniFile);

	char section[g_cam_num][64];
	strcpy(section[E_CAM_CLASS_MONITOR], "MONITOR");
	strcpy(section[E_CAM_CLASS_MONITOR_SPARE], "MONITOR_SPARE");
	strcpy(section[E_CAM_CLASS_STRUCT], "STRUCT");
	strcpy(section[E_CAM_CLASS_TEMPRATE], "TEMPRATE");
	strcpy(section[E_CAM_CLASS_ARC_NOTIFY], "ARC_NOTIFY");
	strcpy(section[E_CAM_CLASS_ARC_COLLECT], "ARC_COLLECT");

	for (int i = 0; i < g_cam_num; i++)
	{
		strSection = section[i];
		if (i == E_CAM_CLASS_MONITOR || i == E_CAM_CLASS_MONITOR_SPARE)
		{
			VideoNSP::VideoDeviceInfo videoDeviceInfo;
			VideoNSP::VideoDeviceFormatInfo videoDeviceFormatInfo;
			GetPrivateProfileString(strSection, _T("Provider"), _T(""), Buffer, SIZE, strIniFile);//PROVIDER_VIDEO_NEWVISION
			videoDeviceInfo.provider = W2A(Buffer);
			if (videoDeviceInfo.provider.empty())
			{
				goto LEAVE;
			}
			GetPrivateProfileString(strSection, _T("sourceUrl"), _T(""), Buffer, SIZE, strIniFile);//"rtsp://admin:12345@192.168.8.2"
			videoDeviceInfo.sourceUrl = W2A(Buffer);
			if (videoDeviceInfo.sourceUrl.empty())
			{
				goto LEAVE;
			}
			GetPrivateProfileString(strSection, _T("DestIP"), _T(""), Buffer, SIZE, strIniFile);//"192.168.8.2"
			videoDeviceInfo.carrigeIp = W2A(Buffer);
			if (videoDeviceInfo.carrigeIp.empty())
			{
				goto LEAVE;
			}

			m_CAbstractModuleArr[i]->SetDevInf(&videoDeviceInfo, &videoDeviceFormatInfo);
			(dynamic_cast<CMonitor*>(m_CAbstractModuleArr[i]))->SetPresetNum(m_pLocalConf->presetNum);
			(dynamic_cast<CMonitor*>(m_CAbstractModuleArr[i]))->SetPantoPos(m_pLocalConf->PantoPos);
		}
		else
		{
			Cam::CamDeviceInfo camDeviceInfo;
			Cam::CamDeviceFormatInfo camDeviceFormatInfo;

			//相机网络参数
			camDeviceInfo.needInitDevice = false;

			GetPrivateProfileString(strSection, _T("Provider"), _T(""), Buffer, SIZE, strIniFile);
			camDeviceInfo.provider = W2A(Buffer);
			if (camDeviceInfo.provider.empty())
			{
				goto LEAVE;
			}
			GetPrivateProfileString(strSection, _T("FriendlyName"), _T(""), Buffer, SIZE, strIniFile);
			camDeviceInfo.friendlyName = W2A(Buffer);
			if (camDeviceInfo.provider == gCameraBasler)
			{
				if (camDeviceInfo.friendlyName.empty())
				{
					goto LEAVE;
				}
			}
			GetPrivateProfileString(strSection, _T("DestIP"), _T(""), Buffer, SIZE, strIniFile);
			camDeviceInfo.destIP = W2A(Buffer);
			if (camDeviceInfo.destIP.empty())
			{
				goto LEAVE;
			}

			GetPrivateProfileString(strSection, _T("LocalIP"), _T("127.0.0.1"), Buffer, SIZE, strIniFile);
			camDeviceInfo.localIP = W2A(Buffer);

			//相机参数
			camDeviceFormatInfo.width = GetPrivateProfileInt(strSection, _T("Width"), 0, strIniFile);
			if (camDeviceFormatInfo.width <= 0 || camDeviceFormatInfo.width > 1024*32)
			{
				goto LEAVE;
			}
			camDeviceFormatInfo.height = GetPrivateProfileInt(strSection, _T("Height"), 0, strIniFile);
			if (camDeviceFormatInfo.height <= 0 || camDeviceFormatInfo.height > 1024*32)
			{
				goto LEAVE;
			}

			int fps = GetPrivateProfileInt(strSection, _T("fps"), 0, strIniFile);
			camDeviceFormatInfo.fps = (float)(fps * 1.0);

			camDeviceFormatInfo.triggerMode = GetPrivateProfileInt(strSection, _T("triggerMode"), 0, strIniFile);
			camDeviceFormatInfo.triggerType = GetPrivateProfileInt(strSection, _T("triggerType"), 0, strIniFile);
			int use_color = GetPrivateProfileInt(strSection, _T("useColor"), -1, strIniFile);
			if (use_color == 1)
			{
				camDeviceFormatInfo.iStreamType = NSP_BS_CM::PixelFormat_YUV422_YUYV_Packed;
			}
			else if (use_color == 0)
			{
				camDeviceFormatInfo.iStreamType = NSP_BS_CM::PixelFormat_Mono8;
			}

			m_CAbstractModuleArr[i]->m_pCameraOpt->isUseExposeCmd = GetPrivateProfileInt(strSection, _T("isUseExposeCmd"), 0, strIniFile);
			m_CAbstractModuleArr[i]->m_pCameraOpt->isAutoExpose = GetPrivateProfileInt(strSection, _T("isAutoExpose"), 1, strIniFile);
			m_CAbstractModuleArr[i]->m_pCameraOpt->exposeTimeUs = GetPrivateProfileInt(strSection, _T("exposeTimeUs"), 10000, strIniFile);
			m_CAbstractModuleArr[i]->m_pCameraOpt->isUseGainCmd = GetPrivateProfileInt(strSection, _T("isUseGainCmd"), 0, strIniFile);
			m_CAbstractModuleArr[i]->m_pCameraOpt->isAutoGain = GetPrivateProfileInt(strSection, _T("isAutoGain"), 1, strIniFile);
			m_CAbstractModuleArr[i]->m_pCameraOpt->gainVal = GetPrivateProfileInt(strSection, _T("gainVal"), 3, strIniFile);
			m_CAbstractModuleArr[i]->m_pCameraOpt->orgOffX = GetPrivateProfileInt(strSection, _T("orgOffX"), 0, strIniFile);
			m_CAbstractModuleArr[i]->m_pCameraOpt->orgOffY = GetPrivateProfileInt(strSection, _T("orgOffY"), 0, strIniFile);

			if (i == E_CAM_CLASS_STRUCT)
			{
				//camDeviceFormatInfo.x = m_CAbstractModuleArr[i]->m_pCameraOpt->orgOffX;
				//camDeviceFormatInfo.y = m_CAbstractModuleArr[i]->m_pCameraOpt->orgOffY;
			}

			m_CAbstractModuleArr[i]->SetDevInf(&camDeviceInfo, &camDeviceFormatInfo);
		}
	}

#if MAINTASK_TEST_USE_DAS
	Ccfg::GetInstance()->Load(W2A(strDasConf));
	m_pCDAS->Initialize("");
#endif

	ret = 0;
LEAVE:
	return ret;
}

void CMainTask::Test()
{
	USES_CONVERSION;

	return;

	if (0)
	{
		//图像算法库测试
		int ret = 0;
		std::string disk_path = "D:\\WorkSpace\\test\\structure\\";
		const int delt_ms = 200;
		std::vector<std::string> vec_file_name;
		NSP_STP_CM::filetitle_list_in_folder(disk_path, vec_file_name);
		unsigned int file_num = vec_file_name.size();
		for (unsigned int i = 0; i < file_num; i ++)
		{
			std::string file_name = vec_file_name[i];
			std::string file_path = disk_path + file_name + ".jpg";

			{
				int train_station[3];
				train_station[0] = 0;
				train_station[1] = 0;
				train_station[2] = 0;
				tagAnalyzeResArr analyzeResArr;
				
				unsigned char* pData = NULL;
				int width = 0;
				int height = 0;
				int frame_size = 0;
				bool is_load = NSP_STP_CM::LoadImgFrame(file_path, false, pData, width, height, frame_size);
				if (is_load)
				{
					ret = g_pCFrameAnalyzeDll->m_imgdetect_deal(
						pData, 
						frame_size, 
						NSP_STP_CM::E_FRAME_FORMAT_JPEG, 
						file_path.c_str(),
						train_station, 
						&analyzeResArr);
					delete[] pData;

					TRACE("file_path=%s,resNum=%d\n", file_path.c_str(), analyzeResArr.resNum);
					ret = 1;
				}
				
			}

			Sleep(delt_ms);
		}
		ret = 1;
		return;
	}

	if (0)
	{
		const bool is_rm_old_export = true;
		if (is_rm_old_export)
		{
			std::string export_path = "E:\\pantoMonitorPath\\exportInf\\";
			std::vector<M_STRING> vec_folder_path;
			NSP_STP_CM::folderpath_list_in_folder(export_path, vec_folder_path);
			MU_INT32 folder_num = vec_folder_path.size();
			if (folder_num > 4)
			{
				NSP_STP_CM::folder_oldest_delete(export_path);
			}
		}
	}

	if (0)
	{
		char arc_val[4*1024];
		int tmp_len = 0;
		for (int i = 0; i < 255; i ++)
		{
			tmp_len += sprintf(arc_val + tmp_len, "%02x&", i);
		}
		for (int i = 0; i < 255; i ++)
		{
			tmp_len += sprintf(arc_val + tmp_len, "%02x&", 255-i);
		}
		tmp_len = 0;
	}

	if (0)
	{
		int ret = 0;
		const int width = 384;
		const int height = 288;
		const int temp_pos_hign_x = width * 70 / 100;
		const int temp_pos_hign_y = height * 70 / 100;
		const int left_up_x = temp_pos_hign_x;
		const int left_up_y = temp_pos_hign_y;
		std::string file_path = "E:\\123.jpg";
		ret = NSP_STP_CM::ImgRectAdd(file_path, left_up_x, left_up_y, width, height);
		ret = NSP_STP_CM::ImgTextAdd(file_path, left_up_x, left_up_y, width, height, "-123.456");
		ret = 1;
	}

	if (0)
	{
		//查询
		int ret = 0;
		char sql_cmd[1024];
		NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

		const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = &m_pLocalConf->dbMySqlParam;
		p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
			p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

		const std::string c_file_dst = "E:\\f_record_info.txt";
		const std::string c_file_rt = "E:\\f_record_info_rt.txt";

		ret = remove(c_file_rt.c_str());

		sprintf(sql_cmd, 
			"select * from t_alarm_info_copy into outfile '%s' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' LINES STARTING BY '>' TERMINATED BY '\r\n';",
			c_file_rt.c_str()
			);
		ret = p_CMySqlDB->WriteDataBase(sql_cmd);
		if (ret != 0)
		{
			gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		}
		else
		{
			ret = NSP_STP_CM::copyFile(c_file_rt.c_str(), c_file_dst.c_str(), false);
			ret = 1;
		}
	}


	if (0)
	{
		//实时数据模拟
		int ret = 0;
		char sql_cmd[1024];
		NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

		const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = &m_pLocalConf->dbMySqlParam;
		p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
			p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

		st_rt_data rt_data;
		NSP_STP_CM::st_time tim_n;
		time_t n_now_time_s = time(NULL);
		char time_inf[256];

		const int c_slq_count = 317440 * 13;
		for (int i = 0; i < c_slq_count; i ++)
		{
			if (i % 2 == 0)
			{
				n_now_time_s --;
			}
			else
			{
				n_now_time_s += 3;
			}

			GetTimeByStamp(n_now_time_s, &tim_n);
			sprintf(time_inf, "%04d-%02d-%02d %02d:%02d:%02d", tim_n.year, tim_n.mon, tim_n.day, tim_n.hour, tim_n.min, tim_n.sec);
			rt_data.BeginTime = time_inf;
			//插入数据
			sprintf(sql_cmd, "insert into  %s  (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s) \
							 values (%d, %d, %d, %d, %d, %d, '%s', '%s', %d, %d);",
							 gTblRt.c_str(), 
							 gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(), gRtFLineId.c_str(), 
							 gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), gRtFBeginTime.c_str(), 
							 gRtFBeginTimeMs.c_str(), gRtFLastTime.c_str(),
							 rt_data.CarriageId, rt_data.CarriageNum, rt_data.StationInd , rt_data.LineId, 
							 rt_data.Direction, rt_data.DataType, rt_data.RtData.c_str(), rt_data.BeginTime.c_str(), 
							 rt_data.BeginTimeMs, rt_data.LastTimeMs                   
							 );
			ret = p_CMySqlDB->WriteDataBase(sql_cmd);
			if (ret != 0)
			{
				gLog.Add("%s WriteDataBase %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
			}
		}

		ret = 1;
		exit(0);
	}

	if (0)
	{
		//查询
		int ret = 0;
		char sql_cmd[1024];
		char sql_cmd1[1024];
		NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

		const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = &m_pLocalConf->dbMySqlParam;
		p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
			p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);

		unsigned long long row_count = 0;
		unsigned int col_count = 0;
		char*** ppp_res = NULL;

		sprintf(sql_cmd, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s > '%s' and %s = 0 ORDER BY %s LIMIT 1024;", 
			gRtFKey.c_str(), gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(),
			gRtFLineId.c_str(), gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), 
			gRtFBeginTime.c_str(), gRtFBeginTimeMs.c_str(), gRtFLastTime.c_str(), 
			gTblRt.c_str(),
			gRtFBeginTime.c_str(), "2017-10-13 23:51:01",
			gRtFSendFlag.c_str(),
			gRtFBeginTime.c_str()
			);

		sprintf(sql_cmd1, "select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s > '%s' and %s = 0 LIMIT 1024;", 
			gRtFKey.c_str(), gRtFCarriageId.c_str(), gRtFCarriageNum.c_str(), gRtFStationInd.c_str(),
			gRtFLineId.c_str(), gRtFDirection.c_str(), gRtFDataType.c_str(), gRtFData.c_str(), 
			gRtFBeginTime.c_str(), gRtFBeginTimeMs.c_str(), gRtFLastTime.c_str(), 
			gTblRt.c_str(),
			gRtFBeginTime.c_str(), "2017-10-13 23:51:01",
			gRtFSendFlag.c_str()
			);

		gLog.Add("1\n");
		ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
		gLog.Add("2\n");
		ret = p_CMySqlDB->WriteAndReadDb(sql_cmd1, row_count, col_count, &ppp_res);
		gLog.Add("3\n");
		exit(0);
	}

	return;
}

void CMainTask::TaskSetPause( bool is_pause )
{
	for (int i = 0; i < g_cam_num; i ++)
	{
		m_CAbstractModuleArr[i]->SetThreadPause(is_pause);
	}
}

CAbstractCamera* CMainTask::GetCamera( int camera_ind )
{
	if (camera_ind > 0 && camera_ind < g_cam_num)
	{
		return m_CAbstractModuleArr[camera_ind];
	}
	return NULL;
}

void CMainTask::QuickStop()
{
	gLog.Add("%s begin\n", __FUNCTION__);

	for (int i = 0; i < g_cam_num; i ++)
	{
#if MAINTASK_TEST_USE_DAS
		if (i == E_CAM_CLASS_STRUCT)
		{
			m_pCDAS->StopDAS();
			m_pCDAS->Destory();
			continue;
		}
#endif
		if (i == E_CAM_CLASS_MONITOR || i == E_CAM_CLASS_MONITOR_SPARE)
		{
			continue;
		}
		if(m_CAbstractModuleArr[i] != NULL)
		{
			m_CAbstractModuleArr[i]->ThreadsStop();
		}
	}

	delete g_pCCamPictureDll;
	Sleep(2000);

	gLog.Add("%s end\n", __FUNCTION__);

#ifdef _DEBUG
	gLog.Add("%s Program Exit %s Debug\n", __FUNCTION__, gVersion.c_str());
#else
	gLog.Add("%s Program Exit %s Release\n", __FUNCTION__, gVersion.c_str());
#endif
}

