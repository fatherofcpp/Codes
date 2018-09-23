// VNMPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "VNMPDlg.h"

extern CWMV_TO_H264Dlg* Main;
static CVNMPDlg* pVNMPDlg=NULL;
// CVNMPDlg 对话框

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


// CVNMPDlg 消息处理程序

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

	m_Log.Add("初始化通道的配置: 参数为:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
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
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV分配内存时出现错误！",SourceCount);
	}

	m_h264[SourceCount]->thisIndex = SourceCount;
	m_h264[SourceCount]->b_Encode	= 1;
	//m_h264[SourceCount]->b_Decode = 1;

	m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[SourceCount].m_devInfo.m_dstIp;
	m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
	m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[SourceCount].m_devInfo.m_dstPub,SourceCount);

	//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
	m_h264[SourceCount]->m_devType = DEF_NETPOSA_DEV;

	SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
	//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);

	m_Log.Add("初始化通道%d的配置: 参数为:DeviceName%d:%s,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		SourceCount,
		SourceCount,m_devParams[SourceCount].m_cameraId,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstIp,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstPub,
		SourceCount,m_devParams[SourceCount].m_devInfo.m_dstStream);//张世俊增加
	TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);

	return 0;
}

/*根据本地的配置文件信息，启动默认转码工作。
 *@date 2014-3-27
 *@author zhh
 *@param void * iniPath	配置文件所在目录.
 *@return int     0：成功 非0：失败
 *
*/
int CVNMPDlg::GetInforFromIni(void * iniPath)
{
	char dst[255];
	CString tmpStr;

	CString thisDir;
	thisDir.Format("%s",(char*)iniPath);

	SourceCount=0;


	while(1)//读取config.ini中所有ntqy,对应new CH264Stream
	{
		if(SourceCount >= g_maxLiveNum)
		{
			m_Log.Add("达到最大转码路数，以下配置停止分析。%d：%d",SourceCount,g_maxLiveNum);
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

		m_Log.Add("初始化通道的配置: 参数为:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
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
					m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV分配内存时出现错误！",SourceCount);
				}

				m_h264[SourceCount]->thisIndex = SourceCount;
				m_h264[SourceCount]->b_Encode	= 1;
				//m_h264[SourceCount]->b_Decode = 1;

				m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[SourceCount].m_devInfo.m_dstIp;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[SourceCount].m_devInfo.m_dstPub,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
				m_h264[SourceCount]->m_devType = DEF_NETPOSA_DEV;

				SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
				//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);
		}

		

		m_Log.Add("初始化通道%d的配置: 参数为:DeviceName%s:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devParams[SourceCount].m_cameraId,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstIp,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstPub,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstStream);//张世俊增加
		TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);
		SourceCount++;

	}


	return 0;
}

