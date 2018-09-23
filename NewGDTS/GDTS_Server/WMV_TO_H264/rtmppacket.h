#ifndef __RTMP_PACKET__  	 	 	 	 	 	
#define __RTMP_PACKET__ 	
 	
#include <stdio.h> 	
#include <string.h>
#include <stdlib.h>
	
typedef struct RTMPPacket
{ 	
	void (*Init)(struct RTMPPacket *pRTMPPacket);
	void (*Close)(struct RTMPPacket *pRTMPPacket);
	void (*Reset)(struct RTMPPacket *pRTMPPacket);
	int  (*AllocPacket)(struct RTMPPacket *pRTMPPacket, int nSize);
	void (*FreePacket)(struct RTMPPacket *pRTMPPacket);
	int  (*IsCompleted)(struct RTMPPacket *pRTMPPacket);

	unsigned char           m_headerType; 	
	unsigned char           m_packetType; 	
	unsigned char           m_nChannel; 	
	int            m_nInfoField1; 	
	int            m_nInfoField2;

	unsigned long  m_nBodySize; 	
	unsigned long  m_nBytesRead; 	
	unsigned long  m_nInternalTimestamp; 	
	char           *m_body; 
}CRTMPPacket; 
 	
extern CRTMPPacket g_RTMPPacket;

#endif 	
								
