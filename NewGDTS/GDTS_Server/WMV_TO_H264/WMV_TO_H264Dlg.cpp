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

#include "BasicDlg.h"
#include "avLib.h"
//#include "dhLib.h"

#include "DahuaDlg.h"
#include "hkDlg.h"
#include "santachiDlg.h"
#include "VmsDlg.h"
#include "h3cIpcDlg.h"
#include "h3cimosDlg.h"
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

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

#if USENEWSTR

void notify_handler(struct evhttp_request *req, void *arg) 
{    
	WaitForSingleObject(Main->m_wowzalock,INFINITE);
		//WaitForSingleObject(m_wowzalock,INFINITE);
	if(EVHTTP_REQ_POST !=req->type)
	{
		//ReleaseMutex(m_wowzalock);
		ReleaseMutex(Main->m_wowzalock);
		return ;
	}

	struct evbuffer *buf;

	buf = evbuffer_new();

	const char *uri = evhttp_request_uri(req); 
	evbuffer *content = evhttp_request_get_input_buffer(req );//NULL;//req->input_buffer->buffer;//evhttp_request_get_input_buffer(req);
	int len = evbuffer_get_length(content)+1;

	if(NULL ==content || len == 1 || len >1024)
	{
		ReleaseMutex(Main->m_wowzalock);
		return ;

	}

	char *data = (char *)malloc(len);
    memset( data , 0 , len );

    int flag = evbuffer_remove( content , data , len );
	
	if (NULL ==content)
	{
		m_Log.Add("空数据");
		evbuffer_add_printf(buf, "400"); 
		evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf); 
		evbuffer_free(buf);

		free(data);
		//ReleaseMutex(m_wowzalock);
		ReleaseMutex(Main->m_wowzalock);
		return;
	}

	//test
	TRACE("[notify_handler]:data:%s",data);
	m_Log.Add("[notify_handler]:data:%s",data);
	//end test

	CString resultStr="";
	int rt;

	if(NULL == Main->pCommonDlg[MICROVIDEO_INDEX])
	{
		Main->pCommonDlg[MICROVIDEO_INDEX]=(CNTGJDlg *)new CMicrovideoDlg();
		Main->pCommonDlg[MICROVIDEO_INDEX]->Create(IDD_MICROVIDEO_DIALOG,NULL);
		//rt = 400;
		//m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
	}/**/
	//Main->CreateNewSub(AfxGetModuleThreadState());
	rt = Main->pCommonDlg[MICROVIDEO_INDEX]->ParseRequest(data,&resultStr);

leaveOut:
	//rt = Main->pNtgjDlg->ParseRequest(data,&resultStr); 
	switch(rt)
	{
	case 400://xml组织错误
		evbuffer_add_printf(buf, "400"); 
		evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf); 
		evbuffer_free(buf);
		break;
	case 503://切换失败
		evbuffer_add_printf(buf, "503"); 
		evhttp_send_reply(req, HTTP_SERVUNAVAIL, "OK", buf); 
		evbuffer_free(buf);
		break;
	case 404://当前视频没有再播放
		evbuffer_add_printf(buf, "404"); 
		evhttp_send_reply(req, HTTP_NOTFOUND, "ERR", buf); 
		evbuffer_free(buf);
		break;
		//http_handle_request( conn->outbuf, request, NULL,NULL,&rt ,NULL);

		break;
	case 200://切换成功
		//http_handle_request( conn->outbuf, request, NULL,NULL,&rt ,resultStr.GetBuffer(resultStr.GetLength()));
		//resultStr.ReleaseBuffer();

		evbuffer_add_printf(buf, "%s",resultStr.GetBuffer(resultStr.GetLength())); 
		resultStr.ReleaseBuffer();
		evhttp_send_reply(req, HTTP_OK, "OK", buf); 
		evbuffer_free(buf);
		break;


	}

	free(data);


	ReleaseMutex(Main->m_wowzalock);
}

#else

