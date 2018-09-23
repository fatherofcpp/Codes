#include "stdafx.h"
#include "C2Htype.h"
#if 1
#include "../../include/dev_init.h"

#include "../include/commapp.h"
//#include "../include/command.h"
//#include "../include/frame.h"
#include "../include/VNMP_datawork_app.h"
//#include "../include/data_srv.h"
//#include "../include/queuesappl.h"
#include "../include/thread.h"
//#include "../../led/include/ledlib.h"
#include "../../comm/include/commbase.h"

#define REQUEST_PERIOD  500


//extern MS_CHAR *g_RecvBuf;
//extern MS_CHAR *g_SendBuf;
//extern MS_INT32		g_stateInfo;

extern CVNMPDlg* Main;

/*
MS_INT32 GetParamFrmHost(MS_INT32 comid)
{//首次主动向HOST发送信息，请求参数
	MS_INT32 len,i;
	MICP_HDR *pmicp_hdr;
	MS_CHAR	msg[DEF_MAX_CHARSTR_LENGHT4];
	
	memset(g_SendBuf,0,sizeof(MICP_HDR));
	pmicp_hdr = (MICP_HDR*)(g_SendBuf);
	
	pmicp_hdr->size      = sizeof(MICP_HDR);
	pmicp_hdr->dev   	 = DEF_MICP_DEV_BRD;
	pmicp_hdr->direct 	 = DEF_MICP_DIRECT_BRD_TO_HOST;
	pmicp_hdr->type      = DEF_MICP_TYPE_BRD_GET;
	pmicp_hdr->devid     = g_boardBaseInfo.card_no;
	pmicp_hdr->body_len  = 0x00;
	pmicp_hdr->seq		 = g_boardBaseInfo.sequence_id;
	pmicp_hdr->ext_info  = 0x00;
	pmicp_hdr->ident     = DEF_MICP_IDENT_BRD_DM642;
	
	sprintf(msg,"发请求参数包 \n");
	//LogPrintf(msg,DEF_MSG_PRINT ,0);
	
	len = sendComToHostW(comid,(MS_CHAR *)pmicp_hdr,sizeof(MICP_HDR),NULL,0);
	if(len < 0)
		return MD_FAILED;

	//sleep(10);
	
	for(i = 0;i<REQUEST_PERIOD;i++)
	{	
		ms_sleep(10);
		kick_dog();
		len = recvComFrHost(comid,g_RecvBuf,DEF_RECV_BUFFER_LEN);

		if(len < 0)
			return MD_FAILED;
		if(len == 0)
			continue;
		
		sprintf(msg,"获得参数包 \n");		
	    //LogPrintf(msg,DEF_MSG_PRINT ,0);	
	    g_RecvBuf[len] = 0;
		return  NetProcessCMD( g_RecvBuf,comid );	
	}
	
	return MD_FAILED;
}

MS_INT32 SendUpdateToHost(MS_INT32 comid)
{
	MICP_HDR *pmicp_hdr;
	
    memset(g_SendBuf,0,sizeof(MICP_HDR));
	pmicp_hdr = (MICP_HDR*)(g_SendBuf);
	
	pmicp_hdr->dev   	 = DEF_MICP_DEV_BRD;
	pmicp_hdr->direct 	 = DEF_MICP_DIRECT_BRD_TO_HOST;
	pmicp_hdr->type      = DEF_MICP_TYPE_BRD_UPDATE;
	pmicp_hdr->devid     = g_boardBaseInfo.card_no;
	
	pmicp_hdr->seq		 = g_boardBaseInfo.sequence_id;
	pmicp_hdr->ext_info  = 0x00;
	pmicp_hdr->ident     = DEF_MICP_IDENT_BRD_DM642;
	pmicp_hdr->none1	 = g_boardBaseInfo.none[0];//VERSION_TYPE <<16 | VERSION_EXTENSION ;
	pmicp_hdr->none2	 = VERSION_NUMBER;//VERSION_EXTENSION;
	//pmicp_hdr->none3	 = 0;	
	pmicp_hdr->size      = sizeof(MICP_HDR);

	return  sendComToHostW(comid,g_SendBuf,sizeof(MICP_HDR),NULL,0);	
}

MS_INT32 SendRespToHost(MS_INT32 comid)
{

	Audio_Info  my_content1;
    MU_INT8 	extradata1[32];
    Video_Info	my_content2;
	MU_INT8		extradata2[32];
	
	
	MICP_BODY_BRD_PARAMS tbrd_param;
	MICP_HDR *pmicp_hdr;
	MS_INT32 ndx,nOffset=0;
	
    memset(g_SendBuf,0,sizeof(MICP_HDR));

    
	pmicp_hdr = (MICP_HDR*)(g_SendBuf);
	
	pmicp_hdr->dev   	 = DEF_MICP_DEV_BRD;
	pmicp_hdr->direct 	 = DEF_MICP_DIRECT_BRD_TO_HOST;
	pmicp_hdr->type      = DEF_MICP_TYPE_RESP | DEF_MICP_TYPE_BRD_SET;
	pmicp_hdr->devid     = g_boardBaseInfo.card_no;
	
	pmicp_hdr->seq		 = g_boardBaseInfo.sequence_id;
	pmicp_hdr->ext_info  = 0x00;
	pmicp_hdr->ident     = DEF_MICP_IDENT_BRD_DM642;
	pmicp_hdr->none1	 = g_boardBaseInfo.chaoping;
	pmicp_hdr->size      = sizeof(MICP_HDR);
	
	nOffset += sizeof(MICP_HDR); 

	
	for(ndx=0;ndx<DEF_BOARD_MAX_CHANNELS;ndx++)
	{	
		if(g_vInfo[ndx].channel_id == 0)
			break;
		get_video_content(&my_content2,g_vInfo[ndx].params,extradata2);
		
		memset(&tbrd_param,0,sizeof(tbrd_param));
			
		strcpy(tbrd_param.flags,"MICROVIDEO");
		tbrd_param.size = sizeof(tbrd_param)-sizeof(MU_INT8*)+sizeof(Video_Info)+my_content2.ExtraDataSize-sizeof(MU_INT8*);//?hao
		tbrd_param.type = DEF_MICP_BODY_MEDIA_VIDEO;
		tbrd_param.channel = ndx+1;//g_vInfo[ndx].channel_id;//hao

		
		
		memcpy(g_SendBuf+nOffset,&tbrd_param,sizeof(MICP_BODY_BRD_PARAMS)-sizeof(MU_INT8*));
		nOffset += sizeof(MICP_BODY_BRD_PARAMS)-sizeof(MU_INT8*);
		
		memcpy(g_SendBuf+nOffset,&my_content2,sizeof(Video_Info)-sizeof(MU_INT8*));
		nOffset += sizeof(Video_Info)-sizeof(MU_INT8*);
		
		memcpy(g_SendBuf+nOffset,&extradata2,my_content2.ExtraDataSize);
		nOffset += my_content2.ExtraDataSize;

		//printf("video param size = %x\n",tbrd_param.size);
		
	}



	for(ndx=0;ndx<DEF_BOARD_MAX_CHANNELS;ndx++)
	{
		if(g_aInfo[ndx].channel_id == 0)
			break;
		get_audio_content(&my_content1,ndx,extradata1);
		
		memset(&tbrd_param,0,sizeof(tbrd_param));
		
		strcpy(tbrd_param.flags,"MICROVIDEO");
		tbrd_param.size = sizeof(tbrd_param)-sizeof(MU_INT8*)+sizeof(Audio_Info)+my_content1.ExtraDataSize-sizeof(MU_INT8*);//?hao
		tbrd_param.type = DEF_MICP_BODY_MEDIA_AUDIO;
		tbrd_param.channel = ndx+1;//g_aInfo[ndx].channel_id;//hao
		
		memcpy(g_SendBuf+nOffset,&tbrd_param,sizeof(MICP_BODY_BRD_PARAMS)-sizeof(MU_INT8*));
		nOffset += sizeof(MICP_BODY_BRD_PARAMS)-sizeof(MU_INT8*);
		
		memcpy(g_SendBuf+nOffset,&my_content1,sizeof(Audio_Info)-sizeof(MU_INT8*));
		nOffset += sizeof(Audio_Info)-sizeof(MU_INT8*);
		
		memcpy(g_SendBuf+nOffset,&extradata1,my_content1.ExtraDataSize);
		nOffset += my_content1.ExtraDataSize;

		//printf("audio param size = %x\n",tbrd_param.size);
		
	}
	

	pmicp_hdr->body_len  = nOffset-sizeof(*pmicp_hdr);
	pmicp_hdr->size      = nOffset;
	

	{
		MS_CHAR msg[64];		
		sprintf(msg,"send (SendRespToHost) packet size = %d\n",nOffset)	;
		//LogPrintf(msg,DEF_MSG_PRINT,0);
	}

	
	return  sendComToHostW(comid,g_SendBuf,nOffset,NULL,0);

}
*/
/*
MU_INT32 sendDataFrmBufW(MS_INT32 comid,MU_INT32 channel,MU_INT8 *send_buf)
{
	FrameOut tframeOut;
	MU_INT32 nlenght,len;	
	
	MICP_HDR *pMicp_hdr = (MICP_HDR*)send_buf;
	MICP_BODY_BRD_VIDEO *pmicp_body_head = (MICP_BODY_BRD_VIDEO*)(send_buf+sizeof(MICP_HDR));
	

	if(MD_OK != readDataFrmBuf(channel,&tframeOut))
		return MD_OK;
		
	memset(pMicp_hdr,0,sizeof(*pMicp_hdr));	
	memset(pmicp_body_head,0,sizeof(*pmicp_body_head));	
		
	pmicp_body_head->channel =  tframeOut.rtmpPackets.m_nChannel;
	pmicp_body_head->timstamp = tframeOut.timeStamp;
	pmicp_body_head->keyframe = tframeOut.keyframe;	
	pmicp_body_head->none[0]  = tframeOut.rtmpPackets.m_nInfoField[0];
	pmicp_body_head->none[1]  = tframeOut.rtmpPackets.m_nInfoField[1];
	pmicp_body_head->none[2]  = tframeOut.rtmpPackets.m_nInfoField[2];
	pmicp_body_head->none[3]  = tframeOut.rtmpPackets.m_nInfoField[3];
	
	pMicp_hdr->dev			= DEF_MICP_DEV_BRD;
	pMicp_hdr->direct		= DEF_MICP_DIRECT_BRD_TO_HOST;
	pMicp_hdr->type			= tframeOut.rtmpPackets.m_packetType;
	pMicp_hdr->body_len		= tframeOut.Outlen + sizeof(MICP_BODY_BRD_VIDEO)-sizeof(MS_CHAR *);
	pMicp_hdr->seq			= g_boardBaseInfo.sequence_id;
	pMicp_hdr->ext_info		= 0x00;//?
	pMicp_hdr->devid		= g_boardBaseInfo.card_no;	
	pMicp_hdr->ident		= DEF_MICP_IDENT_BRD_DM642;//DM642(作为子卡)
	pMicp_hdr->none1		= g_boardBaseInfo.chaoping;
	
	pMicp_hdr->size = pMicp_hdr->body_len + sizeof(*pMicp_hdr);
	
	nlenght =  sizeof(MICP_HDR)+sizeof(MICP_BODY_BRD_VIDEO)-sizeof(MS_CHAR *);
	
	//if (tframeOut.Outlen<=0 || tframeOut.Outlen>0x4000)
	//	printf("-------------index:%d---buffsize:%08x---frameType:%d------\n",channel ,tframe.outputSize,tframe.frameType);

	if (tframeOut.Outlen>DEF_SEND_BUGGER_LEN-nlenght)
	{

		//LogPrintf("Frame is too larger than send buffer\n",DEF_MSG_PRINT,0);
		return MD_FAILED;
		
	}

	len = sendComToHostW(comid,(MS_CHAR*)send_buf,nlenght,tframeOut.pOut,tframeOut.Outlen);

	//if (channel == 0 && 0x1000<(MU_INT32)tframeOut.Outlen)
	//	printf("index:%d,Send Size =%08x,frameSize = %08x,result =%08x\n",channel,nlenght+tframeOut.Outlen,tframeOut.Outlen,len );

	if(len == (nlenght+tframeOut.Outlen))
	{
		releaseReadBuf(channel);
		
		//#define DEBUG_TEST
		#ifdef DEBUG_TEST
		{	
			static MU_INT32 num = 0;
			MS_CHAR msg[64];		
			sprintf(msg,"send %d packet size = %d\n",num++,len)	;
			//LogPrintf(msg,DEF_MSG_PRINT,0);
		}
		#endif
		
	}

	return len;
}
*/
MU_INT32 sendFrmBufToServer(MU_INT32 nChannel,MU_INT8 *send_buf)
{
 //   FrameOut tframeOut;
    MU_INT32 len;	
	MS_CHAR msg[DEF_MAX_CHARSTR_LENGHT2];
    MU_INT32 comid = MD_INVALID;
    MS_CHAR tempaddr[20];

    //if(MD_OK != readDataFrmBuf(nChannel,&tframeOut))
    //    return MD_OK;


    //if (tframeOut.Outlen>DEF_SEND_BUGGER_LEN-nlenght)
    //{

    //	printf("Frame is too larger than send buffer\n");
    //	return MD_FAILED;

    //}
    //sprintf(tempaddr,"%d.%d.%d.%d",tframeOut.ipAddr & 0xFF,tframeOut.ipAddr>>8 & 0xFF,tframeOut.ipAddr>>16 & 0xFF,tframeOut.ipAddr>>24);

	int nPort=10000;
	sprintf(tempaddr,"192.168.0.58");
	char pOut[1024]="11";
	int Outlen=1024;

    if(MD_OK != openNewSession((MS_INT32*)&comid,tempaddr,nPort))
    {
        //sprintf(msg,"ERROR:connect %s:%d error\n",tempaddr,tframeOut.nPort);
        ////LogPrintf(msg,DEF_MSG_PRINT|DEF_MSG_SAVE,0);
        return MD_FAILED;
    }

    if(comid == MD_INVALID)
        return MD_FAILED;

    len = sendComToHostIm(comid,NULL,0,(MS_CHAR*)pOut,Outlen);

    if(len == (Outlen))
    {
      MS_INT32 rtn = MD_OK;
      rtn = checkHumanResponse(comid);
      //if(rtn != MD_OK)
      //    return MD_FAILED;
        
      //releaseReadBuf(nChannel);
      //yellow_light(0x01);

        //#define DEBUG_TEST
#ifdef DEBUG_TEST
        {	
            //MS_CHAR msg[64];
            static MU_INT32 nTimes = 0;

            sprintf(msg,"SEND %d  packet size = %d of %d\n",nTimes++,len,nChannel);
            //LogPrintf(msg,DEF_MSG_PRINT,0);
        }
#endif

    }

    if( comid != MD_INVALID )
    {
        closeSession(&comid);
        comid = MD_INVALID;
        return MD_OK;
    }
	return MD_OK;
}

