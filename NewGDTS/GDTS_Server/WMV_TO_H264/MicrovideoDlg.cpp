// MicrovideoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "MicrovideoDlg.h"

extern CWMV_TO_H264Dlg* Main;
// CMicrovideoDlg �Ի���

IMPLEMENT_DYNAMIC(CMicrovideoDlg, CDialog)

CMicrovideoDlg::CMicrovideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMicrovideoDlg::IDD, pParent)
{
	for(int i=0;i<16;i++)
	{
		memset(&StreamParams[i], 0, sizeof(StreamParams[i]));
		memset(&m_DevParams[i], 0, sizeof(New_DEV_PARAMS));
	}
	
	

}

CMicrovideoDlg::~CMicrovideoDlg()
{
	for(int i=0;i<16;i++)
	{
		if(NULL != StreamParams[i].Url_Name)
			delete []StreamParams[i].Url_Name;
	}
}

void CMicrovideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMicrovideoDlg, CDialog)

END_MESSAGE_MAP()


// CMicrovideoDlg ��Ϣ�������

void CMicrovideoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}



#if USENEWSTR

int CMicrovideoDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255];
	char dst[255];
	ZeroMemory(dst,255);

	CString tmpStr;

//m_companyName
	tmpStr.Format("Company%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_companyName,dst);

//m_proxyIp;m_proxyPort;m_userName;m_passWord;m_srcUrl
	tmpStr.Format("ProxyIp%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_proxyIp,dst);

	tmpStr.Format("ProxyPort%d",nIndex);
	ZeroMemory(dst,255);
	m_DevParams[nIndex].m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

	tmpStr.Format("UserName%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_userName,dst);

	tmpStr.Format("PassWd%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_passWord,dst);

	tmpStr.Format("Source%d",nIndex);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_srcUrl,dst);

//m_deviceId;m_deviceType;m_deviceChn;m_subChn��m_deviceName
	tmpStr.Format("DeviceId%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_deviceId,dst);

	tmpStr.Format("DeviceType%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_deviceType,dst);

	tmpStr.Format("DeviceChn%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_deviceChn,dst);

	tmpStr.Format("SubChn%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_subChn,dst);
#if 0
	tmpStr.Format("DeviceName%d",SourceCount);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_deviceName,dst);
#endif
//m_dstIp;m_dstPub;m_dstStream
	tmpStr.Format("Dstip%d",nIndex);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_dstIp,dst);

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_DevParams[nIndex].m_dstPub,dst);

	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_DevParams[nIndex].m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);


	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;�Ͼ��ж������ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[SourceCount]->thisIndex = nIndex;
	m_h264[SourceCount]->b_Encode	= 1;
	m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;

	m_h264[SourceCount]->m_Info.s_PubServer=m_DevParams[nIndex].m_dstIp;
	m_h264[SourceCount]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_DevParams[nIndex].m_dstPub,nIndex);

	//add
	//sprintf((char*)m_h264[SourceCount]->p_PostData, 
	//	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
	//	"<Command>\r\n"
	//	"<serverip>%s</serverip>\r\n"//dstIp
	//	"<company>0</company>\r\n"
	//	"<%s>\r\n"//company name
	//	"<type>%s</type>\r\n"
	//	"<ver>0</ver>\r\n"
	//	"<ip>%s</ip>\r\n"
	//	"<port>%d</port>\r\n"
	//	"<url>%s</url>\r\n"
	//	"<user>%s</user>\r\n"
	//	"<passwd>%s</passwd>\r\n"
	//	"<channel>%s</channel>\r\n"
	//	"</%s>\r\n"//company name
	//	"<ch>%d</ch>\r\n"
	//	"</Command>\r\n"
	//	"\r\n\r\n"
	//	,m_DevParams.m_dstIp
	//	,m_DevParams.m_companyName
	//	,m_DevParams.m_deviceType
	//	,m_DevParams.m_proxyIp
	//	,m_DevParams.m_proxyPort
	//	,m_DevParams.m_srcUrl
	//	,m_DevParams.m_userName
	//	,m_DevParams.m_passWord
	//	,m_DevParams.m_deviceChn
	//	,m_DevParams.m_companyName
	//	,m_DevParams.m_dstStream
	//	);
	//end add
	SetParams(m_h264[nIndex],&m_DevParams[nIndex]);
	


	//m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:Source%d:%s,Dstip%d:ServerIP:%s,Dstpub%d:PubName:%s, Dststream%d:StreamNumber:v%d",
	//	nIndex,nIndex,
	//	m_MicrovideoParams.SrcUrl,nIndex,
	//	m_MicrovideoParams.ServerIP,nIndex,
	//	m_MicrovideoParams.PubName,nIndex,
	//	m_MicrovideoParams.StreamNumber);


	return 0;

}

#else

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


	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;�Ͼ��ж������ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[SourceCount]->thisIndex = nIndex;
	m_h264[SourceCount]->b_Encode	= 1;
	m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;

	m_h264[SourceCount]->m_Info.s_PubServer=m_MicrovideoParams.ServerIP;
	m_h264[SourceCount]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_MicrovideoParams.PubName,nIndex);

	//m_h264[SourceCount]->Init(SourceStr,ServerIP,PubName,StreamNumber,this->GetSafeHwnd());
	SetParams(m_h264[nIndex],&m_MicrovideoParams);
	//m_h264[SourceCount]->StartGet();
	//StartPull(m_h264[nIndex],&m_MicrovideoParams);
	


	m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:Source%d:%s,Dstip%d:ServerIP:%s,Dstpub%d:PubName:%s, Dststream%d:StreamNumber:v%d",
		nIndex,nIndex,
		m_MicrovideoParams.SrcUrl,nIndex,
		m_MicrovideoParams.ServerIP,nIndex,
		m_MicrovideoParams.PubName,nIndex,
		m_MicrovideoParams.StreamNumber);


	return 0;

}

