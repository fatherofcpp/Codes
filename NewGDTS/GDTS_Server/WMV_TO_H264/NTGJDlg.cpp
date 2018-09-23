// NTGJDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "NTGJDlg.h"


extern CWMV_TO_H264Dlg* Main;
static CNTGJDlg* pNtgjDlg=NULL;

// CNTGJDlg 对话框

IMPLEMENT_DYNAMIC(CNTGJDlg, CDialog)

CNTGJDlg::CNTGJDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNTGJDlg::IDD, pParent)
{
//	pNtgjDlg = this;

//	SourceCount=0;
	//GetInforFromIni()
	for(int i=0;i<16;i++)
	{
	//	m_h264[i] = NULL;
	//	memset(&m_h3cParams[i],0,sizeof(DEV_H3C_PARAMS));
	}

	ULONG ulRet;

//	loadH3cLib();


//	ulRet = IMOS_Initiate("N/A", 8800, BOOL_TRUE, BOOL_TRUE);//IMOS_Initiate(szServerAdd, 8800, BOOL_TRUE, BOOL_TRUE);
//	if (ERR_COMMON_SUCCEED != ulRet)
//	{
//		//log
//		m_Log.Add("IMOS SDK初始化");
//		return;
//	}



}

CNTGJDlg::~CNTGJDlg()
{
	for(int i=0;i<16;i++)
	{
		if(NULL != m_h264[i])
		{
			//if(NULL != m_h264[i]->encodebuff)
			//	delete[] m_h264[i]->encodebuff;
			DWORD code; 
			//while(1)
			{
#if 0
				if(NULL != m_h264[i]->m_hPullThread)
				{
					WaitForSingleObject(m_h264[i]->m_hPullThread,INFINITE);
					CloseHandle( m_h264[i]->m_hPullThread );
				}

				int ret = GetExitCodeThread(m_h264[i]->m_hPullThread, &code); 
				if(code == STILL_ACTIVE) 
				{  
					Sleep(1000);
					//TerminateThread(m_h264[i]->m_hPullThread,   0); 
				}else
					break;
#endif
			}


			delete m_h264[i];
		}
	}
}

void CNTGJDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NETVIDEOCTRL1, m_netVideo[0]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL2, m_netVideo[1]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL3, m_netVideo[2]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL4, m_netVideo[3]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL5, m_netVideo[4]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL6, m_netVideo[5]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL7, m_netVideo[6]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL8, m_netVideo[7]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL9, m_netVideo[8]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL10, m_netVideo[9]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL11, m_netVideo[10]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL12, m_netVideo[11]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL13, m_netVideo[12]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL14, m_netVideo[13]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL15, m_netVideo[14]);
	DDX_Control(pDX, IDC_NETVIDEOCTRL16, m_netVideo[15]);
}