#define DEF_STR_CONTEN_LENGTH        "Content-Length:"


#if 1
MS_INT32 parseHttpHead(MS_CHAR *const buf,MS_INT32 *pLen)
{
	MICP_HDR *pTmp = (MICP_HDR *)buf;

	if(pTmp->dev  != 0x03)//"转码服务器" 和 "拉流程序" 通讯
	{
		TRACE("parseHttpHead:pTmp->dev  != 0x03\n");
		goto leave;
	}
	if(pTmp->direct != 0x01)// "转码服务器" ==>"拉流程序"
	{
		TRACE("parseHttpHead:pTmp->direct != 0x01\n");

		goto leave;
	}
	if(pTmp->type   != 0x01)//请求参数
	{
		TRACE("parseHttpHead:pTmp->type   != 0x01\n");

		goto leave;
	}

	if(pTmp->size - pTmp->body_len !=sizeof(MICP_HDR))
	{
		TRACE("parseHttpHead:ppTmp->size - pTmp->body_len !=sizeof-MICP_HDR\n");

		goto leave;
	}

	*pLen=pTmp->body_len;

	return MD_OK;
leave:
	return MD_FAILED;

}
#else
MS_INT32 parseHttpHead(MS_CHAR *const buf,MS_INT32 *pLen)
{
	MS_INT32 rtn = MD_OK;
	MS_CHAR *pkey;

	pkey = strstr(buf,DEF_STR_CONTEN_LENGTH);
	if(pkey != NULL)
	{
		*pLen = atoi(pkey+strlen(DEF_STR_CONTEN_LENGTH));
	}
	else
	{
		*pLen = 0;
		return MD_FAILED;
	}
	return rtn;
}
#endif