#endif

int CMicrovideoDlg::GetInforFromIni(void * pThisDir)
{
#if 0
	char tmp[255];

	CString tmpStr;
/*	CString SourceStr="";
	CString ServerIP="";
	CString PubName="";
	int StreamNumber=0;
*/
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	SourceCount=0;
	while(true)//��ȡconfig.ini������shsl,��Ӧnew CH264Stream
	{
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("�ﵽ���ת��·������������ֹͣ������%d��%d",SourceCount,g_maxLiveNum);
			break;
		}


		tmpStr.Format("Source%d",SourceCount);
		char Server[255];
		ZeroMemory(Server,255);
		GetPrivateProfileString("mircovideo",tmpStr,"",Server,255,thisDir);
		m_MicrovideoParams.SrcUrl=Server;
		if("" == m_MicrovideoParams.SrcUrl)
		{
			m_Log.Add("config.ini�ļ���δ���ҵ��Ͼ��ж���Source%d",SourceCount);
			break;
		}
		else
		{


			tmpStr.Format("Dstip%d",SourceCount);
			char dst[255];
			ZeroMemory(dst,255);
			GetPrivateProfileString("mircovideo",tmpStr,"",dst,255,thisDir);
			m_MicrovideoParams.ServerIP=dst;

			tmpStr.Format("Dstpub%d",SourceCount);
			ZeroMemory(dst,255);
			GetPrivateProfileString("mircovideo",tmpStr,"",dst,255,thisDir);
			m_MicrovideoParams.PubName=dst;

			tmpStr.Format("Dststream%d",SourceCount);
			ZeroMemory(dst,255);
			m_MicrovideoParams.StreamNumber=GetPrivateProfileInt("mircovideo",tmpStr,-1,thisDir);


			if ("" == m_MicrovideoParams.ServerIP || "" == m_MicrovideoParams.PubName || -1 == m_MicrovideoParams.StreamNumber)
			{
				m_Log.Add("config.ini�ļ���δ����ȫ�Ͼ��ж���Dstip%d,Dstpub%d,Dststream%d",SourceCount,
					SourceCount,
					SourceCount);

				break;
			}
			else
			{
				m_h264[SourceCount]=new CH264Stream;
				if(NULL==m_h264[SourceCount])
				{
					m_Log.Add("m_h264[%d]=new CH264Stream;�Ͼ��ж������ڴ�ʱ���ִ���",SourceCount);
				}

				m_h264[SourceCount]->thisIndex = SourceCount;
				m_h264[SourceCount]->b_Encode	= 1;
				m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;

				m_h264[SourceCount]->m_Info.s_PubServer=m_MicrovideoParams.ServerIP;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_MicrovideoParams.PubName,SourceCount);

				//m_h264[SourceCount]->Init(SourceStr,ServerIP,PubName,StreamNumber,this->GetSafeHwnd());
				SetParams(m_h264[SourceCount],&m_MicrovideoParams);
				//m_h264[SourceCount]->StartGet();
				//StartPull(m_h264[SourceCount],&m_MicrovideoParams);

			}
		}
		m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:Source%d:%s,Dstip%d:ServerIP:%s,Dstpub%d:PubName:%s, Dststream%d:StreamNumber:v%d",
			SourceCount,SourceCount,
			m_MicrovideoParams.SrcUrl,SourceCount,
			m_MicrovideoParams.ServerIP,SourceCount,
			m_MicrovideoParams.PubName,SourceCount,
			m_MicrovideoParams.StreamNumber);//����������

		SourceCount++;

	}
