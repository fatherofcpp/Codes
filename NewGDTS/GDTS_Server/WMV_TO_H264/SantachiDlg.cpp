// SantachiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "SantachiDlg.h"

#include "santachiLib.h"

extern CWMV_TO_H264Dlg* Main;
static CSantachiDlg* pSTDlg=NULL;

// CSantachiDlg �Ի���

IMPLEMENT_DYNAMIC(CSantachiDlg, CDialog)

CSantachiDlg::CSantachiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSantachiDlg::IDD, pParent)
{
	pSTDlg = this;

	if(0 != loadSantachiLib(NULL))
	{
		m_Log.Add("[ERR]:loadSantachiLib error");
		exit(0);
	}

  //---------------------------------------
  //��ʼ�������
	st_net_initClientNetLib();
  //��ʼ�����ſ�
	st_player_initPlayLib();


}

CSantachiDlg::~CSantachiDlg()
{
}

void CSantachiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSantachiDlg, CDialog)
END_MESSAGE_MAP()


// CSantachiDlg ��Ϣ�������


//#define Main->m_secName	"hk"
int CSantachiDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("ProxyIp%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_proxyIp,dst);


	tmpStr.Format("ProxyPort%d",nIndex);
	ZeroMemory(dst,255);
	m_devParams[nIndex].m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_devParams.m_proxyPort:%d\n",m_devParams[nIndex].m_proxyPort);


	tmpStr.Format("UserName%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_userName,dst);
	TRACE("m_devParams[nIndex].m_userName:%s\n",m_devParams[nIndex].m_userName);


	tmpStr.Format("PassWd%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_passWord,dst);
	TRACE("m_devParams[nIndex].m_passWord:%s\n",m_devParams[nIndex].m_passWord);

	//InitProxyServer(NULL);

	m_Log.Add("��ʼ��ͨ��������: ����Ϊ:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		m_devParams[nIndex].m_proxyIp,m_devParams[nIndex].m_proxyPort,m_devParams[nIndex].m_userName,
		m_devParams[nIndex].m_passWord);
#if 0
	tmpStr.Format("PlayChn0%d",nIndex);
	ZeroMemory(dst,255);
	m_devParams[nIndex].m_playChn=GetPrivateProfileInt(common,tmpStr,-1,thisDir);
#endif		
	tmpStr.Format("DeviceChn%d",nIndex);
	ZeroMemory(dst,255);

	//GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	//strcpy(m_devParams[nIndex].m_deviceChn,dst);
	m_devParams[nIndex].m_deviceChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir)-1;

	tmpStr.Format("Dstip%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_devInfo.m_dstIp,dst);

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_devInfo.m_dstPub,dst);

	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_devParams[nIndex].m_devInfo.m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV�����ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[nIndex]->thisIndex = nIndex;
	m_h264[nIndex]->b_Encode	= 1;//(g_bUseTc==1?1:0);
	//m_h264[nIndex]->b_Decode = 1;

	m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_devInfo.m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_devInfo.m_dstPub,nIndex);
	//m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,nIndex);

	//m_h264[nIndex]->pNetVideo = &m_netVideo[nIndex];//��ʱʹ��
	m_h264[nIndex]->m_devType = DEF_SANTACHI_DEV;

	SetParams(m_h264[nIndex],&m_devParams[nIndex]);
	//StartPull(m_h264[nIndex],&m_devParams[nIndex]);

	m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceName%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		nIndex,
		nIndex,m_devParams[nIndex].m_deviceChn,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstIp,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstPub,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstStream);
	//TRACE("[GetInforFromIni]:nIndex=%d\n",nIndex);

	return 0;
}

