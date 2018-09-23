// NtqyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "NtqyDlg.h"

extern CWMV_TO_H264Dlg* Main;

static CNtqyDlg* pNtqyDlg=NULL;

typedef void	(*PLAYERINIT)(void);
typedef char*	(*GETDLLVERSION)(void);
typedef int		(*STARTPLAYER)(const char *ipc,int Chan,void* handle);
typedef void	(*STOPPLAYER)(void *handle);
typedef int		(*SETPLAYERFUNC)(void * handle,void *,void *);
typedef int		(*PLAYERLOGIN)(const char *usr, const char *pw, const char *addr, const char *port,void * &handle);
typedef void	(*PLAYERUNLOGIN)(void *handle);
typedef int		(*GETDEVCOUNT)(const char* &list,int &len,void *handle);
typedef void	(*PLAYERFREE)(void);
//1
PLAYERINIT		PlayerInit=NULL;
GETDLLVERSION	GetDllVersion=NULL;
STARTPLAYER		StartPlayer=NULL;
STOPPLAYER		StopPlayer=NULL;
SETPLAYERFUNC	SetPlayerFunc=NULL;
PLAYERLOGIN		PlayerLogin=NULL;
PLAYERUNLOGIN	PlayerUnLogin=NULL;
GETDEVCOUNT		GetDevCount=NULL;
PLAYERFREE		PlayerFree=NULL;


// CNtqyDlg �Ի���

IMPLEMENT_DYNAMIC(CNtqyDlg, CDialog)

CNtqyDlg::CNtqyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNtqyDlg::IDD, pParent)
{
	pNtqyDlg=this;
	for(int i=0;i<16;i++)
	{
	//	m_h264[i] = NULL;
		m_devParams.m_devInfo[i].m_handle = NULL;
	//	memset(&m_h3cParams[i],0,sizeof(DEV_H3C_PARAMS));
	}

	ULONG ulRet;

	//loadH3cLib();


	//ulRet = IMOS_Initiate("N/A", 8800, BOOL_TRUE, BOOL_TRUE);//IMOS_Initiate(szServerAdd, 8800, BOOL_TRUE, BOOL_TRUE);
	//if (ERR_COMMON_SUCCEED != ulRet)
	//{
	//	//log
	//	m_Log.Add("IMOS SDK��ʼ��");
	//	return;
	//}


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	HMODULE hProcessModule=NULL;
	hProcessModule	=  LoadLibraryA("LNT_Player.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR��load LNT_Player.dll failed\n");
		return;
	}
	//2
	PlayerInit=(PLAYERINIT)GetProcAddress(hProcessModule,"PlayerInit");
	if(PlayerInit == NULL)
	{
		m_Log.Add("ERR��load PlayerInit failed\n");
		return;
	}
	GetDllVersion=(GETDLLVERSION)GetProcAddress(hProcessModule,"GetDllVersion");
	if(GetDllVersion == NULL)
	{
		m_Log.Add("ERR��load GetDllVersion failed\n");
		return;
	}
	StartPlayer=(STARTPLAYER)GetProcAddress(hProcessModule,"StartPlayer");
	if(StartPlayer == NULL)
	{
		m_Log.Add("ERR��load StartPlayer failed\n");
		return;
	}
	StopPlayer=(STOPPLAYER)GetProcAddress(hProcessModule,"StopPlayer");
	if(StopPlayer == NULL)
	{
		m_Log.Add("ERR��load StopPlayer failed\n");
		return;
	}
	SetPlayerFunc=(SETPLAYERFUNC)GetProcAddress(hProcessModule,"SetPlayerFunc");
	if(SetPlayerFunc == NULL)
	{
		m_Log.Add("ERR��load SetPlayerFunc failed\n");
		return;
	}
	PlayerLogin=(PLAYERLOGIN)GetProcAddress(hProcessModule,"PlayerLogin");
	if(PlayerLogin == NULL)
	{
		m_Log.Add("ERR��load PlayerLogin failed\n");
		return;
	}
	PlayerUnLogin=(PLAYERUNLOGIN)GetProcAddress(hProcessModule,"PlayerUnLogin");
	if(PlayerUnLogin == NULL)
	{
		m_Log.Add("ERR��load PlayerUnLogin failed\n");
		return;
	}
	GetDevCount=(GETDEVCOUNT)GetProcAddress(hProcessModule,"GetDevCount");
	if(GetDevCount == NULL)
	{
		m_Log.Add("ERR��load GetDevCount failed\n");
		return;
	}
	PlayerFree=(PLAYERFREE)GetProcAddress(hProcessModule,"PlayerFree");
	if(PlayerFree == NULL)
	{
		m_Log.Add("ERR��load PlayerFree failed\n");
		return;
	}

	TRACE("This version is %s\n",GetDllVersion());

	PlayerInit();

}

