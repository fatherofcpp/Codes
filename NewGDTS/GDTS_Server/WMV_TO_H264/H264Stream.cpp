#include "StdAfx.h"
#include "H264Stream.h"
#include "WMV_TO_H264Dlg.h"
//#define MAXBUFFSIZE 120000
//DWORD portNum =1;
#include "SimpleResize.h"
#include "Log.h"
#include "avLib.h"
//#define STREAMPATH "microvideo"
#define STREAMPATH "live"
STREAMPROCESSOPEN ProcessOpen;
STREAMPROCESSSTART ProcessStart;
STREAMPROCESSCLOSE ProcessClose;
//1024
ENCODERSTREAMPROCESSOPEN	encoderstreamProcessOpen=NULL;
ENCODERSTREAMPROCESSSTART	encoderstreamProcessStart=NULL;
ENCODERSTREAMPROCESSCLOSE	encoderstreamProcessClose=NULL;
ENCODERSTREAMPROCESSPUTDATA encoderstreamProcessPutDataAddress=NULL;

CLog m_Log;
CCriticalSection m_ffmpeglock;

extern CWMV_TO_H264Dlg* Main;


#define DEF_PUSHSTREAM_SLEEP_TIME 10 //25  //40 //


int readNoVideFile(void* H264Stream,BYTE **pData, int bStart)//张世俊
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	FileInformation *p_fileInfo=&p_H264Stream->m_novideoFile;

    FILE *pf=NULL;
    char *pBuf = p_fileInfo->pData;
	unsigned int len,ret;
	//char o_file[64];
	//memset(o_file, 0, sizeof(o_file));
	//strcpy(o_file, "E:\\");
	//strcat(o_file, m_fileInfo.fileName);

	if(NULL == pBuf )
	{
		pf = fopen("SleepVideoSignal.data","rb");//pfile"base_xml.xml"
		if(pf)
		{
			fseek(pf,0,SEEK_END);
		    p_fileInfo->fileLen = ftell(pf);	
			fseek(pf,0,SEEK_SET);

			pBuf = new char[p_fileInfo->fileLen+1];
			if(pBuf==NULL)
			{
				fclose(pf);
				TRACE("ERROR:malloc pxmlbuf failed\n");
			 }
			 ret = fread(pBuf,1,p_fileInfo->fileLen,pf);
			 p_fileInfo->pData = pBuf;
			 fclose(pf);	
		 }  
	}
    if(p_fileInfo->offset >= (p_fileInfo->fileLen/2))//[iIndex]
	{
		 p_fileInfo->offset = 0;
		 //TRACE("读文件完成，重头开始读文件!\n");
	 }
     if(bStart == 1)
     {
         p_fileInfo->offset = 0;
		 //TRACE("第二次重头开始读文件!\n");
		 //m_Log.Add("[INFO]%s无视频数据，启用本地无直播流",p_H264Stream->m_Info.s_DevID);
     }
	 else
	 {
		 //m_Log.Add("The value of the m_fileInfo.offset is:%d",m_fileInfo.offset);
		 //TRACE("The value of the m_fileInfo.offset is:%d\n",p_fileInfo->offset);
	 }

	 len = *(unsigned short*)&pBuf[p_fileInfo->offset];
	 p_fileInfo->offset += 2;
	 *pData = (BYTE *)&pBuf[p_fileInfo->offset];
	 p_fileInfo->offset += len;

	 //TRACE("The len is:%d buffer is %08x offset is:%d fileLen is%d\n",len,*pData,p_fileInfo->offset,p_fileInfo->fileLen);
 
	return len;
}

#define USE_DEFAULT_TIMESTAMP 

#define MAXCHANNELNUMS 16

#define VIDEODINSTANCE 0x200

static int VideoDistance[MAXCHANNELNUMS]; 
static int FirstTimeRunning[MAXCHANNELNUMS];
static LONGLONG PreviousTimeVideo[MAXCHANNELNUMS];
static LONGLONG pFrameTime[MAXCHANNELNUMS];
static LONGLONG BaseTimeVideo[MAXCHANNELNUMS];
static int frame_count[MAXCHANNELNUMS];
static int DeltaTimeVideo[MAXCHANNELNUMS];

static int initTimeStamp( int index1)
{
	VideoDistance[index1]=VIDEODINSTANCE; 
	FirstTimeRunning[index1]=TRUE;
	PreviousTimeVideo[index1]=0;
	pFrameTime[index1]=0;
	BaseTimeVideo[index1]=0;
	frame_count[index1] = 0;
	DeltaTimeVideo[index1]=40;

	return 0;
}

static int getTimeStamp( int inx )//计算时间戳函数
{
	LONGLONG            CurTimeVideo=0;

	CurTimeVideo=GetTickCount();//CLK_getltime();    

	if (FirstTimeRunning[inx]) 
	{
		switch (frame_count[inx]) 
		{
			case 0:
			case 1:
				BaseTimeVideo[inx]=CurTimeVideo;
				PreviousTimeVideo[inx]=CurTimeVideo;
				DeltaTimeVideo[inx]=10*(100)/30;//inputvideo[inx].capFrmRate;//40;
				break;
			case VIDEODINSTANCE:
				FirstTimeRunning[inx]=FALSE;
				pFrameTime[inx]=0;
				break;
			default:
				DeltaTimeVideo[inx]=(int)(CurTimeVideo-PreviousTimeVideo[inx]);
				PreviousTimeVideo[inx]=CurTimeVideo;
				if (DeltaTimeVideo[inx]>200) 
					DeltaTimeVideo[inx]=1000/30;//inputvideo[inx].capFrmRate;//40;//44;
				break;
		}
	}

	if (frame_count[inx]==VideoDistance[inx]) 
	{//以后就是VideoDistance个视频帧一组，时间间隔一样。
		DeltaTimeVideo[inx]=(int)(CurTimeVideo-BaseTimeVideo[inx]+pFrameTime[inx])/frame_count[inx];
		if (frame_count[inx]<VIDEODINSTANCE)
			pFrameTime[inx]=(CurTimeVideo-BaseTimeVideo[inx]+pFrameTime[inx])%frame_count[inx];
		BaseTimeVideo[inx]=CurTimeVideo;    
		VideoDistance[inx]=20;      
		frame_count[inx]=0;
	}

	frame_count[inx]++;

	return DeltaTimeVideo[inx];
}

#define DEF_AFTER_NOVIDEO_TIMES  20

UINT _stdcall push2mss(void* H264Stream)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	int rtn=0;
	//int i_NoDataTime=0;
	unsigned int i_NoData=0,nTimes = 0;
	//CRTMPPacket sPacket=g_RTMPPacket;//hao add
	int m_CurrentWH[2];
	int b_first=1;//来标识以前是否有过数据

	MyAvFrame *m_Frame=NULL;

	int sendlength=0;


	MyAvFrame *noVideoFrame=new MyAvFrame(352,288);
	if (NULL == noVideoFrame)
	{
		m_Log.Add("push2mss函数中new MyAvFrame分配内存时出现错误！");
		return TRUE;
	}
	if (NULL == noVideoFrame->p_data)
	{
		m_Log.Add("push2mss函数中m_Frame->p_data分配内存时出现错误！");
		return TRUE;
	}
	//memcpy(noVideoFrame->p_data,pOut->data[0],pOut->width*pOut->height);
	{
		int i_Len,fLen;

		FILE *pf = fopen("nolive_YUV420.yuv","rb");//pfile"base_xml.xml"
		if(pf)
		{
			fseek(pf,0,SEEK_END);
			fLen = ftell(pf);	
			fseek(pf,0,SEEK_SET);

			i_Len = fread(noVideoFrame->p_data,1,fLen,pf);

			fclose(pf);	
			m_Log.Add("push2mss函数中打开nolive_YUV420.yuv成功！");
		}else
		{
			m_Log.Add("push2mss函数中打开nolive_YUV420.yuv错误！");
			return TRUE;

		}
	}

	//char *pPspPps = new char[1024*1024];

