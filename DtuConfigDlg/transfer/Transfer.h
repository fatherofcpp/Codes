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
	void SetIpPort(st_ip_port_domain& ip_inf);//����ip���˿�
	void SetReadTimeOut(struct timeval& time_out);

	//uart
	void SetSerialConf(st_serial_conf*serial);

	//net && uart
	void SetTransType(e_transfer_type trans_type);//����ͨѶ����

	int SendReqAndReceiveBody(int send_len,int type);//���ڣ�����,�����
	void CloseFd();//�Ͽ�fd�������
	int GetFd();
	int ConnectFd();//����fd,��������


public:
	int OnSend(char *p_data, int i_data );
private:
	int OnReceive(char *p_data, int i_data, bool b_retry = true );
	void ClearSendData();

private:
	int sendDataLen;
	char* sendData;//���ݵķ�������

	e_transfer_type transType;//ͨ�ŷ�ʽ
	CCriticalSection SocketLock;//���߳�socket��
	CSerialAPI* pCSerialAPI;//����ͨ��
	CNetApi* pCNetApi;//����ͨ��
};
extern CTransfer gTransf;
