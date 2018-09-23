/*
	控制程序和手持机APP通信
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
	string GetTrainStorage(){return trainStorage;}	//车库号
	string GetTrainGudao(){return trainGudao;}		//股道号
	string GetTrainType(){return trainType;}		//车型号
	string GetTrainNum(){return trainNum;}			//车组号
	string GetTrainPort(){return trainPort;}		//端位
	string GetTrainMsship(){return trainMs;};		//主机/从机
	bool   IsTrainConn() {return istrainconn;}
	CRITICAL_SECTION m_cs;
	void SetMSGSendThreadID(unsigned int id);		//设置消息要发送到哪个线程的ID
	int  SenMsg2App(char* buff, int sendlen);		//发送消息到APP手持机
private:
	SOCKET m_ser;
	SOCKADDR_IN m_serAddr;	//local addr_struct	
	SOCKADDR_IN addrClient;	//remote addr_struct 
	char buf[1024];
	unsigned int m_port;
	char m_recvbuff[1024];
	int m_encoderCnt;		//编码器计数	
	queue<string>* m_queueMsg;
	string trainStation;	//动车所
	string trainStorage;	//车库
	string trainGudao;		//股道
	string trainType;		//车型	
	string trainNum;		//车号
	string trainPort;		//端位
	string trainMs;			//主机/从机	
	bool   istrainconn;		//重连车

	unsigned int nSendID;	//自定义消息要发送的线程ID 

	LARGE_INTEGER litmp; 
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
};
