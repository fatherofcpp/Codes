// HKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "HKDlg.h"

#include "hkLib.h"

extern CWMV_TO_H264Dlg* Main;
static CHKDlg* pHkDlg=NULL;
// CHKDlg 对话框

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
  // 初始化
  NET_DVR_Init();

  //设置连接时间与重连时间
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

	m_Log.Add("初始化通道的配置: 参数为:ProxyIp:%s,ProxyPort:%d,UserName:%s,PassWd:%s",
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
		m_Log.Add("m_h264[%d]=new CH264Stream;DEF_SHSL_DEV分配内存时出现错误！",nIndex);
	}

	m_h264[nIndex]->thisIndex = nIndex;
	m_h264[nIndex]->b_Encode	= 1;//(g_bUseTc==1?1:0);
	//m_h264[nIndex]->b_Decode = 1;

	m_h264[nIndex]->m_Info.s_PubServer=m_devParams[nIndex].m_devInfo.m_dstIp;
	m_h264[nIndex]->m_Info.s_CH.Format("%d",nIndex);
	m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",m_devParams[nIndex].m_devInfo.m_dstPub,nIndex);
	//m_h264[nIndex]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,nIndex);

	//m_h264[nIndex]->pNetVideo = &m_netVideo[nIndex];//临时使用
	m_h264[nIndex]->m_devType = DEF_HK_DEV;

	SetParams(m_h264[nIndex],&m_devParams[nIndex]);
	//StartPull(m_h264[nIndex],&m_devParams[nIndex]);

	m_Log.Add("初始化通道%d的配置: 参数为:DeviceName%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
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
			m_Log.Add("config.ini文件中未能找全南通汽运的DeviceId,DeviceChn,Dstip,Dstpub,Dststream");
			break;
		}

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
				m_h264[SourceCount]->m_Info.s_PubName.Format("%s_v%d",Main->m_pubNameFlag,SourceCount);

				//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
				m_h264[SourceCount]->m_devType = DEF_HK_DEV;

				SetParams(m_h264[SourceCount],&m_devParams[SourceCount]);
				//StartPull(m_h264[SourceCount],&m_devParams[SourceCount]);
		}

		

		m_Log.Add("初始化通道%d的配置: 参数为:DeviceChn%d:%d,Dstip%d:%s,Dstpub%d:%s,Dststream%d:%d",
			SourceCount,
			SourceCount,m_devParams[SourceCount].m_deviceChn,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstIp,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstPub,
			SourceCount,m_devParams[SourceCount].m_devInfo.m_dstStream);//张世俊增加
		TRACE("[GetInforFromIni]:SourceCount=%d\n",SourceCount);
		SourceCount++;

	}


	return 0;
}

//对于其它的没有从INI配置文件中读取到的路，采用以下方式来处理。
//该处理过程只是创建实例，但该实例创建后并不马上参加运行
int CHKDlg::SetDefaultParams(void * pParams)
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

		//m_h264[SourceCount]->Init("http://192.168.0.251:8080/live1",ServerIP,PubName,SourceCount,this->GetSafeHwnd());
		//m_h264[SourceCount]->StartGet();

		//m_h264[SourceCount]->pNetVideo = &m_netVideo[SourceCount];//临时使用
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
<type></type>//设备类型
<ver></ver>//版本
<ip></ip>//地址
<port></port>//端口
<user></user>//用户名
<passwd></passwd>//密码
<channel></channel>//默认播放通道
<pipe></pipe>//默认设备通道
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
		strcpy(tmpHK.m_devInfo.m_dstIp,node->data); 
		//m_Log.Add("解析到的serverip: %s",pubserverip);
	}

	node = f_xmlnode_sch_elem_ext(root, "company", MAX_LEN);
	if(node == NULL || atoi(node->data)!=3)
	{
		m_Log.Add("xml解析错误，未能找到company节点,或者company节点内容不是hk == 4!\n");
		rt = 400;
		goto leave;
	}

	subroot = f_xmlnode_sch_elem(root, "hk");
	if(subroot==NULL)
	{
		m_Log.Add("xml解析错误，未能找到microvideo节点!");
		rt = 400;
		goto leave;
	}
	else
	{ 
		m_Log.Add("开始解析HK的XML!");
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
			//tmpHK.ServerIP=node->data;
			strcpy(tmpHK.m_proxyIp,node->data);
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
			//tmpHK.nPort=atoi(node->data);
			tmpHK.m_proxyPort = atoi(node->data);
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
			//tmpHK.SrcUrl = node->data;
			strcpy(tmpHK.m_userName,node->data);
			//m_Log.Add("解析到的url：%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到url节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "passwd", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.SrcUrl = node->data;
			strcpy(tmpHK.m_passWord,node->data);
			//m_Log.Add("解析到的url：%s",tmpSource);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到url节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "channel", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.nPort=atoi(node->data);
			tmpHK.m_deviceChn = atoi(node->data);
			
			//m_Log.Add("解析到的port：%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到port节点!");
			rt = 400;
			goto leave;
		}

		node = f_xmlnode_sch_elem_ext(subroot, "pipe", MAX_LEN);
		if (node != NULL)
		{
			//tmpHK.nPort=atoi(node->data);
			tmpHK.m_playChn = atoi(node->data);
			
			//m_Log.Add("解析到的port：%s",tmpPort);
		}
		else
		{
			m_Log.Add("xml解析错误，未能找到port节点!");
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
			sprintf(srcCam,"%s:%d:%s:%s:%d",tmpHK.m_proxyIp,tmpHK.m_proxyPort,tmpHK.m_userName,tmpHK.m_passWord,tmpHK.m_deviceChn);
			m_Log.Add("查找视频：%s是否存在！",srcCam);

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
			m_Log.Add("解析到的ch: %s",CH);
			m_Log.Add("切换%s,至通道%d",tmpSrcID,atoi(CH));

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
		m_Log.Add("xml解析错误，ch节点的值为空!");
		rt = 400;
		goto leave;
	}
	//m_Log.Add("解析到通道%d: 参数为:Serverip:%s Company:%s DevType:%s Version:%s IP:%s Port:%s Url:%s\n",atoi(CH), pubserverip,"microvideo",tmpType, tmpVer, tmpIp, tmpPort, tmpSource);//张世俊增加
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


