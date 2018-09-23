#pragma once

namespace VideoNSP
{
	class CVideoObjectService;
}

typedef VideoNSP::CVideoObjectService* (*CREATESERVICE_CAMVIDEO)();
typedef void (*RELEASEALLSERVICE_CAMVIDEO)();
typedef void (*RELEASESERVICE_CAMVIDEO)(VideoNSP::CVideoObjectService** pp_CVideoObjectService);

class CCamVideoDll
{
public:
	CCamVideoDll(void);
	~CCamVideoDll(void);
public:
	int LoadDll();
public:
	CREATESERVICE_CAMVIDEO createService;
	RELEASEALLSERVICE_CAMVIDEO releaseAllService;
	RELEASESERVICE_CAMVIDEO releaseService;

	VideoNSP::CVideoObjectService* m_pCVideoObjectService;
};
extern CCamVideoDll* g_pCCamVideoDll;

