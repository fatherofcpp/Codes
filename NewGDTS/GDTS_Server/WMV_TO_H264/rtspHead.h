
#ifndef _DEF_RTSP_H_
#define _DEF_RTSP_H_
//
#include "stdafx.h"

typedef struct tagDEV_RTSP{//RTSP使用参数结构
 
	char m_Url[128];
	char m_serverIp[32];
	int m_Port;
	char m_pubName[64];
	int m_streamNo;
	int b_decoder;
	
}DEV_RTSP_PARAMS;


#endif//_DEF_RTSP_H_