CString CHKDlg::ChangeChannel(CString sId,int CH,CString str)
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
	//m_h264[CH]->m_Info.s_DevID = sId;//作为唯一性的标识，如id为dns:7856,视频通道为2，则表示为：dns:7856：1


	//m_Log.Add("海康准备切换%s,至通道%d,服务器%s",m_h264[CH]->m_Info.s_DevID,CH,str);
	//if (NULL!=m_h264[CH]->h_stream)
	//{//该路已经被使用，需要先关闭再进行操作
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
		m_Log.Add("海康视频%s 切换成功%s",m_h264[CH]->m_Info.s_DevID,UrlStr);

		//SaveConfigIni(NULL,CH);//保存正确的配置到ini文件中
		m_h264[CH]->b_stop = 0;//不论之前是否在播放，现在应当启动播放。且持续时间从现在开始算起。

		return UrlStr;
	}


	return UrlStr;


}

//LONG lPort; //全局的播放库port号
/**
 * @brief HK sdk dec callback fun.
 * @param[in] nPort
 * @param[in] pBuf
 * @param[in] nSize
 * @param[in] p_FrameInfo
 * @param[in] nUser
 * @param[in] reserved2
 * 
 * 函数描述: 海康解码回调函数
 * 创建时间：2012-12-28
 * 创建人:   许乐
 * 修改日期        
 * 版本号  ：V1.0   
 * 修改人	      
 * 修改内容
 * 修改日期
 * 入参：nPort，pBuf，nSize，p_FrameInfo，nUser，nReserved2
 * 输出: 视频yuv数据
 * 返回 ：无
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
			m_Log.Add("myVideo函数MyAvFrame *m_Frame=new MyAvFrame内存分配时出现错误！");

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
		TRACE("数组过大，%d\n",pH264->p_YuvArray.GetSize());
	}
	
	


	return;

}


// CHKDlg 消息处理程序
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dUser)
{
    HWND hWnd = NULL;//GetConsoleWindow();
	DWORD dwUser = *(DWORD *)dUser;

	CH264Stream* pH264=(CH264Stream*)m_h264[dwUser];
	if(pH264 == NULL)
		return;

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头
		if(-1 != pHkDlg->m_devParams[dwUser].m_playChn)
		{
			PlayM4_Stop(pHkDlg->m_devParams[dwUser].m_playChn);
			PlayM4_CloseStream(pHkDlg->m_devParams[dwUser].m_playChn);
			PlayM4_FreePort(pHkDlg->m_devParams[dwUser].m_playChn);

			pHkDlg->m_devParams[dwUser].m_playChn = -1;
		}

		//if (!PlayM4_GetPort(&lPort))  //获取播放库未使用的通道号
		if (!PlayM4_GetPort(&pHkDlg->m_devParams[dwUser].m_playChn))  //获取播放库未使用的通道号
		{
			break;
		}
		//m_iPort = lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(pHkDlg->m_devParams[dwUser].m_playChn, STREAME_REALTIME))  //设置实时流播放模式
			{
				break;
			}

			if (!PlayM4_OpenStream(pHkDlg->m_devParams[dwUser].m_playChn, pBuffer, dwBufSize, 1024*1024)) //打开流接口
			{
				break;
			}

			if (!PlayM4_SetDecCallBackMend(pHkDlg->m_devParams[dwUser].m_playChn,HKDecCallBack3,dwUser))
			{
				break;
			}

			if (!PlayM4_Play(pHkDlg->m_devParams[dwUser].m_playChn, hWnd)) //播放开始
			{
				break;
			}
		}
	case NET_DVR_STREAMDATA:   //码流数据
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
    case EXCEPTION_RECONNECT:    //预览时重连
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

	// 注册设备
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
  //设置异常消息回调函数
  //NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	//---------------------------------------
	//启动预览并设置回调数据流
	//LONG lRealPlayHandle;
	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	ClientInfo.lChannel     = pHKParams->m_deviceChn;//1;       //预览通道号
	ClientInfo.lLinkMode    = 0;       //最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
	ClientInfo.sMultiCastIP = NULL;   //多播地址，需要多播预览时配置

	NET_DVR_PREVIEWINFO struPlayInfo = {0};
	struPlayInfo.hPlayWnd = NULL;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel     = pHKParams->m_deviceChn;//33;       //预览通道号
	struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP



	BOOL bPreviewBlock = false;       //请求码流过程是否阻塞，0：否，1：是
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
	//关闭预览
	NET_DVR_StopRealPlay(pHKParams->m_devInfo.m_realPlayHandle);

	PlayM4_Stop(pHKParams->m_playChn);
	PlayM4_CloseStream(pHKParams->m_playChn);
	PlayM4_FreePort(pHKParams->m_playChn);

	//注销用户
	NET_DVR_Logout_V30(pHKParams->m_devInfo.m_userId);
	//NET_DVR_Cleanup();
	pHKParams->m_devInfo.m_userId = -1;
	pHKParams->m_playChn = -1;

	pH264Stream->h_stream = NULL;

	return 0;
}
