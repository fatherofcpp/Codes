#include <time.h>

#include "StdAfx.h"
#include "rtmp.h" 	
#include "OBJ.h"
#include "kmp.h"
// #include <sys/types.h>
// #include <sys/timeb.h>

#define RTMP_SIG_SIZE 1536 	
#define RTMP_LARGE_HEADER_SIZE 12

static void my_Init(struct RTMP *pRTMP)
{
	int n;
	pRTMP->m_bHandshake = 0;
	pRTMP->m_socket = 0;
	pRTMP->m_bStart = 0;
	pRTMP->m_chunkSize = 128; 	
	pRTMP->m_nBWCheckCounter = 0; 
	pRTMP->m_bSendPars = 0;
	memset(pRTMP->m_sIP, 0, sizeof(pRTMP->m_sIP));
	memset(pRTMP->m_sPath, 0, sizeof(pRTMP->m_sPath));
	memset(pRTMP->m_sLive, 0, sizeof(pRTMP->m_sLive));
	memset(&pRTMP->m_MetaData, 0, sizeof(pRTMP->m_MetaData));

	pRTMP->m_stream_id = 1;
	pRTMP->m_nBytesIn = 0;
	pRTMP->m_nBytesInSent = 0;
	pRTMP->m_nmethodCalls = 0;	
	pRTMP->m_nmethodCallsRead = 0;
	for (n = 0; n < MAX_INVOKE; n++){
		memset(pRTMP->m_methodCalls[n], 0, sizeof(pRTMP->m_methodCalls[0])); 
	}	
	
	for (n = 0; n < 64; n++){
		pRTMP->m_vecChannelsIn[n] = g_RTMPPacket;
		pRTMP->m_vecChannelsOut[n] = g_RTMPPacket;
	}
}

static void my_Close(struct RTMP *pRTMP)
{
	CRTMPPacket packet;
	int i;
	pRTMP->m_bHandshake = 0;
	pRTMP->m_bStart = 0;
	pRTMP->m_bSendPars = 0;
	if (pRTMP->IsConnected(pRTMP)) 	
	  closesocket(pRTMP->m_socket);
		
	pRTMP->m_socket = -1; 	
	pRTMP->m_chunkSize = 128; 	
	pRTMP->m_nBWCheckCounter = 0; 	

	for (i = 0; i < 64; i++){ 
		packet = pRTMP->m_vecChannelsIn[i];	
		packet.Close(&packet); 	
		pRTMP->m_vecChannelsIn[i] = g_RTMPPacket;

		packet = pRTMP->m_vecChannelsOut[i];
		packet.Close(&packet);  
		pRTMP->m_vecChannelsOut[i] = g_RTMPPacket; 	
	} 	

	for (i = 0; i < MAX_INVOKE; i++)
		memset(pRTMP->m_methodCalls[i], 0, sizeof(pRTMP->m_methodCalls[0])); 
 
	pRTMP->m_stream_id = 1;
	pRTMP->m_nBytesIn = 0;
	pRTMP->m_nBytesInSent = 0;
	pRTMP->m_nmethodCalls = 0;
	pRTMP->m_nmethodCallsRead = 0;	
	memset(pRTMP->m_sIP, 0, sizeof(pRTMP->m_sIP));
	memset(pRTMP->m_sPath, 0, sizeof(pRTMP->m_sPath));
	memset(pRTMP->m_sLive, 0, sizeof(pRTMP->m_sLive));
	//memset(&pRTMP->m_MetaData, 0, sizeof(pRTMP->m_MetaData));
} 	


static int my_Connect(struct RTMP *pRTMP, const char *pIP, const char *pPath, const char *pLive)
{ 	
	sockaddr_in service;
	struct hostent *host = NULL;
	pRTMP->Close(pRTMP); 	

	strcpy(pRTMP->m_sIP, pIP);
	strcpy(pRTMP->m_sPath, pPath);
	strcpy(pRTMP->m_sLive, pLive); 

   	
	memset(&service, 0, sizeof(sockaddr_in)); 	
	service.sin_family = AF_INET; 	
	service.sin_addr.s_addr = inet_addr(pRTMP->m_sIP); 	
	if (service.sin_addr.s_addr == INADDR_NONE){ 	
		host = gethostbyname(pRTMP->m_sIP); 	
		if (host == NULL || host->h_addr == NULL)	
			return -1; 	
    
		service.sin_addr = *(struct in_addr*)host->h_addr; 	
	}   	
	service.sin_port = htons(g_rtmpPort);//1935); 	
	pRTMP->m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 	
	if (pRTMP->m_socket > 0){ 	
		if (connect(pRTMP->m_socket, (sockaddr*) &service, sizeof(struct sockaddr)) < 0){ 	
			pRTMP->Close(pRTMP); 	
			return -2; 	
		} 	

		if (pRTMP->HandShake(pRTMP) < 0){	
			pRTMP->Close(pRTMP); 	
			return -3; 	
		} 	

		if (pRTMP->SendConnectPacket(pRTMP) < 0){ 	
			pRTMP->Close(pRTMP);
			return -4; 	
		} 
	} 	
	else { 	
		return -5; 	
	}

	//pRTMP->m_bStart = 1;
	return 1; 	
} 	

static int	my_IsConnected(const struct RTMP *pRTMP)
{
	return (pRTMP->m_socket > 0);
}

static int my_ReadN(struct RTMP *pRTMP, char *buffer, int n)
{ 	
	int nBytes = 0;
	char *ptr = NULL;
	int nOriginalSize = n; 	
	memset(buffer, 0, n); 	
	ptr = buffer; 	
	while (n > 0){     	
		nBytes = recv(pRTMP->m_socket, ptr, n, 0);
		if (nBytes < 0){ 	
			pRTMP->Close(pRTMP); 	
			return -1; 	
		} 	
    
		if (nBytes == 0){ 	
			pRTMP->Close(pRTMP); 	
			break; 	
		} 	
    
		pRTMP->m_nBytesIn += nBytes;
		if (pRTMP->m_nBytesIn > pRTMP->m_nBytesInSent + (125*1024))
			pRTMP->SendBytesReceived(pRTMP); 

		n -= nBytes; 	
		ptr += nBytes; 	
	} 	
	return nOriginalSize - n; 	
} 	

