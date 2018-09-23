// RtspDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "RtspDlg.h"

extern CWMV_TO_H264Dlg* Main;

static CRtspDlg* pRtspDlg=NULL;

// CRtspDlg 对话框

IMPLEMENT_DYNAMIC(CRtspDlg, CDialog)

CRtspDlg::CRtspDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRtspDlg::IDD, pParent)
{

	pRtspDlg=this;

	for(int i=0;i<16;i++)
	{
		memset(&m_dssRtsp[i], 0, sizeof(DSS_RTSP));
		memset(&m_devParams[i],0,sizeof(DEV_RTSP_PARAMS));

	}

	if(0 != loadRTSPLib(NULL))
	{
		m_Log.Add("[ERR]:loadRTSPLib error");
		exit(0);
	}

}

CRtspDlg::~CRtspDlg()
{
}

void CRtspDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRtspDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRtspDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRtspDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

int CRtspDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("Source%d",SourceCount);
	char Server[255];
	ZeroMemory(Server,255);
	GetPrivateProfileString("common",tmpStr,"",Server,255,thisDir);
	strcpy(m_devParams[nIndex].m_Url,Server);


	tmpStr.Format("Dstip%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_serverIp,dst);
	//m_MicrovideoParams.ServerIP=dst;

	tmpStr.Format("Dstpub%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_pubName,dst);
	//m_MicrovideoParams.PubName=dst;

	tmpStr.Format("Dststream%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[nIndex].m_streamNo=GetPrivateProfileInt("common",tmpStr,-1,thisDir);


	if (0 == strlen(m_devParams[nIndex].m_serverIp) || 0 == strlen(m_devParams[nIndex].m_pubName) 
		|| -1 == m_devParams[nIndex].m_streamNo)
	{
		m_Log.Add("config.ini文件中未能找全RTSP的Dstip%d,Dstpub%d,Dststream%d",SourceCount,
			SourceCount,
			SourceCount);

	}

	m_h264[SourceCount]=new CH264Stream;
	if(NULL==m_h264[SourceCount])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;RTSP分配内存时出现错误！",SourceCount);
	}

	m_h264[nIndex]->thisIndex = nIndex;
	m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);
	//m_h264[nIndex]->b_Decode = 1;

	m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_serverIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_pubName,nIndex);

	m_h264[nIndex]->m_devType = DEF_RTSP_DEV;
	SetParams(m_h264[nIndex],&m_devParams[nIndex]);



	m_Log.Add("初始化通道%d的配置: 参数为:Source%d:%s,Dstip%d:ServerIP:%s,Dstpub%d:PubName:%s, Dststream%d:StreamNumber:v%d",
			SourceCount,SourceCount,
			m_devParams[nIndex].m_Url,SourceCount,
			m_devParams[nIndex].m_serverIp,SourceCount,
			m_devParams[nIndex].m_pubName,SourceCount,
			m_devParams[nIndex].m_streamNo);

	return 0;
}
// CRtspDlg 消息处理程序

//对于其它的没有从INI配置文件中读取到的路，采用以下方式来处理。
//该处理过程只是创建实例，但该实例创建后并不马上参加运行
int CRtspDlg::SetDefaultParams(void * pParams)
{
	//return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream补全16的时候分配内存时出现错误！",SourceCount);
		}

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->b_Encode	=  (g_bUseTc==1?1:0);

		m_h264[SourceCount]->m_Info.s_PubServer.Format("%s",m_devParams[0].m_serverIp);// m_MicrovideoParams.ServerIP;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[0].m_pubName ,SourceCount);

		m_h264[SourceCount]->m_devType = DEF_RTSP_DEV;

	}
	return 0;
}

