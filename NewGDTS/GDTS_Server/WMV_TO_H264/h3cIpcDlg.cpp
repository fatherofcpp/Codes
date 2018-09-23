// h3cIpcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "h3cIpcDlg.h"
#include "WMV_TO_H264Dlg.h"


extern CWMV_TO_H264Dlg* Main;
static Ch3cIpcDlg* pH3CDlg=NULL;
// Ch3cIpcDlg 对话框

IMPLEMENT_DYNAMIC(Ch3cIpcDlg, CDialog)

Ch3cIpcDlg::Ch3cIpcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ch3cIpcDlg::IDD, pParent)
{
	for(int i=0;i<16;i++)
	{
		memset(&m_h3cIpcParams[i],0,sizeof(DEV_H3CIPC_PARAMS));
	}

	if(0!=loadH3cIPCLib())
	{
		m_Log.Add("[ERR]:loadH3cIPCLib error");
		exit(0);
	}

	ULONG ulRet = ERR_COMMON_SUCCEED;

	ulRet = IMOS_MW_Initiate();
	if(ulRet != ERR_COMMON_SUCCEED)
	{
		m_Log.Add("[INFO]: IMOS_MW_Initiate失败!");
		//ReleaseMutex(m_Locker);
		exit(0);
	}

	//FirstLog=1;
	//m_Locker=CreateMutex(NULL,FALSE,"jc_lock");

}

Ch3cIpcDlg::~Ch3cIpcDlg()
{
	//CloseHandle(m_Locker);//=CreateMutex(NULL,FALSE,"jc_lock");

}

void Ch3cIpcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Ch3cIpcDlg, CDialog)
END_MESSAGE_MAP()


// Ch3cIpcDlg 消息处理程序
//add 

static void __stdcall DADA_CallBack(LONG lChannelHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, DWORD dwUser)
{
	char s[256];
	memset(s,0,sizeof(s));

    switch (dwDataType)
    {
		case 0x00000001://媒体流数据
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("媒体流数据"));
		}
		break;
		case 0x00000002://解码前的视频
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("解码前的视频"));
			Svl_COMObject::COM_VideoFrame * pVideoFrame = (Svl_COMObject::COM_VideoFrame *)param;
			CH264Stream *pH264 = (CH264Stream *)dwUser;

			pH264->b_PullLive=0;
			TRACE("coder is %d----%d\n",pVideoFrame->height,pVideoFrame->width);

			HANDLE handle;
			MyAvFrame *m_Frame=NULL;
			m_Frame = new MyAvFrame(dwBufSize+1,1);
			if (NULL == m_Frame)
			{
				m_Log.Add("new MyAvFrame的时候内存分配错误！");
				return ;
			}
			if (NULL == m_Frame->p_data)
			{
				m_Log.Add("分配到的MyAvFrame里的p_data指针为空！");
				return ;
			}
			memcpy(m_Frame->p_data+1,pBuffer,dwBufSize);
			pH264->m_streamlock.Lock();
			pH264->p_EncodeArray.Add(m_Frame);
			pH264->m_streamlock.Unlock();

		}//end case 0x00000002
		break;
		case 0x00000004://解码前的音频
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("解码前的音频"));
			Svl_COMObject::COM_VideoFrame *pPCM = (Svl_COMObject::COM_VideoFrame *)param;
		}
		break;
		case 0x00000008://解码后的视频
		{
			Svl_COMObject::COM_PICTURE * p_Picture = (Svl_COMObject::COM_PICTURE *)param;

			CH264Stream *pH264 = (CH264Stream *)dwUser;

			pH264->b_PullLive=0;

			TRACE("decoder is %d:%d----%d\n",p_Picture->uVideoWidth,p_Picture->uVideoHeight,p_Picture->bYuvOrRgb);

			if (0 == p_Picture->bYuvOrRgb)
			{
				//m_buff.bYUV=false;
				return ;
			}
			else
			{
				//m_buff.bYUV=true;
			}
			if(pH264 == NULL)
				return;

			MyAvFrame *m_Frame=NULL;
			m_Frame = new MyAvFrame(p_Picture->uVideoWidth,p_Picture->uVideoHeight);
			if (NULL == m_Frame)
			{
				m_Log.Add("new MyAvFrame的时候内存分配错误！");
				return ;
			}
			if (NULL == m_Frame->p_data)
			{
				m_Log.Add("分配到的MyAvFrame里的p_data指针为空！");
				return ;
			}
			memcpy(m_Frame->p_data,pBuffer,dwBufSize);
			//memcpy(m_Frame->p_data,(char*)p_Picture->data[0],p_Picture->uVideoWidth*p_Picture->uVideoHeight);
			//memcpy(&m_Frame->p_data[p_Picture->uVideoWidth*p_Picture->uVideoHeight],(char*)p_Picture->data[1],p_Picture->uVideoWidth*p_Picture->uVideoHeight/4);
			//memcpy(&m_Frame->p_data[p_Picture->uVideoWidth*p_Picture->uVideoHeight+(p_Picture->uVideoWidth*p_Picture->uVideoHeight)/4],(char*)p_Picture->data[2],p_Picture->uVideoWidth*p_Picture->uVideoHeight/4);

			pH264->m_streamlock.Lock();
			pH264->p_YuvArray.Add(m_Frame);
			pH264->m_streamlock.Unlock();
		}
		break;
		case 0x00000010://解码后的音频
		{
	//		 thisdlg->GetDlgItem(IDC_CONTEXT)->SetWindowText(LPCTSTR("解码后的音频"));
			Svl_COMObject::COM_VideoFrame *pPCM = (Svl_COMObject::COM_VideoFrame *)param;
		}
		break;
	}//end switch (dwDataType)
    return;
}