static int my_WriteN(struct RTMP *pRTMP, const char *buffer, int n)
{ 	
	int nBytes;
	const char *ptr = buffer; 	

	while (n > 0){ 	
		nBytes = send(pRTMP->m_socket, ptr, n, 0); 	
		if (nBytes < 0){ 	
			pRTMP->Close(pRTMP); 	
			return -1; 	
		} 	
    
		if (nBytes == 0) 	
			break; 	
    
		n -= nBytes; 	
		ptr += nBytes; 	
	} 
	return 1;	
} 	
 	
static int my_SendConnectPacket(struct RTMP *pRTMP) 	
{ 	
	char sURL[256] = {0};
	int bRtn;
	char *enc = NULL;
	//TRACE("**********send to socket id is %d\n",pRTMP->m_socket);
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);
	packet.m_nChannel = 0x03; 	
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 	
	packet.m_packetType = 0x14;	
	packet.AllocPacket(&packet, 1024); 	
	enc = packet.m_body; 	
	enc += EncodeString_short(enc, "connect"); 	
	enc += EncodeNumber_short(enc, 0); 	
	*enc = 0x03; 	
	enc++; 	
	sprintf(sURL, "rtmp://%s/%s", pRTMP->m_sIP, pRTMP->m_sPath);
	enc += EncodeString(enc, "app", pRTMP->m_sPath); 
	enc += EncodeString(enc, "flashVer", "WIN 9,0,115,0");
	enc += EncodeString(enc, "swfUrl", sURL);
	enc += EncodeString(enc, "tcUrl", sURL);	 
	enc += EncodeBoolean(enc, "fpad", 0);
	enc += EncodeNumber(enc, "capabilities", 15);
	enc += EncodeNumber(enc, "audioCodecs", 1639);
	enc += EncodeNumber(enc, "videoCodecs", 252);
	enc += EncodeNumber(enc, "videoFunction", 1.0);
	enc += EncodeString(enc, "pageUrl", "http://demo.learnall.com/flv3.htm");
	


	enc += 2; 	
	*enc = 0x09; 	
	enc++; 	
	packet.m_nBodySize = enc-packet.m_body; 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn; 	
} 	


static int my_SendCheckBWResult(struct RTMP *pRTMP) 	
{ 	
	int bRtn;
	char *enc = NULL;
	RTMPPacket packet = g_RTMPPacket; 
	packet.Init(&packet);
	packet.m_nChannel = 0x03;	
	packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM; 	
	packet.m_packetType = 0x14;	
	packet.m_nInfoField1 = 0x16 * pRTMP->m_nBWCheckCounter; 	
	packet.AllocPacket(&packet, 256); 	
	enc = packet.m_body; 	
	enc += EncodeString_short(enc, "_result"); 	
	enc += EncodeNumber_short(enc, (double)time(NULL));	
	*enc = 0x05;	
	enc++; 	
	enc += EncodeNumber_short(enc, (double)pRTMP->m_nBWCheckCounter++); 	
	packet.m_nBodySize = enc - packet.m_body; 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn; 	
} 	
 	

static int my_SendPing(struct RTMP *pRTMP, short nType, unsigned int nObject, unsigned int nTime)
{ 	
	int bRtn; 
	char *buf = NULL;
	int nSize;
	RTMPPacket packet = g_RTMPPacket;
 	packet.Init(&packet);
	packet.m_nChannel = 0x02;   	
	packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM; 	
	packet.m_packetType = 0x04; 
	packet.m_nInfoField1 = timeGetTime(); 	
	nSize = (nType==0x03?10:6); 	
	packet.AllocPacket(&packet, nSize); 	
	packet.m_nBodySize = nSize; 	
	buf = packet.m_body; 	
	buf += EncodeInt16(buf, nType); 	
	if (nSize > 2) 	
		buf += EncodeInt32(buf, nObject); 	
	if (nSize > 6) 	
		buf += EncodeInt32(buf, nTime); 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn;  	
} 	

static int my_ConnectIsOK(const struct RTMP *pRTMP, const COBJ obj)
{
	COBJProperty *p = NULL;
	COBJProperty *p1 = NULL;
	COBJ *o = NULL;
	int dt;
	int nItem;
	int bRtn;
	int n;
	int nOK = 0;	
	int i;
	char *sVal;
	char *sName;	
	int nCount = obj.GetPropertyCount(&obj);

	for (i = 0; i < nCount; i++){
		p = obj.GetProperty(&obj, i);
		dt = p->GetType(p);
		if (dt == 0x03){
			//p = obj.GetProperty(&obj, i);
			o = p->GetObject(p);
			nItem = o->GetPropertyCount(o);
			for (n = 0; n < /*nItem*/nCount; n++){
				p1 = o->GetProperty(o, n);
				sName = p1->GetPropName(p1);
				if (strcmp(sName, "level") == 0){
					sVal = p1->GetString(p1);
					if (strcmp(sVal, "status") == 0)
						nOK++;
				}
				if (strcmp(sName, "code") == 0){
					p1 = o->GetProperty(o, n);
					sVal = p1->GetString(p1);
					if (strcmp(sVal, "NetConnection.Connect.Success") == 0)
						nOK++;
				}				
			}
		}
	}

	(nOK == 2) ? bRtn = 1 : bRtn = -1;
	return bRtn;
}

