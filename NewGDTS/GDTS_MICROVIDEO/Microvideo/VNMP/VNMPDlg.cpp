// VNMPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VNMP.h"
#include "VNMPDlg.h"
//#include "comm/include/VNMP_datawork_app.h"
#include "comm/include/datawork_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CVNMPDlg* Main=NULL;



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CVNMPDlg �Ի���




CVNMPDlg::CVNMPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVNMPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CVNMPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVNMPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


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

	ret=MD_FAILED;

	CString str;
	str.Format("%d", g_devId);
	Main->SetDlgItemText(IDC_VNMP_URL, str);
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
				Main->SetParams(NULL,&Main->m_devParams);
				Main->StartPull(NULL,&Main->m_devParams);

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

// CVNMPDlg ��Ϣ�������

BOOL CVNMPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	int ret=0;
	ret=GetProcessParam();
	if(ret!=0)
	{
		m_Log.Add("OnInitDialog:GetProcessParam fail\n");
		exit(0);
	}

    MyInitLog("VNMP");

   	CString thisDir="";
	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	//thisDir+="\\config.ini";
	g_CurrentDir = thisDir;


	
	Main=this;

 

	
	if(-1 == loadVNMPLib())
	{
		m_Log.Add("[ERR]:loadVNMPLib error");
		exit(0);
	}

		
	if(0 > video_device_init())
	{
		m_Log.Add("[ERR]:CVNMPDlg::CVNMPDlg->video_device_init");
		//return -1;
	}



//	GetInforFromIni4Company((LPSTR)(LPCTSTR)thisDir,0);


	CWinThread* m_hChangeThread2;

	m_hChangeThread2 = AfxBeginThread( SendTsk,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVNMPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVNMPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVNMPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




#if 0


<command>
<serverip>192.168.0.251</serverip>
<company>11</company>//������Զ

<vnmp>
<ip>192.168.0.1</ip>//��ַ
<port>2100</port>//�˿�
<user>admin</user>//�û���
<passwd>admin</passwd>//����
<channel>0001</channel>//����ͨ���ı�ʶ

</vnmp>
<ch>0</ch>
</command>

#endif


/*YUV���ݻص���������ȡ������YUV���ݣ�������YUV���ݴ��뵽YUV�����������ݵ���һ���빦��
 *@date 2014-3-26
 *@author zhh
 *@param long nPort	�û�����
 *@param char * pBuf  YUV���ָ��
 *@param long nSize   YUV���ݴ�С
 *@param VNMP_FRAME_INFO * pFrameInfo  VNMP_FRAME_INFO�ṹָ�룬�洢��֡ͼ��ĳ���ʱ�����Ϣ
 *@param long nReserved1 ��������
 *@param long nReserved2 ��������
 *@return ��
 *
*/
void CALLBACK myDecCBFun(long nPort,
						 char * pBuf,
						 long nSize,
						 VNMP_FRAME_INFO * pFrameInfo,
						 long nReserved1,
						 long nReserved2)
{
	//YUV���ݻص������ص�Ϊyuv12����
	CVNMPDlg*pMain=Main;


	int mWidth = pFrameInfo->nWidth;
	int mHeight = pFrameInfo->nHeight;


	if(pMain->p_YuvArray.GetSize()<60)
	{
		if (NULL == pBuf)
		{

			return ;
		}
		if (0 == nSize)
		{

			return ;
		}
		MyAvFrame *m_Frame=new MyAvFrame(mWidth,mHeight);
		if (NULL == m_Frame)
		{
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
			return ;
		}
		if (NULL == m_Frame->p_data)
		{
			return ;
		}

		memcpy(m_Frame->p_data,pBuf,nSize);

		pMain->m_streamlock.Lock();
		pMain->p_YuvArray.Add(m_Frame);
		pMain->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pMain->p_YuvArray.GetSize());
	}	
}