void notify_handler(struct evhttp_request *req, void *arg) 
{    
	WaitForSingleObject(Main->m_wowzalock,INFINITE);
		//WaitForSingleObject(m_wowzalock,INFINITE);
	if(EVHTTP_REQ_POST !=req->type)
	{
		//ReleaseMutex(m_wowzalock);
		ReleaseMutex(Main->m_wowzalock);
		return ;
	}

	struct evbuffer *buf;

	buf = evbuffer_new();

	const char *uri = evhttp_request_uri(req); 
	evbuffer *content = evhttp_request_get_input_buffer(req );//NULL;//req->input_buffer->buffer;//evhttp_request_get_input_buffer(req);
	int len = evbuffer_get_length(content)+1;

	if(NULL ==content || len == 1 || len >1024)
	{
		ReleaseMutex(Main->m_wowzalock);
		return ;

	}

	char *data = (char *)malloc(len);
    memset( data , 0 , len );

    int flag = evbuffer_remove( content , data , len );
	
	if (NULL ==content)
	{
		m_Log.Add("空数据");
		evbuffer_add_printf(buf, "400"); 
		evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf); 
		evbuffer_free(buf);

		free(data);
		//ReleaseMutex(m_wowzalock);
		ReleaseMutex(Main->m_wowzalock);
		return;
	}

	//test
	m_Log.Add("[notify_handler]:data:%s",data);
	//end test

	CString resultStr="";
	int rt;

	{
		XMLNODE *root = NULL;
		XMLNODE *subroot = NULL;

		XMLNODE *node = NULL,*chNode=NULL;
		XML xml;

		CString CH;
		//CString tmpSource,CH,tmpType,tmpVer,tmpIp, tmpPort, pubserverip;
		//tmpSource = m_Info.s_DevID;
		//tmpDstip = m_Info.s_PubServer;

		rt = f_xmlparse_init(&xml, (const char*)data);
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
			m_Log.Add("xml解析错误，未能找到command节点!");
			rt = 400;
			goto leave;
		}

		root = xml.root;


		node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);

		if(node == NULL )
		{
			m_Log.Add("xml解析错误，未能找到company节点!");
			rt = 400;
		}
		else if(atoi(node->data) ==1)
		{
			if(NULL == Main->pCommonDlg[MICROVIDEO_INDEX])
			{
				Main->pCommonDlg[MICROVIDEO_INDEX]=(CNTGJDlg *)new CMicrovideoDlg();
				Main->pCommonDlg[MICROVIDEO_INDEX]->Create(IDD_MICROVIDEO_DIALOG,NULL);
				//rt = 400;
				//m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
				//goto leave;
			}/**/
			//Main->CreateNewSub(AfxGetModuleThreadState());
			rt = Main->pCommonDlg[MICROVIDEO_INDEX]->ParseRequest(data,&resultStr);
		}else if(atoi(node->data) ==3)
		{
			if(NULL == Main->pCommonDlg[HKDEV_INDEX])
			{
				//Main->pCommonDlg[HKDEV_INDEX]=(CNTGJDlg *)new CHKDlg();
				//Main->pCommonDlg[HKDEV_INDEX]->Create(IDD_HK_DIALOG,Main);
				rt = 400;
				m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
				goto leave;
			}
			rt = Main->pCommonDlg[HKDEV_INDEX]->ParseRequest(data,&resultStr);
		}else if(atoi(node->data) ==4)
		{
			if(NULL == Main->pCommonDlg[DHDEV_INDEX])
			{
				//Main->pCommonDlg[DHDEV_INDEX]=(CNTGJDlg *)new CDahuaDlg();
				//Main->pCommonDlg[DHDEV_INDEX]->Create(IDD_DAHUA_DIALOG,Main);
				rt = 400;
				m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
				goto leave;
			}
			rt = Main->pCommonDlg[DHDEV_INDEX]->ParseRequest(data,&resultStr);
		}else if(atoi(node->data) ==10)
		{
			if(NULL == Main->pCommonDlg[NETPOSA_INDEX])
			{
				//Main->pCommonDlg[NETPOSA_INDEX]=(CNTGJDlg *)new CNetPosaDlg();
				//Main->pCommonDlg[NETPOSA_INDEX]->Create(IDD_NETPOSA_DIALOG,Main);
				rt = 400;
				m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
				goto leave;
			}
			rt = Main->pCommonDlg[NETPOSA_INDEX]->ParseRequest(data,&resultStr);
		}else if(atoi(node->data) ==11)
		{
			if(NULL == Main->pCommonDlg[VNMP_INDEX])
			{
				//Main->pCommonDlg[VNMP_INDEX]=(CNTGJDlg *)new CVNMPDlg();
				//Main->pCommonDlg[VNMP_INDEX]->Create(IDD_VNMP_DIALOG,Main);
				rt = 400;
				m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
				goto leave;
			}
			rt = Main->pCommonDlg[VNMP_INDEX]->ParseRequest(data,&resultStr);
		}else if(atoi(node->data) == 12)
		{
			if(Main->pCommonDlg[SANTACHIDEV_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				//Main->pCommonDlg[SANTACHIDEV_INDEX]=(CNTGJDlg *)new CSantachiDlg();
				//Main->pCommonDlg[SANTACHIDEV_INDEX]->Create(IDD_SANTACHI_DIALOG,Main);
				rt = 400;
				m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
				goto leave;
			}
			rt = Main->pCommonDlg[SANTACHIDEV_INDEX]->ParseRequest(data,&resultStr);
		}else if(atoi(node->data) ==13)
		{
			if(NULL == Main->pCommonDlg[VMSDEV_INDEX])
			{
				//Main->pCommonDlg[VMSDEV_INDEX]=(CNTGJDlg *)new CVmsDlg();
				//Main->pCommonDlg[VMSDEV_INDEX]->Create(IDD_VMS_DIALOG,Main);
				rt = 400;
				m_Log.Add("该company编号为[%d]对象尚未创建!",atoi(node->data));
				goto leave;
			}
			rt = Main->pCommonDlg[VMSDEV_INDEX]->ParseRequest(data,&resultStr);
		}else if(atoi(node->data) ==2)//h3c
		{
			subroot = f_xmlnode_sch_elem(root, "h3c");
			if(subroot==NULL)
			{
				m_Log.Add("xml解析错误，未能找到h3c节点!");
				rt = 400;
				goto leave;
			}
			else
			{ 
				node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
				if (node != NULL)
				{
					if(0 == strcmp(node->data,"ipc"))
					{
						if(NULL == Main->pCommonDlg[H3CIPCDEV_INDEX])
						{
							Main->pCommonDlg[H3CIPCDEV_INDEX]=(CNTGJDlg *)new CVmsDlg();
							Main->pCommonDlg[H3CIPCDEV_INDEX]->Create(IDD_VMS_DIALOG,Main);
						}
						rt = Main->pCommonDlg[H3CIPCDEV_INDEX]->ParseRequest(data,&resultStr);
					}
					
					if(0 == strcmp(node->data,"imos"))
					{
						if(NULL == Main->pCommonDlg[H3CIMOS_INDEX])
						{
							Main->pCommonDlg[H3CIMOS_INDEX]=(CNTGJDlg *)new CH3CIMOSDlg();
							Main->pCommonDlg[H3CIMOS_INDEX]->Create(IDD_H3CIMOS_DIALOG,Main);
						}
						rt = Main->pCommonDlg[H3CIMOS_INDEX]->ParseRequest(data,&resultStr);
					}
				}
				//add
				else
				{
					if(NULL == Main->pCommonDlg[H3CIMOS_INDEX])
					{
						Main->pCommonDlg[H3CIMOS_INDEX]=(CNTGJDlg *)new CH3CIMOSDlg();
						Main->pCommonDlg[H3CIMOS_INDEX]->Create(IDD_H3CIMOS_DIALOG,Main);
					}
					rt = Main->pCommonDlg[H3CIMOS_INDEX]->ParseRequest(data,&resultStr);
				}
				//end add
				//else
				//{
				//	m_Log.Add("xml解析错误，未能找到type节点!");
				//	rt = 400;
				//	goto leave;
				//}
			}
		}
		//add NTGJ
		else if(atoi(node->data) == 5)//SHSL
		{
			if(Main->pCommonDlg[SHSL_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				Main->pCommonDlg[SHSL_INDEX]=(CNTGJDlg *)new CNTGJDlg();
				Main->pCommonDlg[SHSL_INDEX]->Create(IDD_NTGJ_DIALOG,Main);
			}
			rt = Main->pCommonDlg[SHSL_INDEX]->ParseRequest(data,&resultStr);
		}
		//end add NTGJ
		//add NTQY
		else if(atoi(node->data) == 7)//NTQY
		{
			if(Main->pCommonDlg[NTQY_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				Main->pCommonDlg[NTQY_INDEX]=(CNTGJDlg *)new CNtqyDlg();
				Main->pCommonDlg[NTQY_INDEX]->Create(IDD_NTQY_DIALOG,Main);
			}
			rt = Main->pCommonDlg[NTQY_INDEX]->ParseRequest(data,&resultStr);
		}
		//end add NTQY
		//add NTHD
		else if(atoi(node->data) == 8)//NTHD
		{
			if(Main->pCommonDlg[NTHD_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				Main->pCommonDlg[NTHD_INDEX]=(CNTGJDlg *)new CNthdDlg();
				Main->pCommonDlg[NTHD_INDEX]->Create(IDD_NTHD_DIALOG,Main);
			}
			rt = Main->pCommonDlg[NTHD_INDEX]->ParseRequest(data,&resultStr);
		}
		//end add NTHD
		//add DFHS
		else if(atoi(node->data) == 6)//DFHS
		{
			if(Main->pCommonDlg[DFHS_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				Main->pCommonDlg[DFHS_INDEX]=(CNTGJDlg *)new CDfhsDlg();
				Main->pCommonDlg[DFHS_INDEX]->Create(IDD_DFHS_DIALOG,Main);
			}
			rt = Main->pCommonDlg[DFHS_INDEX]->ParseRequest(data,&resultStr);
		}
		//end add DFHS
		else
		{
			rt = 400;
			m_Log.Add("xml暂时不支持该company编号为[%d]!",atoi(node->data));
		}


	

leave:
		f_xmlnode_del(&xml);
		
	}

leaveOut:

	//rt = Main->pNtgjDlg->ParseRequest(data,&resultStr); 

	switch(rt)
	{
	case 400://xml组织错误
		evbuffer_add_printf(buf, "400"); 
		evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf); 
		evbuffer_free(buf);
		break;
	case 503://切换失败
		evbuffer_add_printf(buf, "503"); 
		evhttp_send_reply(req, HTTP_SERVUNAVAIL, "OK", buf); 
		evbuffer_free(buf);
		break;
	case 404://当前视频没有再播放
		evbuffer_add_printf(buf, "404"); 
		evhttp_send_reply(req, HTTP_NOTFOUND, "ERR", buf); 
		evbuffer_free(buf);
		break;
		//http_handle_request( conn->outbuf, request, NULL,NULL,&rt ,NULL);

		break;
	case 200://切换成功
		//http_handle_request( conn->outbuf, request, NULL,NULL,&rt ,resultStr.GetBuffer(resultStr.GetLength()));
		//resultStr.ReleaseBuffer();

		evbuffer_add_printf(buf, "%s",resultStr.GetBuffer(resultStr.GetLength())); 
		resultStr.ReleaseBuffer();
		evhttp_send_reply(req, HTTP_OK, "OK", buf); 
		evbuffer_free(buf);
		break;


	}

	free(data);

#if 0

#endif
	ReleaseMutex(Main->m_wowzalock);
}

#endif
//#include "klhttp.h"
//#include "klhttp-internal.h"

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

void handle_request( struct http_connection *conn, const struct http_request *request, void *arg )
{
//	int rt;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
//	XML xml;

	if(NULL == request)
		return;

	m_Log.Add("切换或查询进入计时%u",GetTickCount());
#if 0
	rt = Main->pNtgjDlg->ParseRequest((char*)&request->body.data[0],&resultStr); 

	switch(rt)
	{
	case 400://xml组织错误
	case 503://切换失败
	case 404://当前视频没有再播放
		http_handle_request( conn->outbuf, request, NULL,NULL,&rt ,NULL);

		break;
	case 200://切换成功
		http_handle_request( conn->outbuf, request, NULL,NULL,&rt ,resultStr.GetBuffer(resultStr.GetLength()));
		resultStr.ReleaseBuffer();
		break;

	}
#endif
	m_Log.Add("切换或查询完成离开%u",GetTickCount());
	//if (arg == NULL)
	//	http_handle_request( conn->outbuf, request, NULL, NULL );
	//else
	//	http_handle_request( conn->outbuf, request, file_exist, load_file );
}

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "command.h"
//;
//int nFrameSize = 0;
//int nRecvSize = 0;
int orgfd=0;

extern BOOL myVideo(BYTE *pOut,DWORD width,DWORD height,int devId);

int FormatReplyData(char*dst,CH264Stream *pH264Stream)
{
	char *buf = dst;
	MICP_HDR *pTmp = (MICP_HDR *)buf;
	int nOffset=sizeof(MICP_HDR);
	//nOffset 

	nOffset += sprintf(buf+nOffset,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	nOffset += sprintf(buf+nOffset,"<command>\r\n");
	nOffset += sprintf(buf+nOffset,"<serverip>%s</serverip>\r\n",pH264Stream->m_DevParams.m_dstIp);//dstIp
	nOffset += sprintf(buf+nOffset,"<company>%s</company>\r\n",pH264Stream->m_DevParams.m_companyName);
	nOffset += sprintf(buf+nOffset,"<%s>\r\n",pH264Stream->m_DevParams.m_companyName);//company name
	nOffset += sprintf(buf+nOffset,"<type>%s</type>\r\n",pH264Stream->m_DevParams.m_deviceType);
	nOffset += sprintf(buf+nOffset,"<ver>0</ver>\r\n");
	nOffset += sprintf(buf+nOffset,"<ip>%s</ip>\r\n",pH264Stream->m_DevParams.m_proxyIp);
	nOffset += sprintf(buf+nOffset,"<port>%d</port>\r\n",pH264Stream->m_DevParams.m_proxyPort);
	nOffset += sprintf(buf+nOffset,"<url>%s</url>\r\n",pH264Stream->m_DevParams.m_srcUrl);
	nOffset += sprintf(buf+nOffset,"<user>%s</user>\r\n",pH264Stream->m_DevParams.m_userName);
	nOffset += sprintf(buf+nOffset,"<passwd>%s</passwd>\r\n",pH264Stream->m_DevParams.m_passWord);
	nOffset += sprintf(buf+nOffset,"<channel>%s</channel>\r\n",pH264Stream->m_DevParams.m_deviceChn);
//	nOffset += sprintf(buf+nOffset,"<devicename>%s</devicename>\r\n",pH264Stream->m_DevParams.m_deviceName);
	nOffset += sprintf(buf+nOffset,"</%s>\r\n",pH264Stream->m_DevParams.m_companyName);//company name
	nOffset += sprintf(buf+nOffset,"<ch>%d</ch>\r\n",pH264Stream->m_DevParams.m_dstStream);
	nOffset += sprintf(buf+nOffset,"</command>\r\n");


	pTmp->size      = nOffset;
	pTmp->dev   	 = 0x03;//"转码服务器" 和 "拉流程序" 通讯
	pTmp->direct 	 = 0x01;// "转码服务器" ==>"拉流程序"
	pTmp->type      = 0x01;//请求参数
	pTmp->devid     = 0;//reversed
	pTmp->body_len  = nOffset-sizeof(MICP_HDR);
	pTmp->seq		 = 0;//reversed
	pTmp->ext_info  = 0x00;//reversed
	pTmp->ident     = 0x00003001;//拉流子程序
	pTmp->none1	=0;//reversed
	pTmp->none2  =0;//reversed
	return nOffset;
}

MS_INT32 parseFirstRequire(MS_CHAR *const buf)
{
	MICP_HDR *pTmp = (MICP_HDR *)buf;

	if(pTmp->dev  != 0x03)//"转码服务器" 和 "拉流程序" 通讯
	{
		TRACE("parseFirstRequire:pTmp->dev[%d]  != 0x03\n",pTmp->dev);
		goto leave;
	}
	if(pTmp->direct != 0x02)//  "拉流程序" ==> "转码服务器"
	{
		TRACE("parseFirstRequire:pTmp->direct[%d] != 0x02\n",pTmp->direct);

		goto leave;
	}
	if(pTmp->type   != 0x01)//请求参数
	{
		TRACE("parseFirstRequire:pTmp->type[%d]   != 0x01\n",pTmp->type);

		goto leave;
	}

	if(pTmp->size - pTmp->body_len !=sizeof(MICP_HDR))
	{
		TRACE("parseFirstRequire:ppTmp->size - pTmp->body_len !=sizeof-MICP_HDR\n");

		goto leave;
	}


	return 0;
leave:
	return -1;

}



MS_INT32 parseFrameHead(MS_CHAR *const buf)
{
	MICP_HDR *pTmp = (MICP_HDR *)buf;

	if(pTmp->dev  != 0x03)//"转码服务器" 和 "拉流程序" 通讯
	{
		TRACE("parseFrameHead:pTmp->dev[%d]  != 0x03\n",pTmp->dev);
		goto leave;
	}
	if(pTmp->direct != 0x02)//  "拉流程序" ==> "转码服务器"
	{
		TRACE("parseFrameHead:pTmp->direct[%d] != 0x02\n",pTmp->direct);

		goto leave;
	}
	if(pTmp->type   != 0x03)// 图像的YUV数据
	{
		TRACE("parseFrameHead:pTmp->type[%d]   != 0x03\n",pTmp->type);

		goto leave;
	}

	if(pTmp->size - pTmp->body_len !=sizeof(MICP_HDR))
	{
		TRACE("parseFrameHead:ppTmp->size - pTmp->body_len !=sizeof-MICP_HDR\n");

		goto leave;
	}


	return 0;
leave:
	return -1;

}


static void
echo_read_cb(struct bufferevent *bev, void *ctx)
{
        /* This callback is invoked when there is data to read on bev. */
        struct evbuffer *input = bufferevent_get_input(bev);
        struct evbuffer *output = bufferevent_get_output(bev);
		struct evbuffer *buf = evbuffer_new();
 
        /* Copy all the data from the input buffer to the output buffer. */
		int len = (int)evbuffer_get_length(input)+1;
		char * data=NULL;
		int ret=0;

		if(NULL ==input)// || len == 1 || len >1024)
		{
			return ;
		}
		data = (char*)malloc(len);

		//char *data = (char *)malloc(len);
		memset( data , 0 , len );
		//将TCP内容存放到DATA中
		evbuffer_remove( input , data , len );
		
	//	m_Log.Add("echo_read_cb :dataLen[%d]",len);
		len -= 1;

		int i=0;
		int bIsHead = 0;
	//	TRACE("[echo_read_cb]:len is  %d\n",len );
		//if(orgfd != bev->ev_read.ev_fd )
		//{
		//	TRACE("[RECV]:len is  %p\n",bev );
		//	bIsHead = 1;
		//	orgfd = bev->ev_read.ev_fd ;

		//}
		//if(bev->)



		for(i =0;i<16;i++)
		{//查找当前连接是否之前连接过
			if((Main->m_subInfor[i].p == (void*)bev->ev_read.ev_fd)
				||(Main->m_subInfor[i].p1 == (void*)bev))
				break;
		}

		if(i == 16)//没有找到 说明当前连接为首次连接，则其必发送关键信息
			bIsHead = 1;


		//if((Main->m_subInfor[i].bIsHead == 1)||(bIsHead == 1))
		if(bIsHead == 1)//first connect
		{
			if((len+0)>=sizeof(MICP_HDR))
			{
				MICP_HDR *pmicp_hdr = (MICP_HDR *)data;
				TRACE("echo_read_cb:[%d]bIsHead 1.0 body size is %d:%d:%p:%d\n",pmicp_hdr->devid,pmicp_hdr->size,pmicp_hdr->seq,bev,bev->ev_read.ev_fd);
			//	TRACE("the fd is %d\n",evconnlistener_get_fd(bev));

				
				//TODO:firt require
				ret=parseFirstRequire(data);
				if(ret !=0)
				{
					TRACE("echo_read_cb:[%d]parseFirstRequire fail,goto leaveOut,ev_fd[%p],bev[%p]\n",pmicp_hdr->devid,(void*)bev->ev_read.ev_fd,(void*)bev);
					m_Log.Add("echo_read_cb:[%d]parseFirstRequire fail,goto leaveOut,bev[%p]\n",pmicp_hdr->devid,bev);
					bufferevent_free(bev);
					goto leaveOut;
				}

				//if(pmicp_hdr->devid<0 || pmicp_hdr->devid>15){
				//	TRACE("echo_read_cb:pmicp_hdr->devid[%d],goto leaveOut",pmicp_hdr->devid);
				//	goto leaveOut;
				//}

				CH264Stream *pH264Stream = m_h264[pmicp_hdr->devid];

				m_Log.Add("echo_read_cb :pmicp_hdr->devid[%d]",pmicp_hdr->devid);

				int dataLen=FormatReplyData((char*)pH264Stream->p_PostData,pH264Stream);

				TRACE("echo_read_cb :[%d]body_len[%d],pH264Stream->p_PostData[%s]\n",pmicp_hdr->devid,((MICP_HDR*)pH264Stream->p_PostData)->body_len,pH264Stream->p_PostData+sizeof(MICP_HDR));

				evbuffer_add(output,pH264Stream->p_PostData,dataLen);
			//	evbuffer_add_printf(buf,"%s",pH264Stream->p_PostData);
			//	evbuffer_add_buffer(output, buf);
				
			//	m_Log.Add("echo_read_cb :evbuffer_add_buffer[%s]",ReplyData);

				//连接的唯一性记录
				Main->m_subInfor[pmicp_hdr->devid].p =(void*)bev->ev_read.ev_fd;
				Main->m_subInfor[pmicp_hdr->devid].p1 =(void*)bev;

				Main->m_subInfor[pmicp_hdr->devid].bIsHead = 1;

				//nFrameSize = pmicp_hdr->size;

				//nRecvSize = len;
				//bIsHead = 0;
			}else//if((len+0)>=sizeof(MICP_HDR))
			{
				TRACE("echo_read_cb :bIsHead == 1:len[%d]<sizeof(MICP_HDR\n",len);
				m_Log.Add("echo_read_cb :bIsHead == 1:len[%d]<sizeof(MICP_HDR\n",len);

				bufferevent_free(bev);
				//Main->m_subInfor[i].p = NULL;

			}
		}//end first connect
#if ADDFRAMELEN
		else //if(Main->m_subInfor[i].bIsHead == 1)
		{
			int nSrcLen = len;
			int nDstLen = 0;
			int nOffset = 0;

			while(nSrcLen>0)
			{

				if(Main->m_subInfor[i].m_MicpSize == sizeof(MICP_HDR))
				{//表明头已经接收完成
					if(Main->m_subInfor[i].m_RecvSize == Main->m_subInfor[i].m_MicpHdr.body_len)
					{//收全frame
						Main->m_subInfor[i].m_MicpSize = 0;
						Main->m_subInfor[i].m_RecvSize = 0;
						m_Log.Add("echo_read_cb:[%d]Main->m_subInfor[i].m_RecvSize == Main->m_subInfor[i].m_MicpHdr.body_len",i);
						continue;

					}else
					{//没有收全frame
						nDstLen = Main->m_subInfor[i].m_MicpHdr.body_len;

						if((nSrcLen+Main->m_subInfor[i].m_RecvSize)<nDstLen)
						{
							memcpy(Main->m_subInfor[i].pRecvBuf+Main->m_subInfor[i].m_RecvSize,data+nOffset,nSrcLen);//offset原因
							Main->m_subInfor[i].m_RecvSize += nSrcLen;
							break;
						}

						if((nSrcLen+Main->m_subInfor[i].m_RecvSize)>=nDstLen)
						{
							memcpy(Main->m_subInfor[i].pRecvBuf+Main->m_subInfor[i].m_RecvSize,data+nOffset,nDstLen-Main->m_subInfor[i].m_RecvSize);

							myVideo((BYTE *)Main->m_subInfor[i].pRecvBuf,
								Main->m_subInfor[i].m_MicpHdr.none1,
								Main->m_subInfor[i].m_MicpHdr.none2,i);

							nSrcLen -= (nDstLen-Main->m_subInfor[i].m_RecvSize);
							nOffset += (nDstLen-Main->m_subInfor[i].m_RecvSize);
							Main->m_subInfor[i].m_MicpSize = 0;
							Main->m_subInfor[i].m_RecvSize = 0;
							continue;
						}
					}//end 没有收全frame
				}else
				{//表明头还没有完全接收，需要进一步完善
					nDstLen = sizeof(MICP_HDR);

					if((nSrcLen+Main->m_subInfor[i].m_MicpSize)<nDstLen)
					{
						memcpy((char*)&Main->m_subInfor[i].m_MicpHdr+Main->m_subInfor[i].m_MicpSize,data+nOffset,nSrcLen);//offset原因
						Main->m_subInfor[i].m_MicpSize += nSrcLen;
						break;
					}
					if((nSrcLen+Main->m_subInfor[i].m_MicpSize)>=nDstLen)
					{
						memcpy((char*)&Main->m_subInfor[i].m_MicpHdr+Main->m_subInfor[i].m_MicpSize,data+nOffset,nDstLen-Main->m_subInfor[i].m_MicpSize);
						{
							//判断数据头的合法性
							//todo
							ret=parseFrameHead((MS_CHAR *const)&Main->m_subInfor[i].m_MicpHdr);

							if(ret!=0)
							{
								TRACE("echo_read_cb:[%d]parseFrameHead fail close socket,p[%p],ev_fd[%p],p1[%p],bev[%p]\n",
									i,Main->m_subInfor[i].p,Main->m_subInfor[i].p1,(void*)bev->ev_read.ev_fd,(void*)bev);
								m_Log.Add("echo_read_cb:[%d]parseFrameHead fail close socket",i);
								bufferevent_free(bev);
								Main->m_subInfor[i].p = NULL;
								Main->m_subInfor[i].p1 = NULL;
								break;
							}

							if(Main->m_subInfor[i].pRecvBuf != NULL)
								delete []Main->m_subInfor[i].pRecvBuf;

							Main->m_subInfor[i].pRecvBuf = new char[Main->m_subInfor[i].m_MicpHdr.body_len];

						}

						nSrcLen -= (nDstLen-Main->m_subInfor[i].m_MicpSize);
						nOffset += (nDstLen-Main->m_subInfor[i].m_MicpSize);
						Main->m_subInfor[i].m_MicpSize = sizeof(MICP_HDR);
						Main->m_subInfor[i].m_RecvSize = 0;

					}

				}//end 头还没有完全接收
			}//end while
		}//end else if
#else
		else if(Main->m_subInfor[i].bIsHead == 1)
		{

			if((len+0)>=sizeof(MICP_HDR))
			{
#if ADDLastSize
				//默认一帧数据量>>2K(udp传输每次的最大数据)
				memcpy(&Main->m_subInfor[i].m_MicpHdr+Main->m_subInfor[i].m_MicpSize,data,sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize);
				MICP_HDR *pmicp_hdr = (MICP_HDR *)&Main->m_subInfor[i].m_MicpHdr;
		//		TRACE("echo_read_cb:bis head 1.1 body size is %d:%d:%d:%d\n",pmicp_hdr->size,pmicp_hdr->seq,bev,bev->ev_read.ev_fd);
				if(pmicp_hdr->devid != i)
				{
					m_Log.Add("echo_read_cb:in frame head pmicp_hdr->devid[%d] != i[%d]",pmicp_hdr->devid,i);
					m_Log.Add("echo_read_cb:bis head 1.1 body size is %d:%d\n",pmicp_hdr->size,pmicp_hdr->seq);
					m_Log.Add("echo_read_cb:m_MicpSize[%d]",Main->m_subInfor[i].m_MicpSize);
					exit(0);
				}else
				{
				//	TRACE("echo_read_cb pmicp_hdr->devid[%d] == i[%d]",pmicp_hdr->devid,i);
				}

				if(Main->m_subInfor[pmicp_hdr->devid].pRecvBuf != NULL)
					delete []Main->m_subInfor[pmicp_hdr->devid].pRecvBuf;

				Main->m_subInfor[pmicp_hdr->devid].pRecvBuf = new char[pmicp_hdr->body_len];

				if(pmicp_hdr->body_len >len-(sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize))
				{
					memcpy(Main->m_subInfor[pmicp_hdr->devid].pRecvBuf,data+sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize,len-(sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize));
					Main->m_subInfor[pmicp_hdr->devid].m_RecvSize = len-(sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize);

					Main->m_subInfor[pmicp_hdr->devid].m_MicpSize=0;

					Main->m_subInfor[pmicp_hdr->devid].p =(void*) bev;
					Main->m_subInfor[pmicp_hdr->devid].bIsHead = 0;
				}else
				{
					memcpy(Main->m_subInfor[pmicp_hdr->devid].pRecvBuf,data+sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize,len-(sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize));
					Main->m_subInfor[pmicp_hdr->devid].m_RecvSize = len-(sizeof(MICP_HDR)-Main->m_subInfor[i].m_MicpSize);

					Main->m_subInfor[pmicp_hdr->devid].m_MicpSize=0;

					Main->m_subInfor[pmicp_hdr->devid].p =(void*) bev;
					Main->m_subInfor[pmicp_hdr->devid].bIsHead = 0;
				}


#else
				MICP_HDR *pmicp_hdr = (MICP_HDR *)data;
		//		TRACE("echo_read_cb:bis head 1.1 body size is %d:%d:%d:%d\n",pmicp_hdr->size,pmicp_hdr->seq,bev,bev->ev_read.ev_fd);
				//默认一帧数据量>>2K(udp传输每次的最大数据)
				memcpy(&Main->m_subInfor[pmicp_hdr->devid].m_MicpHdr,data,sizeof(MICP_HDR));
				if(Main->m_subInfor[pmicp_hdr->devid].pRecvBuf != NULL)
					delete []Main->m_subInfor[pmicp_hdr->devid].pRecvBuf;

				Main->m_subInfor[pmicp_hdr->devid].pRecvBuf = new char[pmicp_hdr->body_len];

				memcpy(Main->m_subInfor[pmicp_hdr->devid].pRecvBuf,data+sizeof(MICP_HDR),len-sizeof(MICP_HDR));
				Main->m_subInfor[pmicp_hdr->devid].m_RecvSize = len-sizeof(MICP_HDR);
				
				Main->m_subInfor[pmicp_hdr->devid].p =(void*) bev;
				Main->m_subInfor[pmicp_hdr->devid].bIsHead = 0;
#endif
				//nFrameSize = pmicp_hdr->size;

				//nRecvSize = len;
				//bIsHead = 0;
			}else//if((len+0)>=sizeof(MICP_HDR))
			{
				TRACE("echo_read_cb :Main->m_subInfor[i].bIsHead == 1:len[%d]<sizeof(MICP_HDR)\n",len);
				m_Log.Add("echo_read_cb :Main->m_subInfor[i].bIsHead == 1:len[%d]<sizeof(MICP_HDR)\n",len);

				bufferevent_free(bev);
				Main->m_subInfor[i].p = NULL;
			}
		}else//frame body
		{
			if((Main->m_subInfor[i].m_RecvSize + len)>Main->m_subInfor[i].m_MicpHdr.body_len)
			{//已经接收到的数据+现在收到的数据,超过完整帧的数据量
			//1)组装后完整帧
			//2)保存余下的数据
				//produce the complete frame todo
		//		TRACE("echo_read_cb:produce *****new frame\n");
				memcpy(Main->m_subInfor[i].pRecvBuf+Main->m_subInfor[i].m_RecvSize,data,
					Main->m_subInfor[i].m_MicpHdr.body_len-Main->m_subInfor[i].m_RecvSize);
				//m_RecvSize = len-sizeof(MICP_HDR);

				myVideo((BYTE *)Main->m_subInfor[i].pRecvBuf,
					Main->m_subInfor[i].m_MicpHdr.none1,
					Main->m_subInfor[i].m_MicpHdr.none2,i);
				
				//the last data have not ,剩余数据
				int nLastSize = Main->m_subInfor[i].m_RecvSize + len - Main->m_subInfor[i].m_MicpHdr.body_len;
				//剩余数据的偏移
				int nOffset =  Main->m_subInfor[i].m_MicpHdr.body_len - Main->m_subInfor[i].m_RecvSize;

				if(nLastSize < sizeof(MICP_HDR))
				{
				//	TRACE("********<= MICP_HDR*********\n");
					TRACE("echo_read_cb :nLastSize[%d] < sizeof(MICP_HDR)\n",nLastSize);
					m_Log.Add("echo_read_cb :nLastSize[%d] < sizeof(MICP_HDR)\n",nLastSize);
#if ADDLastSize
					memcpy(&Main->m_subInfor[i].m_MicpHdr,data+nOffset,nLastSize);
					Main->m_subInfor[i].m_MicpSize=nLastSize;
					Main->m_subInfor[i].bIsHead = 1;
					
					goto leaveOut;

#else
					bufferevent_free(bev);
					Main->m_subInfor[i].p = NULL;
					goto leaveOut;
#endif
				}
				//if()
				MICP_HDR *pmicp_hdr = (MICP_HDR *)(data+nOffset);

			//	TRACE("2 body size is %d:%d:%p:%d\n",pmicp_hdr->size,pmicp_hdr->seq,bev,bev->ev_read.ev_fd);

				memcpy(&Main->m_subInfor[pmicp_hdr->devid].m_MicpHdr,data+nOffset,sizeof(MICP_HDR));
				if(Main->m_subInfor[pmicp_hdr->devid].pRecvBuf != NULL)
					delete []Main->m_subInfor[pmicp_hdr->devid].pRecvBuf;

				Main->m_subInfor[pmicp_hdr->devid].pRecvBuf = new char[pmicp_hdr->body_len];
//TODO?
				memcpy(Main->m_subInfor[pmicp_hdr->devid].pRecvBuf,data+nOffset+sizeof(MICP_HDR),nLastSize-sizeof(MICP_HDR));
				Main->m_subInfor[pmicp_hdr->devid].m_RecvSize = nLastSize-sizeof(MICP_HDR);
				
				Main->m_subInfor[pmicp_hdr->devid].p =(void*) bev;

				Main->m_subInfor[pmicp_hdr->devid].bIsHead = 0;

				//nRecvSize = nLastSize;
				//nFrameSize = pmicp_hdr->size;
				//nRecvSize 
			}else if((Main->m_subInfor[i].m_RecvSize + len)<Main->m_subInfor[i].m_MicpHdr.body_len)
			{//已经接收到的数据+现在收到的数据,还达不到完整帧的数据量
				memcpy(Main->m_subInfor[i].pRecvBuf+Main->m_subInfor[i].m_RecvSize,data,len);
				Main->m_subInfor[i].m_RecvSize += len;
				//nRecvSize += len;
			}else
			{
				//complete frame
				//该数据包分析完正要是帧的结束
				memcpy(Main->m_subInfor[i].pRecvBuf+Main->m_subInfor[i].m_RecvSize,data,len);
			
				myVideo((BYTE *)Main->m_subInfor[i].pRecvBuf,
					Main->m_subInfor[i].m_MicpHdr.none1,
					Main->m_subInfor[i].m_MicpHdr.none2,i);

				Main->m_subInfor[i].m_RecvSize += len;

			//	TRACE("echo_read_cb:produce ####new frame %d\n",len);

				Main->m_subInfor[i].bIsHead =1;
				Main->m_subInfor[i].m_RecvSize = 0;

				//nFrameSize = nRecvSize=0;
			}
		}//end frame body		
#endif
leaveOut:
		free(data);	

		//将操作后情况返回控制端
		//evbuffer_add_printf(buf,"%08x",len);
		//m_Log.Add("PTZ控制返回信息：[%08x]",len);

		//发送到控制终端
        //evbuffer_add_buffer(output, buf);
		//释放资源
		evbuffer_free(buf);

		//event_base_loopexit(base, NULL);
}
 
static void
echo_event_cb(struct bufferevent *bev, short events, void *ctx)
{
	//int i =0;
	//for(i =0;i<16;i++)
	//{//查找当前连接是否之前连接过
	//	if(Main->m_subInfor[i].p == (void*)bev)
	//		break;
	//}

	//if(i != 16)
	//	Main->m_subInfor[i].p = NULL;


        if (events & BEV_EVENT_ERROR)
                perror("Error from bufferevent");
        if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
                bufferevent_free(bev);
        }
}
 
static void
accept_conn_cb(struct evconnlistener *listener,
    evutil_socket_t fd, struct sockaddr *address, int socklen,
    void *ctx)
{
        /* We got a new connection! Set up a bufferevent for it. */
        struct event_base *base = evconnlistener_get_base(listener);
        struct bufferevent *bev = bufferevent_socket_new(
                base, fd, BEV_OPT_CLOSE_ON_FREE);
 
        bufferevent_setcb(bev, echo_read_cb, NULL, echo_event_cb, NULL);
 
        bufferevent_enable(bev, EV_READ|EV_WRITE);
}
 
static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
        struct event_base *base = evconnlistener_get_base(listener);
        int err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d (%s) on the listener. "
                "Shutting down./n", err, evutil_socket_error_to_string(err));
 
        event_base_loopexit(base, NULL);
}

