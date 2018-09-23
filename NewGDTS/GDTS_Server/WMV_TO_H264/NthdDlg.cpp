// NthdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "NthdDlg.h"


#include "HKLib/plaympeg4.h"
#include "HKLib/HCNetSDK.h"

extern CWMV_TO_H264Dlg* Main;
static CNthdDlg* pHdDlg=NULL;
// CNthdDlg �Ի���

//MP4
typedef BOOL (__stdcall* _PLAYM4_GETPORT)(LONG* nPort);
typedef BOOL (__stdcall* _PLAYM4_PLAY)(LONG nPort, HWND hWnd);
typedef BOOL (__stdcall* _PLAYM4_SETDECCALLBACKMEND)(LONG nPort,void (CALLBACK* DecCBFun)(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2), long nUser);
typedef BOOL (__stdcall* _PLAYM4_SETSTREAMOPENMODE)(LONG nPort,DWORD nMode);
typedef BOOL (__stdcall* _PlayM4_OPENSTREAM)(LONG nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);
typedef BOOL (__stdcall* _PLAYM4_INPUTDATA)(LONG nPort,PBYTE pBuf,DWORD nSize);
typedef BOOL (__stdcall* _PLAYM4_STOP)(LONG nPort);
typedef BOOL (__stdcall* _PLAYM4_CLOSESTREAM)(LONG nPort);


_PLAYM4_GETPORT				_PlayM4_GetPort = NULL;
_PLAYM4_PLAY				_PlayM4_Play = NULL;
_PLAYM4_SETDECCALLBACKMEND	_PlayM4_SetDecCallBackMend = NULL;
_PLAYM4_SETSTREAMOPENMODE	_PlayM4_SetStreamOpenMode = NULL;
_PlayM4_OPENSTREAM			_PlayM4_OpenStream = NULL;
_PLAYM4_INPUTDATA			_PlayM4_InputData = NULL;
_PLAYM4_STOP				_PlayM4_Stop = NULL;
_PLAYM4_CLOSESTREAM			_PlayM4_CloseStream = NULL;

//Client
#include "HKLib/clntsink.h"
typedef int (__stdcall *pDataRec)(int sid, int iusrdata, int idatatype, char* pdata, int ilen);
typedef int (__stdcall *pMsgBack)(int sid, int opt, int param1, int param2);
typedef int HSESSION;

typedef int (__stdcall* INITSTREAMCLIENTLIB)(void);
typedef int (__stdcall* FINISTREAMCLIENTLIB)(void);
typedef int (__stdcall* HIKS_CREATEPLAYER)(IHikClientAdviseSink* pSink, void* pWndSiteHandle, pDataRec pRecFunc, pMsgBack pMsgFunc,int TransMethod);
typedef int (__stdcall* HIKS_OPENURL)(HSESSION hSession,const char* pszURL,int iusrdata);
typedef int (__stdcall* HIKS_PLAY)(HSESSION hSession);
typedef int (__stdcall* HIKS_STOP)(HSESSION hSession);
typedef int (__stdcall* HIKS_DESTROY)(HSESSION hSession);


INITSTREAMCLIENTLIB			InitStreamClientLib = NULL;
FINISTREAMCLIENTLIB			FiniStreamClientLib = NULL;
HIKS_CREATEPLAYER			HIKS_CreatePlayer = NULL;
HIKS_OPENURL				HIKS_OpenURL = NULL;
HIKS_PLAY					HIKS_Play = NULL;
HIKS_STOP					HIKS_Stop = NULL;
HIKS_DESTROY				HIKS_Destroy = NULL;



IMPLEMENT_DYNAMIC(CNthdDlg, CDialog)