static int my_FCPublishIsOK(const struct RTMP *pRTMP, const COBJ obj)
{
	COBJProperty *p = NULL;
	COBJProperty *p1 = NULL;
	COBJ *o = NULL;
	int dt;
	int i;
	int n;
	int nItem;
	int nCount = obj.GetPropertyCount(&obj);
	int nOK = 0;
	char *sVal;
	char *sName;
	int bRtn;
		
	for (i = 0; i < nCount; i++){
		p = obj.GetProperty(&obj, i);
		dt = p->GetType(p);
		if (dt == 0x03){
			p = obj.GetProperty(&obj, i);
			o = p->GetObject(p);
			nItem = o->GetPropertyCount(o);
			for (n = 0; n < nItem; n++){
				p1 = o->GetProperty(o, n);
				sName = p1->GetPropName(p1);
				if (strcmp(sName, "code") == 0){
					sVal = p1->GetString(p1);
					if (strcmp(sVal, "NetStream.Publish.Start") == 0)
						nOK++;
				}				
			}
		}
	}

	(nOK == 1) ? bRtn = 1 : bRtn = -1;
	return bRtn;
}


static int my_PublishIsOK(const struct RTMP *pRTMP, const COBJ obj)
{
	COBJProperty *p = NULL;
	COBJProperty *p1 = NULL;
	COBJ *o = NULL;	
	int dt;
	int i;
	int nCount = obj.GetPropertyCount(&obj);
	int nOK = 0;
	char *sVal;
	char *sName;
	int bRtn;
	int nItem;
	int n;
		
	for (i = 0; i < nCount; i++){
		p = obj.GetProperty(&obj, i);
		dt = p->GetType(p);
		if (dt == 0x03){
			p = obj.GetProperty(&obj, i);
			o = p->GetObject(p);
			nItem = o->GetPropertyCount(o);
			for (n = 0; n < nItem; n++){
				p1 = o->GetProperty(o, n);
				sName = p1->GetPropName(p1);
				if (strcmp(sName, "level") == 0){
					sVal = p1->GetString(p1);
					if (strcmp(sVal, "status") == 0)
						nOK++;
				}
				if (strcmp(sName, "code") == 0){
					p1 = o->GetProperty(o, n);
					sVal = p1->GetString(p1);					
					if (strcmp(sVal, "NetStream.Publish.Start") == 0)
						nOK++;
				}	
				if (strcmp(sName, "clientid") == 0){

				}
			}
		}
	}
	(nOK == 2) ? bRtn = 1 : bRtn = -1;
	return bRtn;
}

static void my_HandleChangeChunkSize(struct RTMP *pRTMP, CRTMPPacket *packet)
{ 	
  if (packet->m_nBodySize >= 4) 	
    pRTMP->m_chunkSize = ReadInt32(packet->m_body);
} 	
 	
static void my_HandlePing(struct RTMP *pRTMP, CRTMPPacket *packet) 	
{ 	
	unsigned int nTime;
	short nType = -1; 	
	if (packet->m_body && packet->m_nBodySize >= 2) 	
		nType = ReadInt16(packet->m_body); 	
	if (nType == 0x06 && packet->m_nBodySize >= 6){ 	
		nTime = ReadInt32(packet->m_body + 2); 	
		pRTMP->SendPing(pRTMP, 0x07, nTime, 0); 	
	} 	
} 	
#if 0
static int my_ReadPacket(struct RTMP *pRTMP, CRTMPPacket **packet)
{ 	
	int nChunk;
	int nToRead;
	int nSize;
	char type; 	
	int headerType,nChannel,nInfoField1 = -1,packetType = -1,nInfoField2 = -1;
	CRTMPPacket *p = NULL;
	char header[RTMP_LARGE_HEADER_SIZE] = {0};

	*packet = NULL;
	if (pRTMP->ReadN(pRTMP, &type, 1) != 1)//读包头类型失败 	
		return -1; 	

	headerType = (type & 0xc0) >> 6; 	
	nChannel = (type & 0x3f); 	
	nSize = packetSize[headerType]; 	

	p = &pRTMP->m_vecChannelsIn[nChannel]; 

	if (nSize == RTMP_LARGE_HEADER_SIZE) {
		p->m_headerType = headerType;
		p->m_nChannel = nChannel;
	}

	nSize--; 

	if (nSize > 0 && pRTMP->ReadN(pRTMP, header,nSize) != nSize)//有后继包头,但是读取失败 	
		return -2;

	if (nSize >= 3)//包头有时间戳 	
		nInfoField1 = ReadInt24(header);

	if (nSize >= 6){//包头有长度,说明是新包,释放原来该通道的包内容 	
		p->m_nBodySize = ReadInt24(header + 3); 	
		p->m_nBytesRead = 0; 	
		p->FreePacket(p); 	
		if(p->m_nBodySize > 1024)
			return -3;
	}

	if (nSize > 6) //包类型	
		packetType = header[6]; 

	if (nSize == 11)//流ID 	
		nInfoField2 = ReadInt32(header+7);

	//包分配失败
	if (p->m_nBodySize == 261)
		printf("-------->\n");
	if (p->m_nBodySize > 0 && p->m_body == NULL && (p->AllocPacket(p, p->m_nBodySize) < 0)) 	
		return -3;


	nToRead = p->m_nBodySize - p->m_nBytesRead; 	
	nChunk = pRTMP->m_chunkSize; 	
	if (nToRead < nChunk) 	
		nChunk = nToRead;
	if (pRTMP->ReadN(pRTMP, p->m_body + p->m_nBytesRead, nChunk) != nChunk){ 	
		p->FreePacket(p);  	
		return -4;  	
	} 	
	p->m_nBytesRead += nChunk; 	

	if (nInfoField1 != -1)
		p->m_nInfoField1 = nInfoField1;
	if (packetType != -1)
		p->m_packetType = packetType;
	if (nInfoField2 != -1)
		p->m_nInfoField2 = nInfoField2;

	if (p->IsCompleted(p)) {//一个包完整读完
		*packet = p;
		return p->m_nChannel;	
	} 	
	else 	
		return 0;

}
#endif

