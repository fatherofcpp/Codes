// NetPosaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "NetPosaDlg.h"

extern CWMV_TO_H264Dlg* Main;
static CNetPosaDlg* pNpDlg=NULL;

// CNetPosaDlg 对话框

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


// CNetPosaDlg 消息处理程序

void CNetPosaDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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

	m_Log.Add("初始化通道的配置: 参数为:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
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
			m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV分配内存时出现错误！",nIndex);
		}

		m_h264[nIndex]->thisIndex = nIndex;
		m_h264[nIndex]->b_Encode	= (g_bUseTc==1?1:0);
		//m_h264[nIndex]->b_Decode = 1;

		m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_devInfo.m_dstIp;
		m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
		m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_devInfo.m_dstPub,nIndex);
		//m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,nIndex);

		//m_h264[nIndex]->pNetVideo = &m_netVideo[nIndex];//临时使用
		m_h264[nIndex]->m_devType = DEF_NETPOSA_DEV;

		SetParams(m_h264[nIndex],&m_devParams[nIndex]);
		//StartPull(m_h264[nIndex],&m_devParams[nIndex]);




	m_Log.Add("初始化通道%d的配置: 参数为:DeviceName%d:%s,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
		nIndex,
		nIndex,m_devParams[nIndex].m_devName,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstIp,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstPub,
		nIndex,m_devParams[nIndex].m_devInfo.m_dstStream);
	//TRACE("[GetInforFromIni]:nIndex=%d\n",nIndex);

	return 0;
}