/*对于其它的没有从INI配置文件中读取到的路，采用以下方式来处理。
 *该处理过程只是创建实例，但该实例创建后并不马上参加运行
 *@date 2014-3-27
 *@author zhh
 *@param void * pParams	保留参数.
 *@return int     0：成功 非0：失败
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
			m_Log.Add("m_h264[%d]=new CH264Stream补全16的时候分配内存时出现错误！",SourceCount);
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
<company>11</company>//长天智远

<vnmp>
<ip>192.168.0.1</ip>//地址
<port>2100</port>//端口
<user>admin</user>//用户名
<passwd>admin</passwd>//密码
<channel>0001</channel>//播放通道的标识

</vnmp>
<ch>0</ch>
</command>

#endif

/*对网络发送的XML查询或者是切换信息进行解析，并将处理后结果返回给请求端。
 *如果不存在，则返回空
 *@date 2014-3-26
 *@author zhh
 *@param char * pXmlBuf	接收到的XML信息指针.
 *@param CString *str   返回URL存储信息空间,可供播放的URL
 *@return int     200：表示操作成功，返回200
 *				  400：表示XML存在非法信息
 *				  404：表示查询结果不存在
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
		//m_MicrovideoParams.ServerIP = node->data;
		strcpy(tmpVNMP.m_devInfo.m_dstIp,node->data); 
		//m_Log.Add("解析到的serverip: %s",pubserverip);
	}
#if 0
	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=3)
	{
		m_Log.Add("xml解析错误，未能找到company节点,或者company节点内容不是hk == 4!\n");
		rt = 400;
		goto leave;
	}
#endif
	subroot = f_xmlnode_sch_elem(root, "vnmp");
	if(subroot==NULL)
	{
		m_Log.Add("xml解析错误，未能找到vnmp节点!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("开始解析VNMP的XML!");

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpVNMP.ServerIP=node->data;
			strcpy(tmpVNMP.m_proxyIp,node->data);
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
			//tmpVNMP.nPort=atoi(node->data);
			tmpVNMP.m_proxyPort = atoi(node->data);
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
			//tmpVNMP.SrcUrl = node->data;
			strcpy(tmpVNMP.m_userName,node->data);
			//m_Log.Add("解析到的url：%s",tmpSource);
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
			//tmpVNMP.SrcUrl = node->data;
			strcpy(tmpVNMP.m_passWord,node->data);
			//m_Log.Add("解析到的url：%s",tmpSource);
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
			//tmpVNMP.nPort=atoi(node->data);
			//tmpVNMP.m_playChn = atoi(node->data);
			strcpy(tmpVNMP.m_cameraId,node->data);
			
			//m_Log.Add("解析到的port：%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到channel节点!");
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
			m_Log.Add("解析到的ch为空值!");		
			//sprintf(srcCam,"%s:%d:%s:%s:%s",tmpVNMP.m_proxyIp,tmpVNMP.m_proxyPort,tmpVNMP.m_userName,tmpVNMP.m_passWord,tmpVNMP.m_cameraId);

			sprintf(srcCam,"%s",tmpVNMP.m_cameraId);
			m_Log.Add("查找视频：%s是否存在！",srcCam);

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
			m_Log.Add("解析到的ch: %s",CH);
			m_Log.Add("切换%s,至通道%d",tmpSrcID,atoi(CH));

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
		m_Log.Add("xml解析错误，ch节点的值为空!");
		rt = 400;
		goto leave;
	}
	//m_Log.Add("解析到通道%d: 参数为:Serverip:%s Company:%s DevType:%s Version:%s IP:%s Port:%s Url:%s\n",atoi(CH), pubserverip,"microvideo",tmpType, tmpVer, tmpIp, tmpPort, tmpSource);//张世俊增加
	//TRACE("DevType:%s Version:%s IP:%s Port:%s Url:%s", tmpType, tmpVer, tmpIp, tmpPort, tmpSource);

//#endif

leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;
}

/*根据传入的唯一标识，判断该路是否在进行转码，如果进行转码返回可供播放的URL，
 *如果不存在，则返回空
 *@date 2014-3-26
 *@author zhh
 *@param CString str	准备切换的视频的唯一标识，不局限于cameraId.
 *@return CString 空：表示切换失败
 *				  非空：表示可供播放的URL信息
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

/*根据传入的参数和所使用的通道号进行视频切换操作,并返回切换后的可供播放的URL
 *@date 2014-3-26
 *@author zhh
 *@param CString sId	准备切换的视频的唯一标识，不局限于cameraId.
 *@param int CH  切换后视频使用的发布通道
 *@param CString str   服务器地址信息
 *@return CString 空：表示切换失败
 *				  非空：表示可供播放的URL信息
 *
*/
CString CVNMPDlg::ChangeChannel(CString sId,int CH,CString str)
{
	CString UrlStr="";

	if(CH >= g_maxLiveNum)
	{//临时设置最大转码路数
		m_Log.Add("原视频应当切换通道为:%d \n!",CH);
		CH = CH%g_maxLiveNum;
		SourceCount = g_maxLiveNum;
		m_Log.Add("实际视频应当切换通道为:%d \n!",CH);
	}

	if (CH>=SourceCount)
	{

		m_Log.Add("CH的值不能大于%d，出错!",g_maxLiveNum);
		return "";
	}
	


	m_Log.Add("长天智远准备切换%s,至通道%d,服务器%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	//if (NULL!=m_h264[CH]->h_stream)
	//{//该路已经被使用，需要先关闭再进行操作
	//	//m_netVideo[CH].CloseVideo();

	//	Main->ClosePullFromSub(m_h264[CH],NULL);//ClosePull(m_h264[CH],&m_devParams[CH]);

	//	m_h264[CH]->h_stream=NULL;

	//}
	//m_h264[CH]->m_Info.s_DevID = sId;//作为唯一性的标识，如id为dns:7856,视频通道为2，则表示为：dns:7856：1


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
		m_Log.Add("长天智远视频%s 切换成功%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//保存正确的配置到ini文件中
		m_h264[CH]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。且持续时间从现在开始算起。

		return UrlStr;
	}
	return UrlStr;
}


/*YUV数据回调函数，获取解码后的YUV数据，并将该YUV数据存入到YUV缓冲区，传递到下一编码功能
 *@date 2014-3-26
 *@author zhh
 *@param long nPort	用户数据
 *@param char * pBuf  YUV存放指针
 *@param long nSize   YUV数据大小
 *@param VNMP_FRAME_INFO * pFrameInfo  VNMP_FRAME_INFO结构指针，存储该帧图像的长宽，时间等信息
 *@param long nReserved1 保留数据
 *@param long nReserved2 保留数据
 *@return 无
 *
*/
void CALLBACK myDecCBFun(long nPort,
						 char * pBuf,
						 long nSize,
						 VNMP_FRAME_INFO * pFrameInfo,
						 long nReserved1,
						 long nReserved2)
{
	//YUV数据回调函数回调为yuv12数据
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
			m_Log.Add("myVideo函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");
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
		TRACE("数组过大，%d\n",pH264->p_YuvArray.GetSize());
	}	
}