#ifdef USE_DEFAULT_TIMESTAMP

	initTimeStamp(p_H264Stream->thisIndex);

#endif


	m_CurrentWH[0] = p_H264Stream->m_Info.i_Width;
	m_CurrentWH[1] = p_H264Stream->m_Info.i_Height;

	p_H264Stream->enocderInit(NULL);
	p_H264Stream->encoderOpen();
	p_H264Stream->encoderStart();



	while (true)
	{
		if(Main->g_Stop == 1)
			break;

		if(nTimes % 500 == 0)
		{
			TRACE("[pushstream]:该路[%s]线程正在运行【%d】。\n",p_H264Stream->m_Info.s_PubName,nTimes);
		}
		nTimes++;

		if(1 == p_H264Stream->b_stop)
		{//一段时间内没有人拉该路视频，将启动该部分。

#if 0	
			if(NULL != p_H264Stream->h_stream)
			{
				Main->pNtgjDlg->ClosePull(p_H264Stream,NULL);
				TRACE("INFO:该路[%s]视频停止拉流。\n",p_H264Stream->m_Info.s_PubName);

			}

			if(-1 != p_H264Stream->g_rtmp.m_socket)
			{//关闭rtmp通讯
				p_H264Stream->g_rtmp.Close(&p_H264Stream->g_rtmp);

				//p_H264Stream->b_start = false;
				TRACE("INFO:该路[%s]视频停止播放。\n",p_H264Stream->m_Info.s_PubName);
			}

			Sleep(1000);
			continue;
#endif
		}

		m_Frame=NULL;//宽，高，I帧等
		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_YuvArray.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_YuvArray.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_YuvArray.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();


		int i_Len=0;
		if (NULL!=m_Frame)//从模板CArray获取到了视频数据
		{
			if ((m_Frame->i_width!=m_CurrentWH[0] || m_Frame->i_height != m_CurrentWH[1]))//?&&(g_EncodeType == 0)
				//非标准视频数据格式，要对获取的视频数据编码
			{
				SimpleResize m_s(m_Frame->i_width,m_Frame->i_height,m_CurrentWH[0],m_CurrentWH[1],&rtn);
				//构造对象m_s
				if (rtn!=-1)
				{
					m_s.resize((BYTE*)m_Frame->p_data,p_H264Stream->encodebuff);

					i_Len=p_H264Stream->encoderPut(p_H264Stream->encodebuff,m_CurrentWH[0]*m_CurrentWH[1]*3/2,STREAM_VIDEO);

				}
				else
				{
					if (NULL!=m_Frame)
					{
						delete m_Frame;
					}
				}
			}
			else//标准视频数据格式，不需要对获取的视频数据编码
			{
				i_Len=p_H264Stream->encoderPut(m_Frame->p_data,(m_Frame->i_width)*(m_Frame->i_height)*3/2,STREAM_VIDEO);
			}

			i_NoData=0;

			p_H264Stream->usetime = getTimeStamp(p_H264Stream->thisIndex);
			b_first = 0;

		}
		else if(b_first==0 || i_NoData > 50)
			//模板CArray之前获取过数据，但现在已经没有视频数据了
		{

			i_NoData++;

#if 1
			if(i_NoData<50)
				i_Len=p_H264Stream->encoderPut(STREAM_VIDEO);
			//#else			
			else
			{
				if (noVideoFrame->i_width!=m_CurrentWH[0] || noVideoFrame->i_height != m_CurrentWH[1])
					//非标准视频数据格式，要对获取的视频数据编码
				{
					SimpleResize m_s(noVideoFrame->i_width,noVideoFrame->i_height,m_CurrentWH[0],m_CurrentWH[1],&rtn);
					//构造对象m_s
					if (rtn!=-1)
					{
						m_s.resize((BYTE*)noVideoFrame->p_data,p_H264Stream->encodebuff);
						//encodebuff<--m_Frame->p_data,把从模板获取的视频数据编码并存到encodebuff
						i_Len=p_H264Stream->encoderPut(p_H264Stream->encodebuff,m_CurrentWH[0]*m_CurrentWH[1]*3/2,STREAM_VIDEO);

						//把视频数据编码放入临时数组pbuff并计算数据长度
						if(i_Len <=0 )
							m_Log.Add("INFO:该路[%s]编码出错***1。\n",p_H264Stream->m_Info.s_PubName);
					}

				}
				else//标准视频数据格式，不需要对获取的视频数据编码
				{
					i_Len=p_H264Stream->encoderPut(noVideoFrame->p_data,(noVideoFrame->i_width)*(noVideoFrame->i_height)*3/2,STREAM_VIDEO);

				}
			}

#endif


			p_H264Stream->usetime = getTimeStamp(p_H264Stream->thisIndex);


			Sleep(DEF_PUSHSTREAM_SLEEP_TIME*3);//
			continue;
		}
		else//未从CArray获取过视频数据
		{ 
			i_NoData ++;

			Sleep(DEF_PUSHSTREAM_SLEEP_TIME*3);//Sleep(m_Frame->i_time)
			continue;
		}

		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}
	}

leave:

	if (m_Frame!=NULL)
	{
		delete m_Frame;
		m_Frame=NULL;
	}

	if(NULL != p_H264Stream->h_stream)
	{
		//Main->pNtgjDlg->ClosePull(p_H264Stream,NULL);
		//TRACE("INFO:该路[%s]视频停止拉流。\n",p_H264Stream->m_Info.s_PubName);

	}
	while(1){

		m_Frame=NULL;//宽，高，I帧等
		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_YuvArray.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_YuvArray.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_YuvArray.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();

		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}else
			break;
	}



	p_H264Stream->b_start = false;

	HRESULT  hr=S_OK;
	_endthreadex(hr);
	return hr;

	return 0;
}

//static FILE *pf = NULL;
UINT _stdcall pushstream(void* H264Stream)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	int rtn=0;
	//int i_NoDataTime=0;
	int i_NoData=0,nTimes = 0;
	//CRTMPPacket sPacket=g_RTMPPacket;//hao add

	MyAvFrame *m_Frame=NULL;

	int sendlength=0;
	BYTE* pbuff;//=new BYTE[MAXBUFFSIZE];
	BYTE *pPspPps=NULL;
	int nPspPpsLen=0;

	//if(p_H264Stream->m_devType == DEF_SHSL_DEV){
	if(p_H264Stream->b_Encode == 0){
		pPspPps = new BYTE[1024*1024];

	}

#ifdef USE_DEFAULT_TIMESTAMP
	initTimeStamp(p_H264Stream->thisIndex);