/*根据本地的配置文件信息，启动默认转码工作。
 *@date 2014-3-27
 *@author zhh
 *@param void * iniPath	配置文件所在目录.
 *@return int     0：成功 非0：失败
 *
*/
int CNetPosaDlg::GetInforFromIni(void * iniPath)
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
					m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV分配内存时出现错误！",SourceCount);
				}

				m_h264[SourceCount]->thisIndex = SourceCount;
				m_h264[SourceCount]->b_Encode	= 1;
				//m_h264[SourceCount]->b_Decode = 1;

				m_h264[SourceCount]->m_Info.s_PubServer=m_devParams[SourceCount].m_devInfo.m_dstIp;
				m_h264[SourceCount]->m_Info.s_CH.Format("%d",SourceCount);
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",m_devParams[SourceCount].m_devInfo.m_dstPub,SourceCount);
				//m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
				m_h264[SourceCount]->m_devType = DEF_NETPOSA_DEV;

				SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
				//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);
		}

		

		m_Log.Add("初始化通道%d的配置: 参数为:DeviceName%s:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devParams[SourceCount].m_devName,
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
int CNetPosaDlg::SetDefaultParams(void * pParams)
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
<ip>192.168.0.1</ip>//地址
<port>2100</port>//端口
<user>admin</user>//用户名
<passwd>admin</passwd>//密码
<channel>av/1/00000000000001</channel>//播放通道的标识

</np>
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
		strcpy(tmpNP.m_devInfo.m_dstIp,node->data); 
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
	subroot = f_xmlnode_sch_elem(root, "np");
	if(subroot==NULL)
	{
		m_Log.Add("xml解析错误，未能找到np节点!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("开始解析NP的XML!");

		node = f_xmlnode_sch_elem_ext(subroot, "ip", MAX_LEN);
		if (node != NULL)
		{
			//tmpNP.ServerIP=node->data;
			strcpy(tmpNP.m_proxyIp,node->data);
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
			//tmpNP.nPort=atoi(node->data);
			tmpNP.m_proxyPort = atoi(node->data);
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
			//tmpNP.SrcUrl = node->data;
			strcpy(tmpNP.m_userName,node->data);
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
			//tmpNP.SrcUrl = node->data;
			strcpy(tmpNP.m_passWord,node->data);
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
			//tmpNP.nPort=atoi(node->data);
			//tmpNP.m_playChn = atoi(node->data);
			strcpy(tmpNP.m_devName,node->data);
			
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
			sprintf(srcCam,"%s:%d:%s:%s:%s",tmpNP.m_proxyIp,tmpNP.m_proxyPort,tmpNP.m_userName,tmpNP.m_passWord,tmpNP.m_devName);
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
			tmpSrcID.Format("%s:%d:%s:%s:%s",tmpNP.m_proxyIp,tmpNP.m_proxyPort,tmpNP.m_userName,tmpNP.m_passWord,tmpNP.m_devName);
#if 1
			//m_h264[atoi(CH)]->m_Info.s_DevID = tmpSrcID;
			//m_h264[atoi(CH)]->m_Info.s_PubServer = tmpNP.m_devInfo.m_dstIp;
			m_Log.Add("解析到的ch: %s",CH);
			m_Log.Add("切换%s,至通道%d",tmpSrcID,atoi(CH));

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
CString CNetPosaDlg::CheckExit(CString str)
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
CString CNetPosaDlg::ChangeChannel(CString sId,int CH,CString str)
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
	
	//TRACE("");


	m_Log.Add("东方网力准备切换%s,至通道%d,服务器%s",m_h264[CH]->m_Info.s_DevID,CH,str);
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
		m_Log.Add("东方网力视频%s 切换成功%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//保存正确的配置到ini文件中
		m_h264[CH]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。且持续时间从现在开始算起。

		return UrlStr;
	}
	return UrlStr;
}


void NP_SDK_CALLBACK OnRealDecodeData(NP_PLAY_HD playHD, const void* buf, INT len, const NPPlayFrameInfo* fInfo, void* userParam)
{//YUV数据
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
			m_Log.Add("myVideo函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");

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
		TRACE("数组过大，%d\n",pH264->p_YuvArray.GetSize());
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
			m_Log.Add("视频数据为空！");

			return ;
		}
		if (0 == dataLen )
		{
			m_Log.Add("视频数据为空！");

			return ;
		}

		MyAvFrame *m_Frame=NULL;
		//if(lFrameLen<(352*288*3/2+1))//由于无法判断其大小，估先设置其当前大小作为该次数据量大小
		//	m_Frame = new MyAvFrame(352,288);//默认设置大小 CIF标准
		//else
		m_Frame = new MyAvFrame(dataLen,1);//暂无法判断出视频大小


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
		memcpy(m_Frame->p_data,data,dataLen);//new MyAvFrame(352,288)<--byteVideoData

		pH264->m_streamlock.Lock();
		pH264->p_EncodeArray.Add(m_Frame);//视频数据存进待编码数据缓冲区CArray<MyAvFrame*,MyAvFrame*>
		pH264->m_streamlock.Unlock();
	}
}

/*配置调用东方网力SDK所需要的参数信息，并启动所需要的编码和推流线程
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	该参数作用的CH264Stream类的对象
 *@param void *pParams  配置该路信息的参数结构
 *
 *@return int 操作成功返回0，否则返回-1
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

/*调用东方网力SDK完成拉流操作，并进行回调函数的注册
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	该参数作用的CH264Stream类的对象
 *@param void *pParams  配置该路信息的参数结构
 *
 *@return int 操作成功返回0，否则返回-1
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

	//从这里来判断要得到的是YUV数据还是H264数据
	//如果参与编码说明需要YUV数据
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

/*调用东方网力SDK完成关闭操作，并进行资源的释放操作
 *@date 2014-3-26
 *@author zhh
 *@param void *pStream	该参数作用的CH264Stream类的对象
 *@param void *pParams  配置该路信息的参数结构
 *
 *@return int 操作成功返回0，否则返回-1
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


/*调用东方网力SDK完成网络及播放器的初始化工作
 *@date 2014-3-26
 *@author zhh
 *
 *@return int 操作成功返回0，否则返回-1
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
		m_Log.Add( "NP_PLAY_GetSDK_Version获取版本成功：infoBuf[%s]", infoBuf);
	
	NP_NET_Init();
	ret = NP_NET_GetSDK_Version(infoBuf, sizeof(infoBuf), &realLen);
	if(ret != NPSDK_OK)
		m_Log.Add( "ret[%d] = NP_NET_GetSDK_Version(infoBuf[%s], sizeof(infoBuf)[%d])",
		ret, infoBuf, sizeof(infoBuf));
	else
		m_Log.Add( "NP_NET_GetSDK_Version获取版本成功：infoBuf[%s]", infoBuf);
}

/*调用东方网力SDK对资源的释放
 *@date 2014-3-26
 *@author zhh
 *
 *@return int 操作成功返回0，否则返回-1
 *
*/
void CNetPosaDlg::SDKCleanup()
{
	NP_NET_Cleanup();

	NP_PLAY_Cleanup();
}

/*将CCS发送过来的形如:"av/001:left,76"之类的信息转换为东方网力具体的操作码
 *@date 2014-3-26
 *@author zhh
 *@param char *const buf 输入的操作命令信息
 *@param CCS_CMD *const cmd 转换后操作存放结构
 *@return int 转换失败返回<0，否则返回0
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



/*调用东方网力SDK完成云台的控制,对于输入内容形如：“np:av/001:left,76”，进行转换，并实现控制
 *@date 2014-3-26
 *@author zhh
 *@param char * pBuf 待处理的包含操作信息的字符串
 *@param int nLen 输入字符串的长度
 *@return int 操作成功返回0，否则返回-1
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
	{//请求状态信息
		char statusBuf[1024] = {0};
		INT realLen = 0;
		ret = NP_NET_GetPtzStatus(m_devParams[i].m_devInfo.m_loginid, pBufID, statusBuf, sizeof(statusBuf), &realLen);
		if(ret != NPSDK_OK)
			m_Log.Add( "ret[%d] = NP_NET_GetPtzStatus(m_loginid[%d], pReal->name[%s], statusBuf[%s], sizeof(statusBuf)[%d])",
			ret, m_devParams[i].m_devInfo.m_loginid, pBufID, statusBuf, sizeof(statusBuf));
		else
		{
			//ASSERT(realLen < 1024);
			m_Log.Add("NP_NET_GetPtzStatus调用成功：statusBuf[%s]", statusBuf);
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
