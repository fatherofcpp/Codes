// DaHuaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DaHua.h"
#include "DaHuaDlg.h"
#include "dhLib.h"
#include "comm/include/datawork_app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDaHuaDlg* Main=NULL;

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


// CDaHuaDlg �Ի���




CDaHuaDlg::CDaHuaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDaHuaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDaHuaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDaHuaDlg, CDialog)
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
	Main->SetDlgItemText(IDC_DaHua_Url, str);
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

void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	if(0 != dwUser)
	{
		m_Log.Add("[INFO]:DisConnectFunc information,return code is %d",CLIENT_GetLastError());
	}
}

// CDaHuaDlg ��Ϣ�������

BOOL CDaHuaDlg::OnInitDialog()
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
	Main=this;

	int ret=0;
	ret=GetProcessParam();
	if(ret!=0)
	{
		m_Log.Add("OnInitDialog:GetProcessParam fail\n");
		exit(0);
	}

    MyInitLog("dahua");

   	CString thisDir="";
	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	//thisDir+="\\config.ini";
	g_CurrentDir = thisDir;



	if(-1 == loadDHLib(NULL))
	{
		m_Log.Add("[ERR]:loadDHLib error");
		exit(0);
	}

	BOOL bSuccess = CLIENT_Init(DisConnectFunc, (DWORD)this);
	if (!bSuccess)
	{
		//Display function error occurrs reason.
		m_Log.Add("[ERR]:CLIENT_Init failed,return code is %d",CLIENT_GetLastError());
	}


	


	CWinThread* m_hChangeThread2;

	m_hChangeThread2 = AfxBeginThread( SendTsk,this,THREAD_PRIORITY_ABOVE_NORMAL);// 0,this);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDaHuaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDaHuaDlg::OnPaint()
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
HCURSOR CDaHuaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CALLBACK pDecCBFun(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	CDaHuaDlg *pMain = (CDaHuaDlg*)Main;

//	TRACE("port is %d size is %d w:H[%d,%d]\n",nPort,nSize,pFrameInfo->nWidth,pFrameInfo->nHeight);
	if(pFrameInfo->nType != T_IYUV)
		return;

	if(pMain->p_YuvArray.GetSize()<60)
	{
		if (NULL ==pBuf)
		{
			//pMain->m_streamlock.Unlock();
			return ;
		}
		if (nSize != (pFrameInfo->nWidth*pFrameInfo->nHeight*3/2))
		{
			//pMain->m_streamlock.Unlock();
			return ;
		}
		MyAvFrame *m_Frame=new MyAvFrame(pFrameInfo->nWidth,pFrameInfo->nHeight);
		if (NULL == m_Frame)
		{
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
			//pMain->m_streamlock.Unlock();
			return ;
		}
		if (NULL == m_Frame->p_data)
		{
			//pMain->m_streamlock.Unlock();
			return ;
		}
		//memcpy(m_Frame->p_data,pBuf,nSize);
		memcpy(m_Frame->p_data,pBuf,pFrameInfo->nWidth*pFrameInfo->nHeight);
		memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight*5/4,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
		memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight*5/4],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
		
		
		pMain->m_streamlock.Lock();
		pMain->p_YuvArray.Add(m_Frame);
		pMain->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pMain->p_YuvArray.GetSize());
	}
}

void CALLBACK RealDataCallBackEx(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize, LONG lParam, DWORD dwUser)
{
	//if(dwUser == 0)
	//{
	//	return;
	//}	

//	CH264Stream *pMain = (CH264Stream*)dwUser;

//	pMain->b_PullLive=0;

	//TRACE("[INFO]:%ld:%d%----",lRealHandle,dwDataType,dwUser);


	BOOL bInput=FALSE;

	switch(dwDataType) {
		case 0:
			//Original data 
			bInput = PLAY_InputData(g_devId+1,pBuffer,dwBufSize);
			if (!bInput)
			{
				//TRACE("input data error: %d\n", PLAY_GetLastError(pH264->thisIndex+1));
			}/**/
			break;
		case 1:
			//Standard video data 

			break;
		case 2:
			//yuv data 

			break;
		case 3:
			//pcm audio data 

			break;
		case 4:
			//Original audio data 

			break;
		default:
			break;
	}	

}

