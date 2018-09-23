// DfhsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "DfhsDlg.h"


extern CWMV_TO_H264Dlg* Main;

static CDfhsDlg* pDfhsDlg=NULL;
// CDfhsDlg �Ի���

IMPLEMENT_DYNAMIC(CDfhsDlg, CDialog)

CDfhsDlg::CDfhsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDfhsDlg::IDD, pParent)
{

	pDfhsDlg=this;
	for(int i=0;i<16;i++)
	{
	//	m_h264[i] = NULL;
	}

}

CDfhsDlg::~CDfhsDlg()
{
	//for(int i=0;i<16;i++)
	//{
	//	if(NULL != m_h264[i])
	//	{
	//		delete m_h264[i];
	//	}
	//}
}

void CDfhsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUDEVOCXCTRL1, m_CudevOcx);
}


BEGIN_MESSAGE_MAP(CDfhsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDfhsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDfhsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDfhsDlg ��Ϣ�������

void CDfhsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
BEGIN_EVENTSINK_MAP(CDfhsDlg, CDialog)
	ON_EVENT(CDfhsDlg, IDC_CUDEVOCXCTRL1, 44, CDfhsDlg::OCXInnerNotifyCudevocxctrl1, VTS_I4)
END_EVENTSINK_MAP()

void CDfhsDlg::OCXInnerNotifyCudevocxctrl1(long nEvent)
{
	// TODO: �ڴ˴������Ϣ����������
	m_Log.Add("[OCXInnerNotifyCudevocxctrl1]:received nEvent:%d",nEvent);
	switch(nEvent)
	{
	case 0://�ɹ�
		{
			
			break;
		}
	//case 1:	//��¼ʧ��
	//	{

	//		break;
	//		
	//	}
	case 2:		//ocx�ڲ�������
		{

			break;
		}
	//case 3:		//����
	//	{
	//
	//		InitProxyServer(NULL);
	//		break;
	//	}
	case 5:
		{

			//char tmp[512];
			//sprintf(tmp,"%d\n",nEvent);

			//MessageBox((LPCTSTR)tmp);
			for(int i=0;i<g_maxLiveNum;i++)
			{
				if(true == m_h264[i]->b_start)
				{
					SetParams(m_h264[i],&m_devDfhsParams);
					StartPull(m_h264[i],&m_devDfhsParams);

				}

			}


			break;
		}
	case 10:	//��ƵԴδ����
		{

		}
		break;
	default:
        {
			//��ʱ�ᷢ�ִ���
			m_Log.Add("[OCXInnerNotifyCudevocxctrl1]:�ڲ�����exit");
			exit(0);
        }
		break;
	}
}

typedef void (__stdcall *TRawFrameCBFunc)(TRawFrameInfo *, void *, unsigned long, unsigned long);
typedef void (__stdcall *TFrameCBFunc)(TFrameInfo *, void *, unsigned long, unsigned long);


void __stdcall RawFrameCBFunc(TRawFrameInfo *pFrame, void *pData, unsigned long dwDataSize, unsigned long dwUserData)
{
	char tmp[512];
	sprintf(tmp,"����ǰý������:%d\n",pFrame->m_byMediaType);

	//AfxMessageBox((LPCTSTR)tmp);

	printf("����ǰý������:%d\n",pFrame->m_byMediaType);

	if((pFrame->m_byMediaType == MEDIA_TYPE_MP4 )||
		(pFrame->m_byMediaType == MEDIA_TYPE_H261 )||
		(pFrame->m_byMediaType == MEDIA_TYPE_H262 )||
		(pFrame->m_byMediaType == MEDIA_TYPE_H263 )||
		(pFrame->m_byMediaType == MEDIA_TYPE_H263PLUS )||
		(pFrame->m_byMediaType == MEDIA_TYPE_H264 ))
		printf("��ý������Ϊvideo:��СΪ%d �û�����%d\n",dwDataSize,dwUserData);
	return;
}
int n_time=0;
void __stdcall FrameCBFunc(TFrameInfo *pFrame, void *pData, unsigned long dwDataSize, unsigned long dwUserData)
{

	//char tmp[512];
	//sprintf(tmp,"�����ý������:%d\n",pFrame->m_byMediaType);

//	TRACE("after decoder userdata is %d\n",dwUserData);

	//AfxMessageBox((LPCTSTR)tmp);
	int nIndex = dwUserData - 2222;//���û��������жϸôεĽ���������·����
//	TRACE("after decoder index is %d\n",nIndex);
	if((nIndex <0) || (nIndex >= g_maxLiveNum))//�����û��涨�ĺϷ���Χ��
	{
		TRACE("quite\n");
		return;
	}
//	TRACE("into\n");


	CH264Stream* pH264=m_h264[nIndex];
	pH264->b_PullLive=0;
		if(n_time % 500 == 0)
		{
			TRACE("[FrameCBFunc]:��·[%s]�߳��������С�%d����\n",pH264->m_Info.s_PubName,n_time);
		}
		n_time++;

//	TRACE("�����ý������:%d\n",pFrame->m_byMediaType);
	if(pFrame->m_byMediaType == 0)
	{
#if 0
	{
		static int itimes = 0;
		static DWORD myT= 0;
		if(itimes%100 == 0)
		{
			//time_t old_time=0;
			//time(&old_time);
			TRACE("*******decode 100 frame %d %d %d \n",GetTickCount()- myT,pFrame->m_wVideoWidth,pFrame->m_wVideoHeight);
			myT = GetTickCount();
		}
		itimes++;
	}
	//return TRUE;
#endif
//911
//		TRACE("pFrame->m_byBitCount=%d,pFrame->m_wVideoWidth=%d,pFrame->m_wVideoHeight=%d\n",
//			pFrame->m_byBitCount,pFrame->m_wVideoWidth,pFrame->m_wVideoHeight);
		
		if(pH264->p_YuvArray.GetSize()<60)
		{

			if(!(pFrame->m_byBitCount) || !(pFrame->m_wVideoWidth) || !(pFrame->m_wVideoHeight) )
			{
				//pH264->m_streamlock.Unlock();
				return;
			}
			MyAvFrame *m_Frame=new MyAvFrame(pFrame->m_wVideoWidth,pFrame->m_wVideoHeight);
			if (NULL == m_Frame)
			{
				m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
				//pH264->m_streamlock.Unlock();
				return ;
			}
			if (NULL == m_Frame->p_data)
			{
				
				return ;
			}
		memcpy(m_Frame->p_data,pData,pFrame->m_wVideoWidth*pFrame->m_wVideoHeight);
		memcpy(&m_Frame->p_data[pFrame->m_wVideoWidth*pFrame->m_wVideoHeight],(char*)pData+pFrame->m_wVideoWidth*pFrame->m_wVideoHeight+(pFrame->m_wVideoWidth*pFrame->m_wVideoHeight)/4,pFrame->m_wVideoWidth*pFrame->m_wVideoHeight/4);
		memcpy(&m_Frame->p_data[pFrame->m_wVideoWidth*pFrame->m_wVideoHeight+(pFrame->m_wVideoWidth*pFrame->m_wVideoHeight)/4],(char*)pData+pFrame->m_wVideoWidth*pFrame->m_wVideoHeight,pFrame->m_wVideoWidth*pFrame->m_wVideoHeight/4);

			//memcpy(m_Frame->p_data,pData,dwDataSize);
			pH264->m_streamlock.Lock();
			pH264->p_YuvArray.Add(m_Frame);
			pH264->m_streamlock.Unlock();
		}
		else
		{
			TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
		}
	
		//pH264->m_streamlock.Unlock();

//911

		//sprintf(tmp,"��ý������Ϊvideo:��СΪ%d �û�����%d,�ߣ�%d ��%d\n",dwDataSize,dwUserData,pFrame->m_wVideoHeight,pFrame->m_wVideoWidth);
	//AfxMessageBox((LPCTSTR)tmp);
	}
		return;
}

TRawFrameCBFunc tRaw = RawFrameCBFunc;
TFrameCBFunc    tFrame = FrameCBFunc;



int CDfhsDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_DFHS_PARAMS *pDfhsParams = (DEV_DFHS_PARAMS *)pParams;
	int ret;

	//m_Info.s_DevID=pParams->m_deviceId+_T(":");
	pH264Stream->m_Info.s_DevID.Format("%s:%d",pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId.GetBuffer(pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId.GetLength()),pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn);//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1
	pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId.ReleaseBuffer();

	if (false == pH264Stream->b_start)
	{
		pH264Stream->first=true;
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		//��StartPush�����ÿ�Ⱥ͸߶ȵ���Ϣ�������߳̽���pushstream����
		//��pushstream()ʵ�ְѰѴ�ģ���ȡ����Ƶ���ݱ��룻���浽ָ���ṹ��pMain->g_rtmp
		pH264Stream->b_start=true;
	}
	ret = pH264Stream->StartPull(pStream,pParams);

	return 0;

}