#if 0
static int my_ReadPacket(struct RTMP *pRTMP, CRTMPPacket **packet)
{ 	
	int nChunk;
	int nToRead;
	int nSize;
	char type; 	
	int headerType,nChannel,nInfoField1 = -1,packetType = -1,nInfoField2 = -1;
	CRTMPPacket *p = NULL;
	char header[RTMP_LARGE_HEADER_SIZE] = {0};
	
	*packet = NULL;
	if (pRTMP->ReadN(pRTMP, &type, 1) != 1)//读包头类型失败 	
		return -1; 	
   	
	headerType = (type & 0xc0) >> 6; 	
	nChannel = (type & 0x3f); 	
	nSize = packetSize[headerType]; 	
	
	p = &pRTMP->m_vecChannelsIn[nChannel]; 
	
	//if (nSize == RTMP_LARGE_HEADER_SIZE) {

	//}
	
	nSize--; 
	
	if (nSize > 0 && pRTMP->ReadN(pRTMP, header,nSize) != nSize)//有后继包头,但是读取失败 	
		return -2;
	
	if (nSize >= 3)//包头有时间戳 	
		nInfoField1 = ReadInt24(header);
	
	if (nSize >= 6){//包头有长度,说明是新包,释放原来该通道的包内容 	
		p->m_headerType = headerType;
		p->m_nChannel = nChannel;
		p->m_nBodySize = ReadInt24(header + 3); 	
		p->m_nBytesRead = 0; 	
		p->FreePacket(p); 	
	}
	
	if (nSize > 6) //包类型	
		packetType = header[6]; 
	
	if (nSize == 11)//流ID 	
		nInfoField2 = ReadInt32(header+7);
	
	//包分配失败
	if (p->m_nBodySize > 0 && p->m_body == NULL && (p->AllocPacket(p, p->m_nBodySize) < 0)) 	
		return -3;
	
	
	nToRead = p->m_nBodySize - p->m_nBytesRead; 	
	nChunk = pRTMP->m_chunkSize; 	
	if (nToRead < nChunk) 	
		nChunk = nToRead;
	if (pRTMP->ReadN(pRTMP, p->m_body + p->m_nBytesRead, nChunk) != nChunk){ 	
		p->FreePacket(p);  	
		return -4;  	
	} 	
	p->m_nBytesRead += nChunk; 	
	
	if (nInfoField1 != -1)
		p->m_nInfoField1 = nInfoField1;
	if (packetType != -1)
		p->m_packetType = packetType;
	if (nInfoField2 != -1)
		p->m_nInfoField2 = nInfoField2;
	
	if (p->IsCompleted(p)) {//一个包完整读完
		*packet = p;
		return p->m_nChannel;	
	} 	
	else 	
		return 0;
	
}
#else
static int my_ReadPacket(struct RTMP *pRTMP, CRTMPPacket *packet)
{ 	
	int nChunk;
	int nToRead;
	int nSize;
	char type; 	
	char header[RTMP_LARGE_HEADER_SIZE] = {0};
	if (pRTMP->ReadN(pRTMP, &type, 1) != 1) 	
		return -1; 	
   	
	packet->m_headerType = (type & 0xc0) >> 6; 	
	packet->m_nChannel = (type & 0x3f); 	
	nSize = packetSize[packet->m_headerType]; 	
	if (nSize < RTMP_LARGE_HEADER_SIZE) 	
		*packet = pRTMP->m_vecChannelsIn[packet->m_nChannel]; 	
  	
	nSize--; 	

	if (nSize > 0 && pRTMP->ReadN(pRTMP, header,nSize) != nSize) 	
		return -2; 	

	if (nSize >= 3) 	
		packet->m_nInfoField1 = ReadInt24(header); 	

	if (nSize >= 6){ 	
		packet->m_nBodySize = ReadInt24(header + 3); 	
		packet->m_nBytesRead = 0; 	
		packet->FreePacket(packet); 	
	} 	
  	
	if (nSize > 6) 	
		packet->m_packetType = header[6]; 	
	if (nSize == 11) 	
		packet->m_nInfoField2 = ReadInt32(header+7); 	
	if (packet->m_nBodySize > 0 && packet->m_body == NULL && (packet->AllocPacket(packet, packet->m_nBodySize) < 0)) 	
		return -3; 	
	
	nToRead = packet->m_nBodySize - packet->m_nBytesRead; 	
	nChunk = pRTMP->m_chunkSize; 	
	if (nToRead < nChunk) 	
		nChunk = nToRead; 	
	if (pRTMP->ReadN(pRTMP, packet->m_body + packet->m_nBytesRead, nChunk) != nChunk){ 	
		packet->m_body = NULL;  	
		return -4;  	
	} 	
	packet->m_nBytesRead += nChunk; 	

	pRTMP->m_vecChannelsIn[packet->m_nChannel] = *packet; 	
	if (packet->IsCompleted(packet)) { 	
		pRTMP->m_vecChannelsIn[packet->m_nChannel].m_body = NULL; 	
		pRTMP->m_vecChannelsIn[packet->m_nChannel].m_nBytesRead = 0; 	
	} 	
	else 	
		packet->m_body = NULL; 	
	return 1; 	
} 	
#endif

static int my_HandShake(struct RTMP *pRTMP)
{ 	
	int i;
	char dummy; 
	int bMatch;
	char resp[RTMP_SIG_SIZE];
	char clientsig[RTMP_SIG_SIZE+1]; 	
	char serversig[RTMP_SIG_SIZE]; 	
	clientsig[0] = 0x3; 	
	unsigned long uptime = htonl(timeGetTime()); 	
	memcpy(clientsig + 1, &uptime, sizeof(unsigned long)); 	
	memset(clientsig + 5, 0, 4); 	
	for (i=9; i<=RTMP_SIG_SIZE; i++) 	
		clientsig[i] = 0x00;//(char)(rand() % 256); 	
	if (pRTMP->WriteN(pRTMP, clientsig, RTMP_SIG_SIZE + 1) < 0) 	
		return -1; 	
  	
	if (pRTMP->ReadN(pRTMP, &dummy, 1) != 1) // 0x03 	
		return -2; 	
  	
	if (pRTMP->ReadN(pRTMP, serversig, RTMP_SIG_SIZE) != RTMP_SIG_SIZE) 	
		return -3; 	
   	
	if (pRTMP->ReadN(pRTMP, resp, RTMP_SIG_SIZE) != RTMP_SIG_SIZE) 	
		return -4; 	
    
	//bMatch = (memcmp(resp, clientsig + 1, RTMP_SIG_SIZE) == 0); 	
 	serversig[4] = 0x00;
	serversig[5] = 0x00;
	serversig[6] = 0x00;
	serversig[7] = 0x00;


	if (pRTMP->WriteN(pRTMP, serversig, RTMP_SIG_SIZE) < 0) 	
		return -5; 	
    
	return 1; 	
} 	