#endif
	
	p_H264Stream->g_rtmp.Connect(&p_H264Stream->g_rtmp,p_H264Stream->m_Info.s_PubServer,STREAMPATH,p_H264Stream->m_Info.s_PubName);
	//接口函数
	Sleep(1000);
	p_H264Stream->g_rtmp.SendCreateStream(&p_H264Stream->g_rtmp);
	Sleep(1000);
	p_H264Stream->g_rtmp.SendPublishStream(&p_H264Stream->g_rtmp);
	Sleep(1000);

	while (true)
	{
		DWORD startT = GetTickCount(),stopT ;
		Sleep(1);//
		if(Main->g_Stop == 1)
			break;

		if(nTimes % 500 == 0)
		{
			TRACE("[pushstream]:该路[%s]线程正在运行【%d】。\n",p_H264Stream->m_Info.s_PubName,nTimes);
		}
		nTimes++;

		if(1 == p_H264Stream->b_stop)
		{//一段时间内没有人拉该路视频，将启动该部分。

	
			if(NULL != p_H264Stream->h_stream)
			{
				//Main->pNtgjDlg->ClosePull(p_H264Stream,NULL);
				TRACE("INFO:该路[%s]视频停止拉流。\n",p_H264Stream->m_Info.s_PubName);

			}
#if 0
			if(-1 != p_H264Stream->g_rtmp.m_socket)
			{//关闭rtmp通讯
				p_H264Stream->g_rtmp.Close(&p_H264Stream->g_rtmp);

				//p_H264Stream->b_start = false;
				TRACE("INFO:该路[%s]视频停止播放。\n",p_H264Stream->m_Info.s_PubName);
			}

			Sleep(1000);
			continue;
#endif
		}



		m_Frame=NULL;//宽，高，I帧等
		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_EncodeArray.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_EncodeArray.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_EncodeArray.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();
		
#if 0
	{
		static int itimes = 0;
		if(itimes%100 == 0)
		{
			//time_t old_time=0;
			//time(&old_time);
			TRACE("*******100 frame %d\n",GetTickCount());
		}

		itimes++;		
		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}
		Sleep(10);
		continue;

	}
	//return TRUE;
#endif

#if 0//测试用，查找崩溃
		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
			
		}
			Sleep(10);
		
		continue;

#endif
		int i_Len=0;
		if (NULL!=m_Frame)//从模板CArray获取到了视频数据
		{
			//p_H264Stream->b_novideo = 0;
			//if(p_H264Stream->m_devType == DEF_SHSL_DEV)||(p_H264Stream->m_devType == DEF_NTQY_DEV)//上海三利
			if(p_H264Stream->b_Encode == 0)//上海三利
			{
				if(0x67 ==m_Frame->p_data[4])
				{
					nPspPpsLen = m_Frame->i_height * m_Frame->i_width;
					memcpy(pPspPps,m_Frame->p_data,nPspPpsLen);

				}
				if((i_NoData >= (DEF_AFTER_NOVIDEO_TIMES+1) ) && (0x67 !=m_Frame->p_data[4]))
				{
					pbuff = pPspPps;
					i_Len = nPspPpsLen;

				}else
				{
					pbuff = (BYTE*)m_Frame->p_data;
					i_Len = m_Frame->i_height * m_Frame->i_width;
				}

			}else
			{
				pbuff = (BYTE*)m_Frame->p_data;
					i_Len = m_Frame->i_height * m_Frame->i_width;

			}
			
			i_NoData=0;

#ifdef USE_DEFAULT_TIMESTAMP

			p_H264Stream->usetime = getTimeStamp(p_H264Stream->thisIndex);
#else
			p_H264Stream->nowtime=GetTickCount();
			if (p_H264Stream->oldtime==0)
			{
				p_H264Stream->usetime=0;
				p_H264Stream->oldtime=p_H264Stream->nowtime;
			}
			else
			{
				//p_H264Stream->usetime+=p_H264Stream->nowtime-p_H264Stream->oldtime;
				p_H264Stream->usetime=p_H264Stream->nowtime-p_H264Stream->oldtime;
				p_H264Stream->oldtime=p_H264Stream->nowtime;
			}
#endif

		}
		//else if((true != p_H264Stream->first) && (p_H264Stream->m_devType != DEF_SHSL_DEV))
		else if((true != p_H264Stream->first) && (p_H264Stream->b_Encode == 1))
		//模板CArray之前获取过数据，但现在已经没有视频数据了
		{

			i_NoData++;
			Sleep(DEF_PUSHSTREAM_SLEEP_TIME);
			continue;
		}
		else//未从CArray获取过视频数据
		{ 
			i_NoData ++;
			//if((p_H264Stream->m_devType == DEF_SHSL_DEV)&&(i_NoData > DEF_AFTER_NOVIDEO_TIMES))//上海三利
			if((p_H264Stream->b_Encode == 0)&&(i_NoData > DEF_AFTER_NOVIDEO_TIMES))//上海三利
			{
				//TRACE("1------------->%d\n",shslParams->shsl_noframe_count);
				int ret = (i_NoData == (DEF_AFTER_NOVIDEO_TIMES+1))?1:0;
				
				i_Len = readNoVideFile(p_H264Stream,&pbuff,ret);

#ifdef USE_DEFAULT_TIMESTAMP

			p_H264Stream->usetime = getTimeStamp(p_H264Stream->thisIndex);
#else
				p_H264Stream->nowtime=GetTickCount();
				if(p_H264Stream->oldtime==0)
				{
					p_H264Stream->usetime=0;
					p_H264Stream->oldtime=p_H264Stream->nowtime;
				}
				else
				{
					p_H264Stream->usetime=p_H264Stream->nowtime-p_H264Stream->oldtime; //+=
					p_H264Stream->oldtime=p_H264Stream->nowtime;
				}	
#endif
				Sleep(DEF_PUSHSTREAM_SLEEP_TIME*4);//读取本地文件中时间间隔
			}else//非上海三利
			{
				Sleep(DEF_PUSHSTREAM_SLEEP_TIME*4);//Sleep(m_Frame->i_time)
				continue;
			}
		}


		if (true == p_H264Stream->first)
		//从CArray获取到了视频数据，first在Init（）初始化为true，第一次接收到数据走这步
		{
			p_H264Stream->g_rtmp.SendMetaData(&p_H264Stream->g_rtmp,p_H264Stream->g_rtmp.m_MetaData);
			//if(p_H264Stream->m_devType != DEF_SHSL_DEV)
			if(p_H264Stream->b_Encode == 1)
			 	sendlength=p_H264Stream->g_rtmp.SendVideo(&p_H264Stream->g_rtmp,(char*)&pbuff[494],i_Len-494,p_H264Stream->usetime);
			else
			    sendlength=p_H264Stream->g_rtmp.SendVideo(&p_H264Stream->g_rtmp,(char*)&pbuff[0],i_Len,p_H264Stream->usetime);
			p_H264Stream->first=false;	
		}
		else
			//发送视频数据到g_rtmp，若发送失败则重连继续发送，非第一次接收数据走这步
		{

			sendlength=p_H264Stream->g_rtmp.SendVideo(&p_H264Stream->g_rtmp,(char*)pbuff,i_Len,/*40*/p_H264Stream->usetime);
			//TRACE("#######i_Len is %d,time is %d,[%s]\n",i_Len,p_H264Stream->usetime,p_H264Stream->m_Info.s_PubName);
			if (-1>sendlength)
			{//小于-1说明是发送失败，连接已经断开。
				TRACE("send time is %d:%d:%s\n",i_NoData,sendlength,p_H264Stream->m_Info.s_PubName);

				m_Log.Add("[INFO]:发送失败%s，准备重连服务器!\n",p_H264Stream->m_Info.s_PubName);
				Sleep(100);
				p_H264Stream->g_rtmp.Connect(&p_H264Stream->g_rtmp,
					p_H264Stream->m_Info.s_PubServer,
					STREAMPATH,
					p_H264Stream->m_Info.s_PubName);
				//发送失败，重连服务器；
				Sleep(100);
				p_H264Stream->g_rtmp.SendCreateStream(&p_H264Stream->g_rtmp);
				Sleep(100);
				p_H264Stream->g_rtmp.SendPublishStream(&p_H264Stream->g_rtmp);
				Sleep(100);
				p_H264Stream->g_rtmp.SendMetaData(&p_H264Stream->g_rtmp,p_H264Stream->g_rtmp.m_MetaData);
				m_Log.Add("[INFO]:重连%s,发布名%s\n",p_H264Stream->m_Info.s_PubServer,p_H264Stream->m_Info.s_PubName);
				//MessageBox(NULL,"aa","sdf",MB_OK);
			}else if(-1 == sendlength){
				//-1说明没有解析到正确的数据，或者是数据无效
				TRACE("pushstream:[INFO]:该条数据无效，丢弃[%s]\n",p_H264Stream->m_Info.s_PubName);
			}else{
				//其它数值说明正确
			}

			//add		季成	
			//if((p_H264Stream->m_devType == DEF_SHSL_DEV)&&(NULL != m_Frame))
			if((p_H264Stream->b_Encode == 0)&&(NULL != m_Frame))
				Sleep(m_Frame->i_time>200?0:m_Frame->i_time/2);//可能存在问题，当不同视频流切换的时候 该处数据可能很大。
			else
			{//公路使用中该处sleep为10，
				DWORD delayT;
				stopT = GetTickCount();
				delayT = stopT -startT;
				//Main->pNtgjDlg->MySleep(delayT,p_H264Stream->p_EncodeArray.GetSize());
				if(p_H264Stream->p_EncodeArray.GetSize() < 25)
				{
					//TRACE("*******<25*****\n");
					if(delayT<40 && delayT>=0)//zhh 默认40帧情况下
						Sleep(40-delayT);
				}else
				{
					//TRACE("*******>=25*****\n");
					if(delayT<30 && delayT>=0)//zhh 较多就以30毫 秒为限
						Sleep(30-delayT);
				}

				//Sleep(DEF_PUSHSTREAM_SLEEP_TIME*3);
			}
//add

		}//数据已成功发送
		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}
	}

