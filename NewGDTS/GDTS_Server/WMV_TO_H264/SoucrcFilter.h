#include "stdafx.h"
#pragma once
#import "Svl_COMObject.tlb"
typedef Svl_COMObject::Svl_InterfacePtr SvlInterfacePtr;
typedef Svl_COMObject::Svl_Class SvlClass;

#include "xml\include\mty.h"

typedef struct
{
//	CString s_CamID;
	MS_INT32 i_CamID;
	CString s_AS_Server;
}CamInfo;
class CSoucrcFilter
{
public:
	CSoucrcFilter(void);
	~CSoucrcFilter(void);
	CamInfo a_sr_CamInfo[20];
	SvlInterfacePtr p_SvlCom;
public:
	M_BOOL Init(void);
	M_BOOL Login(CString s_IP, MS_INT32 i_Port, CString s_Name, CString s_Pwd);
	M_BOOL GetAlive(void);
protected:
	
	CString s_Server_Name;
	CString s_Server_Pwd;
	CString s_Server_IP;
	MS_INT32 i_Server_Port;
	MS_INT32 i_Cam_ID;
	CString s_Cam_ID;
	M_BOOL b_LogFlag;

public:
	M_BOOL ReQuestStream(MS_INT32 i_Cam , MS_INT32 i_ch,MS_PINT li_CallBack,MU_INT32 ui_User);
	M_BOOL ReQuestStreamEX(MS_INT32 i_Cam , MS_INT32 i_ch,MS_PINT li_CallBack,MU_INT32 ui_User);
	M_BOOL StopStream(MS_INT32 i_Cam , MS_INT32 i_ch,MS_PINT li_CallBack,MU_INT32 ui_User);
	M_BOOL SetCallBack(MS_PINT li_CallBack);
	M_BOOL Logoff(void);
	M_BOOL FindCam(void);
};