unsigned char const start_code[4] = {0x00, 0x00, 0x00, 0x01};
void metaCallFunc(unsigned char *data,int datasize, int framewidth ,int frameheight ,  MU_INT64 timestamp, int keyflag,  VIDEOFRAMETYPE frametype ,unsigned char *extra_data ,int extra_data_size,   long lUserParam)
{
	CH264Stream* pH264=NULL;
	int nn = lUserParam;
	unsigned int nlen;
	//TRACE("user:%d\n",nn);
	//TRACE("w:%d H:%d  key:%d user:%d\n",framewidth,frameheight,keyflag,nn);

	for(int i = 0; i < 16; i++)
	{
		if((int)&pRtspDlg->m_dssRtsp[i] == (int)lUserParam)
		{
			pH264=pRtspDlg->m_h264[i];
			break;
		}
	}

	if(pH264 == NULL)
		return;

	pH264->b_PullLive = 0;

	if(1 == pH264->b_Encode)
	{
		if(pRtspDlg->m_devParams[pH264->thisIndex].b_decoder == 0)
		{
			pH264->initDeH264(framewidth,frameheight);
			pRtspDlg->m_devParams[pH264->thisIndex].b_decoder = 1;
		}

		DWORD yuvLen = 0;

		if(keyflag != 1)
		{
			//nlen = 4+datasize;
			//fwrite(&nlen,4,1,outputfile);
			memcpy(pH264->decodebuff,start_code,4);
			//fwrite(start_code,4,1, outputfile );
			//fwrite(data, datasize, 1, outputfile);	
			memcpy(pH264->decodebuff+4,data,datasize);

			datasize += 4;
		}else
		{
			//nlen = extra_data_size+4+datasize;
			//fwrite(&nlen,4,1,outputfile);

			//fwrite(extra_data, extra_data_size, 1, outputfile );
			memcpy(pH264->decodebuff,extra_data,extra_data_size);
			//nlen = ;
			//fwrite(&nlen,4,1,outputfile);
			//fwrite(start_code,4,1, outputfile );
			memcpy(pH264->decodebuff+extra_data_size,start_code,4);
			//fwrite(data, datasize, 1, outputfile);
			memcpy(pH264->decodebuff+extra_data_size+4,data,datasize);
			datasize +=(extra_data_size+4);
		}

		int r = pH264->decoderH264(pH264->decodebuff,datasize,pH264->decodebuff,&yuvLen);
		if(r<0 || yuvLen == 0)
			return;

		if(pH264->p_YuvArray.GetSize()<60)
		{

			MyAvFrame *tmp_Frame=new MyAvFrame(pH264->c2->width,pH264->c2->height);
			if (NULL == tmp_Frame)
			{
				TRACE("myVideo函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");
				//pMain->m_streamlock.Unlock();
				return ;
			}
			if (NULL == tmp_Frame->p_data)
			{
				//pMain->m_streamlock.Unlock();
				return ;
			}

			memcpy(tmp_Frame->p_data,pH264->decodebuff,yuvLen);

			//memcpy(tmp_Frame->p_data,pbuff,i_Len);
			pH264->m_streamlock.Lock();
			pH264->p_YuvArray.Add(tmp_Frame);
			pH264->m_streamlock.Unlock();

		}else
		{
			TRACE("descart this time %d\n",pH264->p_YuvArray.GetSize());
		}

		return;


	}



	//

	//if (false == pH264->b_start)
	//{
	//	pH264->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
	//	//在StartPush中设置宽度和高度等信息，再启线程进入pushstream（）
	//	//在pushstream()实现把把从模板获取的视频数据编码；保存到指定结构体pMain->g_rtmp
	//	pH264->b_start=true;
	//}
#if 1
	if(pH264->p_EncodeArray.GetSize()<60)
	{
		if(keyflag != 1)
		{
			nlen = 4+datasize;
		}else
		{
			nlen = extra_data_size+4+datasize;
		}

		MyAvFrame *tmp_Frame=new MyAvFrame(nlen,1);
		if (NULL == tmp_Frame)
		{
			TRACE("myVideo函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");
			//pMain->m_streamlock.Unlock();
			return ;
		}
		if (NULL == tmp_Frame->p_data)
		{
			//pMain->m_streamlock.Unlock();
			return ;
		}

		if(keyflag != 1)
		{
			//nlen = 4+datasize;
			//fwrite(&nlen,4,1,outputfile);
			memcpy(tmp_Frame->p_data,start_code,4);
			//fwrite(start_code,4,1, outputfile );
			//fwrite(data, datasize, 1, outputfile);	
			memcpy(tmp_Frame->p_data+4,data,datasize);
		}else
		{
			//nlen = extra_data_size+4+datasize;
			//fwrite(&nlen,4,1,outputfile);

			//fwrite(extra_data, extra_data_size, 1, outputfile );
			memcpy(tmp_Frame->p_data,extra_data,extra_data_size);
			//nlen = ;
			//fwrite(&nlen,4,1,outputfile);
			//fwrite(start_code,4,1, outputfile );
			memcpy(tmp_Frame->p_data+extra_data_size,start_code,4);
			//fwrite(data, datasize, 1, outputfile);
			memcpy(tmp_Frame->p_data+extra_data_size+4,data,datasize);
		}

		//memcpy(tmp_Frame->p_data,pbuff,i_Len);
		pH264->m_streamlock.Lock();
		pH264->p_EncodeArray.Add(tmp_Frame);
		pH264->m_streamlock.Unlock();

	}else
	{
		TRACE("descart this time %d\n",pH264->p_EncodeArray.GetSize());
	}

#endif
}