leave:

	if (m_Frame!=NULL)
	{
		delete m_Frame;
		m_Frame=NULL;
	}

	if(NULL != p_H264Stream->h_stream)
	{
		//Main->pNtgjDlg->ClosePull(p_H264Stream,NULL);
		//TRACE("INFO:该路[%s]视频停止拉流。\n",p_H264Stream->m_Info.s_PubName);

	}
	while(1){

		m_Frame=NULL;//宽，高，I帧等

		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_EncodeArray.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_EncodeArray.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_EncodeArray.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();



		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}else
			break;
	}

	//if(p_H264Stream->m_devType == DEF_SHSL_DEV){
	if(p_H264Stream->b_Encode == 0){
		if(NULL != pPspPps)
		{
			delete []pPspPps;
			pPspPps = NULL;
		}

	}


	p_H264Stream->g_rtmp.Close(&p_H264Stream->g_rtmp);

	p_H264Stream->b_start = false;

	HRESULT  hr=S_OK;
	_endthreadex(hr);
	return hr;

	return 0;


}

UINT _stdcall encodeTask(void* H264Stream)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;

	int rtn=0,i_NoData=0;
	int delayT;
	MyAvFrame *m_Frame=NULL;
	BYTE* pbuff;//=new BYTE[MAXBUFFSIZE];

	static DWORD total = 0;
	static int nT = 0;

	MyAvFrame *noVideoFrame=new MyAvFrame(352,288);
	if (NULL == noVideoFrame)
	{
		m_Log.Add("encodeTask函数中new MyAvFrame分配内存时出现错误！");
		return TRUE;
	}
	if (NULL == noVideoFrame->p_data)
	{
		m_Log.Add("encodeTask函数中m_Frame->p_data分配内存时出现错误！");
		return TRUE;
	}
	//memcpy(noVideoFrame->p_data,pOut->data[0],pOut->width*pOut->height);
	{
		int i_Len,fLen;

		FILE *pf = fopen("sleeplive_YUV420.yuv","rb");//pfile"base_xml.xml"
		if(pf)
		{
			fseek(pf,0,SEEK_END);
			fLen = ftell(pf);	
			fseek(pf,0,SEEK_SET);

			i_Len = fread(noVideoFrame->p_data,1,fLen,pf);

			fclose(pf);	
			m_Log.Add("encodeTask函数中打开sleeplive_YUV420.yuv成功！");
		}else
		{
			m_Log.Add("encodeTask函数中打开sleeplive_YUV420.yuv错误！");
			return TRUE;

		}
	}


	pbuff=new BYTE[MAXBUFFSIZE];
	if(NULL==pbuff)
	{
		m_Log.Add("encodeTask函数中pbuff=new BYTE[MAXBUFFSIZE]分配内存时出现错误！");
	}
	ZeroMemory(pbuff,MAXBUFFSIZE);

	while (true)
	{
		DWORD startT = GetTickCount(),stopT ;

		//Sleep(1);


		if(Main->g_Stop == 1)
			break;

		m_Frame=NULL;//宽，高，I帧等
		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_YuvArray.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_YuvArray.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_YuvArray.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();


		int i_Len=0;
		if (NULL!=m_Frame)//从模板CArray获取到了视频数据
		{
			i_NoData=0;

			if (m_Frame->i_width!=g_VideoWidth/*DEFAULTWIDTH*/ || m_Frame->i_height != g_VideoHeight/* DEFAULTHEIGHT*/)
				//非标准视频数据格式，要对获取的视频数据编码
			{
				SimpleResize m_s(m_Frame->i_width,m_Frame->i_height,g_VideoWidth/*DEFAULTWIDTH*/,g_VideoHeight/*DEFAULTHEIGHT*/,&rtn);
				//构造对象m_s
				if (rtn!=-1)
				{
					m_s.resize((BYTE*)m_Frame->p_data,p_H264Stream->encodebuff);
					//encodebuff<--m_Frame->p_data,把从模板获取的视频数据编码并存到encodebuff
					i_Len=p_H264Stream->Encode(p_H264Stream->encodebuff,pbuff);	
					//把视频数据编码放入临时数组pbuff并计算数据长度
					if(i_Len <=0 )
						m_Log.Add("INFO:该路[%s]编码出错***1。\n",p_H264Stream->m_Info.s_PubName);
				}
				else
				{
					if (NULL!=m_Frame)
					{
						delete m_Frame;
					}
				}
			}
			else//标准视频数据格式，不需要对获取的视频数据编码
			{
				i_Len=p_H264Stream->Encode(m_Frame->p_data,pbuff);
			}
			i_NoData=0;

			if (m_Frame!=NULL)
			{
				delete m_Frame;
				m_Frame=NULL;
			}
		}else
		{
			//TRACE("YUV array is empty t\n");
			//i_Len=p_H264Stream->Encode(pbuff);
			//stopT = GetTickCount();
			//delayT = (stopT-startT);

			//if(delayT<30 && delayT>0)
			//	Sleep(40-delayT);
			
			if(p_H264Stream->p_EncodeArray.GetSize()>0)
			{
				Sleep(10);
				continue;
			}else
			{
				i_NoData++;
#if 1
				if(i_NoData<200)
					i_Len=p_H264Stream->Encode(pbuff);
				//#else			
				else
				{
					if (noVideoFrame->i_width!=g_VideoWidth/*DEFAULTWIDTH*/ || noVideoFrame->i_height !=g_VideoHeight /*DEFAULTHEIGHT*/)
						//非标准视频数据格式，要对获取的视频数据编码
					{
						SimpleResize m_s(noVideoFrame->i_width,noVideoFrame->i_height,g_VideoWidth/*DEFAULTWIDTH*/,g_VideoHeight/*DEFAULTHEIGHT*/,&rtn);
						//构造对象m_s
						if (rtn!=-1)
						{
							m_s.resize((BYTE*)noVideoFrame->p_data,p_H264Stream->encodebuff);
							//encodebuff<--m_Frame->p_data,把从模板获取的视频数据编码并存到encodebuff
							i_Len=p_H264Stream->Encode(p_H264Stream->encodebuff,pbuff);	
							//把视频数据编码放入临时数组pbuff并计算数据长度
							if(i_Len <=0 )
								m_Log.Add("INFO:该路[%s]编码出错***1。\n",p_H264Stream->m_Info.s_PubName);
						}

					}
					else//标准视频数据格式，不需要对获取的视频数据编码
					{
						i_Len=p_H264Stream->Encode(noVideoFrame->p_data,pbuff);

					}
				}

#endif
			}

			stopT = GetTickCount();
			delayT = (stopT-startT);
			if(delayT<40 && delayT>=0)
				Sleep(40 - delayT);
		}

		if(p_H264Stream->p_EncodeArray.GetSize()<60)
		{

			if (0 == i_Len)
			{
				//pMain->m_streamlock.Unlock();
				return TRUE;
			}
			MyAvFrame *tmp_Frame=new MyAvFrame(i_Len,1);
			if (NULL == tmp_Frame)
			{
				m_Log.Add("myVideo函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");
				//pMain->m_streamlock.Unlock();
				return TRUE;
			}
			if (NULL == tmp_Frame->p_data)
			{
				//pMain->m_streamlock.Unlock();
				return TRUE;
			}
			memcpy(tmp_Frame->p_data,pbuff,i_Len);

			p_H264Stream->m_streamlock.Lock();
			p_H264Stream->p_EncodeArray.Add(tmp_Frame);
			p_H264Stream->m_streamlock.Unlock();
		}else
		{
			TRACE("encodeTask:descart this time %d\n",p_H264Stream->p_EncodeArray.GetSize());
		}


#if 0
		total += delayT;
		nT++;
		if(nT%100 == 0)
		{
			TRACE("*******Enocde  100 frame %d\n",total);
			total = 0;
		}
#endif
		//if(delayT<20 && delayT>=0)
		//Sleep(10);
		//else
		//	TRACE("delay time is %d\n",delayT);

	}

