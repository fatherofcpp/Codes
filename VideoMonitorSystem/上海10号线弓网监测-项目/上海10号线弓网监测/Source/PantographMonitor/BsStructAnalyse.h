#pragma once
#include "BsAbstractAnalyse.h"

class CStructAnalyse :
	public CAbstractAnalyse
{
public:
	CStructAnalyse(void);
	virtual ~CStructAnalyse(void);
public:
	virtual int AnalyzeFrameTemp(NSP_STP_CM::CMySqlDB* p_CMySqlDB, CFrameInf* p_frameCBInf){return 0;}
	virtual void UpdataFramePath(std::string frame_path);
	virtual void SetCameraOpt();
public:
	std::string GetFramePath(){return m_framePath;};
private:
	std::string m_framePath;
};