UINT __cdecl recvSub(void* pParams)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int	err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	// TODO: 在此添加控件通知处理程序代码
	struct event_base *base;
	struct evconnlistener *listener;
	struct sockaddr_in sin;

	int port = g_PtzPort;
	if(g_PtzPort== -1)
	{
		m_Log.Add("[INFO]:this config have no PTZ port,so not used this function\n");
		return 0;
	}


	base = event_base_new();
	if (!base) {
		m_Log.Add("Couldn't open event base");
		return 1;
	}

	/* Clear the sockaddr before using it, in case there are extra
	* platform-specific fields that can mess us up. */
	memset(&sin, 0, sizeof(sin));
	/* This is an INET address */
	sin.sin_family = AF_INET;
	/* Listen on 0.0.0.0 */
	sin.sin_addr.s_addr = htonl(0);
	/* Listen on the given port. */
	sin.sin_port = htons(10000);

	listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
		LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
		(struct sockaddr*)&sin, sizeof(sin));
	if (!listener) {
		m_Log.Add("Couldn't create listener:[%d]",port);
		return 1;
	}
	evconnlistener_set_error_cb(listener, accept_error_cb);
	event_base_dispatch(base);

	return 0;
}

UINT __cdecl changePTZ(void* pParams)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int	err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	// TODO: 在此添加控件通知处理程序代码
	struct event_base *base;
	struct evconnlistener *listener;
	struct sockaddr_in sin;

	int port = g_PtzPort;
	if(g_PtzPort== -1)
	{
		m_Log.Add("[INFO]:this config have no PTZ port,so not used this function\n");
		return 0;
	}


	base = event_base_new();
	if (!base) {
		m_Log.Add("Couldn't open event base");
		return 1;
	}

	/* Clear the sockaddr before using it, in case there are extra
	* platform-specific fields that can mess us up. */
	memset(&sin, 0, sizeof(sin));
	/* This is an INET address */
	sin.sin_family = AF_INET;
	/* Listen on 0.0.0.0 */
	sin.sin_addr.s_addr = htonl(0);
	/* Listen on the given port. */
	sin.sin_port = htons(g_FramePort);