/*��Ƶ�ص�����,�Խ��յ����ݽ����зְ������飬����Ŀǰ����ȷ�ϵõ���ȫΪH264���ݣ�
 *�̸ô�ȫ������YUV���ݽ��б��롣��������Ƶ�ص��м�����YUV�ص�������ע�ᡣ
 *@date 2014-3-26
 *@author zhh
 *@param DWORD dwDataType	��λ�����ݱ������͸�λ������Ǳ�ʾͷ������0Ϊ��ͨ����
 *@param unsigned long bufsize  �ôλص������ݴ�С
 *@param unsigned char *buffer	�ôλص�����ָ��
 *@param unsigned long user		�û�����
 *@return ��
 *
*/
void CALLBACK  CBK_HandVideoData(DWORD dwDataType, 
								 unsigned long bufsize, 	
								 unsigned char *buffer, 	
								 unsigned long user)
{
	//��Ƶ���ݻص�����
	//dwDataType��λ�����ݱ������͸�λ������Ǳ�ʾͷ������0Ϊ��ͨ����

	int iwnd = user;
//	CH264Stream *pH264Stream = m_h264[user];//(CH264Stream *)pStream;
	DWORD IsSysHead;  
	IsSysHead = dwDataType >> 16;
	if ( 1 == IsSysHead )
	{
		int ret = 0;
		dwDataType = dwDataType & 0xffff;
		ret = video_play_SetStreamOpenMode(iwnd, 0);
		ret = video_play_SetDisplayBuf(iwnd,30);
		ret = video_play_SetBufferType(iwnd,dwDataType);
		//YUV���ݻ�ȡ�Ļص�����ע��
		video_play_SetYUVCallBack(iwnd,myDecCBFun);
		ret = video_play_Play(iwnd, buffer,bufsize,NULL);
	}else{
		video_play_InputVideoData(iwnd,bufsize,buffer);
	}
}


