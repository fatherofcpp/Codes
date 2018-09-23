// NetPosaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "NetPosaDlg.h"

extern CWMV_TO_H264Dlg* Main;
static CNetPosaDlg* pNpDlg=NULL;

// CNetPosaDlg �Ի���

IMPLEMENT_DYNAMIC(CNetPosaDlg, CDialog)

CNetPosaDlg::CNetPosaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetPosaDlg::IDD, pParent)
{
		pNpDlg = this;



	if(-1 == loadNpLib())
	{
		m_Log.Add("[ERR]:loadNpLib error");
		exit(0);
	}

	SDKInit();

}

CNetPosaDlg::~CNetPosaDlg()
{

		SDKCleanup();
}

void CNetPosaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetPosaDlg, CDialog)

END_MESSAGE_MAP()


// CNetPosaDlg ��Ϣ�������

void CNetPosaDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

int CNetPosaDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
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
	tmpStr.Format("DeviceName%d",nIndex);
	ZeroMemory(dst,255);

	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[nIndex].m_devName,dst);

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
		m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);
		//m_h264[nIndex]->b_Decode = 1;

		m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_devInfo.m_dstIp;
		m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
		m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_devInfo.m_dstPub,nIndex);
		//m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,nIndex);

		//m_h264[nIndex]->pNetVideo = &m_netVideo[nIndex];//��ʱʹ��
		m_h264[nIndex]->m_devType = DEF_NETPOSA_DEV;

		SetParams(m_h264[nIndex],&m_devParams[nIndex]);
		//StartPull(m_h264[nIndex],&m_devParams[nIndex]);




	m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceName%d:%s,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		nIndex,
		nIndex,m_devParams[nIndex].m_devName,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstIp,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstPub,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstStream);
	//TRACE("[GetInforFromIni]:nIndex=%d\n",nIndex);

	return 0;
}

/*���ݱ��ص������ļ���Ϣ������Ĭ��ת�빤����
 *@date 2014-3-27
 *@author zhh
 *@param void * iniPath	�����ļ�����Ŀ¼.
 *@return int     0���ɹ� ��0��ʧ��
 *
*/
int CNetPosaDlg::GetInforFromIni(void * iniPath)
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
		tmpStr.Format("DeviceName%d",SourceCount);
		ZeroMemory(dst,255);
		
		GetPrivateProfileString(Main->m_secName,tmpStr,"",dst,255,thisDir);
		strcpy(m_devParams[SourceCount].m_devName,dst);

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


		if(0 == strlen(m_devParams[SourceCount].m_devName) || 0 == strlen(m_devParams[SourceCount].m_devInfo.m_dstIp))
			break;

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
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[SourceCount].m_devInfo.m_dstPub,SourceCount);
				//m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
				m_h264[SourceCount]->m_devType = DEF_NETPOSA_DEV;

				SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
				//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);
		}

		

		m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceName%s:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devParams[SourceCount].m_devName,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstIp,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstPub,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstStream);//����������
		TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);
		SourceCount++;

	}


	return 0;
}

/*����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
 *�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
 *@date 2014-3-27
 *@author zhh
 *@param void * pParams	��������.
 *@return int     0���ɹ� ��0��ʧ��
 *
*/
int CNetPosaDlg::SetDefaultParams(void * pParams)
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
		m_h264[SourceCount]->b_Encode	= (g_bUseTc==1?1:0);

		m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[0].m_devInfo.m_dstIp;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[0].m_devInfo.m_dstPub,SourceCount);


		m_h264[SourceCount]->m_devType = DEF_NETPOSA_DEV;

	}
	return 0;
}

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

#if 0

<command>
<serverip>192.168.0.251</serverip>
<company>10</company>

<np>
<ip>192.168.0.1</ip>//��ַ
<port>2100</port>//�˿�
<user>admin</user>//�û���
<passwd>admin</passwd>//����
<channel>av/1/00000000000001</channel>//����ͨ���ı�ʶ