CNtqyDlg::~CNtqyDlg()
{
	for(int i=0;i<16;i++)
	{
		if(NULL != m_h264[i])
		{
			delete m_h264[i];
		}
	}
	PlayerFree();
}

void CNtqyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNtqyDlg, CDialog)
END_MESSAGE_MAP()

void CNtqyDlg::MsgBackFunc(int type,int code,const char *msg,const char *ipc)
{
	TRACE("[MsgBackFunc]:the msg is type %d code %d msg is %s ipc is %s\n",type,code,msg,ipc);
}

void CNtqyDlg::MyCallBackFun(const char *ipc,int datatype,const char *buf,unsigned int len,void*user)
{
//	TRACE("[MyCallBackFun]:the ipc is %s-- data len is %d-- %p\n",ipc,len,user);
	CH264Stream* pH264Stream=NULL;

	int bFull  = 0;
	int i =0;

	for(i=0;i<g_maxLiveNum;i++)
	{
		//if(strcmp(ipc,pNtqyDlg->m_devParams.m_devInfo[i].m_deviceId)==0)
		//	pH264Stream = pNtqyDlg->m_h264[i];
		if(user == pNtqyDlg->m_devParams.m_devInfo[i].m_handle)
		//	pH264Stream = pNtqyDlg->m_h264[i];
			pH264Stream = m_h264[i];


	}
#if 0
	{
		static int itimes = 0;
		static DWORD myT= 0;
		if(itimes%100 == 0)
		{
			//time_t old_time=0;
			//time(&old_time);
			TRACE("*******decode 100 frame %d \n",GetTickCount()- myT);
			myT = GetTickCount();
		}
		itimes++;
	}
	//return TRUE;
#endif
	if(pH264Stream == NULL)
		return;

	do{
		if(bFull == 1)
		{
			TRACE("buffer is full\n");
			Sleep(10);
		}

		pH264Stream->m_streamlock.Lock();
		if(pH264Stream->p_EncodeArray.GetSize()<200)
		{
			if (NULL ==buf)
			{
				m_Log.Add("��Ƶ����Ϊ�գ�");
				pH264Stream->m_streamlock.Unlock();
				return ;
			}
			if (0 == len )
			{
				m_Log.Add("��Ƶ����Ϊ�գ�");
				pH264Stream->m_streamlock.Unlock();
				return ;
			}

			MyAvFrame *m_Frame=NULL;
			//if(lFrameLen<(352*288*3/2+1))//�����޷��ж����С�����������䵱ǰ��С��Ϊ�ô���������С
			//	m_Frame = new MyAvFrame(352,288);//Ĭ�����ô�С CIF��׼
			//else
			m_Frame = new MyAvFrame(len,1);//���޷��жϳ���Ƶ��С


			if (NULL == m_Frame)
			{
				m_Log.Add("new MyAvFrame��ʱ���ڴ�������");
				pH264Stream->m_streamlock.Unlock();
				return ;
			}

			if(0 == pH264Stream->curTime)
			{
				pH264Stream->curTime = GetTickCount();//gettick
				m_Frame->i_time = 0;
				//		TRACE("input %d\n",m_Frame->i_time);
			}
			else
			{

				m_Frame->i_time = (GetTickCount()-pH264Stream->curTime);
				pH264Stream->curTime = GetTickCount();
				//		TRACE("input %d\n",m_Frame->i_time);
			}


			if (NULL == m_Frame->p_data)
			{
				m_Log.Add("���䵽��MyAvFrame���p_dataָ��Ϊ�գ�");
				pH264Stream->m_streamlock.Unlock();
				return ;
			}
			memcpy(m_Frame->p_data,buf,len);//new MyAvFrame(352,288)<--byteVideoData

			pH264Stream->p_EncodeArray.Add(m_Frame);//��Ƶ���ݴ�����������ݻ�����CArray<MyAvFrame*,MyAvFrame*>
			bFull = 0;
		}else 
			bFull = 1;

		pH264Stream->m_streamlock.Unlock();
	}while(bFull == 1);


}

#define DEF_NTQY_FLAY	"ntqy"

int CNtqyDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("ProxyIp%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams.m_proxyIp,dst);


	tmpStr.Format("ProxyPort%d",nIndex);
	ZeroMemory(dst,255);
	m_devParams.m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_devParams.m_proxyPort:%d\n",m_devParams.m_proxyPort);


	tmpStr.Format("UserName%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams.m_userName,dst);
	TRACE("m_devParams.m_userName:%s\n",m_devParams.m_userName);


	tmpStr.Format("PassWd%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams.m_passWord,dst);
	TRACE("m_devParams.m_passWord:%s\n",m_devParams.m_passWord);

	//InitProxyServer(NULL);

	m_Log.Add("��ʼ��ͨ��������: ����Ϊ:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		m_devParams.m_proxyIp,m_devParams.m_proxyPort,m_devParams.m_userName,
		m_devParams.m_passWord);
	
	tmpStr.Format("DeviceId%d",nIndex);
	ZeroMemory(dst,255);

	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams.m_devInfo[SourceCount].m_deviceId,dst);
	//strcpy(m_devParams[nIndex].m_deviceName,dst);


	tmpStr.Format("DeviceChn%d",nIndex);
	ZeroMemory(dst,255);
	m_devParams.m_devInfo[SourceCount].m_deviceChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir);



	tmpStr.Format("Dstip%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams.m_devInfo[SourceCount].m_dstIp,dst);

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams.m_devInfo[SourceCount].m_dstPub,dst);

	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_devParams.m_devInfo[SourceCount].m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);



	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_H3CIPC_DEV�����ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);


	m_h264[nIndex]->thisIndex = SourceCount;
	m_h264[nIndex]->m_Info.s_PubServer=m_devParams.m_devInfo[SourceCount].m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",SourceCount);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams.m_devInfo[SourceCount].m_dstPub,SourceCount);
	m_h264[nIndex]->m_devType = DEF_NTQY_DEV;

	SetParams(m_h264[nIndex],&m_devParams);
	//StartPull(m_h264[nIndex],&m_devParams[nIndex]);



	return 0;
}

// CNtqyDlg ��Ϣ�������
int CNtqyDlg::GetInforFromIni(void* iniPath)
{
	char dst[255];
	CString tmpStr;

	CString thisDir;
	thisDir.Format("%s",(char*)iniPath);

	SourceCount=0;

	tmpStr.Format("ProxyIp");

	char Server[255];
	ZeroMemory(Server,255);

	GetPrivateProfileString(DEF_NTQY_FLAY,tmpStr,"",Server,255,thisDir);

	 //m_devParams.m_proxyIp=Server;
	strcpy(m_devParams.m_proxyIp,Server);

	 TRACE("m_devParams.m_proxyIp:%s\n",m_devParams.m_proxyIp);
	if("" == m_devParams.m_proxyIp)
	{
		m_Log.Add("config.ini�ļ���δ���ҵ��ط����µ�m_proxyIp");
		return 0;
	}
	else
	{
		tmpStr.Format("ProxyPort");
		
		ZeroMemory(dst,255);
		m_devParams.m_proxyPort=GetPrivateProfileInt(DEF_NTQY_FLAY,tmpStr,-1,thisDir);
		TRACE("m_devParams.m_proxyPort:%d\n",m_devParams.m_proxyPort);

		tmpStr.Format("UserName");
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTQY_FLAY,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams.m_userName,dst);
		TRACE("m_devParams.m_userName:%s\n",m_devParams.m_userName);

		tmpStr.Format("PassWd");
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTQY_FLAY,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams.m_passWord,dst);
		TRACE("m_devParams.m_passWord:%s\n",m_devParams.m_passWord);


		if (-1 == m_devParams.m_proxyPort || "" == m_devParams.m_userName || "" == m_devParams.m_passWord)
		{
			m_Log.Add("config.ini�ļ���δ����ȫ�ط����µ�ProxyPort,UserName,PassWd");

			return 0;
		}

		//InitProxyServer(NULL);

		m_Log.Add("��ʼ��ͨ��������: ����Ϊ:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		 m_devParams.m_proxyIp,m_devParams.m_proxyPort,m_devParams.m_userName,
		 m_devParams.m_passWord);
	
	}
	while(1)//��ȡconfig.ini������ntqy,��Ӧnew CH264Stream
	{
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("�ﵽ���ת��·������������ֹͣ������%d��%d",SourceCount,g_maxLiveNum);
			break;
		}
		tmpStr.Format("DeviceId%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTQY_FLAY,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams.m_devInfo[SourceCount].m_deviceId,dst);

		tmpStr.Format("DeviceChn%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams.m_devInfo[SourceCount].m_deviceChn=GetPrivateProfileInt(DEF_NTQY_FLAY,tmpStr,-1,thisDir);

		tmpStr.Format("Dstip%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTQY_FLAY,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams.m_devInfo[SourceCount].m_dstIp,dst);

		tmpStr.Format("Dstpub%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(DEF_NTQY_FLAY,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams.m_devInfo[SourceCount].m_dstPub,dst);

		tmpStr.Format("Dststream%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams.m_devInfo[SourceCount].m_dstStream=GetPrivateProfileInt(DEF_NTQY_FLAY,tmpStr,-1,thisDir);

		if ("" == m_devParams.m_devInfo[SourceCount].m_deviceId || -1 == m_devParams.m_devInfo[SourceCount].m_deviceChn || ""==m_devParams.m_devInfo[SourceCount].m_dstIp || ""==m_devParams.m_devInfo[SourceCount].m_dstPub || -1==m_devParams.m_devInfo[SourceCount].m_dstStream)
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

				m_h264[SourceCount]->m_Info.s_PubServer=m_devParams.m_devInfo[SourceCount].m_dstIp;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",DEF_CS_NTQY_FLAG,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
				m_h264[SourceCount]->m_devType = DEF_NTQY_DEV;

				SetParams(m_h264[SourceCount],&m_devParams);
				StartPull(m_h264[SourceCount],&m_devParams);
		}

		

		m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceId%d:%s,DeviceChn%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devParams.m_devInfo[SourceCount].m_deviceId,
			SourceCount,m_devParams.m_devInfo[SourceCount].m_deviceChn,
			SourceCount,m_devParams.m_devInfo[SourceCount].m_dstIp,
			SourceCount,m_devParams.m_devInfo[SourceCount].m_dstPub,
			SourceCount,m_devParams.m_devInfo[SourceCount].m_dstStream);//����������
		TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);
		SourceCount++;

	}

	return 0;
}

