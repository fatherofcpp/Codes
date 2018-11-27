// WMV_TO_H264Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "inc.h"
#include "H264Stream.h"
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/queue.h>*/
#ifndef WIN32
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#endif
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event.h>
//#include <evutil.h>
//#include <event-internal.h>
#include "log.h"
#include <evhttp.h>
CWMV_TO_H264Dlg* Main=NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HMODULE   GetCurrentModule() 
{    
	//   earlier   than   .NET   compiler   (VC   6.0) 

	//   Here 's   a   trick   that   will   get   you   the   handle   of   the   module 
	//   you 're   running   in   without   any   a-priori   knowledge: 
	//   http://www.dotnet247.com/247reference/msgs/13/65259.aspx 

	MEMORY_BASIC_INFORMATION   mbi; 
	static   int   dummy; 
	VirtualQuery(   &dummy,   &mbi,   sizeof(mbi)   ); 

	return   reinterpret_cast <HMODULE> (mbi.AllocationBase); 
}


volatile int is_done = 0;  

void info_log( const char *msg )
{
	m_Log.Add( "%s\n", msg );
}
#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

//http
#include <event.h>

/*************************************************************************************
函数名称：ReadCon
函数描述: parse con.ini which contains localport serNam picPath outsideport
函数调用：creatHttp
全局变量：
参数1:point to ConfParam to save information from con.ini
参数2:path of conf.ini
返 回 值: suc returns 0;fail returns -1
创建时间：2013-12-25
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-25      V1.0        jicheng         创建
**************************************************************************************/

