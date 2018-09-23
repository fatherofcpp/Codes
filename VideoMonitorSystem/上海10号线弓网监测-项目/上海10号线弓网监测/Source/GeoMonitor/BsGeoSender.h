#pragma once
#include "Thread/CmThreadPool.h"

class CGeoColtor;
class CFrameInf;
class CGeoSender: public NSP_STP_CM::CThreadPool
{
public:
	CGeoSender(void);
	virtual ~CGeoSender(void);

public:
	//≥ı ºªØ
	virtual int TaskStart();

public:
	void SetGeoCamera(CGeoColtor* geo_up, CGeoColtor* geo_down_left, CGeoColtor* geo_down_right);
	void SetGeoPort(int port){m_udpGeoPort = port;}
	void SetGeoPortDown(int port){m_udpGeoPortDown = port;}

public:
	static unsigned int WINAPI thread_send_geo(void* pParam);

private:
	CGeoColtor* m_pCGeoColtorUp;
	CGeoColtor* m_pCGeoColtorDownLeft;
	CGeoColtor* m_pCGeoColtorDownRight;

	CFrameInf* m_pFrameUp;
	CFrameInf* m_pFrameDownLeft;
	CFrameInf* m_pFrameDownRight;

	int m_udpGeoPort;
	int m_udpGeoPortDown;
};

