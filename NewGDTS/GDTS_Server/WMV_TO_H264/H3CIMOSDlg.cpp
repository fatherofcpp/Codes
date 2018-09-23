// H3CIMOSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "H3CIMOSDlg.h"
#include "WMV_TO_H264Dlg.h"

extern CWMV_TO_H264Dlg* Main;
static CH3CIMOSDlg* pH3CDlg=NULL;
// CH3CIMOSDlg 对话框

IMPLEMENT_DYNAMIC(CH3CIMOSDlg, CDialog)

CH3CIMOSDlg::CH3CIMOSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CH3CIMOSDlg::IDD, pParent)
{

	ULONG ulRet;

	for(int i=0;i<16;i++)
	{
		memset(&m_h3cParams[i],0,sizeof(DEV_H3C_PARAMS));
	}

	if(0 != loadH3cLib())
	{
		m_Log.Add("[ERR]:loadH3cIMOSLib error");
		exit(0);
	}


	ulRet = IMOS_Initiate("N/A", 8800, BOOL_TRUE, BOOL_TRUE);//IMOS_Initiate(szServerAdd, 8800, BOOL_TRUE, BOOL_TRUE);
	if (ERR_COMMON_SUCCEED != ulRet)
	{
		//log
		m_Log.Add("IMOS SDK初始化");
		return;
	}

}

CH3CIMOSDlg::~CH3CIMOSDlg()
{
}

void CH3CIMOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CH3CIMOSDlg, CDialog)
END_MESSAGE_MAP()


// CH3CIMOSDlg 消息处理程序