MS_INT32 recvHttpHead(MS_INT32 s,MS_CHAR *pBuf,MS_INT32 maxLen)
{
	MS_INT32 tmpLen = 0;
	MS_INT32 rtn = MD_OK;
	MS_CHAR *pKeyBuf;
	MS_CHAR msg[DEF_MAX_CHARSTR_LENGHT2];
#if 1
	while(tmpLen < maxLen)
	{
		rtn = readDataFrmCommW(s,(MU_CHAR*)(pBuf+tmpLen),maxLen-tmpLen,SOCKET_TCP_MODE,DEF_COMM_READ_WAITTIME_W,1);
		if(rtn < 0)
		{
			m_Log.Add("recvHttpHead:readDataFrmCommW fail tmpLen[%d],maxLen[%d]\n",tmpLen,maxLen);
			TRACE("recvHttpHead:readDataFrmCommW fail tmpLen[%d],maxLen[%d]\n",tmpLen,maxLen);
			
			
			return MD_FAILED;
		}

		tmpLen += rtn;
	}
	return MD_OK;

#else
	while(1)
	{
		rtn = readDataFrmCommW(s,(MU_CHAR*)(pBuf+tmpLen),1,SOCKET_TCP_MODE,DEF_COMM_READ_WAITTIME_W,1);
		if(rtn < 0)
		{
			sprintf(msg,"recvHttpHead:readDataFrmCommW fail \n");
			m_Log.Add(msg);
			return MD_FAILED;
		}

		tmpLen += rtn;

		if(tmpLen >= (maxLen))
		{
			sprintf(msg,"recvHttpHead:tmpLen[%d] >= maxLen[%d] \n",tmpLen,maxLen);
			m_Log.Add(msg);
			return MD_FAILED;
		}

	//	pKeyBuf = strstr(pBuf, "\r\n\r\n");

	//	if(pKeyBuf != NULL)
	//	{
  		//	sprintf(msg,"recvHttpHead:strstr fail \n");
		//	m_Log.Add(msg);
	//		return MD_OK;
	//	}

	}
#endif
	return rtn;
}
MS_INT32 checkHumanResponse(MS_INT32 s)
{
    MS_INT32 rtn = MD_OK;

    MS_CHAR buf[DEF_BUFFER_LENGTH_1K],*dataBuf=NULL;
    MS_INT32 nLen = 0;

    memset(buf,0,DEF_BUFFER_LENGTH_1K);
    rtn = recvHttpHead(s,buf,DEF_BUFFER_LENGTH_1K);
    if(rtn != MD_OK)
    {
        printf("ERROR: checkHumanResponse -> recvHttpHead \n");
        return rtn;

    }

    rtn =  parseHttpHead(buf,&nLen);
    if(rtn != MD_OK || nLen == 0)
    {
        printf("ERROR: checkHumanResponse -> parseHttpHead \n");
        return rtn;
    }
    
    dataBuf = (MS_CHAR*)malloc(nLen*sizeof(MS_CHAR));
    memset(dataBuf,0,nLen);

    rtn = readDataFrmCommW(s,(MU_CHAR*)dataBuf,nLen,SOCKET_TCP_MODE,DEF_COMM_READ_WAITTIME_W,1);
    if(rtn < 0)
    {
        printf("\nERROR: checkHumanResponse->readDataFrmCommW error\n");
        return rtn;
    }

    //rtn =  parseHumanCountContent(dataBuf,nLen);
    //if(rtn != MD_OK)
    //{
    //    printf("ERROR: checkHumanResponse is lawless \n");
    //    return rtn;
    //}


    return rtn;
}