int CNtqyDlg::SetDefaultParams(void * pParams)
{
	if(SourceCount>=g_maxLiveNum)//���������ӣ���ȫ��16·
		return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream��ȫ16��ʱ������ڴ�ʱ���ִ���",SourceCount);
		}

		m_h264[SourceCount]->b_Encode	= (g_bUseTc==1?1:0);
		m_h264[SourceCount]->thisIndex = SourceCount;

		m_h264[SourceCount]->m_Info.s_PubServer=m_devParams.m_devInfo[SourceCount].m_dstIp;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
	//	m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",DEF_CS_NTQY_FLAG,SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams.m_devInfo[0].m_dstPub,SourceCount);

		//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
		m_h264[SourceCount]->m_devType = DEF_NTQY_DEV;

	}
	return 0;

}


#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20


int CNtqyDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	DEV_NTQY_INFO  tmpInfo;
	DEV_H3C_PARAMS tmpH3c;

	m_type = DEF_NTQY_DEV;

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
#if 0
	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=7)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���6!\n");
		rt = 400;
		goto leave;
	}
#endif

	subroot = f_xmlnode_sch_elem(root, "ntqy");
	if(subroot != NULL)
	{//����ntqy

		m_Log.Add("��ʼ��ͨ����xml����!");
		node = f_xmlnode_sch_elem_ext(subroot, "proxyIp", MAX_LEN);
		if (node != NULL)
		{
			sprintf(m_devParams.m_proxyIp,"%s",node->data);
			m_Log.Add("��������proxyIP��%s",m_devParams.m_proxyIp);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�proxyIP�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "proxyPort", MAX_LEN);
		if (node != NULL)
		{
			m_devParams.m_proxyPort=atoi(node->data);
			m_Log.Add("��������proxyPort��%d",m_devParams.m_proxyPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�proxyPort�ڵ�!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "userName", MAX_LEN);
		if (node != NULL)
		{
			strcpy(m_devParams.m_userName,node->data);
			m_Log.Add("��������userName��%s",m_devParams.m_userName);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�userName�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "passWord", MAX_LEN);
		if (node != NULL)
		{
			strcpy(m_devParams. m_passWord,node->data);
			m_Log.Add("��������passWord��%s",m_devParams.m_passWord);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�passWord�ڵ�!");
			rt = 400;
			goto leave;
		}		
		node = f_xmlnode_sch_elem_ext(subroot, "deviceId", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpInfo.m_deviceId,node->data);
			if(strlen(tmpInfo.m_deviceId)==0)
			{
				m_Log.Add("xml��������δ���ҵ�deviceId�ڵ�!");
				rt = 400;
				goto leave;
			}
			m_Log.Add("��������deviceId��%s",tmpInfo.m_deviceId);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�deviceId�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "devicePort", MAX_LEN);
		if (node != NULL)
		{
			tmpInfo.m_deviceChn=atoi(node->data);
			m_Log.Add("��������devicePort��%d",tmpInfo.m_deviceChn);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�devicePort�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpInfo.m_dstIp, node->data);
			m_Log.Add("��������serverip��%s",tmpInfo.m_dstIp);
		}

		chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
		if (chNode == NULL)
		{
			m_Log.Add("xml��������δ���ҵ�ch�ڵ�!");
			rt = 400;
			goto leave;
		}

		m_Log.Add("������ͨ��: ����Ϊ:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,devicePort:%d��serverip:%s\n",
			m_devParams.m_proxyIp,
			m_devParams.m_proxyPort,
			m_devParams.m_userName,
			m_devParams.m_passWord,
			tmpInfo.m_deviceId,
			tmpInfo.m_deviceChn,
			tmpInfo.m_dstIp);//����������

		TRACE("������ͨ��: ����Ϊ:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,devicePort:%d��serverip:%s\n",
			m_devParams.m_proxyIp,
			m_devParams.m_proxyPort,
			m_devParams.m_userName,
			m_devParams.m_passWord,
			tmpInfo.m_deviceId,
			tmpInfo.m_deviceChn,
			tmpInfo.m_dstIp);	

	}