//	sin.sin_addr.s_addr = INADDR_ANY;  

	listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
		LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE|LEV_OPT_THREADSAFE, -1,
		(struct sockaddr*)&sin, sizeof(sin));
	if (!listener) {
		m_Log.Add("Couldn't create listener:[%d]",port);
		return 1;
	}
	evconnlistener_set_error_cb(listener, accept_error_cb);
	event_base_dispatch(base);

	return 0;
}



UINT __cdecl changeCam(void* test)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int	err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	// TODO: 在此添加控件通知处理程序代码
	struct evhttp *httpd;

	if(-1 == g_ListenPort)
	{
		m_Log.Add("[INFO]:this config have no WEB port,so not used this function\n");
		return -1;
	}

	   
	event_init();      

	httpd = evhttp_start("0.0.0.0", g_ListenPort);


	evhttp_set_gencb(httpd,notify_handler, NULL);     
	evhttp_set_timeout(httpd,2);
	event_dispatch();        

	evhttp_free(httpd);        
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


	for(int i=0;i<16;i++)
	{
		m_h264[i] = NULL;
		memset(&m_subInfor[i],0,sizeof(SUB_INFOR));


	}

	for(int i=0;i<DEF_MAX_COMPANY;i++)
	{
		pCommonDlg[i] = NULL;
	}

	//pNtgjDlg[DEF_MAX_COMPANY]
}

