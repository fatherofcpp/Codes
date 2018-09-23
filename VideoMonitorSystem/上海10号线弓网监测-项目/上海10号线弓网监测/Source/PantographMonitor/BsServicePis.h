#pragma once
#include "BsAbstractServiceNet.h"

#include <string>
#include <vector>

struct tag_st_local_conf;
struct tag_st_pis_pack_inf;
namespace NSP_STP_CM
{
	struct tag_st_dbmysql_param;
}

class CMonitor;
class CServicePis: public CAbstractServiceNet
{
public:
	CServicePis(void);
	~CServicePis(void);
public:
	//��ʼ��
	virtual int TaskStart();

	//��ջ���
	virtual void ReleaseRes();

	//���ñ��ض˿�
	void SetTcpPort(int tcpPisPort){m_tcpPort = tcpPisPort;}
	void SetPanToPos(int pan_to_pos){m_pantoPos = pan_to_pos;}
	void SetMonitor(CMonitor* p_CMonitor){m_pCMonitor = p_CMonitor;};
	void SetDbMySqlParam(const NSP_STP_CM::tag_st_dbmysql_param* dbmysql_param){m_pDbMysqlParam = dbmysql_param;};

private:
	//pisͨ���߳�
	static unsigned int WINAPI thread_pis_communicate(void* pParam);
private:
	int m_pantoPos;
	CMonitor* m_pCMonitor;
	//���ݿ���Ϣ
	const NSP_STP_CM::tag_st_dbmysql_param* m_pDbMysqlParam;
};