CNthdDlg::CNthdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNthdDlg::IDD, pParent)
{
	pHdDlg = this;

	for(int i=0;i<16;i++)
	{
	//	m_h264[i] = NULL;
		memset(&m_devParams[i],0,sizeof(DEV_HD_PARAMS));
	}


	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	HMODULE hProcessClient=NULL;
	//SetDllDirectory(g_CurrentDir+"\\HKLib\\");

	HMODULE hProcessModule=NULL;

	hProcessModule	=  LoadLibraryA("PlayCtrl.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR��load PlayCtrl.dll failed\n");
		return;
	}
	//2

	_PlayM4_GetPort=(_PLAYM4_GETPORT)GetProcAddress(hProcessModule,"PlayM4_GetPort");
	if(_PlayM4_GetPort == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_GetPort failed\n");
		return;
	}

	_PlayM4_Play=(_PLAYM4_PLAY)GetProcAddress(hProcessModule,"PlayM4_Play");
	if(_PlayM4_Play == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_Play failed\n");
		return;
	}

	_PlayM4_SetDecCallBackMend=(_PLAYM4_SETDECCALLBACKMEND)GetProcAddress(hProcessModule,"PlayM4_SetDecCallBackMend");
	if(_PlayM4_SetDecCallBackMend == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_SetDecCallBackMend failed\n");
		return;
	}

	_PlayM4_SetStreamOpenMode=(_PLAYM4_SETSTREAMOPENMODE)GetProcAddress(hProcessModule,"PlayM4_SetStreamOpenMode");
	if(_PlayM4_SetStreamOpenMode == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_SetStreamOpenMode failed\n");
		return;
	}

	_PlayM4_OpenStream=(_PlayM4_OPENSTREAM)GetProcAddress(hProcessModule,"PlayM4_OpenStream");
	if(_PlayM4_OpenStream == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_OpenStream failed\n");
		return;
	}

	_PlayM4_InputData=(_PLAYM4_INPUTDATA)GetProcAddress(hProcessModule,"PlayM4_InputData");
	if(_PlayM4_InputData == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_InputData failed\n");
		return;
	}

	_PlayM4_Stop=(_PLAYM4_STOP)GetProcAddress(hProcessModule,"PlayM4_Stop");
	if(_PlayM4_Stop == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_Stop failed\n");
		return;
	}
	
	_PlayM4_CloseStream=(_PLAYM4_CLOSESTREAM)GetProcAddress(hProcessModule,"PlayM4_CloseStream");
	if(_PlayM4_CloseStream == NULL)
	{
		m_Log.Add("ERR��GetProcAddress _PlayM4_Stop failed\n");
		return;
	}


	hProcessClient	=  LoadLibraryA("client.dll");
	if(hProcessClient == NULL)
	{
		m_Log.Add("ERR��load client.dll failed\n");
		return;
	}
	//2

	InitStreamClientLib=(INITSTREAMCLIENTLIB)GetProcAddress(hProcessClient,"InitStreamClientLib");
	if(InitStreamClientLib == NULL)
	{
		m_Log.Add("ERR��GetProcAddress InitStreamClientLib failed\n");
		return;
	}

	FiniStreamClientLib=(FINISTREAMCLIENTLIB)GetProcAddress(hProcessClient,"FiniStreamClientLib");
	if(FiniStreamClientLib == NULL)
	{
		m_Log.Add("ERR��GetProcAddress FiniStreamClientLib failed\n");
		return;
	}

	HIKS_CreatePlayer=(HIKS_CREATEPLAYER)GetProcAddress(hProcessClient,"HIKS_CreatePlayer");
	if(HIKS_CreatePlayer == NULL)
	{
		m_Log.Add("ERR��GetProcAddress HIKS_CreatePlayer failed\n");
		return;
	}

	HIKS_OpenURL=(HIKS_OPENURL)GetProcAddress(hProcessClient,"HIKS_OpenURL");
	if(HIKS_OpenURL == NULL)
	{
		m_Log.Add("ERR��GetProcAddress HIKS_OpenURL failed\n");
		return;
	}

	HIKS_Play=(HIKS_PLAY)GetProcAddress(hProcessClient,"HIKS_Play");
	if(HIKS_Play == NULL)
	{
		m_Log.Add("ERR��GetProcAddress HIKS_Play failed\n");
		return;
	}

	HIKS_Stop=(HIKS_STOP)GetProcAddress(hProcessClient,"HIKS_Stop");
	if(HIKS_Stop == NULL)
	{
		m_Log.Add("ERR��GetProcAddress HIKS_Stop failed\n");
		return;
	}

	HIKS_Destroy=(HIKS_DESTROY)GetProcAddress(hProcessClient,"HIKS_Destroy");
	if(HIKS_Destroy == NULL)
	{
		m_Log.Add("ERR��GetProcAddress HIKS_Destroy failed\n");
		return;
	}


	int ret = InitStreamClientLib( );
	if(ret != 0)
		AfxMessageBox("ERR:InitStreamClientLib");



}

