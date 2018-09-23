#include "StdAfx.h"
#include "BsMainTask.h"

#include "Log/CmLog.h"

#include "BsAbstractCamera.h"
#include "BsAlarm.h"

#include "BsGeoColtor.h"
#include "BsGeoSender.h"

#include "CmFunction.h"
#include "BsDllCamPicture.h"
#include "GeoMonitor.h"

#include "DataBase/CmDBMySql.h"
#include "Mvb/ServerTrans.h"
#include "BsGlobal.h"
#include "CamPicture/CameraObject.h"
#include "CamVideo/VideoObjectDevSer.h"
#include "BsDefStruct.h"
#include "BsFunction.h"

#include "Socket/CmTransUdp.h"

/////////////
#define USE_MAINTASK_TEST 0
/////////////

static const int g_cam_num = E_CAM_CLASS_TATOL;

CMainTask::CMainTask(void)
{
	USES_CONVERSION;

	int ret = 0;

	//new
	m_pLocalConf = new tag_st_local_conf;
	m_CAbstractModuleArr[E_CAM_CLASS_ARC_GEOUP] = (CAbstractCamera*)new CGeoColtor;
	m_CAbstractModuleArr[E_CAM_CLASS_ARC_GEODOWNLEFT] = (CAbstractCamera*)new CGeoColtor;
	m_CAbstractModuleArr[E_CAM_CLASS_ARC_GEODOWNRIGHT] = (CAbstractCamera*)new CGeoColtor;
	m_pCGeoSender = new CGeoSender;
	m_pCAlarm = new CAlarm;

	//log init
	CString current_path = theApp.GetCurrentPath();
	current_path += "\\geo_log.log";
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

	//net init
	ret = NSP_STP_CM::SocketInit();
	if(ret != 0)
	{
		gLog.Add("%s SocketInit\n", __FUNCTION__);
		exit(0);
	}


	g_pCCamPictureDll = new CCamPictureDll;
	ret = g_pCCamPictureDll->LoadDll();
	if (ret != 0)
	{
		gLog.Add("%s gCCamPictureDll.LoadDll\n", __FUNCTION__);
		exit(0);
	}

	//config
	ret = LoadConfig();
	if (ret != 0)
	{
		gLog.Add("%s LoadConfig,ret=%d\n", __FUNCTION__, ret);
		exit(0);
	}


	m_pCGeoSender->SetGeoCamera(dynamic_cast<CGeoColtor*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_GEOUP]),
		dynamic_cast<CGeoColtor*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_GEODOWNLEFT]),
		dynamic_cast<CGeoColtor*>(m_CAbstractModuleArr[E_CAM_CLASS_ARC_GEODOWNRIGHT])
		);
	m_pCGeoSender->SetGeoPort(m_pLocalConf->udpGeoSendPort);
	m_pCGeoSender->SetGeoPortDown(m_pLocalConf->udpGeoSendPortDown);
	
	m_pCAlarm->SetPictureRootPath(m_pLocalConf->pictureRootPath);
	m_pCAlarm->SetFrameSuffix(m_pLocalConf->saveFrameSuffix);
	m_pCAlarm->SetDiskNeedSpace(m_pLocalConf->diskNeedSpace);
	m_pCAlarm->SetDbMySqlParam(&m_pLocalConf->dbMySqlParam);


	for (int i = 0; i < g_cam_num; i ++)
	{
		m_CAbstractModuleArr[i]->m_saveFrameSuffix = m_pLocalConf->saveFrameSuffix;
		m_CAbstractModuleArr[i]->m_saveVideoSuffix = m_pLocalConf->saveVideoSuffix;
		m_CAbstractModuleArr[i]->m_diskNeedSpace = m_pLocalConf->diskNeedSpace;
		m_CAbstractModuleArr[i]->m_fps = m_pLocalConf->fps;
		m_CAbstractModuleArr[i]->m_jgqPreReadyTms = m_pLocalConf->jgqPreReadyTms;
		m_CAbstractModuleArr[i]->m_carriageNum = m_pLocalConf->carriageNum;
		m_CAbstractModuleArr[i]->m_lineId = m_pLocalConf->lineId;

		m_CAbstractModuleArr[i]->SetCameraInd(i);
		m_CAbstractModuleArr[i]->m_pCAlarm = m_pCAlarm;
		m_CAbstractModuleArr[i]->m_pDbMysqlParam = &m_pLocalConf->dbMySqlParam;

		{
			m_CAbstractModuleArr[i]->m_rootPath = m_pLocalConf->pictureRootPath;
			m_CAbstractModuleArr[i]->m_rootPathAlarm = m_pLocalConf->pictureRootPathAlarm;
		}
	}

	//init db
	//create db
	NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;
	p_CMySqlDB->SetMySqlServerInf(m_pLocalConf->dbMySqlParam.dbHost.c_str(), m_pLocalConf->dbMySqlParam.dbUser.c_str(), 
		m_pLocalConf->dbMySqlParam.dbPass.c_str(), m_pLocalConf->dbMySqlParam.dbName.c_str(), m_pLocalConf->dbMySqlParam.dbPort);

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

	//create table
	ret = p_CMySqlDB->ConnectDataBase();
	if (ret != 0)
	{
		delete p_CMySqlDB;
		gLog.Add("%s ConnectDataBase,ret=%d\n", __FUNCTION__, ret);
		exit(0);
	}

	char sql_cmd[1024];
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
		"%s tinyint(1) DEFAULT '0'"
		")", gTblAlarm.c_str(),
		gAlarmFKey.c_str(), gAlarmFCameraPos.c_str(),
		gAlarmFCarriageId.c_str(), gAlarmFCarriageNum.c_str(), gAlarmFStationId.c_str(), gAlarmFLineId.c_str(), 
		gAlarmFDirection.c_str(), gAlarmFFileName.c_str(), gAlarmFFullName.c_str(), gAlarmFBeginTime.c_str(), 
		gAlarmFAlarmType.c_str(), gAlarmFAlarmLevel.c_str(), gAlarmFAlarmMessage.c_str(), 
		gAlarmFVideoFileName.c_str(), gAlarmFVideoFullName.c_str(), gAlarmFAlarmValue.c_str(), gAlarmFSendFlag.c_str()
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

	delete g_pCCamPictureDll;
	//delete g_pCCamVideoDll;

	for (int i = 0; i < g_cam_num; i ++)
	{
		delete m_CAbstractModuleArr[i];
		m_CAbstractModuleArr[i] = NULL;
	}

	delete m_pCGeoSender;
	m_pCGeoSender = NULL;

	delete m_pCAlarm;
	m_pCAlarm = NULL;

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