int ReadCon(ConfParam*conf,char*path)
{
	FILE   *pf=NULL;
	char*pCon=NULL;

	char*p21=NULL;
	char*p22=NULL;

	int ret=0;
	int fLen=0;

	char dst21[32]="<serNam>";
	char dst22[32]="</serNam>";

	char tmp[256];
	memset(tmp,0,sizeof(tmp));

// f:\\我的电脑\\项目\\jhttps\\Debug\\config.ini";

	pf=fopen(path,"rb");

	if(pf==NULL){
		goto err;
	}
	fseek(pf,0,SEEK_END);
	fLen= ftell(pf);	
	fseek(pf,0,SEEK_SET);
	pCon=(char*)malloc(fLen);
	if(!pCon){
		goto err;
	}
	memset(pCon,0,fLen);
	ret=fread(pCon,1,fLen,pf);
	if(ret!=fLen){
		goto err;
	}
	fclose(pf);

	p21=strstr(pCon,dst21);
	if(!p21){
		goto err;
	}
	p22=strstr(pCon,dst22);
	if(!p22){
		goto err;
	}

	memset(tmp,0,sizeof(tmp));
	memcpy(conf->SerNam,p21+strlen(dst21),p22-p21-strlen(dst21));
	if(pCon){
		free(pCon);
		pCon=NULL;
	}
	return 0;
err:
	if(pf){
		fclose(pf);
	}
	if(pCon){
		free(pCon);
		pCon=NULL;
	}
	return -1;
}
/*************************************************************************************
函数名称：strToInt
函数描述: transfer string to int
函数调用：CompareTime
全局变量：
参数1:address of the string 
参数2:lenth of the string
返 回 值: the int data transfered to
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

int strToInt(char* p, int len)
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
函数名称：strToFloat
函数描述: transfer string to float
函数调用：
全局变量：
参数1:address of the string 
参数2:lenth of the string
返 回 值: the float data transfered to
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

float strToFloat(char* p, int len)
{
	int i = 0;
	int XiaoShu=0;
	int XiaoShuDian=0;
	float ret = 0.0;
	while(i < len)
	{
		if ((p[i] >= '0') && (p[i] <= '9'))
		{
			int value = p[i]-48;
			ret = ret*10 +value;
			i++;
			if(XiaoShuDian==1)
			{
				XiaoShu++;
			}
		}
		else if(p[i]=='.')
		{
			XiaoShuDian=1;
			i++;			
		}
		else
		{
		  for(i=0;i<XiaoShu;i++)
		  {
			 ret=ret/10;
		  }
		  break;
		}
	}
	return ret;
}
/*************************************************************************************
函数名称：CheckUrlExist
函数描述: check if post url is being detected
函数调用：StartNewDetect
全局变量：CWMV_TO_H264Dlg* Main
参数1:save which index is being detected 
返 回 值： return index if this url is being detected;return 0 if this url is not being detected
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/
int CheckUrlExist(int*ind)
{
	int i=0;
	for(i=0;i<g_regDetWay;i++)
	{
		if(0==strcmp(Main->pNtgjDlg->m_MicrovideoParamsTsk.SrcUrl,Main->pNtgjDlg->m_h264[i]->SrcUrl))
		{
			*ind=i;
			return 1;
		}
	}
	return 0;
}
/*************************************************************************************
函数名称：FindPull
函数描述: find which h264 detect tsk has ended
函数调用：StartNewDetect
全局变量：MaxLocTsk,Main,MaxNetTsk
参数：
输出 :
返 回 值：return index of which h264 can pull video ;return -1 if no h264 can pull video
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/
int FindPull(void)
{
	int i,ret;
//	for(i=g_regDetWay;i<g_regDetWay+MaxNetTsk;i++)MaxLocTsk
	for(i=MaxLocTsk;i<MaxLocTsk+MaxNetTsk;i++)
	{
		if(Main->pNtgjDlg->m_h264[i]->parTskEnd==1 && Main->pNtgjDlg->m_h264[i]->senTskEnd==1 && Main->pNtgjDlg->m_h264[i]->senTskEnd==1)
		{
			return i;
		}
	}
	return -1;
}

int NetTskAllEnd(void)
{
	int i,ret;
	for(i=MaxLocTsk;i<MaxLocTsk+MaxNetTsk;i++)
	{
		if(Main->pNtgjDlg->m_h264[i]->parTskEnd==1 && Main->pNtgjDlg->m_h264[i]->senTskEnd==1 && Main->pNtgjDlg->m_h264[i]->senTskEnd==1)
		{
			ret=0;
		}else{
			return -1;
		}
	}
	return 0;
}
/*************************************************************************************
函数名称：StartNewDetect
函数描述: start detect video via  data post
函数调用：post_cb
全局变量：Main
参数1:array of _YiChang
参数2:number of video to be detected via data post
返 回 值: 0 as success;-1 as no h264 can pull video ;-2 as xml data err
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

int StartNewDetect(_YiChang(YiChang)[16],int TaskNum)
{
	if(TaskNum<=0 || TaskNum>16)
	{
		return -2;
	}
	int i=0,ret=0;
	int Onind=-1;
	int DoPull=1;
	int CanPull=1;
	int PullInd=0;

#if 1

	for(i=0;i<TaskNum;i++)
	{
		// todo:http->%s
		Main->pNtgjDlg->m_MicrovideoParamsTsk.SrcUrl.Format("%s://%s:%s/%s",YiChang[i].StreamProtocol,YiChang[i].SrvIP,YiChang[i].SrvPort,YiChang[i].StreamName);
		Main->pNtgjDlg->m_MicrovideoParamsTsk.nPort=Main->pNtgjDlg->m_MicrovideoParams.nPort;
		Main->pNtgjDlg->m_MicrovideoParamsTsk.ServerIP=Main->pNtgjDlg->m_MicrovideoParams.ServerIP;

		//查看改路视频是否在检测
		ret=CheckUrlExist(&Onind);
		if(ret==1){//该流正在被检测
			Main->pNtgjDlg->m_h264[PullInd]->direct=1;
			Main->pNtgjDlg->m_h264[Onind]->RePullByNetTsk=1;
		//	Main->pNtgjDlg->m_h264[Onind]->TaskEnd=0;
		//	Main->pNtgjDlg->m_h264[Onind]->IsTask=0;
			Main->pNtgjDlg->m_h264[Onind]->NetTskCmId=YiChang[i].CmdId;
			DoPull=0;//?
		}else{//该流不在被检测
			DoPull=1;
		}
		if(DoPull==1)
		{
			//检测16-32中检测结束的way
			ret=FindPull();
			if(PRINT)
				TRACE("[StartNewDetect]:FindPull-ret:%d\n",ret);
			if(ret==-1){//没有不在拉流的h264
				CanPull=0;
				return -1;
			}else{
				CanPull=1;
				PullInd=ret;
			}
			if(CanPull==1)
			{//all reset and set part
				if(NULL!=Main->pNtgjDlg->m_h264[PullInd]->h_stream)
				{
					ProcessClose(Main->pNtgjDlg->m_h264[PullInd]->h_stream);
					Main->pNtgjDlg->m_h264[PullInd]->h_stream=NULL;
				}

				Main->pNtgjDlg->m_h264[PullInd]->direct=1;
				Main->pNtgjDlg->m_h264[PullInd]->IsTask=1;
				Main->pNtgjDlg->m_h264[PullInd]->parTskEnd=0;
				Main->pNtgjDlg->m_h264[PullInd]->senTskEnd=0;
				Main->pNtgjDlg->m_h264[PullInd]->serStaEnd=0;

				Main->pNtgjDlg->m_h264[PullInd]->CmdId=YiChang[i].CmdId;
				Main->pNtgjDlg->m_h264[PullInd]->ServerCmdId=YiChang[i].CmdId;
				Main->pNtgjDlg->m_h264[PullInd]->StreamOffCmdId=YiChang[i].CmdId;

				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.DiuShi,YiChang[i].DiuShi,sizeof(YiChang->DiuShi));
				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.DongJie,YiChang[i].DongJie,sizeof(YiChang->DongJie));
				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.DuiJiao,YiChang[i].DuiJiao,sizeof(YiChang->DuiJiao));
				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.LiangDu,YiChang[i].LiangDu,sizeof(YiChang->LiangDu));
				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.ShanShuo,YiChang[i].ShanShuo,sizeof(YiChang->ShanShuo));
				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.XueHua,YiChang[i].XueHua,sizeof(YiChang->XueHua));
				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.ZheDang,YiChang[i].ZheDang,sizeof(YiChang->ZheDang));
				memcpy(Main->pNtgjDlg->m_h264[PullInd]->YCH.GunWen,YiChang[i].GunWen,sizeof(YiChang->GunWen));

				Main->pNtgjDlg->SetParams(Main->pNtgjDlg->m_h264[PullInd],&(Main->pNtgjDlg->m_MicrovideoParamsTsk));
				Main->pNtgjDlg->StartPull(Main->pNtgjDlg->m_h264[PullInd],&(Main->pNtgjDlg->m_MicrovideoParamsTsk));
			}//end CanPull==1
		}//end DoPull==1
	}//end for(0-TaskNum)
	//
#endif

	return 0;
}
/*************************************************************************************
函数名称：ParseBody
函数描述: parse post xml
函数调用：post_cb
全局变量：MaxNetTsk
参数1:data of xml
参数2:lenth of xml data
参数3:array of YiChang struct
参数4:save how much way of video to be detected
返 回 值：0 as success ; -1 as fail 
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

int ParseBody(const char*data,int len,_YiChang(YiCha)[16],int*TaskNum)
{

	int ret=0,i=0,j=0;
	int way=0;
	int memNum=13;
	int tskMax=MaxNetTsk;
	const char*p=data;
	const char*p0=NULL;
	const char*p1=NULL;
	const char*ptmp=data;
	const char*proot=NULL;
	const char*pTask=NULL;

	const char root0[32]="<root>";
	const char root1[32]="</root>";
	const char Task0[32]="<Task>";
	const char Task1[32]="</Task>";

	char dst[13][32];
	memset(dst,0,sizeof(dst));
	memcpy(dst[0],"CmdID",strlen("CmdID"));
	memcpy(dst[1],"SrvIP",strlen("SrvIP"));
	memcpy(dst[2],"SrvPort",strlen("SrvPort"));
	memcpy(dst[3],"StreamName",strlen("StreamName"));
	memcpy(dst[4],"StreamProtocol",strlen("StreamProtocol"));
	memcpy(dst[5],"DiuShi",strlen("DiuShi"));
	memcpy(dst[6],"ZheDang",strlen("ZheDang"));
	memcpy(dst[7],"DuiJiao",strlen("DuiJiao"));
	memcpy(dst[8],"LiangDu",strlen("LiangDu"));
	memcpy(dst[9],"DongJie",strlen("DongJie"));
	memcpy(dst[10],"XueHua",strlen("XueHua"));
	memcpy(dst[11],"ShanShuo",strlen("ShanShuo"));
	memcpy(dst[12],"GunWen",strlen("GunWen"));

	char*pYc[13];
	memset(pYc,0,sizeof(pYc));

	char tmp0[64];
	char tmp1[64];
	memset(tmp0,0,sizeof(tmp0));
	memset(tmp1,0,sizeof(tmp1));

	p=strstr(data,root0);
	if(p)
	{
		p=strstr(data,root1);
		if(!p)
			goto err;
	}else{
		goto err;
	}
	ptmp=data;
	while(1)
	{
		p=strstr(ptmp,Task1);
		if(p){
			way++;
			ptmp=p+strlen(Task1);
		}else{
			break;
		}
	}
	if(way>tskMax){//?
		goto err;
	}
	*TaskNum=way;

	ptmp=data;
	for(i=0;i<way;i++)
	{
		pYc[0]=YiCha[i].CmdId;
		pYc[1]=YiCha[i].SrvIP;
		pYc[2]=YiCha[i].SrvPort;
		pYc[3]=YiCha[i].StreamName;
		pYc[4]=YiCha[i].StreamProtocol;
		pYc[5]=YiCha[i].DiuShi;
		pYc[6]=YiCha[i].ZheDang;
		pYc[7]=YiCha[i].DuiJiao;
		pYc[8]=YiCha[i].LiangDu;
		pYc[9]=YiCha[i].DongJie;
		pYc[10]=YiCha[i].XueHua;
		pYc[11]=YiCha[i].ShanShuo;
		pYc[12]=YiCha[i].GunWen;
		for(j=0;j<memNum;j++)
		{
			memset(tmp0,0,sizeof(tmp0));
			sprintf(tmp0,"<%s>",dst[j]);
			p0=strstr(ptmp,tmp0);
			if(!p0){
				goto err;
			}
			memset(tmp1,0,sizeof(tmp1));
			sprintf(tmp1,"</%s>",dst[j]);
			p1=strstr(p0,tmp1);
			if(!p1){
				goto err;
			}else{
			//read canshu
				memcpy(pYc[j],p0+strlen(tmp0),p1-p0-strlen(tmp0));
				
			}
		}//end j each task
		ptmp=p1;
		//拉流质量检测?
	}//end i
/*
	<root>
	<Task>
	<CmdID>12C097A5-CF99-BDF9-DC41-3E270C6F372C</CmdID>
	<SrvIP>192.168.0.251</SrvIP>
	<SrvPort >1935</SrvPort>
	<StreamName>ipcamera</StreamName>
	<StreamProtocol>http</StreamProtocol>
	<DiuShi>0.5000</DiuShi>
	<ZheDang>0.5000</ZheDang>
	<DuiJiao>0.5000</DuiJiao>
	<LiangDu>0.5000</LiangDu>
	<DongJie>0.5000</DongJie>
	<XueHua>0.5000</XueHua>
	<ShanShuo>0.5000</ShanShuo>
	</Task >
	</root>
*/
	return 0;

