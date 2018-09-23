#pragma once

namespace Cam
{
	class CCamObjectService;
}

typedef int (*CREATESERVICE_CAMPICTURE)(Cam::CCamObjectService** pService);
typedef void (*RELEASEALLSERVICE_CAMPICTURE)();

class CCamPictureDll
{
public:
	CCamPictureDll(void);
	~CCamPictureDll(void);
public:
	int LoadDll();
public:
	CREATESERVICE_CAMPICTURE createService;
	RELEASEALLSERVICE_CAMPICTURE releaseAllServices;

	Cam::CCamObjectService* m_pCCamObjectService;
};
extern CCamPictureDll* g_pCCamPictureDll;

