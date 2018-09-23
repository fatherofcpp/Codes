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
	//初始化
	virtual int TaskStart();
	//设置设备信息
	virtual void SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo);
	//分析温度
	virtual int AnalyzeFrameTemp(NSP_STP_CM::CMySqlDB* p_CMySqlDB , CFrameInf* p_frameCBInf) = 0;
	//同步燃弧图像下载路径
	virtual void UpdataFramePath(std::string frame_path) = 0;
	//设置相机参数，增益曝光
	virtual void SetCameraCmOpt();
public:
	//图像回调函数
	static void CALLBACK frame_cb(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//心跳回调函数
	static void CALLBACK heart_beat_cb(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//图像采集线程
	static unsigned int WINAPI thread_pull(void* pParam);
	//图像存储线程
	static unsigned int WINAPI thread_store(void* pParam);
	//图像分析线程
	static unsigned int WINAPI thread_analyse(void* pParam);
public:
	int m_preAnalyzMs;
};

