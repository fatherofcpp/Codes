#ifndef __RTMP_H__  	 	 	 	 	 	 	 	
#define __RTMP_H__ 	
 	
#include "OBJ.h"
#include "rtmppacket.h" 	

static const int packetSize[] = { 12, 8, 4, 1 }; 	

#define RTMP_PACKET_SIZE_LARGE    0 	
#define RTMP_PACKET_SIZE_MEDIUM   1 	
#define RTMP_PACKET_SIZE_SMALL    2 	
#define RTMP_PACKET_SIZE_MINIMUM  3 

#define MAX_INVOKE				20

typedef struct  
{
	char			author[256];
	char 			copyright[256];
	char 			description[256];
	char			keywords[256];
	char			rating[256];
	char			title[256];
	char			presetname[256];
	char			videodevice[256];
	double			framerate;
	double			width;
	double			height;
	double			videodatarate;
	double			avclevel;
	double			avcprofile;
	double			videokeyframe_frequency;
	char			audiodevice[256];
	double			audiosamplerate;
	double			audiochannels;
	double			audiodatarate;
	int				bVideo;
	int				bAudio;
}tagMetaData;

typedef struct RTMP 	
{
	void (*Init)(struct RTMP *pRTMP);
	void (*Close)(struct RTMP *pRTMP);	 	
 	
	int (*Connect)(struct RTMP *pRTMP, const char *pIP, const char *pPath, const char *pLive); 	
	int	(*IsConnected)(const struct RTMP *pRTMP); 	
	
	int (*SendVideo)(struct RTMP *pRTMP, const char *pData, unsigned int nLen, unsigned long dwTime); 
	int (*SendAudio)(struct RTMP *pRTMP, const char *pData, unsigned int nLen, unsigned long dwTime);

	int (*SendReleaseStream)(struct RTMP *pRTMP);	
	int (*SendFCPublish)(struct RTMP *pRTMP);
	int (*SendCreateStream)(struct RTMP *pRTMP);
	int (*SendPublishStream)(struct RTMP *pRTMP);
	int (*SendMetaData)(struct RTMP *pRTMP,tagMetaData tMetaData);

	int (*ConnectIsOK)(const struct RTMP *pRTMP, const COBJ obj);
	int (*PublishIsOK)(const struct RTMP *pRTMP, const COBJ obj);
	int (*FCPublishIsOK)(const struct RTMP *pRTMP, const COBJ obj);
 	
	int (*HandShake)(struct RTMP *pRTMP);	
	int (*SendConnectPacket)(struct RTMP *pRTMP); 	
	int (*SendCheckBWResult)(struct RTMP *pRTMP); 	
	int (*SendPing)(struct RTMP *pRTMP, short nType, unsigned int nObject, unsigned int nTime); 	
 	
	int (*HandleInvoke)(struct RTMP *pRTMP, CRTMPPacket *packet); 	
	void (*HandleChangeChunkSize)(struct RTMP *pRTMP, CRTMPPacket *packet); 	
	void (*HandlePing)(struct RTMP *pRTMP, CRTMPPacket *packet); 	
  
	int (*SendRTMP)(struct RTMP *pRTMP, CRTMPPacket *packet); 	
	int (*ReadPacket)(struct RTMP *pRTMP, CRTMPPacket *packet); 	
	int (*ReadN)(struct RTMP *pRTMP, char *buffer, int n); 	
	int (*WriteN)(struct RTMP *pRTMP, const char *buffer, int n); 
	char *(*SendParsPacket)(struct RTMP *pRTMP, const char *pData, const int nLen);
	int (*SendBytesReceived)(struct RTMP *pRTMP);
	int (*SendServerBW)(struct RTMP *pRTMP);
	int (*QueryAMFMethod)(struct RTMP *pRTMP, const char *pMethod);
	
	int  m_socket; 	
	int  m_chunkSize; 	
	int  m_nBWCheckCounter; 	
	char m_sIP[16];
	char m_sPath[256];
	char m_sLive[256];

	int m_nmethodCalls;	
	int m_nmethodCallsRead;	
	char m_methodCalls[MAX_INVOKE][64]; 	
	tagMetaData	 m_MetaData;
	int			m_stream_id;
	int			m_nBytesIn;
	int			m_nBytesInSent;

	CRTMPPacket m_vecChannelsIn[64]; 	
	CRTMPPacket m_vecChannelsOut[64]; 
	int m_bStart;
	int m_bHandshake;
	int m_bSendPars;
}CRTMP; 	
 
extern CRTMP g_RTMP; 	
#endif 	
										
