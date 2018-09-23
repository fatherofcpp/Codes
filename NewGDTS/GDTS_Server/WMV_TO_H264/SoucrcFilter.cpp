#include "StdAfx.h"
#include "SoucrcFilter.h"
#include <shlwapi.h>  //StrToInt函数的转换

static SvlInterfacePtr s_SvlCom;
void initH3cVMCOM()
{
	CoInitialize(NULL);//初始化COM

	struct _GUID tian = __uuidof(SvlClass);
	try{
		s_SvlCom = SvlInterfacePtr(tian);
	}catch(_com_error &e)
	{
		m_Log.Add("new SvlInterfacePtr fail");
		e.Description();
		TCHAR * ptr = (TCHAR *)e.ErrorMessage();
		TRACE("error%d:%s",0,ptr);
		return ;
	}

}
CSoucrcFilter::CSoucrcFilter(void)
{
	b_LogFlag = FALSE;

}

CSoucrcFilter::~CSoucrcFilter(void)
{
}

/********************************************************
*函数功能：初始化SDK COM接口
*作者：张军
*编写时间：2010-02-21
*最后修改时间： 
*返回 TRUE为成功 FALSE为失败
********************************************************/
M_BOOL CSoucrcFilter::Init(void)
{
 //	CoInitialize(NULL);//初始化COM

	//struct _GUID tian = __uuidof(SvlClass);
	//try{
	//	p_SvlCom = SvlInterfacePtr(tian);
	//}catch(_com_error &e)
	//{
	//	m_Log.Add("new SvlInterfacePtr fail");
	//	e.Description();
	//	TCHAR * ptr = (TCHAR *)e.ErrorMessage();
	//	TRACE("error%d:%s",0,ptr);
	//	return -1;
	//}

	p_SvlCom = s_SvlCom;

	if(NULL == p_SvlCom)
	{
		return FALSE;
	}

	MS_INT32 i_ExitCode = p_SvlCom->InitSdk();
	if(0 != i_ExitCode)
	{
		return FALSE;
	}
	
	return TRUE;
}

/********************************************************
*函数功能：检测
*作者：张军
*编写时间：2010-02-21
*最后修改时间： 
*返回 TRUE为成功 FALSE为失败
********************************************************/
M_BOOL CSoucrcFilter::GetAlive(void)
{
 	if(b_LogFlag==TRUE)
		return TRUE;
	else
		return FALSE;
}

/********************************************************
*函数功能：通过COM接口登陆VM8000/VM5000服务器
*作者：张军
*编写时间：2010-02-21
*最后修改时间： 
*s_IP为VM服务器IP地址
*i_Port为VM服务器登陆端口
*s_Name为VM服务器用户名
*s_Pwd为VM服务器密码
*返回 TRUE为成功 FALSE为失败
********************************************************/
M_BOOL CSoucrcFilter::Login(CString s_IP, int i_Port, CString s_Name, CString s_Pwd)
{
	MS_INT32 i_ret = p_SvlCom->H3C_Logon(s_IP.GetString(), i_Port,s_Name.GetString(), s_Pwd.GetString());
	//MS_INT32 i_ret = p_SvlCom->H3C_Logon("192.168.0.133", 12000,"t12345", "t12345");
	if(0 != i_ret)
	{
		return FALSE;
	}

	s_Server_IP = s_IP;
	i_Server_Port = i_Port;
	s_Server_Name = s_Name;
	s_Server_Pwd = s_Pwd;
	i_ret =p_SvlCom->H3C_InitializePlayer();
	if(0 != i_ret)
	{
		return FALSE;
	}
	//i_ret =p_SvlCom->H3C_SetMPConfig(VARIANT_TRUE,VARIANT_FALSE);
	i_ret =p_SvlCom->H3C_SetMPConfig(VARIANT_FALSE,VARIANT_FALSE);
	b_LogFlag=TRUE;
	return TRUE;
}

/********************************************************
*函数功能：设置回调并开始拉流
*作者：张军
*编写时间：2010-02-21
*最后修改时间： 
*i_Cam为摄像机序号
*i_ch为通道号
*li_CallBack为数据回调函数
*s_Cam为摄像机ID
*当s_Cam不为空时使用s_Cam做为摄像机ID
*否则使用i_Cam
*返回 TRUE为成功 FALSE为失败
********************************************************/
M_BOOL CSoucrcFilter::ReQuestStream(MS_INT32 i_Cam , MS_INT32 i_ch,MS_PINT li_CallBack,MU_INT32 ui_User)
{
	//if ("" == s_Cam)
	{
		p_SvlCom->H3C_PlayStop(i_ch);
		M_BOOL ret = p_SvlCom->H3C_PlayLive(i_ch,/*a_sr_CamInfo[i_Cam].i_CamID*/i_Cam, 0);
		if(0 !=  ret)
		{
			return FALSE;
		}
		//MS_INT32 i_CallBackType = 0x00010001;//回调媒体流并终止接下去的解码显示
		MS_INT32 i_CallBackType=0x00000008;
		ret = p_SvlCom->H3C_SetDataCallBack(i_ch, (long)li_CallBack, (long)ui_User, i_CallBackType);
		if(0 !=  ret)
		{
			return FALSE;
		}
	}
	return TRUE;
}