</np>
<ch>0</ch>
</command>

#endif

/*�����緢�͵�XML��ѯ�������л���Ϣ���н�������������������ظ�����ˡ�
 *��������ڣ��򷵻ؿ�
 *@date 2014-3-26
 *@author zhh
 *@param char * pXmlBuf	���յ���XML��Ϣָ��.
 *@param CString *str   ����URL�洢��Ϣ�ռ�,�ɹ����ŵ�URL
 *@return int     200����ʾ�����ɹ�������200
 *				  400����ʾXML���ڷǷ���Ϣ
 *				  404����ʾ��ѯ���������
 *
*/
int CNetPosaDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_NP_PARAMS tmpNP;

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
		strcpy(tmpNP.m_devInfo.m_dstIp,node->data); 
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
	subroot = f_xmlnode_sch_elem(root, "np");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�np�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����NP��XML!");

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpNP.ServerIP=node->data;
			strcpy(tmpNP.m_proxyIp,node->data);
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
			//tmpNP.nPort=atoi(node->data);
			tmpNP.m_proxyPort = atoi(node->data);
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
			//tmpNP.SrcUrl = node->data;
			strcpy(tmpNP.m_userName,node->data);
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
			//tmpNP.SrcUrl = node->data;
			strcpy(tmpNP.m_passWord,node->data);
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
			//tmpNP.nPort=atoi(node->data);
			//tmpNP.m_playChn = atoi(node->data);
			strcpy(tmpNP.m_devName,node->data);
			
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
		if(0 == chNode->data_siz)
		{
			char srcCam[256];
			m_Log.Add("��������chΪ��ֵ!");		
			sprintf(srcCam,"%s:%d:%s:%s:%s",tmpNP.m_proxyIp,tmpNP.m_proxyPort,tmpNP.m_userName,tmpNP.m_passWord,tmpNP.m_devName);
			m_Log.Add("������Ƶ��%s�Ƿ���ڣ�",srcCam);

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=CheckExit(srcCam);//	Main->ChangeCh(srcCam,-1,"");
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
			tmpSrcID.Format("%s:%d:%s:%s:%s",tmpNP.m_proxyIp,tmpNP.m_proxyPort,tmpNP.m_userName,tmpNP.m_passWord,tmpNP.m_devName);
#if 1
			//m_h264[atoi(CH)]->m_Info.s_DevID = tmpSrcID;
			//m_h264[atoi(CH)]->m_Info.s_PubServer = tmpNP.m_devInfo.m_dstIp;
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",tmpSrcID,atoi(CH));

			{
				strcpy(m_devParams[atoi(CH)].m_proxyIp,tmpNP.m_proxyIp);
				m_devParams[atoi(CH)].m_proxyPort = tmpNP.m_proxyPort;
				strcpy(m_devParams[atoi(CH)].m_userName,tmpNP.m_userName);
				strcpy(m_devParams[atoi(CH)].m_passWord,tmpNP.m_passWord);
				strcpy(m_devParams[atoi(CH)].m_devName,tmpNP.m_devName);

			}

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(tmpSrcID,atoi(CH),tmpNP.m_devInfo.m_dstIp);
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

//#endif

leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}
/*���ݴ����Ψһ��ʶ���жϸ�·�Ƿ��ڽ���ת�룬�������ת�뷵�ؿɹ����ŵ�URL��
 *��������ڣ��򷵻ؿ�
 *@date 2014-3-26
 *@author zhh
 *@param CString str	׼���л�����Ƶ��Ψһ��ʶ����������cameraId.
 *@return CString �գ���ʾ�л�ʧ��
 *				  �ǿգ���ʾ�ɹ����ŵ�URL��Ϣ
 *
*/
CString CNetPosaDlg::CheckExit(CString str)
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