#if 0
	subroot = f_xmlnode_sch_elem(root, "h3c");
	if(subroot != NULL)
	{//����h3c

		m_type = DEF_H3C_DEV;

		memset(&tmpH3c,0,sizeof(DEV_H3C_PARAMS));

		m_Log.Add("��ʼh3c xml����!");
		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			sprintf(tmpH3c.m_proxyIp,"%s",node->data);
			m_Log.Add("��������proxyIP��%s",tmpH3c.m_proxyIp);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�proxyIP�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			tmpH3c.m_proxyPort=atoi(node->data);
			m_Log.Add("��������proxyPort��%d",tmpH3c.m_proxyPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�proxyPort�ڵ�!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "user", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_userName,node->data);
			m_Log.Add("��������userName��%s",tmpH3c.m_userName);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�userName�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_passWord,node->data);
			m_Log.Add("��������passWord��%d",tmpH3c.m_passWord);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�passWord�ڵ�!");
			rt = 400;
			goto leave;
		}		
		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_deviceId,node->data);
			m_Log.Add("��������deviceId��%d",tmpH3c.m_deviceId);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�deviceId�ڵ�!");
			rt = 400;
			goto leave;
		}


		node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpH3c.m_dstIp, node->data);
			m_Log.Add("��������serverip��%s",tmpH3c.m_dstIp);
		}

		chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
		if (chNode == NULL)
		{
			m_Log.Add("xml��������δ���ҵ�ch�ڵ�!");
			rt = 400;
			goto leave;
		}

		m_Log.Add("������ͨ��: ����Ϊ:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,serverip:%s\n",
			tmpH3c.m_proxyIp,
			tmpH3c.m_proxyPort,
			tmpH3c.m_userName,
			tmpH3c.m_passWord,
			tmpH3c.m_deviceId,
			tmpInfo.m_dstIp);//����������

		TRACE("������ͨ��: ����Ϊ:proxyIP:%s,proxyPORT:%d,userName:%s,passWord:%s,deviceId:%s,serverip:%s\n",
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
		m_Log.Add("��������chΪ��ֵ!");	
		if(m_type == DEF_NTQY_DEV)
			m_Log.Add("������Ƶ��%s:%d �Ƿ���ڣ�",tmpInfo.m_deviceId,tmpInfo.m_deviceChn);
		if(m_type == DEF_H3C_DEV)
			m_Log.Add("������Ƶ��%s:%s �Ƿ���ڣ�",tmpH3c.m_proxyIp,tmpH3c.m_deviceId);

		WaitForSingleObject(Main->m_Lock,INFINITE);
		if(m_type == DEF_NTQY_DEV)
			tmpStr.Format("%s:%d",tmpInfo.m_deviceId,tmpInfo.m_deviceChn);
		if(m_type == DEF_H3C_DEV)
			tmpStr.Format("%s:%s",tmpH3c.m_proxyIp,tmpH3c.m_deviceId);

	//	resultStr=	Main->ChangeCh(tmpStr,-1,"");
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
		int m_ch=0;
		//m_Log.Add("�л�%s,��ͨ��%d,������%s",tmpStr,atoi(chNode->data),serverStr);
#if 0
		if("" == tmpInfo.m_dstIp)
		{
			m_Log.Add("�л���Ƶ%s:%dʧ�ܣ�δ�ҵ�serverip!",tmpInfo.m_deviceId,tmpInfo.m_deviceChn);
			rt = 400;
			goto leave;
		}
#endif

		if(atoi(chNode->data)>=g_maxLiveNum)
		{
			//rt = 503;
			//goto leaveOut;
			//char tmp[4]="";
			m_ch = atoi(chNode->data);
			m_ch = m_ch%g_maxLiveNum;

			//m_devDfhsParams.m_devInfo[m_ch] = tmpInfo;
			//TRACE("INFO:***ԭͨ��%d:%d\n",)


		}else
		{
			//m_devDfhsParams.m_devInfo[atoi(chNode->data)] = tmpInfo;
			m_ch = atoi(chNode->data);
		}

		if(m_type == DEF_NTQY_DEV)
		{
		tmpInfo.m_handle = m_devParams.m_devInfo[m_ch].m_handle;
		m_devParams.m_devInfo[m_ch] = tmpInfo;
		}
		//if(m_type == DEF_H3C_DEV)
		//{
		//	memcpy(&tmpH3c.m_loginInfo,&m_h3cParams[m_ch].m_loginInfo,sizeof(LOGIN_INFO_S));
		//	strcpy(tmpH3c.m_szXpCode,m_h3cParams[m_ch].m_szXpCode);
		//	m_h3cParams[m_ch] = tmpH3c;
		//}



		WaitForSingleObject(Main->m_Lock,INFINITE);
		if(m_type == DEF_NTQY_DEV){
			//resultStr=Main->ChangeCh(tmpStr,m_ch,tmpInfo.m_dstIp);
			resultStr=ChangeChannel(tmpStr,m_ch,tmpInfo.m_dstIp);
		}
		//if(m_type == DEF_H3C_DEV)
		//	resultStr=Main->ChangeCh(tmpStr,m_ch,tmpH3c.m_dstIp);
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
	return 0;
}