BEGIN_MESSAGE_MAP(CNTGJDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNTGJDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNTGJDlg 消息处理程序
BEGIN_EVENTSINK_MAP(CNTGJDlg, CDialog)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL1, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl1, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL2, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl2, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL3, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl3, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL4, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl4, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL5, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl5, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL6, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl6, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL7, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl7, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL8, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl8, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL9, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl9, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL10, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl10, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL11, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl11, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL12, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl12, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL13, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl13, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL14, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl14, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL15, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl15, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
ON_EVENT(CNTGJDlg, IDC_NETVIDEOCTRL16, 18, CNTGJDlg::OnVideoDataMessageNetvideoctrl16, VTS_I4 VTS_I4 VTS_I4 VTS_I8 VTS_PUI1 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CNTGJDlg::OnVideoDataMessageNetvideoctrl1(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	//CH264Stream* Main=(CH264Stream*)m_h264[0];

	PushData2Buffer(m_h264[0],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
	//TRACE("recv the ocx %d len is %d timestamp is %ld\n",0,lFrameLen,ullPts);

}

void CNTGJDlg::PushData2Buffer(void* pCstream,long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	CH264Stream* pMain=(CH264Stream*)pCstream;
	//add 
	pMain->b_PullLive = 0;

	//单一性，针对性 只能一个对象有效
	int bFull = 0;
#if 0 //由于无直播流的加入，该处的发送线程启动有时要提前运行，估复制一份在对象的初始化时完成
	if (false ==pMain->b_start)
	{
		pMain->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		//在StartPush中设置宽度和高度等信息，再启线程进入pushstream（）
		//在pushstream()实现把把从模板获取的视频数据编码；保存到指定结构体pMain->g_rtmp
		pMain->b_start=true;
	}
#endif
	do{
		if(bFull == 1)
			Sleep(10);
		pMain->m_streamlock.Lock();
		if(pMain->p_EncodeArray.GetSize()<200)
		{
			if (NULL ==byteVideoData)
			{
				m_Log.Add("视频数据为空！");
				pMain->m_streamlock.Unlock();
				return ;
			}
			if (0 == lFrameLen )
			{
				m_Log.Add("视频数据为空！");
				pMain->m_streamlock.Unlock();
				return ;
			}

			MyAvFrame *m_Frame=NULL;
			//if(lFrameLen<(352*288*3/2+1))//由于无法判断其大小，估先设置其当前大小作为该次数据量大小
			//	m_Frame = new MyAvFrame(352,288);//默认设置大小 CIF标准
			//else
			m_Frame = new MyAvFrame(lFrameLen,1);//暂无法判断出视频大小


			if (NULL == m_Frame)
			{
				m_Log.Add("new MyAvFrame的时候内存分配错误！");
				pMain->m_streamlock.Unlock();
				return ;
			}
			//add  季成
			if(0 == pMain->curTime)
			{
				pMain->curTime = ullPts;//gettick
				m_Frame->i_time = 0;
				//		TRACE("input %d\n",m_Frame->i_time);
			}
			else
			{

				m_Frame->i_time = (ullPts-pMain->curTime)/1000;
				pMain->curTime = ullPts;
				//		TRACE("input %d\n",m_Frame->i_time);
			}


			/*
			if(0x0dc == (lFrameType&0x0dc))
			{
			m_Frame->i_key = 1;

			}else
			m_Frame->i_key = 0;
			*/
			if (NULL == m_Frame->p_data)
			{
				m_Log.Add("分配到的MyAvFrame里的p_data指针为空！");
				pMain->m_streamlock.Unlock();
				return ;
			}
			memcpy(m_Frame->p_data,byteVideoData,lFrameLen);//new MyAvFrame(352,288)<--byteVideoData

			pMain->p_EncodeArray.Add(m_Frame);//视频数据存进待编码数据缓冲区CArray<MyAvFrame*,MyAvFrame*>
			bFull = 0;
		}else 
			bFull = 1;

		pMain->m_streamlock.Unlock();
	}while(bFull == 1);

}

void CNTGJDlg::OnVideoDataMessageNetvideoctrl2(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[1],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
	//TRACE("recv the ocx %d len is %d\n",1,lFrameLen);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl3(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[2],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
	//TRACE("recv the ocx %d len is %d\n",2,lFrameLen);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl4(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[3],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
	//TRACE("recv the ocx %d len is %d\n",3,lFrameLen);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl5(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[4],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl6(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[5],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl7(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[6],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl8(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[7],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl9(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[8],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl10(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[9],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl11(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[10],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnVideoDataMessageNetvideoctrl12(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[11],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}void CNTGJDlg::OnVideoDataMessageNetvideoctrl13(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[12],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}void CNTGJDlg::OnVideoDataMessageNetvideoctrl14(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[13],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}void CNTGJDlg::OnVideoDataMessageNetvideoctrl15(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[14],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}void CNTGJDlg::OnVideoDataMessageNetvideoctrl16(long lFrameType, long lStreamType, long lFrameLen, LONGLONG ullPts, unsigned char* byteVideoData, LPUNKNOWN pUser)
{
	// TODO: 在此处添加消息处理程序代码
	PushData2Buffer(m_h264[15],lFrameType,lStreamType,lFrameLen,ullPts,byteVideoData,pUser);
}
void CNTGJDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

int CNTGJDlg::GetInforFromIni(void * pThisDir)
{

	char tmp[255];

	CString tmpStr;
	CString SourceStr="";
	CString ServerIP="";
	CString PubName="";
	int StreamNumber=0;

	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	

	while(true)//读取config.ini中所有shsl,对应new CH264Stream
	{
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("达到最大转码路数，以下配置停止分析。%d：%d",SourceCount,g_maxLiveNum);
			break;
		}

		tmpStr.Format("ProxyIp%d",SourceCount);
		
		ZeroMemory(tmp,255);
		GetPrivateProfileString("shsl",tmpStr,"",tmp,255,thisDir);
		m_devShslParams.m_proxyIp = tmp;
		if("" == m_devShslParams.m_proxyIp)
		{

			m_Log.Add("config.ini文件中未能找到上海三利的ProxyIp%d",SourceCount);

			break;
		}
		//如何判断文件中是否存在m_proxyPort
		tmpStr.Format("ProxyPort%d",SourceCount);
		m_devShslParams.m_proxyPort=GetPrivateProfileInt("shsl",tmpStr,0,thisDir);


		tmpStr.Format("DeviceId%d",SourceCount);
		ZeroMemory(tmp,255);
		GetPrivateProfileString("shsl",tmpStr,"",tmp,255,thisDir);
		m_devShslParams.m_deviceId = tmp;
		if("" == m_devShslParams.m_deviceId)
		{
			m_Log.Add("config.ini文件中未能找到上海三利的DeviceId%d",SourceCount);
			break;
		}

		tmpStr.Format("DevicePort%d",SourceCount);
		m_devShslParams.m_devicePort=GetPrivateProfileInt("shsl",tmpStr,-1,thisDir);

		tmpStr.Format("DeviceName%d",SourceCount);
		ZeroMemory(tmp,255);
		GetPrivateProfileString("shsl",tmpStr,"",tmp,255,thisDir);
		m_devShslParams.m_deviceName = tmp;


		TRACE("proxyIp:%s ProxyPort:%d DeviceId:%s devicePort:%d\n",m_devShslParams.m_proxyIp,m_devShslParams.m_proxyPort,
			m_devShslParams.m_deviceId,m_devShslParams.m_devicePort);


		tmpStr.Format("Dstip%d",SourceCount);
		char dst[255];
		ZeroMemory(dst,255);
		GetPrivateProfileString("shsl",tmpStr,"",dst,255,thisDir);
		ServerIP=dst;

		tmpStr.Format("Dstpub%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString("shsl",tmpStr,"",dst,255,thisDir);
		PubName=dst;

		tmpStr.Format("Dststream%d",SourceCount);
		ZeroMemory(dst,255);
		StreamNumber=GetPrivateProfileInt("shsl",tmpStr,-1,thisDir);


		if ("" == ServerIP || "" == PubName || -1 == StreamNumber)
		{
			m_Log.Add("config.ini文件中未能找全上海三利的Dstip%d,Dstpub%d,Dststream%d",SourceCount,SourceCount,SourceCount);
			break;
		}
		else
		{
			//for(int i =0;i <16;i++)
			{//根据实际情况进行调整
				//SourceCount = StreamNumber = i;
				m_h264[SourceCount]=new CH264Stream;
				if(NULL==m_h264[SourceCount])
				{
					m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV分配内存时出现错误！",SourceCount);
				}

				m_h264[SourceCount]->thisIndex = SourceCount;

				m_h264[SourceCount]->m_Info.s_PubServer=ServerIP;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",StreamNumber);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",DEF_CS_NTGJ_FLAG,StreamNumber);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
				m_h264[SourceCount]->m_devType = DEF_SHSL_DEV;
				//m_h264[SourceCount]->Init(&m_devShslParams,this->GetSafeHwnd());
				//m_h264[SourceCount]->StartGet(m_devShslParams.m_devicePort);
				SetParams(m_h264[SourceCount],&m_devShslParams);
				StartPull(m_h264[SourceCount],&m_devShslParams);
				//在StartGet()已经pNetVideo->OpenVideo(chn),
				//只需http_poll( httpd, &timeout )端口服务器即可观看
			}

		}
		SourceCount ++;
	}
	m_Log.Add("解析到通道%d: 参数为:proxyIp:%s ProxyPort:%d DeviceId:%s devicePort:%d\n",SourceCount,m_devShslParams.m_proxyIp,m_devShslParams.m_proxyPort,
		m_devShslParams.m_deviceId,m_devShslParams.m_devicePort);//张世俊增加

	return 0;
}

int CNTGJDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("ProxyIp%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devShslParams.m_proxyIp=dst;


	tmpStr.Format("ProxyPort%d",nIndex);
	ZeroMemory(dst,255);
	m_devShslParams.m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_devShslParams.m_proxyPort:%d\n",m_devShslParams.m_proxyPort);


	tmpStr.Format("UserName%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devShslParams.m_userName=dst;
	TRACE("m_devShslParams.m_userName:%s\n",m_devShslParams.m_userName);


	tmpStr.Format("PassWd%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devShslParams.m_passWord=dst;
	TRACE("m_devShslParams.m_passWord:%s\n",m_devShslParams.m_passWord);

	//InitProxyServer(NULL);

	m_Log.Add("初始化通道的配置: 参数为:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		m_devShslParams.m_proxyIp,m_devShslParams.m_proxyPort,m_devShslParams.m_userName,
		m_devShslParams.m_passWord);
	
	tmpStr.Format("DeviceId%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devShslParams.m_deviceId=dst;

	tmpStr.Format("DevicePort%d",SourceCount);
	m_devShslParams.m_devicePort=GetPrivateProfileInt("shsl",tmpStr,-1,thisDir);

	tmpStr.Format("Dstip%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devShslParams.m_dstIp=dst;

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devShslParams.m_dstPub=dst;

	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_devShslParams.m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);


	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_H3CIPC_DEV分配内存时出现错误！",nIndex);
	}

	m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);


	m_h264[nIndex]->thisIndex = SourceCount;
	m_h264[nIndex]->m_Info.s_PubServer=m_devShslParams.m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",SourceCount);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devShslParams.m_dstPub,SourceCount);
	m_h264[nIndex]->m_devType = DEF_H3CIPC_DEV;

	SetParams(m_h264[nIndex],&m_devShslParams);
	//StartPull(m_h264[nIndex],&m_h3cParams[nIndex]);
	return 0;
}

//对于其它的没有从INI配置文件中读取到的路，采用以下方式来处理。
//该处理过程只是创建实例，但该实例创建后并不马上参加运行
#if 0//old 
int CNTGJDlg::SetDefaultParams(void * pParams)
{
	if(SourceCount>=g_maxLiveNum)//张世俊增加，补全到16路
		return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream补全16的时候分配内存时出现错误！",SourceCount);
		}

		m_h264[SourceCount]->thisIndex = SourceCount;

		m_h264[SourceCount]->m_Info.s_PubServer=m_h264[0]->m_Info.s_PubServer;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",DEF_CS_NTGJ_FLAG,SourceCount);

		//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
		m_h264[SourceCount]->m_devType = DEF_SHSL_DEV;

	}

	return 0;

}
#else
int CNTGJDlg::SetDefaultParams(void * pParams)
{
	if(SourceCount>=g_maxLiveNum)//张世俊增加，补全到16路
		return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream补全16的时候分配内存时出现错误！",SourceCount);
		}

		m_h264[SourceCount]->b_Encode	= (g_bUseTc==1?1:0);

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->m_Info.s_PubServer=m_h264[0]->m_Info.s_PubServer;
		m_h264[SourceCount]->m_devType = DEF_SHSL_DEV;
//
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devShslParams.m_dstPub,SourceCount);
	}

	return 0;
}
#endif

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

int CNTGJDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	DEV_H3C_PARAMS tmpH3c;
	m_type = DEF_SHSL_DEV;

	int rt=400;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	//memset(&m_devShslParams,0,sizeof(m_devShslParams));不能清空，里面有CString类型

	rt = f_xmlparse_init(&xml, (const char*)pXmlBuf);
	if (rt != 0)
	{
		m_Log.Add("xml解析错误，xml结构体初始化失败！");
		rt = 400;
		goto leaveOut;
	}

	if(NULL == xml.root)
	{
		m_Log.Add("xml解析错误，无效的XML文件！");
		rt = 400;
		goto leaveOut;
	}

	if (strcmp(xml.root->name, "command") != 0)
	{
		m_Log.Add("xml解析错误，未能找到Command节点!");
		rt = 400;
		goto leave;
	}

	root = xml.root;
#if 0
	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=5)
	{
		m_Log.Add("xml解析错误，未能找到company节点,或者company节点内容不是5!\n");
		rt = 400;
		goto leave;
	}
#endif
	subroot = f_xmlnode_sch_elem(root, "shsl");
	if(subroot!=NULL)
	{
		m_Log.Add("开始上海三利xml解析!");
		node = f_xmlnode_sch_elem_ext(subroot, "proxyIp", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_proxyIp.Format("%s",node->data);
			m_Log.Add("解析到的proxyIP：%s",m_devShslParams.m_proxyIp);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到proxyIP节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "proxyPort", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_proxyPort=atoi(node->data);
			m_Log.Add("解析到的proxyPort：%d",m_devShslParams.m_proxyPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到proxyPort节点!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "deviceId", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_deviceId=node->data;
			m_Log.Add("解析到的deviceId：%s",m_devShslParams.m_deviceId);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到deviceId节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "devicePort", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_devicePort=atoi(node->data);
			m_Log.Add("解析到的devicePort：%d",m_devShslParams.m_devicePort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到devicePort节点!");
			rt = 400;
			goto leave;
		}		
		node = f_xmlnode_sch_elem_ext(subroot, "deviceType", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_deviceType=atoi(node->data);
			m_Log.Add("解析到的deviceType：%d",m_devShslParams.m_deviceType);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到deviceType节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "deviceName", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_deviceName=node->data;
			m_Log.Add("解析到的deviceName：%s",m_devShslParams.m_deviceName);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到deviceName节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "userName", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_userName=node->data;
			m_Log.Add("解析到的userName：%s",m_devShslParams.m_userName);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到userName节点!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "passWord", MAX_LEN);
		if (node != NULL)
		{
			m_devShslParams.m_passWord=node->data;
			m_Log.Add("解析到的passWord：%s",m_devShslParams.m_passWord);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到passWord节点!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
		if (node != NULL)
		{
			serverStr = node->data;
			m_Log.Add("解析到的serverip：%s",serverStr);
		}

		chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
		if (chNode == NULL)
		{
			m_Log.Add("xml解析错误，未能找到ch节点!");
			rt = 400;
			goto leave;
		}

		//**********************************//
		//**********************************//

		m_Log.Add("解析到通道: 参数为:proxyIP:%s,proxyPORT:%d,devID:%s,devPORT:%d,devType:%d,devName:%s，Password:%s, Username:%s\n",
			m_devShslParams.m_proxyIp,m_devShslParams.m_proxyPort,
			m_devShslParams.m_deviceId,m_devShslParams.m_devicePort,
			m_devShslParams.m_deviceType,m_devShslParams.m_deviceName,
			m_devShslParams.m_passWord,m_devShslParams.m_userName);//张世俊增加

		TRACE("\n IP,proxyPORT,ID,devPORT%s :%d %s:%d:%s:%d:%s:%s\n",
			m_devShslParams.m_proxyIp,m_devShslParams.m_proxyPort,
			m_devShslParams.m_deviceId,m_devShslParams.m_devicePort,
			m_devShslParams.m_deviceName,m_devShslParams.m_deviceType,
			m_devShslParams.m_passWord,m_devShslParams.m_userName);	

	}

#if 0
	subroot = f_xmlnode_sch_elem(root, "h3c");
	if(subroot != NULL)
	{//存在h3c

		m_type = DEF_H3C_DEV;

		memset(&tmpH3c,0,sizeof(DEV_H3C_PARAMS));

		node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
		if (node != NULL)
		{
			serverStr = node->data;
			m_Log.Add("解析到的serverip：%s",serverStr);
		}

		m_Log.Add("开始h3c xml解析!");
		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_proxyIp,node->data);
			m_Log.Add("解析到的proxyIP：%s",tmpH3c.m_proxyIp);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到proxyIP节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			tmpH3c.m_proxyPort=atoi(node->data);
			m_Log.Add("解析到的proxyPort：%d",tmpH3c.m_proxyPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到proxyPort节点!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "user", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_userName,node->data);
			m_Log.Add("解析到的userName：%s",tmpH3c.m_userName);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到userName节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_passWord,node->data);
			m_Log.Add("解析到的passWord：%d",tmpH3c.m_passWord);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到passWord节点!");
			rt = 400;
			goto leave;
		}		
		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_deviceId,node->data);
			m_Log.Add("解析到的deviceId：%d",tmpH3c.m_deviceId);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到deviceId节点!");
			rt = 400;
			goto leave;
		}


		node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_dstIp, node->data);
			m_Log.Add("解析到的serverip：%s",tmpH3c.m_dstIp);
		}

		chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
		if (chNode == NULL)
		{
			m_Log.Add("xml解析错误，未能找到ch节点!");
			rt = 400;
			goto leave;
		}

		m_Log.Add("解析到通道: 参数为:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,serverip:%s\n",
			tmpH3c.m_proxyIp,
			tmpH3c.m_proxyPort,
			tmpH3c.m_userName,
			tmpH3c.m_passWord,
			tmpH3c.m_deviceId,
			tmpH3c.m_dstIp);//张世俊增加

		TRACE("解析到通道: 参数为:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,serverip:%s\n",
			tmpH3c.m_proxyIp,
			tmpH3c.m_proxyPort,
			tmpH3c.m_userName,
			tmpH3c.m_passWord,
			tmpH3c.m_deviceId,
			tmpH3c.m_dstIp);	
	}
