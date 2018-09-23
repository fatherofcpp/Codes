#pragma once
#include "bsabstractcamera.h"
namespace NSP_STP_CM
{
	class CMySqlDB;
}

class CAbstractAnalyse :
	public CAbstractCamera
{
public:
	CAbstractAnalyse(void);
	virtual ~CAbstractAnalyse(void);
public:
	//��ʼ��
	virtual int TaskStart();
	//�����豸��Ϣ
	virtual void SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo);
	//�����¶�
	virtual int AnalyzeFrameTemp(NSP_STP_CM::CMySqlDB* p_CMySqlDB , CFrameInf* p_frameCBInf) = 0;
	//ͬ��ȼ��ͼ������·��
	virtual void UpdataFramePath(std::string frame_path) = 0;
	//������������������ع�
	virtual void SetCameraCmOpt();
public:
	//ͼ��ص�����
	static void CALLBACK frame_cb(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//�����ص�����
	static void CALLBACK heart_beat_cb(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//ͼ��ɼ��߳�
	static unsigned int WINAPI thread_pull(void* pParam);
	//ͼ��洢�߳�
	static unsigned int WINAPI thread_store(void* pParam);
	//ͼ������߳�
	static unsigned int WINAPI thread_analyse(void* pParam);
public:
	int m_preAnalyzMs;
};