MS_INT32 checkMetricsResponse(MS_INT32 s)
{
    MS_INT32 rtn = MD_OK;
	MS_CHAR *pkey;

    MS_CHAR buf[DEF_BUFFER_LENGTH_1K],*dataBuf=NULL;
    MS_INT32 nLen = 0;

    memset(buf,0,DEF_BUFFER_LENGTH_1K);
    rtn = recvHttpHead(s,buf,DEF_BUFFER_LENGTH_1K);
    if(rtn != MD_OK)
    {
        printf("ERROR: checkMetricsResponse -> recvHttpHead \n");
        return rtn;

    }

	

	pkey = strstr(buf,"HTTP/1.0 200 OK");
	if(pkey == NULL)
		rtn = MD_FAILED;


    return rtn;
}

MU_INT32 sendStateToServer(MU_INT8 *pBuf1,MS_INT32 len1,MU_INT32 *pBuf2,MS_INT32 len2,MS_CHAR *pIp,MS_INT32 port)
{
    MS_INT32 len;	
    MU_INT32 s = MD_INVALID;
	MS_CHAR msg[DEF_MAX_CHARSTR_LENGHT2];

    if(MD_OK != openNewSession((MS_INT32*)&s,pIp,port))
    {
        sprintf(msg,"ERROR:connect %s:%d error\n",pIp,port);
        //LogPrintf(msg,DEF_MSG_PRINT|DEF_MSG_SAVE,0);
        
        return MD_FAILED;
    }


    len = sendComToHostIm(s,(MS_CHAR*)pBuf1,len1,(MS_CHAR*)pBuf2,len2);

    if(len == (len1+len2))
    {

        MS_INT32 rtn = MD_OK;
        rtn = checkMetricsResponse(s);

    }else
    {

    }

    if( s != MD_INVALID )
    {
        closeSession(&s);
        s = MD_INVALID;
    }
	return MD_OK;
}