err:
	return -1;
}
/*************************************************************************************
函数名称：post_cb
函数描述: callback function of post action
函数调用：creatHttp
全局变量：
参数1:struct evhttp_request pointer
参数2:struct ConfParam pointer
返 回 值 
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

static void
post_cb(struct evhttp_request *req, void *arg)
{
	int ret=0,i=0;
	int TaskNum=0;
	char *data=NULL;
	ConfParam*conf=(ConfParam*)arg;
	struct evbuffer *evb = NULL;
	struct evbuffer *content=NULL;
	_YiChang YiChang[16];
	memset(YiChang,0,sizeof(YiChang));
	evb = evbuffer_new();

//	/vqd/service/videodetectTaskService
	const char *uri = evhttp_request_uri(req); 
	ret=strcmp(uri,conf->SerNam);
	if(ret!=0){
		goto err;
	}
//read body
	content = evhttp_request_get_input_buffer(req);
	int len = evbuffer_get_length(content)+1;
	if(NULL ==content || len == 1){
		goto err;
	}
	data = (char *)malloc(len);
    memset( data , 0 , len );
    int flag = evbuffer_remove( content , data , len );
	//add log
	m_Log.Add("[post_cb]:recv post data:\n%s",data);
	//end log
//parse body
	ret=ParseBody((const char*)data,len,YiChang,&TaskNum);
	if(ret==0)
	{//parse succ
	//拉流质量检测
		ret=StartNewDetect(YiChang,TaskNum);
		if(ret==0)//begin detect success
		{
			evbuffer_add_printf(evb,
									"<backresult>\r\n"
									"	<id>%d</id>\r\n"
									"	<info>Success</info>\r\n",
									0);
			evbuffer_add_printf(evb,"</backresult>\r\n");
			evhttp_send_reply(req, 200, "OK", evb);
			
			goto done;
		}
		if(ret==-1)
		{//没能找到不在拉流的h264
			evbuffer_add_printf(evb,
									"<backresult>\r\n"
									"	<id>%d</id>\r\n"
									"	<info>RetryLateer</info>\r\n"
									"	<info>TaskNum:%d</info>\r\n",
									0,TaskNum);
			evbuffer_add_printf(evb,"</backresult>\r\n");
			evhttp_send_reply(req, 200, "OK", evb);
			
			goto done;			
		}
		if(ret==-2)//xml task number err
		{
			goto err;
		}
	}
//send

err:
	evbuffer_add_printf(evb,
						"<backresult>\r\n"
						"	<id>%d</id>\r\n"
						"	<info>XMLFormatInvalid</info>\r\n",
						1);
	evbuffer_add_printf(evb,"</backresult>\r\n");
	evhttp_send_reply(req, 200, "OK", evb);
done:
	if (evb)
		evbuffer_free(evb);
	if(data)
		free(data);
}
/*************************************************************************************
函数名称：creatHttp
函数描述: create http server
函数调用：HttpServer
全局变量：
参数:config path
返 回 值： 0 as success ;-1 as fail
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

int
creatHttp(char*path)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int	err,ret;
	struct evhttp *httpd;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	// TODO: 在此添加控件通知处理程序代码

//parse ini
	ConfParam confParams;
	memset(confParams.SerNam,0,sizeof(confParams.SerNam));
	//ret=ReadCon(&confParams,path);
	//if(ret==-1){
	//	printf("[main]:ReadCon_ret==-1\n");
	//	return -1;
	//}
	sprintf(confParams.SerNam,"%s",LocalServerPath.GetString());

//end parse http.ini

	event_init();      

	httpd = evhttp_start("0.0.0.0", LocalPort);

	evhttp_set_gencb(httpd,post_cb, &confParams);     
	evhttp_set_timeout(httpd,2);
	event_dispatch();        

	evhttp_free(httpd);  

	return 0 ;
}
/*************************************************************************************
函数名称：NextDetect
函数描述: check whether detect next some video urls 
函数调用：CycleDetect
全局变量：g_regDetWay,Main
参数:save which way is still being detected
返 回 值： 1 as can detect next videos ;else as cannot detect next videos
创建时间：2014-3-20
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-3-20      V1.0        jicheng         创建
**************************************************************************************/

int NextDetect(int*ind)
{
	int i,ret;

	//for(i=0;i<MaxLocTsk;i++)
	for(i=0;i<g_regDetWay;i++)
	{
		if(Main->pNtgjDlg->m_h264[i]->parTskEnd != 1 || Main->pNtgjDlg->m_h264[i]->serStaEnd != 1 || Main->pNtgjDlg->m_h264[i]->senTskEnd != 1)
		{
			*ind=i;
			return -1;
		}
	}

	return 1;
}
/*************************************************************************************
函数名称：FormTim
函数描述: get current time HHMMSS
函数调用：CompareTime
全局变量：
参数:char* to save current time
返 回 值： 
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18      V1.0        jicheng         创建
**************************************************************************************/

static void FormTim(char*dst)
{
	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	sprintf(dst,"%02d%02d%02d",t->tm_hour,t->tm_min,t->tm_sec);
}
/*************************************************************************************
函数名称：CompareTime
函数描述: compare current time with beginTime and endTime which set in config
函数调用：CycleDetect
全局变量：
参数:
返 回 值： when current time is bettwen beginTime and endTime return 0;else return -1
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18      V1.0        jicheng         创建
**************************************************************************************/
static int CompareTime(void)
{
	time_t t;
	int nNowTim=0;
	int nBeginTim=0;
	int nEndTim=0;
	char tmp[16];
	memset(tmp,0,sizeof(tmp));

	FormTim(tmp);
	nNowTim=strToInt(tmp,6);

	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"%s",WorkBeginTime);
	nBeginTim=strToInt(tmp,6);

	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"%s",WorkEndTime);
	nEndTim=strToInt(tmp,6);

	if(nNowTim>=nBeginTim && nNowTim<=nEndTim){
		return 0;
	}else{
		return -1;
	}
}
/*************************************************************************************
函数名称：CurTim
函数描述: get current time as YYYY-MM-DD hh:mm:ss
函数调用：SendStreamOff，parseTask，serverState，CycleDetect
全局变量：
参数:buffer to save this time
返 回 值： 
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6     V1.0        jicheng         创建
**************************************************************************************/