CString CNtqyDlg::CheckExit(CString str)
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

		//	UrlStr.Format("200rtmp://%s:%d/live/%s_v%d",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,DEF_CS_NTQY_FLAG,i);
			UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);

			m_Log.Add("��Ƶ:%s ����!URL:%s",CamID,UrlStr);


			m_h264[i]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š�

			return UrlStr;
		}


	}
	m_Log.Add("��Ƶ:%s ������!",CamID);

	return UrlStr;
}


CString CNtqyDlg::ChangeChannel(CString sId,int CH,CString str)
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
	if(m_type == DEF_NTQY_DEV)
		m_h264[CH]->m_Info.s_DevID.Format("%s:%d",m_devParams.m_devInfo[CH].m_deviceId,m_devParams.m_devInfo[CH].m_deviceChn);//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1
	//if(m_type == DEF_H3C_DEV)
	//	m_h264[CH]->m_Info.s_DevID.Format("%s:%s",m_h3cParams[CH].m_proxyIp,m_h3cParams[CH].m_deviceId);//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1

	m_Log.Add("��ͨ����׼���л�%s,��ͨ��%d,������%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	if (NULL!=m_h264[CH]->h_stream)
	{//��·�Ѿ���ʹ�ã���Ҫ�ȹر��ٽ��в���
		//m_netVideo[CH].CloseVideo();

		ClosePull(m_h264[CH],&m_devParams);

		//m_h264[CH]->h_stream=NULL;

	}

	m_h264[CH]->m_devType = m_type;//�رպ�ſ��Ը������е��豸����

	m_h264[CH]->m_Info.s_PubServer=str;

	//m_h264[CH]->Init(&m_devDfhsParams,this->GetSafeHwnd());
	//m_h264[CH]->StartGet(m_devDfhsParams.m_devicePort);
	if(m_type == DEF_NTQY_DEV)
	{
	SetParams(m_h264[CH],&m_devParams);
	//StartPull(m_h264[CH],&m_devParams);
	}
	
	//if(m_type == DEF_H3C_DEV)
	//{
	//	SetParams(m_h264[CH],&m_h3cParams[CH]);
	//StartPull(m_h264[CH],&m_h3cParams[CH]);

	//}
	//m_h264[CH]->pNetVideo->OpenVideo(m_devDfhsParams.m_devicePort);

	//m_h264[CH]->h_stream = &m_devParams.m_devInfo[CH].m_deviceChn;

	{
		CString UrlStr="";
	//	UrlStr.Format("200rtmp://%s:%d/live/%s_v%d",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,DEF_CS_NTQY_FLAG,CH);
		UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_devParams.m_devInfo[0].m_dstPub);
	
		
		m_Log.Add("��ͨ������Ƶ%s �л��ɹ�%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
		m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����

		return UrlStr;
	}


	return UrlStr;


}

int CNtqyDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;


	DEV_NTQY_PARAMS *pDfhsParams = &m_devParams;
	int ret=0;


	//m_Info.s_DevID=pParams->m_deviceId+_T(":");
	pH264Stream->m_Info.s_DevID.Format("%s:%d",pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn);//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1


	if (false == pH264Stream->b_start)
	{
		pH264Stream->first=true;
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		//��StartPush�����ÿ�Ⱥ͸߶ȵ���Ϣ�������߳̽���pushstream����
		//��pushstream()ʵ�ְѰѴ�ģ���ȡ����Ƶ���ݱ��룻���浽ָ���ṹ��pMain->g_rtmp
		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);

	return 0;
}

static VOID STDCALL myCallH264_1(IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo,
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

//	CH264Stream* pMain=(CH264Stream*)pNtqyDlg->m_h264[lUserParam];
	CH264Stream* pMain=m_h264[lUserParam];

	pMain->b_PullLive = 0;

	if(pMain == NULL)
		return;
		
	if(pMain->p_EncodeArray.GetSize()<200)
	{
		if (NULL ==pstParseVideoData->pucData)
		{
			m_Log.Add("��Ƶ����Ϊ�գ�");

			return ;
		}
		if (0 == pstParseVideoData->ulDataLen )
		{
			m_Log.Add("��Ƶ����Ϊ�գ�");

			return ;
		}

		MyAvFrame *m_Frame=NULL;
		//if(lFrameLen<(352*288*3/2+1))//�����޷��ж����С�����������䵱ǰ��С��Ϊ�ô���������С
		//	m_Frame = new MyAvFrame(352,288);//Ĭ�����ô�С CIF��׼
		//else
		m_Frame = new MyAvFrame(pstParseVideoData->ulDataLen+1,1);//���޷��жϳ���Ƶ��С


		if (NULL == m_Frame)
		{
			m_Log.Add("new MyAvFrame��ʱ���ڴ�������");

			return ;
		}
		//add  ����
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
			m_Log.Add("���䵽��MyAvFrame���p_dataָ��Ϊ�գ�");

			return ;
		}
		memcpy(m_Frame->p_data+1,pstParseVideoData->pucData,pstParseVideoData->ulDataLen);//new MyAvFrame(352,288)<--byteVideoData

		pMain->m_streamlock.Lock();
		pMain->p_EncodeArray.Add(m_Frame);//��Ƶ���ݴ�����������ݻ�����CArray<MyAvFrame*,MyAvFrame*>
		pMain->m_streamlock.Unlock();
	}


	return;
}

// ������������

int CNtqyDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	if(pH264Stream->m_devType == DEF_NTQY_DEV)
	{
		DEV_NTQY_PARAMS *pDfhsParams = &m_devParams;

		if(NULL == m_devParams.m_devInfo[pH264Stream->thisIndex].m_handle)
		{
			//		PlayerLogin("admin","admin","58.221.253.220","555",m_devParams.m_devInfo[pH264Stream->thisIndex].m_handle);
			//3
			char TranPort[32]="";
			sprintf(TranPort,"%d",m_devParams.m_proxyPort);
			PlayerLogin(m_devParams.m_userName,m_devParams.m_passWord,m_devParams.m_proxyIp,TranPort,m_devParams.m_devInfo[pH264Stream->thisIndex].m_handle);
			//3	
			VDataBack func=&CNtqyDlg::MyCallBackFun;
			MsgBack mfunc=&CNtqyDlg::MsgBackFunc;
			SetPlayerFunc(m_devParams.m_devInfo[pH264Stream->thisIndex].m_handle,func,mfunc);
		}

		int status=FALSE;
		/**/
		if (NULL!=pH264Stream->h_stream)
		{
			StopPlayer(m_devParams.m_devInfo[pH264Stream->thisIndex].m_handle);

			pH264Stream->h_stream=NULL;
		}

		//18921466837   18994290374
		//int startplay_ret=StartPlayer("18921466837.ipc",1,pH264Stream->h_stream);

		//1022

		int startplay_ret=StartPlayer(m_devParams.m_devInfo[pH264Stream->thisIndex].m_deviceId,m_devParams.m_devInfo[pH264Stream->thisIndex].m_deviceChn,m_devParams.m_devInfo[pH264Stream->thisIndex].m_handle);
		if(0 != startplay_ret)
		{
			m_Log.Add("[CDfhsDlg::StartPull]:[fail when operate  %s]:devId:%s,devChn:%d,fail to start play ,startplay_ret=%d\n",
				pH264Stream->m_Info.s_PubName,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
				startplay_ret);
			TRACE("[CDfhsDlg::StartPull]:[fail when operate  %s]:devId:%s,devChn:%d,fail to start play ,startplay_ret=%d\n",
				pH264Stream->m_Info.s_PubName,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
				pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn,
				startplay_ret);
		}

		/**/
		pH264Stream->h_stream = &pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn;
		TRACE("[CDfhsDlg::StartPull]:[success operate %s]:devId:%s,devChn:%d\n",
			pH264Stream->m_Info.s_PubName,
			pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceId,
			pDfhsParams->m_devInfo[pH264Stream->thisIndex].m_deviceChn);
	}

