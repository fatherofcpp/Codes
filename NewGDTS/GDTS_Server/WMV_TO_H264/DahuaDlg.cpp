// DahuaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "DahuaDlg.h"


#include "dhLib.h"

extern CWMV_TO_H264Dlg* Main;
static CDahuaDlg* pDhDlg=NULL;

// CDahuaDlg �Ի���

//Callback function when device disconnected
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	if(0 != dwUser)
	{
		m_Log.Add("[INFO]:DisConnectFunc information,return code is %d",CLIENT_GetLastError());
	}
}


IMPLEMENT_DYNAMIC(CDahuaDlg, CDialog)

CDahuaDlg::CDahuaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDahuaDlg::IDD, pParent)
{
	pDhDlg = this;

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
}

CDahuaDlg::~CDahuaDlg()
{
	//Clear SDK and then release occupied resources.
	CLIENT_Cleanup();
}

void CDahuaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDahuaDlg, CDialog)
END_MESSAGE_MAP()


// CDahuaDlg ��Ϣ�������
int CDahuaDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
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
	tmpStr.Format("DeviceId%d",nIndex);
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
	m_h264[nIndex]->b_Encode	= 1;//(g_bUseTc==1?1:0);
	//m_h264[nIndex]->b_Decode = 1;

	m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_devInfo.m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_devInfo.m_dstPub,nIndex);
	//m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,nIndex);

	//m_h264[nIndex]->pNetVideo = &m_netVideo[nIndex];//��ʱʹ��
	m_h264[nIndex]->m_devType = DEF_DH_DEV;

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


/*����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
 *�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
 *@date 2014-3-27
 *@author zhh
 *@param void * pParams	��������.
 *@return int     0���ɹ� ��0��ʧ��
 *
*/
int CDahuaDlg::SetDefaultParams(void * pParams)
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


		m_h264[SourceCount]->m_devType = DEF_DH_DEV;

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
<company>4</company>

<dahua>
<type></type>//�豸����
<ver></ver>//�汾
<ip></ip>//��ַ
<port></port>//�˿�
<user></user>//�û���
<passwd></passwd>//����
<channel></channel>//Ĭ�ϲ���ͨ��
<pipe></pipe>//Ĭ���豸ͨ��
</dahua>

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
int CDahuaDlg::ParseRequest(char * pXmlBuf,CString *str)
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
			m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
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
			m_Log.Add("xml��������δ���ҵ�url�ڵ�!");
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
			sprintf(srcCam,"%s:%d:%s:%s:%s",tmpDH.m_proxyIp,tmpDH.m_proxyPort,tmpDH.m_userName,tmpDH.m_passWord,tmpDH.m_devName);
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
			tmpSrcID.Format("%s:%d:%s:%s:%s",tmpDH.m_proxyIp,tmpDH.m_proxyPort,tmpDH.m_userName,tmpDH.m_passWord,tmpDH.m_devName);