/*���ݴ���Ĳ�������ʹ�õ�ͨ���Ž�����Ƶ�л�����,�������л���Ŀɹ����ŵ�URL
 *@date 2014-3-26
 *@author zhh
 *@param CString sId	׼���л�����Ƶ��Ψһ��ʶ����������cameraId.
 *@param int CH  �л�����Ƶʹ�õķ���ͨ��
 *@param CString str   ��������ַ��Ϣ
 *@return CString �գ���ʾ�л�ʧ��
 *				  �ǿգ���ʾ�ɹ����ŵ�URL��Ϣ
 *
*/
CString CNetPosaDlg::ChangeChannel(CString sId,int CH,CString str)
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


	m_Log.Add("��������׼���л�%s,��ͨ��%d,������%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	//if (NULL!=m_h264[CH]->h_stream)
	//{//��·�Ѿ���ʹ�ã���Ҫ�ȹر��ٽ��в���
	//	//m_netVideo[CH].CloseVideo();

	//	Main->ClosePullFromSub(m_h264[CH],NULL);//ClosePull(m_h264[CH],&m_devParams[CH]);

	//	m_h264[CH]->h_stream=NULL;

	//}
	
	//m_h264[CH]->m_Info.s_DevID = sId;//��ΪΨһ�Եı�ʶ����idΪdns:7856,��Ƶͨ��Ϊ2�����ʾΪ��dns:7856��1


	//m_h264[CH]->m_Info.s_PubServer=str;

	//m_h264[CH]->Init(&m_devDfhsParams,this->GetSafeHwnd());
	//m_h264[CH]->StartGet(m_devDfhsParams.m_devicePort);
	if(0>SetParams(m_h264[CH],&m_devParams[CH]))
		return "";
	//StartPull(m_h264[CH],&m_devParams[CH]);
	

	//m_h264[CH]->pNetVideo->OpenVideo(m_devDfhsParams.m_devicePort);

	//m_h264[CH]->h_stream = &m_devParams.m_devInfo[CH].m_deviceChn;

	{
		CString UrlStr="";
		UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
		m_Log.Add("����������Ƶ%s �л��ɹ�%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
		m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����

		return UrlStr;
	}
	return UrlStr;
}


void NP_SDK_CALLBACK OnRealDecodeData(NP_PLAY_HD playHD, const void* buf, INT len, const NPPlayFrameInfo* fInfo, void* userParam)
{//YUV����
	//TRACE("OnRealDecodeData(NP_PLAY_HD playHD[%d], const void* buf[%d], INT len[%d], const NPPlayFrameInfo* fInfo[%d], void* userParam[%d])\n",
	//	playHD, buf, len, fInfo, userParam);/**/


	CH264Stream *pH264 = (CH264Stream *)userParam;

	if(pH264 == NULL)
		return;

	pH264->b_PullLive=0;

	if(pH264->p_YuvArray.GetSize()<60)
	{
		if (NULL == buf)
		{

			return ;
		}
		if (0 == len)
		{

			return ;
		}
		MyAvFrame *m_Frame=new MyAvFrame(fInfo->width,fInfo->height);
		if (NULL == m_Frame)
		{
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");

			return ;
		}
		if (NULL == m_Frame->p_data)
		{

			return ;
		}

		//memcpy(m_Frame->p_data,buf,fInfo->width*fInfo->height*3/2);
		memcpy(m_Frame->p_data,buf,fInfo->width*fInfo->height);
		memcpy(&m_Frame->p_data[fInfo->width*fInfo->height],(char*)buf+fInfo->width*fInfo->height+(fInfo->width*fInfo->height)/4,fInfo->width*fInfo->height/4);
		memcpy(&m_Frame->p_data[fInfo->width*fInfo->height+(fInfo->width*fInfo->height)/4],(char*)buf+fInfo->width*fInfo->height,fInfo->width*fInfo->height/4);


		pH264->m_streamlock.Lock();
		pH264->p_YuvArray.Add(m_Frame);
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
	}

}

void NP_SDK_CALLBACK OnRealAudioDecodeData(NP_PLAY_HD playHD, NPPlayPcmFrame* pcmFrame,	void* userParam)
{
// 	TRACE("OnRealAudioDecodeData(NP_PLAY_HD playHD[%d], NPPlayPcmFrame* pcmFrame[%d], void* userParam[%d])\n", 
// 		playHD, pcmFrame, userParam);
}

void NP_SDK_CALLBACK fnRobbed(void* userParam, NP_NET_HD userHD, const TCHAR* target, const TCHAR* robUser, const TCHAR* robNode)
{
	TRACE("fnRobbed(void* userParam[%d], NP_NET_HD userHD[%d], const TCHAR* target[%s], const TCHAR* robUser[%s], const TCHAR* robNode[%s])",
		userParam, userHD, target, robUser, robNode);
}

void NP_SDK_CALLBACK DataCB(void* userParam,NP_NET_HD streamHD,const void* data,INT dataLen)
{
	//CtestpvgDlg::m_dataSize += dataLen;

	//DEV_NP_PARAMS *pNpParams = (DEV_NP_PARAMS *)userParam;
	CH264Stream *pH264 = (CH264Stream *)userParam;
	DEV_NP_PARAMS *pNpParams = &pNpDlg->m_devParams[pH264->thisIndex];

	pH264->b_PullLive=0;

	if(pH264->b_Encode == 1)
	{

		NPSDKError ret = NP_PLAY_InputData(pNpParams->m_devInfo.realPlayerHd,(void*)data,dataLen);
		if(ret != NPSDK_OK)
			TRACE("ret[%d] = NP_PLAY_InputData(pReal->realPlayerHd[%d], (void*)data[%d], dataLen[%d])",
			ret, pNpParams->m_devInfo.realPlayerHd, data, dataLen);
		return;
	}

	if(pH264->p_EncodeArray.GetSize()<200)
	{
		if (NULL ==data)
		{
			m_Log.Add("��Ƶ����Ϊ�գ�");

			return ;
		}
		if (0 == dataLen )
		{
			m_Log.Add("��Ƶ����Ϊ�գ�");

			return ;
		}

		MyAvFrame *m_Frame=NULL;
		//if(lFrameLen<(352*288*3/2+1))//�����޷��ж����С�����������䵱ǰ��С��Ϊ�ô���������С
		//	m_Frame = new MyAvFrame(352,288);//Ĭ�����ô�С CIF��׼
		//else
		m_Frame = new MyAvFrame(dataLen,1);//���޷��жϳ���Ƶ��С


		if (NULL == m_Frame)
		{
			m_Log.Add("new MyAvFrame��ʱ���ڴ�������");

			return ;
		}
	
		if (NULL == m_Frame->p_data)
		{
			m_Log.Add("���䵽��MyAvFrame���p_dataָ��Ϊ�գ�");

			return ;
		}
		memcpy(m_Frame->p_data,data,dataLen);//new MyAvFrame(352,288)<--byteVideoData

		pH264->m_streamlock.Lock();
		pH264->p_EncodeArray.Add(m_Frame);//��Ƶ���ݴ�����������ݻ�����CArray<MyAvFrame*,MyAvFrame*>
		pH264->m_streamlock.Unlock();
	}
}

/*���õ��ö�������SDK����Ҫ�Ĳ�����Ϣ������������Ҫ�ı���������߳�
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CNetPosaDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_NP_PARAMS *pNpParams = &m_devParams[pH264Stream->thisIndex];
	int ret= 0;

	//if(NULL != pNpParams)
	//{
	//pH264Stream->m_Info.s_DevID=pNpParams->m_devName;
	pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%s",pNpParams->m_proxyIp,pNpParams->m_proxyPort,
		pNpParams->m_userName,pNpParams->m_passWord,pNpParams->m_devName);

	//}
	
	pH264Stream->m_devType = DEF_NETPOSA_DEV;

	if (false ==pH264Stream->b_start)
	{
		
		pH264Stream->first=true;
		//pH264->StartPush(pFrameInfo->nWidth,pFrameInfo->nHeight);
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);

	return ret;
}

/*���ö�������SDK������������������лص�������ע��
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CNetPosaDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	//DEV_NP_PARAMS *pNpParams = (DEV_NP_PARAMS *)pParams;
	DEV_NP_PARAMS *pNpParams = &m_devParams[pH264Stream->thisIndex];

	pNpParams->m_devInfo.m_loginid = NULL;
	NPSDKError ret = NP_NET_Login(&pNpParams->m_devInfo.m_loginid, pNpParams->m_proxyIp, pNpParams->m_proxyPort, 
		pNpParams->m_userName, pNpParams->m_passWord);

	if(NULL == pNpParams->m_devInfo.m_loginid || ret != NPSDK_OK)
	{
		m_Log.Add( "ret[%d] = NP_PLAY_CreatePlayer(&pNpParams->m_devInfo.m_loginid[%d], pNpParams->m_proxyIp[%s], pNpParams->m_proxyPort[%d],pNpParams->m_userName[%s], pNpParams->m_passWord[%s])",
			ret, pNpParams->m_devInfo.m_loginid, pNpParams->m_proxyIp, pNpParams->m_proxyPort,pNpParams->m_userName, pNpParams->m_passWord);
		pNpParams->m_devInfo.m_loginid = NULL;
		return -1;

	}

	//���������ж�Ҫ�õ�����YUV���ݻ���H264����
	//����������˵����ҪYUV����
	if(pH264Stream->b_Encode == 1)
		ret = NP_PLAY_CreatePlayer(&pNpParams->m_devInfo.realPlayerHd, NULL, NPPLAY_STREAMTYPE_REALSTREAM, OnRealDecodeData, pStream);
	else
		ret = NP_PLAY_CreatePlayer(&pNpParams->m_devInfo.realPlayerHd, NULL, NPPLAY_STREAMTYPE_REALSTREAM, NULL, pStream);
	if (ret != NPSDK_OK)
	{
		m_Log.Add( "ret[%d] = NP_PLAY_CreatePlayer(&pReal->realPlayerHd[%d], (HWND)hwnd[%d], NPPLAY_STREAMTYPE_REALSTREAM, NULL, 0)",
			ret, &pNpParams->m_devInfo.realPlayerHd, (HWND)NULL);
		NP_NET_Logout(pNpParams->m_devInfo.m_loginid);
		pNpParams->m_devInfo.m_loginid = NULL;
		return -1;
	}

	//ret = NP_PLAY_SetAudioCallback(pReal->realPlayerHd, OnRealAudioDecodeData, this);
	//ASSERT(ret == NPSDK_OK);

	ret = NP_NET_StartStream(pNpParams->m_devInfo.m_loginid, &pNpParams->m_devInfo.realLiveHd, pNpParams->m_devName, DataCB, NULL, (void*)pH264Stream);
	if (ret != NPSDK_OK)
	{
		m_Log.Add( "ret[%d] = NP_NET_StartStream(m_loginid[%d], pReal->realLiveHd[%d], pReal->name[%s])", ret, pNpParams->m_devInfo.m_loginid, pNpParams->m_devInfo.realLiveHd, pNpParams->m_devName);
		ret = NP_PLAY_ReleasePlayer(pNpParams->m_devInfo.realPlayerHd);
		if(ret != NPSDK_OK)
			m_Log.Add( "ret[%d] = NP_PLAY_ReleasePlayer(pReal->realPlayerHd[%d])", 
			ret, pNpParams->m_devInfo.realPlayerHd);

		NP_PLAY_ReleasePlayer(pNpParams->m_devInfo.realPlayerHd);
		NP_NET_Logout(pNpParams->m_devInfo.m_loginid);

		pNpParams->m_devInfo.realPlayerHd = NULL;
		pNpParams->m_devInfo.m_loginid = NULL;
		return -1;
	}

	pH264Stream->h_stream = &ret;
	
	return 0;
}

/*���ö�������SDK��ɹرղ�������������Դ���ͷŲ���
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CNetPosaDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_NP_PARAMS *pNpParams = &m_devParams[pH264Stream->thisIndex];
	
	NPSDKError ret;

	if(pNpParams->m_devInfo.realPlayerHd)
	{
		ret= NP_PLAY_ReleasePlayer(pNpParams->m_devInfo.realPlayerHd);
		if(ret != NPSDK_OK)
			m_Log.Add( "ret[%d] = NP_PLAY_ReleasePlayer(pReal->realPlayerHd[%d])", ret, pNpParams->m_devInfo.realPlayerHd);
		pNpParams->m_devInfo.realPlayerHd = NULL;

	}


	if(pNpParams->m_devInfo.realLiveHd)
	{	
		ret = NP_NET_StopStream(pNpParams->m_devInfo.realLiveHd);
		if(ret != NPSDK_OK)
			m_Log.Add( "ret[%d] = NP_NET_StopStream(pReal->realLiveHd[%d]) pReal->name[%s]", ret, pNpParams->m_devInfo.realLiveHd, pNpParams->m_devName);
		pNpParams->m_devInfo.realLiveHd = NULL;
	}


	
	ret = NP_NET_Logout(pNpParams->m_devInfo.m_loginid);
	if(ret != NPSDK_OK)
		m_Log.Add("ret[%d] = NP_NET_Logout(m_loginid[%d])", ret, pNpParams->m_devInfo.m_loginid);

	pH264Stream->h_stream = NULL;

	return 0;
}


/*���ö�������SDK������缰�������ĳ�ʼ������
 *@date 2014-3-26
 *@author zhh
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
void CNetPosaDlg::SDKInit()
{
	char infoBuf[256] = {0};
	INT realLen = 0;
	NP_PLAY_Init();
	NPSDKError ret = NP_PLAY_GetSDK_Version(infoBuf, sizeof(infoBuf), &realLen);
	if(ret != NPSDK_OK)
		m_Log.Add("ret[%d] = NP_PLAY_GetSDK_Version(infoBuf[%s], sizeof(infoBuf)[%d])",
		ret, infoBuf, sizeof(infoBuf));
	else
		m_Log.Add( "NP_PLAY_GetSDK_Version��ȡ�汾�ɹ���infoBuf[%s]", infoBuf);
	
	NP_NET_Init();
	ret = NP_NET_GetSDK_Version(infoBuf, sizeof(infoBuf), &realLen);
	if(ret != NPSDK_OK)
		m_Log.Add( "ret[%d] = NP_NET_GetSDK_Version(infoBuf[%s], sizeof(infoBuf)[%d])",
		ret, infoBuf, sizeof(infoBuf));
	else
		m_Log.Add( "NP_NET_GetSDK_Version��ȡ�汾�ɹ���infoBuf[%s]", infoBuf);
}

/*���ö�������SDK����Դ���ͷ�
 *@date 2014-3-26
 *@author zhh
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
void CNetPosaDlg::SDKCleanup()
{
	NP_NET_Cleanup();

	NP_PLAY_Cleanup();
}

/*��CCS���͹���������:"av/001:left,76"֮�����Ϣת��Ϊ������������Ĳ�����
 *@date 2014-3-26
 *@author zhh
 *@param char *const buf ����Ĳ���������Ϣ
 *@param CCS_CMD *const cmd ת���������Žṹ
 *@return int ת��ʧ�ܷ���<0�����򷵻�0
 *
*/
static int get_ccs_cmd(char *const buf, CCS_CMD *const cmd)
{
    char *p;
    char *tmp = buf;
    char action[32];
	int nspeed=0;

    memset(cmd, 0, sizeof(CCS_CMD));

    /* get addr */
    //p = strchr(tmp, ':');
    //if (p == NULL)
    //{
    //    return (-1);
    //}
    //*p = 0x00;
    //cmd->addr = atoi(tmp);

    /* get action */
    tmp = buf;//p + 1;
    p = strchr(tmp, ',');
    if (p == NULL)
    {
        return (-2);
    }
    *p = 0x00;
    strcpy(action, tmp);

    /* get speed */
    tmp = p + 1;
    nspeed = atoi(tmp);
	cmd->speed = nspeed*15/255;

    /* check addr */
    if (cmd->addr < 0)
    {
        return (-3);
    }

    p = action;
    if (*p == '@')
    {
        p++;
        cmd->stop = 1;
    }
    /* check action */
    if (_stricmp(p, "UP") == 0)
    {
        cmd->action = NPNET_PTZ_TILTUP;
    }
    else if (_stricmp(p, "DOWN") == 0)
    {
        cmd->action = NPNET_PTZ_TILTDOWN;
    }
    else if (_stricmp(p, "LEFT") == 0)
    {
        cmd->action = NPNET_PTZ_PANLEFT;
    }
    else if (_stricmp(p, "RIGHT") == 0)
    {
        cmd->action = NPNET_PTZ_PANRIGHT;
    }

    else if ((_stricmp(p, "ZOOM TELE") == 0) || (_stricmp(p, "ZOOM IN") == 0))
    {
        cmd->action = NPNET_PTZ_ZOOM_IN;
		cmd->speed = 1;
    }
    else if ((_stricmp(p, "ZOOM WIDE") == 0) || (_stricmp(p, "ZOOM OUT") == 0))
    {
		cmd->speed = -1;
        cmd->action = NPNET_PTZ_ZOOM_IN;
    }
    else if (_stricmp(p, "FOCUS NEAR") == 0)
    {
        cmd->action = NPNET_PTZ_FOCUS;
		cmd->speed = 1;
    }
    else if (_stricmp(p, "FOCUS FAR") == 0)
    {
        cmd->action = NPNET_PTZ_FOCUS;
		cmd->speed = -1;
    }
    else if (_stricmp(p, "IRIS CLOSE") == 0)
    {
        cmd->action = NPNET_PTZ_IRIS;
		cmd->speed = -1;
    }
    else if (_stricmp(p, "IRIS OPEN") == 0)
    {
        cmd->action = NPNET_PTZ_IRIS;
		cmd->speed = 1;
    }

#if 0
    else if ((_stricmp(p, "PRESET SET") == 0) || (_stricmp(p, "YUZHI SET") == 0))
    {
        cmd->action = DEF_CCS_ACTION_PRESET_SET;
        if (cmd->stop == 1)
        {
            return (-5);
        }
    }
    else if ((_stricmp(p, "PRESET DEL") == 0) || (_stricmp(p, "YUZHI DEL") == 0))
    {
        cmd->action = DEF_CCS_ACTION_PRESET_DEL;
        if (cmd->stop == 1)
        {
            return (-6);
        }
    }
    else if ((_stricmp(p, "PRESET CALL") == 0) || (_stricmp(p, "YUZHI GET") == 0))
    {
        cmd->action = DEF_CCS_ACTION_PRESET_CALL;
        if (cmd->stop == 1)
        {
            return (-7);
        }
    }
#endif
    else if (_stricmp(p, "WIPER") == 0)
    {
        cmd->action = NPNET_PTZ_WIPE;
    }
    else
    {
        return (-8);
    }

    return (0);
}



/*���ö�������SDK�����̨�Ŀ���,���������������磺��np:av/001:left,76��������ת������ʵ�ֿ���
 *@date 2014-3-26
 *@author zhh
 *@param char * pBuf ������İ���������Ϣ���ַ���
 *@param int nLen �����ַ����ĳ���
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CNetPosaDlg::ParsePTZ(char * pBuf,int nLen)
{
	char *pKeyID=NULL,*pKeyCMD=NULL,
		*pBufID = NULL,*pBufCMD = NULL;
	CCS_CMD cmd;
	int ret,i;


	if(nLen>128)
	{
		m_Log.Add("[ParsePTZ]:[%d]->[%s]",nLen,pBuf);
		return -1;
	}

	pKeyID = strstr(pBuf,":");
	if(pKeyID != NULL)
		pKeyCMD = strstr(pKeyID+1,":");

	if(pKeyID == NULL || pKeyCMD == NULL)
	{
		m_Log.Add("[ParsePTZ]:[%d]->[%s]",nLen,pBuf);
		return -1;
	}

	pBufID = new char[nLen];
	memset(pBufID,0,nLen);

	pBufCMD = new char[nLen];
	memset(pBufCMD,0,nLen);

	memcpy(pBufID,pKeyID+1,pKeyCMD-pKeyID-1);
	memcpy(pBufCMD,pKeyCMD+1,nLen-(pKeyID-pBuf));
	//todo

	for ( i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (strcmp(pBufID,m_devParams[i].m_devName) == 0)
		{
			//if(1 == m_h264[i]->b_stop)
			break;
		}
	}

	if(i == g_maxLiveNum || i== SourceCount )
	{
		ret = -1;
		goto leaveOut;

	}
	if(0 == memcmp(pBufCMD,"?",1))
	{//����״̬��Ϣ
		char statusBuf[1024] = {0};
		INT realLen = 0;
		ret = NP_NET_GetPtzStatus(m_devParams[i].m_devInfo.m_loginid, pBufID, statusBuf, sizeof(statusBuf), &realLen);
		if(ret != NPSDK_OK)
			m_Log.Add( "ret[%d] = NP_NET_GetPtzStatus(m_loginid[%d], pReal->name[%s], statusBuf[%s], sizeof(statusBuf)[%d])",
			ret, m_devParams[i].m_devInfo.m_loginid, pBufID, statusBuf, sizeof(statusBuf));
		else
		{
			//ASSERT(realLen < 1024);
			m_Log.Add("NP_NET_GetPtzStatus���óɹ���statusBuf[%s]", statusBuf);
		}

	}else{

		//NP_NET_PtzControl()
		ret = get_ccs_cmd(pBufCMD, &cmd);
		if(ret < 0)
		{
			m_Log.Add("[ERR]:get_ccs_cmd[%d]->[%s]->[%d]",nLen,pBuf,ret);
			goto leaveOut;

		}

		if(cmd.action == NPNET_PTZ_FOCUS || cmd.action == NPNET_PTZ_ZOOM_IN || cmd.action == NPNET_PTZ_IRIS)
		{
			ret = NP_NET_PtzControl(m_devParams[i].m_devInfo.m_loginid,pBufID,NPNetPTZCommand(cmd.action),cmd.stop==1?0:cmd.speed);
		}else{
			ret = NP_NET_PtzControl(m_devParams[i].m_devInfo.m_loginid,pBufID,NPNetPTZCommand(cmd.action),cmd.stop==1?0:cmd.speed/*NPNET_PTZ_MAX_SPEED*/);
		}

		m_Log.Add("[INFO]:[%d]->[%s]->[%08x]:speed[%d]:action[%d]:stop[%d]",nLen,pBuf,ret,cmd.speed,cmd.action,cmd.stop);

		if(ret != NPSDK_OK)
		{
			m_Log.Add("[ERR]:[%d]->[%s]->[%d]",nLen,pBuf,ret);

		}
	}

	
leaveOut:

	if(NULL != pBufID )
		delete []pBufID;
	if(NULL != pBufCMD)
		delete []pBufCMD;
	m_Log.Add("[INFO]:[%d]->[%s]->[%08x]",nLen,pBuf,ret);
	return ret;
}
