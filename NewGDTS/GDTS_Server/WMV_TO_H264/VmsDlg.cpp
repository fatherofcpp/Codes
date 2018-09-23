// VmsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "VmsDlg.h"
#include "HuaDingLib.h"
#include "Ts2h264Lib.h"

extern CWMV_TO_H264Dlg* Main;
static CVmsDlg* pVmsDlg=NULL;
// CVmsDlg �Ի���

IMPLEMENT_DYNAMIC(CVmsDlg, CDialog)

HWND CVmsDlg::m_wnd;

int ts2frame(char* pBuffer, int size,  int width, int height,long ltime,void *pUserData,int nType)
{
	CH264Stream *pH264 = (CH264Stream *)pUserData;

	//TRACE("ts2frame[%d]:size is %d width is %d,height is %d time is %ld\n",pH264->thisIndex,size,width,height,ltime);

	pH264->b_PullLive = 0;

	if(pH264->b_Encode == 1)
	{

		if((pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.m_width != width)||
			(pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.m_height != height))
		{
			if((width !=0)&&(height != 0))
			{
				pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.m_width = width;
				pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.m_height = height;

			}else
				return 0;
		}

		if(pVmsDlg->p_rawArray[pH264->thisIndex].GetSize()<200)
		{
			if (NULL ==pBuffer)
			{
				m_Log.Add("��Ƶ����Ϊ�գ�");

				return -1;
			}

			if (0 == size )
			{
				m_Log.Add("��Ƶ����Ϊ�գ�");

				return -1;
			}

			MyAvFrame *m_Frame=NULL;
			//if(lFrameLen<(352*288*3/2+1))//�����޷��ж����С�����������䵱ǰ��С��Ϊ�ô���������С
			//	m_Frame = new MyAvFrame(352,288);//Ĭ�����ô�С CIF��׼
			//else
			m_Frame = new MyAvFrame(pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.m_width,
				pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.m_height);//���޷��жϳ���Ƶ��С


			if (NULL == m_Frame)
			{
				m_Log.Add("new MyAvFrame��ʱ���ڴ�������");
				return -1;
			}

			if (NULL == m_Frame->p_data)
			{
				m_Log.Add("���䵽��MyAvFrame���p_dataָ��Ϊ�գ�");
				return -1;
			}
			memcpy(m_Frame->p_data,pBuffer,size);//new MyAvFrame(352,288)<--byteVideoData
			m_Frame->i_realsize = size;

			//pH264->m_streamlock.Lock();
			//pH264->p_EncodeArray.Add(m_Frame);//��Ƶ���ݴ�����������ݻ�����CArray<MyAvFrame*,MyAvFrame*>
			//pH264->m_streamlock.Unlock();
			pVmsDlg->m_rawlock[pH264->thisIndex].Lock();
			pVmsDlg->p_rawArray[pH264->thisIndex].Add(m_Frame);
			pVmsDlg->m_rawlock[pH264->thisIndex].Unlock();

		}else
		{
			TRACE("pVmsDlg->p_rawArray[%d].GetSize()��%d\n",pH264->thisIndex,pVmsDlg->p_rawArray[pH264->thisIndex].GetSize());

		}


#if 0
		if((pH264->YUV420P_picture2!= NULL) &&  (width != 0)&& (height != 0))
		{//�Ѿ��������룬��������������ı�Ĵ�����
			//��Ҫ�ȹر��Ѿ��򿪵Ľ�������
			if((pH264->c2->coded_width != width)||(pH264->c2->coded_height != height))
			{
				m_Log.Add("[INFO:������������ı䣬��Ҫ�ر�ԭ�򿪽������档[%d]->[%d],[%d]->[%d]\n",pH264->c2->coded_width,width,
					pH264->c2->coded_height,height);
				pH264->destroyDeH264();
				

			}

		}

		if((pH264->YUV420P_picture2 == NULL) && (width != 0)&& (height != 0))
		{
			m_Log.Add("[INFO:�����������档[%d]��[%d]\n",width,height);
			pH264->initDeH264(width,height);
			
		}

		if(pH264->YUV420P_picture2 != NULL)
		{
			DWORD nsize=0;
			//TRACE("into 2***********\n");
			MyAvFrame *m_Frame=new MyAvFrame(pH264->c2->coded_width,pH264->c2->coded_height);
			if(-1 == pH264->decoderH264((unsigned char*)pBuffer,size,(unsigned char*)m_Frame->p_data,&nsize))
			{
				delete m_Frame;
				return 0;
			}
//TRACE("into 2.1***********\n");
			if(pH264->p_YuvArray.GetSize()<60)
			{
				if (NULL ==pBuffer)
				{
					//pH264->m_streamlock.Unlock();
					return -1;
				}
				if (nsize != (pH264->c2->coded_width*pH264->c2->coded_height*3/2))
				{
					//pH264->m_streamlock.Unlock();
					return -1;
				}
				//MyAvFrame *m_Frame=new MyAvFrame(width,height);
				if (NULL == m_Frame)
				{
					m_Log.Add("[ERR]:myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
					//pH264->m_streamlock.Unlock();
					return -1;
				}
				if (NULL == m_Frame->p_data)
				{
					//pH264->m_streamlock.Unlock();
					return -1;
				}
				//memcpy(m_Frame->p_data,pBuffer,nsize);
				/*memcpy(m_Frame->p_data,pBuf,pFrameInfo->nWidth*pFrameInfo->nHeight);
				memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight*5/4,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
				memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight*5/4],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight,pFrameInfo->nWidth*pFrameInfo->nHeight/4);*/


				pH264->m_streamlock.Lock();
				pH264->p_YuvArray.Add(m_Frame);
				pH264->m_streamlock.Unlock();
			}else
			{
				TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
				delete m_Frame;
			}
		}
#endif

		return 0;
	}

	if(pH264->p_EncodeArray.GetSize()<200)
	{
		if (NULL ==pBuffer)
		{
			m_Log.Add("��Ƶ����Ϊ�գ�");

			return -1;
		}
		if (0 == size )
		{
			m_Log.Add("��Ƶ����Ϊ�գ�");

			return -1;
		}

		MyAvFrame *m_Frame=NULL;
		//if(lFrameLen<(352*288*3/2+1))//�����޷��ж����С�����������䵱ǰ��С��Ϊ�ô���������С
		//	m_Frame = new MyAvFrame(352,288);//Ĭ�����ô�С CIF��׼
		//else
		m_Frame = new MyAvFrame(size,1);//���޷��жϳ���Ƶ��С


		if (NULL == m_Frame)
		{
			m_Log.Add("new MyAvFrame��ʱ���ڴ�������");

			return -1;
		}
	
		if (NULL == m_Frame->p_data)
		{
			m_Log.Add("���䵽��MyAvFrame���p_dataָ��Ϊ�գ�");

			return -1;
		}
		memcpy(m_Frame->p_data,pBuffer,size);//new MyAvFrame(352,288)<--byteVideoData

		pH264->m_streamlock.Lock();
		pH264->p_EncodeArray.Add(m_Frame);//��Ƶ���ݴ�����������ݻ�����CArray<MyAvFrame*,MyAvFrame*>
		pH264->m_streamlock.Unlock();
	}

	return 0;
}

CVmsDlg::CVmsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVmsDlg::IDD, pParent)
{
	pVmsDlg = this;
	m_wnd = NULL;
	m_Logflag=0;

#if 1
	if(0 != loadTSLib())
	{
		m_Log.Add("[ERR]:loadTSLib error");
		exit(0);
	}
#endif	

	m_Lock=CreateMutex(NULL,FALSE,"jc_lock");
	
	if(0 != loadHuaDingLib())
	{
		m_Log.Add("[ERR]:loadHuaDingLib error");
		exit(0);
	}

	for(int i=0;i<16;i++)
	{
		m_devParams[i].m_devInfo.bFindStart=0;
	}

	memset(m_udpDst,0,sizeof(m_udpDst));
	m_udpPort  =-1;

  //---------------------------------------
  // ��ʼ��
}

