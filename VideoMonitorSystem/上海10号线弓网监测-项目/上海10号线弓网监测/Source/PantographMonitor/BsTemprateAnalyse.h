#pragma once
#include "BsAbstractAnalyse.h"
class CStructAnalyse;

class CTemprateAnalyse :
	public CAbstractAnalyse
{
public:
	CTemprateAnalyse(void);
	virtual ~CTemprateAnalyse(void);
public:
	//设置相机参数
	virtual void SetCameraOpt();

	virtual int AnalyzeFrameTemp(NSP_STP_CM::CMySqlDB* p_CMySqlDB, CFrameInf* p_frameCBInf);
	virtual void UpdataFramePath(std::string frame_path){return;}

public:
	void SetTempThreshold(double temp_threshold_cls1, double temp_threshold_cls2, double temp_threshold_cls3);
	void SetStructAnalyse(CStructAnalyse* p_CStructAnalyse){m_pCStructAnalyse = p_CStructAnalyse;}
	CStructAnalyse* GetStructAnalyse(){return m_pCStructAnalyse;}
	int GetTemp(CFrameInf* p_frameCBInf);
private:
	double m_tempThresholdCls1;
	double m_tempThresholdCls2;
	double m_tempThresholdCls3;

	CStructAnalyse* m_pCStructAnalyse;

};