static int my_SendRTMP(struct RTMP *pRTMP, CRTMPPacket *packet)
{ 
	char sep;
	int nChunkSize;
	char *buffer = NULL;
	int nSize;
	char header[RTMP_LARGE_HEADER_SIZE] = { 0 };
	CRTMPPacket prevPacket = pRTMP->m_vecChannelsOut[packet->m_nChannel]; 	
	if (packet->m_headerType != RTMP_PACKET_SIZE_LARGE) 	{ 	
	if (prevPacket.m_nBodySize == packet->m_nBodySize && packet->m_headerType == RTMP_PACKET_SIZE_MEDIUM) 	
		packet->m_headerType = RTMP_PACKET_SIZE_SMALL; 	
		if (prevPacket.m_nInfoField2 == packet->m_nInfoField2 && packet->m_headerType == RTMP_PACKET_SIZE_SMALL) 	
			packet->m_headerType = RTMP_PACKET_SIZE_MINIMUM; 
    
	} 	
  
	if (packet->m_headerType > 3) 	
 		return -1; 	
   	
	nSize = packetSize[packet->m_headerType]; 	
   	
	header[0] = (char)((packet->m_headerType << 6) | packet->m_nChannel); 	
	if (nSize > 1) 	
		EncodeInt24(header+1, packet->m_nInfoField1); 	

	if (nSize > 4) 	{ 	
		EncodeInt24(header+4, packet->m_nBodySize); 	
		header[7] = packet->m_packetType; 	
	} 	
	if (nSize > 8) 	
		EncodeInt32(header+8, packet->m_nInfoField2); 	
	if (pRTMP->WriteN(pRTMP, header, nSize) < 0) 	
		return -2; 	

	nSize = packet->m_nBodySize; 	
	buffer = packet->m_body; 	
	while (nSize){ 	
		nChunkSize = pRTMP->m_chunkSize;
		if (nSize < pRTMP->m_chunkSize) 	
			nChunkSize = nSize; 	
		if (pRTMP->WriteN(pRTMP, buffer, nChunkSize) < 0) 	
			return -3; 	
		nSize -= nChunkSize; 	
		buffer += nChunkSize; 	
		if (nSize > 0) { 	
			sep = (0xc0 | packet->m_nChannel); 	
			if (pRTMP->WriteN(pRTMP, &sep, 1) < 0) 	
				return -4;  	
		} 	
	} 	
	if (packet->m_packetType == 0x14){ 	
		ReadString(packet->m_body + 1, pRTMP->m_methodCalls[pRTMP->m_nmethodCalls % MAX_INVOKE]);		 
		pRTMP->m_nmethodCalls++;
	}
    	
	pRTMP->m_vecChannelsOut[packet->m_nChannel] = *packet; 
	pRTMP->m_vecChannelsOut[packet->m_nChannel].m_body = NULL;
  return 1; 	
}
					
static int my_SendReleaseStream(struct RTMP *pRTMP) 	
{ 
	int bRtn;	
	char *enc = NULL;
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);
	packet.m_nChannel = 0x03;  
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 	
	packet.m_packetType = 0x14; 	
	packet.m_nInfoField1 = 0x00; 	
	packet.AllocPacket(&packet, 256); 	
	enc = packet.m_body; 	
	enc += EncodeString_short(enc, "releaseStream"); 	
	enc += EncodeNumber_short(enc, 2); 	
	*enc = 0x05;  	
	enc++; 

	enc += EncodeString_short(enc, pRTMP->m_sLive);
	
	packet.m_nBodySize = enc - packet.m_body; 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn; 	
}

static int my_SendFCPublish(struct RTMP *pRTMP) 	
{ 	
	int bRtn;
	char *enc = NULL;
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);
	packet.m_nChannel = 0x03; 
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 	
	packet.m_packetType = 0x14; 
	packet.m_nInfoField1 = 0x00; 	
	packet.AllocPacket(&packet, 256); 
	enc = packet.m_body; 	
	enc += EncodeString_short(enc, "FCPublish"); 	
	enc += EncodeNumber_short(enc, 3); 	
	*enc = 0x05;  
	enc++; 	

	enc += EncodeString_short(enc, pRTMP->m_sLive);

	packet.m_nBodySize = enc - packet.m_body; 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn;  	
}

static int my_SendCreateStream(struct RTMP *pRTMP)
{ 	
	int bRtn;
	char *enc = NULL;
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);
	packet.m_nChannel = 0x03;    	
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 	
	packet.m_packetType = 0x14;  	
	packet.m_nInfoField1 = 0x00; 	
	packet.AllocPacket(&packet, 256);  	
	enc = packet.m_body; 	
	enc += EncodeString_short(enc, "createStream"); 	
	enc += EncodeNumber_short(enc, 1);
	*enc = 0x05; 
	enc++; 	
	packet.m_nBodySize = enc - packet.m_body; 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn;  	
}

static int my_SendPublishStream(struct RTMP *pRTMP) 	
{ 	
	char s[128]={0};
	int bRtn;
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);
	packet.m_nChannel = 0x04;  	
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 	
	packet.m_packetType = 0x14; 	
	packet.m_nInfoField1 = 0x00; 
	packet.m_nInfoField2 = 0x01000000/*NTOHL(pRTMP->m_stream_id)*/;
	packet.AllocPacket(&packet, 256); 	
	char *enc = packet.m_body; 	
	enc += EncodeString_short(enc, "publish"); 	
	enc += EncodeNumber_short(enc, 2);
	*enc = 0x05; 
	enc++; 	
	//sprintf(s, "%s?videoKeyframeFrequency=%d&totalDatarate=%d", pRTMP->m_sLive, 
	//	pRTMP->m_MetaData.videokeyframe_frequency,pRTMP->m_MetaData.videodatarate + pRTMP->m_MetaData.audiodatarate);

	sprintf(s, "%s", pRTMP->m_sLive);

	enc += EncodeString_short(enc, s);
	enc += EncodeString_short(enc, "live");

	packet.m_nBodySize = enc - packet.m_body; 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn; 	
}