CVmsDlg::~CVmsDlg()
{
	//TODO:
	CloseHandle(m_Lock);//=CreateMutex(NULL,FALSE,"jc_lock");

}

void CVmsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_DVSSTREAMPLAYERCTRL1, m_player);
}


BEGIN_MESSAGE_MAP(CVmsDlg, CDialog)
	ON_MESSAGE(WM_LIVEPLAY_PLAYSTOP, &CVmsDlg::OnPlayStop)
	ON_MESSAGE(WM_LIVEPLAY_SETMN, &CVmsDlg::OnSetMN)
	ON_MESSAGE(WM_LIVE_FINAL, &CVmsDlg::OnLiveFinal)
	ON_MESSAGE(WM_GLOBAL_EXIT, &CVmsDlg::OnGlobalExit)
END_MESSAGE_MAP()


// CVmsDlg ��Ϣ�������

int CVmsDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;

	//add
	if(-1 == m_udpPort)
	{

		ZeroMemory(dst,255);
		GetPrivateProfileString("common","UdpDst","",dst,255,thisDir);
		strcpy(m_udpDst,dst);

		ZeroMemory(dst,255);
		m_udpPort=GetPrivateProfileInt("common","UdpPort",-1,thisDir);
	}

	//end add

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

	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	m_devParams[nIndex].m_deviceChn = _atoi64(dst);
	//strcpy(m_devParams[nIndex].m_deviceChn,dst);
	//m_devParams[nIndex].m_deviceChn=GetPrivateProfileInt("common",tmpStr,-1,thisDir);

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
		m_Log.Add("[ERR]:m_h264[%d]=new CH264Stream;DEF_SHSL_DEV�����ڴ�ʱ���ִ���",nIndex);
	}

	m_h264[nIndex]->thisIndex = nIndex;
	m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);
	//m_h264[nIndex]->b_Decode = 1;

	m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_devInfo.m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_devInfo.m_dstPub,nIndex);

	m_h264[nIndex]->m_devType = DEF_VMS_DEV;

	SetParams(m_h264[nIndex],&m_devParams[nIndex]);
	//StartPull(m_h264[nIndex],&m_devParams[nIndex]);

	m_Log.Add("��ʼ��ͨ��%d������: ����Ϊ:DeviceName%d:%lld,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		nIndex,
		nIndex,m_devParams[nIndex].m_deviceChn,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstIp,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstPub,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstStream);
	//TRACE("[GetInforFromIni]:nIndex=%d\n",nIndex);

	return 0;
}



//����������û�д�INI�����ļ��ж�ȡ����·���������·�ʽ������
//�ô������ֻ�Ǵ���ʵ��������ʵ�������󲢲����ϲμ�����
int CVmsDlg::SetDefaultParams(void * pParams)
{
	//return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("[ERR]:m_h264[%d]=new CH264Stream��ȫ16��ʱ������ڴ�ʱ���ִ���",SourceCount);
		}

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->b_Encode	= (g_bUseTc==1?1:0);

		m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[0].m_devInfo.m_dstIp;
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[0].m_devInfo.m_dstPub,SourceCount);

		//m_h264[SourceCount]->h_stream = m_h264[SourceCount]->StreamParams.hWnd;
		m_h264[SourceCount]->m_devType = DEF_VMS_DEV;

	}
	return 0;
}

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20

#if 0

<company>13</company>

<vms>
<type></type>//�豸����
<ver></ver>//�汾
<ip></ip>//��ַ
<port></port>//�˿�
<user></user>//�û���
<passwd></passwd>//����
<channel></channel>//Ĭ�ϲ���ͨ��
<pipe></pipe>//Ĭ���豸ͨ��
</vms>

#endif

int CVmsDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_VMS_PARAMS tmpVMS;

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
		m_Log.Add("[INFO]:xml��������xml�ṹ���ʼ��ʧ�ܣ�");
		rt = 400;
		goto leaveOut;
	}

	if(NULL == xml.root)
	{
		m_Log.Add("[INFO]:xml����������Ч��XML�ļ���");
		rt = 400;
		goto leaveOut;
	}


	if (strcmp(xml.root->name, "command") != 0)
	{
		m_Log.Add("[INFO]:xml��������δ���ҵ�Command�ڵ�!");
		rt = 400;
		goto leave;
	}

	root = xml.root;
	node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
	if(node == NULL)
	{
		m_Log.Add("[INFO]:xml��������δ���ҵ�serverip�ڵ�!\n");
		rt = 400;
		goto leave;
	}
	else
	{
		//m_MicrovideoParams.ServerIP = node->data;
		strcpy(tmpVMS.m_devInfo.m_dstIp,node->data); 
		//m_Log.Add("��������serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=13)
	{
		m_Log.Add("[INFO]:xml��������δ���ҵ�company�ڵ�,����company�ڵ����ݲ���vms == 13!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "vms");
	if(subroot==NULL)
	{
		m_Log.Add("[INFO]:xml��������δ���ҵ�vms�ڵ�!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("[INFO]:��ʼ����VMS��XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			//tmpType = node->data;
			//m_Log.Add("��������type��%s",tmpType);
		}
		else
		{
			m_Log.Add("[INFO]:xml��������δ���ҵ�type�ڵ�!");
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
			m_Log.Add("[INFO]:xml��������δ���ҵ�ver�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpVMS.ServerIP=node->data;
			strcpy(tmpVMS.m_proxyIp,node->data);
			//m_Log.Add("��������ip��%s",tmpIp);
		}
		else
		{
			m_Log.Add("[INFO]:xml��������δ���ҵ�ip�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			//tmpVMS.nPort=atoi(node->data);
			tmpVMS.m_proxyPort = atoi(node->data);
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("[INFO]:xml��������δ���ҵ�port�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "user", MAX_LEN);
		if (node != NULL)
		{
			//tmpVMS.SrcUrl = node->data;
			strcpy(tmpVMS.m_userName,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			m_Log.Add("[INFO]:xml��������δ���ҵ�url�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			//tmpVMS.SrcUrl = node->data;
			strcpy(tmpVMS.m_passWord,node->data);
			//m_Log.Add("��������url��%s",tmpSource);
		}
		else
		{
			m_Log.Add("[INFO]:xml��������δ���ҵ�url�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			//tmpVMS.nPort=atoi(node->data);
			tmpVMS.m_deviceChn = _atoi64(node->data);
			
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("[INFO]:xml��������δ���ҵ�port�ڵ�!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "pipe", MAX_LEN);
		if (node != NULL)
		{
			//tmpVMS.nPort=atoi(node->data);
			tmpVMS.m_playChn = atoi(node->data);
			
			//m_Log.Add("��������port��%s",tmpPort);
		}
		else
		{
			m_Log.Add("[INFO]:xml��������δ���ҵ�port�ڵ�!");
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
			m_Log.Add("[INFO]:��������chΪ��ֵ!");		
			sprintf(srcCam,"%s:%d:%s:%s:%lld",tmpVMS.m_proxyIp,tmpVMS.m_proxyPort,tmpVMS.m_userName,tmpVMS.m_passWord,tmpVMS.m_deviceChn);
			m_Log.Add("[INFO]:������Ƶ��%s�Ƿ���ڣ�",srcCam);

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
			if(atoi(CH)<0)
			{
				rt=404;
				goto leave;
			}
#endif


			CString tmpSrcID;
			tmpSrcID.Format("%s:%d:%s:%s:%lld",tmpVMS.m_proxyIp,tmpVMS.m_proxyPort,tmpVMS.m_userName,tmpVMS.m_passWord,tmpVMS.m_deviceChn);
#if 1
			m_h264[atoi(CH)]->m_Info.s_DevID = tmpSrcID;
			m_h264[atoi(CH)]->m_Info.s_PubServer = tmpVMS.m_devInfo.m_dstIp;
			m_Log.Add("[INFO]:��������ch: %s",CH);
			m_Log.Add("[INFO]:�л�%s,��ͨ��%d",tmpSrcID,atoi(CH));

			{
				strcpy(m_devParams[atoi(CH)].m_proxyIp,tmpVMS.m_proxyIp);
				m_devParams[atoi(CH)].m_proxyPort = tmpVMS.m_proxyPort;
				strcpy(m_devParams[atoi(CH)].m_userName,tmpVMS.m_userName);
				strcpy(m_devParams[atoi(CH)].m_passWord,tmpVMS.m_passWord);
				m_devParams[atoi(CH)].m_deviceChn = tmpVMS.m_deviceChn;

			}

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(tmpSrcID,atoi(CH),tmpVMS.m_devInfo.m_dstIp);
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
		m_Log.Add("[INFO]:xml��������ch�ڵ��ֵΪ��!");
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

CString CVmsDlg::CheckExit(CString str)
{
	CString UrlStr="",CamID = str;

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//���֮ǰ�Ѿ�����ͣ,��ֱ�ӷ��ص�ǰ���л�����BMS�·��л����
				m_Log.Add("[INFO]:��Ƶ:%s �Ѿ�ֹͣ���ţ�������!",CamID);
				//return "";
				continue;

			}

			UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);
			m_Log.Add("[INFO]:��Ƶ:%s ����!URL:%s",CamID,UrlStr);


			m_h264[i]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š�

			return UrlStr;
		}


	}
	m_Log.Add("��Ƶ:%s ������!",CamID);

	return UrlStr;
}


CString CVmsDlg::ChangeChannel(CString sId,int CH,CString str)
{
	CString UrlStr="";

	if(CH >= g_maxLiveNum)
	{//��ʱ�������ת��·��
		m_Log.Add("[INFO]:ԭ��ƵӦ���л�ͨ��Ϊ:%d \n!",CH);
		CH = CH%g_maxLiveNum;
		SourceCount = g_maxLiveNum;
		m_Log.Add("[INFO]:ʵ����ƵӦ���л�ͨ��Ϊ:%d \n!",CH);
	}

	if (CH>=SourceCount || CH<0)
	{

		m_Log.Add("[INFO]:CH��ֵ���ܴ���%d������!",g_maxLiveNum);
		return "";
	}
	
	m_Log.Add("[INFO]:����׼���л�%s,��ͨ��%d,������%s",m_h264[CH]->m_Info.s_DevID,CH,str);

	if(0>SetParams(m_h264[CH],&m_devParams[CH]))
		return "";
	{
		CString UrlStr="";
		UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
		m_Log.Add("[INFO]:������Ƶ%s �л��ɹ�%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		m_h264[CH]->b_stop = 0;//����֮ǰ�Ƿ��ڲ��ţ�����Ӧ���������š��ҳ���ʱ������ڿ�ʼ����

		return UrlStr;
	}
	return UrlStr;
}

UINT __cdecl parseTsFunc(void* p)
 {

	CH264Stream*pH264=(CH264Stream*)p;
	int index=pH264->thisIndex;
	int rtn=0;
	int nTimes = 0;

	InitTsFunc(&pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.ts,ts2frame);
	pVmsDlg->m_devParams[index].m_devInfo.ts.pUserData = pH264;


	TS2FRAME * pts=&(pVmsDlg->m_devParams[index].m_devInfo.ts);

	MyAvFrame *m_Frame=NULL;

	while (true)
	{
		if(Main->g_Stop == 1)
			break;

		if(nTimes % 5000 == 0)
		{
			TRACE("[decoderthread]:��·[%s]�߳��������С�%d����\n",pH264->m_Info.s_PubName,nTimes);
		}
		nTimes++;


		m_Frame=NULL;//���ߣ�I֡��
		//p_H264Stream->m_streamlock.Lock();
		pVmsDlg->m_rawlock[index].Lock();
		if (pVmsDlg->p_tsArray[index].GetSize()>0)// p_H264Stream->p_YuvArray.GetSize()>0)//ģ��CArry������
		{
			m_Frame=pVmsDlg->p_tsArray[index].ElementAt(0);//��ģ��CArray<MyAvFrame*,MyAvFrame*>��ȡ����
			pVmsDlg->p_tsArray[index].RemoveAt(0);//��ȡ��ɾ��
		}
		pVmsDlg->m_rawlock[index].Unlock();


		int i_Len=0;
		if (NULL!=m_Frame)//��ģ��CArray��ȡ������Ƶ����
		{
			unsigned char udata[188];
			unsigned char * buf = (unsigned char *)m_Frame->p_data;
			int size = m_Frame->i_height*m_Frame->i_width;

			int ind = 0;
			if (!pVmsDlg->m_devParams[index].m_devInfo.bFindStart)
			{
				//��û�ҵ���ʼ				
				while(size >= 188 && ind < size - 188)
				{
					if (*(buf + ind)  == 0x47 && *(buf + ind + 188)  == 0x47)
					{
						pVmsDlg->m_devParams[index].m_devInfo.bFindStart = TRUE;
						break;
					}
					ind++;
				}	
			}

			if(pVmsDlg->m_devParams[index].m_devInfo.bFindStart)
			{
				while(ind <= size - 188)
				{
					memcpy(udata, buf+ind, 188);
					ProcessTSData(pts,udata,188);

					ind += 188;
				}
			}


			delete m_Frame;
		}//NULL!=m_Frame
		else
			Sleep(40);
	}

    return 0;
}

UINT __cdecl raw2yuv_hd(void* p)
 {

	CH264Stream*pH264=(CH264Stream*)p;
	int index=pH264->thisIndex;
	int rtn=0;
	int nTimes = 0;

	MyAvFrame *m_Frame=NULL;

	while (true)
	{
		if(Main->g_Stop == 1)
			break;

		if(nTimes % 5000 == 0)
		{
			TRACE("[decoderthread]:��·[%s]�߳��������С�%d����\n",pH264->m_Info.s_PubName,nTimes);
		}
		nTimes++;


		m_Frame=NULL;//���ߣ�I֡��
		//p_H264Stream->m_streamlock.Lock();
		pVmsDlg->m_rawlock[index].Lock();
		if (pVmsDlg->p_rawArray[index].GetSize()>0)// p_H264Stream->p_YuvArray.GetSize()>0)//ģ��CArry������
		{
			m_Frame=pVmsDlg->p_rawArray[index].ElementAt(0);//��ģ��CArray<MyAvFrame*,MyAvFrame*>��ȡ����
			pVmsDlg->p_rawArray[index].RemoveAt(0);//��ȡ��ɾ��
		}
		pVmsDlg->m_rawlock[index].Unlock();


		int i_Len=0;
		if (NULL!=m_Frame)//��ģ��CArray��ȡ������Ƶ����
		{
			if(pH264->YUV420P_picture2!= NULL)// &&  (width != 0)&& (height != 0))
			{//�Ѿ��������룬��������������ı�Ĵ�����
				//��Ҫ�ȹر��Ѿ��򿪵Ľ�������
				if((pH264->c2->coded_width != m_Frame->i_width)||(pH264->c2->coded_height != m_Frame->i_height))
				{
					m_Log.Add("[INFO:������������ı䣬��Ҫ�ر�ԭ�򿪽������档[%d]->[%d],[%d]->[%d]\n",pH264->c2->coded_width,m_Frame->i_width,
						pH264->c2->coded_height,m_Frame->i_height);
					pH264->destroyDeH264();


				}

			}

			if((pH264->YUV420P_picture2 == NULL) && (m_Frame->i_width != 0)&& (m_Frame->i_height != 0))
			{
				m_Log.Add("[INFO:�����������档[%d]��[%d]\n",m_Frame->i_width,m_Frame->i_height);
				pH264->initDeH264(m_Frame->i_width,m_Frame->i_height);

			}

			if(pH264->YUV420P_picture2 != NULL)
			{
				DWORD nsize=0;
				//TRACE("into 2***********\n");
				MyAvFrame *tmp_Frame=new MyAvFrame(pH264->c2->coded_width,pH264->c2->coded_height);

				if(-1 == pH264->decoderH264((unsigned char*)m_Frame->p_data,m_Frame->i_realsize,(unsigned char*)tmp_Frame->p_data,&nsize))
				{
					delete tmp_Frame;
					delete m_Frame;
					continue;
				}
				//TRACE("into 2.1***********\n");
				if(pH264->p_YuvArray.GetSize()<60)
				{
	
					if (nsize != (pH264->c2->coded_width*pH264->c2->coded_height*3/2))
					{
						//pH264->m_streamlock.Unlock();
						delete tmp_Frame;
						delete m_Frame;
						continue;
					}
					//MyAvFrame *m_Frame=new MyAvFrame(width,height);
					
					//memcpy(m_Frame->p_data,pBuffer,nsize);
					/*memcpy(m_Frame->p_data,pBuf,pFrameInfo->nWidth*pFrameInfo->nHeight);
					memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight*5/4,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
					memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight*5/4],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight,pFrameInfo->nWidth*pFrameInfo->nHeight/4);*/


					pH264->m_streamlock.Lock();
					pH264->p_YuvArray.Add(tmp_Frame);
					pH264->m_streamlock.Unlock();
				}else
				{
					TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
					delete tmp_Frame;
					//delete m_Frame;
				}


			}

			delete m_Frame;
		}//NULL!=m_Frame
		else
			Sleep(40);
	}

    return 0;
}


UINT __cdecl RecvLocalGPS(void* test)
{
#define MAXLEN 1500
	CH264Stream*pH264=(CH264Stream*)test;
	int index=pH264->thisIndex;

	//InitTsFunc(&pVmsDlg->m_devParams[pH264->thisIndex].m_devInfo.ts,ts2frame);
	//pVmsDlg->m_devParams[index].m_devInfo.ts.pUserData = pH264;


	unsigned short port = pVmsDlg->m_udpPort+10*index;
	//TS2FRAME * pts=&(pVmsDlg->m_devParams[index].m_devInfo.ts);

	int sockfd;
	struct sockaddr_in bindAddr;

	char recvBuf[MAXLEN];
	ZeroMemory(recvBuf, sizeof(recvBuf));
	int nRecv=0;
	int recvLen=0;
	int ret=0;
	struct sockaddr_in cliAddr;
	int cliAddrLen = sizeof(cliAddr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		TRACE("Invalid socket\n");
		exit(1);
	}

	ZeroMemory(&bindAddr, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(port);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	TRACE("Binding server to port %d\n", port);
	if(bind(sockfd, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
	{
		shutdown(sockfd, 2);
		closesocket(sockfd);
		m_Log.Add("[ERR]:binding err port[%d]!\n",port);
		exit(1);
	}
	TRACE("waiting data from other client...\n");

	static FILE *pf = NULL;

	while(1)
	{
		recvLen = recvfrom(sockfd, recvBuf, MAXLEN, 0, (struct sockaddr*)&cliAddr, &cliAddrLen);
		if(recvLen>0)
		{


			//TRACE("into 2.1***********\n");
			if(pVmsDlg->p_tsArray[index].GetSize()<60)
			{


				//MyAvFrame *m_Frame=new MyAvFrame(width,height);
				MyAvFrame *tmp_Frame=new MyAvFrame(recvLen,1);

				memcpy(tmp_Frame->p_data,recvBuf,recvLen);
				/*memcpy(m_Frame->p_data,pBuf,pFrameInfo->nWidth*pFrameInfo->nHeight);
				memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight*5/4,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
				memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight*5/4],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight,pFrameInfo->nWidth*pFrameInfo->nHeight/4);*/


				pVmsDlg->m_rawlock[index].Lock();
				pVmsDlg->p_tsArray[index].Add(tmp_Frame);
				pVmsDlg->m_rawlock[index].Unlock();
			}else
			{
				TRACE("[INFO]:pVmsDlg->p_tsArray[%d]��%d\n",index,pVmsDlg->p_tsArray[index].GetSize());

			}

			//TRACE("################recvLen:%d\n",recvLen);
			//add
#if 0
			unsigned char udata[188];
			unsigned char * buf = (unsigned char *)recvBuf;
			int size = recvLen;

			int ind = 0;
			if (!pVmsDlg->m_devParams[index].m_devInfo.bFindStart)
			{
				//��û�ҵ���ʼ				
				while(size >= 188 && ind < size - 188)
				{
					if (*(buf + ind)  == 0x47 && *(buf + ind + 188)  == 0x47)
					{
						pVmsDlg->m_devParams[index].m_devInfo.bFindStart = TRUE;
						break;
					}
					ind++;
				}	
			}

			if(pVmsDlg->m_devParams[index].m_devInfo.bFindStart)
			{
				while(ind <= size - 188)
				{
					memcpy(udata, buf+ind, 188);
					ProcessTSData(pts,udata,188);

					ind += 188;
				}
			}
#endif
		}else{
			Sleep(10);
			continue;
		}
	}

	shutdown(sockfd, 2);
	closesocket(sockfd);

	return 0;
}

int CVmsDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VMS_PARAMS *pVMSParams = (DEV_VMS_PARAMS *)pParams;
	int ret;

	pH264Stream->m_Info.s_DevID.Format("%s:%d:%s:%s:%lld",pVMSParams->m_proxyIp,pVMSParams->m_proxyPort,
		pVMSParams->m_userName,pVMSParams->m_passWord,pVMSParams->m_deviceChn);

	if (false ==pH264Stream->b_start)
	{
		
		pH264Stream->first=true;
		//pH264->StartPush(pFrameInfo->nWidth,pFrameInfo->nHeight);
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		AfxBeginThread( RecvLocalGPS,pH264Stream,THREAD_PRIORITY_NORMAL);
		AfxBeginThread( parseTsFunc,pH264Stream,THREAD_PRIORITY_NORMAL);

		if(pH264Stream->b_Encode == 1)
			AfxBeginThread( raw2yuv_hd,pH264Stream,THREAD_PRIORITY_NORMAL);

		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);

	return 0;
}
void CVmsDlg::LogOut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
//	if (m_playflag == TRUE)
	{
		//m_player.Stop(1);
	}
//	m_playflag = FALSE;//ʵ�����ű�־��Ϊ��δ����

	//ע����½
	VMS_Logout();

	//�ͷ�SDK��Դ
	VMS_Destory();
}
#if 0
static FILE *g_pf=NULL;
void SaveTs(char * data,int len)
{
	int wLen=0,i=0;
	if(g_pf == NULL)
		g_pf = fopen("e://test//dst.ts","wb");
	wLen=fwrite(data,1,len,g_pf);
	//TRACE("[SaveTs]:----------\n");
	//for(i=0;i<len;i++)
	//{
	//	TRACE("%x ",data[i]);
	//}
	//TRACE("[SaveTs]:----------\n");
}
#endif
int GetStreamData(long index, StreamType type, void *data, long len, long *pUserData)
{
//	TRACE("index = %d\n", index);
	
	CH264Stream* pH264=NULL;
#if 0	
	int i = 0;
	for( i = 0; i< 16; i++)
	{
		if(pH264 == m_h264[i])
		{
		//	TRACE("GetStreamData:i:%d\n",i);
			break;
		}
	}
	if(i == 16 || i >= g_maxLiveNum)
	{
		return -1;
		TRACE("GetStreamData:i == 16 || i > =g_maxLiveNum,i:%d",i);
		m_Log.Add("GetStreamData:i == 16 || i > =g_maxLiveNum,i:%d",i);

	}
#else
	TS2FRAME * pts=&(pVmsDlg->m_devParams[index-1].m_devInfo.ts);
	pH264=m_h264[index-1];
//	TRACE("GetStreamData:pH264->thisIndex:%d\n",pH264->thisIndex);
#endif
	pH264->b_PullLive=0;
	
	//�û��Զ������ݵ���֤
	//(*((UserDataFuc)(pUserData)))();
	switch(type)
	{
	case VIDEO_RTP_H264:
	case TS_PACKET_DATA:
	case AUDIO_RTP_G711_A:
	case AUDIO_RTP_G711_U:
	case AUDIO_RTP_G726:
	case AUDIO_RAW_G711A:
	case AUDIO_RAW_G711U:
	case AUDIO_RAW_G726:
	//	TRACE("[GetStreamData]:TS_PACKET_DATA:len:%d--data:%d\n", len, *((char*)data));
	//	SaveTs((char*)data,len);
		{
 			unsigned char udata[188];
 			unsigned char * buf = (unsigned char *)data;
 			int size = len;
 			
 			int ind = 0;
 			if (!pVmsDlg->m_devParams[index-1].m_devInfo.bFindStart)
 			{
 				//��û�ҵ���ʼ				
 				while(size >= 188 && ind < size - 188)
 				{
 					if (*(buf + ind)  == 0x47 && *(buf + ind + 188)  == 0x47)
 					{
 						pVmsDlg->m_devParams[index-1].m_devInfo.bFindStart = TRUE;
 						break;
 					}
 					ind++;
 				}	
 			}
 
 			if(pVmsDlg->m_devParams[index-1].m_devInfo.bFindStart)
 			{
 				while(ind <= size - 188)
 				{
 					memcpy(udata, buf+ind, 188);
 				//	ProcessTSData(&pts,udata,188);
 					ProcessTSData(pts,udata,188);

 					ind += 188;
 				}
 			}
		}
	    break;
	case VIDEO_YUV12_RAW:
		{
			PVideoRawInfo ptmp = (PVideoRawInfo)data;
		//	TRACE("[GetStreamData]:VIDEO_YUV12_RAW:%d--%d--%d--%d--%d--%d\n", ptmp->height, ptmp->width, ptmp->length, *((char*)(ptmp->data)), ptmp->pitch, len );
			//save frame
			if(pH264->p_YuvArray.GetSize()<60)
				{
					if (NULL ==ptmp->data)
					{
						//pH264->m_streamlock.Unlock();
						return -1;
					}
					if (ptmp->length != (ptmp->width*ptmp->height*3/2))
					{
						//pH264->m_streamlock.Unlock();
						return -1;
					}
					MyAvFrame *m_Frame=new MyAvFrame(ptmp->width,ptmp->height);
					if (NULL == m_Frame)
					{
						m_Log.Add("[ERR]:myVideo����MyAvFrame *m_Frame=new MyAvFrame�ڴ����ʱ���ִ���");
						//pH264->m_streamlock.Unlock();
						return -1;
					}
					if (NULL == m_Frame->p_data)
					{
						//pH264->m_streamlock.Unlock();
						return -1;
					}
					memcpy(m_Frame->p_data,ptmp->data,ptmp->length);
					/*memcpy(m_Frame->p_data,pBuf,pFrameInfo->nWidth*pFrameInfo->nHeight);
					memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight*5/4,pFrameInfo->nWidth*pFrameInfo->nHeight/4);
					memcpy(&m_Frame->p_data[pFrameInfo->nWidth*pFrameInfo->nHeight*5/4],pBuf+pFrameInfo->nWidth*pFrameInfo->nHeight,pFrameInfo->nWidth*pFrameInfo->nHeight/4);*/
					
					
					pH264->m_streamlock.Lock();
					pH264->p_YuvArray.Add(m_Frame);
					pH264->m_streamlock.Unlock();
				}else
				{
					TRACE("�������%d\n",pH264->p_YuvArray.GetSize());
				}
				//end save frame
		}//end VIDEO_YUV12_RAW
		break;
	default:
	    break;
	}
	return TRUE;
}
int UserFuc()
{
	TRACE("UserFuc\n");
	return TRUE;
}

//�ص�������������ʼʵ��
//��ʵ��������VMS_StratMonitor()���ú󣬵�һ����Ӧ�ص������ⲿʵ��
//������������������������� ��ʼ����ʵ�� ����Ϣ�����Ŀ���ƽ̨
HRESULT CVmsDlg::OnSetMN( WPARAM wParam, LPARAM lParam)
{
	//??
 	PNotifyDCRs pParamFrom = (PNotifyDCRs)wParam;
	int ret = 0;
	long channel = pParamFrom->dc_chan_id.num +1;

	//Int64 m_currentcamid=(Int64)9007199256192805;
	TRACE("OnSetMN:enter1[%d]\n",pParamFrom->dc_chan_id.num);


//	if( ERR_REGISTERCB_FAILED == m_player.RegGetStreamDataCB((long )GetStreamData, (long )m_h264[pParamFrom->dc_chan_id.num]) )
//	{
//		m_Log.Add("[INFO]:CVmsDlg::OnSetMN:RegGetStreamDataCB fail,pParamFrom->dc_chan_id.num:[%d]",pParamFrom->dc_chan_id.num);
//		TRACE("OnSetMN:ע��ʧ�ܣ�\n");
//	}
	TRACE("OnSetMN:enter3\n");

	//TRACE("OnSetMN:channel:%ld,m_currentcamid:%lld\n",channel,m_currentcamid);
	//��ʼ��������
	//ret = m_player.Play(channel, (long)0, (long)0, (long)1,(long)m_currentcamid, (long)VIDEO_STREAM_MAIN, (LPCTSTR)NULL, (long)0, (long)0, (long)0, (long)0);
//	ret = m_player.Play(channel, 0, 0, 1, m_devParams[pParamFrom->dc_chan_id.num].m_deviceChn, VIDEO_STREAM_MAIN, NULL, 0, 0, 0, 0);
	
	TRACE("OnSetMN:enter4\n");

	NotifyMN p2 = {0};
//add
	memset(p2.ip,0,16);
	//p2.ip[0]=192;
	//p2.ip[1]=168;
	//p2.ip[2]=2;
	//p2.ip[3]=31;
	//char tmpIp[64];
	//memset(tmpIp,0,sizeof(tmpIp));
	//memcpy(tmpIp,m_udpDst,sizeof(m_udpDst));
 //   char*p = strtok(tmpIp,".");   
	//int i=0;
 //   while( p )   
 //   {   
 //       p2.ip[i]=atoi(p);   
 //       p = strtok(NULL,".");   
	//	i++;
 //   }

	u_long l = inet_addr(m_udpDst);
	p2.ip[0]=(unsigned char)(l&0xff);
	p2.ip[1]=(unsigned char)((l>>8)&0xff);
	p2.ip[2]=(unsigned char)((l>>16)&0xff);
	p2.ip[3]=(unsigned char)((l>>24)&0xff);


	p2.port =m_udpPort+ pParamFrom->dc_chan_id.num*10; 
	//pVmsDlg->m_devParams[pParamFrom->dc_chan_id.num].m_udpDst
//end add
//	m_player.SetMNInfo((long)pParamFrom, (long)&p2);
	TRACE("OnSetMN:enter5\n");
	
	TRACE("OnSetMN:p2.mc_flag:%d,p2.port:%d,channel:%d\n",
		p2.mc_flag,p2.port,channel);
	TRACE("OnSetMN:ip[0]:%d,ip[1]:%d,ip[2]:%d,ip[3]:%d\n",p2.ip[0],p2.ip[1],p2.ip[2],p2.ip[3]);

	int res = VMS_RSStartReply((void *)pParamFrom, (void *)&p2);
	TRACE("OnSetMN:res:%d\n",res);

	if(res == 32768)
		TRACE("OnSetMN:suc%d\n",pParamFrom->dc_chan_id.num);
		

	return ret;
}
//�ص�����������ֹͣʵ��
//��ʵ��������VMS_StopMonitor()���ú󣬵�һ����Ӧ�ص������ⲿʵ��
//���������ֹͣ������
//��ע���˴����Բ���Ҫ�ȴ��ص������ĵڶ�����Ӧ����Ϊ�ͻ��˿�����ֹͣ������������Ҫ����ն��豸�Ƿ������������
HRESULT CVmsDlg::OnPlayStop( WPARAM wParam, LPARAM lParam)
{
	RSRetInfo *p_ret = (RSRetInfo *)wParam;
	Uint8 DC = p_ret->rs_replay_info.dc_chan_id.num;
	TRACE("DC OnPlayStop is %d\n",DC);

	TRACE("OnPlayStop:enter\n");
//	if ( ERR_STATE_WRONG == m_player.StopGetStreamData(DC+1) )
//		TRACE("OnPlayStop:ֹͣ��ȡʧ�ܣ�\n");
//	m_player.Stop(DC+1);//ֹͣ��������Ĭ�ϴ���Ϊ1
	int res = VMS_RSStopReply((void*)wParam);
	return 0;
}
//�ص�����������ȷ��ʵ��
//��ʵ��������VMS_StratMonitor()���ú󣬵ڶ�����Ӧ�ص������ⲿʵ��
//����������ж��ն��豸�Ƿ�ʼ������������������ʼ���ţ���ʧ����ֹͣ������
HRESULT CVmsDlg::OnLiveFinal( WPARAM wParam, LPARAM lParam)
{
	RSRetInfo *p_ret = (RSRetInfo *)wParam;
	Uint8 DC = p_ret->rs_replay_info.dc_chan_id.num;
	TRACE("OnLiveFinal:enter:%d\n",DC);

	//ʧ�ܴ���
	if (p_ret->status != 0x55)//��ʵ������ɹ����򷵻�0x55
	{
		TRACE("ʵ������ʧ�ܣ�DC:%d\n",DC);
		m_Log.Add("[INFO]:CVmsDlg::OnLiveFinal:ʵ������ʧ��,DC:%d",DC);
		//m_player.Stop(DC+1);
		m_h264[DC]->h_stream=NULL;
		return 0;
	}

	long mask=0;
	if(m_h264[DC]->b_Encode == 0)
	{
	    mask = TS_PACKET_DATA;
	//	if( ERR_STATE_WRONG == m_player.StartGetStreamData(DC+1, mask) )
	//	{
	//		m_Log.Add("[INFO]:CVmsDlg::OnLiveFinal:������ȡʧ��,DC:%d",DC);
	//		TRACE("OnLiveFinal:������ȡʧ�ܣ�\n");
	//	}
	}else{
		mask = VIDEO_YUV12_RAW;
	//	if( ERR_STATE_WRONG == m_player.StartGetStreamData(DC+1, mask) )
	//	{
	//		m_Log.Add("[INFO]:CVmsDlg::OnLiveFinal:������ȡʧ��,DC:%d",DC);
	//		TRACE("OnLiveFinal:������ȡʧ�ܣ�\n");
	//	}
	}

	TRACE("OnLiveFinal:suc:%d\n",DC);

	return 0;
}
LRESULT CVmsDlg::OnGlobalExit( WPARAM wParam, LPARAM lParam )
{
	if (wParam == NVR_TRAP_CLIENT_KICKOUT)
	{
		m_Log.Add("[INFO]:CVmsDlg::OnGlobalExit:��ǰ�û��ѱ�����Աǿ������");
		TRACE("��ǰ�û��ѱ�����Աǿ�����ߣ�"); 
	}
	else if (wParam == NVR_TRAP_CLIENT_LOGINAGAIN)
	{
		m_Log.Add("[INFO]:CVmsDlg::OnGlobalExit:��ǰ�û����������ط���½������ǿ���˳���");

		TRACE("��ǰ�û����������ط���½������ǿ���˳���"); 
	}
	else
	{
		m_Log.Add("[INFO]:CVmsDlg::OnGlobalExit:���������ʧȥ���ӣ������µ�¼����");

		TRACE("���������ʧȥ���ӣ������µ�¼��"); 
	}
	//LogOut();
	m_Logflag = 0;
	return TRUE;
}
int CVmsDlg::CBKeepAliveFail(int CBType, void* p1, void* p2)
{
	::SendMessage(m_wnd, WM_GLOBAL_EXIT, (WPARAM)p1, (LPARAM)p2);
	return 1;
}
int CVmsDlg::ClientCallBackFunc(int CBType, void* p1, void* p2)
{
//����Ϣ����ģʽ����ص�����ԭ�����£�SendMessage�����ȴ�p1��p2��������������
	if (DCNotifyRS == CBType)
	{
		::SendMessage(m_wnd, WM_LIVEPLAY_SETMN, (WPARAM)p1, (LPARAM)p2);
	}
	else if (RSNotifyRet == CBType)
	{
		::SendMessage(m_wnd, WM_LIVE_FINAL, (WPARAM)p1, (LPARAM)p2);
	}
	else if (DCNotifySTOP == CBType)
	{
		::SendMessage(m_wnd, WM_LIVEPLAY_PLAYSTOP, (WPARAM)p1, (LPARAM)p2);
	}
	return 0;
}

int CVmsDlg::StartPull(void *pStream,void* pParams)
{

	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VMS_PARAMS *pVmsParams = &m_devParams[pH264Stream->thisIndex];

	int ret=0;

	TRACE("into CVmsDlg::StartPull %d******************\n",pH264Stream->thisIndex);
//test
//	pH264Stream->h_stream = &ret;
//	return 0;
//endtest

	WaitForSingleObject(m_Lock,INFINITE);

	if(m_Logflag == 0)//first login
//	if(0)//first login
	{
		//init VMS
	//	ret=m_player.SPlayerSetPMode(0); //ʵ��ģʽ

	//	ret=m_player.SPlayerSetchannelNum(16,1);//��������Ϊ1

		int ret = VMS_Init();
		if (ret != SDK_ERROR_SUC)
		{
			m_Log.Add("[INFO]: Stream [%d] StartPull:VMS_Init fail",pH264Stream->thisIndex);
			ReleaseMutex(m_Lock);
			return -1;
		}

		//SDK����ص�
		ret = VMS_RegCallBack(KeepAliveFail,&CBKeepAliveFail);
		if (ret != SDK_ERROR_SUC)
		{		
			VMS_Destory();
			m_Log.Add("[INFO]: Stream [%d] StartPull:VMS_RegCallBack-CBKeepAliveFail fail",pH264Stream->thisIndex);
			ReleaseMutex(m_Lock);
			return -1;
		}

		//�ͻ��˻ص�����ע��
		ret = VMS_RegCallBack(ClientNotify, &ClientCallBackFunc);
		if ( ret != SDK_ERROR_SUC)
		{
			VMS_Destory();
			m_Log.Add("[INFO]: Stream [%d] StartPull:VMS_RegCallBack-ClientCallBackFunc fail",pH264Stream->thisIndex);
			ReleaseMutex(m_Lock);
			return -1;
		}
	//	m_player.Init(); //��ʼ������������Ҫ������sdk
		//end init VMS

		//login
		USES_CONVERSION;

		//������ܴ���
		char logPswd[32];
		memset(logPswd,0,sizeof(logPswd));

		CMD5 md;
		md.MessageDigest((const unsigned char*)pVmsParams->m_passWord,strlen(pVmsParams->m_passWord),(unsigned char*)logPswd);

		Int32 UserPriv = 0;
		//����SDK������½
		//int ret = SDK_ERROR_SUC;
		ret = VMS_Login(inet_addr(pVmsParams->m_proxyIp),htons(pVmsParams->m_proxyPort), pVmsParams->m_userName, logPswd, &UserPriv);
		
		TRACE("ipis %d port is %d name is %s pwd is %s\n",inet_addr(pVmsParams->m_proxyIp), htons(pVmsParams->m_proxyPort), pVmsParams->m_userName, logPswd);

		//��½ʧ�ܴ���
		if (ret != SDK_ERROR_SUC)
		{
			CHAR strBuffer[256] = {0};
			int BufferLen = 256;
			VMS_ErrCHString((SdkErrCode)ret, strBuffer, &BufferLen);
			TRACE("OnBnClickedButton2:VMS_Login fail ret is %d[%d][%s]\n",ret,strBuffer);
			m_Log.Add("[INFO]: Stream [%d] StartPull:VMS_Login fail",pH264Stream->thisIndex);
			ReleaseMutex(m_Lock);
			return -1;
		}
		TRACE("StartPull:VMS_Login suc\n");
		//end login

		m_wnd = GetSafeHwnd();
		
		m_Logflag=1;
	}//end first login
	ReleaseMutex(m_Lock);

	//InitTsFunc(&m_devParams[pH264Stream->thisIndex].m_devInfo.ts,ts2frame);
	//m_devParams[pH264Stream->thisIndex].m_devInfo.ts.pUserData = pH264Stream;

	//play live
	Int64 ec_channel = pVmsParams->m_deviceChn;//(Int64)9007199256192805;	//�����ID������ͨ����

	Int64 dc_channel = pH264Stream->thisIndex;//���ʱ��dc_channel��Ӧ����������ţ��˴���Ϊ1�Ŵ��񲥷�,0-15��Ӧ1-16
	Uint8 main_sub_flag = VIDEO_STREAM_MAIN;//��Ϊֱ������
	Int32 ms = -1;//�鲥����Ϊͨ��ƽ̨Э�̾�����-1Ϊ��Ч
	Uint8 multicast_flga = 0;//��֧���鲥

	TRACE("OnBnClickedButton3:m_CrlLivetype.GetCurSel:%d,ec_channel:%lld,,dc_channel:%lld,main_sub_flag:%d,,ms:%d,multicast_flga:%d\n"
		,0
		,ec_channel
		,dc_channel
		,main_sub_flag
		,ms
		,multicast_flga);
	//����SDK�ӿں���
	ret = VMS_StartMonitor((Uint8)0, ec_channel, dc_channel, main_sub_flag, ms, multicast_flga);

	//ʧ�ܴ���
	if (ret != SDK_ERROR_SUC)
	{
		TRACE("StartPull:����ʵ��ʧ�ܣ�");
		m_Log.Add("[INFO]: Stream [%d] StartPull:VMS_StartMonitor fail",pH264Stream->thisIndex);
		return -1;
	}
	
	TRACE("StartPull:����ʵ��suc��,pH264Stream->thisIndex:%d\n",pH264Stream->thisIndex);
	pH264Stream->h_stream = &ret;
	//end play live

	return 0;
}

int CVmsDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VMS_PARAMS *pVMSParams = &m_devParams[pH264Stream->thisIndex];
//test
//	pH264Stream->h_stream = NULL;
//	return 0;
//endtest

	if (NULL!=pH264Stream->h_stream)
	{
		//ֹͣʵ����ز������ã���Ӧ����ʵ���Ĳ�������
		Uint8 live_type = 0; //����ģʽΪ���
		Int64 ec_channel =(Int64)pVMSParams[pH264Stream->thisIndex].m_deviceChn;//9007199256192805;	//�����ID������ͨ����

		Int64 dc_channel = pH264Stream->thisIndex +1;//���ʱ��dc_channel��Ӧ����������ţ��˴���Ϊ1�Ŵ��񲥷�

		int ret = VMS_StopMonitor(live_type, ec_channel, dc_channel-1);

		//ʧ�ܴ���
		if (ret != SDK_ERROR_SUC)
		{
			TRACE("ֹͣʵ��ʧ�ܣ�");
			m_Log.Add("[INFO]:Stream [%d] ClosePull:VMS_StopMonitorֹͣʵ��ʧ��!",pH264Stream->thisIndex);
			//ʧ��Ҳֹͣ
		//	m_player.Stop(dc_channel);
		}

	}

	pH264Stream->h_stream = NULL;
	
	return 0;
}