static void CurTim(char*dst)
{
	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	sprintf(dst,"%4d-%02d-%02d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
}

int ReadVideoConf(void * pThisDir)
{
	char tmp[255];

	CString tmpStr;
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	Main->pNtgjDlg->SourceCount=0;
	while(true)
	{
		if(Main->pNtgjDlg->SourceCount >= MAX_ONLINE)
		{
			m_Log.Add("达到最大转码路数，以下配置停止分析。%d：%d",Main->pNtgjDlg->SourceCount,MAX_ONLINE);
			break;
		}
		//add
		if(Main->pNtgjDlg->SourceCount >= MaxLocTsk)
		{
			m_Log.Add("达到MaxLocTsk，以下配置停止分析");
			break;
		}
		//end add

	//	tmpStr.Format("Source%d",Main->pNtgjDlg->SourceCount);   //single detect   0-32
		tmpStr.Format("Source%d",Main->pNtgjDlg->SourceInd);	//cycle detect ind 0-1000

		char Server[255];
		ZeroMemory(Server,255);
		GetPrivateProfileString("mircovideo",tmpStr,"",Server,255,thisDir);
		Main->pNtgjDlg->m_MicrovideoParams.SrcUrl=Server;

		if("" == Main->pNtgjDlg->m_MicrovideoParams.SrcUrl)
		{
			m_Log.Add("config.ini文件中未能找到南京感动的Source%d",Main->pNtgjDlg->SourceInd);
			Main->pNtgjDlg->SourceInd=0;
			break;
		}
		else//SrcUrl!=NULL
		{
			if(PRINT){
				TRACE("...................[ReadVideoConf]----tmpStr:%s,SourceCount=%d,srcUrl=%s\n",tmpStr,Main->pNtgjDlg->									SourceCount,Server);
			}
			if (NULL!=Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->h_stream)
			{
				ProcessClose(Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->h_stream);
				Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->h_stream=NULL;
			}
			Main->pNtgjDlg->m_MicrovideoParams.ServerIP=Dstip;
			if(PRINT){
				TRACE("[ReadVideoConf]:%d 1\n",Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->thisIndex);
			}
			Main->pNtgjDlg->m_MicrovideoParams.nPort=Dstport;
			if(PRINT){
				TRACE("[ReadVideoConf]:%d 2\n",Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->thisIndex);
			}
			Main->pNtgjDlg->SetParams(Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount],&(Main->pNtgjDlg->m_MicrovideoParams));
			if(PRINT){
				TRACE("[ReadVideoConf]:%d 3\n",Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->thisIndex);
			}
			Main->pNtgjDlg->StartPull(Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount],&(Main->pNtgjDlg->m_MicrovideoParams));
			if(PRINT){
				TRACE("[ReadVideoConf]:%d 4\n",Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->thisIndex);
			}
			Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->parTskEnd=0;
			Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->serStaEnd=0;
			Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->senTskEnd=0;
			if(PRINT){
				TRACE("[ReadVideoConf]:%d 5\n",Main->pNtgjDlg->m_h264[Main->pNtgjDlg->SourceCount]->thisIndex);
			}

		}//end SrcUrl!=NULL
	

		m_Log.Add("初始化通道%d的配置: 参数为:Source%d:%s,Dstip%d:ServerIP:%s, Dstport%d:port:v%d",
			Main->pNtgjDlg->SourceCount,Main->pNtgjDlg->SourceInd,
			Main->pNtgjDlg->m_MicrovideoParams.SrcUrl,Main->pNtgjDlg->SourceInd,
			Main->pNtgjDlg->m_MicrovideoParams.ServerIP,Main->pNtgjDlg->SourceInd,
			Main->pNtgjDlg->m_MicrovideoParams.nPort);//张世俊增加
	
		(Main->pNtgjDlg->SourceCount)++;
		g_regDetWay=Main->pNtgjDlg->SourceCount;
		(Main->pNtgjDlg->SourceInd)++;
		
	}//end while true

	return 0;
}
/*************************************************************************************
函数名称：TestReadConfToArray
函数描述: read config.ini to common array.
函数调用：CycleDetect
全局变量：SrcList
参数1:config.ini path
返 回 值: number of source read from conf.ini
创建时间：2014-4-15
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-4-15      V1.0        jicheng         创建
**************************************************************************************/

int TestReadConfToArray(void * pThisDir)
{
	int i=0;
	CString tmpStr;
	CString thisDir;
	int len=512;

	thisDir.Format("%s",(char*)pThisDir);
	while(1)
	{
		tmpStr.Format("Source%d",i);
		GetPrivateProfileString("mircovideo",tmpStr,"",SrcList[i],255,thisDir);
		if(strlen(SrcList[i])==0 || i>=len-1){
			return i;
		}
		i++;
	
	}
}
/*************************************************************************************
函数名称：TestReadArrayPull
函数描述: Pull video from index frm to frm+MaxLocTsk if video list enable
函数调用：CycleDetect
全局变量：MaxLocTsk,Main,SrcList
参数1:start index of common array
参数2:list lenth in common array
返 回 值: end index of video list
创建时间：2014-4-15
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-4-15      V1.0        jicheng         创建
**************************************************************************************/

int TestReadArrayPull(int frm,int listLen)
{
	int i=0;
	int j=0;
	int to=frm+MaxLocTsk;
	m_Log.Add("[TestReadArrayPull]:frm:%d,listLen:%d",frm,listLen);
	if(frm<0 || frm>=512){
		return -1;
	}
	if(frm+MaxLocTsk>=listLen){
		to=listLen;
	}
    g_regDetWay=to-frm;
	m_Log.Add("[TestReadArrayPull]:frm:%d,to:%d",frm,to);
	for(i=frm;i<to;i++)
	{
		(Main->cycleThreadIndNew)++;
		j=i-frm;
		if(NULL!=Main->pNtgjDlg->m_h264[j]->h_stream)
		{
			m_Log.Add("[TestReadArrayPull]:before ProcessClose");
			ProcessClose(Main->pNtgjDlg->m_h264[j]->h_stream);
			m_Log.Add("[TestReadArrayPull]:after ProcessClose");
			Main->pNtgjDlg->m_h264[j]->h_stream=NULL;
		}
		Main->pNtgjDlg->m_MicrovideoParams.ServerIP=Dstip;
		Main->pNtgjDlg->m_MicrovideoParams.nPort=Dstport;

		Main->pNtgjDlg->m_MicrovideoParams.SrcUrl=SrcList[i];

		m_Log.Add("[TestReadArrayPull]:before setparams");
		Main->pNtgjDlg->SetParams(Main->pNtgjDlg->m_h264[j],&(Main->pNtgjDlg->m_MicrovideoParams));
		m_Log.Add("[TestReadArrayPull]:before startpull");
		Main->pNtgjDlg->StartPull(Main->pNtgjDlg->m_h264[j],&(Main->pNtgjDlg->m_MicrovideoParams));
		m_Log.Add("[TestReadArrayPull]:after startpull");
		Main->pNtgjDlg->m_h264[j]->parTskEnd=0;
		Main->pNtgjDlg->m_h264[j]->serStaEnd=0;
		Main->pNtgjDlg->m_h264[j]->senTskEnd=0;

		m_Log.Add("初始化通道%d的配置: 参数为:Source%d:%s,Dstip%d:ServerIP:%s, Dstport%d:port:v%d",
			j,i,
			Main->pNtgjDlg->m_MicrovideoParams.SrcUrl,i,
			Main->pNtgjDlg->m_MicrovideoParams.ServerIP,i,
			Main->pNtgjDlg->m_MicrovideoParams.nPort);
	}
	if(to==listLen){
		to=0;
	}
	return to;
}
/*************************************************************************************
函数名称：CloseAllVideo
函数描述: 不工作期间关闭所有拉流的路
函数调用：CycleDetect
全局变量：MaxLocTsk
参数：
输出 :
返 回 值：
创建时间：2014-05-12
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014/05/12      V1.0        jicheng         创建
**************************************************************************************/
int CloseAllVideo(void)
{
	int i=0;
	int frm=0;
	int to=MaxLocTsk;
	for(i=frm;i<to;i++)
	{
		if(NULL!=Main->pNtgjDlg->m_h264[i]->h_stream)
		{
			(Main->cycleThreadIndNew)++;
			m_Log.Add("[CloseAllVideo]:before ProcessClose");
			ProcessClose(Main->pNtgjDlg->m_h264[i]->h_stream);
			m_Log.Add("[CloseAllVideo]:after ProcessClose");
			Main->pNtgjDlg->m_h264[i]->h_stream=NULL;
		}
	}
	return 0;
}
/*************************************************************************************
函数名称：CycleDetect
函数描述: 循环检测n路视频线程
函数调用：CWMV_TO_H264Dlg::OnInitDialog()
全局变量：g_regDetWay
参数：
输出 :
返 回 值：
创建时间：2014-04-16
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014/04/16      V1.0        jicheng         创建
**************************************************************************************/