leave:

	if (m_Frame!=NULL)
	{
		delete m_Frame;
		m_Frame=NULL;
	}

	if(noVideoFrame != NULL)
	{
		delete noVideoFrame;
	}


	while(1){

		m_Frame=NULL;//宽，高，I帧等
		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_YuvArray.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_YuvArray.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_YuvArray.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();

		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}else
			break;
	}


	if(NULL != pbuff)
	{
		delete []pbuff;
		pbuff =NULL;
	}

	HRESULT  hr=S_OK;
	_endthreadex(hr);
	return hr;

	return 0;


}

int parseHttpHead(char *const buf,int *pLen)
{
	int rtn = 0;
	char *pkey;

	pkey = strstr(buf,"Content-Length:");

	if(pkey != NULL)
	{
		*pLen = atoi(pkey+strlen("Content-Length:"));


	}
	else
	{
		*pLen = 0;
		return -1;
	}

	return rtn;
}


static int recvHttpHead(int s,char *pBuf,int maxLen,int *pLen)
{
	int tmpLen = 0;
	int rtn = 0;
	char *pKeyBuf;

	while(1)
	{
		rtn = recv(s,(char*)(pBuf+tmpLen),1,0);
		if(rtn < 0)
		{
			TRACE("ERROR: recvHttpHead->readDataFrmCommW error recv[%d]\n",tmpLen);

			return -1;
		}

		tmpLen += rtn;

		if(tmpLen >= (maxLen))
		{
			pBuf[maxLen-1] = 0; 
			TRACE("ERROR: the http head is lenger than %d [%s]\n",maxLen,pBuf);

			return -1;
		}

		pKeyBuf = strstr(pBuf, "\r\n\r\n");//从字符串str1中查找是否有字符串str2

		if(pKeyBuf != NULL)
		{
			parseHttpHead(pBuf,pLen);//pLen=Content-Length:data
			return 0;
		}

	}
	return rtn;
}