static int my_SendMetaData(struct RTMP *pRTMP, tagMetaData tMetaData)	
{ 	
	int bRtn;
	char timebuf[26];
	time_t ltime;
	struct tm gmt;
	char *enc = NULL;	
	CRTMPPacket packet = g_RTMPPacket; 
	packet.Init(&packet);
	packet.m_nChannel = 0x04;  	
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 	
	packet.m_packetType = 0x12;   	
	packet.m_nInfoField1 = 0x00; 
	packet.m_nInfoField2 = 0x01000000/*NTOHL(pRTMP->m_stream_id)*/;
	packet.AllocPacket(&packet, 1024);  	
	enc = packet.m_body; 	
	enc += EncodeString_short(enc, "@setDataFrame");
	enc += EncodeString_short(enc, "onMetaData");

	*enc = 0x03;  	
	enc++; 	
	enc += EncodeString(enc, "author", tMetaData.author); 	
	enc += EncodeString(enc, "copyright", tMetaData.copyright);
	enc += EncodeString(enc, "description", tMetaData.description); 
	enc += EncodeString(enc, "keywords", tMetaData.keywords);
	enc += EncodeString(enc, "rating", tMetaData.rating);
	enc += EncodeString(enc, "title", tMetaData.title);
	enc += EncodeString(enc, "presetname", tMetaData.presetname);
	time( &ltime );
// 	_gmtime64_s( &gmt, &ltime);
// 	asctime_s(timebuf, 26, &gmt);
	enc += EncodeString(enc, "creationdate", /*timebuf*/"Tue Feb 24 17:41:07 2009");

if(tMetaData.bVideo == true){
	enc += EncodeString(enc, "videodevice", tMetaData.videodevice);
	enc += EncodeNumber(enc, "framerate", tMetaData.framerate);
	enc += EncodeNumber(enc, "width", tMetaData.width);
	enc += EncodeNumber(enc, "height", tMetaData.height); 
	enc += EncodeString(enc, "videocodecid", "avc1");
	enc += EncodeNumber(enc, "videodatarate", tMetaData.videodatarate);//码流
	enc += EncodeNumber(enc, "avclevel", tMetaData.avclevel);
	enc += EncodeNumber(enc, "avcprofile", tMetaData.avcprofile);
	enc += EncodeNumber(enc, "videokeyframe_frequency", tMetaData.videokeyframe_frequency);
}

if(tMetaData.bAudio == true){
	enc += EncodeString(enc, "audiodevice", tMetaData.audiodevice);
	enc += EncodeNumber(enc, "audiosamplerate", tMetaData.audiosamplerate);
	enc += EncodeNumber(enc, "audiochannels", tMetaData.audiochannels);
	enc += EncodeNumber(enc, "audioinputvolume", 80);
	enc += EncodeNumber(enc, "audiocodecid", 1);
	enc += EncodeNumber(enc, "audiodatarate", tMetaData.audiodatarate);
}
	enc += 2; 	
	*enc = 0x09; 	
	enc++; 

	packet.m_nBodySize = enc - packet.m_body; 	
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn; 	
}

