// MicrovideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "MicrovideoDlg.h"

extern CWMV_TO_H264Dlg* Main;
// CMicrovideoDlg 对话框

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


// CMicrovideoDlg 消息处理程序

void CMicrovideoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
			m_Log.Add("达到最大转码路数，以下配置停止分析。%d：%d",SourceCount,MAX_ONLINE);
			break;
		}

		if(SourceCount >= MaxLocTsk)
		{
			m_Log.Add("达到MaxLocTsk，以下配置停止分析");
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
			m_Log.Add("config.ini文件中未能找到南京感动的Source%d",SourceCount);
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
				m_Log.Add("m_h264[%d]=new CH264Stream;南京感动分配内存时出现错误！",SourceCount);
			}
			m_h264[SourceCount]->thisIndex = SourceCount;
			m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;

			SetParams(m_h264[SourceCount],&m_MicrovideoParams);
			StartPull(m_h264[SourceCount],&m_MicrovideoParams);

		}//end SrcUrl!=NULL
	

		m_Log.Add("初始化通道%d的配置: 参数为:Source%d:%s,Dstip%d:ServerIP:%s, Dstport%d:port:v%d",
			SourceCount,SourceInd,
			m_MicrovideoParams.SrcUrl,SourceInd,
			m_MicrovideoParams.ServerIP,SourceInd,
			m_MicrovideoParams.nPort);//张世俊增加
	
		SourceCount++;
		g_regDetWay=SourceCount;
		SourceInd++;
		
	}//end while true

	return 0;
}

//对于其它的没有从INI配置文件中读取到的路，采用以下方式来处理。
//该处理过程只是创建实例，但该实例创建后并不马上参加运行
int CMicrovideoDlg::SetDefaultParams(void * pParams)
{
	//return 0;
	int i=0;
	for(;SourceCount<MAX_ONLINE;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream补全16的时候分配内存时出现错误！",SourceCount);
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
函数名称：my_trace
函数描述: callback of Pull video
函数调用：CMicrovideoDlg::SetParams(void *pStream,void * pParams)
全局变量：
参数1:msg
参数2:msgType
参数3:CH264Stream pointer
返 回 值:
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18     V1.0        jicheng         创建
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
函数名称：myVideo
函数描述: callback of Pull video
函数调用：CMicrovideoDlg::SetParams(void *pStream,void * pParams)
全局变量：
参数1:point to AVPictureOut
参数2:point to CH264Stream
返 回 值:
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18     V1.0        jicheng         创建
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
			m_Log.Add("myVideo函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");
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
		//TRACE("[myVideo]:数组过大，%d\n",pMain->p_Array.GetSize());
	}
	//pMain->m_streamlock.Unlock();
	return TRUE;
}
/*************************************************************************************
函数名称：str_to_int
函数描述: transfer string to int
函数调用：ParSrc
全局变量：
参数1:address of the string 
参数2:lenth of the string
返 回 值: the int data transfered to
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
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
函数名称：ParSrc
函数描述: parse source given in config
函数调用：CMicrovideoDlg::SetParams(void *pStream,void * pParams)
全局变量：
参数1:point to CH264Stream
返 回 值:0 as success;-1 as fail 
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18     V1.0        jicheng         创建
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

	//first=true;放在发送线程开启后设置。

	StreamParams[pH264Stream->thisIndex].Audio_Delay_Packet =4;				//声音延迟播放多少个包，一般设为2－4；
	StreamParams[pH264Stream->thisIndex].Audio_on_flag = 0;					//打开声音线程
	StreamParams[pH264Stream->thisIndex].Video_on_flag = 1;					//打开视频线程
	StreamParams[pH264Stream->thisIndex].Audio_is_Play = 0;					//是否播放声音
	StreamParams[pH264Stream->thisIndex].balance =50; //500;//0;						//视频实时性和流畅性平衡值（50－600），600实时性强，50流畅性好

	StreamParams[pH264Stream->thisIndex].hWnd =NULL;//GetSafeHwnd();						//窗口句柄
	StreamParams[pH264Stream->thisIndex].TimeAdjustFlag =1;					//是否根据时间戳调整播放时间

	StreamParams[pH264Stream->thisIndex].VideoCallBack =myVideo;			//视频解码回调函数：每解出一帧视频会回调此函数；不用设为NULL

	StreamParams[pH264Stream->thisIndex].CallBackParams = (VOID *)pH264Stream;//portNum;//视频回调函数的参数
	StreamParams[pH264Stream->thisIndex].WarnningMsgFunc =my_trace;//warn;		//告警信息回调函数；不用设为NULL

	StreamParams[pH264Stream->thisIndex].VideoFormat =TYPE_YV12;//TYPE_YUY2;//TYPE_RGB24;//TYPE_NV12;//TYPE_RGB24;//TYPE_IMC4;//TYPE_UYVY;//			//视频解码后的图像格式
	StreamParams[pH264Stream->thisIndex].VideoRend_flag = 0;				//是否显示视频图像

	StreamParams[pH264Stream->thisIndex].receiveBuffSize = 512*1024;		//接收流的缓冲区大小
	if(NULL == StreamParams[pH264Stream->thisIndex].Url_Name)
		StreamParams[pH264Stream->thisIndex].Url_Name=new char[255];
	ZeroMemory(StreamParams[pH264Stream->thisIndex].Url_Name,255);
	sprintf(StreamParams[pH264Stream->thisIndex].Url_Name,"%s",pMicrovideoParams->SrcUrl);				//流的URL地址


	StreamParams[pH264Stream->thisIndex].videoDisplayRect.top =0;			//设为0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.left = 0;			//设为0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.right = 0;		//设为0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.bottom = 0;		//设为0
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

// 启动拉流函数
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