// ������������

int CDfhsDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_DFHS_PARAMS *pDfhsParams = &m_devDfhsParams;//(DEV_SANTACHI_PARAMS *)pParams;

	int status=FALSE;
	if (NULL!=pH264Stream->h_stream)
	{
		int stop_ret=m_CudevOcx.StopPlayByWndIndex(pH264Stream->thisIndex);
		if(! stop_ret)
		{
			m_Log.Add("[CDfhsDlg::StartPull]:[fail when operate %s]:devId:%s,devChn:%d,fail to stopplay old play,stop_ret=%d\n",
				pH264Stream->m_Info.s_PubName,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
				stop_ret);
			TRACE("[CDfhsDlg::StartPull]:[fail when operate %s]:devId:%s,devChn:%d,fail to stopplay old play,stop_ret=%d\n",
				pH264Stream->m_Info.s_PubName,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
				stop_ret);
		}
		pH264Stream->h_stream=NULL;
	}
	
	char xmlbuf[512];
	int nlen = 0;
	memset(xmlbuf,0,sizeof(xmlbuf));
	nlen += sprintf(xmlbuf+nlen,"<OCXXML>");
	nlen += sprintf(xmlbuf+nlen,"<id>3016</id>");
	nlen += sprintf(xmlbuf+nlen,"<wndid>%d</wndid>",pH264Stream->thisIndex);
	nlen += sprintf(xmlbuf+nlen,"<nodecode>0</nodecode>");
	nlen += sprintf(xmlbuf+nlen,"<rawstreamcb>%d</rawstreamcb>",tRaw);
	nlen += sprintf(xmlbuf+nlen,"<rawuserdata>%d</rawuserdata>",1111+pH264Stream->thisIndex);
	nlen += sprintf(xmlbuf+nlen,"<streamcb>%d</streamcb>",tFrame);
	nlen += sprintf(xmlbuf+nlen,"<userdata>%d</userdata>",2222+pH264Stream->thisIndex);
	nlen += sprintf(xmlbuf+nlen,"</OCXXML>");

	int comEnt_ret=m_CudevOcx.OperateCommonEntry((LPTSTR)xmlbuf);
	if(comEnt_ret!=0)
	{
		m_Log.Add("[CDfhsDlg::StartPull]:[fail when operate %s]:devId:%s,devChn:%d,fail to OperateCommonEntry,comEnt_ret=%d\n",
					pH264Stream->m_Info.s_PubName,
					pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
					pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
					comEnt_ret);
		TRACE("[CDfhsDlg::StartPull]:[fail when operate %s]:devId:%s,devChn:%d,fail to OperateCommonEntry,comEnt_ret=%d\n",
					pH264Stream->m_Info.s_PubName,
					pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
					pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
					comEnt_ret);
	}
	int startplay_ret=m_CudevOcx.StartPlayByWndIndex(pH264Stream->thisIndex,pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,(short)pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn);
	if(! startplay_ret)
	{
		m_Log.Add("[CDfhsDlg::StartPull]:[fail when operate  %s]:devId:%s,devChn:%d,fail to start play ,startplay_ret=%d\n",
					pH264Stream->m_Info.s_PubName,
					pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
					pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
					startplay_ret);
		TRACE("[CDfhsDlg::StartPull]:[fail when operate  %s]:devId:%s,devChn:%d,fail to start play ,startplay_ret=%d\n",
				pH264Stream->m_Info.s_PubName,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
				startplay_ret);
	}
	
	pH264Stream->h_stream = &pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn;
	TRACE("[CDfhsDlg::StartPull]:[success operate %s]:devId:%s,devChn:%d\n",
			pH264Stream->m_Info.s_PubName,
			pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
			pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn);
	return 0;
}

int CDfhsDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_DFHS_PARAMS *pDfhsParams = (DEV_DFHS_PARAMS *)pParams;
	//pDfhsParams�п���Ϊ��

	int status=FALSE;

	if (NULL!=pH264Stream->h_stream)
	{

		int close_ret=m_CudevOcx.StopPlayByWndIndex(pH264Stream->thisIndex);
		if(! close_ret)
		{
			m_Log.Add("[CDfhsDlg::ClosePull]:[fail when operate %s]:devId:%s  fail to stopplay old play,stop_ret=%d\n",
						pH264Stream->m_Info.s_PubName,
						pH264Stream->m_Info.s_DevID,
						close_ret);
			TRACE("[CDfhsDlg::ClosePull]:[fail when operate %s]:devId:%s  fail to stopplay old play,stop_ret=%d\n",
					pH264Stream->m_Info.s_PubName,
					pH264Stream->m_Info.s_DevID,
					close_ret);
		}
		pH264Stream->h_stream=NULL;
		TRACE("[CDfhsDlg::ClosePull]:[successcloseoldplay when operate %s]:devId:%s\n",
				pH264Stream->m_Info.s_PubName,
				pH264Stream->m_Info.s_DevID);
	}

	return 0;
}

int CDfhsDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	char tmp[255],dst[255];
	CString tmpStr;
	//
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	tmpStr.Format("ProxyIp");
	char Server[255];
	ZeroMemory(Server,255);
	GetPrivateProfileString("common",tmpStr,"",Server,255,thisDir);
	m_devDfhsParams.m_proxyIp=Server;
	TRACE("m_devDfhsParams.m_proxyIp:%s\n",m_devDfhsParams.m_proxyIp);

	tmpStr.Format("ProxyPort");

	ZeroMemory(dst,255);
	m_devDfhsParams.m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_devDfhsParams.m_proxyPort:%d\n",m_devDfhsParams.m_proxyPort);

	tmpStr.Format("UserName");
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devDfhsParams.m_userName=dst;
	TRACE("m_devDfhsParams.m_userName:%s\n",m_devDfhsParams.m_userName);

	tmpStr.Format("PassWd");
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devDfhsParams.m_passWord=dst;
	TRACE("m_devDfhsParams.m_passWord:%s\n",m_devDfhsParams.m_passWord);

	InitProxyServer(NULL);

	tmpStr.Format("DeviceId%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devDfhsParams.m_devInfo[SourceCount].m_deviceId=dst;

	tmpStr.Format("DeviceChn%d",SourceCount);
	ZeroMemory(dst,255);
	m_devDfhsParams.m_devInfo[SourceCount].m_deviceChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

	tmpStr.Format("Dstip%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devDfhsParams.m_devInfo[SourceCount].m_dstIp=dst;

	tmpStr.Format("Dstpub%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devDfhsParams.m_devInfo[SourceCount].m_dstPub=dst;

	tmpStr.Format("Dststream%d",SourceCount);
	ZeroMemory(dst,255);
	m_devDfhsParams.m_devInfo[SourceCount].m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

	//
	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV�����ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[nIndex]->thisIndex = nIndex;
	m_h264[nIndex]->b_Encode=1;//(g_bUseTc==1?1:0);
	//m_h264[nIndex]->b_Decode = 1;

	m_h264[nIndex]->m_Info.s_PubServer=m_devDfhsParams.m_devInfo[nIndex].m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devDfhsParams.m_devInfo[nIndex].m_dstPub,nIndex);
	//m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,nIndex);

	//m_h264[nIndex]->pNetVideo = &m_netVideo[nIndex];//��ʱʹ��
	m_h264[nIndex]->m_devType = DEF_DFHS_DEV;

	SetParams(m_h264[nIndex],&m_devDfhsParams);
	//StartPull(m_h264[nIndex],&m_devParams[nIndex]);

	m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceId%d:%s,DeviceChn%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		SourceCount,
		SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_deviceId,
		SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_deviceChn,
		SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_dstIp,
		SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_dstPub,
		SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_dstStream);//����������
	TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);
	return 0;
}