CWMV_TO_H264Dlg::~CWMV_TO_H264Dlg(void)
{
	DWORD code; 

	for(int i=0;i<DEF_MAX_COMPANY;i++)
	{
		if(pCommonDlg[i] == NULL)
			delete pCommonDlg[i];
	}

	for(int i=0;i<16;i++)
	{
		if(NULL != m_h264[i])
		{
			delete m_h264[i];
		}
	}

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
END_MESSAGE_MAP()

#define _CRTDBG_MAP_ALLOC    
#include <stdlib.h>    
#include <crtdbg.h>
// CWMV_TO_H264Dlg 消息处理程序

//#include "h3cLib.h"
//#include "santachiLib.h"
extern void initH3cVMCOM();

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

	//具体放置位置有待确认，对ffmpeg有影响
	//initH3cVMCOM();


	CString logName;

	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	//thisDir.ReleaseBuffer();   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	g_CurrentDir = thisDir;
	logName = thisDir+"\\tc.log";
	thisDir+="\\config.ini";

	//loadSantachiLib(NULL);


	CFileFind finder;
	BOOL bResult = finder.FindFile(thisDir);

	if (bResult)
	{

		finder.Close();

	}
	else{
		CBasicDlg cbdlg;
		cbdlg.DoModal();

	}



/*
	char *ptest = new char[32];
	ptest = new char[64];
*/
	//avcodec_init();
	//av_register_all();
	

	
	HANDLE test=GetModuleHandle(NULL);
	int a=0;a++;
	CString haha="";
	haha.Format("程序装载地址：0x%08x",test);
		//MessageBox(haha);

	m_devType = DEF_MICROVIDEO_DEV;//DEF_SHSL_DEV;//

	m_Log.InitLog(logName);//("c:\\log.log");
	m_Log.Add(haha);
	Main=this;

	int ret = loadAVLib();
	if(ret != 0)
	{
		m_Log.Add("[ERR]:load av lib is error[%d][%d]\n",ret,GetLastError());
		exit(0);
	}

	//initH3cVMCOM();

	//ret = loadVNMPLib();
	//if(ret != 0)
	//{
	//	exit(0);
	//}
	//exit(0);




	HMODULE hProcessModule=NULL;
	hProcessModule	=  LoadLibraryA("multiprocess.dll");
	if(NULL == hProcessModule)
		m_Log.Add("[ERR]:load multiprocess.dll  is error[%d]\n",GetLastError());
		
	ProcessOpen=(STREAMPROCESSOPEN)GetProcAddress(hProcessModule,"streamProcessOpen");
	if(NULL == ProcessOpen)
		m_Log.Add("[ERR]:ProcessOpen==NULL\n");

	ProcessStart=(STREAMPROCESSSTART)GetProcAddress(hProcessModule,"streamProcessStart");
	if(NULL == ProcessStart)
		m_Log.Add("[ERR]:ProcessStart==NULL\n");

	ProcessClose=(STREAMPROCESSCLOSE)GetProcAddress(hProcessModule,"streamProcessClose");
	if(NULL == ProcessClose)
		m_Log.Add("[ERR]:ProcessClose==NULL\n");
	//1024
#if 1
	HMODULE hProcessModule6=NULL;
	hProcessModule6	=  LoadLibraryA("encoder.dll");
	if(NULL == hProcessModule6)
		m_Log.Add("[ERR]:hProcessModule6==NULL\n");
	encoderstreamProcessOpen=(ENCODERSTREAMPROCESSOPEN)GetProcAddress(hProcessModule6,"encoderstreamProcessOpen");
	if(NULL == encoderstreamProcessOpen)
		m_Log.Add("[ERR]:encoderstreamProcessOpen==NULL\n");
	encoderstreamProcessStart=(ENCODERSTREAMPROCESSSTART)GetProcAddress(hProcessModule6,"encoderstreamProcessStart");
	if(NULL == encoderstreamProcessStart)
		m_Log.Add("[ERR]:encoderstreamProcessStart==NULL\n");
	encoderstreamProcessClose=(ENCODERSTREAMPROCESSCLOSE)GetProcAddress(hProcessModule6,"encoderstreamProcessClose");
	if(NULL == encoderstreamProcessClose)
		m_Log.Add("[ERR]:encoderstreamProcessClose==NULL\n");
	encoderstreamProcessPutDataAddress=(ENCODERSTREAMPROCESSPUTDATA)GetProcAddress(hProcessModule6,"encoderstreamProcessPutDataAddress");	
	if(NULL == encoderstreamProcessPutDataAddress)
		m_Log.Add("[ERR]:encoderstreamProcessPutDataAddress==NULL\n");
#endif
	
	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	//thisDir.ReleaseBuffer();   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	thisDir+="\\config.ini";

	{
		g_bStop= GetPrivateProfileInt("system","BStop",-1,thisDir);
		if(1 == g_bStop)
		{
		//	m_Log.Add("读取config.ini文件中BStop信息出错，不使用全局转码配置");
			g_bStop = 1;
		}else
			g_bStop = 0;

		
		g_PubType= GetPrivateProfileInt("system","PubType",-1,thisDir);
		if(-1 == g_bUseTc)
		{
			m_Log.Add("读取config.ini文件中PubType信息出错，不使用全局转码配置");
			g_PubType = 0;
		}

		g_bUseTc = GetPrivateProfileInt("system","UseTc",-1,thisDir);
		if(1 != g_bUseTc)
		{
			m_Log.Add("读取config.ini文件中UseTc信息出错，不使用全局转码配置");
			g_bUseTc = 0;
		}

		g_BitRate = GetPrivateProfileInt("system","BitRate",-1,thisDir);
		if(-1 == g_BitRate)
		{
			m_Log.Add("读取config.ini文件中BitRate信息出错，使用默认值：500");
			g_BitRate  = 500;
		}

		g_PtzPort = GetPrivateProfileInt("system","PtzPort",-1,thisDir);
		if(-1 == g_PtzPort)
		{
			m_Log.Add("读取config.ini文件中PtzPort信息出错，去除该功能");
			//g_PtzPort  = 9876;
		}

		g_VideoWidth = GetPrivateProfileInt("system","VideoWidth",-1,thisDir);
		if(-1 == g_VideoWidth)
		{
			m_Log.Add("读取config.ini文件中VideoWidth信息出错，使用默认值：352");
			g_VideoWidth  = 352;
		}

		g_VideoHeight = GetPrivateProfileInt("system","VideoHeight",-1,thisDir);
		if(-1 == g_VideoHeight)
		{
			m_Log.Add("读取config.ini文件中ListenPort信息出错，使用默认值：288");
			g_VideoHeight  = 288;
		}

		g_ListenPort = GetPrivateProfileInt("system","ListenPort",-1,thisDir);
		if(-1 == g_ListenPort)
		{
			m_Log.Add("读取config.ini文件中ListenPort信息出错，使用默认值：9999");
			//g_ListenPort  = 9999;
		}

		g_rtmpPort = GetPrivateProfileInt("system","RtmpPort",-1,thisDir);
		if(-1 == g_rtmpPort)
		{
			m_Log.Add("读取config.ini文件中RtmpPort信息出错，使用默认值：1935");
			g_rtmpPort  = 1935;
		}

		g_maxLiveNum = GetPrivateProfileInt("system","MaxLiveNum",-1,thisDir);
		if(g_maxLiveNum>16 || g_maxLiveNum < 1)
		{
			m_Log.Add("读取config.ini文件中MaxLiveNum信息出错，使用默认值：16");
			g_maxLiveNum  = 16;
		}

		g_FramePort = GetPrivateProfileInt("system","FramePort",-1,thisDir);
		if(-1 == g_FramePort)
		{
			m_Log.Add("读取config.ini文件中ListenPort信息出错，使用默认值：10000");
			g_FramePort  = 10000;
		}

		char dst[64];
		ZeroMemory(dst,64);
		GetPrivateProfileString("system","FrameIp","",dst,64,thisDir);
		sprintf(g_FrameIp,dst);

	}
//zhh modify
	int mDefalutZero=0;
	SourceCount=0;
	while(true)//读取config.ini中所有shsl,对应new CH264Stream
	{
		CString tmpStr;
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("达到最大转码路数，以下配置停止分析。%d：%d",SourceCount,g_maxLiveNum);
			break;
		}


		tmpStr.Format("Company%d",SourceCount);
		char tmpCompany[255];
		ZeroMemory(tmpCompany,255);
		GetPrivateProfileString("common",tmpStr,"",tmpCompany,255,thisDir);
		if(strlen(tmpCompany)==0)
			break;

		if(1)//0 == strcmp(tmpCompany,"microvideo"))
		{
			if(pCommonDlg[MICROVIDEO_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;
				pCommonDlg[MICROVIDEO_INDEX]=(CNTGJDlg *)new CMicrovideoDlg();
				pCommonDlg[MICROVIDEO_INDEX]->Create(IDD_MICROVIDEO_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = MICROVIDEO_INDEX;

			pCommonDlg[MICROVIDEO_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}
#if 0

		if(0 == strcmp(tmpCompany,"np"))//公安
		{
			if(pCommonDlg[NETPOSA_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[NETPOSA_INDEX]=(CNTGJDlg *)new CNetPosaDlg();
				pCommonDlg[NETPOSA_INDEX]->Create(IDD_NETPOSA_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = NETPOSA_INDEX;

			pCommonDlg[NETPOSA_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"vnmp"))
		{
			if(pCommonDlg[VNMP_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[VNMP_INDEX]=(CNTGJDlg *)new CVNMPDlg();
				pCommonDlg[VNMP_INDEX]->Create(IDD_VNMP_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = VNMP_INDEX;

			pCommonDlg[VNMP_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"h3cvm"))
		{
			if(pCommonDlg[H3CVM_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[H3CVM_INDEX]=(CNTGJDlg *)new CH3cVMDlg();
				pCommonDlg[H3CVM_INDEX]->Create(IDD_H3CVM_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = H3CVM_INDEX;

			pCommonDlg[H3CVM_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"dhd"))
		{
			if(pCommonDlg[DHDEV_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[DHDEV_INDEX]=(CNTGJDlg *)new CDahuaDlg();
				pCommonDlg[DHDEV_INDEX]->Create(IDD_DAHUA_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = DHDEV_INDEX;

			pCommonDlg[DHDEV_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"hkd"))
		{
			if(pCommonDlg[HKDEV_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[HKDEV_INDEX]=(CNTGJDlg *)new CHKDlg();
				pCommonDlg[HKDEV_INDEX]->Create(IDD_HK_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = HKDEV_INDEX;

			pCommonDlg[HKDEV_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"std"))
		{
			if(pCommonDlg[SANTACHIDEV_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[SANTACHIDEV_INDEX]=(CNTGJDlg *)new CSantachiDlg();
				pCommonDlg[SANTACHIDEV_INDEX]->Create(IDD_SANTACHI_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = SANTACHIDEV_INDEX;

			pCommonDlg[SANTACHIDEV_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		//huading
		if(0 == strcmp(tmpCompany,"vms"))
		{
			if(pCommonDlg[VMSDEV_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[VMSDEV_INDEX]=(CNTGJDlg *)new CVmsDlg();
				pCommonDlg[VMSDEV_INDEX]->Create(IDD_VMS_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = VMSDEV_INDEX;

			pCommonDlg[VMSDEV_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"ipc"))
		{
			if(pCommonDlg[H3CIPCDEV_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[H3CIPCDEV_INDEX]=(CNTGJDlg *)new Ch3cIpcDlg();
				pCommonDlg[H3CIPCDEV_INDEX]->Create(IDD_H3CIPC_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = H3CIPCDEV_INDEX;

			pCommonDlg[H3CIPCDEV_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"rtsp"))
		{
			if(pCommonDlg[RTSP_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[RTSP_INDEX]=(CNTGJDlg *)new CRtspDlg();
				pCommonDlg[RTSP_INDEX]->Create(IDD_RTSP_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = RTSP_INDEX;

			pCommonDlg[RTSP_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}

		if(0 == strcmp(tmpCompany,"h3cimos"))
		{
			if(pCommonDlg[H3CIMOS_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;

				pCommonDlg[H3CIMOS_INDEX]=(CNTGJDlg *)new CH3CIMOSDlg();
				pCommonDlg[H3CIMOS_INDEX]->Create(IDD_H3CIMOS_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = H3CIMOS_INDEX;

			pCommonDlg[H3CIMOS_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}
		//add NTGJ
		if(0 == strcmp(tmpCompany,"shsl"))//苏柯达,NTGJ
		{
			if(pCommonDlg[SHSL_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;
				pCommonDlg[SHSL_INDEX]=(CNTGJDlg *)new CNTGJDlg();
				pCommonDlg[SHSL_INDEX]->Create(IDD_NTGJ_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = SHSL_INDEX;

			pCommonDlg[SHSL_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}
		//end add NTGJ
		//add NTQY
		if(0 == strcmp(tmpCompany,"ntqy"))//NTQY
		{
			if(pCommonDlg[NTQY_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;
				pCommonDlg[NTQY_INDEX]=(CNTGJDlg *)new CNtqyDlg();
				pCommonDlg[NTQY_INDEX]->Create(IDD_NTQY_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = NTQY_INDEX;

			pCommonDlg[NTQY_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}
		//end add NTQY
		//add NTHD
		if(0 == strcmp(tmpCompany,"nthd"))//NTHD
		{
			if(pCommonDlg[NTHD_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;
				pCommonDlg[NTHD_INDEX]=(CNTGJDlg *)new CNthdDlg();
				pCommonDlg[NTHD_INDEX]->Create(IDD_NTHD_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = NTHD_INDEX;

			pCommonDlg[NTHD_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}
		//end add NTHD
#if 1
		//add DFHS
		if(0 == strcmp(tmpCompany,"dfhs"))//DFHS
		{
			if(pCommonDlg[DFHS_INDEX] == NULL)
			{
				//m_devType = DEF_MICROVIDEO_DEV;
				pCommonDlg[DFHS_INDEX]=(CNTGJDlg *)new CDfhsDlg();
				pCommonDlg[DFHS_INDEX]->Create(IDD_DFHS_DIALOG,this);
			}

			if(0 == SourceCount)
				mDefalutZero = DFHS_INDEX;

			pCommonDlg[DFHS_INDEX]->GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,SourceCount);
		}
		//end add DFHS
#endif
#endif
		SourceCount++;

	}

	//for(int i=0;i<DEF_MAX_COMPANY;i++)
	{
		//if(pNtgjDlg[i] != NULL)
		{
			pCommonDlg[mDefalutZero]->SetDefaultParams(NULL);
			//break;
		}
	}

//如果不在配置中，如果需要不同协议间的切换则一定来自网络，帮该部分在网络接收处增加
//对于这种修改是否合理，过后再做进一步测试。至少可以少增加程序启动时加载的多余的库

#if 0 
	for(int i=0;i<DEF_MAX_COMPANY;i++)
	{
		if(pCommonDlg[i] != NULL)
		{
			continue;
		}

		if(i == MICROVIDEO_INDEX)
		{
			pCommonDlg[MICROVIDEO_INDEX]=(CNTGJDlg *)new CMicrovideoDlg();
			pCommonDlg[MICROVIDEO_INDEX]->Create(IDD_MICROVIDEO_DIALOG,this);
		}
		if(i == NETPOSA_INDEX)
		{
			pCommonDlg[NETPOSA_INDEX]=(CNTGJDlg *)new CNetPosaDlg();
			pCommonDlg[NETPOSA_INDEX]->Create(IDD_NETPOSA_DIALOG,this);
		}
		if(i == VNMP_INDEX)
		{
			pCommonDlg[VNMP_INDEX]=(CNTGJDlg *)new CVNMPDlg();
			pCommonDlg[VNMP_INDEX]->Create(IDD_VNMP_DIALOG,this);
		}
	}
#endif


	m_hChangeThread = AfxBeginThread( changeCam,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);
	m_hPTZThread = AfxBeginThread( changePTZ,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);
	//changePTZ
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


int CWMV_TO_H264Dlg::SaveConfigIni(void * pParams,int nIndex)
{
	CString tmpStr;
	char dst[255];
	ZeroMemory(dst,255);
#if 0
	if(m_devType == DEF_SHSL_DEV)
	{

		tmpStr.Format("Dstip%d",nIndex);
		WritePrivateProfileString("shsl",tmpStr,m_h264[nIndex]->m_Info.s_PubServer,thisDir);

		tmpStr.Format("Dstpub%d",nIndex);
		WritePrivateProfileString("shsl",tmpStr,"live",thisDir);

		tmpStr.Format("Dststream%d",nIndex);
		sprintf(dst,"%d",nIndex);
		WritePrivateProfileString("shsl",tmpStr,dst,thisDir);


		tmpStr.Format("ProxyIp%d",nIndex);
		WritePrivateProfileString("shsl",tmpStr,m_devShslParams.m_proxyIp,thisDir);

		tmpStr.Format("ProxyPort%d",nIndex);
		sprintf(dst,"%d",m_devShslParams.m_proxyPort);
		WritePrivateProfileString("shsl",tmpStr,dst,thisDir);


		tmpStr.Format("DeviceId%d",nIndex);
		WritePrivateProfileString("shsl",tmpStr,m_devShslParams.m_deviceId,thisDir);

		tmpStr.Format("DevicePort%d",nIndex);
		sprintf(dst,"%d",m_devShslParams.m_devicePort);
		WritePrivateProfileString("shsl",tmpStr,dst,thisDir);

		tmpStr.Format("DeviceName%d",nIndex);
		WritePrivateProfileString("shsl",tmpStr,m_devShslParams.m_deviceName,thisDir);
	}
	
	if(m_devType == DEF_MICROVIDEO_DEV)
	{
		tmpStr.Format("Source%d",nIndex);
		WritePrivateProfileString("mircovideo",tmpStr,m_h264[nIndex]->m_Info.s_DevID,thisDir);

		tmpStr.Format("Dstip%d",nIndex);
		WritePrivateProfileString("mircovideo",tmpStr,m_h264[nIndex]->m_Info.s_PubServer,thisDir);

		tmpStr.Format("Dstpub%d",nIndex);
		WritePrivateProfileString("mircovideo",tmpStr,"live",thisDir);

		tmpStr.Format("Dststream%d",nIndex);
		sprintf(dst,"%d",nIndex);
		WritePrivateProfileString("mircovideo",tmpStr,dst,thisDir);
	}
#endif
	return 0;
}
#define DEF_SHSL_TMP_MAX	6

CString CWMV_TO_H264Dlg::ChangeCh(CString CamID, int CH,CString pubserver)
{
#if 0
	if(DEF_SHSL_DEV == m_devType)
	{//临时设置最大转码路数
		//CH = CH%6;
		SourceCount = 6;
	}
#endif


	if (-1 == CH)
	{
		//return pNtgjDlg->CheckExit(CamID);
	}else
	{
		//return pNtgjDlg->ChangeChannel(CamID,CH,pubserver);

	}

	return "";
}

BEGIN_EVENTSINK_MAP(CWMV_TO_H264Dlg, CDialog)
END_EVENTSINK_MAP()


void CWMV_TO_H264Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	g_Stop = 1;
	m_Log.Add("ClickedCancel exit");
	exit(0);
	//Sleep(5000);

	//if(NULL != pNtgjDlg)
	//	delete pNtgjDlg;
	for(int i=0;i<16;i++)
	{
		if(m_h264[i] != NULL)
			delete m_h264[i];
	}

	for(int i=0;i<DEF_MAX_COMPANY;i++)
	{
		if(pCommonDlg[i] != NULL)
			delete pCommonDlg[i];
	}

	OnCancel();
}


void CWMV_TO_H264Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

int CWMV_TO_H264Dlg::CreateNewSub(PVOID ref_mts)
{
	AFX_MODULE_THREAD_STATE* mts=AfxGetModuleThreadState();
	AFX_MODULE_THREAD_STATE backup_mts;
	memcpy(&backup_mts,mts,sizeof(AFX_MODULE_THREAD_STATE));
	
	memcpy(mts,(AFX_MODULE_THREAD_STATE*)ref_mts,sizeof(AFX_MODULE_THREAD_STATE));
	//if(NULL == Main->pCommonDlg[MICROVIDEO_INDEX])
	{
		pCommonDlg[MICROVIDEO_INDEX]=(CNTGJDlg *)new CMicrovideoDlg();
		pCommonDlg[MICROVIDEO_INDEX]->Create(IDD_MICROVIDEO_DIALOG,this);
	}
	memcpy(mts,&backup_mts,sizeof(AFX_MODULE_THREAD_STATE));
	return 0;
}

int CWMV_TO_H264Dlg::ClosePullFromSub(void* pStream, CONVERT_DEV_TYPE tmpType)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;

	if(tmpType == DEF_MICROVIDEO_DEV)
		pCommonDlg[MICROVIDEO_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_NETPOSA_DEV)
		pCommonDlg[NETPOSA_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_VNMP_DEV)
		pCommonDlg[VNMP_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_DH_DEV)
		pCommonDlg[DHDEV_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_HK_DEV)
		pCommonDlg[HKDEV_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_SANTACHI_DEV)
		pCommonDlg[SANTACHIDEV_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_VMS_DEV)
		pCommonDlg[VMSDEV_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_H3CIPC_DEV)
		pCommonDlg[H3CIPCDEV_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_RTSP_DEV)
		pCommonDlg[RTSP_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_SHSL_DEV)
		pCommonDlg[SHSL_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_H3CIMOS_DEV)
		pCommonDlg[H3CIMOS_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_NTQY_DEV)
		pCommonDlg[NTQY_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_NTHD_DEV)
		pCommonDlg[NTHD_INDEX]->ClosePull(pStream,NULL);

	if(tmpType == DEF_DFHS_DEV)
		pCommonDlg[DFHS_INDEX]->ClosePull(pStream,NULL);

	return 0;
}