#endif
	if (0 == chNode->data_siz)
	{
		m_Log.Add("解析到的ch为空值!");	
		if(m_type == DEF_SHSL_DEV)
		{
			m_Log.Add("查找视频：%s:%d 是否存在！",m_devShslParams.m_deviceId,m_devShslParams.m_devicePort);
		}
		if(m_type == DEF_H3C_DEV)
			m_Log.Add("查找视频：%s:%s 是否存在！",tmpH3c.m_proxyIp,tmpH3c.m_deviceId);

		WaitForSingleObject(Main->m_Lock,INFINITE);
		if(m_type == DEF_SHSL_DEV)
		{
			tmpStr.Format("%s:%d",m_devShslParams.m_deviceId.GetBuffer(m_devShslParams.m_deviceId.GetLength()),m_devShslParams.m_devicePort);//作为唯一性的标识，如id为dns:7856,视频通道为2，则表示为：dns:7856：1
			m_devShslParams.m_deviceId.ReleaseBuffer();
		}
		if(m_type == DEF_H3C_DEV)
			tmpStr.Format("%s:%s",tmpH3c.m_proxyIp,tmpH3c.m_deviceId);

//		resultStr=	Main->ChangeCh(tmpStr,-1,"");
		
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
		//m_Log.Add("切换%s,至通道%d,服务器%s",tmpStr,atoi(chNode->data),serverStr);
#if 0
		if("" == serverStr)
		{
			m_Log.Add("切换视频%s:%d失败，未找到serverip!",m_devShslParams.m_deviceId,m_devShslParams.m_devicePort);
			rt = 400;
			goto leave;
		}
#endif
		int m_ch;
		if(atoi(chNode->data)>=g_maxLiveNum)
		{
			m_ch = atoi(chNode->data);
			m_ch = m_ch%g_maxLiveNum;

		}else
		{
			m_ch = atoi(chNode->data);
		}

		//if(m_type == DEF_H3C_DEV)
		//{
		//	memcpy(&tmpH3c.m_loginInfo,&m_h3cParams[m_ch].m_loginInfo,sizeof(LOGIN_INFO_S));
		//	strcpy(tmpH3c.m_szXpCode,m_h3cParams[m_ch].m_szXpCode);
		//	m_h3cParams[m_ch] = tmpH3c;
		//}

		WaitForSingleObject(Main->m_Lock,INFINITE);
		resultStr=ChangeChannel(tmpStr,m_ch,serverStr);
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

leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}
#if 0//old
int CNTGJDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	if(pH264Stream->m_devType == DEF_SHSL_DEV)
	{
		DEV_SHSL_PARAMS *pShslParams = (DEV_SHSL_PARAMS *)pParams;

		m_netVideo[pH264Stream->thisIndex].SetDeviceInfo(121,pShslParams->m_deviceName,pShslParams->m_deviceId,
			0,pShslParams->m_proxyIp, 17891, pShslParams->m_userName, pShslParams->m_passWord);
		//UpdateData();
		//m_NetVideo.SetProxyIP((LPTSTR)m_TransmitIP);
		m_netVideo[pH264Stream->thisIndex].SetUser((LPUNKNOWN)NULL,false);

		pH264Stream->first=true;

		//m_Info.s_DevID=pParams->m_deviceId+_T(":");

		pH264Stream->m_Info.s_DevID.Format("%s:%d",pShslParams->m_deviceId.GetBuffer(pShslParams->m_deviceId.GetLength()),pShslParams->m_devicePort);//作为唯一性的标识，如id为dns:7856,视频通道为2，则表示为：dns:7856：1
		pShslParams->m_deviceId.ReleaseBuffer();
	}

	if(pH264Stream->m_devType == DEF_H3C_DEV)
	{
		DEV_H3C_PARAMS *pH3cParams = (DEV_H3C_PARAMS *)pParams;

		pH264Stream->first=true;

		//m_Info.s_DevID=pParams->m_deviceId+_T(":");
		pH264Stream->m_Info.s_DevID.Format("%s:%s",pH3cParams->m_proxyIp,pH3cParams->m_deviceId);

	}


	if (false == pH264Stream->b_start)
	{
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		//在StartPush中设置宽度和高度等信息，再启线程进入pushstream（）
		//在pushstream()实现把把从模板获取的视频数据编码；保存到指定结构体pMain->g_rtmp
		pH264Stream->b_start=true;
	}
	return 0;
}
#else
int CNTGJDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_SHSL_PARAMS *pShslParams = (DEV_SHSL_PARAMS *)pParams;

	int ret=0;


	m_netVideo[pH264Stream->thisIndex].SetDeviceInfo(121,pShslParams->m_deviceName,pShslParams->m_deviceId,
		0,pShslParams->m_proxyIp, 17891, pShslParams->m_userName, pShslParams->m_passWord);
	//UpdateData();
	//m_NetVideo.SetProxyIP((LPTSTR)m_TransmitIP);
	m_netVideo[pH264Stream->thisIndex].SetUser((LPUNKNOWN)NULL,false);

	pH264Stream->first=true;

	//m_Info.s_DevID=pParams->m_deviceId+_T(":");

	pH264Stream->m_Info.s_DevID.Format("%s:%d",pShslParams->m_deviceId.GetBuffer(pShslParams->m_deviceId.GetLength()),pShslParams->m_devicePort);//作为唯一性的标识，如id为dns:7856,视频通道为2，则表示为：dns:7856：1
	pShslParams->m_deviceId.ReleaseBuffer();


	pH264Stream->m_devType = DEF_SHSL_DEV;

	if (false == pH264Stream->b_start)
	{
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);


	return ret;
}
#endif
VOID STDCALL myCallH264(IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo,
                                                         IN const CHAR *pcChannelCode,
                                                         IN const XP_PARSE_VIDEO_DATA_S *pstParseVideoData,
                                                         IN LONG lUserParam, 
                                                         IN LONG lReserved)
{
	TRACE("timestamp:%lld length:%d height:%d width:%d  user:%d\n",pstParseVideoData->dlTimeStamp,pstParseVideoData->ulDataLen,pstParseVideoData->ulHeight,
		pstParseVideoData->ulWidth,lUserParam);

	//static FILE *pf=NULL;
	//if(pf == NULL)
	//	pf = fopen("test.yuv","wb");
	//fwrite(pstParseVideoData->pucData,1,pstParseVideoData->ulDataLen,pf);

//	CH264Stream* pMain=(CH264Stream*)pNtgjDlg->m_h264[lUserParam];
	CH264Stream* pMain=m_h264[lUserParam];
	pMain->b_PullLive=0;
	if(pMain == NULL)
		return;
		
	if(pMain->p_EncodeArray.GetSize()<200)
	{
		if (NULL ==pstParseVideoData->pucData)
		{
			m_Log.Add("视频数据为空！");

			return ;
		}
		if (0 == pstParseVideoData->ulDataLen )
		{
			m_Log.Add("视频数据为空！");

			return ;
		}

		MyAvFrame *m_Frame=NULL;
		//if(lFrameLen<(352*288*3/2+1))//由于无法判断其大小，估先设置其当前大小作为该次数据量大小
		//	m_Frame = new MyAvFrame(352,288);//默认设置大小 CIF标准
		//else
		m_Frame = new MyAvFrame(pstParseVideoData->ulDataLen+1,1);//暂无法判断出视频大小


		if (NULL == m_Frame)
		{
			m_Log.Add("new MyAvFrame的时候内存分配错误！");

			return ;
		}
		//add  季成
		if(0 == pMain->curTime)
		{
			pMain->curTime = pstParseVideoData->dlTimeStamp;//gettick
			m_Frame->i_time = 0;
			//		TRACE("input %d\n",m_Frame->i_time);
		}
		else
		{

			m_Frame->i_time = (pstParseVideoData->dlTimeStamp-pMain->curTime);
			pMain->curTime = pstParseVideoData->dlTimeStamp;
			//		TRACE("input %d\n",m_Frame->i_time);
		}


		/*
		if(0x0dc == (lFrameType&0x0dc))
		{
		m_Frame->i_key = 1;

		}else
		m_Frame->i_key = 0;
		*/
		if (NULL == m_Frame->p_data)
		{
			m_Log.Add("分配到的MyAvFrame里的p_data指针为空！");

			return ;
		}
		memcpy(m_Frame->p_data+1,pstParseVideoData->pucData,pstParseVideoData->ulDataLen);//new MyAvFrame(352,288)<--byteVideoData

		pMain->m_streamlock.Lock();
		pMain->p_EncodeArray.Add(m_Frame);//视频数据存进待编码数据缓冲区CArray<MyAvFrame*,MyAvFrame*>
		pMain->m_streamlock.Unlock();
	}


	return;
}

