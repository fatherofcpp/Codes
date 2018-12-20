#pragma once
#include "Thread/CmThreadPool.h"

#include <string>
extern std::string gVersion;

#include "BsAnalyzeGeo.h"

class BsMainTask: public NSP_STP_CM::CThreadPool
{
public:
	BsMainTask(void);
	~BsMainTask(void);

public:
	int TaskStart();
	void TaskStop();

private:
	int LoadConfig();

private:
	static unsigned int WINAPI thread_recv_geo(void* pParam);
	static unsigned int WINAPI thread_recv_handle_geo(void* pParam);
	static unsigned int WINAPI thread_send_geo(void* pParam);

private:
	char m_handleBuf[256];

	int m_udpPortRecv;
	int m_udpPortSend;
	std::string m_udpIpSend;

	CAnalyzeGeo m_anaGeoArr[E_CAMERA_COUNT];
	tag_geo m_geoArr[E_CAMERA_COUNT];

	CCriticalSection m_geoContLock;
	std::deque<st_geo> m_geoCont;

public:
	static int m_s_areaWith;
	static int m_s_areaHeight;
	static int m_s_top;
	static int m_s_bottom;
	static int m_s_left;
	static int m_s_rigth;
	static int m_s_threPoint;
};

