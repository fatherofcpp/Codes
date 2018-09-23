// H3CVM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "h3cvmDlg.h"
extern CWMV_TO_H264Dlg* Main;
static CH3cVMDlg* pH3CDlg=NULL;

//SvlInterfacePtr  PSvlCom;

//#include "gd_type.h"

// CH3cVMDlg �Ի���




static void __stdcall DADA_CallBack(LONG lChannelHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, DWORD dwUser)
{
	char s[256];
	memset(s,0,sizeof(s));

    switch (dwDataType)
    {
		case 0x00000001://ý��������
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("ý��������"));
		}
		break;
		case 0x00000002://����ǰ����Ƶ
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("����ǰ����Ƶ"));
			Svl_COMObject::COM_VideoFrame * pVideoFrame = (Svl_COMObject::COM_VideoFrame *)param;
			CH264Stream *pH264 = (CH264Stream *)dwUser;

			pH264->b_PullLive=0;
			TRACE("coder is %d----%d\n",pVideoFrame->height,pVideoFrame->width);

			HANDLE handle;
			MyAvFrame *m_Frame=NULL;
			m_Frame = new MyAvFrame(dwBufSize+1,1);
			if (NULL == m_Frame)
			{
				m_Log.Add("new MyAvFrame��ʱ���ڴ�������");
				return ;
			}
			if (NULL == m_Frame->p_data)
			{
				m_Log.Add("���䵽��MyAvFrame���p_dataָ��Ϊ�գ�");
				return ;
			}
			memcpy(m_Frame->p_data+1,pBuffer,dwBufSize);
			pH264->m_streamlock.Lock();
			pH264->p_EncodeArray.Add(m_Frame);
			pH264->m_streamlock.Unlock();

		}//end case 0x00000002
		break;
		case 0x00000004://����ǰ����Ƶ
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("����ǰ����Ƶ"));
			Svl_COMObject::COM_VideoFrame *pPCM = (Svl_COMObject::COM_VideoFrame *)param;
		}
		break;
		case 0x00000008://��������Ƶ
		{
			Svl_COMObject::COM_PICTURE * p_Picture = (Svl_COMObject::COM_PICTURE *)param;

			CH264Stream *pH264 = (CH264Stream *)dwUser;

			pH264->b_PullLive=0;

			TRACE("decoder is %d:%d----%d\n",p_Picture->uVideoWidth,p_Picture->uVideoHeight,p_Picture->bYuvOrRgb);

			if (0 == p_Picture->bYuvOrRgb)
			{
				//m_buff.bYUV=false;
				return ;
			}
			else
			{
				//m_buff.bYUV=true;
			}
			if(pH264 == NULL)
				return;

			MyAvFrame *m_Frame=NULL;
			m_Frame = new MyAvFrame(p_Picture->uVideoWidth,p_Picture->uVideoHeight);
			if (NULL == m_Frame)
			{
				m_Log.Add("new MyAvFrame��ʱ���ڴ�������");
				return ;
			}
			if (NULL == m_Frame->p_data)
			{
				m_Log.Add("���䵽��MyAvFrame���p_dataָ��Ϊ�գ�");
				return ;
			}
			memcpy(m_Frame->p_data,pBuffer,dwBufSize);
			//memcpy(m_Frame->p_data,(char*)p_Picture->data[0],p_Picture->uVideoWidth*p_Picture->uVideoHeight);
			//memcpy(&m_Frame->p_data[p_Picture->uVideoWidth*p_Picture->uVideoHeight],(char*)p_Picture->data[1],p_Picture->uVideoWidth*p_Picture->uVideoHeight/4);
			//memcpy(&m_Frame->p_data[p_Picture->uVideoWidth*p_Picture->uVideoHeight+(p_Picture->uVideoWidth*p_Picture->uVideoHeight)/4],(char*)p_Picture->data[2],p_Picture->uVideoWidth*p_Picture->uVideoHeight/4);

			pH264->m_streamlock.Lock();
			pH264->p_YuvArray.Add(m_Frame);
			pH264->m_streamlock.Unlock();
		}
		break;
		case 0x00000010://��������Ƶ
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("��������Ƶ"));
			Svl_COMObject::COM_VideoFrame *pPCM = (Svl_COMObject::COM_VideoFrame *)param;
		}
		break;
	}//end switch (dwDataType)
    return;
}