UINT _stdcall xmlrequire(void* H264Stream)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	int  sock=-1;
	int b_send=0;
	int wait_time=60;//60秒的等待间隔

	time_t old_time=0;
	time_t new_time=0;

	//p_H264Stream->m_Info.s_ServerIP;

	//char ip[]="192.168.0.161";
	//int port=1234;
	//char pubName[]="a1";

	while(1)
	{
		if(Main->g_Stop == 1)
			break;

		Sleep(1000);

		if(1 == p_H264Stream->b_stop)
		{
			Sleep(1000);
			continue;
		}

		if(b_send == 1)
		{	
			//time(&old_time);

			struct sockaddr_in sockaddr;   
			sockaddr.sin_family = AF_INET; 
			sockaddr.sin_port = htons(1234);  
			sockaddr.sin_addr.s_addr = inet_addr(p_H264Stream->m_Info.s_PubServer); 
			sock=-1;
			sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			connect(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)); 			

			int rtn = -1,tmpLen=0,xmlLen=0;
			int dataLen=0;
			char tmpBuf[512*4];

			if(sock == -1)
			{
				TRACE("ERROR: the socket have not created\n");
				closesocket(sock);
				Sleep(1000);
			}
			memset(tmpBuf,0,sizeof(tmpBuf));
			tmpLen += sprintf(tmpBuf + tmpLen, "GET /xlive?query=publish&name=%s HTTP/1.0\r\n",p_H264Stream->m_Info.s_PubName);
			tmpLen += sprintf(tmpBuf + tmpLen, "Host: %s:%d\r\n", p_H264Stream->m_Info.s_PubServer,1234);
			tmpLen += sprintf(tmpBuf + tmpLen, "content-type:application/xml;charset=utf-8\r\n");
			tmpLen += sprintf(tmpBuf + tmpLen, "\r\n");

			rtn=send(sock, tmpBuf, tmpLen, 0);
			if(rtn <= 0)
			{
				closesocket(sock);
				Sleep(1000);
				continue;
			}

			//接收完数据头后，解析数据

			tmpLen = 0;

			rtn = 0;

			memset(tmpBuf,0,sizeof(tmpBuf));
			//接收返回数据HTTP头
			rtn = recvHttpHead(sock,tmpBuf,sizeof(tmpBuf),&dataLen);
			//dataLen为协议长度
			if(rtn == -1)
			{
				TRACE("ERROR:data len is longer than the malloc size\n");
				closesocket(sock);
				Sleep(1000);
				continue;
			}

			TRACE("the xml size is %d\n",dataLen);
			memset(tmpBuf,0,sizeof(tmpBuf));

			if(dataLen>512*4)
			{
				m_Log.Add("ERROR:data len is longer[%d] than the malloc size[%d]\n",dataLen,512*4);
				closesocket(sock);
				Sleep(1000);
				continue;

			}

			while(1)//recv data
			{
				rtn=recv(sock,(char*)(tmpBuf+tmpLen),dataLen-tmpLen,0);
				if(rtn < 0)
				{
					TRACE("ERROR: readDataFrmCommW->readDataFrmCommW error\n");
					closesocket(sock);
					break;					
				}

				tmpLen += rtn;
				if(tmpLen == dataLen)
				{
					closesocket(sock);
					//<publish name="??">-1</publish>

					char srcBuf[128];
					sprintf(srcBuf,"<publish name=\"%s\">0</publish>",p_H264Stream->m_Info.s_PubName);
					char *pKey = strstr(tmpBuf,srcBuf);
					if(pKey!=NULL)
					{
						p_H264Stream->b_stop = 1;

						TRACE("INFO:检测到该路[%s]视频可以停止播放。\n",p_H264Stream->m_Info.s_PubName);
						m_Log.Add("INFO:检测到该路[%s]视频可以停止播放。\n",p_H264Stream->m_Info.s_PubName);
					}else{
						sprintf(srcBuf,"<publish name=\"%s\">-1</publish>",p_H264Stream->m_Info.s_PubName);
						pKey = strstr(tmpBuf,srcBuf);
						if(pKey!=NULL)
						{
							p_H264Stream->b_stop = 1;

							TRACE("INFO:检测到该路[%s]视频可以停止播放。\n",p_H264Stream->m_Info.s_PubName);
							m_Log.Add("INFO:检测到该路[%s]视频可以停止播放。\n",p_H264Stream->m_Info.s_PubName);
						}

					}
					Sleep(100);
					//old_time = 0;
					break;
				}
			}//endif while(1) //recv data



		}//end if(b_send == 1	)

		b_send=0;

		if(1 == p_H264Stream->b_stop)
		{//如果需要停止转码，则old_time不需要进行后面的操作。
			old_time=0;//目的是让下一次的访问时间同该次等待时间相同
			continue;
		}

		time(&new_time);

		if(old_time==0)
		{
			old_time=new_time;
			TRACE("开始时间Reset\n");
		}

		if(new_time-old_time>=wait_time)
		{
			b_send=1;
			old_time=0;//目的是让下一次的访问时间同该次等待时间相同
		}
	}
	return 0;
}



CH264Stream::CH264Stream(void)
{
//	h_encoder=NULL;

// 	CString str="";
// 	str.Format("%d",this);
	m_hPullThread = NULL;
	m_hPushThread = NULL;
	m_hXmlThread = NULL;
	m_hEncodeThread = NULL;

	b_Encode = 0;
	b_PullLive = 0;

	memset(&encoderparams, 0, sizeof(encoderparams));

	memset(&Params, 0, sizeof(Params));

	memset(&m_DevParams, 0, sizeof(New_DEV_PARAMS));

	//memset(&StreamParams, 0, sizeof(StreamParams));
	h_stream=NULL;
	b_start=false;
	nowtime=0;
	oldtime=0;
	usetime=0;

	curTime = 0;
	b_restart = 0;
	b_novideo = 0;

	b_stop = -1;

	YUV420P_picture2 = NULL;


	memset(&m_novideoFile,0,sizeof(m_novideoFile));

	encodebuff=new BYTE[MAXBUFFSIZE];
	decodebuff= new BYTE[MAXBUFFSIZE];
	if(NULL==encodebuff)
	{
		m_Log.Add("CH264Stream构造函数encodebuff=new BYTE[MAXBUFFSIZE]分配内存时出现错误！");
	}
	ZeroMemory(encodebuff,MAXBUFFSIZE);

	p_PostData=new BYTE[1024*4];
	if(NULL==p_PostData)
	{
		m_Log.Add("CH264Stream构造函数p_PostData=new BYTE[1024*64]分配内存时出现错误！");
	}
}

CH264Stream::~CH264Stream(void)
{
	if(NULL != m_hXmlThread)
	{
		WaitForSingleObject(m_hXmlThread,INFINITE);
		CloseHandle( m_hXmlThread );
	}

	if(NULL != m_hEncodeThread)
	{
		WaitForSingleObject(m_hEncodeThread,INFINITE);
		CloseHandle( m_hEncodeThread );
	}

	if(NULL != m_hPullThread)
	{
		WaitForSingleObject(m_hPullThread,INFINITE);
		CloseHandle( m_hPullThread );
	}

	if(NULL != m_hPushThread)
	{
		WaitForSingleObject(m_hPushThread,INFINITE);
		CloseHandle( m_hPushThread );
	}

	delete[] encodebuff;
	delete[] decodebuff;
	if(NULL != m_novideoFile.pData)
		delete []m_novideoFile.pData;

	delete[] p_PostData;
}



int CH264Stream::Encode(BYTE* p_inputbuff, BYTE* p_outputbuff)
{
	memcpy(YUV420P_picture->data[0],p_inputbuff,m_Info.i_Width*m_Info.i_Height);
	memcpy(YUV420P_picture->data[1],&p_inputbuff[m_Info.i_Width*m_Info.i_Height+m_Info.i_Width*m_Info.i_Height/4],m_Info.i_Width*m_Info.i_Height/4);
	memcpy(YUV420P_picture->data[2],&p_inputbuff[m_Info.i_Width*m_Info.i_Height],m_Info.i_Width*m_Info.i_Height/4);
	int size=GDavcodec_encode_video(c,p_outputbuff,MAXBUFFSIZE,YUV420P_picture); 
	return size; 	
}

int CH264Stream::Encode(BYTE* p_outputbuff)//没图像放上一帧
{
	int size=GDavcodec_encode_video(c,encodebuff,MAXBUFFSIZE,YUV420P_picture); 
	memcpy(p_outputbuff,encodebuff,size);
	return size;
}

