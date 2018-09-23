// VNMPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "VNMPDlg.h"

extern CWMV_TO_H264Dlg* Main;
static CVNMPDlg* pVNMPDlg=NULL;
// CVNMPDlg �Ի���

IMPLEMENT_DYNAMIC(CVNMPDlg, CDialog)

CVNMPDlg::CVNMPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVNMPDlg::IDD, pParent)
{

	pVNMPDlg = this;



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
}

CVNMPDlg::~CVNMPDlg()
{

	video_device_destroy();
}

void CVNMPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVNMPDlg, CDialog)
END_MESSAGE_MAP()


// CVNMPDlg ��Ϣ�������

int CVNMPDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("ProxyIp%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_proxyIp,dst);


	tmpStr.Format("ProxyPort%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[SourceCount].m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_devParams.m_proxyPort:%d\n",m_devParams[SourceCount].m_proxyPort);


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

	m_Log.Add("��ʼ��ͨ��������: ����Ϊ:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		m_devParams[SourceCount].m_proxyIp,m_devParams[SourceCount].m_proxyPort,m_devParams[SourceCount].m_userName,
		m_devParams[SourceCount].m_passWord);
#if 0
	tmpStr.Format("PlayChn0%d",SourceCount);
	ZeroMemory(dst,255);
	m_devParams[SourceCount].m_playChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
#endif		
	tmpStr.Format("DeviceName%d",SourceCount);
	ZeroMemory(dst,255);

	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_devParams[SourceCount].m_cameraId,dst);

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
	m_devParams[SourceCount].m_devInfo.m_dstStream=GetPrivateProfileInt(Main->m_secName,tmpStr,-1,thisDir);

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

	//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
	m_h264[SourceCount]->m_devType = DEF_NETPOSA_DEV;

	SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
	//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);

	m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceName%d:%s,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		SourceCount,
		SourceCount,m_devParams[SourceCount].m_cameraId,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstIp,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstPub,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstStream);//����������
	TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);

	return 0;
}

/*���ݱ��ص������ļ���Ϣ������Ĭ��ת�빤����
 *@date 2014-3-27
 *@author zhh
 *@param void * iniPath	�����ļ�����Ŀ¼.
 *@return int     0���ɹ� ��0��ʧ��
 *
*/
int CVNMPDlg::GetInforFromIni(void * iniPath)
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
		strcpy(m_devParams[SourceCount].m_cameraId,dst);

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


		if(0 == strlen(m_devParams[SourceCount].m_cameraId) || 0 == strlen(m_devParams[SourceCount].m_devInfo.m_dstIp))
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

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//��ʱʹ��
				m_h264[SourceCount]->m_devType = DEF_NETPOSA_DEV;

				SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
				//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);
		}

		

		m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceName%s:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devParams[SourceCount].m_cameraId,
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
int CVNMPDlg::SetDefaultParams(void * pParams)
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
int CVNMPDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_VNMP_PARAMS tmpVNMP;

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
		if(0 == chNode->data_siz)
		{
			char srcCam[256];
			m_Log.Add("��������chΪ��ֵ!");		
			//sprintf(srcCam,"%s:%d:%s:%s:%s",tmpVNMP.m_proxyIp,tmpVNMP.m_proxyPort,tmpVNMP.m_userName,tmpVNMP.m_passWord,tmpVNMP.m_cameraId);

			sprintf(srcCam,"%s",tmpVNMP.m_cameraId);
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
			//tmpSrcID.Format("%s:%d:%s:%s:%s",tmpVNMP.m_proxyIp,tmpVNMP.m_proxyPort,tmpVNMP.m_userName,tmpVNMP.m_passWord,tmpVNMP.m_cameraId);
			tmpSrcID.Format("%s",tmpVNMP.m_cameraId);

#if 1
			//m_h264[atoi(CH)]->m_Info.s_DevID = tmpSrcID;
			//m_h264[atoi(CH)]->m_Info.s_PubServer = tmpVNMP.m_devInfo.m_dstIp;
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",tmpSrcID,atoi(CH));

			{
				strcpy(m_devParams[atoi(CH)].m_proxyIp,tmpVNMP.m_proxyIp);
				m_devParams[atoi(CH)].m_proxyPort = tmpVNMP.m_proxyPort;
				strcpy(m_devParams[atoi(CH)].m_userName,tmpVNMP.m_userName);
				strcpy(m_devParams[atoi(CH)].m_passWord,tmpVNMP.m_passWord);
				strcpy(m_devParams[atoi(CH)].m_cameraId,tmpVNMP.m_cameraId);

			}

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(tmpSrcID,atoi(CH),tmpVNMP.m_devInfo.m_dstIp);
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
CString CVNMPDlg::CheckExit(CString str)
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
CString CVNMPDlg::ChangeChannel(CString sId,int CH,CString str)
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
	


	m_Log.Add("������Զ׼���л�%s,��ͨ��%d,������%s",m_h264[CH]->m_Info.s_DevID,CH,str);
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
		m_Log.Add("������Զ��Ƶ%s �л��ɹ�%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
		m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����

		return UrlStr;
	}
	return UrlStr;
}


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
	CH264Stream *pH264 = m_h264[nPort];
	if(NULL == pH264)
		return;

	pH264->b_PullLive=0;

	int mWidth = pFrameInfo->nWidth;
	int mHeight = pFrameInfo->nHeight;


	if(pH264->p_YuvArray.GetSize()<60)
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

		pH264->m_streamlock.Lock();
		pH264->p_YuvArray.Add(m_Frame);
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
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
	CH264Stream *pH264Stream = m_h264[user];//(CH264Stream *)pStream;
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
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VNMP_PARAMS *pVNMPParams = &m_devParams[pH264Stream->thisIndex];
	int ret = 0;

	//pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%s",pVNMPParams->m_proxyIp,pVNMPParams->m_proxyPort,
	//	pVNMPParams->m_userName,pVNMPParams->m_passWord,pVNMPParams->m_cameraId);
	pH264Stream->m_Info.s_DevID.Format("%s",pVNMPParams->m_cameraId);

	pH264Stream->m_devType = DEF_VNMP_DEV;
	

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
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VNMP_PARAMS *pVNMPParams = (DEV_VNMP_PARAMS *)pParams;
	if(NULL == pVNMPParams)
	{
		pVNMPParams = &m_devParams[pH264Stream->thisIndex];
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
	pVNMPParams->m_devInfo.m_videoHandle = video_device_capturing_start(&videoDev,pH264Stream->thisIndex,CBK_HandVideoData);
	if(0 >pVNMPParams->m_devInfo.m_videoHandle)
	{
		m_Log.Add("[ERR]:[%d]CVNMPDlg::StartPull->video_device_capturing_start:[%ld]",pH264Stream->thisIndex,pVNMPParams->m_devInfo.m_videoHandle);
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

	pH264Stream->h_stream = &videoDev;
		
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
int CVNMPDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VNMP_PARAMS *pVNMPParams = &m_devParams[pH264Stream->thisIndex];
	int i = 0;
	
	VIDEODEVICE videodev;
	memset(&videodev,0,sizeof(VIDEODEVICE));

	//ֹͣ���봫��videodev.handle��videodev.devnum
	videodev.handle = pVNMPParams->m_devInfo.m_loginHandle;
	videodev.devnum = pVNMPParams->m_devInfo.m_videoHandle;
	video_device_capturing_stop(&videodev);
	video_play_Stop(pH264Stream->thisIndex);
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

	pH264Stream->h_stream = NULL;

	return 0;
}


