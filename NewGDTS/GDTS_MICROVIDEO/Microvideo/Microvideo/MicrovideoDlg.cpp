// MicrovideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Microvideo.h"
#include "MicrovideoDlg.h"
#include "avLib.h"
#include "comm/include/datawork_app.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMicrovideoDlg* Main=NULL;
STREAMPROCESSOPEN ProcessOpen;
STREAMPROCESSSTART ProcessStart;
STREAMPROCESSCLOSE ProcessClose;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CMicrovideoDlg 对话框




CMicrovideoDlg::CMicrovideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMicrovideoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wowzalock=CreateMutex(NULL,FALSE,"jc_m_wowzalock");
	m_Lock=CreateMutex(NULL,FALSE,"jc_m_Lock");
		
	memset(&StreamParams, 0, sizeof(StreamParams));

}

CMicrovideoDlg::~CMicrovideoDlg(void)
{
	CloseHandle(m_wowzalock);//=CreateMutex(NULL,FALSE,"zhangjun_wowza");
	CloseHandle(m_Lock);//=CreateMutex(NULL,FALSE,"zhangjun_lock");

}
void CMicrovideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CMicrovideoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

void notify_handler(struct evhttp_request *req, void *arg) 
{    
	WaitForSingleObject(Main->m_wowzalock,INFINITE);
	if(EVHTTP_REQ_POST !=req->type)
	{
		ReleaseMutex(Main->m_wowzalock);
		return ;
	}

	struct evbuffer *buf;
	buf = evbuffer_new();
	const char *uri = evhttp_request_uri(req); 
	evbuffer *content = evhttp_request_get_input_buffer(req );//NULL;//req->input_buffer->buffer;//
	int len = evbuffer_get_length(content)+1;
	char *data = (char *)malloc(len);
    memset( data , 0 , len );

    int flag = evbuffer_remove( content , data , len );
	if (NULL ==content)
	{
		m_Log.Add("notify_handler:data null");
		evbuffer_add_printf(buf, "400"); 
		evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf); 
		evbuffer_free(buf);

		free(data);
		ReleaseMutex(Main->m_wowzalock);
		return;
	}
	m_Log.Add("[notify_handler]:data:%s",data);

	CString resultStr="";
	int rt;
	{
		XMLNODE *root = NULL;
		XMLNODE *subroot = NULL;
		XMLNODE *node = NULL,*chNode=NULL;
		XML xml;

		CString CH;

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

		}
		else
		{
			rt = 400;
			m_Log.Add("xml暂时不支持该company编号为[%d]!",atoi(node->data));
		}
leave:
		f_xmlnode_del(&xml);
	}
leaveOut:
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

	case 200://切换成功
		evbuffer_add_printf(buf, "%s",resultStr.GetBuffer(resultStr.GetLength())); 
		resultStr.ReleaseBuffer();
		evhttp_send_reply(req, HTTP_OK, "OK", buf); 
		evbuffer_free(buf);
		break;
	}

	free(data);
	ReleaseMutex(Main->m_wowzalock);
}


