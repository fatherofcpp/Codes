#pragma once
#include "BsServicePis.h"

class CServicePtu :
	public CServicePis
{
public:
	CServicePtu(void);
	~CServicePtu(void);
public:
	//初始化
	virtual int TaskStart();

	//清空缓存
	virtual void ReleaseRes();

	//设置本地端口
	void SetUdpPtuPort(int udpPtuPort){m_udpPort = udpPtuPort;}

private:
	int m_udpPort;
};

