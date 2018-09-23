#pragma once
#include "bsabstractservicenet.h"

#include <string>
#include <vector>

struct tag_st_local_conf;
struct tag_st_pis_pack_inf;
struct tag_st_dbmysql_param;
class CMonitor;

class CServiceSpare :
	public CAbstractServiceNet
{
public:
	CServiceSpare(void);
	~CServiceSpare(void);

public:
	//��ʼ��
	virtual int TaskStart();

	//��ջ���
	virtual void ReleaseRes();

	//���ñ��ض˿�
	void SetTcpPort(int tcp_port){m_tcpPort = tcp_port;}
	void SetMonitor(CMonitor* p_CMonitor){m_pCMonitor = p_CMonitor;};
	void SetServerInf(std::string ip, int port);

private:
	static unsigned int WINAPI thread_spare_server_communicate(void* pParam);
	static unsigned int WINAPI thread_spare_client_communicate(void* pParam);
private:
	CMonitor* m_pCMonitor;
	//���ݿ���Ϣ

	std::string m_serverIp;
	int m_serverPort;
};