/********************************************************
*函数功能：停止回调并停止拉流
*作者：赵号
*编写时间：2014-5-13
*最后修改时间： 
*i_Cam为摄像机序号
*i_ch为通道号
*li_CallBack为数据回调函数
*s_Cam为摄像机ID
*当s_Cam不为空时使用s_Cam做为摄像机ID
*否则使用i_Cam
*返回 TRUE为成功 FALSE为失败
********************************************************/
M_BOOL CSoucrcFilter::StopStream(MS_INT32 i_Cam , MS_INT32 i_ch,MS_PINT li_CallBack,MU_INT32 ui_User)
{

	p_SvlCom->H3C_SetDataCallBack(i_ch, 0, 0, 0);
	M_BOOL ret = p_SvlCom->H3C_PlayStop(i_ch);
	if(0 !=  ret)
	{
		return FALSE;
	}

	return TRUE;
}

/********************************************************
*函数功能：设置回调并开始拉流,只获取H264数据流
*作者：张军
*编写时间：2010-02-21
*修改时间：2014-04-28
*修改人员：zhh
*i_Cam为摄像机序号
*i_ch为通道号
*li_CallBack为数据回调函数
*s_Cam为摄像机ID
*当s_Cam不为空时使用s_Cam做为摄像机ID
*否则使用i_Cam
*返回 TRUE为成功 FALSE为失败
********************************************************/
M_BOOL CSoucrcFilter::ReQuestStreamEX(MS_INT32 i_Cam , MS_INT32 i_ch,MS_PINT li_CallBack,MU_INT32 ui_User)
{
	//if ("" == s_Cam)
	{ 
		p_SvlCom->H3C_PlayStop(i_ch);
		M_BOOL ret = p_SvlCom->H3C_PlayLive(i_ch,/*a_sr_CamInfo[i_Cam].i_CamID*/i_Cam, 0);
		if(0 !=  ret)
		{
			return FALSE;
		}
		//MS_INT32 i_CallBackType = 0x00010001;//回调媒体流并终止接下去的解码显示
		MS_INT32 i_CallBackType=0x00000002;//|解码前的视频
		ret = p_SvlCom->H3C_SetDataCallBack(i_ch, (long)li_CallBack, (long)ui_User, i_CallBackType);
		if(0 !=  ret)
		{
		//	MessageBox(NULL,"yun3","cuowu",MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}

M_BOOL CSoucrcFilter::Logoff(void)
{
//	p_SvlCom->H3C_Logoff();
	return TRUE;
}

M_BOOL CSoucrcFilter::FindCam(void)
{
	return TRUE;
	unsigned long iNum = 0;
	int ret =  p_SvlCom->H3C_GetCameraInit(&iNum);
	if(0 != ret)
	{
		return FALSE;
	}

	Svl_COMObject::CAMERA_INFO tempInfo;
	CString strCameraID;
	CString strAsserverID;
	CString strCameraInfo;
	int i_Count=0;
// 	CFile m_file;
// 	m_file.Open("c:\\CamID.txt",CFile::modeCreate|CFile::modeReadWrite,NULL);
// 	while(p_SvlCom->H3C_GetCameraNext(&tempInfo))
// 	{
// 		CString str;
// 		str.Format("%d__________%s______%s_______%s\r\n",tempInfo.Id,tempInfo.name,tempInfo.EcPortId);
// 		m_file.Write(str,str.GetLength());
// 	}
// 	m_file.Close();
// 	for (int i=0;i<20;i++)
// 	{
// 		if (p_SvlCom->H3C_GetCameraNext(&tempInfo))
// 		{
// 			//a_sr_CamInfo[i].s_CamID.Format("%d",tempInfo.Id);
// 			a_sr_CamInfo[i].i_CamID=tempInfo.Id;
// 			//a_sr_CamInfo[i].s_AS_Server=tempInfo.BelongedAsId;
// 			i_Count++;
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 		
// 	}
// 	if (20>i_Count)
// 	{
// 		iNum=0;
// 		p_SvlCom->H3C_GetLowerCameraInit(&iNum);
// 		if(0 != ret)
// 		{
// 			return FALSE;
// 		}
// 		Svl_COMObject::LOWER_CAMERA_INFO tempInfo;
// 		CString strCameraID;
// 		CString strAsserverID;
// 		CString strCameraInfo;
// 
// 		while (m_pSvlCom->H3C_GetNextLowerCamera(&tempInfo))
// 		{		
// 			strCameraID.Format(L"%d", tempInfo.Id);
// 			strAsserverID = tempInfo.BelongedAsId;
// 			strCameraInfo = strCameraID + "@" + strAsserverID;
// 			m_cCameraEx.AddString(strCameraInfo);
// 		}
// 	}
		
	
	
	return TRUE;
}