UINT _stdcall pullstream(void* H264Stream)
{

	int ret= -1;
	CH264Stream* pH264Stream=(CH264Stream*)H264Stream;

	CString strDevId;// = pH264Stream->m_Info.s_DevID;
	enum CONVERT_DEV_TYPE tmpType;


	for(;;)
	{
		if(1 == pH264Stream->b_stop)
		{
			if(pH264Stream->h_stream != NULL)
			{
				TRACE("[INFO]:%s stop this because TIME >300",strDevId);
				m_Log.Add("[INFO]:%s stop this because TIME >300",strDevId);
				//tmpType = pH264Stream->m_devType;
				Main->ClosePullFromSub(H264Stream,tmpType);
				pH264Stream->b_PullLive = 0;
				continue;
			}

				Sleep(1000);
				continue;
			

		}

		if(strDevId != pH264Stream->m_Info.s_DevID)
		{//新的摄像机出现，判断之前是否为打开状态，如果为打开则关闭
			if(pH264Stream->h_stream != NULL)
			{
				TRACE("[INFO]:stop this because %s!=%s\n",strDevId,pH264Stream->m_Info.s_DevID);
				m_Log.Add("[INFO]:stop this because %s!=%s\n",strDevId,pH264Stream->m_Info.s_DevID);
				tmpType = pH264Stream->m_devType;
				Main->ClosePullFromSub(H264Stream,tmpType);
			}

			strDevId = pH264Stream->m_Info.s_DevID;
			tmpType = pH264Stream->m_devType;

			//break;
		}

		if(pH264Stream->h_stream != NULL)
		{//当拉流成功后，循环检测是否打开新的拉流设备
			//if(pH264Stream->b_PullLive++>10)
			if(pH264Stream->b_PullLive++>20)
			{
				TRACE("[INFO]:%s stop this because b_PullLive >10",strDevId);
				m_Log.Add("[INFO]:%s stop this because b_PullLive >10",strDevId);
				tmpType = pH264Stream->m_devType;
				Main->ClosePullFromSub(H264Stream,tmpType);
				pH264Stream->b_PullLive = 0;
			}else{
				Sleep(1000);
				continue;
			}

		}


		if(pH264Stream->m_devType == DEF_VNMP_DEV)
			ret =Main->pCommonDlg[VNMP_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_MICROVIDEO_DEV)
			ret =Main->pCommonDlg[MICROVIDEO_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_NETPOSA_DEV)
			ret =Main->pCommonDlg[NETPOSA_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_H3CVM_DEV)
			ret =Main->pCommonDlg[H3CVM_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_DH_DEV)
			ret =Main->pCommonDlg[DHDEV_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_HK_DEV)
			ret =Main->pCommonDlg[HKDEV_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_SANTACHI_DEV)
			ret =Main->pCommonDlg[SANTACHIDEV_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_VMS_DEV)
			ret =Main->pCommonDlg[VMSDEV_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_H3CIPC_DEV)
			ret =Main->pCommonDlg[H3CIPCDEV_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_RTSP_DEV)
			ret =Main->pCommonDlg[RTSP_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_SHSL_DEV)
			ret =Main->pCommonDlg[SHSL_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_H3CIMOS_DEV)
			ret =Main->pCommonDlg[H3CIMOS_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_NTQY_DEV)
			ret =Main->pCommonDlg[NTQY_INDEX]->StartPull(H264Stream,NULL);
		else if(pH264Stream->m_devType == DEF_NTHD_DEV)
			ret =Main->pCommonDlg[NTHD_INDEX]->StartPull(H264Stream,NULL);	
		else if(pH264Stream->m_devType == DEF_DFHS_DEV)
			ret =Main->pCommonDlg[DFHS_INDEX]->StartPull(H264Stream,NULL);
		else
			break;

		if(ret != 0)
		{
			Sleep(1000);
			continue;
		}
	}

	HRESULT  hr=S_OK;
	_endthreadex(hr);
	return hr;

	return 0;
}

int CH264Stream::StartPull(void *pStream, void *pParams)
{
	unsigned int threadid;

	if(NULL != m_hPullThread)
	{
		//if(WAIT_OBJECT_0 != WaitForSingleObject(m_hPullThread,2000))
		//{
		//	m_Log.Add("[ERR]:无法停止原拉流操作，请稍后再试[%s]，",m_Info.s_DevID);
		//	return -1;

		//}
		//CloseHandle( m_hPullThread );
		return 0;
	}

	b_PullLive++;

	m_hPullThread=(HANDLE)_beginthreadex(NULL,0,pullstream,(void *)this,CREATE_SUSPENDED,&threadid);

	SetThreadPriority(m_hPullThread, THREAD_PRIORITY_NORMAL);

	ResumeThread(m_hPullThread);

	return 0;
}

int CH264Stream::StartPush(int Width, int Height)
{

	m_Info.i_Width=g_VideoWidth;//Width;
	m_Info.i_Height=g_VideoHeight;//Height;

	m_ffmpeglock.Lock();
	
	if(1 == g_PubType)
	{
		m_Info.s_PubType = "mss";
		YUV420P_picture = alloc_picture(PIX_FMT_YUV420P,m_Info.i_Width,m_Info.i_Height);
	}

	//初始化编码引擎，是否需要编码由各个厂商决定。
	//Main->pNtgjDlg->InitEncode(NULL);
#if 1
//	if(m_devType != DEF_SHSL_DEV){
	if((1==b_Encode)&&("wowza" == m_Info.s_PubType)){
		YUV420P_picture = alloc_picture(PIX_FMT_YUV420P,g_VideoWidth,g_VideoHeight);

		
		oc = GDav_alloc_format_context(); 
		st = GDav_new_stream(oc, 0);
		c = st->codec;
		c->codec_id = CODEC_ID_H264;
		c->codec_type = CODEC_TYPE_VIDEO;
		c->bit_rate =g_BitRate;//500;
		//	c->bit_rate_tolerance=400;
		c->width = g_VideoWidth;//m_Info.i_Width;//352;//
		c->height = g_VideoHeight;//m_Info.i_Height;//288;//
		c->time_base.den = 25;//25;
		c->time_base.num = 1;
		c->gop_size = 50;//12; /* emit one intra frame every twelve frames at most */
		c->level=30;
		c->profile=0;//FF_PROFILE_H264_BASELINE;//0;//FF_PROFILE_H264_MAIN

		 //	c->IFrame_distance=25;
		// 	c->
		// 	c->framerate=30;
		c->pix_fmt = PIX_FMT_YUV420P;

		codec = GDavcodec_find_encoder(CODEC_ID_H264);
		int z=GDavcodec_open(c, codec);

		//AfxBeginThread( encodeTask,this,THREAD_PRIORITY_ABOVE_NORMAL);
		unsigned int threadid;
		

		m_hEncodeThread=(HANDLE)_beginthreadex(NULL,0,encodeTask,(void *)this,CREATE_SUSPENDED,&threadid);

		SetThreadPriority(m_hEncodeThread, THREAD_PRIORITY_ABOVE_NORMAL);

		ResumeThread(m_hEncodeThread);
	}
#endif 

	BOOL status=false;
// 	h_encoder = m_encoderstreamProcessOpen(&EncodeStreamParams,&status);
// 	m_encoderstreamProcessStart(h_encoder);
	g_rtmp=g_RTMP;
	g_rtmp.Init(&g_rtmp);
	memset(&g_rtmp.m_MetaData, 0, sizeof(tagMetaData));
	//
	strcpy(g_rtmp.m_MetaData.author, "bxt");
	strcpy(g_rtmp.m_MetaData.presetname,"Custom");
	strcpy(g_rtmp.m_MetaData.videodevice, "USB 视频设备");


	g_rtmp.m_MetaData.framerate = 25;
	g_rtmp.m_MetaData.width = m_Info.i_Width;//352;
	g_rtmp.m_MetaData.height = m_Info.i_Height;//288;

	g_rtmp.m_MetaData.avcprofile = 100;
	g_rtmp.m_MetaData.avclevel =21; //30;//21;张军测试
	g_rtmp.m_MetaData.videodatarate =g_BitRate; //200;
	g_rtmp.m_MetaData.videokeyframe_frequency=25.0;
	g_rtmp.m_MetaData.bAudio = false;
	g_rtmp.m_MetaData.bVideo = true;

	if ("wowza" == m_Info.s_PubType)
	{
// 		while(true)
// 		{
// 			int Result = g_rtmp.Connect(&g_rtmp,m_Info.s_PubServer,STREAMPATH,m_Info.s_PubName);
// 			if (0<Result)
// 			{
// 				break;
// 			}
// 		}UINT __cdecl xmlrequire(void* H264Stream)

		//AfxBeginThread( xmlrequire,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);
		//m_hPullThread = AfxBeginThread( pushstream,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);
		unsigned int threadid;
		if(1 == g_bStop)
		{
			m_hXmlThread=(HANDLE)_beginthreadex(NULL,0,xmlrequire,(void *)this,CREATE_SUSPENDED,&threadid);

			SetThreadPriority(m_hXmlThread, THREAD_PRIORITY_ABOVE_NORMAL);

			ResumeThread(m_hXmlThread);
		}

		//unsigned int threadid;
		m_hPushThread=(HANDLE)_beginthreadex(NULL,0,pushstream,(void *)this,CREATE_SUSPENDED,&threadid);

		SetThreadPriority(m_hPushThread, THREAD_PRIORITY_ABOVE_NORMAL);

		ResumeThread(m_hPushThread);
		
	}else if ("mss" == m_Info.s_PubType)
	{
		unsigned int threadid;

		m_hPushThread=(HANDLE)_beginthreadex(NULL,0,push2mss,(void *)this,CREATE_SUSPENDED,&threadid);

		SetThreadPriority(m_hPushThread, THREAD_PRIORITY_ABOVE_NORMAL);

		ResumeThread(m_hPushThread);
		
	}
	else
	{

	}
	m_ffmpeglock.Unlock();



	return 0;
}