/*视频回调函数,对接收到数据进进行分包和重组，由于目前不能确认得到的全为H264数据，
 *固该次全部采用YUV数据进行编码。所以在视频回调中加入了YUV回调函数的注册。
 *@date 2014-3-26
 *@author zhh
 *@param DWORD dwDataType	低位是数据编码类型高位如果是是表示头部数据0为普通数据
 *@param unsigned long bufsize  该次回调的数据大小
 *@param unsigned char *buffer	该次回调数据指针
 *@param unsigned long user		用户数据
 *@return 无
 *
*/
void CALLBACK  CBK_HandVideoData(DWORD dwDataType, 
								 unsigned long bufsize, 	
								 unsigned char *buffer, 	
								 unsigned long user)
{
	//视频数据回调函数
	//dwDataType低位是数据编码类型高位如果是是表示头部数据0为普通数据

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
		//YUV数据获取的回调函数注册
		video_play_SetYUVCallBack(iwnd,myDecCBFun);
		ret = video_play_Play(iwnd, buffer,bufsize,NULL);
	}else{
		video_play_InputVideoData(iwnd,bufsize,buffer);
	}
}


/*配置调用长天智远SDK所需要的参数信息，并启动所需要的编码和推流线程
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	该参数作用的CH264Stream类的对象
 *@param void *pParams  配置该路信息的参数结构
 *
 *@return int 操作成功返回0，否则返回-1
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

/*调用长天智远SDK完成拉流操作，并进行回调函数的注册
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	该参数作用的CH264Stream类的对象
 *@param void *pParams  配置该路信息的参数结构
 *
 *@return int 操作成功返回0，否则返回-1
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

	//登录时设置videodev.reserve，其中是SDK标识
	sprintf(videoDev.reserve,"%d%d",0,512);

	if((pVNMPParams->m_devInfo.m_loginHandle = video_device_login(&videoDev)) < 0){
		m_Log.Add("[ERR]:[%d]CVNMPDlg::StartPull->video_device_login",pH264Stream->thisIndex);
		video_device_destroy();
		return -1;
	}

	//videodev.handle与videodev.channel必须填写
	videoDev.handle = pVNMPParams->m_devInfo.m_loginHandle;
#endif

	strcpy(videoDev.channel,pVNMPParams->m_cameraId);

#if 0
	//videodev.reuse,videodev.reserve 固定填写如下
	videoDev.reuse = 1;
	videoDev.reuse = videoDev.reuse | 0x200;
// 	//正常内网访问
     strcpy(videoDev.reserve,".");
#endif
	//NET网络跨网访问
//	strcpy(videodev.reserve,"./NET.ini");
	//video_device_capturing_start的第二个参数是用户参数
	//SDK内部不修改在数据回调中原样作为参数传入用于标识码流
	pVNMPParams->m_devInfo.m_videoHandle = video_device_capturing_start(&videoDev,pH264Stream->thisIndex,CBK_HandVideoData);
	if(0 >pVNMPParams->m_devInfo.m_videoHandle)
	{
		m_Log.Add("[ERR]:[%d]CVNMPDlg::StartPull->video_device_capturing_start:[%ld]",pH264Stream->thisIndex,pVNMPParams->m_devInfo.m_videoHandle);
		//video_device_logout(&videoDev);
		//video_device_destroy();

		return -1;
	}

	pVNMPParams->m_devInfo.m_videoHandle = videoDev.devnum;
	//函数修改了videodev中数据
	//返回值>=0 成功if m_videohandle = videodev.devnum then success
	//返回值>=0 成功if m_videohandle != videodev.devnum then 重复打开了摄像机
	//返回值>=0 成功videodev.channel是真实打开的摄像机ID 可能和传入的不一至
	//返回值>=0 成功videodev.reserve是真实打开的摄像机的名称

	pH264Stream->h_stream = &videoDev;
		
	return 0;
}

/*调用长天智远SDK完成关闭操作，并进行资源的释放操作
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	该参数作用的CH264Stream类的对象
 *@param void *pParams  配置该路信息的参数结构
 *
 *@return int 操作成功返回0，否则返回-1
 *
*/
int CVNMPDlg::ClosePull(void *pStream,void * pParams)
{
	CH264Stream *pH264Stream = (CH264Stream *)pStream;
	DEV_VNMP_PARAMS *pVNMPParams = &m_devParams[pH264Stream->thisIndex];
	int i = 0;
	
	VIDEODEVICE videodev;
	memset(&videodev,0,sizeof(VIDEODEVICE));

	//停止必须传入videodev.handle、videodev.devnum
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