int CSantachiDlg::GetInforFromIni(void * iniPath)
{
	char dst[255];
	CString tmpStr;

	CString thisDir;
	thisDir.Format("%s",(char*)iniPath);

	SourceCount=0;


	while(1)//��ȡconfig.ini������ntqy,��Ӧnew CH264Stream
	{
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("�ﵽ���ת��·������������ֹͣ������%d��%d",SourceCount,g_maxLiveNum);
			break;
		}

		tmpStr.Format("ProxyIp%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(Main->m_secName,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_proxyIp,dst);


		tmpStr.Format("ProxyPort%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_proxyPort=GetPrivateProfileInt(Main->m_secName,tmpStr,-1,thisDir);
		TRACE("m_devParams.m_proxyPort:%d\n",m_devParams[SourceCount].m_proxyPort);


		tmpStr.Format("UserName%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(Main->m_secName,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_userName,dst);
		TRACE("m_devParams[SourceCount].m_userName:%s\n",m_devParams[SourceCount].m_userName);


		tmpStr.Format("PassWd%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(Main->m_secName,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_passWord,dst);
		TRACE("m_devParams[SourceCount].m_passWord:%s\n",m_devParams[SourceCount].m_passWord);

		//InitProxyServer(NULL);

		m_Log.Add("��ʼ��ͨ��������: ����Ϊ:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
			m_devParams[SourceCount].m_proxyIp,m_devParams[SourceCount].m_proxyPort,m_devParams[SourceCount].m_userName,
			m_devParams[SourceCount].m_passWord);
#if 0
		tmpStr.Format("PlayChn0%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_playChn=GetPrivateProfileInt(Main->m_secName,tmpStr,-1,thisDir);
#endif		
		tmpStr.Format("DeviceChn%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_deviceChn=GetPrivateProfileInt(Main->m_secName,tmpStr,-1,thisDir)-1;

		tmpStr.Format("Dstip%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(Main->m_secName,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_devInfo.m_dstIp,dst);

		tmpStr.Format("Dstpub%d",SourceCount);
		ZeroMemory(dst,255);
		GetPrivateProfileString(Main->m_secName,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_devInfo.m_dstPub,dst);

		tmpStr.Format("Dststream%d",SourceCount);
		ZeroMemory(dst,255);
		m_devParams[SourceCount].m_devInfo.m_dstStream=GetPrivateProfileInt(Main->m_secName,tmpStr,-1,thisDir);

		if ( -2 == m_devParams[SourceCount].m_deviceChn || ""==m_devParams[SourceCount].m_devInfo.m_dstIp || ""==m_devParams[SourceCount].m_devInfo.m_dstPub || -1==m_devParams[SourceCount].m_devInfo.m_dstStream)
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
				//m_h264[SourceCount]->b_Decode = 1;

				m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[SourceCount].m_devInfo.m_dstIp;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
				m_h264[SourceCount]->m_devType = DEF_SANTACHI_DEV;

				SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
				//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);
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

//����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
//�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
int CSantachiDlg::SetDefaultParams(void * pParams)
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
		m_h264[SourceCount]->b_Encode	= 1;

		m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[0].m_devInfo.m_dstIp;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[0].m_devInfo.m_dstPub,SourceCount);

		//m_h264[SourceCount]->Init("http://192.168.0.251:8080/live1",ServerIP,PubName,SourceCount,this->GetSafeHwnd());
		//m_h264[SourceCount]->StartGet();

		//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
		//m_h264[SourceCount]->h_stream = m_h264[SourceCount]->StreamParams.hWnd;
		m_h264[SourceCount]->m_devType = DEF_SANTACHI_DEV;

	}
	return 0;
}

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

#if 0

<company>12</company>

<santachi>
<type></type>//�豸����
<ver></ver>//�汾
<ip></ip>//��ַ
<port></port>//�˿�
<user></user>//�û���
<passwd></passwd>//����
<channel></channel>//Ĭ�ϲ���ͨ��
<pipe></pipe>//Ĭ���豸ͨ��
</santachi>

#endif

int CSantachiDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_SANTACHI_PARAMS tmpHK;

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
		strcpy(tmpHK.m_devInfo.m_dstIp,node->data); 
		//m_Log.Add("��������serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=12)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���hk == 4!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "santachi");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�santachi�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����HK��XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			//tmpType = node->data;
			//m_Log.Add("��������type��%s",tmpType);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�type�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ver", MAX_LEN);
		if (node != NULL)
		{
			//tmpVer=node->data;
			//m_Log.Add("��������ver��%s",tmpVer);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�ver�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.ServerIP=node->data;
			strcpy(tmpHK.m_proxyIp,node->data);
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
			//tmpHK.nPort=atoi(node->data);
			tmpHK.m_proxyPort = atoi(node->data);
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
			//tmpHK.SrcUrl = node->data;
			strcpy(tmpHK.m_userName,node->data);
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
			//tmpHK.SrcUrl = node->data;
			strcpy(tmpHK.m_passWord,node->data);
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
			//tmpHK.nPort=atoi(node->data);
			tmpHK.m_deviceChn = atoi(node->data)-1;
			
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�channel�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "pipe", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.nPort=atoi(node->data);
			tmpHK.m_playChn = atoi(node->data);
			
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�pip�ڵ�!");
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
			sprintf(srcCam,"%s:%d:%s:%s:%d",tmpHK.m_proxyIp,tmpHK.m_proxyPort,tmpHK.m_userName,tmpHK.m_passWord,tmpHK.m_deviceChn);
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
#if 1
			if(atoi(CH)>=g_maxLiveNum)
			{
				int ntmp = atoi(CH);
				ntmp = ntmp%g_maxLiveNum;
				CH.Format("%d",ntmp);
			}
#endif


			CString tmpSrcID;
			tmpSrcID.Format("%s:%d:%s:%s:%d",tmpHK.m_proxyIp,tmpHK.m_proxyPort,tmpHK.m_userName,tmpHK.m_passWord,tmpHK.m_deviceChn);
#if 1
			m_h264[atoi(CH)]->m_Info.s_DevID = tmpSrcID;
			m_h264[atoi(CH)]->m_Info.s_PubServer = tmpHK.m_devInfo.m_dstIp;
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",tmpSrcID,atoi(CH));

			{
				strcpy(m_devParams[atoi(CH)].m_proxyIp,tmpHK.m_proxyIp);
				m_devParams[atoi(CH)].m_proxyPort = tmpHK.m_proxyPort;
				strcpy(m_devParams[atoi(CH)].m_userName,tmpHK.m_userName);
				strcpy(m_devParams[atoi(CH)].m_passWord,tmpHK.m_passWord);
				m_devParams[atoi(CH)].m_deviceChn = tmpHK.m_deviceChn;

			}

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(tmpSrcID,atoi(CH),tmpHK.m_devInfo.m_dstIp);
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

CString CSantachiDlg::CheckExit(CString str)
{
	CString UrlStr="",CamID = str;

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//���֮ǰ�Ѿ�����ͣ,��ֱ�ӷ��ص�ǰ���л�����BMS�·��л����
				m_Log.Add("��Ƶ:%s �Ѿ�ֹͣ���ţ�������!",CamID);
				//return "";
				continue;

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


CString CSantachiDlg::ChangeChannel(CString sId,int CH,CString str)
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
	//m_h264[CH]->m_Info.s_DevID = sId;//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1


	//m_Log.Add("����׼���л�%s,��ͨ��%d,������%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	//if (NULL!=m_h264[CH]->h_stream)
	//{//��·�Ѿ���ʹ�ã���Ҫ�ȹر��ٽ��в���
	//	//m_netVideo[CH].CloseVideo();

	//	ClosePull(m_h264[CH],&m_devParams[CH]);

	//	m_h264[CH]->h_stream=NULL;

	//}

	//m_h264[CH]->m_Info.s_PubServer=str;

	//m_h264[CH]->Init(&m_devDfhsParams,this->GetSafeHwnd());
	//m_h264[CH]->StartGet(m_devDfhsParams.m_devicePort);
	SetParams(m_h264[CH],&m_devParams[CH]);
	//StartPull(m_h264[CH],&m_devParams[CH]);
	

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

//************************************
// Method:    alarm_call_back
// FullName:  alarm_call_back
// Access:    public static 
// Returns:   int
// Qualifier: �澯�ص�
// Parameter: msg_alarm_info_t msg_alarm_info
// Parameter: void * context
//************************************
int CALLBACK alarm_call_back(msg_alarm_info_t msg_alarm_info, void *context)
{
	//CH264DemoDlg * pthis = (CH264DemoDlg *)context;
	
	char szTime[128] = {0};
	LV_ITEM lv_item;
	lv_item.mask = LVIF_TEXT;
	
	//IP
	lv_item.iItem = 0;
	lv_item.iSubItem = 0;
	lv_item.pszText = msg_alarm_info.ip;
	//pthis->m_ctrlWarningInfo.InsertItem(&lv_item);
	TRACE("alarm_call_back:%s\n",lv_item.pszText);
	char szIP[16];
	memcpy(szIP, msg_alarm_info.ip, sizeof(szIP));
	
	//ʱ��
	sprintf(szTime,"%d-%d-%d %d:%d:%d",msg_alarm_info.tm.year,msg_alarm_info.tm.month,msg_alarm_info.tm.day,
		msg_alarm_info.tm.hour,msg_alarm_info.tm.minute,msg_alarm_info.tm.second);
	lv_item.iSubItem = 1;
	lv_item.pszText = szTime;
	//pthis->m_ctrlWarningInfo.SetItem(&lv_item);
	TRACE("alarm_call_back:%s\n",lv_item.pszText);
	
	//�澯��Ϣ
	if (msg_alarm_info.alarm_type != ALARM_GPS_INFO)
	{	
		lv_item.iSubItem = 2;
		lv_item.pszText = msg_alarm_info.alarm_info;
		//pthis->m_ctrlWarningInfo.SetItem(&lv_item);
		TRACE("alarm_call_back:%s\n",lv_item.pszText);
	}
	
	return 0;
}

int CALLBACK data_callback(char *pserver_ip, int channel,void *buf, int len, void *context)
{
	unsigned long play_id = (unsigned long)context;
	frame_t *pFrame = (frame_t *)buf;
	if (pFrame->frame_head.frame_type == 6)
	{
		//֡����6ΪMJPEG��, lJpegSize ΪͼƬ��С  lJpegDataΪͼƬ���ݵ�ַ
		unsigned long lJpegSize = pFrame->frame_head.frame_size;
		unsigned char *lJpegData = pFrame->frame_data;

		//������Ա�����Ӧ���ݻ��ߵ���st_player_inputNetFrame�ͽ�����ʾ
		//....
		//		return 0;
	}

	st_player_inputNetFrame(play_id, (char *)buf, len);

	return 0;
}

int CALLBACK DecCallback(long hHandle, char *pData, long lSize, ST_FRAME_INFO *pInfo, DWORD dwUser)
{
	
	//TRACE(".................. %d  lSize = %d  nWidth = %d, nHeight = %d  \n",dwUser,lSize, pInfo->nWidth, pInfo->nHeight);

	CH264Stream* pH264=(CH264Stream*)dwUser;
	if(pH264 == NULL)
		return 0;

	pH264->b_PullLive = 0;
	
	if(pH264->p_YuvArray.GetSize()<60)
	{
		if (NULL == pData)
		{
			return 0;
		}
		if (0 == lSize)
		{

			return 0;
		}
		MyAvFrame *m_Frame=new MyAvFrame(pInfo->nWidth,pInfo->nHeight);
		if (NULL == m_Frame)
		{
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");

			return 0;
		}
		if (NULL == m_Frame->p_data)
		{

			return 0;
		}

		memcpy(m_Frame->p_data,pData,pInfo->nWidth*pInfo->nHeight*3/2);

		pH264->m_streamlock.Lock();
		pH264->p_YuvArray.Add(m_Frame);
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
	}
	return 0;
}

unsigned int _stdcall HeartbeatStatus(LPVOID lpParam)
{
	DEV_SANTACHI_PARAMS * pParams = (DEV_SANTACHI_PARAMS *)lpParam;

	int iret;
	CString strLoad;
	int iDeadCount = 0;
	
	
	while (1)
	{	
		iret = st_net_getHeartbeatStatus(pParams->m_devInfo.m_userId);	 //�����ɹ�
		if (iret == 1)
		{
			//TRACE("############������.�������ڸ�λ num = %d ����������������\n", iDeadCount);
			iDeadCount=0;
		}
		else
		{
			//����3������ʧ�������µ�½
			TRACE("����������������ʼ��ض���������������������\n");
			iDeadCount++;
			if(3 == iDeadCount)
				return 0;
		}
		//if(pParams->m_devInfo.m_realPlayHandle == 0)
		//	return 0;


		Sleep(1000);
	}

	return 0;
}

int CSantachiDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_SANTACHI_PARAMS *pSTParams = (DEV_SANTACHI_PARAMS *)pParams;
	int ret;

	pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%d",pSTParams->m_proxyIp,pSTParams->m_proxyPort,
		pSTParams->m_userName,pSTParams->m_passWord,pSTParams->m_deviceChn);

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

int CSantachiDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_SANTACHI_PARAMS *pSTParams = &m_devParams[pH264Stream->thisIndex];//(DEV_SANTACHI_PARAMS *)pParams;

	unsigned long m_hUserID;

	m_hUserID = st_net_initServerInfo(pSTParams->m_proxyIp, pSTParams->m_proxyPort, 
		pSTParams->m_userName, pSTParams->m_passWord);
	if(m_hUserID <= 0)
	{
		//if (strLoad.LoadString(IDS_H2640_CSHWLKSB))
		m_Log.Add("ERR:st_net_initServerInfo[%s:%d]->[%s:%s]",pSTParams->m_proxyIp, pSTParams->m_proxyPort, 
		pSTParams->m_userName, pSTParams->m_passWord);
		return -1;
	}

	//��½
	int iret = st_net_userLogin(m_hUserID);
	if(iret == ACK_ERROR_NAME)
	{
		st_net_deinitServerInfo(m_hUserID);
		m_hUserID = 0;

		//if (strLoad.LoadString(IDS_H2640_YHMCW))
		m_Log.Add("ERR:st_net_userLogin-ACK_ERROR_NAME");
		return -1;
	}
	else if(iret == ACK_ERROR_PWD)
	{
		st_net_deinitServerInfo(m_hUserID);
		m_hUserID = 0;

		//if (strLoad.LoadString(IDS_H2640_MMCW))
		m_Log.Add("ERR:st_net_userLogin-ACK_ERROR_PWD");
		return -1;
	}
	else if(iret == -1)
	{
		st_net_deinitServerInfo(m_hUserID);
		m_hUserID = 0;

		//if (strLoad.LoadString(IDS_H2640_LJFWQSB))
		m_Log.Add("ERR:st_net_userLogin-(-1)");
		return -1;
	}
	else
	{

	}
	//��ȡ�豸����
	int m_hDevType = st_net_getDeviceType(m_hUserID);
	if (m_hDevType <= 0)
	{
		st_net_userLogout(m_hUserID);
		st_net_deinitServerInfo(m_hUserID);
		m_hUserID = 0;

		//if (strLoad.LoadString(IDS_H2640_HQSBLXSB))
		m_Log.Add("ERR:IDS_H2640_HQSBLXSB");
		return -1;
	}

	switch (m_hDevType)
	{
	case DEV_TYPE_NT200HA:			
	case DEV_TYPE_NT200HB:			
	case DEV_TYPE_NT200HC:			
	case DEV_TYPE_NT200HD:

	case DEV_TYPE_NT6050:	
	case DEV_TYPE_NT8189:	
	case DEV_TYPE_NT4094:
	case DEV_TYPE_NT6050H:	
	case DEV_TYPE_NT8189H:	
	case DEV_TYPE_NT4094H:

	case DEV_TYPE_NT9072H:
	case DEV_TYPE_NT1102H:
	case DEV_TYPE_NT1102H_D:

	case DEV_TYPE_NT6050H_1:
	case DEV_TYPE_NT6050H_2:
		{
			m_hDevType = DEV_TYPE_NT200HD;
		}
		break;
	case DEV_TYPE_NT202HD_1:
	case DEV_TYPE_NT202HD:
	case DEV_TYPE_NT202HA:
		{
			m_hDevType = DEV_TYPE_NT202HD;
		}
		break;
	case DEV_TYPE_NT204HA:		
	case DEV_TYPE_NT204HD_1:
	case DEV_TYPE_NT204HD:
		{
			m_hDevType = DEV_TYPE_NT204HD;
		}
		break;
	case DEV_TYPE_NT3511H:
	case DEV_TYPE_HI3515:
	case DEV_TYPE_AMBAR:
		{	
			sys_config_t sys_config;
			memset(&sys_config,0,sizeof(sys_config_t));
			if (st_net_getParam(m_hUserID,PARAM_3511_SYSINFO,&sys_config,sizeof(sys_config_t)) < 0)
			{
				st_net_userLogout(m_hUserID);
				st_net_deinitServerInfo(m_hUserID);
				m_hUserID = 0;

				//if (strLoad.LoadString(IDS_3511SYS_GET))
				TRACE("ERR:IDS_3511SYS_GET");
				return -1;
			}	
			//Dev3511_iSubtype= sys_config.sub_type;
			//m_ChannNo = sys_config.channel_num;//�ɱ�ͨ��
			TRACE("DEV_TYPE_AMBAR");
		}
		break;
	case DEV_TYPE_HI3520:
		{
			m_hDevType = DEV_TYPE_HI3520;

			nt3520h_sys_config_t sys_config;
			memset(&sys_config,0,sizeof(nt3520h_sys_config_t));
			if (st_net_getParam(m_hUserID,PARAM_3520_SYSINFO,&sys_config,sizeof(nt3520h_sys_config_t)) < 0)
			{
				st_net_userLogout(m_hUserID);
				st_net_deinitServerInfo(m_hUserID);
				m_hUserID = 0;
				//if (strLoad.LoadString(IDS_3511SYS_GET))  //
				TRACE("ERR:IDS_3511SYS_GET");
				return -1;
			}	

			TRACE("DEV_TYPE_HI3520");

		}
		break;
	default:
		m_hDevType = DEV_TYPE_NT204HD;

		break;
	}

	pSTParams->m_devInfo.m_userId = m_hUserID;

	//����
	//m_iMsgThreadFlag = 1;
	//m_hMsgheartBeatThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HeartbeatStatus,(LPVOID)this,0,0);
	pSTParams->m_devInfo.m_hMsgheartBeatThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HeartbeatStatus,(LPVOID)pSTParams,0,0);
	/*
	unsigned int threadid;
	pSTParams->m_devInfo.m_hMsgheartBeatThread=(HANDLE)_beginthreadex(NULL,0,HeartbeatStatus,pSTParams,CREATE_SUSPENDED,&threadid);
	SetThreadPriority(pSTParams->m_devInfo.m_hMsgheartBeatThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(pSTParams->m_devInfo.m_hMsgheartBeatThread);
	*/

	//ע��澯��Ϣ�ص�
	st_net_registerServerAlarmInfoCallback(m_hUserID,alarm_call_back,(void *)pH264Stream);

	int ret = st_net_openChannelStream(m_hUserID, pSTParams->m_deviceChn, PROTOCOL_TCP, 0, 1);
	ret = st_net_startChannelStream(m_hUserID, pSTParams->m_deviceChn, PROTOCOL_TCP, 0);
	long m_hPlayID = st_player_createPlayInstance(st_net_getDeviceType(m_hUserID), 2);
	ret = st_net_registerChannelStreamCallback(m_hUserID,pSTParams->m_deviceChn, data_callback, (void *)m_hPlayID);
	ret = st_player_setPlayWnd(m_hPlayID, NULL);
	ret = st_player_startPlay(m_hPlayID);
	st_player_setCushionRange(m_hPlayID, 0,0,0);
	st_player_registerDecCallback(m_hPlayID, DecCallback, (DWORD)pH264Stream); //YUV�ص�
	if(-1 == ret)
	{
		m_Log.Add("ERR:����YUV�ص�ʧ��");
		return -1;

	}

	pSTParams->m_devInfo.m_realPlayHandle = m_hPlayID;

	pH264Stream->h_stream = &pSTParams->m_deviceChn;
	return 0;
}

int CSantachiDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_SANTACHI_PARAMS *pSTParams = &m_devParams[pH264Stream->thisIndex];
	//---------------------------------------
//TRACE("stop **********1*******\n");
	st_net_registerChannelStreamCallback(pSTParams->m_devInfo.m_userId, pSTParams->m_deviceChn, NULL, NULL);
//TRACE("stop **********2*******\n");
	st_player_stopPlay(pSTParams->m_devInfo.m_realPlayHandle);
//TRACE("stop **********3*******\n");
	st_player_destroyPlayInstance(pSTParams->m_devInfo.m_realPlayHandle);
//TRACE("stop **********4*******\n");
	pSTParams->m_devInfo.m_realPlayHandle = 0;
	st_net_closeChannelStream(pSTParams->m_devInfo.m_userId, pSTParams->m_deviceChn, 0,1);
//TRACE("stop **********5*******\n");
	if(NULL != pSTParams->m_devInfo.m_hMsgheartBeatThread)
	{
		//TRACE("stop **********5.1****id:%p***\n",pSTParams->m_devInfo.m_hMsgheartBeatThread);
		//WaitForSingleObject(pSTParams->m_devInfo.m_hMsgheartBeatThread,INFINITE);
		TerminateThread(pSTParams->m_devInfo.m_hMsgheartBeatThread,0);
		CloseHandle( pSTParams->m_devInfo.m_hMsgheartBeatThread );
		pSTParams->m_devInfo.m_hMsgheartBeatThread = NULL;
	}
	//TRACE("stop **********6****id:%ld***\n",pSTParams->m_devInfo.m_userId);
	st_net_userLogout(pSTParams->m_devInfo.m_userId);
//TRACE("stop **********6.1*******\n");
	st_net_deinitServerInfo(pSTParams->m_devInfo.m_userId);
//TRACE("stop **********7*******\n");
	pH264Stream->h_stream = NULL;

	return 0;
}