#include "./xml/include/xmlnode.h"
#include "./xml/include/xmlparse.h"
#define MAX_LEN 256
#define MAX_LEN_MIN 4*1024
#define MAX_ITEM_SIZE 20


MS_INT32 parseGDContent(MS_CHAR*pXmlBuf)
{
#if 0
	if(NULL != strstr(pXmlBuf,"ok"))
	{
	//	Main->m_MicrovideoParams.nPort=10000;
	//	Main->m_MicrovideoParams.PubName="TEST_JC";
	//	Main->m_MicrovideoParams.ServerIP="192.168.100.113";
		Main->m_MicrovideoParams.SrcUrl="http://10.20.90.90:8080/CD2";
		Main->m_MicrovideoParams.StreamNumber=0;
		return MD_OK;
	}
#endif
	int rt;
	CString resultStr="",tmpStr,serverStr;
	DEV_VNMP_PARAMS tmpVNMP;

	XMLNODE *root = NULL;
	XMLNODE *subroot = NULL;

	XMLNODE *node = NULL,*chNode=NULL;
	XML xml;

	CString CH;

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


	if (strcmp(xml.root->name, "Command") != 0)
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
//	subroot = f_xmlnode_sch_elem(root, "microvideo");
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
		
		CH = chNode->data;
		if(atoi(CH)>=g_maxLiveNum)
		{
			int ntmp = atoi(CH);
			ntmp = ntmp%g_maxLiveNum;
			CH.Format("%d",ntmp);
		}
	//	Main->m_MicrovideoParams.StreamNumber=atoi(CH);
		rt=MD_OK;
		m_Log.Add("解析到的ch: %s",CH);
	}
	else
	{
		m_Log.Add("xml解析错误，ch节点的值为空!");
		rt = 400;
		goto leave;
	}

	
	strcpy(Main->m_devParams.m_proxyIp,tmpVNMP.m_proxyIp);
	Main->m_devParams.m_proxyPort = tmpVNMP.m_proxyPort;
	strcpy(Main->m_devParams.m_userName,tmpVNMP.m_userName);
	strcpy(Main->m_devParams.m_passWord,tmpVNMP.m_passWord);
	strcpy(Main->m_devParams.m_cameraId,tmpVNMP.m_cameraId);

	

leave:
	f_xmlnode_del(&xml);

