#pragma once
#include "BsAbstractCamera.h"

struct _Msg_TimingInfo;
struct _Video_ADDTextInfo;
struct _Msg_PresetInfo;
struct _Msg_CameraSwitchInfo;
struct _Msg_PantoLedSwitchInfo;
struct _Msg_PtzControlInfo;
struct _Msg_SearchVideoInfo;
struct _Msg_SearchJpgInfo;

struct tag_st_pis_pack_inf;

namespace VideoNSP
{
	struct tag_st_video_copy;
}

class CMonitor :
	public CAbstractCamera
{
public:
	CMonitor(void);
	~CMonitor(void);
public:
	//初始化
	virtual int TaskStart();
	//设置设备信息
	virtual void SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo);
	//设置相机参数
	virtual void SetCameraOpt(){return;};
	//图像分析
	virtual int AnalyseFrame(CFrameInf* p_frameCBInf){return 0;};
	//存储
	virtual int Store(CFrameInf* p_frameCBInf){return 0;};
	//存储桢保存
	virtual void AddStoreFrame(CFrameInf* p_frameCBInf){return;};
	//分析桢保存
	virtual int AddAnalyseFrame(CFrameInf* p_frameCBInf){return 0;};
	//存储桢取出
	virtual CFrameInf* PickStoreFrame(){return 0;};
	//分析桢取出
	virtual CFrameInf* PickAnalyzeFrame(){return 0;};
	//清空缓存
	virtual void ReleaseRes(){return;};

	//控制
	int Control(int cmd_type, const char* cmd);
	//下载下一个视频
	bool PlayNextCam(std::string download_path);
	//设置预置点个数
	void SetPresetNum(int preset_num){m_presetNum = preset_num;}
	void SetPantoPos(int panto_pos){m_pantoPos = panto_pos;}

	//校时
	void OptionTimer(_Msg_TimingInfo timing_Info);
	//字符叠加
	void OptionAddText(_Video_ADDTextInfo video_ADDTextInfo);
	//预置位设置
	void OptionPreset(_Msg_PresetInfo preset_Info);
	//相机开关
	void OptionCameraSwitch(_Msg_CameraSwitchInfo camera_SwitchInfo);
	//补光灯开关
	void OptionLedSwitch(_Msg_PantoLedSwitchInfo led_switchInfo);
	//云台控制
	void OptionPtz(_Msg_PtzControlInfo ptz_ControlInfo);

	//视频拷贝
	void OptionBuildVideo(VideoNSP::tag_st_video_copy video_copy);

	void SetRunSpare(bool is_run_spare);

public:
	int HandlePisCmd(tag_st_pis_pack_inf* p_pis_pack_inf, bool& is_wait_heart_beat, std::vector<tag_st_pis_pack_inf>* p_vec_send_pack);
private:
	void SetPresetReset(bool is_reset){m_presetReset = is_reset;}

	int GetAlarmMessageList(std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf);
	int GetVideoRecordList(_Msg_SearchVideoInfo SearchInfo, std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf);
	int GetAlarmPictureList(_Msg_SearchJpgInfo SearchJpgInfo, std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf);
	int GetAlarmVideoList(_Msg_SearchVideoInfo SearchVideoInfo, std::vector<tag_st_pis_pack_inf>* p_vec_pack_inf);

private:
	//心跳回调函数
	static void CALLBACK heart_beat_cb(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//拉流线程
	static unsigned int WINAPI thread_pull(void* pParam);
	//云台控制线程
	static unsigned int WINAPI thread_control(void* pParam);
private:
	int m_presetNum;//预置点个数
	int m_presetNo;//当前预置号
	int m_pantoPos;//受电弓位置号
	bool m_presetReset;//预置位是否复位,是则不轮询，否则轮询。
	CCriticalSection m_videoCtrLock;
	bool m_runSpare;//开启备份
	bool m_firstRun;//开机启动
};

