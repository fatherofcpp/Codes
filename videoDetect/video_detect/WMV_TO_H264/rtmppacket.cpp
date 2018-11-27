#include "StdAfx.h"
#include "rtmppacket.h" 	

static void my_Init(struct RTMPPacket *pRTMPPacket)
{
	pRTMPPacket->Reset(pRTMPPacket);
}

static void my_Close(struct RTMPPacket *pRTMPPacket)
{
	pRTMPPacket->FreePacket(pRTMPPacket);
	pRTMPPacket->Reset(pRTMPPacket);
}

static void my_Reset(struct RTMPPacket *pRTMPPacket)
{
	pRTMPPacket->m_headerType = 0; 	
	pRTMPPacket->m_packetType = 0; 	
	pRTMPPacket->m_nChannel = 0; 	
	pRTMPPacket->m_nInfoField1 = 0; 	
	pRTMPPacket->m_nInfoField2 = 0; 	
	pRTMPPacket->m_nBodySize = 0; 	
	pRTMPPacket->m_nBytesRead = 0; 	
	pRTMPPacket->m_nInternalTimestamp = 0; 	
	pRTMPPacket->m_body = NULL; 
}


static int my_AllocPacket(struct RTMPPacket *pRTMPPacket, int nSize)
{
	pRTMPPacket->m_body = (char *)malloc(nSize); 	
	if (!pRTMPPacket->m_body) 	
		return -1; 	
	memset(pRTMPPacket->m_body,0,nSize); 	
	pRTMPPacket->m_nBytesRead = 0; 	
  return 1;
}

static void my_FreePacket(struct RTMPPacket *pRTMPPacket)
{
	if (pRTMPPacket->m_body) 	
		free(pRTMPPacket->m_body); 	
	pRTMPPacket->m_body = NULL;
}

static int my_IsCompleted(struct RTMPPacket *pRTMPPacket)
{
	return (pRTMPPacket->m_nBytesRead == pRTMPPacket->m_nBodySize);
}
	

CRTMPPacket g_RTMPPacket = 
{
	my_Init, 
	my_Close, 
	my_Reset, 
	my_AllocPacket, 
	my_FreePacket, 
	my_IsCompleted,
	};
											