leaveOut:

	return rt;

}
MS_INT32 checkGDServerResponse(MS_INT32 s)
{
    MS_INT32 rtn = MD_OK;

    MS_CHAR buf[DEF_BUFFER_LENGTH_1K],*dataBuf=NULL;
    MS_INT32 nLen = 0;

    memset(buf,0,DEF_BUFFER_LENGTH_1K);
    rtn = recvHttpHead(s,buf,sizeof(MICP_HDR));
    if(rtn != MD_OK)
    {
		TRACE("checkGDServerResponse:recvHttpHead fail buf[%s]\n",buf);
		m_Log.Add("checkGDServerResponse:recvHttpHead fail buf[%s]\n",buf);
        return MD_FAILED;
    }

//	m_Log.Add("checkGDServerResponse:buf[%s]",buf);
    rtn =  parseHttpHead(buf,&nLen);
    if(rtn != MD_OK || nLen == 0)
    {
		TRACE("checkGDServerResponse:parseHttpHead fail buf[%s]\n",buf);

        return MD_FAILED;
    }	
	TRACE("checkGDServerResponse:parseHttpHead nLen[%d]\n",nLen);
    
    dataBuf = (MS_CHAR*)malloc(nLen*sizeof(MS_CHAR)+1);
    memset(dataBuf,0,nLen*sizeof(MS_CHAR)+1);

    rtn = readDataFrmCommW(s,(MU_CHAR*)dataBuf,nLen,SOCKET_TCP_MODE,DEF_COMM_READ_WAITTIME_W,1);
    if(rtn < 0)
    {
		TRACE("checkGDServerResponse:readDataFrmCommW fail \n");
        goto leave;
    }

    rtn =  parseGDContent(dataBuf);
    if(rtn != MD_OK)
    {
		TRACE("checkGDServerResponse:parseGDContent fail \n");
        goto leave;
	}else{
        goto done;

	}

leave:
	if(dataBuf)
	{
		free(dataBuf);
	}
	return MD_FAILED;
done:
	if(dataBuf)
	{
		free(dataBuf);
	}
    return MD_OK;
}

MU_INT32 sendRequireToServer(MU_INT8 nDevId)
{
    MS_INT32 len;	
    MU_INT32 s = MD_INVALID;
	MS_INT32 len1=sizeof(MICP_HDR);
	MU_INT32 *pBuf2=NULL;
	MS_INT32 len2=0;
//	MS_CHAR *pIp="192.168.0.58";
	MS_CHAR *pIp=g_ip;

	MS_INT32 port=g_port;
    if(MD_OK != openNewSession((MS_INT32*)&s,pIp,port))
    {
		m_Log.Add("sendRequireToServer:openNewSession connect %s:%d error\n",pIp,port);
		TRACE("sendRequireToServer:openNewSession connect %s:%d error\n",pIp,port);
        return MD_FAILED;
    }
	//	TRACE("sendRequireToServer:openNewSession connect %s:%d suc\n",pIp,port);

	MICP_HDR sMicpHdr;
	memset(&sMicpHdr,0,sizeof(MICP_HDR));
	//pmicp_hdr = (MICP_HDR*)(&SendBufHead);

	len = 0;

	sMicpHdr.size      = sizeof(MICP_HDR)+len;
	sMicpHdr.dev   	 = 0x03;//"转码服务器" 和 "拉流程序" 通讯
	sMicpHdr.direct 	 = DEF_MICP_DIRECT_BRD_TO_HOST;//"拉流程序" ==> "转码服务器"
	sMicpHdr.type      = 0x01;//请求参数
	sMicpHdr.devid     = nDevId;//reversed
	sMicpHdr.body_len  = len;
	sMicpHdr.seq		 = 0;//reversed
	sMicpHdr.ext_info  = 0x00;//reversed
	sMicpHdr.ident     = 0x00003001;//拉流子程序
	sMicpHdr.none1	=0;//reversed
	sMicpHdr.none2  =0;//reversed

    len = sendComToHostW(s,(MS_CHAR*)&sMicpHdr,sizeof(MICP_HDR),(MS_CHAR*)pBuf2,len2);

    if(len == (len1+len2))
    {

        MS_INT32 rtn = MD_OK;
        rtn = checkGDServerResponse(s);
		if(rtn == MD_OK)
		{
			if( s != MD_INVALID )
			{
				closeSession(&s);
				s = MD_INVALID;
			}
			return MD_OK;
		}else{
			TRACE("sendRequireToServer:checkGDServerResponse fail socket[%d]\n",s);
			m_Log.Add("sendRequireToServer:checkGDServerResponse fail socket[%d]",s);
			goto leave;
		}

    }else
    {
		TRACE("sendRequireToServer:sendComToHostW fail len[%d]\n",len);
		m_Log.Add("sendRequireToServer:sendComToHostW fail len[%d]\n",len);
		goto leave;
    }


leave:
	if( s != MD_INVALID )
	{
		closeSession(&s);
		s = MD_INVALID;
	}
	return MD_FAILED;
}