UINT _stdcall rtspPullStream(void* p)
{
	int ret;
	DSS_RTSP *p_st = (DSS_RTSP *)p;
	
	ret =  dss_live555_pullinit( );
	ret =  dss_live555_pullstart(p_st, metaCallFunc,  NULL);
//	ret = dss_live555_pullclose();

	return 0;
}

void CRtspDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();

	//dss_live555_pullinit();


	memset(&m_dssRtsp[0],0,sizeof(DSS_RTSP));
	strcpy(m_dssRtsp[0].rtspUrl ,"rtsp://192.168.0.161:4502/live/pub118");

	m_h264[0]=new CH264Stream;
	if(NULL==m_h264[0])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV分配内存时出现错误！",0);
	}

	m_h264[0]->thisIndex = 0;
	m_h264[0]->b_Encode	= 0;

	m_h264[0]->m_Info.s_PubServer="192.168.0.161";
	m_h264[0]->m_Info.s_CH.Format("%d",0);
	m_h264[0]->m_Info.s_PubName.Format("%s_v%d","test",0);

	//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
	m_h264[0]->m_devType = DEF_RTSP_DEV;


	//mainParam.s_Channel = ;"rtsp://127.0.0.1:8554/a.sdp"
	//strcpy(mainParam[0].rtspUrl ,"rtsp://192.168.0.161:4502/live/va");
	//strcpy(mainParam[0].s_Channel,"rtsp://192.168.100.200:7070/fg.sdp");

	//ret =  dss_live555_pullstart(&mainParam[0], metaCallFunc,  NULL);

	HANDLE m_hLoopThread=NULL;
	unsigned int threadid;

	m_hLoopThread=(HANDLE)_beginthreadex(NULL,0,rtspPullStream,&m_dssRtsp[0],CREATE_SUSPENDED,&threadid);

	SetThreadPriority(m_hLoopThread, THREAD_PRIORITY_ABOVE_NORMAL);

	ResumeThread(m_hLoopThread);
}


int CRtspDlg::SetParams(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_RTSP_PARAMS *prtspParams = (DEV_RTSP_PARAMS *)pParams;
	int ret;

	pH264Stream->m_Info.s_DevID.Format("%s",prtspParams->m_Url);// =pMicrovideoParams->SrcUrl;

	if (false ==pH264Stream->b_start)
	{
		
		pH264Stream->first=true;
		//pH264->StartPush(pFrameInfo->nWidth,pFrameInfo->nHeight);
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);

	return 0;
}

int CRtspDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_RTSP_PARAMS *prtspParams = &m_devParams[pH264Stream->thisIndex];

	//
	//pH264Stream->m_Info.s_PubServer.Format("%s",prtspParams->m_serverIp);//=pMicrovideoParams->ServerIP;
	//pH264Stream->m_Info.s_CH.Format("%d",prtspParams->m_streamNo);
	//CString tmpstr="";
	//tmpstr.Format("%s_v%d",DEF_CS_RTSP_FLAG,prtspParams->m_streamNo);
	//pH264Stream->m_Info.s_PubName=tmpstr;

	HANDLE m_hLoopThread=NULL;
	unsigned int threadid;

	strcpy(m_dssRtsp[prtspParams->m_streamNo].rtspUrl,prtspParams->m_Url);

	m_hLoopThread=(HANDLE)_beginthreadex(NULL,0,rtspPullStream,&m_dssRtsp[prtspParams->m_streamNo],CREATE_SUSPENDED,&threadid);

	SetThreadPriority(m_hLoopThread, THREAD_PRIORITY_ABOVE_NORMAL);

	ResumeThread(m_hLoopThread);

	pH264Stream->h_stream = m_hLoopThread;

	return 0;
}

int CRtspDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;

	dss_live555_pullstop( &m_dssRtsp[pH264Stream->thisIndex]);

	WaitForSingleObject(pH264Stream->h_stream,INFINITE);

	CloseHandle( pH264Stream->h_stream );

	if(1 == m_devParams[pH264Stream->thisIndex].b_decoder)
	{
		pH264Stream->destroyDeH264();
	}

	m_devParams[pH264Stream->thisIndex].b_decoder = 0;

	pH264Stream->h_stream = NULL;

	return 0;
}

void CRtspDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//dss_live555_pullstop( &m_dssRtsp[0]);
}
