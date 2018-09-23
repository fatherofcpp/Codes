#pragma once

namespace MVB
{
	class CServerTrans;
}

typedef MVB::CServerTrans* (*CREATESERVICE_MVB)(int TransWay);
typedef int (*RELEASESERVICE_MVB)(MVB::CServerTrans** pService, int TransWay);

class CMvbDll
{
public:
	CMvbDll(void);
	~CMvbDll(void);
public:
	int LoadDll();
public:
	CREATESERVICE_MVB createService;
	RELEASESERVICE_MVB releaseService;

	MVB::CServerTrans* m_pCServerTrans;//服务接口
};
extern CMvbDll* g_pCMvbDll;