#if USE_MAINTASK_TEST
	ret = m_CAbstractModuleArr[E_CAM_CLASS_ARC_GEOUP]->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_CAbstractModuleArr->TaskStart\n", __FUNCTION__);
		return -1;
	}

	ret = m_pCGeoSender->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCGeoSender->TaskStart\n", __FUNCTION__);
		return -1;
	}

	return 0;
#endif

	//wait system
	Sleep(1000 * m_pLocalConf->waitSystemTs);

	ret = m_pCAlarm->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCAlarm->TaskStart\n", __FUNCTION__);
		return -1;
	}

	ret = m_pCGeoSender->TaskStart();
	if (ret != 0)
	{
		gLog.Add("%s m_pCGeoSender->TaskStart\n", __FUNCTION__);
		return -1;
	}

	for (int i = 0; i < g_cam_num; i ++)
	{
		ret = m_CAbstractModuleArr[i]->TaskStart();
		if (ret != 0)
		{
			gLog.Add("%s m_pCAbstractModule[%d]->TaskStart\n", __FUNCTION__, i);
			return -1;
		}
	}

	return 0;
}

void CMainTask::TaskStop()
{
	gLog.Add("%s begin\n", __FUNCTION__);
	for (int i = 0; i < g_cam_num; i ++)
	{
		if(m_CAbstractModuleArr[i] != NULL)
		{
			m_CAbstractModuleArr[i]->ThreadsStop();
			m_CAbstractModuleArr[i]->ReleaseRes();
		}
	}
	m_pCAlarm->ThreadsStop();
	m_pCGeoSender->ThreadsStop();
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
	CString strIniFile(current_path + _T("\\configGeo.ini"));

	CString strSection;

	strSection.Format(_T("system"));
	
	
	m_pLocalConf->udpGeoSendPort = GetPrivateProfileInt(strSection, _T("udpGeoSendPort"), -1, strIniFile);
	if (m_pLocalConf->udpGeoSendPort <= 0)
	{
		goto LEAVE;
	}

	m_pLocalConf->udpGeoSendPortDown = GetPrivateProfileInt(strSection, _T("udpGeoSendPortDown"), -1, strIniFile);
	if (m_pLocalConf->udpGeoSendPortDown <= 0)
	{
		goto LEAVE;
	}


	GetPrivateProfileString(strSection, _T("pictureDownPath"), _T("D:\\pictureDownPath\\"), Buffer, SIZE, strIniFile);
	m_pLocalConf->pictureRootPath = W2A(Buffer);

	m_pLocalConf->diskNeedSpace = GetPrivateProfileInt(strSection, _T("DiskNeedSpaceG"), -1, strIniFile);
	if (m_pLocalConf->diskNeedSpace <= 0)
	{
		goto LEAVE;
	}

	int fps = GetPrivateProfileInt(strSection, _T("fps"), 10, strIniFile);
	m_pLocalConf->fps = (float)(fps * 1.0);

	m_pLocalConf->jgqPreReadyTms = GetPrivateProfileInt(strSection, _T("jgqPreReadyTms"), 10, strIniFile);

	m_pLocalConf->waitSystemTs = GetPrivateProfileInt(strSection, _T("waitSystemTs"), 30, strIniFile);
	if (m_pLocalConf->waitSystemTs <= 0)
	{
		goto LEAVE;
	}

	GetPrivateProfileString(strSection, _T("SaveFrameSuffix"), _T(".jpg"), Buffer, SIZE, strIniFile);
	m_pLocalConf->saveFrameSuffix = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("dbHost"), _T("127.0.0.1"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbHost = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("dbUser"), _T("root"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbUser = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("dbPass"), _T("pass"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbPass = W2A(Buffer);

	GetPrivateProfileString(strSection, _T("dbName"), _T("dbGeoCollector"), Buffer, SIZE, strIniFile);
	m_pLocalConf->dbMySqlParam.dbName = W2A(Buffer);

	m_pLocalConf->dbMySqlParam.dbPort = GetPrivateProfileInt(strSection, _T("dbPort"), 3306, strIniFile);

	char section[g_cam_num][64];
	strcpy(section[E_CAM_CLASS_ARC_GEOUP], "GEOUP");
	strcpy(section[E_CAM_CLASS_ARC_GEODOWNLEFT], "GEODOWNLEFT");
	strcpy(section[E_CAM_CLASS_ARC_GEODOWNRIGHT], "GEODOWNRIGHT");

	for (int i = 0; i < g_cam_num; i++)
	{
		strSection = section[i];
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
				camDeviceFormatInfo.iStreamType = 7;
			}
			else if (use_color == 0)
			{
				camDeviceFormatInfo.iStreamType = 0;
			}

			m_CAbstractModuleArr[i]->SetDevInf(&camDeviceInfo, &camDeviceFormatInfo);
		}
	}



	ret = 0;
LEAVE:
	return ret;
}

void CMainTask::Test()
{
	USES_CONVERSION;

	if(0)
	{
		NSP_STP_CM::CUdp* p_CUdp = new NSP_STP_CM::CUdp;
		p_CUdp->SetServerPort(1234);
		p_CUdp->SetServerIp("127.0.0.1");
		char send_msg[256];
		std::string file_down_left = "D:\\file_down_left";
		std::string file_down_right = "D:\\file_down_right";
		std::string file_up = "D:\\file_up";
		sprintf(send_msg, "{JH}&%s&%s&%s", file_down_left.c_str(), file_down_right.c_str(), file_up.c_str());
		const int c_send_len = strlen(send_msg);
		int real_send_len = p_CUdp->Send2Service(send_msg, c_send_len);
		real_send_len = 0;
	}

	return;

	if (0)
	{
		int ret = 0;
		char sql_cmd[1024];
		NSP_STP_CM::CMySqlDB* p_CMySqlDB = new NSP_STP_CM::CMySqlDB;

		const NSP_STP_CM::tag_st_dbmysql_param* p_DbMysqlParam = &m_pLocalConf->dbMySqlParam;
		p_CMySqlDB->SetMySqlServerInf(p_DbMysqlParam->dbHost.c_str(), p_DbMysqlParam->dbUser.c_str(), 
			p_DbMysqlParam->dbPass.c_str(), p_DbMysqlParam->dbName.c_str(), p_DbMysqlParam->dbPort);
		st_db_record db_record;

		sprintf(sql_cmd, "select %s from %s where  %s = '%s' and %s > %s LIMIT 1;", 
			gAlarmFKey.c_str(),
			gTblAlarm.c_str(), 
			gAlarmFFullName.c_str(), "D:\\\\1.jep",
			gAlarmFBeginTime.c_str(), gDbVideoPicTimeMaxDayAgo.c_str()
			);
		long long alarm_id = 0;
		unsigned long long row_count = 0;
		unsigned int col_count = 0;
		char*** ppp_res = NULL;
		ret = p_CMySqlDB->WriteAndReadDb(sql_cmd, row_count, col_count, &ppp_res);
		if (ret == 0)
		{
			NSP_STP_CM::str_to_int(ppp_res[0][0], alarm_id);

			p_CMySqlDB->ReleaseReadData(row_count, col_count, &ppp_res);
		}
		else
		{
			gLog.Add("%s WriteAndReadDb %s,ret=%d\n", __FUNCTION__, sql_cmd, ret);
		}
		ret = 1;
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
#ifdef _DEBUG
	gLog.Add("%s Program Exit %s Debug\n", __FUNCTION__, gVersion.c_str());
#else
	gLog.Add("%s Program Exit %s Release\n", __FUNCTION__, gVersion.c_str());
#endif
	return;
}