UINT __cdecl changeCam(void* test)
{
	int g_ListenPort=9999;

	WORD wVersionRequested;
	WSADATA wsaData;
	int	err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	struct evhttp *httpd;

	if(-1 == g_ListenPort)
	{
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


MU_INT32 GetProcessParam()
{

	MS_CHAR chParams[64];
	MS_CHAR* pTmp0=NULL;
	MS_CHAR* pTmp1=NULL;

	MS_CHAR  chIp[64];
	MU_INT8 nDevId=0;
	MU_INT32 nPort=10000;

	memset(chParams,0,sizeof(chParams));
	memset(chIp,0,sizeof(chIp));

	sprintf(chParams,theApp.m_lpCmdLine);
	pTmp0=strstr(chParams," ");
	if(pTmp0==NULL)
	{
		m_Log.Add("GetProcessParam:pTmp0==NULL,chParams[%s]\n",chParams);
		return -1;
	}

	pTmp1=strstr(pTmp0+1," ");
	if(pTmp1==NULL)
	{
		m_Log.Add("GetProcessParam:pTmp1==NULL,chParams[%s]\n",chParams);
		return -1;
	}

	nDevId=atoi(chParams);

	memcpy(chIp,pTmp0+1,pTmp1-pTmp0-1);
	nPort=atoi(pTmp1+1);
	

	//[0 192.168.0.58 10000]
	m_Log.Add("GetProcessParam:nDevId[%d],chIp[%s],nPort[%d],Paramer[%s]\n",nDevId,chIp,nPort,chParams);
	TRACE("GetProcessParam:nDevId[%d],chIp[%s],nPort[%d],Paramer[%s]\n",nDevId,chIp,nPort,chParams);

	sprintf(g_ip,"%s",chIp);
	g_port=nPort;
	g_devId=nDevId;

	m_Log.Add("GetProcessParam:g_devId[%d],g_ip[%s],g_port[%d]\n",g_devId,g_ip,g_port);


	return 0;
}

UINT __cdecl SendTsk(void* test)
{
//MU_INT32 sendDataToServer(MU_INT8 *pBuf1,MS_INT32 len1,MU_INT32 *pBuf2,MS_INT32 len2,MS_CHAR *pIp,MS_INT32 port);
//MS_INT32 FormatFrameHead(MS_CHAR* dst,MU_INT8 devid);
	WORD wVersionRequested;
	WSADATA wsaData;
	int	err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );

	MU_INT32 ret=MD_FAILED;

	//ret=GetProcessParam();
	//if(ret!=0)
	//{
	//	m_Log.Add("SendTsk:GetProcessParam fail\n");
	//	exit(0);
	//}
	ret=MD_FAILED;

	CString str;
	str.Format("%d", g_devId);
	Main->SetDlgItemText(IDC_URL, str);
	while(1)
	{
		Sleep(10);
		//TODO:create socket
		//TODO:send require
		if(ret== MD_FAILED)
		//if(0)
		{
			ret=sendRequireToServer(g_devId);
			if(ret != MD_OK)
			{
				ret=MD_FAILED;
				Sleep(1000);
				TRACE("SendTsk:sendRequireToServer fail exit\n");

				m_Log.Add("SendTsk:sendRequireToServer fail exit\n");
				exit(0);
				continue;
			}else
			{
				Main->SetParams(NULL,&Main->m_MicrovideoParams);
				Main->StartPull(NULL,&Main->m_MicrovideoParams);

			}
		}
		ret=sendDataToServer(g_devId); 
		if(ret== MD_FAILED)//socket err
		{
			Sleep(1000);
			m_Log.Add("SendTsk:sendDataToServer fail socket err exit\n");
			TRACE("SendTsk:sendDataToServer fail socket err exit\n");
			exit(0);
		}else if(ret== MD_NODATA){//no data
		//	TRACE("SendTsk:no data\n");
		}else if(ret == MD_OK){
		//	TRACE("SendTsk:suc\n");
		}
	}
	return 0;
}

//
#if 1
 GD_avcodec_encode_video GDavcodec_encode_video = NULL;
 GD_avcodec_open GDavcodec_open = NULL;
 GD_avcodec_find_encoder GDavcodec_find_encoder = NULL;
 GD_av_new_stream GDav_new_stream = NULL;
 GD_av_alloc_format_context GDav_alloc_format_context = NULL;
 GD_avpicture_fill GDavpicture_fill = NULL;
 GD_av_free GDav_free = NULL;
 GD_av_malloc GDav_malloc = NULL;
 GD_avpicture_get_size GDavpicture_get_size = NULL;
 GD_avcodec_alloc_frame GDavcodec_alloc_frame = NULL;
 GD_avcodec_init GDavcodec_init = NULL;
 GD_av_register_all GDav_register_all = NULL;

GD_avcodec_find_decoder GDavcodec_find_decoder = NULL;
GD_avcodec_decode_video GDavcodec_decode_video = NULL;
GD_avcodec_close GDavcodec_close = NULL;
int loadAVLib()
{
	//avcodec.dll
	HMODULE hProcessModule1=NULL;
	//SetDllDirectory(NULL);
	SetDllDirectory(g_CurrentDir+"\\avdll\\");
	CString path=g_CurrentDir+"\\avdll\\avcodec.dll";
	//m_Log.Add(g_CurrentDir);
	hProcessModule1	=LoadLibraryA("gdcodec.dll");//LoadLibraryEx(path,NULL, LOAD_WITH_ALTERED_SEARCH_PATH);// 
	//TRACE("ERR is %d\n",GetLastError());
	if(NULL == hProcessModule1)
		return -1;
	GDavcodec_encode_video=(GD_avcodec_encode_video)GetProcAddress(hProcessModule1,"avcodec_encode_video");
	if(NULL == GDavcodec_encode_video)
		return -2;

	GDavcodec_find_decoder=(GD_avcodec_find_decoder)GetProcAddress(hProcessModule1,"avcodec_find_decoder");
	if(NULL == GDavcodec_find_decoder)
		return -102;
	GDavcodec_decode_video=(GD_avcodec_decode_video)GetProcAddress(hProcessModule1,"avcodec_decode_video");
	if(NULL == GDavcodec_decode_video)
		return -202;
	GDavcodec_close=(GD_avcodec_close)GetProcAddress(hProcessModule1,"avcodec_close");
	if(NULL == GDavcodec_close)
		return -302;

	GDavcodec_open=(GD_avcodec_open)GetProcAddress(hProcessModule1,"avcodec_open");
	if(NULL == GDavcodec_open)
		return -3;
	GDavcodec_find_encoder=(GD_avcodec_find_encoder)GetProcAddress(hProcessModule1,"avcodec_find_encoder");
	if(NULL == GDavcodec_find_encoder)
		return -4;
	GDavpicture_fill=(GD_avpicture_fill)GetProcAddress(hProcessModule1,"avpicture_fill");
	if(NULL == GDavpicture_fill)
		return -5;
	GDavpicture_get_size=(GD_avpicture_get_size)GetProcAddress(hProcessModule1,"avpicture_get_size");
	if(NULL == GDavpicture_get_size)
		return -6;
	GDavcodec_alloc_frame=(GD_avcodec_alloc_frame)GetProcAddress(hProcessModule1,"avcodec_alloc_frame");
	if(NULL == GDavcodec_alloc_frame)
		return -7;
	GDavcodec_init=(GD_avcodec_init)GetProcAddress(hProcessModule1,"avcodec_init");
	if(NULL == GDavcodec_init)
		return -8;

	//FreeLibraryA(hProcessModule1);
//	TRACE("%s_______1.1_______n","cqdq");
//avformat.dll
	HMODULE hProcessModule2=NULL;
	//SetDllDirectory(g_CurrentDir);
	hProcessModule2	=  LoadLibraryA("gdformat.dll");
	GDav_new_stream=(GD_av_new_stream)GetProcAddress(hProcessModule2,"av_new_stream");
	if(NULL == GDav_new_stream)
		return -11;
	GDav_alloc_format_context=(GD_av_alloc_format_context)GetProcAddress(hProcessModule2,"av_alloc_format_context");
	if(NULL == GDav_alloc_format_context)
		return -12;
	GDav_register_all=(GD_av_register_all)GetProcAddress(hProcessModule2,"av_register_all");
	if(NULL == GDav_register_all)
		return -13;
	TRACE("%s_______1.2_______n","cqdq");
//avutil.dll
	HMODULE hProcessModule3=NULL;
	//SetDllDirectory(g_CurrentDir);
	hProcessModule3	=  LoadLibraryA("gdutil.dll");
	if(NULL == hProcessModule3)
		return -21;
	GDav_free=(GD_av_free)GetProcAddress(hProcessModule3,"av_free");//
	if(NULL == GDav_free)
		return -22;
	GDav_malloc=(GD_av_malloc)GetProcAddress(hProcessModule3,"av_malloc");//
	if(NULL == GDav_malloc)
		return -23;

	GDavcodec_init();
	GDav_register_all();

	SetDllDirectory(g_CurrentDir+"\\");
	return 0;
}
#endif

//

// CMicrovideoDlg 消息处理程序

BOOL CMicrovideoDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	int ret=0;
	ret=GetProcessParam();
	if(ret!=0)
	{
		m_Log.Add("OnInitDialog:GetProcessParam fail\n");
		exit(0);
	}
	
	Main=this;

    MyInitLog("microvideo");
	ret= loadAVLib();
	if(ret != 0){
		m_Log.Add("[ERR]:load av lib is error[%d][%d]\n",ret,GetLastError());
		exit(0);
	}

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

	CString thisDir="";
	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	thisDir+="\\config.ini";

//	GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,0);


	CWinThread* m_hChangeThread1;
	CWinThread* m_hChangeThread2;

	//m_hChangeThread1 = AfxBeginThread( changeCam,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);
	m_hChangeThread2 = AfxBeginThread( SendTsk,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if 1

BOOL myVideo(AVPictureOut *pOut,VOID *params)
{
	//add 
		//char tmp[1024*576]="";
		//memcpy(tmp,pOut->data[0],pOut->width*pOut->height);
		// static FILE*fp=NULL;
		//if(fp==NULL)
		//{
		//	fp=fopen("C://tmp.yuv","ab");
		//}
		//if(fp)
		//{	
		//	fwrite(tmp,pOut->width*pOut->height,1,fp);
		//}
	//
#if 1
	{
		static int itimes = 0;
		static DWORD myT= 0;
		if(itimes%100 == 0)
		{
			//time_t old_time=0;
			//time(&old_time);
			TRACE("myVideo:*******decode 100 frame %d\n",GetTickCount()- myT);
			myT = GetTickCount();
		}
		itimes++;
	}
	//return TRUE;
#endif
	
	CMicrovideoDlg*pMain=Main;
	//pMain->m_streamlock.Lock();
	if(pMain->p_YuvArray.GetSize()<60)
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
		memcpy(m_Frame->p_data,pOut->data[0],pOut->width*pOut->height);
		memcpy(&m_Frame->p_data[pOut->width*pOut->height],pOut->data[1],pOut->width*pOut->height/4);
		memcpy(&m_Frame->p_data[pOut->width*pOut->height+(pOut->width*pOut->height)/4],pOut->data[2],pOut->width*pOut->height/4);
		
		pMain->m_streamlock.Lock();
		pMain->p_YuvArray.Add(m_Frame);
		pMain->m_streamlock.Unlock();
	}else
	{
		//TRACE("数组过大，%d\n",pMain->p_YuvArray.GetSize());
	}
	
	//pMain->m_streamlock.Unlock();

	return TRUE;

}

int CMicrovideoDlg::SetParams(void *pStream,void * pParams)
{
//	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;
	int ret= 0;




	StreamParams.Audio_Delay_Packet =4;				//声音延迟播放多少个包，一般设为2－4；
	StreamParams.Audio_on_flag = 0;					//打开声音线程
	StreamParams.Video_on_flag = 1;					//打开视频线程
	StreamParams.Audio_is_Play = 0;					//是否播放声音
	StreamParams.balance =50; //500;//0;						//视频实时性和流畅性平衡值（50－600），600实时性强，50流畅性好

	StreamParams.hWnd =NULL;//GetSafeHwnd();						//窗口句柄
	StreamParams.TimeAdjustFlag =0;					//是否根据时间戳调整播放时间
	StreamParams.VideoCallBack =myVideo;			//视频解码回调函数：每解出一帧视频会回调此函数；不用设为NULL

	StreamParams.CallBackParams = NULL;//(VOID *)pH264Stream;//portNum;//视频回调函数的参数
	StreamParams.WarnningMsgFunc =NULL;//warn;		//告警信息回调函数；不用设为NULL

	StreamParams.VideoFormat =TYPE_YV12;//TYPE_YUY2;//TYPE_RGB24;//TYPE_NV12;//TYPE_RGB24;//TYPE_IMC4;//TYPE_UYVY;//			//视频解码后的图像格式
	StreamParams.VideoRend_flag = 0;				//是否显示视频图像

	StreamParams.receiveBuffSize = 512*1024;		//接收流的缓冲区大小
	if(NULL == StreamParams.Url_Name)
	{
		StreamParams.Url_Name=new char[255];
	}
	ZeroMemory(StreamParams.Url_Name,255);

	if(NULL != pParams)
		sprintf(StreamParams.Url_Name,"%s",pMicrovideoParams->SrcUrl);				//流的URL地址


	StreamParams.videoDisplayRect.top =0;			//设为0
	StreamParams.videoDisplayRect.left = 0;			//设为0
	StreamParams.videoDisplayRect.right = 0;		//设为0
	StreamParams.videoDisplayRect.bottom = 0;		//设为0

	return ret;
}
int CMicrovideoDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255];
	CString tmpStr;

	tmpStr.Format("Source%d",nIndex);
	char Server[255];
	ZeroMemory(Server,255);
	GetPrivateProfileString("common",tmpStr,"",Server,255,thisDir);
	m_MicrovideoParams.SrcUrl=Server;

	tmpStr.Format("Dstip%d",nIndex);
	char dst[255];
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_MicrovideoParams.ServerIP=dst;

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_MicrovideoParams.PubName=dst;

	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_MicrovideoParams.StreamNumber=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

	SetParams(NULL,&m_MicrovideoParams);

	StartPull(NULL,&m_MicrovideoParams);
	


	m_Log.Add("初始化通道%d的配置: 参数为:Source%d:%s,Dstip%d:ServerIP:%s,Dstpub%d:PubName:%s, Dststream%d:StreamNumber:v%d",
		nIndex,nIndex,
		m_MicrovideoParams.SrcUrl,nIndex,
		m_MicrovideoParams.ServerIP,nIndex,
		m_MicrovideoParams.PubName,nIndex,
		m_MicrovideoParams.StreamNumber);


	return 0;

}

