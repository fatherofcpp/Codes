#pragma once

#include "BsDefStruct.h"

class CAbstractCamera;
class CGeoParamAnalyse;
class CAlarm;
class CServicePis;
class CServicePtu;
class CServiceSpare;
class CServiceHKPis;
class CMvb;

namespace HXZY
{
	namespace TEDS
	{
		class CDAS;
	}
}

class CMainTask
{
public:
	CMainTask(void);
	~CMainTask(void);

public:
	int TaskStart();
	void TaskStop();

	void TaskSetPause(bool is_pause);
	CAbstractCamera* GetCamera(int camera_ind);
	CGeoParamAnalyse* GetGeoParamAnalyse(){return m_pCAnalyseGeoParam;}
	tag_st_local_conf* GetLocalConf(){return m_pLocalConf;}
	void QuickStop();
private:
	int LoadConfig();
	void Test();
private:
	CAbstractCamera* m_CAbstractModuleArr[E_CAM_CLASS_TATOL];
	CGeoParamAnalyse* m_pCAnalyseGeoParam;
	CAlarm* m_pCAlarm;
	CServicePis* m_pCServicePis;
	CServicePtu* m_pCServicePtu;
	CServiceSpare* m_pCServiceSpare;
	CServiceHKPis* m_pCServiceHKPis;
	CMvb* m_pCMvb;
	tag_st_local_conf* m_pLocalConf;
	HXZY::TEDS::CDAS* m_pCDAS;
};

