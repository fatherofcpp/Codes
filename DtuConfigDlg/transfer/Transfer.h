#pragma once
#include "DataTypeDef.h"
#include "Uart.h"
#include "netapi.h"

class CTransfer
{
public:
	CTransfer(void);
	~CTransfer(void);

public:
	//net
	void SetIpPort(st_ip_port_domain& ip_inf);//设置ip，端口
	void SetReadTimeOut(struct timeval& time_out);

	//uart
	void SetSerialConf(st_serial_conf*serial);

	//net && uart
	void SetTransType(e_transfer_type trans_type);//设置通讯类型

	int SendReqAndReceiveBody(int send_len,int type);//串口，网络,需加锁
	void CloseFd();//断开fd，需加锁
	int GetFd();
	int ConnectFd();//连接fd,串口网络


public:
	int OnSend(char *p_data, int i_data );
private:
	int OnReceive(char *p_data, int i_data, bool b_retry = true );
	void ClearSendData();

private:
	int sendDataLen;
	char* sendData;//备份的发送数据

	e_transfer_type transType;//通信方式
	CCriticalSection SocketLock;//多线程socket锁
	CSerialAPI* pCSerialAPI;//串口通信
	CNetApi* pCNetApi;//网络通信
};
extern CTransfer gTransf;