#if 0//old
// 启动拉流函数
int CNTGJDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	if(pH264Stream->m_devType == DEF_SHSL_DEV)
	{
		DEV_SHSL_PARAMS *pShslParams = (DEV_SHSL_PARAMS *)pParams;

		int status=FALSE;
		if (NULL!=pH264Stream->h_stream)
		{
			m_netVideo[pH264Stream->thisIndex].CloseVideo();

			pH264Stream->h_stream=NULL;
		}
		m_netVideo[pH264Stream->thisIndex].SetReconnect2(1,0);
		status = m_netVideo[pH264Stream->thisIndex].OpenVideo(pShslParams->m_devicePort);


		if(status != 0)//打开失败
		{
			m_Log.Add("控件OpenVideo（）时失败！");	
		}

		pH264Stream->h_stream = &pShslParams->m_devicePort;
	}

	if(pH264Stream->m_devType == DEF_H3C_DEV)
	{
		DEV_H3C_PARAMS *pH3cParams = (DEV_H3C_PARAMS *)pParams;
		ULONG ulRet;
		int i;

		CHAR szPasswd[IMOS_PASSWD_ENCRYPT_LEN] = {0};
		ulRet = IMOS_Encrypt(pH3cParams->m_passWord, strlen(pH3cParams->m_passWord), szPasswd);
		if (ERR_COMMON_SUCCEED != ulRet)
		{
			//log
			//DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, "", "密码加密失败");
			return -1;
		}


		//用户登陆
		ulRet = IMOS_LoginEx(pH3cParams->m_userName, szPasswd, pH3cParams->m_proxyIp, "N/A", &pH3cParams->m_loginInfo);//IMOS_Login(szUserLoginName, szPasswd, szServerAdd, &g_stLoginInfo);
		if (ERR_COMMON_SUCCEED != ulRet)
		{
			//log
			m_Log.Add("用户登陆:用户名%s:密码%s", pH3cParams->m_userName,szPasswd);
			return -1;
		}

		/* 分配内存获取窗格编码 */
		PLAY_WND_INFO_S *pstPlayWndInfoList = NULL;
		pstPlayWndInfoList = (PLAY_WND_INFO_S*)malloc(DEMO_MAX_PLAYS * sizeof(PLAY_WND_INFO_S));
		if (NULL == pstPlayWndInfoList)
		{
			//		DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "分配内存失败");
			return -1;
		}
		memset(pstPlayWndInfoList, 0, DEMO_MAX_PLAYS * sizeof(PLAY_WND_INFO_S));

		/* 启动播放器组件并获得窗格编码 */
		ulRet = IMOS_StartPlayer(&pH3cParams->m_loginInfo.stUserLoginIDInfo, DEMO_MAX_PLAYS, pstPlayWndInfoList);
		if (ERR_COMMON_SUCCEED != ulRet)
		{
			//		DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "启动播放器组件失败");
			return -1;
		}
		free(pstPlayWndInfoList);

		//char szXpCode[IMOS_RES_CODE_LEN] = {0};
		ulRet = IMOS_GetChannelCode(&pH3cParams->m_loginInfo.stUserLoginIDInfo, pH3cParams->m_szXpCode);


		ulRet = IMOS_SetParseVideoDataCB  ( &pH3cParams->m_loginInfo.stUserLoginIDInfo,  
			pH3cParams->m_szXpCode,  
			myCallH264,  
			1,  
			pH264Stream->thisIndex);


		/* 调用SDK接口开始实时监控，操作码为0 */
		ulRet = IMOS_StartMonitor(&pH3cParams->m_loginInfo.stUserLoginIDInfo,
			pH3cParams->m_deviceId,
			pH3cParams->m_szXpCode,
			IMOS_FAVORITE_STREAM_ANY,
			USER_OPERATE_SERVICE);
		if (ERR_COMMON_SUCCEED != ulRet)
		{
			//log
			//DEMO_LOG_2(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "实时监控", 
			//	"摄像机", pCurNode->m_nodeInfo.szResCode,
			//	"窗格", pCurPlay->m_szXpCode);
			return -1;
		}

		pH264Stream->h_stream = &pH3cParams->m_szXpCode;

	}

	return 0;
}
#else
int CNTGJDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;

	DEV_SHSL_PARAMS *pShslParams = &m_devShslParams;

	int status=FALSE;
	if (NULL!=pH264Stream->h_stream)
	{
		m_netVideo[pH264Stream->thisIndex].CloseVideo();

		pH264Stream->h_stream=NULL;
	}
	m_netVideo[pH264Stream->thisIndex].SetReconnect2(1,0);
	status = m_netVideo[pH264Stream->thisIndex].OpenVideo(pShslParams->m_devicePort);


	if(status != 0)//打开失败
	{
		m_Log.Add("控件OpenVideo（）时失败！");	
	}

	pH264Stream->h_stream = &pShslParams->m_devicePort;

	return 0;
}
#endif

int CNTGJDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	if(pH264Stream->m_devType == DEF_SHSL_DEV)
	{
		DEV_SHSL_PARAMS *pShslParams = (DEV_SHSL_PARAMS *)pParams;

		int status=FALSE;
		if (NULL!=pH264Stream->h_stream)
		{
			m_netVideo[pH264Stream->thisIndex].CloseVideo();

			pH264Stream->h_stream=NULL;
		}
	}
	//if(pH264Stream->m_devType == DEF_H3C_DEV)
	//{

	//	DEV_H3C_PARAMS *pH3cParams = &pNtgjDlg->m_h3cParams[pH264Stream->thisIndex];
	//	ULONG ulRet;

	//	if (NULL!=pH264Stream->h_stream)
	//	{

	//		ulRet = IMOS_StopMonitor(&pH3cParams->m_loginInfo.stUserLoginIDInfo,
	//			pH3cParams->m_szXpCode,
	//			0);

	//		IMOS_FreeChannelCode(&pH3cParams->m_loginInfo.stUserLoginIDInfo, pH3cParams->m_szXpCode);

	//		IMOS_LogoutEx(&pH3cParams->m_loginInfo.stUserLoginIDInfo);
	//		pH264Stream->h_stream=NULL;
	//	}

	//}

	return 0;
}

