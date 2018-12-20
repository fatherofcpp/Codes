#pragma once
#include "Thread/CmThreadPool.h"

#include <string>
extern std::string gVersion;

typedef enum tag_e_camera
{
	E_CAMERA_UP = 0,
	E_CAMERA_LEFT,
	E_CAMERA_RIGHT,
	E_CAMERA_COUNT,
}e_camera;

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
};