CNthdDlg::~CNthdDlg()
{
	//for(int i=0;i<16;i++)
	//{
	//	if(NULL != m_h264[i])
	//	{
	//		delete m_h264[i];
	//	}
	//}

	FiniStreamClientLib();
}

void CNthdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNthdDlg, CDialog)
END_MESSAGE_MAP()


// CNthdDlg ��Ϣ�������

#define DEF_NTHD_FLAG	"nthd"

int CNthdDlg::GetInforFromIni(void * iniPath)
{
	char dst[255];
	CString tmpStr;

	CString thisDir;
	thisDir.Format("%s",(char*)iniPath);

	SourceCount=0;

	ZeroMemory(dst,255);
	GetPrivateProfileString(DEF_NTHD_FLAG,"ProxyIp","",dst,32,thisDir);
	strcpy(m_proxyIp,dst);

	m_proxyPort=GetPrivateProfileInt(DEF_NTHD_FLAG,"ProxyPort",-1,thisDir);
	TRACE("m_proxyIp:%s m_proxyPort:%d\n",m_proxyIp,m_proxyPort);

	while(1)//��ȡconfig.ini������ntqy,��Ӧnew CH264Stream
	{
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("�ﵽ���ת��·������������ֹͣ������%d��%d",SourceCount,g_maxLiveNum);
			break;
		}

		tmpStr.Format("DeviceType%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTHD_FLAG,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_deviceType,dst);


		tmpStr.Format("DeviceIp%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTHD_FLAG,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_deviceIp,dst);


		tmpStr.Format("DevicePort%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_devicePort=GetPrivateProfileInt(DEF_NTHD_FLAG,tmpStr,-1,thisDir);
		TRACE("m_devParams.m_proxyPort:%d\n",m_devParams[SourceCount].m_devicePort);


		tmpStr.Format("UserName%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTHD_FLAG,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_userName,dst);
		TRACE("m_devParams[SourceCount].m_userName:%s\n",m_devParams[SourceCount].m_userName);


		tmpStr.Format("PassWd%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTHD_FLAG,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_passWord,dst);
		TRACE("m_devParams[SourceCount].m_passWord:%s\n",m_devParams[SourceCount].m_passWord);

		//InitProxyServer(NULL);

		m_Log.Add("��ʼ��ͨ��������: ����Ϊ:DeviceIp:%s,DevicePort:%d,UserName:%s,PassWd:%s",
			m_devParams[SourceCount].m_deviceIp,m_devParams[SourceCount].m_devicePort,m_devParams[SourceCount].m_userName,
			m_devParams[SourceCount].m_passWord);
#if 0
		tmpStr.Format("PlayChn0%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_playChn=GetPrivateProfileInt(DEF_NTHD_FLAG,tmpStr,-1,thisDir);
#endif		
		tmpStr.Format("DeviceChn%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_deviceChn=GetPrivateProfileInt(DEF_NTHD_FLAG,tmpStr,-1,thisDir);

		tmpStr.Format("SubChn%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_subChn=GetPrivateProfileInt(DEF_NTHD_FLAG,tmpStr,-1,thisDir);

		tmpStr.Format("Dstip%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTHD_FLAG,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_devInfo.m_dstIp,dst);

		tmpStr.Format("Dstpub%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTHD_FLAG,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_devInfo.m_dstPub,dst);

		tmpStr.Format("Dststream%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_devInfo.m_dstStream=GetPrivateProfileInt(DEF_NTHD_FLAG,tmpStr,-1,thisDir);

		if ( -1 == m_devParams[SourceCount].m_deviceChn || ""==m_devParams[SourceCount].m_devInfo.m_dstIp || ""==m_devParams[SourceCount].m_devInfo.m_dstPub || -1==m_devParams[SourceCount].m_devInfo.m_dstStream)
		{
			m_Log.Add("config.ini�ļ���δ����ȫ��ͨ���˵�DeviceId,DeviceChn,Dstip,Dstpub,Dststream");
			break;
		}

		{
			m_h264[SourceCount]=new CH264Stream;
				if(NULL==m_h264[SourceCount])
				{
					m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV�����ڴ�ʱ���ִ���",SourceCount);
				}

				m_h264[SourceCount]->thisIndex = SourceCount;
				m_h264[SourceCount]->b_Encode	= 1;

				m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[SourceCount].m_devInfo.m_dstIp;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",DEF_CS_NTHD_FLAG,SourceCount);

				m_h264[SourceCount]->m_devType = DEF_NTHD_DEV;

				SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
				StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);
		}

		

		m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceChn%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devParams[SourceCount].m_deviceChn,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstIp,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstPub,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstStream);//����������
		TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);
		SourceCount++;

	}


	return 0;
}

//#define Main->m_secName	"hk"
int CNthdDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{

//
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;

	ZeroMemory(dst,255);
	GetPrivateProfileString("common","ProxyIp","",dst,32,thisDir);
	strcpy(m_proxyIp,dst);

	m_proxyPort=GetPrivateProfileInt("common","ProxyPort",-1,thisDir);
	TRACE("m_proxyIp:%s m_proxyPort:%d\n",m_proxyIp,m_proxyPort);

	tmpStr.Format("DeviceType%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_deviceType,dst);


	tmpStr.Format("DeviceIp%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_deviceIp,dst);


	tmpStr.Format("DevicePort%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[SourceCount].m_devicePort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_devParams.m_proxyPort:%d\n",m_devParams[SourceCount].m_devicePort);


	tmpStr.Format("UserName%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_userName,dst);
	TRACE("m_devParams[SourceCount].m_userName:%s\n",m_devParams[SourceCount].m_userName);


	tmpStr.Format("PassWd%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_passWord,dst);
	TRACE("m_devParams[SourceCount].m_passWord:%s\n",m_devParams[SourceCount].m_passWord);

	//InitProxyServer(NULL);

	m_Log.Add("��ʼ��ͨ��������: ����Ϊ:DeviceIp:%s,DevicePort:%d,UserName:%s,PassWd:%s",
		m_devParams[SourceCount].m_deviceIp,m_devParams[SourceCount].m_devicePort,m_devParams[SourceCount].m_userName,
		m_devParams[SourceCount].m_passWord);
#if 0
	tmpStr.Format("PlayChn0%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[SourceCount].m_playChn=GetPrivateProfileInt(DEF_NTHD_FLAG,tmpStr,-1,thisDir);
#endif		
	tmpStr.Format("DeviceChn%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[SourceCount].m_deviceChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

	tmpStr.Format("SubChn%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[SourceCount].m_subChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

	tmpStr.Format("Dstip%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_devInfo.m_dstIp,dst);

	tmpStr.Format("Dstpub%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_devInfo.m_dstPub,dst);

	tmpStr.Format("Dststream%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[SourceCount].m_devInfo.m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	//

#if 0
	tmpStr.Format("PlayChn0%d",nIndex);
	ZeroMemory(dst,255);
	m_devParams[nIndex].m_playChn=GetPrivateProfileInt(common,tmpStr,-1,thisDir);
#endif		


	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV�����ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[nIndex]->thisIndex = nIndex;
	m_h264[nIndex]->b_Encode	= 1;//(g_bUseTc==1?1:0);

	m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_devInfo.m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_devInfo.m_dstPub,nIndex);
	//m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,nIndex);

	//m_h264[nIndex]->pNetVideo = &m_netVideo[nIndex];//��ʱʹ��
	m_h264[nIndex]->m_devType = DEF_NTHD_DEV;

	SetParams(m_h264[nIndex],&m_devParams[nIndex]);
	//StartPull(m_h264[nIndex],&m_devParams[nIndex]);

	m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceChn%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		SourceCount,
		SourceCount,m_devParams[SourceCount].m_deviceChn,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstIp,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstPub,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstStream);//����������
	TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);

	return 0;
}



//����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
//�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
int CNthdDlg::SetDefaultParams(void * pParams)
{
	//return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream��ȫ16��ʱ������ڴ�ʱ���ִ���",SourceCount);
		}

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->b_Encode	= 1;//(g_bUseTc==1?1:0);

		m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[0].m_devInfo.m_dstIp;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[0].m_devInfo.m_dstPub,SourceCount);

		//m_h264[SourceCount]->Init("http://192.168.0.251:8080/live1",ServerIP,PubName,SourceCount,this->GetSafeHwnd());
		//m_h264[SourceCount]->StartGet();

		//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
		//m_h264[SourceCount]->h_stream = m_h264[SourceCount]->StreamParams.hWnd;
		m_h264[SourceCount]->m_devType = DEF_NTHD_DEV;

	}
	return 0;
}

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

#if 0

<company>4</company>

<hk>
<type></type>//�豸����
<ver></ver>//�汾
<ip></ip>//��ַ
<port></port>//�˿�
<user></user>//�û���
<passwd></passwd>//����
<channel></channel>//Ĭ�ϲ���ͨ��
<pipe></pipe>//Ĭ���豸ͨ��
</hk>

#endif

#if 0

<nthd>
<type></type>//�豸����
<ip></ip>//��ַ
<port></port>//�˿�
<user></user>//�û���
<passwd></passwd>//����
<mainchl></mainchl>//��ͨ��
<subchl></subchl>//��ͨ��
</nthd>

#endif


int CNthdDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_HD_PARAMS tmpHD;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	CString CH;

	memset(&tmpHD,0,sizeof(tmpHD));

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
		m_Log.Add("xml��������δ���ҵ�Command�ڵ�!");
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
		strcpy(tmpHD.m_devInfo.m_dstIp,node->data); 
		//m_Log.Add("��������serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=8)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���hk == 4!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "nthd");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�microvideo�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����HK��XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpHD.m_deviceType,node->data);
			//m_Log.Add("��������type��%s",tmpType);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�type�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpHD.ServerIP=node->data;
			strcpy(tmpHD.m_deviceIp,node->data);
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
			//tmpHD.nPort=atoi(node->data);
			tmpHD.m_devicePort = atoi(node->data);
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
			//tmpHD.SrcUrl = node->data;
			strcpy(tmpHD.m_userName,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			//tmpHD.SrcUrl = node->data;
			strcpy(tmpHD.m_passWord,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "mainchl", MAX_LEN);
		if (node != NULL)
		{
			//tmpHD.nPort=atoi(node->data);
			tmpHD.m_deviceChn = atoi(node->data) - 1 ;
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "subchl", MAX_LEN);
		if (node != NULL)
		{
			//tmpHD.nPort=atoi(node->data);
			tmpHD.m_subChn = atoi(node->data) ;
		//	m_Log.Add("��������m_subChn��%d",tmpHD.m_subChn);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
			rt = 400;
			goto leave;
		}
	}

	chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
	if (chNode != NULL)
	{
		if(0 == chNode->data_siz)
		{
			char srcCam[256];
			m_Log.Add("��������chΪ��ֵ!");		
			sprintf(srcCam,"rtsp://%s:%d/%s:%d:%s:%d:%d:%s:%s/av_stream",m_proxyIp,m_proxyPort,tmpHD.m_deviceIp,tmpHD.m_devicePort,
				tmpHD.m_deviceType,tmpHD.m_deviceChn,tmpHD.m_subChn,tmpHD.m_userName,tmpHD.m_passWord);
			m_Log.Add("������Ƶ��%s�Ƿ���ڣ�",srcCam);

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=	CheckExit(srcCam);
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
			CH = chNode->data;
			if(atoi(CH)>=g_maxLiveNum)
			{
				int ntmp = atoi(CH);
				ntmp = ntmp%g_maxLiveNum;
				CH.Format("%d",ntmp);
			}

			CString tmpSrcID;
			tmpSrcID.Format("rtsp://%s:%d/%s:%d:%s:%d:%d:%s:%s/av_stream",m_proxyIp,m_proxyPort,tmpHD.m_deviceIp,tmpHD.m_devicePort,
				tmpHD.m_deviceType,tmpHD.m_deviceChn,tmpHD.m_subChn,tmpHD.m_userName,tmpHD.m_passWord);

			m_h264[atoi(CH)]->m_Info.s_DevID = tmpSrcID;
			m_h264[atoi(CH)]->m_Info.s_PubServer = tmpHD.m_devInfo.m_dstIp;
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",tmpSrcID,atoi(CH));

			{
				strcpy(m_devParams[atoi(CH)].m_deviceIp,tmpHD.m_deviceIp);
				m_devParams[atoi(CH)].m_devicePort = tmpHD.m_devicePort;
				strcpy(m_devParams[atoi(CH)].m_userName,tmpHD.m_userName);
				strcpy(m_devParams[atoi(CH)].m_passWord,tmpHD.m_passWord);
				m_devParams[atoi(CH)].m_deviceChn = tmpHD.m_deviceChn;
				m_devParams[atoi(CH)].m_subChn = tmpHD.m_subChn;

				strcpy(m_devParams[atoi(CH)].m_deviceType,tmpHD.m_deviceType);

			}

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(tmpSrcID,atoi(CH),tmpHD.m_devInfo.m_dstIp);
			ReleaseMutex(Main->m_Lock);

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
		m_Log.Add("xml��������ch�ڵ��ֵΪ��!");
		rt = 400;
		goto leave;
	}
	//m_Log.Add("������ͨ��%d: ����Ϊ:Serverip:%s Company:%s DevType:%s Version:%s IP:%s Port:%s Url:%s\n",atoi(CH), pubserverip,"microvideo",tmpType, tmpVer, tmpIp, tmpPort, tmpSource);//����������
	//TRACE("DevType:%s Version:%s IP:%s Port:%s Url:%s", tmpType, tmpVer, tmpIp, tmpPort, tmpSource);



leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}

CString CNthdDlg::CheckExit(CString str)
{
	CString UrlStr="",CamID = str;

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//���֮ǰ�Ѿ�����ͣ,��ֱ�ӷ��ص�ǰ���л�����BMS�·��л����
				m_Log.Add("��Ƶ:%s �Ѿ�ֹͣ���ţ�������!",CamID);
				return "";

			}

			UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);
			m_Log.Add("��Ƶ:%s ����!URL:%s",CamID,UrlStr);


			m_h264[i]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š�

			return UrlStr;
		}


	}
	m_Log.Add("��Ƶ:%s ������!",CamID);

	return UrlStr;
}


CString CNthdDlg::ChangeChannel(CString sId,int CH,CString str)
{
	CString UrlStr="";

	if(CH >= g_maxLiveNum)
	{//��ʱ�������ת��·��
		m_Log.Add("ԭ��ƵӦ���л�ͨ��Ϊ:%d \n!",CH);
		CH = CH%g_maxLiveNum;
		SourceCount = g_maxLiveNum;
		m_Log.Add("ʵ����ƵӦ���л�ͨ��Ϊ:%d \n!",CH);
	}

	if (CH>=SourceCount)
	{

		m_Log.Add("CH��ֵ���ܴ���%d������!",g_maxLiveNum);
		return "";
	}
	
	//TRACE("");
	m_h264[CH]->m_Info.s_DevID = sId;//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1


	m_Log.Add("����׼���л�%s,��ͨ��%d,������%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	if (NULL!=m_h264[CH]->h_stream)
	{//��·�Ѿ���ʹ�ã���Ҫ�ȹر��ٽ��в���
		//m_netVideo[CH].CloseVideo();

		ClosePull(m_h264[CH],&m_devParams[CH]);

		m_h264[CH]->h_stream=NULL;

	}

	m_h264[CH]->m_Info.s_PubServer=str;

	//m_h264[CH]->Init(&m_devDfhsParams,this->GetSafeHwnd());
	//m_h264[CH]->StartGet(m_devDfhsParams.m_devicePort);
	SetParams(m_h264[CH],&m_devParams[CH]);
//	StartPull(m_h264[CH],&m_devParams[CH]);
	

	//m_h264[CH]->pNetVideo->OpenVideo(m_devDfhsParams.m_devicePort);

	//m_h264[CH]->h_stream = &m_devParams.m_devInfo[CH].m_deviceChn;

	{
		CString UrlStr="";
		UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
		m_Log.Add("������Ƶ%s �л��ɹ�%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
		m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����

		return UrlStr;
	}


	return UrlStr;


}

//LONG lPort; //ȫ�ֵĲ��ſ�port��
/**
 * @brief HK sdk dec callback fun.
 * @param[in] nPort
 * @param[in] pBuf
 * @param[in] nSize
 * @param[in] p_FrameInfo
 * @param[in] nUser
 * @param[in] reserved2
 * 
 * ��������: ��������ص�����
 * ����ʱ�䣺2012-12-28
 * ������:   ����
 * �޸�����        
 * �汾��  ��V1.0   
 * �޸���	      
 * �޸�����
 * �޸�����
 * ��Σ�nPort��pBuf��nSize��p_FrameInfo��nUser��nReserved2
 * ���: ��Ƶyuv����
 * ���� ����
*/
void __stdcall HKDecCallBack2(long nPort, char*pBuf, long size, FRAME_INFO* pFrameInfo, long nUser, long reserved2)
{

//	TRACE(" W:%d H:%d stamp:%d type:%d %d %d\n",pFrameInfo->nWidth,pFrameInfo->nHeight,pFrameInfo->nStamp,pFrameInfo->nType,pFrameInfo->nFrameRate,pFrameInfo->dwFrameNum);

	if(pFrameInfo->nType != T_YV12)
		return;

#if 1
	CH264Stream* pH264=m_h264[nUser];
	if(pH264 == NULL)
		return;

	pH264->b_PullLive = 0;

	if (false ==pH264->b_start)
	{
		
		pH264->first=true;
		//pH264->StartPush(pFrameInfo->nWidth,pFrameInfo->nHeight);
		pH264->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264->b_start=true;
	}
	
	if(pH264->p_YuvArray.GetSize()<60)
	{
		if (NULL == pBuf)
		{
			return ;
		}
		if (0 == size)
		{
			return ;
		}
		MyAvFrame *m_Frame=new MyAvFrame(pFrameInfo->nWidth,pFrameInfo->nHeight);
		if (NULL == m_Frame)
		{
			m_Log.Add("HKDecCallBack2����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
			return ;
		}
		if (NULL == m_Frame->p_data)
		{

			return ;
		}

		memcpy(m_Frame->p_data,pBuf,pFrameInfo->nWidth*pFrameInfo->nHeight*3/2);

		pH264->m_streamlock.Lock();
		pH264->p_YuvArray.Add(m_Frame);
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
	}

#endif
	return;

}


// CNthdDlg ��Ϣ�������
//void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dUser)
int __stdcall dataCBFunc(int sid, int iusrdata, int idatatype, char* pBuffer, int dwBufSize)
{
    //HWND hWnd = GetConsoleWindow();
	DWORD dwUser = iusrdata;
	

	CH264Stream* pH264=m_h264[dwUser];
	if(pH264 == NULL)
		return -1;
//TRACE("user data is %d dwBufSize is %d type is %d  chn is %d\n",iusrdata,dwBufSize,idatatype,pHdDlg->m_devParams[dwUser].m_playChn);
	switch (idatatype)
	{
	case 0: //ϵͳͷ
		if(-1 != pHdDlg->m_devParams[dwUser].m_playChn)
			_PlayM4_Stop(pHdDlg->m_devParams[dwUser].m_playChn);

		//if (!_PlayM4_GetPort(&lPort))  //��ȡ���ſ�δʹ�õ�ͨ����
		if (!_PlayM4_GetPort(&pHdDlg->m_devParams[dwUser].m_playChn))  //��ȡ���ſ�δʹ�õ�ͨ����
		{
			TRACE("************1***********\n");
			break;
		}
		//m_iPort = lPort; //��һ�λص�����ϵͳͷ������ȡ�Ĳ��ſ�port�Ÿ�ֵ��ȫ��port���´λص�����ʱ��ʹ�ô�port�Ų���
		if (dwBufSize > 0)
		{
			if (!_PlayM4_SetStreamOpenMode(pHdDlg->m_devParams[dwUser].m_playChn, STREAME_REALTIME))  //����ʵʱ������ģʽ
			{
				TRACE("************2***********\n");
				break;
			}

			if (!_PlayM4_OpenStream(pHdDlg->m_devParams[dwUser].m_playChn, (PBYTE)pBuffer, dwBufSize, 1024*1024)) //�����ӿ�
			{
				TRACE("************3***********\n");
				break;
			}

			if (!_PlayM4_SetDecCallBackMend(pHdDlg->m_devParams[dwUser].m_playChn,HKDecCallBack2,dwUser))
			{
				TRACE("************4***********\n");
				break;
			}

			if (!_PlayM4_Play(pHdDlg->m_devParams[dwUser].m_playChn, NULL)) //���ſ�ʼ
			{
				TRACE("************5***********\n");
				break;
			}
		}
	case 1:   //��������
		if (dwBufSize > 0 && pHdDlg->m_devParams[dwUser].m_playChn != -1)
		{
			if (!_PlayM4_InputData(pHdDlg->m_devParams[dwUser].m_playChn, (PBYTE)pBuffer, dwBufSize))
			{
				break;
			} 
		}
	}

	return 0;
}

int __stdcall msgCBFunc(int sid, int opt, int param1, int param2)
{
	TRACE("sid is %d opt is %d param1 is %d param2 is%d\n",sid,opt,param1,param2);

	return 0;
}

int CNthdDlg::SetParams(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_HD_PARAMS *pHDParams = (DEV_HD_PARAMS *)pParams;//&m_devParams[pH264Stream->thisIndex];

	pH264Stream->m_Info.s_DevID.Format("rtsp://%s:%d/%s:%d:%s:%d:%d:%s:%s/av_stream",
		m_proxyIp,m_proxyPort,							//��ý���ַ���˿�
		pHDParams->m_deviceIp,pHDParams->m_devicePort,	//�豸��ַ���˿�
		pHDParams->m_deviceType,						//�豸����
		pHDParams->m_deviceChn,pHDParams->m_subChn,		//����ͨ��
		pHDParams->m_userName,pHDParams->m_passWord);	//�û�������

	if (false ==pH264Stream->b_start)
	{
		pH264Stream->first=true;
		//pH264->StartPush(pFrameInfo->nWidth,pFrameInfo->nHeight);
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}
	int ret = pH264Stream->StartPull(pStream,pParams);

	return 0;

}

int CNthdDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_HD_PARAMS *pHDParams = &m_devParams[pH264Stream->thisIndex];
	int ret=0;

	pHDParams->m_devInfo.m_userId = HIKS_CreatePlayer(NULL, NULL, dataCBFunc, msgCBFunc, 0);
	if (pHDParams->m_devInfo.m_userId == -1)
	{
		TRACE("HIKS_CreatePlayer error\n");
		return -1;
	}

	ret = HIKS_OpenURL(pHDParams->m_devInfo.m_userId,pH264Stream->m_Info.s_DevID,pH264Stream->thisIndex) ;
	if (ret == -1)
	{
		TRACE("HIKS_OpenURL error\n");
		HIKS_Destroy(pHDParams->m_devInfo.m_userId);
		return -1;
	}

	ret = HIKS_Play(pHDParams->m_devInfo.m_userId);
	if(ret == -1)
	{
		TRACE("HIKS_Play error\n");
		return -1;
	}

	pH264Stream->h_stream = &pHDParams->m_deviceChn;
	return 0;
}

int CNthdDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_HD_PARAMS *pHDParams = (DEV_HD_PARAMS *)pParams;
	//---------------------------------------

	HIKS_Stop(m_devParams[pH264Stream->thisIndex].m_devInfo.m_userId);// pHDParams->m_devInfo.m_userId);

	_PlayM4_Stop(m_devParams[pH264Stream->thisIndex].m_playChn);

	_PlayM4_CloseStream(m_devParams[pH264Stream->thisIndex].m_playChn);

	pH264Stream->h_stream = NULL;

	return 0;
}

