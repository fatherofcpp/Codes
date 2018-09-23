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
	//��ʼ��
	virtual int TaskStart();
	//�����豸��Ϣ
	virtual void SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo);
	//�����������
	virtual void SetCameraOpt(){return;};
	//ͼ�����
	virtual int AnalyseFrame(CFrameInf* p_frameCBInf){return 0;};
	//�洢
	virtual int Store(CFrameInf* p_frameCBInf){return 0;};
	//�洢�屣��
	virtual void AddStoreFrame(CFrameInf* p_frameCBInf){return;};
	//�����屣��
	virtual int AddAnalyseFrame(CFrameInf* p_frameCBInf){return 0;};
	//�洢��ȡ��
	virtual CFrameInf* PickStoreFrame(){return 0;};
	//������ȡ��
	virtual CFrameInf* PickAnalyzeFrame(){return 0;};
	//��ջ���
	virtual void ReleaseRes(){return;};

	//����
	int Control(int cmd_type, const char* cmd);
	//������һ����Ƶ
	bool PlayNextCam(std::string download_path);
	//����Ԥ�õ����
	void SetPresetNum(int preset_num){m_presetNum = preset_num;}
	void SetPantoPos(int panto_pos){m_pantoPos = panto_pos;}

	//Уʱ
	void OptionTimer(_Msg_TimingInfo timing_Info);
	//�ַ�����
	void OptionAddText(_Video_ADDTextInfo video_ADDTextInfo);
	//Ԥ��λ����
	void OptionPreset(_Msg_PresetInfo preset_Info);
	//�������
	void OptionCameraSwitch(_Msg_CameraSwitchInfo camera_SwitchInfo);
	//����ƿ���
	void OptionLedSwitch(_Msg_PantoLedSwitchInfo led_switchInfo);
	//��̨����
	void OptionPtz(_Msg_PtzControlInfo ptz_ControlInfo);

	//��Ƶ����
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
	//�����ص�����
	static void CALLBACK heart_beat_cb(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader);
	//�����߳�
	static unsigned int WINAPI thread_pull(void* pParam);
	//��̨�����߳�
	static unsigned int WINAPI thread_control(void* pParam);
private:
	int m_presetNum;//Ԥ�õ����
	int m_presetNo;//��ǰԤ�ú�
	int m_pantoPos;//�ܵ繭λ�ú�
	bool m_presetReset;//Ԥ��λ�Ƿ�λ,������ѯ��������ѯ��
	CCriticalSection m_videoCtrLock;
	bool m_runSpare;//��������
	bool m_firstRun;//��������
};