#if 1
			//m_h264[atoi(CH)]->m_Info.s_DevID = tmpSrcID;
			//m_h264[atoi(CH)]->m_Info.s_PubServer = tmpDH.m_devInfo.m_dstIp;
			m_Log.Add("��������ch: %s",CH);
			m_Log.Add("�л�%s,��ͨ��%d",tmpSrcID,atoi(CH));

			{
				strcpy(m_devParams[atoi(CH)].m_proxyIp,tmpDH.m_proxyIp);
				m_devParams[atoi(CH)].m_proxyPort = tmpDH.m_proxyPort;
				strcpy(m_devParams[atoi(CH)].m_userName,tmpDH.m_userName);
				strcpy(m_devParams[atoi(CH)].m_passWord,tmpDH.m_passWord);
				strcpy(m_devParams[atoi(CH)].m_devName,tmpDH.m_devName);

			}

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(tmpSrcID,atoi(CH),tmpDH.m_devInfo.m_dstIp);
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
CString CDahuaDlg::CheckExit(CString str)
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
CString CDahuaDlg::ChangeChannel(CString sId,int CH,CString str)
{

	m_Log.Add("���豸׼���л�%s,��ͨ��%d,������%s",sId,CH,str);
	//if (NULL!=m_h264[CH]->h_stream)
	//{//��·�Ѿ���ʹ�ã���Ҫ�ȹر��ٽ��в���
	//	//m_netVideo[CH].CloseVideo();

	//	Main->ClosePullFromSub(m_h264[CH],NULL);//ClosePull(m_h264[CH],&m_devParams[CH]);

	//	m_h264[CH]->h_stream=NULL;

	//}	

	if(0>SetParams(m_h264[CH],&m_devParams[CH]))
		return "";

	CString UrlStr="";
	UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
	m_Log.Add("���豸��Ƶ%s �л��ɹ�%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

	//SaveConfigIni(NULL,CH);//������ȷ�����õ�ini�ļ���
	m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����

	return UrlStr;

}

void CALLBACK pDecCBFun(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	CH264Stream *pH264 = (CH264Stream*)nReserved1;

	//TRACE("port is %d size is %d w:H[%d,%d]\n",nPort,nSize,pFrameInfo->nWidth,pFrameInfo->nHeight);
	if(pFrameInfo->nType != T_IYUV)
		return;

	if(pH264->p_YuvArray.GetSize()<60)
	{
		if (NULL ==pBuf)
		{
			//pH264->m_streamlock.Unlock();
			return ;
		}
		if (nSize != (pFrameInfo->nWidth*pFrameInfo->nHeight*3/2))
		{
			//pH264->m_streamlock.Unlock();
			return ;
		}
		MyAvFrame *m_Frame=new MyAvFrame(pFrameInfo->nWidth,pFrameInfo->nHeight);
		if (NULL == m_Frame)
		{
			m_Log.Add("myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
			//pH264->m_streamlock.Unlock();
			return ;
		}
		if (NULL == m_Frame->p_data)
		{
			//pH264->m_streamlock.Unlock();
			return ;
		}
		//memcpy(m_Frame->p_data,pBuf,nSize);
		memcpy(m_Frame->p_data,pBuf,pFrameInfo->nWidth*pFrameInfo->nHeight);
		memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight*5/4,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
		memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight*5/4],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
		
		
		pH264->m_streamlock.Lock();
		pH264->p_YuvArray.Add(m_Frame);
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
	}
}

void CALLBACK RealDataCallBackEx(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize, LONG lParam, DWORD dwUser)
{
	if(dwUser == 0)
	{
		return;
	}	

	CH264Stream *pH264 = (CH264Stream*)dwUser;

	pH264->b_PullLive=0;

	//TRACE("[INFO]:%ld:%d%----",lRealHandle,dwDataType,dwUser);


	BOOL bInput=FALSE;

	switch(dwDataType) {
		case 0:
			//Original data 
			bInput = PLAY_InputData(pH264->thisIndex+1,pBuffer,dwBufSize);
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
int CDahuaDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_DH_PARAMS *pDhParams = &m_devParams[pH264Stream->thisIndex];
	int ret= 0;

	//if(NULL != pDhParams)
	//{
	//pH264Stream->m_Info.s_DevID=pDhParams->m_devName;
	pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%s",pDhParams->m_proxyIp,pDhParams->m_proxyPort,
		pDhParams->m_userName,pDhParams->m_passWord,pDhParams->m_devName);

	//}
	
	pH264Stream->m_devType = DEF_DH_DEV;

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

/*���ô��豸SDK������������������лص�������ע��
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CDahuaDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	//DEV_DH_PARAMS *pDhParams = (DEV_DH_PARAMS *)pParams;
	DEV_DH_PARAMS *pDhParams = &m_devParams[pH264Stream->thisIndex];
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
	//BOOL bOpenRet = PLAY_OpenStream(pH264Stream->thisIndex+1,0,0,1024*500);
	//if(FALSE == bOpenRet)
	//{
	//	m_Log.Add("[ERR]:PLAY_OpenStream failed, error: %d\n", PLAY_GetLastError(pH264Stream->thisIndex+1));
	//	return -1;
	//}

	////Begin play 
	//BOOL bPlayRet = PLAY_Play(pH264Stream->thisIndex+1,NULL);
	//if(FALSE == bPlayRet)
	//{
	//	PLAY_CloseStream(pH264Stream->thisIndex+1);
	//	m_Log.Add("[ERR]:PLAY_Play failed, error: %d\n", PLAY_GetLastError(pH264Stream->thisIndex+1));
	//	return -1;
	//}

	//Real-time play 
	lRet = CLIENT_RealPlayEx(pDhParams->m_devInfo.m_loginid,atoi(pDhParams->m_devName),0,DH_RType_Realplay);
	if(0 != lRet)
	{
		//Enable stream
		BOOL bOpenRet = PLAY_OpenStream(pH264Stream->thisIndex+1,0,0,1024*500);
		if(FALSE == bOpenRet)
		{
			m_Log.Add("[ERR]:PLAY_OpenStream failed, error: %d\n", PLAY_GetLastError(pH264Stream->thisIndex+1));
			CLIENT_StopRealPlayEx(lRet);
			return -1;
		}
		PLAY_SetStreamOpenMode(pH264Stream->thisIndex+1,1);

		PLAY_SetDecCallBackEx(pH264Stream->thisIndex+1,pDecCBFun,(DWORD)pH264Stream);

		//Begin play 
		BOOL bPlayRet = PLAY_Play(pH264Stream->thisIndex+1,NULL);
		if(FALSE == bPlayRet)
		{
			PLAY_CloseStream(pH264Stream->thisIndex+1);
			CLIENT_StopRealPlayEx(lRet);
			m_Log.Add("[ERR]:PLAY_Play failed, error: %d\n", PLAY_GetLastError(pH264Stream->thisIndex+1));
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
			CLIENT_SetRealDataCallBackEx(lRet, RealDataCallBackEx, (DWORD)pH264Stream, 0x00000001);
	}
	else
	{
		//MessageBox(ConvertString("Fail to play!"), ConvertString("Prompt"));
		//PLAY_Stop(pH264Stream->thisIndex+1);
		//PLAY_CloseStream(pH264Stream->thisIndex+1);
		m_Log.Add("[ERR]:CLIENT_RealPlayEx failed, error: %d\n", PLAY_GetLastError(pH264Stream->thisIndex+1));
		return -1;
	}

	pDhParams->m_devInfo.realPlayerHd = lRet;

	pH264Stream->h_stream = &deviceInfo;
	
	return 0;
}

/*���ô��豸SDK��ɹرղ�������������Դ���ͷŲ���
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	�ò������õ�CH264Stream��Ķ���
 *@param void *pParams  ���ø�·��Ϣ�Ĳ����ṹ
 *
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CDahuaDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_DH_PARAMS *pDhParams = &m_devParams[pH264Stream->thisIndex];

	//First close CLIENT_RealPlay

	//Then close PLAY_Play
	BOOL bPlay = PLAY_Stop(pH264Stream->thisIndex+1);
	if(bPlay)
	{
		//At last close PLAY_OpenStream
		BOOL bStream = PLAY_CloseStream(pH264Stream->thisIndex+1);
	}

	BOOL bRealPlay = CLIENT_StopRealPlayEx(pDhParams->m_devInfo.realPlayerHd);
	if(bRealPlay)
	{

	}

	CLIENT_Logout(pDhParams->m_devInfo.m_loginid);

	pH264Stream->h_stream = NULL;

	return 0;
}


/*��CCS���͹���������:"av/001:left,76"֮�����Ϣת��Ϊ���豸����Ĳ�����
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

    memset(cmd, 0, sizeof(CCS_CMD));

    /* get addr */
    p = strchr(tmp, ':');
    if (p == NULL)
    {
        return (-1);
    }
    *p = 0x00;
    cmd->addr = atoi(tmp);

    /* get action */
    tmp = p + 1;
    p = strchr(tmp, ',');
    if (p == NULL)
    {
        return (-2);
    }
    *p = 0x00;
    strcpy(action, tmp);

    /* get speed */
    tmp = p + 1;
    cmd->speed = atoi(tmp);

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



/*���ô��豸SDK�����̨�Ŀ���,���������������磺��np:av/001:left,76��������ת������ʵ�ֿ���
 *@date 2014-3-26
 *@author zhh
 *@param char * pBuf ������İ���������Ϣ���ַ���
 *@param int nLen �����ַ����ĳ���
 *@return int �����ɹ�����0�����򷵻�-1
 *
*/
int CDahuaDlg::ParsePTZ(char * pBuf,int nLen)
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
		pKeyCMD = strstr(pKeyID,":");

	if(pKeyID == NULL || pKeyCMD == NULL)
	{
		m_Log.Add("[ParsePTZ]:[%d]->[%s]",nLen,pBuf);
		return -1;
	}

	pBufID = new char(nLen);
	memset(pBufID,0,nLen);

	pBufCMD = new char(nLen);
	memset(pBufCMD,0,nLen);

	memcpy(pBufID,pKeyID,pKeyCMD-pKeyID);
	memcpy(pBufCMD,pKeyCMD,nLen-(pKeyID-pBuf));
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

	//NP_NET_PtzControl()
	ret = get_ccs_cmd(pBufCMD, &cmd);
	if(ret < 0)
	{
		goto leaveOut;

	}

	if(cmd.action == NPNET_PTZ_FOCUS || cmd.action == NPNET_PTZ_ZOOM_IN || cmd.action == NPNET_PTZ_IRIS)
	{
		ret = 0;//NP_NET_PtzControl(m_devParams[i].m_devInfo.m_loginid,pBufID,NPNetPTZCommand(cmd.action),cmd.stop==1?0:cmd.speed);
	}else{
		ret = 0;//NP_NET_PtzControl(m_devParams[i].m_devInfo.m_loginid,pBufID,NPNetPTZCommand(cmd.action),cmd.stop==1?0:NPNET_PTZ_MAX_SPEED);
	}
	
	if(ret != 0/*NPSDK_OK*/)
	{
		m_Log.Add("[ERR]:[%d]->[%s]->[%d]",nLen,pBuf,ret);

	}

	
leaveOut:

	if(NULL != pBufID )
		delete []pBufID;
	if(NULL != pBufCMD)
		delete []pBufCMD;

	return ret;
}