int CDfhsDlg::GetInforFromIni(void * pThisDir)
{
//	char tmp[255];
	char dst[255];
	CString tmpStr;
/*	CString SourceStr="";
	CString ServerIP="";
	CString PubName="";
	int StreamNumber=0;
*/
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);
	SourceCount=0;
	tmpStr.Format("ProxyIp");
	char Server[255];
	ZeroMemory(Server,255);
	GetPrivateProfileString("dfhs",tmpStr,"",Server,255,thisDir);
	 m_devDfhsParams.m_proxyIp=Server;
	 TRACE("m_devDfhsParams.m_proxyIp:%s\n",m_devDfhsParams.m_proxyIp);
	if("" == m_devDfhsParams.m_proxyIp)
	{
		m_Log.Add("config.ini�ļ���δ���ҵ��ط����µ�m_proxyIp");
		return 0;
	}
	else
	{
		tmpStr.Format("ProxyPort");
		
		ZeroMemory(dst,255);
		m_devDfhsParams.m_proxyPort=GetPrivateProfileInt("dfhs",tmpStr,-1,thisDir);
		TRACE("m_devDfhsParams.m_proxyPort:%d\n",m_devDfhsParams.m_proxyPort);

		tmpStr.Format("UserName");
		ZeroMemory(dst,255);
		GetPrivateProfileString("dfhs",tmpStr,"",dst,255,thisDir);
		m_devDfhsParams.m_userName=dst;
		TRACE("m_devDfhsParams.m_userName:%s\n",m_devDfhsParams.m_userName);

		tmpStr.Format("PassWd");
		ZeroMemory(dst,255);
		GetPrivateProfileString("dfhs",tmpStr,"",dst,255,thisDir);
		m_devDfhsParams.m_passWord=dst;
		TRACE("m_devDfhsParams.m_passWord:%s\n",m_devDfhsParams.m_passWord);


		if (-1 == m_devDfhsParams.m_proxyPort || "" == m_devDfhsParams.m_userName || "" == m_devDfhsParams.m_passWord)
		{
			m_Log.Add("config.ini�ļ���δ����ȫ�ط����µ�ProxyPort,UserName,PassWd");

			return 0;
		}

		InitProxyServer(NULL);

		m_Log.Add("��ʼ��ͨ��������: ����Ϊ:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		 m_devDfhsParams.m_proxyIp,m_devDfhsParams.m_proxyPort,m_devDfhsParams.m_userName,
		 m_devDfhsParams.m_passWord);
	
	}
	while(1)//��ȡconfig.ini������shsl,��Ӧnew CH264Stream
	{
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("�ﵽ���ת��·������������ֹͣ������%d��%d",SourceCount,g_maxLiveNum);
			break;
		}
		tmpStr.Format("DeviceId%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString("dfhs",tmpStr,"",dst,255,thisDir);
		m_devDfhsParams.m_devInfo[SourceCount].m_deviceId=dst;

		tmpStr.Format("DeviceChn%d",SourceCount);
		ZeroMemory(dst,255);
		m_devDfhsParams.m_devInfo[SourceCount].m_deviceChn=GetPrivateProfileInt("dfhs",tmpStr,-1,thisDir);

		tmpStr.Format("Dstip%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString("dfhs",tmpStr,"",dst,255,thisDir);
		m_devDfhsParams.m_devInfo[SourceCount].m_dstIp=dst;

		tmpStr.Format("Dstpub%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString("dfhs",tmpStr,"",dst,255,thisDir);
		m_devDfhsParams.m_devInfo[SourceCount].m_dstPub=dst;

		tmpStr.Format("Dststream%d",SourceCount);
		ZeroMemory(dst,255);
		m_devDfhsParams.m_devInfo[SourceCount].m_dstStream=GetPrivateProfileInt("dfhs",tmpStr,-1,thisDir);

		if ("" == m_devDfhsParams.m_devInfo[SourceCount].m_deviceId || -1 == m_devDfhsParams.m_devInfo[SourceCount].m_deviceChn || ""==m_devDfhsParams.m_devInfo[SourceCount].m_dstIp || ""==m_devDfhsParams.m_devInfo[SourceCount].m_dstPub || -1==m_devDfhsParams.m_devInfo[SourceCount].m_dstStream)
		{
			m_Log.Add("config.ini�ļ���δ����ȫ�ط����µ�DeviceId,DeviceChn,Dstip,Dstpub,Dststream");
			break;
		}

		{
			m_h264[SourceCount]=new CH264Stream;
				if(NULL==m_h264[SourceCount])
				{
					m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV�����ڴ�ʱ���ִ���",SourceCount);
				}

				m_h264[SourceCount]->thisIndex = SourceCount;
				m_h264[SourceCount]->b_Encode	= 1;

				m_h264[SourceCount]->m_Info.s_PubServer=m_devDfhsParams.m_devInfo[SourceCount].m_dstIp;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",DEF_CS_DFHS_FLAG,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
				m_h264[SourceCount]->m_devType = DEF_DFHS_DEV;
		}

		

		m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceId%d:%s,DeviceChn%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_deviceId,
			SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_deviceChn,
			SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_dstIp,
			SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_dstPub,
			SourceCount,m_devDfhsParams.m_devInfo[SourceCount].m_dstStream);//����������
		TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);
		SourceCount++;

	}

	return 0;
}

//����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
//�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
int CDfhsDlg::SetDefaultParams(void * pParams)
{
	if(SourceCount>=g_maxLiveNum)//���������ӣ���ȫ��16·
		return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		TRACE("[SetDefaultParams]:SourceCount=%d\n",SourceCount);
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream��ȫ16��ʱ������ڴ�ʱ���ִ���",SourceCount);
		}

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->b_Encode	= 1;//(g_bUseTc==1?1:0);

		m_h264[SourceCount]->m_Info.s_PubServer=m_devDfhsParams.m_devInfo[0].m_dstIp;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devDfhsParams.m_devInfo[0].m_dstPub,SourceCount);

		//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
		m_h264[SourceCount]->m_devType = DEF_DFHS_DEV;

	}
	TRACE("[SetDefaultParams]:End SourceCount=%d\n",SourceCount);

	return 0;
}

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20


int CDfhsDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	DEV_DFHS_INFO  tmpInfo;
	int rt;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	CString CH;
	//CString tmpSource,CH,tmpType,tmpVer,tmpIp, tmpPort, pubserverip;
	//tmpSource = m_Info.s_DevID;
	//tmpDstip = m_Info.s_PubServer;

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
	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=6)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���6!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "dfhs");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�dfhs�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
		{
			m_Log.Add("��ʼ�ط�����xml����!");
			node = f_xmlnode_sch_elem_ext(subroot, "proxyIp", MAX_LEN);
			if (node != NULL)
			{
				m_devDfhsParams.m_proxyIp.Format("%s",node->data);
				m_Log.Add("��������proxyIP��%s",m_devDfhsParams.m_proxyIp);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�proxyIP�ڵ�!");
				rt = 400;
				goto leave;
			}

			node = f_xmlnode_sch_elem_ext(subroot, "proxyPort", MAX_LEN);
			if (node != NULL)
			{
				m_devDfhsParams.m_proxyPort=atoi(node->data);
				m_Log.Add("��������proxyPort��%d",m_devDfhsParams.m_proxyPort);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�proxyPort�ڵ�!");
				rt = 400;
				goto leave;
			}
			node = f_xmlnode_sch_elem_ext(subroot, "userName", MAX_LEN);
			if (node != NULL)
			{
				m_devDfhsParams.m_userName=node->data;
				m_Log.Add("��������userName��%s",m_devDfhsParams.m_userName);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�userName�ڵ�!");
				rt = 400;
				goto leave;
			}

			node = f_xmlnode_sch_elem_ext(subroot, "passWord", MAX_LEN);
			if (node != NULL)
			{
				m_devDfhsParams. m_passWord=node->data;
				m_Log.Add("��������passWord��%d",m_devDfhsParams.m_passWord);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�passWord�ڵ�!");
				rt = 400;
				goto leave;
			}		
			node = f_xmlnode_sch_elem_ext(subroot, "deviceId", MAX_LEN);
			if (node != NULL)
			{
				tmpInfo.m_deviceId=node->data;
				m_Log.Add("��������deviceId��%s",tmpInfo.m_deviceId);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�deviceId�ڵ�!");
				rt = 400;
				goto leave;
			}

			node = f_xmlnode_sch_elem_ext(subroot, "devicePort", MAX_LEN);
			if (node != NULL)
			{
				tmpInfo.m_deviceChn=atoi(node->data);
				m_Log.Add("��������devicePort��%d",tmpInfo.m_deviceChn);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�devicePort�ڵ�!");
				rt = 400;
				goto leave;
			}
#if 0
			node = f_xmlnode_sch_elem_ext(subroot, "dstPub", MAX_LEN);
			if (node != NULL)
			{
				tmpInfo.m_dstPub=node->data;
				m_Log.Add("��������dstPub��%s",tmpInfo.m_dstPub);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�dstPub�ڵ�!");
				rt = 400;
				goto leave;
			}
			node = f_xmlnode_sch_elem_ext(subroot, "dstStream", MAX_LEN);
			if (node != NULL)
			{
				tmpInfo.m_dstStream=atoi(node->data);
				m_Log.Add("��������dstStream��%d",tmpInfo.m_dstStream);
			}
			else
			{
				m_Log.Add("xml��������δ���ҵ�dstStream�ڵ�!");
				rt = 400;
				goto leave;
			}
#endif
			node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
			if (node != NULL)
			{
				tmpInfo.m_dstIp = node->data;
				m_Log.Add("��������serverip��%s",tmpInfo.m_dstIp);
			}

			chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
			if (chNode == NULL)
			{
				m_Log.Add("xml��������δ���ҵ�ch�ڵ�!");
				rt = 400;
				goto leave;
			}

			//**********************************//
			//**********************************//

			m_Log.Add("������ͨ��: ����Ϊ:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,devicePort:%d��dstPub:%s, dstStream:%d,serverip:%s\n",
				m_devDfhsParams.m_proxyIp,
				m_devDfhsParams.m_proxyPort,
				m_devDfhsParams.m_userName,
				m_devDfhsParams.m_passWord,
				tmpInfo.m_deviceId,
				tmpInfo.m_deviceChn,
				tmpInfo.m_dstPub,
				tmpInfo.m_dstStream,
				tmpInfo.m_dstIp);//����������

			TRACE("������ͨ��: ����Ϊ:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,devicePort:%d��dstPub:%s, dstStream:%d,serverip:%s\n",
				m_devDfhsParams.m_proxyIp,
				m_devDfhsParams.m_proxyPort,
				m_devDfhsParams.m_userName,
				m_devDfhsParams.m_passWord,
				tmpInfo.m_deviceId,
				tmpInfo.m_deviceChn,
				tmpInfo.m_dstPub,
				tmpInfo.m_dstStream,
				tmpInfo.m_dstIp);	

		}

#if 1
		{

			if (0 == chNode->data_siz)
			{
				m_Log.Add("��������chΪ��ֵ!");		
				m_Log.Add("������Ƶ��%s:%d �Ƿ���ڣ�",tmpInfo.m_deviceId,tmpInfo.m_deviceChn);
				WaitForSingleObject(Main->m_Lock,INFINITE);
				tmpStr.Format("%s:%d",tmpInfo.m_deviceId,tmpInfo.m_deviceChn);

				resultStr=	CheckExit(tmpStr);
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
				int m_ch=0;
				//m_Log.Add("�л�%s,��ͨ��%d,������%s",tmpStr,atoi(chNode->data),serverStr);
				if("" == tmpInfo.m_dstIp)
				{
					m_Log.Add("�л���Ƶ%s:%dʧ�ܣ�δ�ҵ�serverip!",tmpInfo.m_deviceId,tmpInfo.m_deviceChn);
					rt = 400;
					goto leave;
				}
				if(atoi(chNode->data)>=g_maxLiveNum)
				{
					//rt = 503;
					//goto leaveOut;
					//char tmp[4]="";
					m_ch = atoi(chNode->data);
					m_ch = m_ch%g_maxLiveNum;

					m_devDfhsParams.m_devInfo[m_ch] = tmpInfo;
					//TRACE("INFO:***ԭͨ��%d:%d\n",)

					
				}else
				{
					m_devDfhsParams.m_devInfo[atoi(chNode->data)] = tmpInfo;
					m_ch = atoi(chNode->data);
				}

				WaitForSingleObject(Main->m_Lock,INFINITE);
				resultStr=ChangeChannel(tmpStr,m_ch,tmpInfo.m_dstIp);
				ReleaseMutex(Main->m_Lock);

				if ("" == resultStr)
				{
					rt = 503;
					goto leave;
				}
				else
				{
					rt = 200;
					*str = resultStr;
					
				}
			}

		}
#endif

leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}


CString CDfhsDlg::CheckExit(CString str)
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


CString CDfhsDlg::ChangeChannel(CString sId,int CH,CString str)
{
	CString UrlStr="";

	if(CH >= g_maxLiveNum)
	{//��ʱ�������ת��·��
		m_Log.Add("ԭ��ƵӦ���л�ͨ��Ϊ:%d \n!",CH);
		CH = CH%g_maxLiveNum;
		SourceCount = g_maxLiveNum;
		m_Log.Add("ʵ����ƵӦ���л�ͨ��Ϊ:%d \n!",CH);
	}

	if (CH>=SourceCount)
	{

		m_Log.Add("CH��ֵ���ܴ���%d������!",g_maxLiveNum);
		return "";
	}
	
	//TRACE("");
	m_h264[CH]->m_Info.s_DevID.Format("%s:%d",m_devDfhsParams.m_devInfo[CH].m_deviceId.GetBuffer(m_devDfhsParams.m_devInfo[CH].m_deviceId.GetLength()),m_devDfhsParams.m_devInfo[CH].m_deviceChn);//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1
	m_devDfhsParams.m_devInfo[CH].m_deviceId.ReleaseBuffer();

	m_Log.Add("�ط�����׼���л�%s,��ͨ��%d,������%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	if (NULL!=m_h264[CH]->h_stream)
	{//��·�Ѿ���ʹ�ã���Ҫ�ȹر��ٽ��в���
		//m_netVideo[CH].CloseVideo();

		ClosePull(m_h264[CH],&m_devDfhsParams);

		m_h264[CH]->h_stream=NULL;

	}

	m_h264[CH]->m_Info.s_PubServer=str;

	//m_h264[CH]->Init(&m_devDfhsParams,this->GetSafeHwnd());
	//m_h264[CH]->StartGet(m_devDfhsParams.m_devicePort);
	SetParams(m_h264[CH],&m_devDfhsParams);
//	StartPull(m_h264[CH],&m_devDfhsParams);
	

	//m_h264[CH]->pNetVideo->OpenVideo(m_devDfhsParams.m_devicePort);

	m_h264[CH]->h_stream = &m_devDfhsParams.m_devInfo[CH].m_deviceChn;

	{
		CString UrlStr="";
		UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
		m_Log.Add("�ط�������Ƶ%s �л��ɹ�%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
		m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����

		return UrlStr;
	}


	return UrlStr;


}
int CDfhsDlg::InitProxyServer(void * pParams)
{
			
	m_CudevOcx.SetProxyNatInfo(1, 0, (LPTSTR)"", 0, (LPTSTR)"",(LPTSTR)"");//�������Ϣ�����úϷ���Ч��

	int perConf_ret=m_CudevOcx.PersonalConfig(0x10, 1);
	if(perConf_ret!=0)
	{
		m_Log.Add("[CDfhsDlg::InitProxyServer]:fail to PersonalConfig,perConf_ret=%d\n",perConf_ret);
		TRACE("[CDfhsDlg::InitProxyServer]:fail to PersonalConfig,perConf_ret=%d\n",perConf_ret);
	}
	int connect_ret=m_CudevOcx.ConnectToServer((LPTSTR)"10.23.113.139",1722,(LPTSTR)"admin@ntdfhs.mpf",(LPTSTR)"134679");//�������Ϣ�����úϷ���Ч��
	if(!connect_ret)
	{
		m_Log.Add("[CDfhsDlg::InitProxyServer]:fail to ConnectToServer,connect_ret=%d\n",connect_ret);
		TRACE("[CDfhsDlg::InitProxyServer]:fail to ConnectToServer,connect_ret=%d\n",connect_ret);
	}
	
	return 0;
}

void CDfhsDlg::MySleep(int iDelay,int iCount)
{
	if(iCount > 25)
		Sleep(35);
	else
		Sleep(40);

	return;
}

void CDfhsDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