static char *FindStr(const char *pData, const int nLen)
{
	char pFind[]={0x00,0x00,0x00,0x01};
	int nFind = KMP((char *)pData, nLen, &pFind[0], sizeof(pFind));
	if (nFind == -1)
		return NULL;
	else 
		return (char *)(pData + nFind);
}
/*
 my_SendVideo返回值说明：
	小于-1说明是发送失败，连接已经断开。
	-1说明没有解析到正确的数据，或者是数据无效
	其它数值说明正确

*/
static int my_SendVideo(struct RTMP *pRTMP, const char *pData, unsigned int nLen, unsigned long dwTime)
{

	int bRtn;
	char *enc = NULL;
	CRTMPPacket packet = g_RTMPPacket;
	if (pRTMP->m_bSendPars == 0){
		enc = pRTMP->SendParsPacket(pRTMP, pData, nLen);
		if (enc == NULL)
			return -1;
		pRTMP->m_bSendPars = 1;
		packet.Init(&packet);
		packet.m_nChannel = 0x05;  	
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;// RTMP_PACKET_SIZE_MEDIUM;//	
		packet.m_packetType = 0x09;  	
		packet.m_nInfoField1 = dwTime; 
		packet.m_nInfoField2 = 0x01000000/*NTOHL(pRTMP->m_stream_id)*/;	
		
#if 0
		bRtn = enc - pData+9;//去掉 00 00 00 01 06信息
#else
		while (1){			
			enc = FindStr(enc, nLen - (enc - pData));
			if (enc == NULL)
				return -1;
			enc += 4;

			if ((*enc & 0x06) == 0x06)	
				continue;
			break;
		}
		bRtn = enc - pData;
#endif

		packet.AllocPacket(&packet, nLen - bRtn + 10);  
		enc = packet.m_body; 
		
		if ((*(pData + bRtn) & 0x1f) == 5){
			*enc = 0x17; 	
			enc++; 	
		}
		else{
			*enc = 0x27; 	
			enc++; 
		}
		
		*enc = 0x01; 	
		enc++; 
		*enc = 0x00; 	
		enc++; 
		*enc = 0x00; 	
		enc++; 
		*enc = 0x00; 	
		enc++; 
	
		enc += EncodeInt32(enc, nLen - bRtn);	
		
		memcpy(enc, pData + bRtn, nLen - bRtn);
		
		packet.m_nBodySize = nLen - bRtn + 9;
		bRtn = pRTMP->SendRTMP(pRTMP, &packet);
		packet.FreePacket(&packet);
	}
	else {
		enc = (char *)pData;
		while (1){			
			enc = FindStr(enc, nLen - (enc - pData));
			if (enc == NULL)
				return -1;
			enc += 4;
#if 0
			if (((*enc & 0x1f) == 7) ||((*enc & 0x1f) == 8))	
				continue;
//#else
			if ((*enc & 0x1f) == 7) //(||((*enc & 0x1f) == 8))//赵号修改，
			{
				enc = pRTMP->SendParsPacket(pRTMP, enc-4, nLen-(enc-pData));//重新在握手过程中发送、关键帧。
				continue;
			}
#endif
			{
				if(g_csType == DEF_MICROVIDEO_DEV)
				{
					if (((*enc & 0x1f) == 7) ||((*enc & 0x1f) == 8))	
						continue;
				}else
				{

					if ((*enc & 0x1f) == 7) //(||((*enc & 0x1f) == 8))//赵号修改，
					{
						enc = pRTMP->SendParsPacket(pRTMP, enc-4, nLen-(enc-pData));//重新在握手过程中发送、关键帧。
						continue;
					}
				}
			}
			if ((*enc & 0x06) == 0x06)	
				continue;
			break;
		}

		packet.Init(&packet);
		packet.m_nChannel = 0x05;  	
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;//RTMP_PACKET_SIZE_LARGE; 	//
		packet.m_packetType = 0x09;  	
		packet.m_nInfoField1 = dwTime; 
		packet.m_nInfoField2 = 0x01000000/*NTOHL(pRTMP->m_stream_id)*/;	
		
		bRtn = enc - pData;
		packet.AllocPacket(&packet, nLen - bRtn + 10);  
		enc = packet.m_body; 
		
		if ((*(pData + bRtn) & 0x1f) == 5){
			*enc = 0x17; 	
			enc++; 
		}
		else{
			*enc = 0x27; 	
			enc++; 
		}

		*enc = 0x01; 	
		enc++; 
		*enc = 0x00; 	
		enc++; 
		*enc = 0x00; 	
		enc++; 
		*enc = 0x00; 	
		enc++; 
		
		enc += EncodeInt32(enc, nLen - bRtn);
		
		memcpy(enc, pData + bRtn, nLen - bRtn);
		
		packet.m_nBodySize = nLen - bRtn + 9;
		bRtn = pRTMP->SendRTMP(pRTMP, &packet);
		packet.FreePacket(&packet);
	}

	return bRtn;
}

static int my_SendAudio(struct RTMP *pRTMP, const char *pData, unsigned int nLen, unsigned long dwTime)
{
	int bRtn;
	char *enc = NULL;
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);
	packet.m_nChannel = 0x05;  	
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 	
	packet.m_packetType = 0x08;  	
	packet.m_nInfoField1 = dwTime; 
	packet.m_nInfoField2 = 0x01000000/*NTOHL(pRTMP->m_stream_id)*/;	

	packet.AllocPacket(&packet, nLen + 2);  
	enc = packet.m_body; 
	switch((int)(pRTMP->m_MetaData.audiosamplerate)){
		case    44100:
			bRtn = 0x10 + 0x0e;
		break;
		case    22050:
			bRtn = 0x10 + 0x0a;
		break;
		case    11025:
			bRtn = 0x10 + 0x06;
		break;
		case     8000: 
		case     5512: 
			bRtn = 0x10+0x02;
		break;
	}
	if (pRTMP->m_MetaData.audiochannels == 2)
		bRtn += 1;
	*enc = bRtn;
	enc++;
	memcpy(enc, pData, nLen);

	packet.m_nBodySize = nLen+1;
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn;
}

static char *my_SendParsPacket(struct RTMP *pRTMP, const char *pData, const int nLen)
{
	int bRtn;
	char *enc = NULL;
	CRTMPPacket packet = g_RTMPPacket;

	char *pb = (char *)pData;		
	char *pFind1 = NULL;
	char *pFind2 = NULL;
	char *pFind3 = NULL;	
	pFind1 = FindStr(pb, nLen);
	if (pFind1 == NULL)
		return NULL;
	pFind1 += 4;
	if ((*pFind1 & 0x1f) != 7)
		return NULL;
	pb = pFind1;

	pFind2 = FindStr(pb, nLen - (pData - pb));
	if (pFind2 == NULL)
		return NULL;
	pFind2 += 4;
	if ((*pFind2 & 0x1f) != 8)
		return NULL;
	pb = pFind2;

	pFind3 = FindStr(pb, nLen - (pData - pb));
	if (pFind3 == NULL)
		return NULL;
	bRtn = (pFind2 - pFind1) - 4;	
	bRtn += pFind3 - pFind2;
	
	packet.Init(&packet);
	packet.m_nChannel = 0x05;  	
		if(pRTMP->m_bSendPars == 0)
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;//绝对时间戳 	
	else
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;//RTMP_PACKET_SIZE_LARGE;//绝对时间戳 
	packet.m_packetType = 0x09;  	
	packet.m_nInfoField1 = 0x00000000; 
	packet.m_nInfoField2 = 0x01000000/*NTOHL(pRTMP->m_stream_id)*/;	
	
	packet.AllocPacket(&packet, bRtn + 17);  
	enc = packet.m_body;
	put_byte(&enc, 0x17); 
	put_byte(&enc, 0x00); 
	put_byte(&enc, 0x00); 
	put_byte(&enc, 0x00); 
	put_byte(&enc, 0x00); 

	put_byte(&enc, 1); 
	put_byte(&enc, pFind1[1]); 
	put_byte(&enc, pFind1[2]); 
	put_byte(&enc, pFind1[3]); 
	put_byte(&enc, 0x03); 
	put_byte(&enc, 0x01); 
	
	put_be16(&enc, (pFind2 - pFind1) - 4);
	memcpy(enc, pFind1, (pFind2 - pFind1) - 4);
	enc += (pFind2 - pFind1) - 4;
	put_byte(&enc, 1); 
	put_be16(&enc, pFind3 - pFind2);
    memcpy(enc, pFind2, pFind3 - pFind2);
	enc += pFind3 - pFind2;
	
	packet.m_nBodySize = bRtn + 16;
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);


	//pFind3 += 4;
	return pFind3;

}

