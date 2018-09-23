// HKDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "HKDlg.h"

#include "hkLib.h"

extern CWMV_TO_H264Dlg* Main;
static CHKDlg* pHkDlg=NULL;
// CHKDlg �Ի���

IMPLEMENT_DYNAMIC(CHKDlg, CDialog)

CHKDlg::CHKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHKDlg::IDD, pParent)
{

	pHkDlg = this;

	if(0 != loadHKLib(NULL))
	{
		m_Log.Add("[ERR]:loadHKLib error");
		exit(0);
	}

  //---------------------------------------
  // ��ʼ��
  NET_DVR_Init();

  //��������ʱ��������ʱ��
  NET_DVR_SetConnectTime(2000, 1);
  NET_DVR_SetReconnect(10000, true);


}

CHKDlg::~CHKDlg()
{

	NET_DVR_Cleanup();
}

void CHKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHKDlg, CDialog)
END_MESSAGE_MAP()


//#define Main->m_secName	"hk"
int CHKDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
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
	m_devParams[nIndex].m_deviceChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

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
	m_h264[nIndex]->m_devType = DEF_HK_DEV;

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

int CHKDlg::GetInforFromIni(void * iniPath)
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
		m_devParams[SourceCount].m_deviceChn=GetPrivateProfileInt(Main->m_secName,tmpStr,-1,thisDir);

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
				//m_h264[SourceCount]->b_Decode = 1;

				m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[SourceCount].m_devInfo.m_dstIp;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
				m_h264[SourceCount]->m_devType = DEF_HK_DEV;

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
int CHKDlg::SetDefaultParams(void * pParams)
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
		m_h264[SourceCount]->m_devType = DEF_HK_DEV;

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

int CHKDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_HK_PARAMS tmpHK;

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
	if(node == NULL || atoi(node->data)!=3)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���hk == 4!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "hk");
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
			m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
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
			m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.nPort=atoi(node->data);
			tmpHK.m_deviceChn = atoi(node->data);
			
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
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

CString CHKDlg::CheckExit(CString str)
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


CString CHKDlg::ChangeChannel(CString sId,int CH,CString str)
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
void __stdcall HKDecCallBack3(long nPort, char*pBuf, long size, FRAME_INFO* pFrameInfo, long nUser, long reserved2)
{

	//TRACE(" W:%d H:%d stamp:%d type:%d %d %d\n",pFrameInfo->nWidth,pFrameInfo->nHeight,pFrameInfo->nStamp,pFrameInfo->nType,pFrameInfo->nFrameRate,pFrameInfo->dwFrameNum);



	CH264Stream* pH264=(CH264Stream*)m_h264[nUser];
	if(pH264 == NULL)
		return;

	pH264->b_PullLive = 0;

	if(pFrameInfo->nType != T_YV12)
		return;
	//return ;
	
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
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");

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
	
	


	return;

}


// CHKDlg ��Ϣ�������
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dUser)
{
    HWND hWnd = NULL;//GetConsoleWindow();
	DWORD dwUser = *(DWORD *)dUser;

	CH264Stream* pH264=(CH264Stream*)m_h264[dwUser];
	if(pH264 == NULL)
		return;

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //ϵͳͷ
		if(-1 != pHkDlg->m_devParams[dwUser].m_playChn)
		{
			PlayM4_Stop(pHkDlg->m_devParams[dwUser].m_playChn);
			PlayM4_CloseStream(pHkDlg->m_devParams[dwUser].m_playChn);
			PlayM4_FreePort(pHkDlg->m_devParams[dwUser].m_playChn);

			pHkDlg->m_devParams[dwUser].m_playChn = -1;
		}

		//if (!PlayM4_GetPort(&lPort))  //��ȡ���ſ�δʹ�õ�ͨ����
		if (!PlayM4_GetPort(&pHkDlg->m_devParams[dwUser].m_playChn))  //��ȡ���ſ�δʹ�õ�ͨ����
		{
			break;
		}
		//m_iPort = lPort; //��һ�λص�����ϵͳͷ������ȡ�Ĳ��ſ�port�Ÿ�ֵ��ȫ��port���´λص�����ʱ��ʹ�ô�port�Ų���
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(pHkDlg->m_devParams[dwUser].m_playChn, STREAME_REALTIME))  //����ʵʱ������ģʽ
			{
				break;
			}

			if (!PlayM4_OpenStream(pHkDlg->m_devParams[dwUser].m_playChn, pBuffer, dwBufSize, 1024*1024)) //�����ӿ�
			{
				break;
			}

			if (!PlayM4_SetDecCallBackMend(pHkDlg->m_devParams[dwUser].m_playChn,HKDecCallBack3,dwUser))
			{
				break;
			}

			if (!PlayM4_Play(pHkDlg->m_devParams[dwUser].m_playChn, hWnd)) //���ſ�ʼ
			{
				break;
			}
		}
	case NET_DVR_STREAMDATA:   //��������
		if (dwBufSize > 0 && pHkDlg->m_devParams[dwUser].m_playChn != -1)
		{
			if (!PlayM4_InputData(pHkDlg->m_devParams[dwUser].m_playChn, pBuffer, dwBufSize))
			{
				break;
			} 
		}
	}
}

int CHKDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_HK_PARAMS *pHKParams = (DEV_HK_PARAMS *)pParams;
	int ret;

	pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%d",pHKParams->m_proxyIp,pHKParams->m_proxyPort,
		pHKParams->m_userName,pHKParams->m_passWord,pHKParams->m_deviceChn);

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


void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch(dwType) 
    {
    case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
    TRACE("----------reconnect--------%d\n", time(NULL));
    break;
	default:
    break;
    }

	TRACE("----------into exception func--------%d\n", time(NULL));
}

int CHKDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_HK_PARAMS *pHKParams = &m_devParams[pH264Stream->thisIndex];//(DEV_HK_PARAMS *)pParams;

	// ע���豸
	//LONG lUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	//pHKParams->m_devInfo.m_userId = NET_DVR_Login_V30("172.0.0.100", 8000, "admin", "12345", &struDeviceInfo);
	pHKParams->m_devInfo.m_userId = NET_DVR_Login_V30(pHKParams->m_proxyIp, pHKParams->m_proxyPort, 
		pHKParams->m_userName, pHKParams->m_passWord, &struDeviceInfo);
	if (pHKParams->m_devInfo.m_userId < 0)
	{
	//	TRACE("Login error[%s]:[%d], ERR:%d\n", pHKParams->m_proxyIp, pHKParams->m_proxyPort,NET_DVR_GetLastError());
		TRACE("Login error[%s]:[%d], ERR:%08x\n", 
			pHKParams->m_proxyIp, pHKParams->m_proxyPort,GD_GetInforFromHK(NET_DVR_GetLastError()));
		//NET_DVR_Cleanup();
		return -1;
	}

	//---------------------------------------
  //�����쳣��Ϣ�ص�����
  //NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	//---------------------------------------
	//����Ԥ�������ûص�������
	//LONG lRealPlayHandle;
	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = NULL;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	ClientInfo.lChannel     = pHKParams->m_deviceChn;//1;       //Ԥ��ͨ����
	ClientInfo.lLinkMode    = 0;       //���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
	ClientInfo.sMultiCastIP = NULL;   //�ಥ��ַ����Ҫ�ಥԤ��ʱ����

	NET_DVR_PREVIEWINFO struPlayInfo = {0};
	struPlayInfo.hPlayWnd = NULL;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	struPlayInfo.lChannel     = pHKParams->m_deviceChn;//33;       //Ԥ��ͨ����
	struPlayInfo.dwStreamType = 0;       //0-��������1-��������2-����3��3-����4���Դ�����
	struPlayInfo.dwLinkMode   = 0;       //0- TCP��ʽ��1- UDP��ʽ��2- �ಥ��ʽ��3- RTP��ʽ��4-RTP/RTSP��5-RSTP/HTTP



	BOOL bPreviewBlock = false;       //�������������Ƿ�������0����1����
	//pHKParams->m_devInfo.m_realPlayHandle = NET_DVR_RealPlay_V30(pHKParams->m_devInfo.m_userId, &ClientInfo, NULL, NULL, 0);
	pHKParams->m_devInfo.m_realPlayHandle = NET_DVR_RealPlay_V40(pHKParams->m_devInfo.m_userId, &struPlayInfo, (REALDATACALLBACK)g_RealDataCallBack_V30, &pH264Stream->thisIndex);

	if (pHKParams->m_devInfo.m_realPlayHandle < 0)
	{
		TRACE("NET_DVR_RealPlay_V30 error[%s]:[%d]->[%d] ERR:%d\n",pHKParams->m_proxyIp, pHKParams->m_proxyPort,
			pHKParams->m_deviceChn,NET_DVR_GetLastError());
		NET_DVR_Logout_V30(pHKParams->m_devInfo.m_userId);
		//NET_DVR_Cleanup();
		return -1;
	}
#if 0
	if (!NET_DVR_SetRealDataCallBack(pHKParams->m_devInfo.m_realPlayHandle, g_RealDataCallBack_V30, pH264Stream->thisIndex))
	{
		TRACE("NET_DVR_SetRealDataCallBack error\n");
	}
#endif
	pH264Stream->h_stream = &pHKParams->m_deviceChn;
	return 0;
}

int CHKDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_HK_PARAMS *pHKParams = &m_devParams[pH264Stream->thisIndex];
	//---------------------------------------
	//�ر�Ԥ��
	NET_DVR_StopRealPlay(pHKParams->m_devInfo.m_realPlayHandle);

	PlayM4_Stop(pHKParams->m_playChn);
	PlayM4_CloseStream(pHKParams->m_playChn);
	PlayM4_FreePort(pHKParams->m_playChn);

	//ע���û�
	NET_DVR_Logout_V30(pHKParams->m_devInfo.m_userId);
	//NET_DVR_Cleanup();
	pHKParams->m_devInfo.m_userId = -1;
	pHKParams->m_playChn = -1;

	pH264Stream->h_stream = NULL;

	return 0;
}