/*���õ��ô��豸SDK����Ҫ�Ĳ�����Ϣ������������Ҫ�ı���������߳�
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CDaHuaDlg::SetParams(void *pStream,void * pParams)
{
//	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_DH_PARAMS *pDhParams = (DEV_DH_PARAMS*)pParams;
	int ret= 0;

	//if(NULL != pDhParams)
	//{
	//pH264Stream->m_Info.s_DevID=pDhParams->m_devName;
	//pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%s",pDhParams->m_proxyIp,pDhParams->m_proxyPort,
	//	pDhParams->m_userName,pDhParams->m_passWord,pDhParams->m_devName);

	//}
	
//	pH264Stream->m_devType = DEF_DH_DEV;




	return ret;
}

/*���ô��豸SDK������������������лص�������ע��
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CDaHuaDlg::StartPull(void *pStream,void* pParams)
{
	DEV_DH_PARAMS *pDhParams = (DEV_DH_PARAMS*)pParams;
	int ret;

	pDhParams->m_devInfo.m_loginid = NULL;

	NET_DEVICEINFO deviceInfo = {0};
		//Call log in interface 
	int err = 0;	//Storage the possible error return value.
	long lRet = CLIENT_Login(pDhParams->m_proxyIp,pDhParams->m_proxyPort,
		pDhParams->m_userName, pDhParams->m_passWord,&deviceInfo,&err);
	//NPSDKError ret = NP_NET_Login(&pDhParams->m_devInfo.m_loginid, pDhParams->m_proxyIp, pDhParams->m_proxyPort, 
	//	pDhParams->m_userName, pDhParams->m_passWord);

	if(lRet == 0)
	{//ʧ�ܷ���0���ɹ������豸ID����¼�ɹ�֮����豸�Ĳ���������ͨ����ֵ(�豸���)��Ӧ����Ӧ���豸��
		//m_Log.Add( "err[%d] = CLIENT_Login(&pDhParams->m_devInfo.m_loginid[%d], pDhParams->m_proxyIp[%s], pDhParams->m_proxyPort[%d],pDhParams->m_userName[%s], pDhParams->m_passWord[%s])",
		//	err, pDhParams->m_devInfo.m_loginid, pDhParams->m_proxyIp, pDhParams->m_proxyPort,pDhParams->m_userName, pDhParams->m_passWord);
		m_Log.Add( "err[%08x] = CLIENT_Login(&pDhParams->m_devInfo.m_loginid[%d], pDhParams->m_proxyIp[%s], pDhParams->m_proxyPort[%d],pDhParams->m_userName[%s], pDhParams->m_passWord[%s])",
			GD_GetInforFromDaHua(err), pDhParams->m_devInfo.m_loginid, pDhParams->m_proxyIp, pDhParams->m_proxyPort,pDhParams->m_userName, pDhParams->m_passWord);
		pDhParams->m_devInfo.m_loginid = NULL;
		return -1;

	}
	pDhParams->m_devInfo.m_loginid = lRet;

	////Enable stream
	//BOOL bOpenRet = PLAY_OpenStream(g_devId+1,0,0,1024*500);
	//if(FALSE == bOpenRet)
	//{
	//	m_Log.Add("[ERR]:PLAY_OpenStream failed, error: %d\n", PLAY_GetLastError(g_devId+1));
	//	return -1;
	//}

	////Begin play 
	//BOOL bPlayRet = PLAY_Play(g_devId+1,NULL);
	//if(FALSE == bPlayRet)
	//{
	//	PLAY_CloseStream(g_devId+1);
	//	m_Log.Add("[ERR]:PLAY_Play failed, error: %d\n", PLAY_GetLastError(g_devId+1));
	//	return -1;
	//}

	//Real-time play 
	lRet = CLIENT_RealPlayEx(pDhParams->m_devInfo.m_loginid,atoi(pDhParams->m_devName),0,DH_RType_Realplay);
	if(0 != lRet)
	{
		//Enable stream
		BOOL bOpenRet = PLAY_OpenStream(g_devId+1,0,0,1024*500);
		if(FALSE == bOpenRet)
		{
			m_Log.Add("[ERR]:PLAY_OpenStream failed, error: %d\n", PLAY_GetLastError(g_devId+1));
			CLIENT_StopRealPlayEx(lRet);
			return -1;
		}
		PLAY_SetStreamOpenMode(g_devId+1,1);

	//	PLAY_SetDecCallBackEx(g_devId+1,pDecCBFun,(DWORD)pH264Stream);
		PLAY_SetDecCallBackEx(g_devId+1,pDecCBFun,(DWORD)NULL);

		//Begin play 
		BOOL bPlayRet = PLAY_Play(g_devId+1,NULL);
		if(FALSE == bPlayRet)
		{
			PLAY_CloseStream(g_devId+1);
			CLIENT_StopRealPlayEx(lRet);
			m_Log.Add("[ERR]:PLAY_Play failed, error: %d\n", PLAY_GetLastError(g_devId+1));
			return -1;
		}

		//Real-time play 

		//SetPlayVideoInfo(iDispNum,iChannel,ServerMode);
		//Callback monitor data and then save 
		//���������ж�Ҫ�õ�����YUV���ݻ���H264����
		//����������˵����ҪYUV����
		//���ֱ��ҪYUV���ݻ�����֡������Ҫ���ž�����ð취��Щ�����ʣ�
		//�ʲ����˲������еõ�YUV���ݣ����������������ŵĲ���
		//if(pH264Stream->b_Encode == 1)//YUV����
		//	CLIENT_SetRealDataCallBackEx(lRet, RealDataCallBackEx, (DWORD)pH264Stream, 0x07/*0x00000004*/);
		//else//MPEG4/H264��׼����//���Ƽ�ʹ�ã���Ϊ��MPEG4
		//	CLIENT_SetRealDataCallBackEx(lRet, RealDataCallBackEx, (DWORD)pH264Stream, 0x00000001);
			CLIENT_SetRealDataCallBackEx(lRet, RealDataCallBackEx, (DWORD)NULL, 0x00000001);
	}
	else
	{
		//MessageBox(ConvertString("Fail to play!"), ConvertString("Prompt"));
		//PLAY_Stop(g_devId+1);
		//PLAY_CloseStream(g_devId+1);
		m_Log.Add("[ERR]:CLIENT_RealPlayEx failed, error: %d\n", PLAY_GetLastError(g_devId+1));
		return -1;
	}

	pDhParams->m_devInfo.realPlayerHd = lRet;

	
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
	DEV_DH_PARAMS tmpDH;

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
		strcpy(tmpDH.m_devInfo.m_dstIp,node->data); 
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
	subroot = f_xmlnode_sch_elem(root, "dahua");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�dahua�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����NP��XML!");

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpDH.ServerIP=node->data;
			strcpy(tmpDH.m_proxyIp,node->data);
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
			//tmpDH.nPort=atoi(node->data);
			tmpDH.m_proxyPort = atoi(node->data);
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
			//tmpDH.SrcUrl = node->data;
			strcpy(tmpDH.m_userName,node->data);
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
			//tmpDH.SrcUrl = node->data;
			strcpy(tmpDH.m_passWord,node->data);
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
			//tmpDH.nPort=atoi(node->data);
			//tmpDH.m_playChn = atoi(node->data);
			strcpy(tmpDH.m_devName,node->data);
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�channel�ڵ�!");
			rt = 400;
			goto leave;
		}

		strcpy(Main->m_devParams.m_proxyIp,tmpDH.m_proxyIp);
		Main->m_devParams.m_proxyPort = tmpDH.m_proxyPort;
		strcpy(Main->m_devParams.m_userName,tmpDH.m_userName);
		strcpy(Main->m_devParams.m_passWord,tmpDH.m_passWord);
		strcpy(Main->m_devParams.m_devName,tmpDH.m_devName);

	}


leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
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