// 启动拉流函数
int CMicrovideoDlg::StartPull(void *pStream,void* pParams)
{
//	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;

	BOOL status=FALSE;
	if (NULL!=pStream)
	{
		try
		{
			ProcessClose(pStream);
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
		
		pStream=NULL;
	}
	try
	{
		pStream = ProcessOpen(&StreamParams,&status);
		ProcessStart(pStream);
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
//	CH264Stream *pH264Stream = (CH264Stream *)pStream;
//	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;


	if (NULL!=pStream)
	{
		try
		{
			ProcessClose(pStream);
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

		pStream=NULL;
	}

	return 0;
}

#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

int CMicrovideoDlg::ParseRequest(char * pXmlBuf,CString *str)
{
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
	node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
	if(node == NULL)
	{
		m_Log.Add("xml解析错误，未能找到serverip节点!\n");
		rt = 400;
		goto leave;
	}
	else
	{
		m_MicrovideoParams.ServerIP = node->data;
		//m_Log.Add("解析到的serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=1)
	{
		m_Log.Add("xml解析错误，未能找到company节点,或者company节点内容不是1!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "microvideo");
	if(subroot==NULL)
	{
		m_Log.Add("xml解析错误，未能找到microvideo节点!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("开始解析MICROVIDOE的XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			//tmpType = node->data;
			//m_Log.Add("解析到的type：%s",tmpType);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到type节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ver", MAX_LEN);
		if (node != NULL)
		{
			//tmpVer=node->data;
			//m_Log.Add("解析到的ver：%s",tmpVer);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到ver节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			m_MicrovideoParams.ServerIP=node->data;
			//m_Log.Add("解析到的ip：%s",tmpIp);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到ip节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			m_MicrovideoParams.nPort=atoi(node->data);
			//m_Log.Add("解析到的port：%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到port节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "url", MAX_LEN);
		if (node != NULL)
		{
			m_MicrovideoParams.SrcUrl = node->data;
			//m_Log.Add("解析到的url：%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到url节点!");
			rt = 400;
			goto leave;
		}
	}

	chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
	if (chNode != NULL)
	{

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
			//m_h264[atoi(CH)]->m_Info.s_DevID = m_MicrovideoParams.SrcUrl;
			//m_h264[atoi(CH)]->m_Info.s_PubServer = m_MicrovideoParams.ServerIP;
			m_Log.Add("解析到的ch: %s",CH);
			m_Log.Add("切换%s,至通道%d",m_MicrovideoParams.SrcUrl,atoi(CH));
			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(m_MicrovideoParams.SrcUrl,atoi(CH),m_MicrovideoParams.ServerIP);
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
		m_Log.Add("xml解析错误，ch节点的值为空!");
		rt = 400;
		goto leave;
	}
	//m_Log.Add("解析到通道%d: 参数为:Serverip:%s Company:%s DevType:%s Version:%s IP:%s Port:%s Url:%s\n",atoi(CH), pubserverip,"microvideo",tmpType, tmpVer, tmpIp, tmpPort, tmpSource);//张世俊增加
	//TRACE("DevType:%s Version:%s IP:%s Port:%s Url:%s", tmpType, tmpVer, tmpIp, tmpPort, tmpSource);



leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}


CString CMicrovideoDlg::ChangeChannel(CString sId,int CH,CString str)
{
	CString UrlStr="";


	m_MicrovideoParams.StreamNumber = CH;

	if(0>SetParams(NULL,&m_MicrovideoParams))
		return "";

	m_Log.Add("南京感动视频准备切换%u:%s",GetTickCount(),m_MicrovideoParams.SrcUrl);

	StartPull(NULL,&m_MicrovideoParams);


//	UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
//	m_Log.Add("南京感动视频切换成功%u:%s",GetTickCount(),UrlStr);
	//SaveConfigIni(NULL,CH);//保存正确的配置到ini文件中
	//[CH]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。且持续时间从现在开始算起。
	return UrlStr;

}

#endif

void CMicrovideoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMicrovideoDlg::OnPaint()
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
HCURSOR CMicrovideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int getElement(void **e)
{
	Main->m_streamlock.Lock();
	if (Main->p_YuvArray.GetSize()>0)
	{
		*e=Main->p_YuvArray.ElementAt(0);
		Main->p_YuvArray.RemoveAt(0);
	}else
		*e = NULL;

	Main->m_streamlock.Unlock();

	
	return 0;
}


#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

MS_INT32 parseGDContent(char*pXmlBuf)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	CString CH;

	rt = f_xmlparse_init(&xml, (const char*)pXmlBuf);
	if (rt != 0)
	{
		m_Log.Add("xml解析错误，xml结构体初始化失败！");
		TRACE("parseGDContent:pXmlBuf[%s]\n",pXmlBuf);
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
	node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
	if(node == NULL)
	{
		m_Log.Add("xml解析错误，未能找到serverip节点!\n");
		rt = 400;
		goto leave;
	}
	else
	{
		Main->m_MicrovideoParams.ServerIP = node->data;
		//m_Log.Add("解析到的serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL)// || atoi(node->data)!=1)
	{
		m_Log.Add("xml解析错误，未能找到company节点,或者company节点内容不是1!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, node->data);
	if(subroot==NULL)
	{
		m_Log.Add("xml解析错误，未能找到%s节点!",node->data);
		rt = 400;
		goto leave;
	}
	else
	{ 
	//	m_Log.Add("开始解析MICROVIDOE的XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			//tmpType = node->data;
			//m_Log.Add("解析到的type：%s",tmpType);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到type节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ver", MAX_LEN);
		if (node != NULL)
		{
			//tmpVer=node->data;
			//m_Log.Add("解析到的ver：%s",tmpVer);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到ver节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
		//	Main->m_MicrovideoParams.ServerIP=node->data;
			//m_Log.Add("解析到的ip：%s",tmpIp);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到ip节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			Main->m_MicrovideoParams.nPort=atoi(node->data);
			//m_Log.Add("解析到的port：%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到port节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "url", MAX_LEN);
		if (node != NULL)
		{
			Main->m_MicrovideoParams.SrcUrl = node->data;
			//m_Log.Add("解析到的url：%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到url节点!");
			rt = 400;
			goto leave;
		}
	}

	chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
	if (chNode != NULL)
	{
		
		CH = chNode->data;
		if(atoi(CH)>=g_maxLiveNum)
		{
			int ntmp = atoi(CH);
			ntmp = ntmp%g_maxLiveNum;
			CH.Format("%d",ntmp);
		}
	//	Main->m_MicrovideoParams.StreamNumber=atoi(CH);
		rt=MD_OK;
		m_Log.Add("解析到的ch: %s",CH);
	}
	else
	{
		m_Log.Add("xml解析错误，ch节点的值为空!");
		rt = 400;
		goto leave;
	}

leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;

}