CString CNTGJDlg::CheckExit(CString CamID)
{
	CString UrlStr="";

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{
		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//如果之前已经被暂停,则直接返回当前无切换，让BMS下发切换命令。
				m_Log.Add("视频:%s 已经停止播放，不存在!",CamID);
				//return "";
				continue;
			}

		//	UrlStr.Format("200rtmp://%s:%d/live/%s_v%d",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,DEF_CS_NTGJ_FLAG,i);
			UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);

			m_Log.Add("视频:%s 存在!URL:%s",CamID,UrlStr);

			m_h264[i]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。

			return UrlStr;
		}
	}
	m_Log.Add("视频:%s 不存在!",CamID);

	return UrlStr;
}

CString CNTGJDlg::ChangeChannel(CString sId,int CH,CString str)
{
	CString UrlStr="";

	if(CH >= g_maxLiveNum)
	{//临时设置最大转码路数
		m_Log.Add("原视频应当切换通道为:%d \n!",CH);
		CH = CH%g_maxLiveNum;
		SourceCount = g_maxLiveNum;
		m_Log.Add("实际视频应当切换通道为:%d \n!",CH);
	}

	if (CH>=SourceCount)
	{
		m_Log.Add("CH的值不能大于%d，出错!",g_maxLiveNum);
		return "";
	}

	if(m_type == DEF_SHSL_DEV)
	{
		m_h264[CH]->m_Info.s_DevID.Format("%s:%d",m_devShslParams.m_deviceId.GetBuffer(m_devShslParams.m_deviceId.GetLength()),m_devShslParams.m_devicePort);//作为唯一性的标识，如id为dns:7856,视频通道为2，则表示为：dns:7856：1
		m_devShslParams.m_deviceId.ReleaseBuffer();
	}

	//if(m_type == DEF_H3C_DEV)
	//	m_h264[CH]->m_Info.s_DevID.Format("%s:%s",m_h3cParams[CH].m_proxyIp,m_h3cParams[CH].m_deviceId);//作为唯一性的标识，如id为dns:7856,视频通道为2，则表示为：dns:7856：1


	m_Log.Add("上海三利准备切换%s,至通道%d,服务器%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	if (NULL!=m_h264[CH]->h_stream)
	{//该路已经被使用，需要先关闭再进行操作
		//m_netVideo[CH].CloseVideo();
		ClosePull(m_h264[CH],&m_devShslParams);
		//m_h264[CH]->h_stream=NULL;

	}

	m_h264[CH]->m_devType = m_type;//关闭后才可以更改其中的设备类型

	m_h264[CH]->m_Info.s_PubServer=str;

	//m_h264[CH]->Init(&m_devShslParams,this->GetSafeHwnd());
	//m_h264[CH]->StartGet(m_devShslParams.m_devicePort);
	if(m_type == DEF_SHSL_DEV)
	{
	SetParams(m_h264[CH],&m_devShslParams);
//	StartPull(m_h264[CH],&m_devShslParams);
	}
	//if(m_type == DEF_H3C_DEV)
	//{
	//	SetParams(m_h264[CH],&m_h3cParams[CH]);
	//StartPull(m_h264[CH],&m_h3cParams[CH]);

	//}

	m_h264[CH]->h_stream = &m_devShslParams.m_devicePort;

	{
		CString UrlStr="";
	//	UrlStr.Format("200rtmp://%s:%d/live/%s_v%d",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,DEF_CS_NTGJ_FLAG,CH);
		UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);

		m_Log.Add("上海三利视频%s 切换成功%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//保存正确的配置到ini文件中
		m_h264[CH]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。且持续时间从现在开始算起。

		return UrlStr;
	}


	return UrlStr;


}

int CNTGJDlg::InitEncode(void *pParams)
{

	return 0;
}

int CNTGJDlg::EncodeFrame(void *pIn,void *pOut)
{

	return 0;
}

int CNTGJDlg::CloseEncode(void *pParams)
{

	return 0;
}
