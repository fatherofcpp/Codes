#pragma once
#include "Thread/CmThreadPool.h"
#include <string>

class CMainTask;

namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}


namespace MVB
{
	class CServerTrans;
	struct tag_st_mvb_recv_data;
	struct tag_st_mvb_alarm_data;
	struct tag_st_station_info;
}

class CMvb :
	public NSP_STP_CM::CThreadPool
{
public:
	CMvb(void);
	~CMvb(void);
public:
	//��ʼ��
	virtual int TaskStart();

	//���ô�����Ϣ
	void SetSerialInf(std::string serial_port_mvb, int baud_rate_mvb, std::string serial_port_scm, int baud_rate_scm);
	//�������߳�ָ��
	void SetMainTask(CMainTask* p_CMainTask){m_pCMainTask = p_CMainTask;};
	//����MVBλ�ñ��
	void SetMvbPos(int mp_code){m_mvbPos = mp_code;};
	//�������ݿ���Ϣ
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* p_st_dbmysql_param){m_pDbMysqlParam = p_st_dbmysql_param;};

	//��ȡ���յ�MVB����
	MVB::tag_st_mvb_recv_data* GetMvbRecvData();
	//��ȡMVB����״̬
	bool GetMvbState(){return m_mvbState;}
	//��ȡ��ǰվ����Ϣ
	MVB::tag_st_station_info* GetCurrentStationInf();
	int GetCurrentStationInd();
	//��ȡվ����Ϣ
	MVB::tag_st_station_info* GetStationInfByInd(int station_ind);
	//������Ӧ�ı���,������true�ı�����������ȫ����
	MVB::tag_st_mvb_alarm_data* GetMvbAlarmData(){return m_pMvbAlarmData;};
	CCriticalSection* GetMvbAlarmLock(){return &m_MvbAlarmDataLock;}
	//��ջ���
	void ReleaseRes();
	void SetTrainNum(int train_num){m_trainNum = train_num;}
	void SetVideoMaxSec(int video_max_sec){m_videoMaxSec = video_max_sec;}
private:
	//����MVB״̬
	void SetMvbState(bool state);
private:
	//mvbͨ���߳�
	static unsigned int WINAPI thread_mvb(void* pParam);
	static unsigned int WINAPI thread_scm(void* pParam);
	static unsigned int WINAPI thread_clear_alarm(void* pParam);
private:
	CMainTask* m_pCMainTask;
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;//���ݿ�

	MVB::CServerTrans* m_pCServerTrans;//����ӿ�
	std::string m_serialPortMvb;//"COM1"
	int m_baudRateMvb;//38400
	int m_mvbPos;//1=MP1,2=MP2

	MVB::tag_st_mvb_recv_data* m_pMvbRecvData;//���յ�����
	MVB::tag_st_mvb_alarm_data* m_pMvbAlarmData;//���͵ı���
	CCriticalSection m_MvbAlarmDataLock;

	bool m_mvbState;//ͨ��״̬

	HANDLE m_comScmHand;
	std::string m_serialPortScm;//"COM2"
	int m_baudRateScm;//38400
	int m_trainNum;

	int m_videoMaxSec;
};