UINT __cdecl CycleDetect(void *p)
{
//1.当配置文件没有视频路的判断,直接认为一个循环结束.
//2.切换视频阻塞问题待解决
	int i=0;
	int rtn=0;
	int Tim=0;

	int det=0;//begin next detect,to read config
	int Ind=0;//index of way detecting video
	int ListLen=0;//len of video list in array 
	int listInd=g_regDetWay;//start from listInd
	int ReadArray=1;//read from array
	int sleepTim=TestCycle;

	char beginTim[255];
	char nowTim[255];

	memset(beginTim,0,sizeof(beginTim));//begin time of process running
	memset(nowTim,0,sizeof(nowTim));//now time of local

	CurTim(beginTim);
	if(ReadArray==1){
		ListLen=TestReadConfToArray(Main->thisDir.GetBuffer(Main->thisDir.GetLength()));
		Main->thisDir.ReleaseBuffer();
	}
	while (1)
	{
		//timer
		if(10000 <= Main->cycleThreadIndNew){
			Main->cycleThreadIndNew = 0;
		}
		(Main->cycleThreadIndNew)++;
		//timer
#if 1
		if(Main->g_Stop == 1){
			m_Log.Add("[CycleDetect]:Main->g_Stop == 1\n");
			break;
		}

		//compare work time
		rtn=CompareTime();//here didnot care if netTsk end
		if(rtn==-1)
		{//off work
			CloseAllVideo();
		//	m_Log.Add("[CycleDetect]:CompareTime ret -1\n");
			det=0;
			Sleep(1000);
			continue;
		}
		//end com work time
#endif
		Sleep(500);
		if(det==1)
		{
			//add
			int cycTim=1;
			if(listInd == 0)
			{
				(Main->killTim)++;
				m_Log.Add("[CycleDetect]:Main->killTim:%d\n",Main->killTim);
				if((Main->killTim)>=cycTim)
				{
				//change for NetTsk
					//all NetTsk end
					rtn=NetTskAllEnd();
					if(rtn==0)//all end
					{
						m_Log.Add("[CycleDetect]:exit");
						exit(0);
					}
				//end change
				}
			}
			//end add

			if(ReadArray==0){
				ReadVideoConf(Main->thisDir.GetBuffer(Main->thisDir.GetLength()));//read from config
				Main->thisDir.ReleaseBuffer();	
			}else{
				listInd=TestReadArrayPull(listInd,ListLen);
			}
		}
		if(parseTest==1)
		{
			Sleep(1000*sleepTim);
			det=1;
		}else{
			det=NextDetect(&Ind);//detect whether can detect next videos
		}
#if 0
		if(Tim%40==0)
		{
			if(PRINT)
			{
				memset(nowTim,0,sizeof(nowTim));
				CurTim(nowTim);
				if(det!=1){

					TRACE("[CycleDetect]:det:%d,Ind:%d,src:%s,parTskEnd:%d,senTskEnd:%d,serStaEnd:%d,h_stream:%p\n",
						det,Ind,Main->pNtgjDlg->m_h264[Ind]->SrcUrl,
						Main->pNtgjDlg->m_h264[Ind]->parTskEnd,
						Main->pNtgjDlg->m_h264[Ind]->senTskEnd,
						Main->pNtgjDlg->m_h264[Ind]->serStaEnd,
						Main->pNtgjDlg->m_h264[Ind]->h_stream);
				}
				TRACE("[CycleDetect]:beginTim:%s,nowTim:%s\n",
					beginTim,nowTim);
			}
			Tim=0;
if(parseTest==1)
{
				det=1;
}
		}
		Tim++;
#endif
	}//end while(1)

	HRESULT  hr=S_OK;
	_endthreadex(hr);

	return 0;
}

int ParseSrcXmlToListArray(char*srcXml,void**dstList,int srcXmlLen)
{
#if 0
<?xml version="1.0" encoding="utf-8" ?>
<backresult>
<id>0</id>
<videoCount>2</videoCount>
<videoList Index="01" Company="淮安市公路管理处" CameraName="G205崔庄">rtmp://10.20.133.119:1935/live/cuizh_142_13</videoList>
<videoList Index="02" Company="淮安市公路管理处" CameraName="G205沿河大桥">rtmp://10.20.133.119:1935/live/yhdq_142_16</videoList>
</backresult>
#endif
	char(*dst)[255]=(char (*)[255])dstList;

	return 0;
}

