#include "StdAfx.h"
#include "SoucrcFilter.h"
#include <shlwapi.h>  //StrToInt������ת��

static SvlInterfacePtr s_SvlCom;
void initH3cVMCOM()
{
	CoInitialize(NULL);//��ʼ��COM

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
*�������ܣ���ʼ��SDK COM�ӿ�
*���ߣ��ž�
*��дʱ�䣺2010-02-21
*����޸�ʱ�䣺 
*���� TRUEΪ�ɹ� FALSEΪʧ��
********************************************************/
M_BOOL CSoucrcFilter::Init(void)
{
 //	CoInitialize(NULL);//��ʼ��COM

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
*�������ܣ����
*���ߣ��ž�
*��дʱ�䣺2010-02-21
*����޸�ʱ�䣺 
*���� TRUEΪ�ɹ� FALSEΪʧ��
********************************************************/
M_BOOL CSoucrcFilter::GetAlive(void)
{
 	if(b_LogFlag==TRUE)
		return TRUE;
	else
		return FALSE;
}

/********************************************************
*�������ܣ�ͨ��COM�ӿڵ�½VM8000/VM5000������
*���ߣ��ž�
*��дʱ�䣺2010-02-21
*����޸�ʱ�䣺 
*s_IPΪVM������IP��ַ
*i_PortΪVM��������½�˿�
*s_NameΪVM�������û���
*s_PwdΪVM����������
*���� TRUEΪ�ɹ� FALSEΪʧ��
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
*�������ܣ����ûص�����ʼ����
*���ߣ��ž�
*��дʱ�䣺2010-02-21
*����޸�ʱ�䣺 
*i_CamΪ��������
*i_chΪͨ����
*li_CallBackΪ���ݻص�����
*s_CamΪ�����ID
*��s_Cam��Ϊ��ʱʹ��s_Cam��Ϊ�����ID
*����ʹ��i_Cam
*���� TRUEΪ�ɹ� FALSEΪʧ��
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
		//MS_INT32 i_CallBackType = 0x00010001;//�ص�ý��������ֹ����ȥ�Ľ�����ʾ
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
*�������ܣ�ֹͣ�ص���ֹͣ����
*���ߣ��Ժ�
*��дʱ�䣺2014-5-13
*����޸�ʱ�䣺 
*i_CamΪ��������
*i_chΪͨ����
*li_CallBackΪ���ݻص�����
*s_CamΪ�����ID
*��s_Cam��Ϊ��ʱʹ��s_Cam��Ϊ�����ID
*����ʹ��i_Cam
*���� TRUEΪ�ɹ� FALSEΪʧ��
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
*�������ܣ����ûص�����ʼ����,ֻ��ȡH264������
*���ߣ��ž�
*��дʱ�䣺2010-02-21
*�޸�ʱ�䣺2014-04-28
*�޸���Ա��zhh
*i_CamΪ��������
*i_chΪͨ����
*li_CallBackΪ���ݻص�����
*s_CamΪ�����ID
*��s_Cam��Ϊ��ʱʹ��s_Cam��Ϊ�����ID
*����ʹ��i_Cam
*���� TRUEΪ�ɹ� FALSEΪʧ��
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
		//MS_INT32 i_CallBackType = 0x00010001;//�ص�ý��������ֹ����ȥ�Ľ�����ʾ
		MS_INT32 i_CallBackType=0x00000002;//|����ǰ����Ƶ
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