#if 1
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
			//DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, "", "�������ʧ��");
			return -1;
		}


		//�û���½
		ulRet = IMOS_LoginEx(pH3cParams->m_userName, szPasswd, pH3cParams->m_proxyIp, "N/A", &pH3cParams->m_loginInfo);//IMOS_Login(szUserLoginName, szPasswd, szServerAdd, &g_stLoginInfo);
		if (ERR_COMMON_SUCCEED != ulRet)
		{
			//log
			m_Log.Add("�û���½:�û���%s:����%s", pH3cParams->m_userName,szPasswd);
			return -1;
		}

		/* �����ڴ��ȡ������� */
		PLAY_WND_INFO_S *pstPlayWndInfoList = NULL;
		pstPlayWndInfoList = (PLAY_WND_INFO_S*)malloc(DEMO_MAX_PLAYS * sizeof(PLAY_WND_INFO_S));
		if (NULL == pstPlayWndInfoList)
		{
			//		DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "�����ڴ�ʧ��");
			return -1;
		}
		memset(pstPlayWndInfoList, 0, DEMO_MAX_PLAYS * sizeof(PLAY_WND_INFO_S));

		/* �����������������ô������ */
		ulRet = IMOS_StartPlayer(&pH3cParams->m_loginInfo.stUserLoginIDInfo, DEMO_MAX_PLAYS, pstPlayWndInfoList);
		if (ERR_COMMON_SUCCEED != ulRet)
		{
			//		DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "�������������ʧ��");
			return -1;
		}
		free(pstPlayWndInfoList);

		//char szXpCode[IMOS_RES_CODE_LEN] = {0};
		ulRet = IMOS_GetChannelCode(&pH3cParams->m_loginInfo.stUserLoginIDInfo, pH3cParams->m_szXpCode);


		ulRet = IMOS_SetParseVideoDataCB  ( &pH3cParams->m_loginInfo.stUserLoginIDInfo,  
			pH3cParams->m_szXpCode,  
			myCallH264_1,  
			1,  
			pH264Stream->thisIndex);


		/* ����SDK�ӿڿ�ʼʵʱ��أ�������Ϊ0 */
		ulRet = IMOS_StartMonitor(&pH3cParams->m_loginInfo.stUserLoginIDInfo,
			pH3cParams->m_deviceId,
			pH3cParams->m_szXpCode,
			IMOS_FAVORITE_STREAM_ANY,
			USER_OPERATE_SERVICE);
		if (ERR_COMMON_SUCCEED != ulRet)
		{
			//log
			//DEMO_LOG_2(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "ʵʱ���", 
			//	"�����", pCurNode->m_nodeInfo.szResCode,
			//	"����", pCurPlay->m_szXpCode);
			return -1;
		}

		pH264Stream->h_stream = &pH3cParams->m_szXpCode;

	}
#endif
	return 0;
}

int CNtqyDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	if(pH264Stream->m_devType == DEF_NTQY_DEV)
	{
		DEV_NTQY_PARAMS *pDfhsParams = (DEV_NTQY_PARAMS *)pParams;
		//pDfhsParams�п���Ϊ��

		int status=FALSE;

		if (NULL!=pH264Stream->h_stream)
		{

			StopPlayer(m_devParams.m_devInfo[pH264Stream->thisIndex].m_handle);

			pH264Stream->h_stream=NULL;
			TRACE("[CDfhsDlg::ClosePull]:[successcloseoldplay when operate %s]:devId:%s\n",
				pH264Stream->m_Info.s_PubName,
				pH264Stream->m_Info.s_DevID);
		}

	}

	//if(pH264Stream->m_devType == DEF_H3C_DEV)
	//{

	//	DEV_H3C_PARAMS *pH3cParams = &pNtqyDlg->m_h3cParams[pH264Stream->thisIndex];
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