// CH3cVMDlg ��Ϣ�������
/*************************************************************************************
�������ƣ�CheckExit
��������: check if video is online
�������ã�CWMV_TO_H264Dlg::ChangeCh(CString CamID, int CH,CString pubserver)
ȫ�ֱ�����
����1��Camera Id
�� �� ֵ:
����ʱ�䣺2014-4-21
�޸�����        �汾��     �޸���           �޸�����
-------------------------------------------------
2014-4-21     V1.0        jicheng         ����
**************************************************************************************/
CString CH3cVMDlg::CheckExit(CString str)
{
	CString UrlStr="",CamID = str;

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//���֮ǰ�Ѿ�����ͣ,��ֱ�ӷ��ص�ǰ���л�����BMS�·��л����
				m_Log.Add("��Ƶ:%s �Ѿ�ֹͣ���ţ�������!",CamID);
				//return "";
				continue;

			}

			UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);
			m_Log.Add("��Ƶ:%s ����!URL:%s",CamID,UrlStr);


			m_h264[i]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š�

			return UrlStr;
		}


	}
	m_Log.Add("��Ƶ:%s ������!",CamID);

	return UrlStr;
}

/*************************************************************************************
�������ƣ�ChangeChannel
��������: change video to channel
�������ã�CWMV_TO_H264Dlg::ChangeCh(CString CamID, int CH,CString pubserver)
ȫ�ֱ�����
����1��CamId
����2��way to change to
����3��
�� �� ֵ:
����ʱ�䣺2014-4-21
�޸�����        �汾��     �޸���           �޸�����
-------------------------------------------------
2014-4-21     V1.0        jicheng         ����
**************************************************************************************/
CString CH3cVMDlg::ChangeChannel(CString sId,int CH,CString str)
{
	int ret=0;
	CString UrlStr="";

	//m_h264[CH]->m_Info.s_DevID=sId;
	//tmpParams.m_dstStream = CH;

	SetParams(m_h264[CH],&m_h3cVMParams[CH]);
	m_Log.Add("��Ƶ׼���л�%u:%s",GetTickCount(),m_h264[CH]->m_Info.s_DevID);


//end close and pull
	UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
	m_Log.Add("Ƶ�л��ɹ�%u:%s",GetTickCount(),UrlStr);
	m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����
	return UrlStr;

}


#if 0
<h3c> 
<type>vm8000</type>//�豸����
<ver>1120p25</ver>//�汾
<ip>192.168.0.5</ip>//IP��ַ  //
<port>8000</port>//�˿�
<user>admin</user>//�û���
<passwd>123456</passwd>//����
<channel>123</channel>//Ĭ�ϲ���ͨ��
</h3c>
#endif

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20
/*************************************************************************************
�������ƣ�ParseRequest
��������: parse post information
�������ã�notify_handler
ȫ�ֱ�����
������post data
������
�� �� ֵ:
����ʱ�䣺2014-4-21
�޸�����        �汾��     �޸���           �޸�����
-------------------------------------------------
2014-4-21     V1.0        jicheng         ����
**************************************************************************************/
int CH3cVMDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	DEV_H3CVM_PARAMS  tmpParams;

	CString CH;
	CString CamId;
	//CString tmpSource,CH,tmpType,tmpVer,tmpIp, tmpPort, pubserverip;
	//tmpSource = m_Info.s_DevID;
	//tmpDstip = m_Info.s_PubServer;

	memset(&tmpParams,0,sizeof(DEV_H3CVM_PARAMS));

	rt = f_xmlparse_init(&xml, (const char*)pXmlBuf);
	if (rt != 0)
	{
		m_Log.Add("xml��������xml�ṹ���ʼ��ʧ�ܣ�");
		rt = 400;
		goto leaveOut;
	}

	if(NULL == xml.root)
	{
		m_Log.Add("xml����������Ч��XML�ļ���");
		rt = 400;
		goto leaveOut;
	}


	if (strcmp(xml.root->name, "command") != 0)
	{
		m_Log.Add("xml��������δ���ҵ�Command�ڵ�!");
		rt = 400;
		goto leave;
	}

	root = xml.root;
	node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
	if(node == NULL)
	{
		m_Log.Add("xml��������δ���ҵ�serverip�ڵ�!\n");
		rt = 400;
		goto leave;
	}
	else
	{
		strcpy(tmpParams.m_dstIp, node->data);
		//m_Log.Add("��������serverip: %s",serverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=2)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���2!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "h3c");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�h3c�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����h3c��XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			//tmpType = node->data;
			//m_Log.Add("��������type��%s",tmpType);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�type�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ver", MAX_LEN);
		if (node != NULL)
		{
			//tmpVer=node->data;
			//m_Log.Add("��������ver��%s",tmpVer);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�ver�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpParams.m_proxyIp,node->data);
			//m_Log.Add("��������ip��%s",tmpIp);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�ip�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			tmpParams.m_proxyPort=atoi(node->data);
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "user", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpParams.m_userName, node->data);
			//m_Log.Add("��������user��%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�user�ڵ�!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpParams.m_passWord, node->data);
			//m_Log.Add("��������passwd��%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�passwd�ڵ�!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			tmpParams.m_deviceId = atoi(node->data);
			//m_Log.Add("��������channel��%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�channel�ڵ�!");
			rt = 400;
			goto leave;
		}
	}