int WriteListToLocXml(char*pXmlBuf)
{
#if 0
<?xml version="1.0" encoding="utf-8" ?>
<backresult>
<id>0</id>
<videoCount>2</videoCount>
<videoList Index="01" Company="淮安市公路管理处" CameraName="G205崔庄">rtmp://10.20.133.119:1935/live/cuizh_142_13</videoList>
<videoList Index="02" Company="淮安市公路管理处" CameraName="G205沿河大桥">rtmp://10.20.133.119:1935/live/yhdq_142_16</videoList>
</backresult>

<?xml version="1.0" encoding="utf-8"?>
<MES_EXT>
<Index0>1</Index0>
<Company0>淮安市公路管理处1</Company0>
<CameraName0>G201崔庄111</CameraName0>
<videoList0>rtsp://1.1.1.1:1/pub1</videoList0>
<Index1>2</Index1>
<Company1>淮安市公路管理处2</Company1>
<CameraName1>G201崔庄2</CameraName1>
<videoList1>rtsp://1.1.1.1:1/pub2</videoList1>
</MES_EXT>
#endif

	int listNum=0;
	int tmpLen=0;
	char tmpBuf[1024*255];
	char IndexNode[255];
	char Company[255];
	char CameraName[255];
	char videoList[255];
	memset(tmpBuf,0,sizeof(tmpBuf));
	memset(IndexNode,0,sizeof(IndexNode));
	memset(Company,0,sizeof(Company));
	memset(CameraName,0,sizeof(CameraName));
	memset(videoList,0,sizeof(videoList));

	tmpLen += sprintf(tmpBuf + tmpLen, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n");
	tmpLen += sprintf(tmpBuf + tmpLen,"<MES_EXT>\r\n");

	int rt;
	XMLNODE *root = NULL;
	XMLNODE *node = NULL;
	XML xml;

	rt = f_xmlparse_init(&xml, (const char*)pXmlBuf);
	if (rt != 0)
	{
		m_Log.Add("WriteListToLocXml:xml解析错误，xml结构体初始化失败！");
		rt = 400;
		goto leaveOut;
	}

	if(NULL == xml.root)
	{
		m_Log.Add("WriteListToLocXml:xml解析错误，无效的XML文件！");
		rt = 400;
		goto leaveOut;
	}


	if (strcmp(xml.root->name, "backresult") != 0)
	{
		m_Log.Add("WriteListToLocXml:xml解析错误，未能找到backresult节点!");
		rt = 400;
		goto leave;
	}

	root = xml.root;
	node = f_xmlnode_sch_elem_ext(root, "id", MAX_LEN);
	if(node == NULL)
	{
		m_Log.Add("WriteListToLocXml:xml解析错误，未能找到id节点!\n");
		rt = 400;
		goto leave;
	}
	else
	{
		 listNum= atoi(node->data);
		 if(listNum!=0){
			m_Log.Add("WriteListToLocXml:xml解析错误，id=%d节点!\n",listNum);
			rt = 400;
			goto leave;
		 }
	}


DONE:
	tmpLen += sprintf(tmpBuf + tmpLen,"</MES_EXT>\r\n");


leave:
	f_xmlnode_del(&xml);
leaveOut:
	return rt;

	return 0;
}

int	GetSrcXml(char*tmpBuf,int*len)
{
	int tmpLen=0;
	tmpLen += sprintf(tmpBuf + tmpLen, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n");
	tmpLen += sprintf(tmpBuf + tmpLen, "<backresult>\r\n");
	tmpLen += sprintf(tmpBuf + tmpLen, "<id>0</id>\r\n");
	tmpLen += sprintf(tmpBuf + tmpLen, "<videoCount>2</videoCount>\r\n");
	tmpLen += sprintf(tmpBuf + tmpLen, "<videoList Index=\"01\" Company=\"淮安市公路管理处\" CameraName=\"G205崔庄\">rtmp://10.20.133.119:1935/live/cuizh_142_13</videoList>\r\n");
	tmpLen += sprintf(tmpBuf + tmpLen, "<videoList Index=\"02\" Company=\"淮安市公路管理处\" CameraName=\"G205沿河大桥\">rtmp://10.20.133.119:1935/live/yhdq_142_16</videoList>\r\n");
	tmpLen += sprintf(tmpBuf + tmpLen, "</backresult>\r\n");

	*len=tmpLen;
	return 0;
}

/*************************************************************************************
函数名称：GetVideoList
函数描述: 获取待检测视频列表
函数调用：VideoListDetect
全局变量：
参数：
输出 :
返 回 值：
创建时间：2014-10-24
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014/10/24     V1.0        jicheng         创建
**************************************************************************************/
int GetVideoList(void**dstList,int*listLen)
{
	char(*dst)[255]=(char (*)[255])dstList;
	char srcXml[1024];
	int srcXmlLen=0;
	int len=3;
	int ret=0;

	ret=GetSrcXml(srcXml,&srcXmlLen);

	ret=WriteListToLocXml(srcXml);

	ret=ParseSrcXmlToListArray(srcXml,dstList,srcXmlLen);

	TRACE("GetVideoList:srcXml=\n%s\n",srcXml);

	*listLen=len;

	return 0;
}

/*************************************************************************************
函数名称：VideoListDetect
函数描述: 循环检测n路视频线程
函数调用：CWMV_TO_H264Dlg::OnInitDialog()
全局变量：
参数：
输出 :
返 回 值：
创建时间：2014-04-16
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014/09/15      V1.0        jicheng         创建
**************************************************************************************/

UINT __cdecl VideoListDetect(void *p)
{


//1.当配置文件没有视频路的判断,直接认为一个循环结束.
//2.切换视频阻塞问题待解决
	int i=0;
	int rtn=0;
	int Tim=0;

	int det=0;//begin next detect,to read config
	int Ind=0;//index of way detecting video
	int ListLen=0;//len of video list in array 
	int listInd=g_regDetWay;//start from listInd
	int ReadArray=1;//read from array
	int sleepTim=TestCycle;

	char beginTim[255];
	char nowTim[255];

	memset(beginTim,0,sizeof(beginTim));//begin time of process running
	memset(nowTim,0,sizeof(nowTim));//now time of local

	CurTim(beginTim);
	if(ReadArray==1){
		GetVideoList((void**)SrcList,&ListLen);

	}
	while (1)
	{
		//timer
		if(10000 <= Main->cycleThreadIndNew){
			Main->cycleThreadIndNew = 0;
		}
		(Main->cycleThreadIndNew)++;
		//timer
#if 1
		if(Main->g_Stop == 1){
			m_Log.Add("[CycleDetect]:Main->g_Stop == 1\n");
			break;
		}

		//compare work time
		rtn=CompareTime();//here didnot care if netTsk end
		if(rtn==-1)
		{//off work
			CloseAllVideo();
		//	m_Log.Add("[CycleDetect]:CompareTime ret -1\n");
			det=0;
			Sleep(1000);
			continue;
		}
		//end com work time
#endif
		Sleep(500);
		if(det==1)
		{
			//add
			int cycTim=2;//1 for local config,2 for net video list
			if(listInd == 0)
			{
				(Main->killTim)++;
				m_Log.Add("[CycleDetect]:Main->killTim:%d\n",Main->killTim);
				if((Main->killTim)>=cycTim)
				{
				//change for NetTsk
					//all NetTsk end
					rtn=NetTskAllEnd();
					if(rtn==0)//all end
					{
						m_Log.Add("[CycleDetect]:exit");
						exit(0);
					}
				//end change
				}
			}
			//end add

			if(ReadArray==0){
				ReadVideoConf(Main->thisDir.GetBuffer(Main->thisDir.GetLength()));//read from config
				Main->thisDir.ReleaseBuffer();	
			}else{
				listInd=TestReadArrayPull(listInd,ListLen);
			}
		}
		if(parseTest==1)
		{
			Sleep(1000*sleepTim);
			det=1;
		}else{
			det=NextDetect(&Ind);//detect whether can detect next videos
		}
#if 0
		if(Tim%40==0)
		{
			if(PRINT)
			{
				memset(nowTim,0,sizeof(nowTim));
				CurTim(nowTim);
				if(det!=1){

					TRACE("[CycleDetect]:det:%d,Ind:%d,src:%s,parTskEnd:%d,senTskEnd:%d,serStaEnd:%d,h_stream:%p\n",
						det,Ind,Main->pNtgjDlg->m_h264[Ind]->SrcUrl,
						Main->pNtgjDlg->m_h264[Ind]->parTskEnd,
						Main->pNtgjDlg->m_h264[Ind]->senTskEnd,
						Main->pNtgjDlg->m_h264[Ind]->serStaEnd,
						Main->pNtgjDlg->m_h264[Ind]->h_stream);
				}
				TRACE("[CycleDetect]:beginTim:%s,nowTim:%s\n",
					beginTim,nowTim);
			}
			Tim=0;
if(parseTest==1)
{
				det=1;
}
		}
		Tim++;
#endif
	}//end while(1)

	HRESULT  hr=S_OK;
	_endthreadex(hr);

	return 0;

	return 0;
}
/*************************************************************************************
函数名称：HttpServer
函数描述: http server thread
函数调用：CWMV_TO_H264Dlg::OnInitDialog()
全局变量：
参数:
返 回 值： 
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

UINT __cdecl HttpServer(void *p)
{
	int ret=0;
	CString thisDir;
	char fpath[128];
	memset(fpath,0,sizeof(fpath));

	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);     
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	thisDir+="\\http.ini";

	ret=creatHttp(thisDir.GetBuffer(thisDir.GetLength()));
	thisDir.ReleaseBuffer();
	if(ret==0){//succ
	}
	if(ret==-1){//read config fail
		m_Log.Add("creatHttp read config fail\n");
	}

	return 0;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWMV_TO_H264Dlg 对话框




CWMV_TO_H264Dlg::CWMV_TO_H264Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWMV_TO_H264Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wowzalock=CreateMutex(NULL,FALSE,"zhangjun_wowza");
	m_Lock=CreateMutex(NULL,FALSE,"zhangjun_lock");
	g_Stop = 0;
	m_hChangeThread = NULL;
	cycleThreadIndNew=0;
	cycleThreadIndOld=0;
	killTim=0;
}

CWMV_TO_H264Dlg::~CWMV_TO_H264Dlg(void)
{
	DWORD code; 

	while(1)
	{
		if(m_hChangeThread == NULL)
			break;
		GetExitCodeThread(m_hChangeThread, &code); 
		if(code == STILL_ACTIVE) 
		{  
			Sleep(1000);
			continue;

		}else
			break;

	}

	CloseHandle(m_wowzalock);//=CreateMutex(NULL,FALSE,"zhangjun_wowza");
	CloseHandle(m_Lock);//=CreateMutex(NULL,FALSE,"zhangjun_lock");

}

void CWMV_TO_H264Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CWMV_TO_H264Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CWMV_TO_H264Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CWMV_TO_H264Dlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

#define _CRTDBG_MAP_ALLOC    
#include <stdlib.h>    
#include <crtdbg.h>

/*************************************************************************************
函数名称：parseSystem
函数描述: parse conf.ini
函数调用：CWMV_TO_H264Dlg::OnInitDialog()
全局变量：TestCycle,IsSavePicture,IsSaveLog,LogSaveType,PicturePath,
		  LogPath,Dstip,Dstport,WorkBeginTime,WorkEndTime,VedioDetectPath,
		  ServiceStatePath,LocalPort,YiChPercent,MaxLocTsk,MaxNetTsk
参数1:path of conf.ini
返 回 值：0 as success 
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/
int parseSystem(CString thisDir)
	{
#if 0
		g_maxLiveNum = GetPrivateProfileInt("system","MaxLiveNum",-1,thisDir);
		if(g_maxLiveNum>16 || g_maxLiveNum < 0)
		{
			m_Log.Add("读取config.ini文件中MaxLiveNum信息出错，使用默认值：16");
			g_maxLiveNum  = 16;
		}
#endif
		TestCycle = GetPrivateProfileInt("system","TestCycle",-1,thisDir);
		if(TestCycle<1)
		{
			m_Log.Add("读取config.ini文件中TestCycle信息出错，使用默认值：120");
			TestCycle  = 120;
		}	
		IsSavePicture = GetPrivateProfileInt("system","IsSavePicture",-1,thisDir);
		if(IsSavePicture!=0 && IsSavePicture!=1)
		{
			m_Log.Add("读取config.ini文件中IsSavePicture信息出错，使用默认值：0");
			IsSavePicture  = 0;
		}	
		IsSaveLog = GetPrivateProfileInt("system","IsSaveLog",-1,thisDir);
		if(IsSaveLog!=0 && IsSaveLog!=1)
		{
			m_Log.Add("读取config.ini文件中IsSaveLog信息出错，使用默认值：0");
			IsSaveLog  = 0;
		}	
		LogSaveType = GetPrivateProfileInt("system","LogSaveType",-1,thisDir);////0按日期保存不删除；1按1M容量保存超出删除。
		if(LogSaveType != 0  && LogSaveType != 1)
		{
			m_Log.Add("读取config.ini文件中LogSaveType信息出错，使用默认值：1");
			LogSaveType  = 1;
		}	
		char PathTmp[255];
		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","PicturePath","",PathTmp,255,thisDir);
		PicturePath=PathTmp;
		if("" == PicturePath)
		{
			m_Log.Add("读取config.ini文件中PicturePath信息出错，使用默认值：c:/picture");
			PicturePath="c:\\picture";
		}
		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","LogPath","",PathTmp,255,thisDir);
		LogPath=PathTmp;
		if("" == LogPath)
		{
			m_Log.Add("读取config.ini文件中LogPath信息出错，使用默认值：c:/log");
			LogPath="c:\\sendlog";
		}

		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","Dstip","",PathTmp,255,thisDir);
		Dstip=PathTmp;
		if("" == Dstip)
		{
			m_Log.Add("读取config.ini文件中Dstip信息出错");
			return -1;
		}
		Dstport = GetPrivateProfileInt("system","Dstport",-1,thisDir);////0按日期保存不删除；1按1M容量保存超出删除。
		if(Dstport==-1)
		{
			m_Log.Add("读取config.ini文件中Dstport信息出错，使用默认值");
			Dstport  = 1;
		}
		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","WorkBeginTime","",PathTmp,255,thisDir);
		WorkBeginTime=PathTmp;
		if("" == WorkBeginTime)
		{
			m_Log.Add("读取config.ini文件中WorkBeginTime信息出错,使用默认值08:30:00");
			WorkBeginTime="08:30:00";
		}
		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","WorkEndTime","",PathTmp,255,thisDir);
		WorkEndTime=PathTmp;
		if("" == WorkEndTime)
		{
			m_Log.Add("读取config.ini文件中WorkEndTime信息出错,使用默认值18:30:00");
			WorkEndTime="18:30:00";
		}
		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","VedioDetectPath","",PathTmp,255,thisDir);
		VedioDetectPath=PathTmp;
		if("" == VedioDetectPath)
		{
			m_Log.Add("读取config.ini文件中VedioDetectPath信息出错,使用默认值/vqd/service/videodetectreportService");
			VedioDetectPath="/vqd/service/videodetectreportService";
		}
		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","ServiceStatePath","",PathTmp,255,thisDir);
		ServiceStatePath=PathTmp;
		if("" == ServiceStatePath)
		{
			m_Log.Add("读取config.ini文件中ServiceStatePath信息出错,使用默认值/vqd/service/serverdetectreportService");
			ServiceStatePath="/vqd/service/serverdetectreportService";
		}
		LocalPort = GetPrivateProfileInt("system","LocalPort",-1,thisDir);//
		if(LocalPort==-1)
		{
			m_Log.Add("读取config.ini文件中LocalPort信息出错，使用默认值");
			LocalPort  = 87;
		}
		YiChPercent = GetPrivateProfileInt("system","YiChPercent",-1,thisDir);//video exception percent
		if(YiChPercent==-1 || YiChPercent<0 || YiChPercent>100 ||YiChPercent!=10)
		{
			m_Log.Add("读取config.ini文件中YiChPercent信息出错，使用默认值");
			YiChPercent  = 10;
		}
		MaxLocTsk = GetPrivateProfileInt("system","MaxLocTsk",-1,thisDir);//video exception percent
		if(MaxLocTsk==-1 || MaxLocTsk>16)
		{
			m_Log.Add("读取config.ini文件中MaxLocTsk信息出错，使用默认值");
			MaxLocTsk  = 4;
		}
		MaxNetTsk = GetPrivateProfileInt("system","MaxNetTsk",-1,thisDir);//video exception percent
		if(MaxNetTsk==-1 || MaxNetTsk>16)
		{
			m_Log.Add("读取config.ini文件中MaxNetTsk信息出错，使用默认值");
			MaxNetTsk  = 4;
		}
		//LocalServerPath
		ZeroMemory(PathTmp,255);
		GetPrivateProfileString("system","LocalServerPath","",PathTmp,255,thisDir);
		LocalServerPath=PathTmp;
		if("" == LocalServerPath)
		{
			m_Log.Add("读取config.ini文件中LocalServerPath信息出错,使用默认值/vqd/service/videodetectTaskService");
			LocalServerPath="/vqd/service/videodetectTaskService";
		}
		return 0;
	}
//end parse [system]

// CWMV_TO_H264Dlg 消息处理程序

BOOL CWMV_TO_H264Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

#if 0
	int listLen=0;
	GetVideoList((void**)SrcList,&listLen);
	TRACE("listLen=%d\n",listLen);
	exit(0);
#endif
/*
	char *ptest = new char[32];
	ptest = new char[64];
*/
	memset(SrcList,0,sizeof(SrcList));

	avcodec_init();
	av_register_all();
	
	HANDLE test=GetModuleHandle(NULL);
	int a=0;a++;
	CString haha="";
	haha.Format("程序装载地址：0x%08x",test);
		//MessageBox(haha);


	//init video detect 
		int i,j;
		int ObjIndex=4;
		int Lock=1;
		int ret=0;
		char   path[256];	
		char path2[256];

	if(parseTest==0)
	{
		GetModuleFileName(NULL, path, MAX_PATH);
		for (i=0;i<256-3;i++)
		{
			if (path[i]=='e'&&path[i+1]=='x'&&path[i+2]=='e'){
				for (j=i;j>0;j--){
					if (path[j] == '\\'){
						path[j] ='\0';
						goto PATHOK;
					}
				}
			}
		}
	PATHOK:
		sprintf(path2,"%s\\Camera.ini",path);
		memset(AVidDet,0,sizeof(AVidDet));
		IntSiVa_General_API_Ini(APP,"CamAbn",MAX_DETECT_NUM);//32
		for(i=0;i<MAX_DETECT_NUM;i++)//32
		{
			APP[i]->IntSiVa_Parameter_LoadFile(path2);
			APP[i]->IntSiVa_Parameter_Get(&AVidDet[i].Param);
		
			APP[i]->IntSiVa_Parameter_Set((&AVidDet[i].Param));
		}
	//end init video detect
	}


	m_devType = DEF_MICROVIDEO_DEV;//DEF_SHSL_DEV;//

	//m_Log.InitLog("c:\\log.log");
	char cLogPath[255];
	sprintf(cLogPath,"%s\\videoDetect.log",path);
	m_Log.InitLog(cLogPath);

	m_Log.Add(haha);
	Main=this;
	HMODULE hProcessModule=NULL;
	hProcessModule	=  LoadLibraryA("multiprocess.dll");
	ProcessOpen=(STREAMPROCESSOPEN)GetProcAddress(hProcessModule,"streamProcessOpen");
	ProcessStart=(STREAMPROCESSSTART)GetProcAddress(hProcessModule,"streamProcessStart");
	ProcessClose=(STREAMPROCESSCLOSE)GetProcAddress(hProcessModule,"streamProcessClose");

	
	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	//thisDir.ReleaseBuffer();   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	thisDir+="\\config.ini";
	ret=parseSystem(thisDir);
	if(ret!=0){
		return 0;
	}
//add //季成
#if 1
	char secNames[256]={0};//获取thisDir中的section集合:  microvideo0shsl0... 
	char *secName[16];
	int iOffset=0,iIndex=0;//获取的section集合长度包括最后一个0

	GetPrivateProfileSectionNames(secNames,256,thisDir); 

	//"microvideo",0,"shsl",0,0.....
	secName[iIndex] = secNames;
	iIndex ++;

	for(iOffset=0;iOffset<256;iOffset++)
	{
		if(( 0==secNames[iOffset])&&( 0!=secNames[iOffset+1]))
		{
			secName[iIndex] = &secNames[iOffset+1];
			iIndex++;
		}

		if(( 0==secNames[iOffset])&&( 0==secNames[iOffset+1]))
		{
			iOffset++;
			break;
		}
	}
	//TRACE("读取的section数为%d,secName[0]:%s,secName[1]:%s",iIndex,secName[0],secName[1]);


	if(0==strcmp(secName[1],"mircovideo"))
	{
		m_devType = DEF_MICROVIDEO_DEV;
		pNtgjDlg=new CMicrovideoDlg();
		pNtgjDlg->Create(IDD_MICROVIDEO_DIALOG,this);
		//pNtgjDlg->ShowWindow(SW_SHOW);
	}


	g_csType = m_devType;
#endif


	pNtgjDlg->GetInforFromIni(thisDir.GetBuffer(thisDir.GetLength()));
	thisDir.ReleaseBuffer();
	pNtgjDlg->SetDefaultParams(NULL);


	AfxBeginThread( CycleDetect,this,THREAD_PRIORITY_ABOVE_NORMAL);

	if(parseTest==0)
	{
		AfxBeginThread( HttpServer,this,THREAD_PRIORITY_ABOVE_NORMAL);
	}
	
	//add 0915
	//AfxBeginThread( VideoListDetect,this,THREAD_PRIORITY_ABOVE_NORMAL);
	//end add 
	SetTimer(1,1000*TestCycle,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWMV_TO_H264Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWMV_TO_H264Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWMV_TO_H264Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#define DEF_SHSL_TMP_MAX	6



BEGIN_EVENTSINK_MAP(CWMV_TO_H264Dlg, CDialog)
END_EVENTSINK_MAP()


void CWMV_TO_H264Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	g_Stop = 1;

	//Sleep(5000);

	if(NULL != pNtgjDlg)
		delete pNtgjDlg;

	OnCancel();
}


void CWMV_TO_H264Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CWMV_TO_H264Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int cycTim=1;//检测2个周期kill进程

	if(cycleThreadIndNew == cycleThreadIndOld)
	{
		m_Log.Add("[OnTimer]:cycleThreadIndNew:%d,cycleThreadIndOld:%d",cycleThreadIndNew,cycleThreadIndOld);
		exit(0);
	}
	//if((Main->killTim)>=cycTim)
	//{
	//	m_Log.Add("[OnTimer]:Main->killTim:%d",Main->killTim);
	//	exit(0);
	//}
	cycleThreadIndOld=cycleThreadIndNew;
	CDialog::OnTimer(nIDEvent);
}