MS_INT32 FormatFrameHead(MS_CHAR* dst,MU_INT8 devid,MU_INT32 bodyLen)
{//首次主动向HOST发送信息，请求参数
	MS_INT32 len;
	MICP_HDR *pmicp_hdr;
	MS_CHAR	msg[DEF_MAX_CHARSTR_LENGHT4];
	
	MICP_HDR SendBufHead;
	memset(&SendBufHead,0,sizeof(MICP_HDR));
	pmicp_hdr = (MICP_HDR*)(&SendBufHead);

	
	pmicp_hdr->size      = sizeof(MICP_HDR);
	pmicp_hdr->dev   	 = DEF_MICP_DEV_BRD;
	pmicp_hdr->direct 	 = DEF_MICP_DIRECT_BRD_TO_HOST;
	pmicp_hdr->type      = DEF_MICP_TYPE_BRD_ADJUST;
	pmicp_hdr->devid     = devid;
	pmicp_hdr->body_len  = bodyLen;
	pmicp_hdr->seq		 = 0x00;
	pmicp_hdr->ext_info  = 0x00;
	pmicp_hdr->ident     = DEF_MICP_IDENT_BRD_DM642;

	memcpy(dst,(MS_CHAR*)pmicp_hdr,sizeof(MICP_HDR));
	
	return MD_OK;
	
}
static int nTimess=0;
static MU_INT32 fdClient=MD_INVALID;
MU_INT32 sendDataToServer(MU_INT8 devId)//MU_INT8 *pBuf1,MS_INT32 len1,MU_INT32 *pBuf2,MS_INT32 len2,MS_CHAR *pIp,MS_INT32 port)
{
    MS_INT32 len;	
  //  MU_INT32 s = MD_INVALID;
    MS_CHAR tempaddr[20];
	MyAvFrame *m_Frame=NULL;

	//MS_CHAR *pIp="192.168.100.114";
	//MS_CHAR *pIp="192.168.0.58";
	MS_CHAR *pIp=g_ip;

	MS_INT32 port=g_port;
    //MU_INT32 dd;	

    //sprintf(tempaddr,"%d.%d.%d.%d",pIp & 0xFF,pIp>>8 & 0xFF,pIp>>16 & 0xFF,pIp>>24);

	if(fdClient==MD_INVALID)
	{
		if(MD_OK != openNewSession((MS_INT32*)&fdClient,pIp,port))
			//if(MD_OK != openNewSession((MS_INT32*)&s,"192.168.0.15",8888))
		{
			m_Log.Add("sendDataToServer:openNewSession %s:%d error\n",pIp,port);
			return MD_FAILED;
		}
	}

	//
	m_Frame=NULL;
	Main->m_streamlock.Lock();
	if (Main->p_YuvArray.GetSize()>0)
	{
		m_Frame=Main->p_YuvArray.ElementAt(0);
		Main->p_YuvArray.RemoveAt(0);
	}
	Main->m_streamlock.Unlock();	
	if (NULL!=m_Frame)//从模板CArray获取到了视频数据
	{

		MICP_HDR sMicpHdr;
		memset(&sMicpHdr,0,sizeof(MICP_HDR));
		//pmicp_hdr = (MICP_HDR*)(&SendBufHead);

		len = (m_Frame->i_height*m_Frame->i_width)*3/2;


		sMicpHdr.size      = sizeof(MICP_HDR)+len;
		sMicpHdr.dev   	 = 0x03;
		sMicpHdr.direct 	 = DEF_MICP_DIRECT_BRD_TO_HOST;
		sMicpHdr.type      = DEF_MICP_TYPE_BRD_SET;
		sMicpHdr.devid     = devId;
		sMicpHdr.body_len  = len;
		sMicpHdr.seq		 = nTimess++;
		sMicpHdr.ext_info  = 0x00;
		sMicpHdr.ident     = 0x00003001;
		sMicpHdr.none1	=m_Frame->i_width;
		sMicpHdr.none2  =m_Frame->i_height;



	//
		len = sendComToHostW(fdClient,(MS_CHAR*)&sMicpHdr,sizeof(MICP_HDR),
			(MS_CHAR*)m_Frame->p_data,(m_Frame->i_height*m_Frame->i_width)*3/2);


		if(len == (m_Frame->i_height*m_Frame->i_width)*3/2 + sizeof(MICP_HDR))
		{
			//TRACE("sendDataToServer:len suc\n");

			if(m_Frame)
			delete m_Frame;

			
		}else
		{
			TRACE("sendDataToServer:sendComToHostW fail,len[%d]\n",len);

			m_Log.Add("sendDataToServer:sendComToHostW fail,len[%d]\n",len);

			if( fdClient != MD_INVALID )
			{
				closeSession(&fdClient);
				fdClient = MD_INVALID;

			}
			
			if(m_Frame)
				delete m_Frame;

			return MD_FAILED;
		}
		
	}else{
		return MD_NODATA;
	}
    //if( fdClient != MD_INVALID )
    //{
    //    closeSession(&fdClient);
    //    fdClient = MD_INVALID;
    //}

	return MD_OK;
}