#if 1
	chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
	if (chNode != NULL)
	{
		CamId.Format("%d",tmpParams.m_deviceId);
		if(0 == chNode->data_siz)
		{
			m_Log.Add("��������chΪ��ֵ!");		
			m_Log.Add("������Ƶ��%s�Ƿ���ڣ�",tmpParams.m_dstPub);
			WaitForSingleObject(Main->m_Lock,INFINITE);
			
			resultStr=	CheckExit(CamId);
			ReleaseMutex(Main->m_Lock);
			if ("" == resultStr)
			{
				rt = 404;

				goto leave;
			}
			else
			{
				rt = 200;
				*str = resultStr;
			}

		}

		else
		{
			CH = chNode->data;
			if(atoi(CH)>=g_maxLiveNum)
			{
				//rt = 503;
				//goto leaveOut;
				//char tmp[4]="";
				int ntmp = atoi(CH);
				ntmp = ntmp%g_maxLiveNum;

				//itoa(ntmp,tmp,2);
				CH.Format("%d",ntmp);
			}
#if 1
			m_h264[atoi(CH)]->m_Info.s_DevID = CamId;
			m_h264[atoi(CH)]->m_Info.s_PubServer = tmpParams.m_dstIp;
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",CamId,atoi(CH));
			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(CamId,atoi(CH),tmpParams.m_dstIp);
			ReleaseMutex(Main->m_Lock);
#endif
			if ("" == resultStr)
			{
				rt = 503;
				goto leaveOut;
			}
			else
			{
				rt = 200;
				m_Log.Add(resultStr);
				*str = resultStr;
			}
		}
	}
	else
	{
		m_Log.Add("xml��������ch�ڵ��ֵΪ��!");
		rt = 400;
		goto leave;
	}
	//m_Log.Add("������ͨ��%d: ����Ϊ:Serverip:%s Company:%s DevType:%s Version:%s IP:%s Port:%s Url:%s\n",atoi(CH), pubserverip,"microvideo",tmpType, tmpVer, tmpIp, tmpPort, tmpSource);//����������
	//TRACE("DevType:%s Version:%s IP:%s Port:%s Url:%s", tmpType, tmpVer, tmpIp, tmpPort, tmpSource);
#endif


leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}


int CH3cVMDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("ProxyIp%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cVMParams[nIndex].m_proxyIp,dst);


	tmpStr.Format("ProxyPort%d",nIndex);
	ZeroMemory(dst,255);
	m_h3cVMParams[nIndex].m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_h3cVMParams.m_proxyPort:%d\n",m_h3cVMParams[nIndex].m_proxyPort);


	tmpStr.Format("UserName%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cVMParams[nIndex].m_userName,dst);
	TRACE("m_h3cVMParams[nIndex].m_userName:%s\n",m_h3cVMParams[nIndex].m_userName);


	tmpStr.Format("PassWd%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cVMParams[nIndex].m_passWord,dst);
	TRACE("m_h3cVMParams[nIndex].m_passWord:%s\n",m_h3cVMParams[nIndex].m_passWord);

	//InitProxyServer(NULL);

	m_Log.Add("��ʼ��ͨ��������: ����Ϊ:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		m_h3cVMParams[nIndex].m_proxyIp,m_h3cVMParams[nIndex].m_proxyPort,m_h3cVMParams[nIndex].m_userName,
		m_h3cVMParams[nIndex].m_passWord);
	
	tmpStr.Format("DeviceId%d",nIndex);
	ZeroMemory(dst,255);

	m_h3cVMParams[nIndex].m_deviceId=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	//strcpy(m_h3cVMParams[nIndex].m_deviceName,dst);

	tmpStr.Format("Dstip%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cVMParams[nIndex].m_dstIp,dst);

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cVMParams[nIndex].m_dstPub,dst);

	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_h3cVMParams[nIndex].m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);



	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV�����ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);


	m_h264[nIndex]->thisIndex = SourceCount;
	m_h264[nIndex]->m_Info.s_PubServer=m_h3cVMParams[nIndex].m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",SourceCount);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_h3cVMParams[nIndex].m_dstPub,SourceCount);
	m_h264[nIndex]->m_devType = DEF_H3CVM_DEV;

	SetParams(m_h264[nIndex],&m_h3cVMParams[nIndex]);
	//StartPull(m_h264[nIndex],&m_h3cVMParams[nIndex]);



	return 0;
}

//����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
//�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
int CH3cVMDlg::SetDefaultParams(void * pParams)
{
	if(SourceCount>=g_maxLiveNum)//���������ӣ���ȫ��16·
		return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream��ȫ16��ʱ������ڴ�ʱ���ִ���",SourceCount);
		}

		m_h264[SourceCount]->b_Encode	= (g_bUseTc==1?1:0);

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->m_Info.s_PubServer=m_h264[0]->m_Info.s_PubServer;
		m_h264[SourceCount]->m_devType = DEF_H3CVM_DEV;
	}

	return 0;
}

int CH3cVMDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3CVM_PARAMS* pH3cVM=&m_h3cVMParams[pH264Stream->thisIndex];
	int ret=0;

	pH264Stream->m_Info.s_DevID.Format("%d",pH3cVM->m_deviceId);
	pH264Stream->m_devType = DEF_H3CVM_DEV;

	if (false == pH264Stream->b_start)
	{
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);


	return ret;
}



// ������������
int CH3cVMDlg::StartPull(void *pStream,void* pParams)
{
	int ret=0;
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3CVM_PARAMS* pH3cVM=&m_h3cVMParams[pH264Stream->thisIndex];

	M_BOOL b_Ret=FALSE;

	b_Ret = m_SourceFilter.GetAlive();
	if(b_Ret == FALSE)
	{

		b_Ret =m_SourceFilter.Init();
		if (FALSE == b_Ret)
		{
			m_Log.Add("SDK1��ʼ��ʧ��");
			return -1;
		}

		//b_Ret =m_SourceFilter.Login(a_sr_StreamInfo[0].s_VMIP,a_sr_StreamInfo[0].i_VM_Port,a_sr_StreamInfo[0].s_VM_User,a_sr_StreamInfo[0].s_VM_Pwd);
		b_Ret =m_SourceFilter.Login(pH3cVM->m_proxyIp,pH3cVM->m_proxyPort,pH3cVM->m_userName,pH3cVM->m_passWord);

		if (FALSE == b_Ret)
		{
			m_Log.Add("SDK1��½ʧ��,��ַ%s,�˿�%d,�û���%s,����%s",pH3cVM->m_proxyIp,pH3cVM->m_proxyPort,pH3cVM->m_userName,pH3cVM->m_passWord);
			return -1;
		}
	}

	if(pH264Stream->b_Encode == 1)
		b_Ret = m_SourceFilter.ReQuestStream(pH3cVM->m_deviceId,pH264Stream->thisIndex+1,(long)DADA_CallBack,(MU_INT32)pStream);
	else
		b_Ret = m_SourceFilter.ReQuestStreamEX(pH3cVM->m_deviceId,pH264Stream->thisIndex+1,(long)DADA_CallBack,(MU_INT32)pStream);

	if (TRUE == b_Ret)
	{
		m_Log.Add("��%d�����ɹ�",pH3cVM->m_deviceId);
	}
	else
	{
		m_Log.Add("��%d����ʧ��",pH3cVM->m_deviceId);
	}

	pH264Stream->h_stream=&b_Ret;

	return 0;
}

int CH3cVMDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;

	m_SourceFilter.StopStream(0, pH264Stream->thisIndex+1,0,0);
	
	pH264Stream->h_stream=NULL;

	return 0;
}



// H3cVMDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "H3cVMDlg.h"


// CH3cVMDlg �Ի���

IMPLEMENT_DYNAMIC(CH3cVMDlg, CDialog)

CH3cVMDlg::CH3cVMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CH3cVMDlg::IDD, pParent)
{

}

CH3cVMDlg::~CH3cVMDlg()
{
}

void CH3cVMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CH3cVMDlg, CDialog)
END_MESSAGE_MAP()


// CH3cVMDlg ��Ϣ�������
