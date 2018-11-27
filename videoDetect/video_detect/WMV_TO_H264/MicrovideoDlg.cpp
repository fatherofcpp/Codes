// MicrovideoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "MicrovideoDlg.h"

extern CWMV_TO_H264Dlg* Main;
// CMicrovideoDlg �Ի���

IMPLEMENT_DYNAMIC(CMicrovideoDlg, CDialog)

CMicrovideoDlg::CMicrovideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMicrovideoDlg::IDD, pParent)
{
	for(int i=0;i<MAX_ONLINE;i++)
	{
		memset(&StreamParams[i], 0, sizeof(StreamParams[i]));
	
		m_h264[i] = NULL;
	}
	SourceInd=0;
}

CMicrovideoDlg::~CMicrovideoDlg()
{
	for(int i=0;i<MAX_ONLINE;i++)
	{
		if(NULL != m_h264[i])
		{
			//if(NULL != m_h264[i]->encodebuff)
			//	delete[] m_h264[i]->encodebuff;
			DWORD code; 
			//while(1)
			{
#if 0
				if(NULL == m_h264[i]->m_hSendThread)
					break;
				WaitForSingleObject(m_h264[i]->m_hSendThread,INFINITE);
				CloseHandle( m_h264[i]->m_hSendThread );

				int ret = GetExitCodeThread(m_h264[i]->m_hSendThread, &code); 
				if(code == STILL_ACTIVE) 
				{  
					Sleep(1000);
					//TerminateThread(m_h264[i]->m_hSendThread,   0); 
				}else
					break;
#endif
			}

			if(NULL != StreamParams[i].Url_Name)
				delete []StreamParams[i].Url_Name;

			delete m_h264[i];
		}
	}
}

void CMicrovideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMicrovideoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMicrovideoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMicrovideoDlg ��Ϣ�������

void CMicrovideoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

int CMicrovideoDlg::GetInforFromIni(void * pThisDir)
{

	char tmp[255];

	CString tmpStr;
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	SourceCount=0;
	g_regDetWay=SourceCount;

	while(true)
	{
		if(SourceCount >= MAX_ONLINE)
		{
			m_Log.Add("�ﵽ���ת��·������������ֹͣ������%d��%d",SourceCount,MAX_ONLINE);
			break;
		}

		if(SourceCount >= MaxLocTsk)
		{
			m_Log.Add("�ﵽMaxLocTsk����������ֹͣ����");
			break;
		}

		tmpStr.Format("Source%d",SourceCount);   //single detect   0-32

		char Server[255];
		ZeroMemory(Server,255);
		GetPrivateProfileString("mircovideo",tmpStr,"",Server,255,thisDir);
		m_MicrovideoParams.SrcUrl=Server;

		if(PRINT){
			TRACE("...................[GetInforFromIni]----tmpStr:%s,SourceCount=%d,srcUrl=%s\n",tmpStr,SourceCount,Server);
		}

		if("" == m_MicrovideoParams.SrcUrl)
		{
			m_Log.Add("config.ini�ļ���δ���ҵ��Ͼ��ж���Source%d",SourceCount);
			SourceInd=0;
			break;
		}
		else//SrcUrl!=NULL
		{
			m_MicrovideoParams.ServerIP=Dstip;
			m_MicrovideoParams.nPort=Dstport;

			m_h264[SourceCount]=new CH264Stream;
			if(NULL==m_h264[SourceCount])
			{
				m_Log.Add("m_h264[%d]=new CH264Stream;�Ͼ��ж������ڴ�ʱ���ִ���",SourceCount);
			}
			m_h264[SourceCount]->thisIndex = SourceCount;
			m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;

			SetParams(m_h264[SourceCount],&m_MicrovideoParams);
			StartPull(m_h264[SourceCount],&m_MicrovideoParams);

		}//end SrcUrl!=NULL
	

		m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:Source%d:%s,Dstip%d:ServerIP:%s, Dstport%d:port:v%d",
			SourceCount,SourceInd,
			m_MicrovideoParams.SrcUrl,SourceInd,
			m_MicrovideoParams.ServerIP,SourceInd,
			m_MicrovideoParams.nPort);//����������
	
		SourceCount++;
		g_regDetWay=SourceCount;
		SourceInd++;
		
	}//end while true

	return 0;
}

//����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
//�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
int CMicrovideoDlg::SetDefaultParams(void * pParams)
{
	//return 0;
	int i=0;
	for(;SourceCount<MAX_ONLINE;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream��ȫ16��ʱ������ڴ�ʱ���ִ���",SourceCount);
		}

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;
	}

	for(i=MaxLocTsk;i<MaxLocTsk+MaxNetTsk;i++)//when first NetTsk ,we check it
	{
		m_h264[i]->parTskEnd=1;
		m_h264[i]->senTskEnd=1;
		m_h264[i]->serStaEnd=1;
	}
	return 0;
}

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20
/*************************************************************************************
�������ƣ�my_trace
��������: callback of Pull video
�������ã�CMicrovideoDlg::SetParams(void *pStream,void * pParams)
ȫ�ֱ�����
����1:msg
����2:msgType
����3:CH264Stream pointer
�� �� ֵ:
����ʱ�䣺2013-12-18
�޸�����        �汾��     �޸���           �޸�����
-------------------------------------------------
2013-12-18     V1.0        jicheng         ����
**************************************************************************************/
BOOL my_trace(CHAR *msg,DWORD msgType,VOID *params)
{
	CH264Stream* p_H264Stream=(CH264Stream*)params;
	if (msg!=NULL)// && ((msgType &0x80000000)!= 0))
	{
		//TRACE(msg);
	}

	if(0x00000700 == msgType)//connect server fail
	{
		//server off
		//m_Log.Add("my_trace:0x00000700 == msgType,srcUrl:%s\n",p_H264Stream->SrcUrl);
		//p_H264Stream->ServerOff = 1;
	}

	if(0x80000401 == msgType)//connect server suc
	{
		//server on
		//m_Log.Add("my_trace:0x80000401 == msgType,srcUrl:%s\n",p_H264Stream->SrcUrl);
		//p_H264Stream->ServerOff=0;
	}

	return FALSE;

}

/*************************************************************************************
�������ƣ�myVideo
��������: callback of Pull video
�������ã�CMicrovideoDlg::SetParams(void *pStream,void * pParams)
ȫ�ֱ�����
����1:point to AVPictureOut
����2:point to CH264Stream
�� �� ֵ:
����ʱ�䣺2013-12-18
�޸�����        �汾��     �޸���           �޸�����
-------------------------------------------------
2013-12-18     V1.0        jicheng         ����
**************************************************************************************/
BOOL myVideo(AVPictureOut *pOut,VOID *params)
{
	CH264Stream* pMain=(CH264Stream*)params;
if(parseTest==1)
{
//	TRACE("---------myvideo %d",pMain->thisIndex);
	return TRUE;
}

	//pMain->m_streamlock.Lock();
	if(pMain->p_Array.GetSize()<25*2)
	{
		if (NULL ==pOut->data[0] || NULL == pOut->data[1] || NULL == pOut->data[2])
		{
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		if (0 == pOut->width || 0 == pOut->height)
		{
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		MyAvFrame *m_Frame=new MyAvFrame(pOut->width,pOut->height);
		if (NULL == m_Frame)
		{
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		if (NULL == m_Frame->p_data)
		{
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		memcpy(m_Frame->p_data,pOut->data[0],pOut->width*pOut->height);//Y
		memcpy(&m_Frame->p_data[pOut->width*pOut->height],pOut->data[1],pOut->width*pOut->height/4);
		memcpy(&m_Frame->p_data[pOut->width*pOut->height+(pOut->width*pOut->height)/4],pOut->data[2],pOut->width*pOut->height/4);

		memcpy(m_Frame->p_Ydata,pOut->data[0],pOut->width*pOut->height);//Y

		pMain->m_streamlock.Lock();
		pMain->p_Array.Add(m_Frame);
		pMain->m_streamlock.Unlock();
	}else
	{
		//TRACE("[myVideo]:�������%d\n",pMain->p_Array.GetSize());
	}
	//pMain->m_streamlock.Unlock();
	return TRUE;
}
/*************************************************************************************
�������ƣ�str_to_int
��������: transfer string to int
�������ã�ParSrc
ȫ�ֱ�����
����1:address of the string 
����2:lenth of the string
�� �� ֵ: the int data transfered to
����ʱ�䣺2014-1-14
�޸�����        �汾��     �޸���           �޸�����
-------------------------------------------------
2014-1-14      V1.0        jicheng         ����
**************************************************************************************/
int str_to_int(char* p, int len)
{
	int i = 0, ret = 0;
	while(i < len)
	{
		if ((p[i] >= '0') && (p[i] <= '9'))
		{
			int value = p[i]-48;
			ret = ret*10 +value;
			i++;
		}
		else
		  break;
	}
	return ret;
}
/*************************************************************************************
�������ƣ�ParSrc
��������: parse source given in config
�������ã�CMicrovideoDlg::SetParams(void *pStream,void * pParams)
ȫ�ֱ�����
����1:point to CH264Stream
�� �� ֵ:0 as success;-1 as fail 
����ʱ�䣺2013-12-18
�޸�����        �汾��     �޸���           �޸�����
-------------------------------------------------
2013-12-18     V1.0        jicheng         ����
**************************************************************************************/

int ParSrc(CH264Stream* p_H264Stream)
{
	//Source0=http://192.168.0.251:8080/ipcamera1
//	Source0=rtmp://10.20.133.119:1935/live/nlljk_142_38
	char*p1=NULL;
	char*p2=NULL;
	char*p3=NULL;
	char url[512];
	char ip[32];
	char name[64];
	memset(url,0,sizeof(url));
	memset(ip,0,sizeof(ip));
	memset(name,0,sizeof(name));
	sprintf(url,"%s",p_H264Stream->SrcUrl);
	p1=strstr(url,":");
	if(!p1){
		return -1;
	}
	p2=strstr(p1+1,":");
	if(!p2){
		return -1;
	}
	p3=strstr(p2,"/");
	if(!p3){
		return -1;
	}
	char *pKey = strstr(p3+1,"/");
	if(pKey != NULL)
		p3 = pKey;

	memcpy(ip,p1+3,p2-p1-3);
	//
	p_H264Stream->SrcPort=str_to_int(p2+1,p3-p2-1);
	strcpy(name,p3+1);
	p_H264Stream->SrcIp=ip;
	p_H264Stream->SrcNam=name;
	return 0;
}
int CMicrovideoDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;

	pH264Stream->SrcUrl=pMicrovideoParams->SrcUrl;
	pH264Stream->DstIp=pMicrovideoParams->ServerIP;
	pH264Stream->DstPort=pMicrovideoParams->nPort;
	ParSrc(pH264Stream);
//	TRACE("%s\n",pH264Stream->SrcUrl);

	//first=true;���ڷ����߳̿��������á�

	StreamParams[pH264Stream->thisIndex].Audio_Delay_Packet =4;				//�����ӳٲ��Ŷ��ٸ�����һ����Ϊ2��4��
	StreamParams[pH264Stream->thisIndex].Audio_on_flag = 0;					//�������߳�
	StreamParams[pH264Stream->thisIndex].Video_on_flag = 1;					//����Ƶ�߳�
	StreamParams[pH264Stream->thisIndex].Audio_is_Play = 0;					//�Ƿ񲥷�����
	StreamParams[pH264Stream->thisIndex].balance =50; //500;//0;						//��Ƶʵʱ�Ժ�������ƽ��ֵ��50��600����600ʵʱ��ǿ��50�����Ժ�

	StreamParams[pH264Stream->thisIndex].hWnd =NULL;//GetSafeHwnd();						//���ھ��
	StreamParams[pH264Stream->thisIndex].TimeAdjustFlag =1;					//�Ƿ����ʱ�����������ʱ��

	StreamParams[pH264Stream->thisIndex].VideoCallBack =myVideo;			//��Ƶ����ص�������ÿ���һ֡��Ƶ��ص��˺�����������ΪNULL

	StreamParams[pH264Stream->thisIndex].CallBackParams = (VOID *)pH264Stream;//portNum;//��Ƶ�ص������Ĳ���
	StreamParams[pH264Stream->thisIndex].WarnningMsgFunc =my_trace;//warn;		//�澯��Ϣ�ص�������������ΪNULL

	StreamParams[pH264Stream->thisIndex].VideoFormat =TYPE_YV12;//TYPE_YUY2;//TYPE_RGB24;//TYPE_NV12;//TYPE_RGB24;//TYPE_IMC4;//TYPE_UYVY;//			//��Ƶ������ͼ���ʽ
	StreamParams[pH264Stream->thisIndex].VideoRend_flag = 0;				//�Ƿ���ʾ��Ƶͼ��

	StreamParams[pH264Stream->thisIndex].receiveBuffSize = 512*1024;		//�������Ļ�������С
	if(NULL == StreamParams[pH264Stream->thisIndex].Url_Name)
		StreamParams[pH264Stream->thisIndex].Url_Name=new char[255];
	ZeroMemory(StreamParams[pH264Stream->thisIndex].Url_Name,255);
	sprintf(StreamParams[pH264Stream->thisIndex].Url_Name,"%s",pMicrovideoParams->SrcUrl);				//����URL��ַ


	StreamParams[pH264Stream->thisIndex].videoDisplayRect.top =0;			//��Ϊ0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.left = 0;			//��Ϊ0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.right = 0;		//��Ϊ0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.bottom = 0;		//��Ϊ0
if(parseTest==0)
{
	if (false ==pH264Stream->b_start)
	{
		m_Log.Add("[SetParams]:thisIndex:%d,b_start:%d,before StartPush\n",pH264Stream->thisIndex,pH264Stream->b_start);
		pH264Stream->first=true;
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}
}
	return 0;
}

// ������������
int CMicrovideoDlg::StartPull(void *pStream,void* pParams)
{
	//return 0;

	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;
		//m_ffmpeglock.Lock();
	BOOL status=FALSE;


	if (NULL!=pH264Stream->h_stream)
	{
		try
		{
			ProcessClose(pH264Stream->h_stream);
		}
		catch (CMemoryException* e)
		{
			
		}
		catch (CFileException* e)
		{

		}
		catch (CException* e)
		{

		}
		
		pH264Stream->h_stream=NULL;
	}

	try
	{
		pH264Stream->h_stream = ProcessOpen(&StreamParams[pH264Stream->thisIndex],&status);

		ProcessStart(pH264Stream->h_stream);
	}
	catch (CMemoryException* e)
	{

	}
	catch (CFileException* e)
	{

	}
	catch (CException* e)
	{

	}
	
	return 0;
}

int CMicrovideoDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;

	if (NULL!=pH264Stream->h_stream)
	{
		try
		{
			ProcessClose(pH264Stream->h_stream);
		}
		catch (CMemoryException* e)
		{

		}
		catch (CFileException* e)
		{
		}
		catch (CException* e)
		{
		}

		pH264Stream->h_stream=NULL;
	}

	return 0;
}

