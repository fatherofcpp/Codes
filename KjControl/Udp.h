/*
	���Ƴ�����ֳֻ�APPͨ��
*/
#pragma once
#include "stdafx.h"
#include "thread.h"
#include <WinSock2.h>
#include <vector>
#include <string>
#include <queue>

class CUdp :
	public CThread
{
public:
	CUdp(void);
	virtual int Run();
	virtual ~CUdp(void);
	int initlized(unsigned short port);	
	//int &GetCurCnt(){return m_encoderCnt;};
	//queue<string>* GetMsgQueue(){return m_queueMsg;}
	string GetTrainStation() {return trainStation;}
	string GetTrainStorage(){return trainStorage;}	//�����
	string GetTrainGudao(){return trainGudao;}		//�ɵ���
	string GetTrainType(){return trainType;}		//���ͺ�
	string GetTrainNum(){return trainNum;}			//�����
	string GetTrainPort(){return trainPort;}		//��λ
	string GetTrainMsship(){return trainMs;};		//����/�ӻ�
	bool   IsTrainConn() {return istrainconn;}
	CRITICAL_SECTION m_cs;
	void SetMSGSendThreadID(unsigned int id);		//������ϢҪ���͵��ĸ��̵߳�ID
	int  SenMsg2App(char* buff, int sendlen);		//������Ϣ��APP�ֳֻ�
private:
	SOCKET m_ser;
	SOCKADDR_IN m_serAddr;	//local addr_struct	
	SOCKADDR_IN addrClient;	//remote addr_struct 
	char buf[1024];
	unsigned int m_port;
	char m_recvbuff[1024];
	int m_encoderCnt;		//����������	
	queue<string>* m_queueMsg;
	string trainStation;	//������
	string trainStorage;	//����
	string trainGudao;		//�ɵ�
	string trainType;		//����	
	string trainNum;		//����
	string trainPort;		//��λ
	string trainMs;			//����/�ӻ�	
	bool   istrainconn;		//������

	unsigned int nSendID;	//�Զ�����ϢҪ���͵��߳�ID 

	LARGE_INTEGER litmp; 
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
};