#endif
	return 0;
}

#if USENEWSTR

//����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
//�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
int CMicrovideoDlg::SetDefaultParams(void * pParams)
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

		m_h264[SourceCount]->m_Info.s_PubServer=m_DevParams[0].m_dstIp;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_DevParams[0].m_dstPub,SourceCount);

		m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;

	}
	return 0;
}
#else
int CMicrovideoDlg::SetDefaultParams(void * pParams)
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

		m_h264[SourceCount]->m_Info.s_PubServer=m_MicrovideoParams.ServerIP;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_MicrovideoParams.PubName,SourceCount);

		m_h264[SourceCount]->m_devType = DEF_MICROVIDEO_DEV;

	}
	return 0;
}
#endif


#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

#if USENEWSTR
//<?xml version="1.0" encoding="utf-8"?>
//<command>
//<serverip>192.168.100.114</serverip>
//<company>1</company>
//<microvideo>
//<type></type>
//<ver></ver>
//<ip></ip>
//<port></port>
//<url>http://192.168.0.251:8080/ipcamera</url>
//</microvideo>
//<ch>0</ch>
//</command>

int CMicrovideoDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;
	 
	New_DEV_PARAMS tmpDevParams;

	memset(&tmpDevParams,0,sizeof(New_DEV_PARAMS));

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
		strcpy(tmpDevParams.m_dstIp , node->data);
		//m_Log.Add("��������serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL )//|| atoi(node->data)!=1)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�!\n");
		rt = 400;
		goto leave;
	}
		
	strcpy(tmpDevParams.m_companyName , node->next->name);

	subroot = f_xmlnode_sch_elem(root, tmpDevParams.m_companyName);
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�%s�ڵ�!", tmpDevParams.m_companyName);
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����MICROVIDOE��XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			//tmpType = node->data;
			strcpy(tmpDevParams.m_deviceType,node->data);
			//m_Log.Add("��������type��%s",tmpType);
		}
		else
		{
		//	m_Log.Add("xml��������δ���ҵ�type�ڵ�!");
			memset(tmpDevParams.m_deviceType,0,sizeof(tmpDevParams.m_deviceType));
		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ver", MAX_LEN);
		if (node != NULL)
		{
			//tmpVer=node->data;
			//m_Log.Add("��������ver��%s",tmpVer);
		}
		else
		{
		//	m_Log.Add("xml��������δ���ҵ�ver�ڵ�!");
		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "mainchl", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpDevParams.m_deviceChn,node->data);
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
		//	m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
			memset(tmpDevParams.m_deviceChn,0,sizeof(tmpDevParams.m_deviceChn));
		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "subchl", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpDevParams.m_subChn,node->data);
		//	m_Log.Add("��������m_subChn��%d",tmpHD.m_subChn);
		}
		else
		{
		//	m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
			memset(tmpDevParams.m_subChn,0,sizeof(tmpDevParams.m_deviceChn));
		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{

			strcpy(tmpDevParams.m_proxyIp , node->data);
			//m_Log.Add("��������ip��%s",tmpIp);
		}
		else
		{
		//	m_Log.Add("xml��������δ���ҵ�ip�ڵ�!");
			memset(tmpDevParams.m_proxyIp,0,sizeof(tmpDevParams.m_proxyIp));

		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			tmpDevParams.m_proxyPort =atoi(node->data);
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			tmpDevParams.m_proxyPort =0;

		//	m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "url", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpDevParams.m_srcUrl , node->data);
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			memset(tmpDevParams.m_srcUrl,0,sizeof(tmpDevParams.m_srcUrl));

		//	m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
		//	rt = 400;
		//	goto leave;
		}
//

		node = f_xmlnode_sch_elem_ext(subroot, "user", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.SrcUrl = node->data;
			strcpy(tmpDevParams.m_userName,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			memset(tmpDevParams.m_userName,0,sizeof(tmpDevParams.m_userName));

		//	m_Log.Add("xml��������δ���ҵ�user�ڵ�!");
		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.SrcUrl = node->data;
			strcpy(tmpDevParams.m_passWord,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			memset(tmpDevParams.m_passWord,0,sizeof(tmpDevParams.m_passWord));

		//	m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
		//	rt = 400;
		//	goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.nPort=atoi(node->data);
			strcpy(tmpDevParams.m_deviceChn , (node->data));
			//strcpy(tmpDevParams.m_deviceName , (node->data));
			
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			memset(tmpDevParams.m_deviceChn,0,sizeof(tmpDevParams.m_deviceChn));

		//	m_Log.Add("xml��������δ���ҵ�channel�ڵ�!");
		//	rt = 400;
		//	goto leave;
		}

	}

	chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
	if (chNode != NULL)
	{
		if(0 == chNode->data_siz)
		{
			m_Log.Add("��������chΪ��ֵ!");		
			m_Log.Add("������Ƶ��%s�Ƿ���ڣ�",tmpDevParams.m_deviceChn);
			WaitForSingleObject(Main->m_Lock,INFINITE);

			CString tmp;
			tmp.Format("%s;%s;%d;%s;%s;%s;%s;%s;%s;%s",
				tmpDevParams.m_companyName,
				tmpDevParams.m_proxyIp,
				tmpDevParams.m_proxyPort,
				tmpDevParams.m_userName,
				tmpDevParams.m_passWord,
				tmpDevParams.m_srcUrl,
				tmpDevParams.m_deviceId,
				tmpDevParams.m_deviceType,
				tmpDevParams.m_deviceChn,
				tmpDevParams.m_subChn
				);
			resultStr=	CheckExit(tmp);//Main->ChangeCh(m_MicrovideoParams.SrcUrl,-1,m_MicrovideoParams.ServerIP);
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
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",tmpDevParams.m_srcUrl,atoi(CH));

			m_DevParams[atoi(CH)] = tmpDevParams;

	//		sprintf((char *)m_h264[atoi(CH)]->p_PostData,"%s",pXmlBuf);

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(tmpDevParams.m_srcUrl,atoi(CH),tmpDevParams.m_dstIp);
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
#else

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
		m_MicrovideoParams.ServerIP = node->data;
		//m_Log.Add("��������serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=1)
	{
		m_Log.Add("xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���1!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "microvideo");
	if(subroot==NULL)
	{
		m_Log.Add("xml��������δ���ҵ�microvideo�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("��ʼ����MICROVIDOE��XML!");
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
			m_MicrovideoParams.ServerIP=node->data;
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
			m_MicrovideoParams.nPort=atoi(node->data);
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�port�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "url", MAX_LEN);
		if (node != NULL)
		{
			m_MicrovideoParams.SrcUrl = node->data;
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
			rt = 400;
			goto leave;
		}
	}

	chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
	if (chNode != NULL)
	{
		if(0 == chNode->data_siz)
		{
			m_Log.Add("��������chΪ��ֵ!");		
			m_Log.Add("������Ƶ��%s�Ƿ���ڣ�",m_MicrovideoParams.SrcUrl);
			WaitForSingleObject(Main->m_Lock,INFINITE);

			resultStr=	CheckExit(m_MicrovideoParams.SrcUrl);//Main->ChangeCh(m_MicrovideoParams.SrcUrl,-1,m_MicrovideoParams.ServerIP);
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
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",m_MicrovideoParams.SrcUrl,atoi(CH));
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
#endif

#if 1
BOOL myVideo(BYTE *pOut,DWORD width,DWORD height,int devId)
{
	CH264Stream* pMain=(CH264Stream*)m_h264[devId];
#if 0
	{
		static int itimes = 0;
		static DWORD myT= 0;
		if(itimes%100 == 0)
		{
			//time_t old_time=0;
			//time(&old_time);
			TRACE("*******decode 100 frame %d\n",GetTickCount()- myT);
			myT = GetTickCount();
		}
		itimes++;
	}
	//return TRUE;
#endif
	
	pMain->b_PullLive=0;

	if (false ==pMain->b_start)
	{
		
		pMain->first=true;
		//Main->StartPush(pOut->width,pOut->height);
		pMain->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pMain->b_start=true;
	}
	//pMain->m_streamlock.Lock();
	if(pMain->p_YuvArray.GetSize()<60)
	{
		if (NULL ==pOut)
		{
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		if (0 == width || 0 == height)
		{
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		MyAvFrame *m_Frame=new MyAvFrame(width,height);
		if (NULL == m_Frame)
		{
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		if (NULL == m_Frame->p_data)
		{
			//pMain->m_streamlock.Unlock();
			return TRUE;
		}
		memcpy(m_Frame->p_data,pOut,width*height*3/2);

		pMain->m_streamlock.Lock();
		pMain->p_YuvArray.Add(m_Frame);
		pMain->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pMain->p_YuvArray.GetSize());
	}
	
	//pMain->m_streamlock.Unlock();

	return TRUE;

}
#else
BOOL myVideo(AVPictureOut *pOut,VOID *params)
{
	CH264Stream* pMain=(CH264Stream*)params;
#if 0
	{
		static int itimes = 0;
		static DWORD myT= 0;
		if(itimes%100 == 0)
		{
			//time_t old_time=0;
			//time(&old_time);
			TRACE("*******decode 100 frame %d\n",GetTickCount()- myT);
			myT = GetTickCount();
		}
		itimes++;
	}
	//return TRUE;
#endif
	
	pMain->b_PullLive=0;

	if (false ==pMain->b_start)
	{
		
		pMain->first=true;
		//Main->StartPush(pOut->width,pOut->height);
		pMain->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pMain->b_start=true;
	}
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
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
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
		TRACE("�������%d\n",pMain->p_YuvArray.GetSize());
	}
	
	//pMain->m_streamlock.Unlock();

	return TRUE;

}
#endif


//typedef struct stNew_DEV_PARAMS{
//	char    m_companyName[32];//��������
//	char	m_proxyIp[32];	//ת��������IP
//	int		m_proxyPort;	//ת���������˿�
//	char	m_userName[32];	//�û���
//	char	m_passWord[32];	//����
//	char	m_srcUrl[64];	//��url
//
//	char	m_deviceId[32];	//�豸ID
//	char	m_deviceType[32];//�豸���ͣ�dvr 121��dvs 117
//	char	m_deviceChn[64];//�豸ͨ��
//	char	m_subChn[64];	//��ͨ��
//
//	char	m_dstIp[32];	//Ŀ�������IP
//	char	m_dstPub[64];	//������
//	int		m_dstStream;	//���±�
//
//}New_DEV_PARAMS;
#if USENEWSTR

int CMicrovideoDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	New_DEV_PARAMS *pMicrovideoParams = (New_DEV_PARAMS *)pParams;
	int ret= 0;

	if(NULL != pParams)
	{
		pH264Stream->m_Info.s_DevID.Format("%s;%s;%d;%s;%s;%s;%s;%s;%s;%s",
			pMicrovideoParams->m_companyName,
			pMicrovideoParams->m_proxyIp,
			pMicrovideoParams->m_proxyPort,
			pMicrovideoParams->m_userName,
			pMicrovideoParams->m_passWord,
			pMicrovideoParams->m_srcUrl,
			pMicrovideoParams->m_deviceId,
			pMicrovideoParams->m_deviceType,
			pMicrovideoParams->m_deviceChn,
			pMicrovideoParams->m_subChn

			);
		pH264Stream->m_devType = DEF_MICROVIDEO_DEV;

		memcpy(&pH264Stream->m_DevParams,pMicrovideoParams,sizeof(New_DEV_PARAMS));
	}

	ret = pH264Stream->StartPull(pStream,pParams);
	return ret;
}
#else

int CMicrovideoDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;
	int ret= 0;

	if(NULL != pParams)
	{
		pH264Stream->m_Info.s_DevID=pMicrovideoParams->SrcUrl;
		pH264Stream->m_devType = DEF_MICROVIDEO_DEV;
	}
#if 0
	//pH264Stream->m_Info.s_PubServer=pMicrovideoParams->ServerIP;
	//pH264Stream->m_Info.s_CH.Format("%d",pMicrovideoParams->StreamNumber);
	//CString tmpstr="";
	//tmpstr.Format("%s_v%d",pMicrovideoParams->PubName,pMicrovideoParams->StreamNumber);
	//pH264Stream->m_Info.s_PubName=tmpstr;

	//first=true;���ڷ����߳̿��������á�

	StreamParams[pH264Stream->thisIndex].Audio_Delay_Packet =4;				//�����ӳٲ��Ŷ��ٸ�����һ����Ϊ2��4��
	StreamParams[pH264Stream->thisIndex].Audio_on_flag = 0;					//�������߳�
	StreamParams[pH264Stream->thisIndex].Video_on_flag = 1;					//����Ƶ�߳�
	StreamParams[pH264Stream->thisIndex].Audio_is_Play = 0;					//�Ƿ񲥷�����
	StreamParams[pH264Stream->thisIndex].balance =50; //500;//0;						//��Ƶʵʱ�Ժ�������ƽ��ֵ��50��600����600ʵʱ��ǿ��50�����Ժ�

	StreamParams[pH264Stream->thisIndex].hWnd =NULL;//GetSafeHwnd();						//���ھ��
	StreamParams[pH264Stream->thisIndex].TimeAdjustFlag =0;					//�Ƿ����ʱ�����������ʱ��
	StreamParams[pH264Stream->thisIndex].VideoCallBack =NULL;//myVideo;			//��Ƶ����ص�������ÿ���һ֡��Ƶ��ص��˺�����������ΪNULL

	StreamParams[pH264Stream->thisIndex].CallBackParams = (VOID *)pH264Stream;//portNum;//��Ƶ�ص������Ĳ���
	StreamParams[pH264Stream->thisIndex].WarnningMsgFunc =NULL;//warn;		//�澯��Ϣ�ص�������������ΪNULL

	StreamParams[pH264Stream->thisIndex].VideoFormat =TYPE_YV12;//TYPE_YUY2;//TYPE_RGB24;//TYPE_NV12;//TYPE_RGB24;//TYPE_IMC4;//TYPE_UYVY;//			//��Ƶ������ͼ���ʽ
	StreamParams[pH264Stream->thisIndex].VideoRend_flag = 0;				//�Ƿ���ʾ��Ƶͼ��

	StreamParams[pH264Stream->thisIndex].receiveBuffSize = 512*1024;		//�������Ļ�������С
	if(NULL == StreamParams[pH264Stream->thisIndex].Url_Name)
		StreamParams[pH264Stream->thisIndex].Url_Name=new char[255];
	ZeroMemory(StreamParams[pH264Stream->thisIndex].Url_Name,255);

	if(NULL != pParams)
		sprintf(StreamParams[pH264Stream->thisIndex].Url_Name,"%s",pH264Stream->m_Info.s_DevID);				//����URL��ַ


	StreamParams[pH264Stream->thisIndex].videoDisplayRect.top =0;			//��Ϊ0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.left = 0;			//��Ϊ0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.right = 0;		//��Ϊ0
	StreamParams[pH264Stream->thisIndex].videoDisplayRect.bottom = 0;		//��Ϊ0
#endif
	ret = pH264Stream->StartPull(pStream,pParams);
	return ret;
}
#endif


#if USENEWSTR
// ������������
int CMicrovideoDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
//	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;

	if (false ==pH264Stream->b_start)
	{
		pH264Stream->first=true;
		//Main->StartPush(pOut->width,pOut->height);
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ZeroMemory(&pH264Stream->si, sizeof(pH264Stream->si)); 
	pH264Stream->si.cb = sizeof(pH264Stream->si); 
	ZeroMemory(&pH264Stream->pi, sizeof(pH264Stream->pi)); 
	do{ 
		// �����ӽ��̣��ж��Ƿ�ִ�гɹ�
		CString tmp;
	//	tmp.Format(".\\microvideo\\microvideo.exe %d",pH264Stream->thisIndex);
		tmp.Format(".\\%s\\%s.exe %d %s %d",pH264Stream->m_DevParams.m_companyName,pH264Stream->m_DevParams.m_companyName,pH264Stream->thisIndex,g_FrameIp,g_FramePort);

		if(!CreateProcess( NULL,(LPSTR)tmp.GetString(),NULL,NULL,FALSE,0,NULL,NULL,&pH264Stream->si,&pH264Stream->pi)) 
		{ 
			pH264Stream->h_stream=NULL;
			TRACE( "StartPull:CreateProcess[%d] fail\n",pH264Stream->thisIndex); 

			m_Log.Add( "StartPull:CreateProcess[%d] fail\n",pH264Stream->thisIndex); 
			return 0; 
		} 
		//����ִ�гɹ�����ӡ������Ϣ
		TRACE( "StartPull:����ID pi.dwProcessID: [%d],pi.dwThreadID[%d]\n", pH264Stream->pi.dwProcessId,pH264Stream->pi.dwThreadId );
		m_Log.Add( "StartPull:����ID pi.dwProcessID: [%d],pi.dwThreadID[%d]\n", pH264Stream->pi.dwProcessId,pH264Stream->pi.dwThreadId );

		TRACE( "�������ӽ��̵���Ϣ��\n"); 
		TRACE( "����ID pi.dwProcessID:  %d\n", pH264Stream->pi.dwProcessId ); 
		TRACE(  "�߳�ID pi.dwThreadID : %d\n",pH264Stream->pi.dwThreadId ); 
		// �ȴ�֪���ӽ����˳�... 
		//WaitForSingleObject( pi.hProcess, INFINITE);//�������Ƿ�ֹͣ
		//WaitForSingleObject()�����������״̬�������δȷ������ȴ�����ʱ
		//�ӽ����˳�

		//system("pause");//ִ����Ϻ�ȴ�
	}while(0);//��������Ƴ����ٴ�ִ�з���

	pH264Stream->h_stream=pH264Stream;

	return 0;
}
#else
int CMicrovideoDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
//	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;

	
	if (false ==pH264Stream->b_start)
	{
		
		pH264Stream->first=true;
		//Main->StartPush(pOut->width,pOut->height);
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ZeroMemory(&pH264Stream->si, sizeof(pH264Stream->si)); 
	pH264Stream->si.cb = sizeof(pH264Stream->si); 
	ZeroMemory(&pH264Stream->pi, sizeof(pH264Stream->pi)); 
	do{ 
		// �����ӽ��̣��ж��Ƿ�ִ�гɹ�
		CString tmp;
		tmp.Format(".\\microvideo\\microvideo.exe %d",pH264Stream->thisIndex);
		if(!CreateProcess( NULL,(LPSTR)tmp.GetString(),NULL,NULL,FALSE,0,NULL,NULL,&pH264Stream->si,&pH264Stream->pi)) 
		{ 
			pH264Stream->h_stream=NULL;

			return 0; 
		} 
		//����ִ�гɹ�����ӡ������Ϣ
		TRACE( "�������ӽ��̵���Ϣ��\n"); 
		TRACE( "����ID pi.dwProcessID:  %d\n", pH264Stream->pi.dwProcessId ); 
		TRACE(  "�߳�ID pi.dwThreadID : %d\n",pH264Stream->pi.dwThreadId ); 
		// �ȴ�֪���ӽ����˳�... 
		//WaitForSingleObject( pi.hProcess, INFINITE);//�������Ƿ�ֹͣ
		//WaitForSingleObject()�����������״̬�������δȷ������ȴ�����ʱ
		//�ӽ����˳�

		//system("pause");//ִ����Ϻ�ȴ�
	}while(0);//��������Ƴ����ٴ�ִ�з���

	pH264Stream->h_stream=pH264Stream;

#if 0
		//m_ffmpeglock.Lock();
	BOOL status=FALSE;
	if (NULL!=pH264Stream->h_stream)
	{
		try
		{
			ProcessClose(pH264Stream->h_stream);
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
		
		pH264Stream->h_stream=NULL;
	}
	try
	{
		pH264Stream->h_stream = ProcessOpen(&StreamParams[pH264Stream->thisIndex],&status);
		ProcessStart(pH264Stream->h_stream);
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
#endif
	return 0;
}
#endif
int CMicrovideoDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_MICROVIDEO_PARAMS *pMicrovideoParams = (DEV_MICROVIDEO_PARAMS *)pParams;

	CString tmp;

	//tmp.Format("Cmd.exe /C ntsd -c q -p %d",pi.dwProcessId);
	if(pH264Stream->pi.hProcess !=0 && pH264Stream->pi.hThread !=0)
	{
		tmp.Format("Cmd.exe /C taskkill /pid %d /f",pH264Stream->pi.dwProcessId);

		WinExec(tmp, SW_HIDE);//SW_SHOW);

		TRACE( "ClosePull:WinExec[%d]...\n",pH264Stream->thisIndex );

		m_Log.Add( "ClosePull:WinExec[%d]...\n",pH264Stream->thisIndex );
		//�رս��̺;��
		CloseHandle(pH264Stream->pi.hProcess); 
		CloseHandle(pH264Stream->pi.hThread); 
	}
//add
	Main->m_subInfor[pH264Stream->thisIndex].p = NULL;
	Main->m_subInfor[pH264Stream->thisIndex].p1 = NULL;
//add
	if (NULL!=pH264Stream->h_stream)
	{
		try
		{
			//ProcessClose(pH264Stream->h_stream);
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

		pH264Stream->h_stream=NULL;
	}
	return 0;
}

CString CMicrovideoDlg::CheckExit(CString str)
{
	CString UrlStr="",CamID = str;

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//���֮ǰ�Ѿ�����ͣ,��ֱ�ӷ��ص�ǰ���л�����BMS�·��л����
				m_Log.Add("��Ƶ:%s �Ѿ�ֹͣ���ţ�������!",str);
				//return "";
				continue;

			}

			UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);
			m_Log.Add("��Ƶ:%s ����!URL:%s",str,UrlStr);


			m_h264[i]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š�

			return UrlStr;
		}


	}
	m_Log.Add("��Ƶ:%s ������!",str);

	return UrlStr;
}

#if USENEWSTR

CString CMicrovideoDlg::ChangeChannel(CString sId,int CH,CString str)
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


	m_DevParams[CH].m_dstStream = CH;

	if(0>SetParams(m_h264[CH],&m_DevParams[CH]))
		return "";

	m_Log.Add("�Ͼ��ж���Ƶ׼���л�%u:%s",GetTickCount(),m_h264[CH]->m_Info.s_DevID);


	UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
	m_Log.Add("�Ͼ��ж���Ƶ�л��ɹ�%u:%s",GetTickCount(),UrlStr);
	//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
	m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����
	return UrlStr;

}
#else

CString CMicrovideoDlg::ChangeChannel(CString sId,int CH,CString str)
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


	m_MicrovideoParams.StreamNumber = CH;

	//if (NULL!=m_h264[CH]->h_stream)
	//{
	//	Main->ClosePullFromSub(m_h264[CH],NULL);
	//}


	if(0>SetParams(m_h264[CH],&m_MicrovideoParams))
		return "";


	m_Log.Add("�Ͼ��ж���Ƶ׼���л�%u:%s",GetTickCount(),m_h264[CH]->m_Info.s_DevID);

	//StartPull(m_h264[CH],&m_MicrovideoParams);

#if 0
	BOOL status=FALSE;
	if (NULL!=m_h264[CH]->h_stream)
	{
		try
		{
			ProcessClose(m_h264[CH]->h_stream);
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

		m_h264[CH]->h_stream=NULL;
	}

	try
	{
		m_h264[CH]->h_stream = ProcessOpen(&StreamParams[CH],&status);
		ProcessStart(m_h264[CH]->h_stream);
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
#endif

	UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
	m_Log.Add("�Ͼ��ж���Ƶ�л��ɹ�%u:%s",GetTickCount(),UrlStr);
	//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
	m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����
	return UrlStr;

}
#endif
int CMicrovideoDlg::InitEncode(void *pParams)
{

	return 0;
}

int CMicrovideoDlg::EncodeFrame(void *pIn,void *pOut)
{

	return 0;
}

int CMicrovideoDlg::CloseEncode(void *pParams)
{

	return 0;
}


void CMicrovideoDlg::MySleep(int iDelay,int iCount)
{
	if(iCount > 25)
		Sleep(10);
	else
		Sleep(30);

	return;
}