static int my_SendBytesReceived(struct RTMP *pRTMP)
{
	int bRtn;
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);


	packet.m_nChannel = 0x02;   
	packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
	packet.m_packetType = 0x03; 

	packet.AllocPacket(&packet, 4);
	packet.m_nBodySize = 4;

	EncodeInt32(packet.m_body, pRTMP->m_nBytesIn);
	pRTMP->m_nBytesInSent = pRTMP->m_nBytesIn;

	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn; 
}

static int my_SendServerBW(struct RTMP *pRTMP)
{
	int bRtn;
	CRTMPPacket packet = g_RTMPPacket; 	
	packet.Init(&packet);

	packet.m_nChannel = 0x02;
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet.m_packetType = 0x05;

	packet.AllocPacket(&packet, 4);
	packet.m_nBodySize = 4;

	EncodeInt32(packet.m_body, 2500000);
	bRtn = pRTMP->SendRTMP(pRTMP, &packet);
	packet.FreePacket(&packet);
	return bRtn; 
}


char *FindInvokeStr(const char *pData, const char *pSearch, int nLen)
{
	char *p = NULL;
	int nPos = 0;
	int nStrLen;
	while (1){
		p = (char *)strstr(pData + nPos, pSearch);
		if (p != NULL)
			return p;
		nStrLen = strlen(pData + nPos);
		if ((nPos + nStrLen + 1) >= nLen)
			return NULL;
		nPos += (nStrLen+1);
	}
	return NULL;
}

static int my_HandleInvoke(struct RTMP *pRTMP, CRTMPPacket *packet)
{ 	
	int n;
	char *pInfoStr = NULL;
	char *pData = NULL;
	char *method = NULL;
	//char *methodInvoked = NULL;

	if (packet->m_body[0] != 0x02) 	
		return -1; 	

	pData = packet->m_body;
	method = FindInvokeStr(pData, "_result", packet->m_nBodySize);
	if (method != NULL){ 
		//methodInvoked = pRTMP->m_methodCalls[pRTMP->m_nmethodCallsRead % MAX_INVOKE];
		//pRTMP->m_nmethodCallsRead++;

		n = (pData - method);
		//if (strcmp(methodInvoked, "connect") == 0){ 		
		if (pRTMP->QueryAMFMethod(pRTMP, "connect") == 0){
			pInfoStr = FindInvokeStr(pData + n, "NetConnection.Connect.Success", packet->m_nBodySize - n);
			if (pInfoStr == NULL)	
				return -2;
			else{	
				pRTMP->SendReleaseStream(pRTMP);
				pRTMP->SendFCPublish(pRTMP);
				pRTMP->SendCreateStream(pRTMP);
				//pRTMP->SendPublishStream(pRTMP);
	
			}
		}
		else if (pRTMP->QueryAMFMethod(pRTMP, "releaseStream") == 0){

		}
		else if (pRTMP->QueryAMFMethod(pRTMP, "createStream") == 0){
      	
		}
		return 1;
	}
	method = FindInvokeStr(pData, "onStatus", packet->m_nBodySize);
	if (method != NULL){ 
		//methodInvoked = pRTMP->m_methodCalls[pRTMP->m_nmethodCallsRead % MAX_INVOKE];
		//pRTMP->m_nmethodCallsRead++;

		n = (pData - method);
		if (pRTMP->QueryAMFMethod(pRTMP, "publish") == 0){
			pInfoStr = FindInvokeStr(pData + n, "NetStream.Publish.Start", packet->m_nBodySize - n);
			if (pInfoStr == NULL)
				return -3;
			else{
				pRTMP->m_bHandshake = 1;
			}
		}
		return 1;

	}

	method = FindInvokeStr(pData, "onFCPublish", packet->m_nBodySize);
	if (method != NULL){
		if (FindInvokeStr(pData, "NetStream.Publish.Start", packet->m_nBodySize) == NULL)
			return -4;
		else
			pRTMP->SendPublishStream(pRTMP);		
		return 1;
	}

	method = FindInvokeStr(pData, "_onbwcheck", packet->m_nBodySize);
	if (method != NULL){ 
		pRTMP->SendCheckBWResult(pRTMP);
		return 1;
	}
	method = FindInvokeStr(pData, "_error", packet->m_nBodySize);
	if (method != NULL)
		return -5;

	method = FindInvokeStr(pData, "close", packet->m_nBodySize);
	if (method != NULL)
		return -6;

	return 1;
} 

static int my_QueryAMFMethod(struct RTMP *pRTMP, const char *pMethod)
{
	int n;
	for(n = 0; n < MAX_INVOKE; n++){
		if (strcmp(pRTMP->m_methodCalls[n], pMethod) == 0){
			memset(pRTMP->m_methodCalls[n], 0, sizeof(pRTMP->m_methodCalls[n]));
			return 0;
		}
	}
	return -1;

}

CRTMP g_RTMP = {
	my_Init,
	my_Close,
	my_Connect,
	my_IsConnected,
	my_SendVideo,
	my_SendAudio,
	my_SendReleaseStream,
	my_SendFCPublish,
	my_SendCreateStream,
	my_SendPublishStream,
	my_SendMetaData,
	my_ConnectIsOK,
	my_PublishIsOK,
	my_FCPublishIsOK,
	my_HandShake,
	my_SendConnectPacket,
	my_SendCheckBWResult,
	my_SendPing,
	my_HandleInvoke,
	my_HandleChangeChunkSize,
	my_HandlePing,
	my_SendRTMP,
	my_ReadPacket,
	my_ReadN,
	my_WriteN,
	my_SendParsPacket,	
	my_SendBytesReceived,
	my_SendServerBW,
	my_QueryAMFMethod,
	
}; 