/*���õ��ó�����ԶSDK����Ҫ�Ĳ�����Ϣ������������Ҫ�ı���������߳�
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CVNMPDlg::SetParams(void *pStream,void * pParams)
{
	DEV_VNMP_PARAMS *pVNMPParams =(DEV_VNMP_PARAMS *) pParams;
	int ret = 0;

	//pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%s",pVNMPParams->m_proxyIp,pVNMPParams->m_proxyPort,
	//	pVNMPParams->m_userName,pVNMPParams->m_passWord,pVNMPParams->m_cameraId);
//	pH264Stream->m_Info.s_DevID.Format("%s",pVNMPParams->m_cameraId);

//	pH264Stream->m_devType = DEF_VNMP_DEV;
	

	return ret;
}

/*���ó�����ԶSDK������������������лص�������ע��
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CVNMPDlg::StartPull(void *pStream,void* pParams)
{
//	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VNMP_PARAMS *pVNMPParams = (DEV_VNMP_PARAMS *)pParams;
	if(NULL == pVNMPParams)
	{
//		pVNMPParams = &m_devParams[pH264Stream->thisIndex];
	}

	VIDEODEVICE videoDev;
	memset(&videoDev,0,sizeof(videoDev));

#if 0
	strcpy(videoDev.ip,pVNMPParams->m_proxyIp);
	videoDev.port =  pVNMPParams->m_proxyPort;
	strcpy(videoDev.username,pVNMPParams->m_userName);
	strcpy(videoDev.password,pVNMPParams->m_passWord);

	//��¼ʱ����videodev.reserve��������SDK��ʶ
	sprintf(videoDev.reserve,"%d%d",0,512);

	if((pVNMPParams->m_devInfo.m_loginHandle = video_device_login(&videoDev)) < 0){
		m_Log.Add("[ERR]:[%d]CVNMPDlg::StartPull->video_device_login",pH264Stream->thisIndex);
		video_device_destroy();
		return -1;
	}

	//videodev.handle��videodev.channel������д
	videoDev.handle = pVNMPParams->m_devInfo.m_loginHandle;
#endif

	strcpy(videoDev.channel,pVNMPParams->m_cameraId);

#if 0
	//videodev.reuse,videodev.reserve �̶���д����
	videoDev.reuse = 1;
	videoDev.reuse = videoDev.reuse | 0x200;
// 	//������������
     strcpy(videoDev.reserve,".");
#endif
	//NET�����������
//	strcpy(videodev.reserve,"./NET.ini");
	//video_device_capturing_start�ĵڶ����������û�����
	//SDK�ڲ����޸������ݻص���ԭ����Ϊ�����������ڱ�ʶ����
	 TRACE("*********the url is %s\n",pVNMPParams->m_cameraId);
	pVNMPParams->m_devInfo.m_videoHandle = video_device_capturing_start(&videoDev,g_devId,CBK_HandVideoData);//TODO 0
	if(0 >pVNMPParams->m_devInfo.m_videoHandle)
	{
		m_Log.Add("[ERR]:[%d]CVNMPDlg::StartPull->video_device_capturing_start:[%ld]",g_devId,pVNMPParams->m_devInfo.m_videoHandle);
		//video_device_logout(&videoDev);
		//video_device_destroy();

		return -1;
	}

	pVNMPParams->m_devInfo.m_videoHandle = videoDev.devnum;
	//�����޸���videodev������
	//����ֵ>=0 �ɹ�if m_videohandle = videodev.devnum then success
	//����ֵ>=0 �ɹ�if m_videohandle != videodev.devnum then �ظ����������
	//����ֵ>=0 �ɹ�videodev.channel����ʵ�򿪵������ID ���ܺʹ���Ĳ�һ��
	//����ֵ>=0 �ɹ�videodev.reserve����ʵ�򿪵������������

//	pH264Stream->h_stream = &videoDev;
		
	return 0;
}

/*���ó�����ԶSDK��ɹرղ�������������Դ���ͷŲ���
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
#if 0
int CVNMPDlg::ClosePull(void *pStream,void * pParams)
{
//	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VNMP_PARAMS *pVNMPParams =(DEV_VNMP_PARAMS *)pParams;
	int i = 0;
	
	VIDEODEVICE videodev;
	memset(&videodev,0,sizeof(VIDEODEVICE));

	//ֹͣ���봫��videodev.handle��videodev.devnum
	videodev.handle = pVNMPParams->m_devInfo.m_loginHandle;
	videodev.devnum = pVNMPParams->m_devInfo.m_videoHandle;
	video_device_capturing_stop(&videodev);
	video_play_Stop(g_devId);//TODO 0
#if 0
	for(i = 0; i< g_maxLiveNum;i++)
	{
		if((m_h264[i]->m_devType == DEF_VNMP_DEV)&&(i != pH264Stream->thisIndex)&&(m_h264[i]->b_start == true))
			break;
	}
	if(i == g_maxLiveNum)
		video_device_logout(&videodev);
	//video_device_destroy();
#endif

//	pH264Stream->h_stream = NULL;

	return 0;
}
#endif


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


//#include "./xml/include/xmlnode.h"
//#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

MS_INT32 parseGDContent(MS_CHAR*pXmlBuf)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_VNMP_PARAMS tmpVNMP;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	CString CH;

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
		m_Log.Add("xml��������δ���ҵ�command�ڵ�!");
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
		//m_MicrovideoParams.ServerIP = node->data;
		strcpy(tmpVNMP.m_devInfo.m_dstIp,node->data); 
		//m_Log.Add("��������serverip: %s",pubserverip);
	}
#if 0
	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=3)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���hk == 4!\n");
		rt = 400;
		goto leave;
	}
#endif
	subroot = f_xmlnode_sch_elem(root, "vnmp");
//	subroot = f_xmlnode_sch_elem(root, "microvideo");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�vnmp�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����VNMP��XML!");

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpVNMP.ServerIP=node->data;
			strcpy(tmpVNMP.m_proxyIp,node->data);
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
			//tmpVNMP.nPort=atoi(node->data);
			tmpVNMP.m_proxyPort = atoi(node->data);
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
			//tmpVNMP.SrcUrl = node->data;
			strcpy(tmpVNMP.m_userName,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
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
			//tmpVNMP.SrcUrl = node->data;
			strcpy(tmpVNMP.m_passWord,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
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
			//tmpVNMP.nPort=atoi(node->data);
			//tmpVNMP.m_playChn = atoi(node->data);
			strcpy(tmpVNMP.m_cameraId,node->data);
			
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�channel�ڵ�!");
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
		m_Log.Add("��������ch: %s",CH);
	}
	else
	{
		m_Log.Add("xml��������ch�ڵ��ֵΪ��!");
		rt = 400;
		goto leave;
	}

	
	strcpy(Main->m_devParams.m_proxyIp,tmpVNMP.m_proxyIp);
	Main->m_devParams.m_proxyPort = tmpVNMP.m_proxyPort;
	strcpy(Main->m_devParams.m_userName,tmpVNMP.m_userName);
	strcpy(Main->m_devParams.m_passWord,tmpVNMP.m_passWord);
	strcpy(Main->m_devParams.m_cameraId,tmpVNMP.m_cameraId);

	

leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;

}