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
	//����ʼ
	virtual int TaskStart() = 0;
	//�����豸��Ϣ
	virtual void SetDevInf(tagAbstractDeviceInfo* p_AbstractDeviceInfo, tagAbstractFormatInfo* p_AbstractFormatInfo) = 0;
	//�����������
	virtual void SetCameraOpt() = 0;
	//�ɼ������ʼ��
	virtual int CameraPictureInit();
	//��������ʼ��
	virtual int CameraVideoInit();
	//����
	virtual bool PlayCam(unsigned int uUseCPU);
	//ֹͣ
	virtual bool StopCam();
	//��ͣ
	virtual bool PauseCam();
	//����״̬���
	virtual bool CheckWorkState();
	//ͼ�����
	virtual int AnalyseFrame(CFrameInf* p_frameCBInf);
	//�洢
	virtual int Store(CFrameInf* p_frameCBInf);
	//�洢�屣��
	virtual void AddStoreFrame(CFrameInf* p_frameCBInf);
	//�����屣��
	virtual int AddAnalyseFrame(CFrameInf* p_frameCBInf);
	//�洢��ȡ��
	virtual CFrameInf* PickStoreFrame();
	//������ȡ��
	virtual CFrameInf* PickAnalyzeFrame();
	//��ջ���
	virtual void ReleaseRes();
	//��ȡͼ�񱣴�·��
	virtual std::string GetDownFullPath(bool is_video);
	//����������
	virtual void SetCameraInd(int camera_ind);
	//�߳�״̬�ж�
	virtual int ThreadStateCheck(int thread_ind);
	
public:
	//�ļ��洢·��
	std::string m_rootPath;
	//�ļ��洢·��-����
	std::string m_rootPathAlarm;
	//��洢��׺
	std::string m_saveFrameSuffix;
	//��Ƶ�洢��׺
	std::string m_saveVideoSuffix;
	//��������Ԥ���ռ�
	int m_diskNeedSpace;
	//֡��
	float m_fps;
	//������Ԥ��ʱ��
	int m_jgqPreReadyTms;
	// �����
	int m_carriageNum;
	// ��·��
	int m_lineId;
	//�洢ͼ����
	std::deque<CFrameInf*> m_frameStore;
	CCriticalSection m_frameStoreLock;
	//����ͼ����
	std::deque<CFrameInf*> m_frameAnalyse;
	CCriticalSection m_frameAnalyseLock;
	//�澯�ϱ���
	CAlarm* m_pCAlarm;
	//Mvbͨ����
	//CMvb* m_pCMvb;
	//�豸��Ϣ
	tagAbstractDeviceInfo* m_pAbstractDeviceInfo;
	tagAbstractFormatInfo* m_pAbstractFormatInfo;
	CObjectService* m_pCObjectService;
	CObjectDevice* m_pCObjectDevice;
	//���ݿ���Ϣ
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
	//������
	int m_cameraInd;
	//���������
	tag_e_frame_analyse_type m_frameAnalyzeType;
	//�������
	unsigned int m_cameraHeartBeat;
	//�������
	bool m_cameraOnline;
	//֡��
	long long m_frameCount;
	//��ʼʱ���
	unsigned int m_beginTimeStampMs;
	//��󻺳�֡��
	unsigned int m_maxFrameCount;

};