//1024

VOID CH264Stream::enocderInit(WARNNINGMSGCALLBACK	WarnningMsgFunc)
{

	ZeroMemory(&encoderparams,sizeof(encoderparams));

	encoderparams.Audio_on_nums = 0;
	encoderparams.Video_on_nums =1;
	encoderparams.sendBuffSize = 512*1024;
	encoderparams.WarnningMsgFunc =WarnningMsgFunc;
	encoderparams.CallBackParams = NULL;
	//strcpy_s(encoderparams.Url_Name,"http://192.168.0.251:8000/jctest?enablesave=1");
	sprintf(encoderparams.Url_Name,"http://%s:8000/%s?enablesave=1",m_Info.s_PubServer,m_Info.s_PubName);
	strcpy_s(encoderparams.Directory_Name,"");
	encoderparams.videoParams[0].bitrate = 400;
	encoderparams.videoParams[0].framerate = 25;
	encoderparams.videoParams[0].quality = 80;
	encoderparams.videoParams[0].width = m_Info.i_Width;//
	encoderparams.videoParams[0].height = m_Info.i_Height;//
	encoderparams.videoParams[0].IFrame_distance = 50;
	encoderparams.videoParams[0].input_format = TYPE_YV12;//TYPE_YUY2;//


	encoderparams.audioParams[0].wBitsPerSample=8;
	encoderparams.audioParams[0].nSamplesPerSec=8000;
	encoderparams.audioParams[0].nChannels =1;
	encoderparams.audioParams[0].cbSize=800;


}

BOOL CH264Stream::encoderOpen()
{
	BOOL status;
	h_encoder = encoderstreamProcessOpen(&encoderparams,&status);

	if (h_encoder == NULL || status == FALSE)
		return FALSE;

	return TRUE;
}
BOOL CH264Stream::encoderStart()
{

	return encoderstreamProcessStart(h_encoder);

}
VOID CH264Stream::encoderStop()
{

	encoderstreamProcessClose(h_encoder);

}

BOOL CH264Stream::encoderPut(BYTE *dataIn,DWORD dataSize,DWORD metiaType)
{
	memcpy(YUV420P_picture->data[0],dataIn,dataSize);

	return encoderstreamProcessPutDataAddress(h_encoder,(BYTE *)(YUV420P_picture->data[0]),dataSize,metiaType,0);


}

BOOL CH264Stream::encoderPut(DWORD metiaType)
{
	//memcpy(YUV420P_picture->data,dataIn,dataSize);

	return encoderstreamProcessPutDataAddress(h_encoder,(BYTE *)(YUV420P_picture->data[0]),(m_Info.i_Width)*(m_Info.i_Height)*3/2 ,metiaType,0);


}

int CH264Stream::initDeH264(int Width, int Height)
{
	YUV420P_picture2 = alloc_picture(PIX_FMT_YUV420P,Width, Height);

	oc2 = GDav_alloc_format_context(); 
	st2 = GDav_new_stream(oc2, 0);
	c2 = st2->codec;
	c2->codec_id = CODEC_ID_H264;
	c2->codec_type = CODEC_TYPE_VIDEO;
	c2->bit_rate =300;//500;
	//	c->bit_rate_tolerance=400;
	c2->width = Width;//
	c2->height = Height;//
	c2->time_base.den = 50;//25;
	c2->time_base.num = 1;
	c2->gop_size = 12;//12; /* emit one intra frame every twelve frames at most */

	c2->pix_fmt = PIX_FMT_YUV420P;

	codec2 = GDavcodec_find_decoder(CODEC_ID_H264);
	int z=GDavcodec_open(c2, codec2);

	return 0;
}

int CH264Stream::destroyDeH264()
{
	GDavcodec_close(c2);

	GDav_free(YUV420P_picture2);

	YUV420P_picture2 = NULL;

	return 0;
}

int CH264Stream::decoderH264(BYTE *dataIn,DWORD dataSize,BYTE* dataOut,DWORD *outLen)
{
	int z3=0;
	int fd9=GDavcodec_decode_video(c2,YUV420P_picture2,&z3,dataIn,dataSize);
	if (fd9<0)
	{
		return -1;
	}

	unsigned	char *tmp = YUV420P_picture2->data[0];
	int i = 0;
	int m_w= c2->width;;
	int m_h= c2->height;
	int offset = 0;
	for(i=0;i<m_h;i++)
	{
		memcpy(dataOut+offset,tmp + i * YUV420P_picture2->linesize[0],m_w);
		offset += m_w;
		//fwrite(tmp + i * pDlg->SecI420_picture->linesize[0], 1, m_w, pf );
	}

	tmp = YUV420P_picture2->data[2];
	for(i=0;i<m_h/2;i++)
	{
		memcpy(dataOut+offset,tmp + i * YUV420P_picture2->linesize[2],m_w/2);
		offset += m_w/2;
		//fwrite(tmp + i * pDlg->SecI420_picture->linesize[2], 1, m_w/2, pf );
	}
	tmp = YUV420P_picture2->data[1];
	for(i=0;i<m_h/2;i++)
	{
		memcpy(dataOut+offset,tmp + i * YUV420P_picture2->linesize[1],m_w/2);
		offset += m_w/2;
		//fwrite(tmp + i * pDlg->SecI420_picture->linesize[1], 1, m_w/2, pf );
	}
	*outLen = offset;
	//fwrite(pDlg->SecI420_picture->data[0],1,352*288*3/2,pf);
	//fwrite(pDlg->SecI420_picture->data[1],1,352*288/4,pf);
	//fwrite(pDlg->SecI420_picture->data[2],1,352*288/4,pf);

	return 0;

}

