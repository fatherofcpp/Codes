#pragma once
#include "Thread/CmThreadPool.h"

#include <string>
#include <deque>
#include <vector>

struct tagAbstractDeviceInfo;
struct tagAbstractFormatInfo;
enum tag_e_frame_analyse_type;
namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}
class CAlarm;
class CFrameInf;
class CObjectService;
class CObjectDevice;

class CAbstractCamera: public NSP_STP_CM::CThreadPool
{
public:
	CAbstractCamera(void);
	virtual ~CAbstractCamera(void);
public:
	//任务开始
	virtual int TaskStart() = 0;
	//设置设备信息
	virtual void SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo) = 0;
	//设置相机参数
	virtual void SetCameraOpt() = 0;
	//采集相机初始化
	virtual int CameraPictureInit();
	//监控相机初始化
	virtual int CameraVideoInit();
	//播放
	virtual bool PlayCam(unsigned int uUseCPU);
	//停止
	virtual bool StopCam();
	//暂停
	virtual bool PauseCam();
	//运行状态检测
	virtual bool CheckWorkState();
	//图像分析
	virtual int AnalyseFrame(CFrameInf* p_frameCBInf);
	//存储
	virtual int Store(CFrameInf* p_frameCBInf);
	//存储桢保存
	virtual void AddStoreFrame(CFrameInf* p_frameCBInf);
	//分析桢保存
	virtual int AddAnalyseFrame(CFrameInf* p_frameCBInf);
	//存储桢取出
	virtual CFrameInf* PickStoreFrame();
	//分析桢取出
	virtual CFrameInf* PickAnalyzeFrame();
	//清空缓存
	virtual void ReleaseRes();
	//获取图像保存路径
	virtual std::string GetDownFullPath(bool is_video);
	//设置相机编号
	virtual void SetCameraInd(int camera_ind);
	//线程状态判断
	virtual int ThreadStateCheck(int thread_ind);
	
public:
	//文件存储路径
	std::string m_rootPath;
	//文件存储路径-报警
	std::string m_rootPathAlarm;
	//桢存储后缀
	std::string m_saveFrameSuffix;
	//视频存储后缀
	std::string m_saveVideoSuffix;
	//磁盘最少预留空间
	int m_diskNeedSpace;
	//帧率
	float m_fps;
	//激光器预热时间
	int m_jgqPreReadyTms;
	// 车厢号
	int m_carriageNum;
	// 线路号
	int m_lineId;
	//存储图像桢
	std::deque<CFrameInf*> m_frameStore;
	CCriticalSection m_frameStoreLock;
	//分析图像桢
	std::deque<CFrameInf*> m_frameAnalyse;
	CCriticalSection m_frameAnalyseLock;
	//告警上报类
	CAlarm* m_pCAlarm;
	//Mvb通信类
	//CMvb* m_pCMvb;
	//设备信息
	tagAbstractDeviceInfo* m_pAbstractDeviceInfo;
	tagAbstractFormatInfo* m_pAbstractFormatInfo;
	CObjectService* m_pCObjectService;
	CObjectDevice* m_pCObjectDevice;
	//数据库信息
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
	//相机编号
	int m_cameraInd;
	//桢分析类型
	tag_e_frame_analyse_type m_frameAnalyzeType;
	//相机心跳
	unsigned int m_cameraHeartBeat;
	//相机在线
	bool m_cameraOnline;
	//帧数
	long long m_frameCount;
	//开始时间戳
	unsigned int m_beginTimeStampMs;
	//最大缓冲帧数
	unsigned int m_maxFrameCount;

};