/*
MU_INT32 sendStateInfo(MS_INT32 Comid)//,DSP_STAT *pdsp_stat
{
	MICP_HDR tMicp_hdr;
	MS_INT32 bFlag;
	BOARDINFO bi ;
	
	memset(&tMicp_hdr,0,sizeof(tMicp_hdr));
	memset(&bi,0,sizeof(bi));
		
	bi.code = MAKE_SX_XR_EXT(XR_GINF,(XR_DEV>>16)| (g_boardBaseInfo.card_no & 0xFF) , 3, g_stateInfo & 0x1FFF);
	bi.gtime = GetBoardTimeSec();
	//dequeue_sq( &bi);
	//if(bi.code == 0)
	//	return MD_OK;
	
	tMicp_hdr.dev = DEF_MICP_DEV_BRD;
	tMicp_hdr.direct = DEF_MICP_DIRECT_BRD_TO_HOST;
	tMicp_hdr.type = DEF_MICP_TYPE_BRD_WARN;//DEF_MICP_TYPE_BRD_PING;
	tMicp_hdr.body_len = 0;//sizeof(DSP_STAT);
	tMicp_hdr.seq = g_boardBaseInfo.sequence_id;
	tMicp_hdr.ext_info = bi.code;//?
	tMicp_hdr.devid = g_boardBaseInfo.card_no;	
	tMicp_hdr.ident = DEF_MICP_IDENT_BRD_DM642;//DM642(作为子卡)
	tMicp_hdr.none1	= bi.gtime;
	
	tMicp_hdr.size = sizeof(tMicp_hdr) + tMicp_hdr.body_len;
	//bFlag = SendData( &tMicp_hdr,pdsp_stat,sizeof(MICP_HDR),sizeof(DSP_STAT) ,1 );
	bFlag = sendComToHostIm(Comid,(MS_CHAR *)&tMicp_hdr,sizeof(MICP_HDR),NULL,0);//pdsp_stat,sizeof(DSP_STAT)

	//g_stateInfo = 0 ;
	
	if(bFlag == (sizeof(MICP_HDR)))//+sizeof(DSP_STAT)
	{
		//#define DEBUG_TEST
		#ifdef DEBUG_TEST
		{	
			static MU_INT32 num = 0;
			MS_CHAR msg[64];		
			sprintf(msg,"send %d state info time = %u\n",num++,bi.gtime)	;
			//LogPrintf(msg,DEF_MSG_PRINT,0);
		}
		#endif

		//printf("send status is 0x%x\n",bi.code);
	}	
	
	return MD_OK;
}
*/

/*
MU_INT32 sendWarnFrmBuf(MS_INT32 Comid)
{
	MICP_HDR tMicp_hdr;
	MS_INT32 bFlag;
	BOARDINFO bi ;//必须有初始值，否则会是随机数
	
	memset(&tMicp_hdr,0,sizeof(tMicp_hdr));
	memset(&bi,0,sizeof(bi));
	
	dequeue_sq( &bi);
	if((bi.code&0x1FFF) == 0)
		return MD_OK;
		
	
	tMicp_hdr.dev = DEF_MICP_DEV_BRD;
	tMicp_hdr.direct = DEF_MICP_DIRECT_BRD_TO_HOST;
	tMicp_hdr.type = DEF_MICP_TYPE_BRD_WARN;
	tMicp_hdr.body_len = 0;
	tMicp_hdr.seq = g_boardBaseInfo.sequence_id;
	tMicp_hdr.ext_info = bi.code;
	tMicp_hdr.devid = g_boardBaseInfo.card_no;	
	tMicp_hdr.ident = DEF_MICP_IDENT_BRD_DM642;//DM642(作为子卡)
	tMicp_hdr.none1	= bi.gtime;
	
	tMicp_hdr.size = sizeof(tMicp_hdr) + tMicp_hdr.body_len;
	//bFlag = SendData( &tMicp_hdr,pdsp_stat,sizeof(MICP_HDR),sizeof(DSP_STAT) ,1 );
	bFlag = sendComToHostIm(Comid,(MS_CHAR *)&tMicp_hdr,sizeof(MICP_HDR),NULL,0);//pdsp_stat,sizeof(DSP_STAT)

	if(bFlag == (sizeof(MICP_HDR)))//+sizeof(DSP_STAT)
	{
		//#define DEBUG_TEST
		#ifdef DEBUG_TEST
		{	
			static MU_INT32 num = 0;
			MS_CHAR msg[64];		
			sprintf(msg,"send %d state info time = %u\n",num++,bi.gtime)	;
			//LogPrintf(msg,DEF_MSG_PRINT,0);
		}
		#endif
	}	
	return MD_OK;
}
*/
/*
MS_INT32 SendTimeRequest(MS_INT32 comid)
{//首次主动向HOST发送信息，请求参数
	MS_INT32 len;
	MICP_HDR *pmicp_hdr;
	MS_CHAR	msg[DEF_MAX_CHARSTR_LENGHT4];
	
	memset(g_SendBuf,0,sizeof(MICP_HDR));
	pmicp_hdr = (MICP_HDR*)(g_SendBuf);
	
	pmicp_hdr->size      = sizeof(MICP_HDR);
	pmicp_hdr->dev   	 = DEF_MICP_DEV_BRD;
	pmicp_hdr->direct 	 = DEF_MICP_DIRECT_BRD_TO_HOST;
	pmicp_hdr->type      = DEF_MICP_TYPE_BRD_ADJUST;
	pmicp_hdr->devid     = g_boardBaseInfo.card_no;
	pmicp_hdr->body_len  = 0x00;
	pmicp_hdr->seq		 = g_boardBaseInfo.sequence_id;
	pmicp_hdr->ext_info  = 0x00;
	pmicp_hdr->ident     = DEF_MICP_IDENT_BRD_DM642;
	
	sprintf(msg,"发送时间校时请求...\n");
	//LogPrintf(msg,DEF_MSG_PRINT ,0);
	
	len = sendComToHostW(comid,(MS_CHAR *)pmicp_hdr,sizeof(MICP_HDR),NULL,0);
	if(len < 0)
		return MD_FAILED;
	
	return MD_OK;
	
}

*/

#endif
