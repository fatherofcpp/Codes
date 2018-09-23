#pragma once
#include "BsAbstractCamera.h"

struct tag_st_arc_inf;
struct tag_st_current_inf;
class CArcCollector;
class CStructAnalyse;

class CArcNotify :
	public CAbstractCamera
{
public:
	CArcNotify(void);
	~CArcNotify(void);
public:
	//��ʼ��
	virtual int TaskStart();
	//�����豸��Ϣ
	virtual void SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo);
	//�����������
	virtual void SetCameraOpt();
	//��ջ���
	virtual void ReleaseRes();

public:
	void SetUdpCurrentPort(int current_port){m_udpCurrentPort = current_port;}
	void SetArcCollector(CArcCollector* p_CArcCollector){m_pCArcCollector = p_CArcCollector;}
	void SetStructAnalyse(CStructAnalyse* p_CStructAnalyse){m_pCStructAnalyse = p_CStructAnalyse;}
	void SetArcRateThreshold(double arc_rate_cls1, double arc_rate_cls2, double arc_rate_cls3);
	void SetCurrentRated(int current_rated){m_currentRated = current_rated;}
	void SetArcThreshold(int arcBeginThreshold, int arcEndThreshold, int arcDeltTimeMsThreshold, 
		int arcContinuTimeUsThreshold, int arcStrenthXsThreshold, int arcTimeLenGainThreshold);
	void SetCurrentTimeoutS(int tm_outs){m_currentTimeoutS = tm_outs;}
private:
	//ȼ����Ϣ�ص�����
	static void CALLBACK arc_inf_frame_cb( const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//�����ص�����
	static void CALLBACK thread_heart_beat_cb(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//ȼ����Ϣ�ɼ��߳�
	static unsigned int WINAPI thread_pull_arcinf(void* pParam);
	//��������߳�
	static unsigned int WINAPI thread_current_recv(void* pParam);
	static unsigned int WINAPI thread_current_handle(void* pParam);
	//ȼ���ʷ����߳�
	static unsigned int WINAPI thread_analyse_arc(void* pParam);

private:
	void AddArcInf(tag_st_arc_inf* p_ac_inf);
	tag_st_arc_inf* PickArcInf();

	void AddCurrentInf(tag_st_current_inf* p_current_inf);
	tag_st_current_inf* PickCurrentInf();

	//ȼ���ʣ���ǰȼ��ʱ��/������������30%����ʱ��
	double ArcCalcOnce(tag_st_arc_inf* p_arc_inf);
	//ȼ���ʣ�����ȼ��ʱ��/���е��������30%����ʱ��
	double ArcCalcTotal(tag_st_arc_inf* p_arc_inf);
private:
	std::deque<tag_st_arc_inf*> m_arcInf;//ȼ����Ϣ,�Ѵ���
	CCriticalSection m_ArcInfLock;

	std::deque<tag_st_current_inf*> m_currentInf;//������Ϣ,�� ����
	CCriticalSection m_CurrentInfLock;

	int m_udpCurrentPort;

	double m_arcRateCls1;
	double m_arcRateCls2;
	double m_arcRateCls3;

	int m_currentRated;

	int m_arcBeginThreshold;//default:200
	int m_arcEndThreshold;//default:100
	int m_arcDeltTimeMsThreshold;//default:500
	int m_arcContinuTimeUsThreshold;//10us,defalt:3
	int m_arcStrenthXsThreshold;//default
	int m_arcTimeLenGainThreshold;

	CArcCollector* m_pCArcCollector;
	CStructAnalyse* m_pCStructAnalyse;

	//����������
	char m_handleBuf[256];

	int m_currentTimeoutS;

	time_t m_currentLastTimeMs;
	time_t m_arcLastTimeMs;
	unsigned int m_arcCbNum;
};

