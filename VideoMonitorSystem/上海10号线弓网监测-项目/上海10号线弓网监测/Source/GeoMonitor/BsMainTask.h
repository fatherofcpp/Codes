#pragma once

#include "BsDefStruct.h"

class CAbstractCamera;
class CAlarm;
class CGeoSender;

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

	void QuickStop();
private:
	int LoadConfig();
	void Test();
private:
	CAbstractCamera* m_CAbstractModuleArr[E_CAM_CLASS_TATOL];
	CGeoSender* m_pCGeoSender;
	CAlarm* m_pCAlarm;
	tag_st_local_conf* m_pLocalConf;
};