// CH3CIMOSDlg 消息处理程序
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
CString CH3CIMOSDlg::CheckExit(CString str)
{
	CString UrlStr="",CamID = str;

	for (int i=0;i<g_maxLiveNum && i<SourceCount;i++)
	{

		if (CamID ==m_h264[i]->m_Info.s_DevID)
		{
			if(1 == m_h264[i]->b_stop)
			{//如果之前已经被暂停,则直接返回当前无切换，让BMS下发切换命令。
				m_Log.Add("[CheckExit]视频:%s 已经停止播放，不存在!",CamID);
				//return "";
				continue;

			}
			if("mss" == m_h264[i]->m_Info.s_PubType)
			{//wmv
				UrlStr.Format("200http://%s:%d/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);
			}else
			{
				UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[i]->m_Info.s_PubServer,g_rtmpPort,m_h264[i]->m_Info.s_PubName);
			}
			m_Log.Add("[CheckExit]视频:%s 存在!URL:%s",CamID,UrlStr);


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
CString CH3CIMOSDlg::ChangeChannel(CString sId,int CH,CString str)
{
	int ret=0;
	CString UrlStr="";

	//m_h264[CH]->m_Info.s_DevID=sId;
	//tmpParams.m_dstStream = CH;

	SetParams(m_h264[CH],&m_h3cParams[CH]);
	m_Log.Add("[ChangeChannel]视频准备切换%u:%s",GetTickCount(),m_h264[CH]->m_Info.s_DevID);


//end close and pull

	//add wmv
	if("mss" == m_h264[CH]->m_Info.s_PubType)
	{//wmv
		UrlStr.Format("200http://%s:%d/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
	}//end add wmv
	else
	{
		UrlStr.Format("200rtmp://%s:%d/live/%s",m_h264[CH]->m_Info.s_PubServer,g_rtmpPort,m_h264[CH]->m_Info.s_PubName);
	}
	m_Log.Add("[ChangeChannel]频切换成功%u:%s",GetTickCount(),UrlStr);
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
<type>imos</type>
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
int CH3CIMOSDlg::ParseRequest(char * pXmlBuf,CString *str)
{
	int rt;
	CString resultStr="",tmpStr,serverStr;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	DEV_H3C_PARAMS  tmpParams;

	CString CH;
	CString CamId;
	//CString tmpSource,CH,tmpType,tmpVer,tmpIp, tmpPort, pubserverip;
	//tmpSource = m_Info.s_DevID;
	//tmpDstip = m_Info.s_PubServer;

	memset(&tmpParams,0,sizeof(DEV_H3C_PARAMS));

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
			//m_Log.Add("xml解析错误，未能找到type节点!");
			//rt = 400;
			//goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "ver", MAX_LEN);
		if (node != NULL)
		{
			//tmpVer=node->data;
			//m_Log.Add("解析到的ver：%s",tmpVer);
		}
		else
		{
			//m_Log.Add("xml解析错误，未能找到ver节点!");
			//rt = 400;
			//goto leave;
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
			strcpy(tmpParams.m_deviceId ,node->data);
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
		CamId.Format("%s",tmpParams.m_deviceId);
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
			m_Log.Add("[ParseRequest]解析到的ch: %s",CH);
			m_Log.Add("[ParseRequest]切换%s,至通道%d",CamId,atoi(CH));

			{
				strcpy(m_h3cParams[atoi(CH)].m_proxyIp,tmpParams.m_proxyIp);
				m_h3cParams[atoi(CH)].m_proxyPort = tmpParams.m_proxyPort;
				strcpy(m_h3cParams[atoi(CH)].m_userName,tmpParams.m_userName);
				strcpy(m_h3cParams[atoi(CH)].m_passWord,tmpParams.m_passWord);
				strcpy(m_h3cParams[atoi(CH)].m_deviceId ,tmpParams.m_deviceId);

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


int CH3CIMOSDlg::GetInforFromIni4Company(void * pThisDir,int nIndex)
{
	CString thisDir;
	thisDir.Format("%s",(char*)pThisDir);

	char tmp[255],dst[255];
	CString tmpStr;


	tmpStr.Format("ProxyIp%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cParams[nIndex].m_proxyIp,dst);


	tmpStr.Format("ProxyPort%d",nIndex);
	ZeroMemory(dst,255);
	m_h3cParams[nIndex].m_proxyPort=GetPrivateProfileInt("common",tmpStr,-1,thisDir);
	TRACE("m_h3cParams.m_proxyPort:%d\n",m_h3cParams[nIndex].m_proxyPort);


	tmpStr.Format("UserName%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cParams[nIndex].m_userName,dst);
	TRACE("m_h3cParams[nIndex].m_userName:%s\n",m_h3cParams[nIndex].m_userName);


	tmpStr.Format("PassWd%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cParams[nIndex].m_passWord,dst);
	TRACE("m_h3cParams[nIndex].m_passWord:%s\n",m_h3cParams[nIndex].m_passWord);

	//InitProxyServer(NULL);

	m_Log.Add("初始化通道的配置: 参数为:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
		m_h3cParams[nIndex].m_proxyIp,m_h3cParams[nIndex].m_proxyPort,m_h3cParams[nIndex].m_userName,
		m_h3cParams[nIndex].m_passWord);
	
	tmpStr.Format("DeviceId%d",nIndex);
	ZeroMemory(dst,255);

	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cParams[nIndex].m_deviceId,dst);

	tmpStr.Format("Dstip%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cParams[nIndex].m_dstIp,dst);

	tmpStr.Format("Dstpub%d",nIndex);
	ZeroMemory(dst,255);
	GetPrivateProfileString("common",tmpStr,"",dst,255,thisDir);
	strcpy(m_h3cParams[nIndex].m_dstPub,dst);


	tmpStr.Format("Dststream%d",nIndex);
	ZeroMemory(dst,255);
	m_h3cParams[nIndex].m_dstStream=GetPrivateProfileInt("common",tmpStr,-1,thisDir);



	m_h264[nIndex]=new CH264Stream;
	if(NULL==m_h264[nIndex])
	{
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_H3CIPC_DEV分配内存时出现错误！",nIndex);
	}

	m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);


	m_h264[nIndex]->thisIndex = SourceCount;
	m_h264[nIndex]->m_Info.s_PubServer=m_h3cParams[nIndex].m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",SourceCount);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_h3cParams[nIndex].m_dstPub,SourceCount);
	m_h264[nIndex]->m_devType = DEF_H3CIMOS_DEV;

	SetParams(m_h264[nIndex],&m_h3cParams[nIndex]);
	//StartPull(m_h264[nIndex],&m_h3cParams[nIndex]);



	return 0;
}

//对于其它的没有从INI配置文件中读取到的路，采用以下方式来处理。
//该处理过程只是创建实例，但该实例创建后并不马上参加运行
int CH3CIMOSDlg::SetDefaultParams(void * pParams)
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
		m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_h3cParams[0].m_dstPub,SourceCount);
	}

	return 0;
}

int CH3CIMOSDlg::SetParams(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3C_PARAMS* pH3cVM=&m_h3cParams[pH264Stream->thisIndex];
	int ret=0;

	pH264Stream->m_Info.s_DevID.Format("%s",pH3cVM->m_deviceId);
	pH264Stream->m_devType = DEF_H3CIMOS_DEV;

	if (false == pH264Stream->b_start)
	{
		pH264Stream->StartPush(DEFAULTWIDTH,DEFAULTHEIGHT);
		pH264Stream->b_start=true;
	}

	ret = pH264Stream->StartPull(pStream,pParams);


	return ret;
}


VOID STDCALL myCallYV_1(IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo,
                                  IN const CHAR *pcChannelCode,
                                  IN const XP_PICTURE_DATA_S *pPictureData,
                                  IN LONG lUserParam, 
                                  IN LONG lReserved)
{

   	CH264Stream* pH264= m_h264[lUserParam];

	if(pH264 == NULL)
		return;
		
	pH264->b_PullLive=0;

	if(pH264->p_YuvArray.GetSize()<60)
	{

		MyAvFrame *m_Frame=NULL;
		//if(lFrameLen<(352*288*3/2+1))//由于无法判断其大小，估先设置其当前大小作为该次数据量大小
		//	m_Frame = new MyAvFrame(352,288);//默认设置大小 CIF标准
		//else
		m_Frame = new MyAvFrame(pPictureData->ulPicWidth,pPictureData->ulPicHeight);//暂无法判断出视频大小

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
		memcpy(m_Frame->p_data,pPictureData->pucData[0],pPictureData->ulPicWidth*pPictureData->ulPicHeight);
		memcpy(m_Frame->p_data+pPictureData->ulPicWidth*pPictureData->ulPicHeight,pPictureData->pucData[1],pPictureData->ulPicWidth*pPictureData->ulPicHeight/4);
		memcpy(m_Frame->p_data+pPictureData->ulPicWidth*pPictureData->ulPicHeight*5/4,pPictureData->pucData[2],pPictureData->ulPicWidth*pPictureData->ulPicHeight/4);

		pH264->m_streamlock.Lock();
		pH264->p_YuvArray.Add(m_Frame);//视频数据存进待编码数据缓冲区CArray<MyAvFrame*,MyAvFrame*>
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("数组过大，%d\n",pH264->p_YuvArray.GetSize());
	}

    return;
}


VOID STDCALL myCallH264_1(IN const USER_LOGIN_ID_INFO_S *pstUserLoginIDInfo,
                                                         IN const CHAR *pcChannelCode,
                                                         IN const XP_PARSE_VIDEO_DATA_S *pstParseVideoData,
                                                         IN LONG lUserParam, 
                                                         IN LONG lReserved)
{
	//TRACE("timestamp:%lld length:%d height:%d width:%d  user:%d\n",pstParseVideoData->dlTimeStamp,pstParseVideoData->ulDataLen,pstParseVideoData->ulHeight,pstParseVideoData->ulWidth,lUserParam);

	//static FILE *pf=NULL;
	//if(pf == NULL)
	//	pf = fopen("test.yuv","wb");
	//fwrite(pstParseVideoData->pucData,1,pstParseVideoData->ulDataLen,pf);

	CH264Stream* pH264= m_h264[lUserParam];

	if(pH264 == NULL)
		return;
		
	pH264->b_PullLive=0;

	if(pH264->p_EncodeArray.GetSize()<200)
	{
		if (NULL ==pstParseVideoData->pucData)
		{
			m_Log.Add("视频数据为空！");

			return ;
		}
		if (0 == pstParseVideoData->ulDataLen )
		{
			m_Log.Add("视频数据为空！");

			return ;
		}

		MyAvFrame *m_Frame=NULL;
		//if(lFrameLen<(352*288*3/2+1))//由于无法判断其大小，估先设置其当前大小作为该次数据量大小
		//	m_Frame = new MyAvFrame(352,288);//默认设置大小 CIF标准
		//else
		m_Frame = new MyAvFrame(pstParseVideoData->ulDataLen+1,1);//暂无法判断出视频大小


		if (NULL == m_Frame)
		{
			m_Log.Add("new MyAvFrame的时候内存分配错误！");

			return ;
		}
		//add  季成
		if(0 == pH264->curTime)
		{
			pH264->curTime = pstParseVideoData->dlTimeStamp;//gettick
			m_Frame->i_time = 0;
			//		TRACE("input %d\n",m_Frame->i_time);
		}
		else
		{

			m_Frame->i_time = (pstParseVideoData->dlTimeStamp-pH264->curTime);
			pH264->curTime = pstParseVideoData->dlTimeStamp;
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
			m_Log.Add("分配到的MyAvFrame里的p_data指针为空！");

			return ;
		}
		memcpy(m_Frame->p_data+1,pstParseVideoData->pucData,pstParseVideoData->ulDataLen);//new MyAvFrame(352,288)<--byteVideoData

		pH264->m_streamlock.Lock();
		pH264->p_EncodeArray.Add(m_Frame);//视频数据存进待编码数据缓冲区CArray<MyAvFrame*,MyAvFrame*>
		pH264->m_streamlock.Unlock();
	}else
	{
		TRACE("数组过大，%d\n",pH264->p_EncodeArray.GetSize());
	}


	return;
}

#if 0
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
#endif

// 启动拉流函数
int CH3CIMOSDlg::StartPull(void *pStream,void* pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3C_PARAMS *pH3cParams = &m_h3cParams[pH264Stream->thisIndex];
	ULONG ulRet;
	int i;
	
	m_Log.Add("[CH3CIMOSDlg::StartPull]:thisIndex:%d", pH264Stream->thisIndex);

	CHAR szPasswd[IMOS_PASSWD_ENCRYPT_LEN] = {0};
	ulRet = IMOS_Encrypt(pH3cParams->m_passWord, strlen(pH3cParams->m_passWord), szPasswd);
	if (ERR_COMMON_SUCCEED != ulRet)
	{
		//log
		//DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, "", "密码加密失败");
		return -1;
	}


	//用户登陆
	ulRet = IMOS_LoginEx(pH3cParams->m_userName, szPasswd, pH3cParams->m_proxyIp, "N/A", &pH3cParams->m_loginInfo);//IMOS_Login(szUserLoginName, szPasswd, szServerAdd, &g_stLoginInfo);
	if (ERR_COMMON_SUCCEED != ulRet)
	{
		//log
		m_Log.Add("用户登陆:用户名%s:密码%s", pH3cParams->m_userName,szPasswd);

		return -1;
	}

	/* 分配内存获取窗格编码 */
	PLAY_WND_INFO_S *pstPlayWndInfoList = NULL;
	pstPlayWndInfoList = (PLAY_WND_INFO_S*)malloc(DEMO_MAX_PLAYS * sizeof(PLAY_WND_INFO_S));
	if (NULL == pstPlayWndInfoList)
	{
		//		DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "分配内存失败");
		return -1;
	}
	memset(pstPlayWndInfoList, 0, DEMO_MAX_PLAYS * sizeof(PLAY_WND_INFO_S));

	/* 启动播放器组件并获得窗格编码 */
	ulRet = IMOS_StartPlayer(&pH3cParams->m_loginInfo.stUserLoginIDInfo, DEMO_MAX_PLAYS, pstPlayWndInfoList);
	if (ERR_COMMON_SUCCEED != ulRet)
	{
		//		DEMO_LOG_0(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "启动播放器组件失败");
		return -1;
	}
	free(pstPlayWndInfoList);

	//char szXpCode[IMOS_RES_CODE_LEN] = {0};
	ulRet = IMOS_GetChannelCode(&pH3cParams->m_loginInfo.stUserLoginIDInfo, pH3cParams->m_szXpCode);

	if(pH264Stream->b_Encode == 0)
	{
		ulRet = IMOS_SetParseVideoDataCB  ( &pH3cParams->m_loginInfo.stUserLoginIDInfo,  
			pH3cParams->m_szXpCode,  
			myCallH264_1,  
			1,  
			pH264Stream->thisIndex);
	}else
	{
#if 1
		ulRet = IMOS_SetDecodeVideoDataCB(&pH3cParams->m_loginInfo.stUserLoginIDInfo,
			pH3cParams->m_szXpCode, 
			myCallYV_1, 
			1, 
			pH264Stream->thisIndex);

#endif
	}


	/* 调用SDK接口开始实时监控，操作码为0 */
	ulRet = IMOS_StartMonitor(&pH3cParams->m_loginInfo.stUserLoginIDInfo,
		pH3cParams->m_deviceId,
		pH3cParams->m_szXpCode,
		IMOS_FAVORITE_STREAM_ANY,
		USER_OPERATE_SERVICE);
	if (ERR_COMMON_SUCCEED != ulRet)
	{
		//log
		//DEMO_LOG_2(LOG_TYPE_FAIL, ulRet, g_stLoginInfo.stUserLoginIDInfo.szUserCode, "实时监控", 
		//	"摄像机", pCurNode->m_nodeInfo.szResCode,
		//	"窗格", pCurPlay->m_szXpCode);
		return -1;
	}

	pH264Stream->h_stream = &pH3cParams->m_szXpCode;

	return 0;
}

int CH3CIMOSDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_H3C_PARAMS* pH3cVM=&m_h3cParams[pH264Stream->thisIndex];


	ULONG ulRet;

	if (NULL!=pH264Stream->h_stream)
	{

		ulRet = IMOS_StopMonitor(&pH3cVM->m_loginInfo.stUserLoginIDInfo,
			pH3cVM->m_szXpCode,
			0);

		IMOS_FreeChannelCode(&pH3cVM->m_loginInfo.stUserLoginIDInfo, pH3cVM->m_szXpCode);

		IMOS_LogoutEx(&pH3cVM->m_loginInfo.stUserLoginIDInfo);
		pH264Stream->h_stream=NULL;
	}

	return 0;
}