// Ch3cIpcDlg 消息处理程序
/*************************************************************************************
函数名称：CheckExit
函数描述: check if video is online
函数调用：CWMV_TO_H264Dlg::ChangeCh(CString CamID, int CH,CString pubserver)
全局变量：
参数1：Camera Id
返 回 值:
创建时间：2014-4-21
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-4-21     V1.0        jicheng         创建
**************************************************************************************/
CString Ch3cIpcDlg::CheckExit(CString str)
{
	CString UrlStr="",CamID = str;

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//如果之前已经被暂停,则直接返回当前无切换，让BMS下发切换命令。
				m_Log.Add("视频:%s 已经停止播放，不存在!",CamID);
				//return "";
				continue;

			}

			UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);
			m_Log.Add("视频:%s 存在!URL:%s",CamID,UrlStr);


			m_h264[i]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。

			return UrlStr;
		}


	}
	m_Log.Add("视频:%s 不存在!",CamID);

	return UrlStr;
}

/*************************************************************************************
函数名称：ChangeChannel
函数描述: change video to channel
函数调用：CWMV_TO_H264Dlg::ChangeCh(CString CamID, int CH,CString pubserver)
全局变量：
参数1：CamId
参数2：way to change to
参数3：
返 回 值:
创建时间：2014-4-21
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-4-21     V1.0        jicheng         创建
**************************************************************************************/
CString Ch3cIpcDlg::ChangeChannel(CString sId,int CH,CString str)
{
	int ret=0;
	CString UrlStr="";

	//m_h264[CH]->m_Info.s_DevID=sId;
	//tmpParams.m_dstStream = CH;

	SetParams(m_h264[CH],&m_h3cIpcParams[CH]);
	m_Log.Add("视频准备切换%u:%s",GetTickCount(),m_h264[CH]->m_Info.s_DevID);


//end close and pull
	UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
	m_Log.Add("频切换成功%u:%s",GetTickCount(),UrlStr);
	m_h264[CH]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。且持续时间从现在开始算起。
	return UrlStr;

}


#if 0
<h3c> 
<type>ipc</type>//设备类型
<ver>1120p25</ver>//版本
<ip>192.168.0.5</ip>//IP地址  //
<port>8000</port>//端口
<user>admin</user>//用户名
<passwd>123456</passwd>//密码
<channel>123</channel>//默认播放通道
</h3c>

<command>
<serverip>192.168.100.114</serverip>
<company>2</company>
<h3c>
<type>ipc</type>
<ver>2</ver>
<ip>10.121.0.1</ip>
<port>19999</port>
<user>xgs</user>
<passwd>xgs88888</passwd>
<channel>9007199256192805</channel>
<pipe>2</pipe>
</h3c>
<ch>0</ch>
</command>
#endif

