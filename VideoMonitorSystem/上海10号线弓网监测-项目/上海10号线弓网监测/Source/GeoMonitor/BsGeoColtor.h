#pragma once
#include "bsabstractanalyse.h"
class CGeoColtor :
	public CAbstractAnalyse
{
public:
	CGeoColtor(void);
	virtual ~CGeoColtor(void);
public:
	virtual int AnalyzeFrameTemp(NSP_STP_CM::CMySqlDB* p_CMySqlDB, CFrameInf* p_frameCBInf){return 0;}
	virtual void UpdataFramePath(std::string frame_path){return;}
	virtual void SetCameraOpt();
};