#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20
/*************************************************************************************
函数名称：ParseRequest
函数描述: parse post information
函数调用：notify_handler
全局变量：
参数：post data
参数：
返 回 值:
创建时间：2014-4-21
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-4-21     V1.0        jicheng         创建
**************************************************************************************/
int Ch3cIpcDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	DEV_H3CIPC_PARAMS  tmpParams;

	CString CH;
	CString CamId;
	//CString tmpSource,CH,tmpType,tmpVer,tmpIp, tmpPort, pubserverip;
	//tmpSource = m_Info.s_DevID;
	//tmpDstip = m_Info.s_PubServer;

	memset(&tmpParams,0,sizeof(DEV_H3CIPC_PARAMS));

	rt = f_xmlparse_init(&xml, (const char*)pXmlBuf);
	if (rt != 0)
	{
		m_Log.Add("xml解析错误，xml结构体初始化失败！");
		rt = 400;
		goto leaveOut;
	}

	if(NULL == xml.root)
	{
		m_Log.Add("xml解析错误，无效的XML文件！");
		rt = 400;
		goto leaveOut;
	}


	if (strcmp(xml.root->name, "command") != 0)
	{
		m_Log.Add("xml解析错误，未能找到Command节点!");
		rt = 400;
		goto leave;
	}

	root = xml.root;
	node = f_xmlnode_sch_elem_ext(root, "serverip", MAX_LEN);
	if(node == NULL)
	{
		m_Log.Add("xml解析错误，未能找到serverip节点!\n");
		rt = 400;
		goto leave;
	}
	else
	{
		strcpy(tmpParams.m_dstIp, node->data);
		//m_Log.Add("解析到的serverip: %s",serverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=2)
	{
		m_Log.Add("xml解析错误，未能找到company节点,或者company节点内容不是2!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "h3c");
	if(subroot==NULL)
	{
		m_Log.Add("xml解析错误，未能找到h3c节点!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("开始解析h3c的XML!");
		node = f_xmlnode_sch_elem_ext(subroot, "type", MAX_LEN);
		if (node != NULL)
		{
			//tmpType = node->data;
			//m_Log.Add("解析到的type：%s",tmpType);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到type节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ver", MAX_LEN);
		if (node != NULL)
		{
			//tmpVer=node->data;
			//m_Log.Add("解析到的ver：%s",tmpVer);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到ver节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpParams.m_proxyIp,node->data);
			//m_Log.Add("解析到的ip：%s",tmpIp);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到ip节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "port", MAX_LEN);
		if (node != NULL)
		{
			tmpParams.m_proxyPort=atoi(node->data);
			//m_Log.Add("解析到的port：%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到port节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "user", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpParams.m_userName, node->data);
			//m_Log.Add("解析到的user：%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到user节点!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			strcpy(tmpParams.m_passWord, node->data);
			//m_Log.Add("解析到的passwd：%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到passwd节点!");
			rt = 400;
			goto leave;
		}
		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			tmpParams.m_deviceId = atoi(node->data);
			//m_Log.Add("解析到的channel：%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到channel节点!");
			rt = 400;
			goto leave;
		}
	}
#if 1
	chNode = f_xmlnode_sch_elem_ext(root, "ch", MAX_LEN);//
	if (chNode != NULL)
	{
		CamId.Format("%d",tmpParams.m_deviceId);
		if(0 == chNode->data_siz)
		{
			m_Log.Add("解析到的ch为空值!");		
			m_Log.Add("查找视频：%s是否存在！",tmpParams.m_dstPub);
			WaitForSingleObject(Main->m_Lock,INFINITE);
			
			resultStr=	CheckExit(CamId);
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

			if(atoi(CH)<0)
			{
				rt=404;
				goto leave;
			}
#if 1
			m_h264[atoi(CH)]->m_Info.s_DevID = CamId;
			m_h264[atoi(CH)]->m_Info.s_PubServer = tmpParams.m_dstIp;
			m_Log.Add("解析到的ch: %s",CH);
			m_Log.Add("切换%s,至通道%d",CamId,atoi(CH));

			{
				strcpy(m_h3cIpcParams[atoi(CH)].m_proxyIp,tmpParams.m_proxyIp);
				m_h3cIpcParams[atoi(CH)].m_proxyPort = tmpParams.m_proxyPort;
				strcpy(m_h3cIpcParams[atoi(CH)].m_userName,tmpParams.m_userName);
				strcpy(m_h3cIpcParams[atoi(CH)].m_passWord,tmpParams.m_passWord);
				m_h3cIpcParams[atoi(CH)].m_deviceId = tmpParams.m_deviceId;

			}

			WaitForSingleObject(Main->m_Lock,INFINITE);
			resultStr=ChangeChannel(CamId,atoi(CH),tmpParams.m_dstIp);
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
		m_Log.Add("xml解析错误，ch节点的值为空!");
		rt = 400;
		goto leave;
	}
	//m_Log.Add("解析到通道%d: 参数为:Serverip:%s Company:%s DevType:%s Version:%s IP:%s Port:%s Url:%s\n",atoi(CH), pubserverip,"microvideo",tmpType, tmpVer, tmpIp, tmpPort, tmpSource);//张世俊增加
	//TRACE("DevType:%s Version:%s IP:%s Port:%s Url:%s", tmpType, tmpVer, tmpIp, tmpPort, tmpSource);
#endif


leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}


int Ch3cIpcDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("ProxyIp%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cIpcParams[nIndex].m_proxyIp,dst);


	tmpStr.Format("ProxyPort%d",nIndex);
	ZeroMemory(dst,255);
	m_h3cIpcParams[nIndex].m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_h3cIpcParams.m_proxyPort:%d\n",m_h3cIpcParams[nIndex].m_proxyPort);


	tmpStr.Format("UserName%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cIpcParams[nIndex].m_userName,dst);
	TRACE("m_h3cIpcParams[nIndex].m_userName:%s\n",m_h3cIpcParams[nIndex].m_userName);


	tmpStr.Format("PassWd%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cIpcParams[nIndex].m_passWord,dst);
	TRACE("m_h3cIpcParams[nIndex].m_passWord:%s\n",m_h3cIpcParams[nIndex].m_passWord);

	//InitProxyServer(NULL);

	m_Log.Add("初始化通道的配置: 参数为:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		m_h3cIpcParams[nIndex].m_proxyIp,m_h3cIpcParams[nIndex].m_proxyPort,m_h3cIpcParams[nIndex].m_userName,
		m_h3cIpcParams[nIndex].m_passWord);
	
	tmpStr.Format("DeviceId%d",nIndex);
	ZeroMemory(dst,255);

	m_h3cIpcParams[nIndex].m_deviceId=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	//strcpy(m_h3cIpcParams[nIndex].m_deviceName,dst);

	tmpStr.Format("Dstip%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cIpcParams[nIndex].m_dstIp,dst);

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cIpcParams[nIndex].m_dstPub,dst);

	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_h3cIpcParams[nIndex].m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);



	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_H3CIPC_DEV分配内存时出现错误！",nIndex);
	}

	m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);


	m_h264[nIndex]->thisIndex = SourceCount;
	m_h264[nIndex]->m_Info.s_PubServer=m_h3cIpcParams[nIndex].m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",SourceCount);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_h3cIpcParams[nIndex].m_dstPub,SourceCount);
	m_h264[nIndex]->m_devType = DEF_H3CIPC_DEV;

	SetParams(m_h264[nIndex],&m_h3cIpcParams[nIndex]);
	//StartPull(m_h264[nIndex],&m_h3cIpcParams[nIndex]);



	return 0;
}

//对于其它的没有从INI配置文件中读取到的路，采用以下方式来处理。
//该处理过程只是创建实例，但该实例创建后并不马上参加运行
int Ch3cIpcDlg::SetDefaultParams(void * pParams)
{
	if(SourceCount>=g_maxLiveNum)//张世俊增加，补全到16路
		return 0;

	for(;SourceCount<g_maxLiveNum;SourceCount++)
	{
		m_h264[SourceCount]=new CH264Stream;
		if(NULL==m_h264[SourceCount])
		{
			m_Log.Add("m_h264[%d]=new CH264Stream补全16的时候分配内存时出现错误！",SourceCount);
		}

		m_h264[SourceCount]->b_Encode	= (g_bUseTc==1?1:0);

		m_h264[SourceCount]->thisIndex = SourceCount;
		m_h264[SourceCount]->m_Info.s_PubServer=m_h264[0]->m_Info.s_PubServer;
		m_h264[SourceCount]->m_devType = DEF_H3CIPC_DEV;
//
		m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_h3cIpcParams[0].m_dstPub,SourceCount);
	}

	return 0;
}

int Ch3cIpcDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3CIPC_PARAMS* pH3cVM=&m_h3cIpcParams[pH264Stream->thisIndex];
	int ret=0;

	pH264Stream->m_Info.s_DevID.Format("%d",pH3cVM->m_deviceId);
	pH264Stream->m_devType = DEF_H3CIPC_DEV;

	if (false == pH264Stream->b_start)
	{
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);


	return ret;
}

void __stdcall decodeCALLBACK(IN ULONG ulStreamHandle, IN const IMOS_MW_PICTURE_DATA_S *pstPictureData, IN LONG lUserParam, IN LONG lReserved)
{
#if 1
//	TRACE("ulStreamHandle:%ld--ulPicWidth:%d ulPicHeight:%d,lUserParam:%d,lReserved:%d\n",
//		ulStreamHandle,pstPictureData->ulPicWidth,pstPictureData->ulPicHeight,lUserParam,lReserved);

//	TRACE("ulLineSize[0]:%d,ulLineSize[1]:%d,ulLineSize[2]:%d,ulLineSize[3]:%d\n",
//		pstPictureData->ulLineSize[0],pstPictureData->ulLineSize[1],pstPictureData->ulLineSize[2],pstPictureData->ulLineSize[3]);
	if(lUserParam<0 || lUserParam>15)
		return;
	CH264Stream*pH264=m_h264[lUserParam];
	const IMOS_MW_PICTURE_DATA_S *ptmp=pstPictureData;


	pH264->b_PullLive=0;

	if(pH264->p_YuvArray.GetSize()<60)
	{
		if (NULL ==ptmp->pucData)
		{
			return;
		}
//		if (ptmp->ulLineSize != (ptmp->ulPicWidth*ptmp->ulPicHeight*3/2))
//		{
//			return;
//		}
		MyAvFrame *m_Frame=new MyAvFrame(ptmp->ulPicWidth,ptmp->ulPicHeight);
		if (NULL == m_Frame)
		{
			m_Log.Add("[ERR]:decodeCALLBACK函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");
			return;
		}
		if (NULL == m_Frame->p_data)
		{
			return;
		}

		int len3=ptmp->ulPicWidth*ptmp->ulPicHeight;

		memcpy(m_Frame->p_data,ptmp->pucData[0],len3);
		memcpy(&m_Frame->p_data[len3],ptmp->pucData[1],len3/4);
		memcpy(&m_Frame->p_data[len3*5/4],ptmp->pucData[2],len3/4);

		pH264->m_streamlock.Lock();
		pH264->p_YuvArray.Add(m_Frame);
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("数组过大，%d\n",pH264->p_YuvArray.GetSize());
	}
#endif
}

// 启动拉流函数
int Ch3cIpcDlg::StartPull(void *pStream,void* pParams)
{
	int ret=0;
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3CIPC_PARAMS* pH3cVM=&m_h3cIpcParams[pH264Stream->thisIndex];

	M_BOOL b_Ret=FALSE;

	ULONG ulRet = ERR_COMMON_SUCCEED;
#if 0
	WaitForSingleObject(m_Locker,INFINITE);

	if(0)//FirstLog == 1)
	{
		 ulRet = IMOS_MW_Initiate();
		if(ulRet != ERR_COMMON_SUCCEED)
		{
			m_Log.Add("[INFO]:Stream [%d] StartPull:IMOS_MW_Initiate失败!",pH264Stream->thisIndex);
			ReleaseMutex(m_Locker);
			return -1;
		}
		FirstLog=0;
	}
	ReleaseMutex(m_Locker);

#endif
	ulRet = IMOS_MW_Login(pH3cVM->m_userName, 
							pH3cVM->m_passWord,pH3cVM->m_proxyIp,
							0,pH3cVM->m_ipcWay.m_szSDKUserID);
	TRACE("StartPull[%d]:IMOS_MW_Login,ulRet:%d,name:%s,paswd:%s,Ip:%s,userId:%s\n",pH264Stream->thisIndex,ulRet,pH3cVM->m_userName,pH3cVM->m_passWord,pH3cVM->m_proxyIp,pH3cVM->m_ipcWay.m_szSDKUserID);
	if (ERR_COMMON_SUCCEED != ulRet)
	{
		m_Log.Add("[INFO]:Stream [%d] StartPull:IMOS_MW_Initiate失败!",pH264Stream->thisIndex);
		return -1;
	}

//	将成功登录返回的用户ID 反馈给主对话框类，在通知各个子控件 
//  OWNER_SENDMESSAGE(WM_LOGIN, (WPARAM)m_szSDKUserID, 0);
//	OnOK();

//play
	/*
	调用SDK 建流接口，依赖于SDK login接口返回的用户ID,
	建立Tcp码流，配置低配显示参数（防止外场黑屏）；
	
	若建流失败，则停流接口继续灰显；否则建流接口灰显；
	*/
	CHAR szIPAddr[IMOS_MW_IPADDR_LEN] = {0};
	IMOS_MW_DISPLAY_PARAM_S stDisplayParam = {0};
	
	if (TRUE == pH3cVM->m_ipcWay.m_bStreamOn)
	{
	//	return -1;
	}

	stDisplayParam.ulPixelFormat = IMOS_MW_RENDER_MODE_DDRAW_NORMAL;
	stDisplayParam.ulRenderMode = IMOS_MW_PIXEL_FORMAT_YUV420;//IMOS_MW_PIXEL_FORMAT_RGB32;

	ulRet = IMOS_MW_StartMediaStream(pH3cVM->m_ipcWay.m_szSDKUserID, 0,	IMOS_MW_STREAM_ID_MAIN, IMOS_MW_TRANSFER_MODE_RTP_TCP, NULL, 0, BOOL_TRUE, &pH3cVM->m_ipcWay.m_ulStreamHd);
	if(ulRet == ERR_COMMON_SUCCEED)
	{
	//	pH3cVM->m_ipcWay.m_bStreamOn = TRUE;
	}
	else
	{
		m_Log.Add("[INFO]:Stream [%d] StartPull:IMOS_MW_StartMediaStream失败!",pH264Stream->thisIndex);
		return -1;
	}
	
	ulRet=IMOS_MW_SetDisplayParam(&stDisplayParam);
	if(ulRet!=ERR_COMMON_SUCCEED)
	{
		return -1;
	}

//	ulRet =	IMOS_MW_StartPlay((ULONG)(m_VideoPlayForm.m_wndVideoWnd.m_hWnd), m_ulStreamHd);
	ulRet =	IMOS_MW_StartPlay((ULONG)(pH264Stream->thisIndex), pH3cVM->m_ipcWay.m_ulStreamHd);
	if(ulRet == ERR_COMMON_SUCCEED)
	{
	//	m_VideoPlayForm.UpdataPlayStatus(PLAY_STATUS_PLAYING);
	}
	else
	{
		m_Log.Add("[INFO]:Stream [%d] StartPull:IMOS_MW_StartPlay失败!",pH264Stream->thisIndex);
		return -1;
	}

	/* 对于需要设置码流回调的情况， 所有回调函数的设置均在IMOS_MW_StartPlay之后调用 
	   	IMOS_MW_SetOrgMediaDataCallback、
		IMOS_MW_SetParseVideoDataCallback、
		IMOS_MW_SetDecodedVideoDataCallback 
	*/
	ulRet=IMOS_MW_SetDecodedVideoDataCallback(pH3cVM->m_ipcWay.m_ulStreamHd,decodeCALLBACK,0,pH264Stream->thisIndex);
	if(ulRet!=ERR_COMMON_SUCCEED)
	{
		m_Log.Add("[INFO]:Stream [%d] StartPull:IMOS_MW_SetDecodedVideoDataCallback失败!",pH264Stream->thisIndex);
		return -1;
	}
//end play

	pH264Stream->h_stream=&b_Ret;

	return 0;
}

int Ch3cIpcDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3CIPC_PARAMS* pH3cVM=&m_h3cIpcParams[pH264Stream->thisIndex];

	ULONG ulRet = -1;
	ulRet = IMOS_MW_StopPlay((ULONG)(pH264Stream->thisIndex));
	if(ulRet != ERR_COMMON_SUCCEED)
	{
		m_Log.Add("[INFO]:Stream [%d] ClosePull:IMOS_MW_StopPlay失败!",pH264Stream->thisIndex);
	}

	ulRet = IMOS_MW_StopMediaStream(pH3cVM->m_ipcWay.m_szSDKUserID,pH3cVM->m_ipcWay.m_ulStreamHd);
	if(ulRet == ERR_COMMON_SUCCEED)
	{
		//pH3cVM->m_ipcWay.m_bStreamOn = FALSE;
	}
	else
	{
		m_Log.Add("[INFO]:Stream [%d] ClosePull:IMOS_MW_StopMediaStream失败!",pH264Stream->thisIndex);
	}
	 ulRet = IMOS_MW_Logout(pH3cVM->m_ipcWay.m_szSDKUserID);
	if(ulRet != ERR_COMMON_SUCCEED)
	{
		m_Log.Add("[INFO]:Stream [%d] ClosePull:IMOS_MW_Logout失败!",pH264Stream->thisIndex);
	}	
	// ulRet = IMOS_MW_Cleanup();
	//if(ulRet != ERR_COMMON_SUCCEED)
	//{
	//	m_Log.Add("[INFO]:Stream [%d] ClosePull:IMOS_MW_Cleanup失败!",pH264Stream->thisIndex);
	//}

	pH264Stream->h_stream=NULL;

	return 0;
}

//end add