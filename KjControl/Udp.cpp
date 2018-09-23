#include "StdAfx.h"
#include "Udp.h"

#include "MsgDefinition.h"
#include "utility.h"
#include "FIFO.h"
#include "Cfg.h"

using namespace std;


CUdp::CUdp(void):CThread(false),
	m_ser(0), 
	m_encoderCnt(0),
	m_queueMsg(NULL)
{
	trainStation = "";
	trainStorage = ""; //车库
	trainGudao = "";   //股道
	trainType = "";	   //车型	
	trainNum = "";     //车号
	trainPort = "";    //端位
	istrainconn = false;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
}
int CUdp::initlized(unsigned short port)
{
	m_port = port;
	memset(&m_serAddr,0,sizeof(SOCKADDR_IN));	
	//this->m_pQueue = queue;
	::InitializeCriticalSection(&m_cs);
	m_queueMsg = new queue<string>;
	this->m_release = true;
	return 0;
}

void CUdp::SetMSGSendThreadID(unsigned int id)
{
	nSendID = id;
}

/* UDP 对平台的接收通信，接收到的消息通知work.cpp中的testthread线程*/
int CUdp::Run()
{
	DWORD dw = 0;
	WSADATA data;
	int err = 0;
	WORD version = MAKEWORD(2,2);
	err = WSAStartup(version,&data);
	if (err)
	{
		return 0;
	}
	if (LOBYTE(version) != 2 || HIBYTE(version) != 2)
	{
		WSACleanup();
	}
	m_serAddr.sin_family = AF_INET;
	m_serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serAddr.sin_port = htons(m_port);

	m_ser = socket(AF_INET,SOCK_DGRAM,0);
	if (m_ser == INVALID_SOCKET)
	{
		WSACleanup();
		return 0;
	}
	unsigned int TimeOut = 10;
	::setsockopt(m_ser,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	if (bind(m_ser,(SOCKADDR*)&m_serAddr,sizeof(SOCKADDR_IN)))
	{
		WSACleanup();
		return 0;
	}

	string str;
	int ind;
	CString stro,cstr;
	string tmp, cmd;
	static int recvTick = 0;
	const  int timeEvel = 10;
	while(this->m_running)
	{
		//SOCKADDR_IN addrClient = {0};
		//TRACE("CUdp thread running, threadID = %d\r\n", this->GetThreadID());
		ZeroMemory(&addrClient, sizeof(SOCKADDR_IN));
		int len = sizeof(SOCKADDR);
		int ncount = -1;
		int ret = 0;
		memset(m_recvbuff,'\0',1024);
		ncount = recvfrom(m_ser,m_recvbuff,1024,0,(SOCKADDR*)&addrClient,&len);
		//当已经收到了APP的消息，或者已经发送APP的消息后，再重复发送的消息不做处理
		/*
			{start,库&股道&车型&车号&端位}
			{estop,库&股道}
			{home,库&股道}
			{go,库&股道&x}
			{back,库&股道&x}
		*/
		
		if (0 < ncount)
		{
			str = m_recvbuff;
			cstr.Format("收到APP手持机信息：%s\r\n", str.c_str());
			//CConsole::GetInstance()->Output(cstr.GetString());
			//CLog::GetInstance()->WriteLog(cstr, logDay);
			CFIFO::GetInstance()->Write(cstr.GetString());
#if 0			
			recvTick++;
			if(recvTick == 1)
			{
				QueryPerformanceCounter(&litmp);
				QPart1 = litmp.QuadPart;// TIME1	
			}
			else
			{
				QueryPerformanceCounter(&litmp);
				QPart2 = litmp.QuadPart;// TIME2

				dfMinus = (double)(QPart2 - QPart1);
				dfTim = dfMinus / dfFreq;// eclipse

				
				if(dfTim < timeEvel)   // < 10s  
				{
					QPart1 = QPart2;
					//CConsole::GetInstance()->Output("recvTime < 10s \r\n");
					continue;
				}
				else				   // > 10s
				{
					QPart1 = QPart2;
					recvTick = 1;
					//CConsole::GetInstance()->Output("recvTime > 10s \r\n");
				}
			}
#endif
			//CConsole::GetInstance()->Output("excuted command \r\n");
#if 0	
			//字符串解析过程			
			if(StringFind(str.c_str(),"start")>-1)
			{
				//当小车在运动过程中收到启动命令不做处理
				//if(CStatus::GetInstance()->GetTrainState() == CAR_RUN_FORW )
				//	continue;
				tmp = "{start,ok}";
				addrClient.sin_port = htons(10000);
				ret = sendto(m_ser, tmp.c_str(), tmp.size(), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
				cstr.Format("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//CConsole::GetInstance()->Output(cstr.GetString());
				//CLog::GetInstance()->WriteLog(cstr, logDay);
				CFIFO::GetInstance()->Write(cstr.GetString());

				//CStatus::GetInstance()->SetTrainState(CAR_SND_APP);

				ind = str.find(',',0);
				string left = str.substr(1,ind-1);
				string right = str.substr(ind+1, str.size()- ind -1-1);
				cmd = left;

				ind = right.find('&', 0);
				trainStorage = right.substr(0, ind);
				right = right.erase(0, ind+1);

				ind = right.find('&', 0);
				trainGudao = right.substr(0, ind);
				right = right.erase(0, ind+1);

				ind = right.find('&', 0);
				trainType = right.substr(0, ind);
				right = right.erase(0, ind+1);

				ind = right.find('&', 0);
				trainNum = right.substr(0, ind);
				right = right.erase(0, ind+1);

				ind = right.find('&', 0);
				trainPort = right.substr(0, ind);
				right = right.erase(0, ind+1);

				ind = right.find('&', 0);
				trainMs = right.substr(0, ind);//主机/从机 信息
				if(!strcmp("Master",trainMs.c_str()))
				{
					/// TODO 当前设备是主机，APP发来的信息匹配则处理
				}
				else
				{
					/// TODO 当前设备是主机，APP发来的信息不匹配则不处理
				}
				
				PostThreadMessage( nSendID, APP_CMD_START, 0 , 0 );
			}
#else
			if(StringFind(str.c_str(),"start")>-1)
			{
				tmp = "{start,ok}";
				addrClient.sin_port = htons(10000);
				ret = sendto(m_ser, tmp.c_str(), tmp.size(), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
				cstr.Format("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				
				CFIFO::GetInstance()->Write(cstr.GetString());

				vector<APPINFO> parsecmd;
				parsecmd.clear();
				int l, r;
				l = r = -1;

				cmd.assign(str);
				while(cmd.size()>0)
				{
					l = cmd.find_first_of('{', 0);
					r = cmd.find_first_of('}', 0);
					if(l >= 0 && r >= 0 && r>l)
					{
						string substr = cmd.substr(l,r-l+1);
						cmd.erase(l,r-l+1);
						if(StringFind(substr.c_str(),"start")>-1)
						{
							string ssub = substr.substr(1, substr.size()-2);
							vector<string> pps;
							pps.clear();
							Split(ssub, pps, ",");
							for(auto it = pps.begin(); it != pps.end(); it++)
							{
								if(*it != "start")
								{
									vector<string> ppstr;
									ppstr.clear();
									Split(*it, ppstr, "&");
									
									parsecmd.push_back(make_appinfo(ppstr.at(0),
										ppstr.at(1),
										ppstr.at(2),
										ppstr.at(3),
										ppstr.at(4),
										ppstr.at(5),
										ppstr.at(6)));
								}
							}
						}
					}
					else
					{
						stro.Format("收到手持机APP发来错误格式消息:%s ,\r\n", cmd.c_str());
						CFIFO::GetInstance()->Write(stro.GetString());
						cmd.clear();
					}
				}
				string lw = Ccfg::GetInstance()->GetTrainLW();
				//解析出来一条指令
				if(parsecmd.size()==1)
				{
					//走8编组的车长
					if(parsecmd.at(0).Gudaoliewei !=  lw)
						continue;
					trainStation = parsecmd.at(0).Station;
					trainStorage = parsecmd.at(0).Storage;
					trainGudao   = parsecmd.at(0).Gudao;
					trainType    = parsecmd.at(0).TrainType;
					trainNum     = parsecmd.at(0).TrainNo;
					trainPort	 = parsecmd.at(0).TrainPort;
					trainMs		 = parsecmd.at(0).Gudaoliewei;
					istrainconn  = false;
					PostThreadMessage( nSendID, APP_CMD_START, 0 , 0 );

				}
				//解析出来两条指令
				if(parsecmd.size()==2)
				{
					//走8+3.5节车长
					for(auto it = parsecmd.begin(); it != parsecmd.end(); it++)
					{
						if(it->Gudaoliewei != lw)
							continue;

						trainStation = it->Station;
						trainStorage = it->Storage;
						trainGudao   = it->Gudao;
						trainType    = it->TrainType;
						trainNum     = it->TrainNo;
						trainPort	 = it->TrainPort;
						trainMs		 = it->Gudaoliewei;
						istrainconn  = true;
						PostThreadMessage( nSendID, APP_CMD_START, 0 , 0 );

					}
				}
			}
#endif
			if(StringFind(str.c_str(),"estop")>-1)
			{
				tmp = "{estop,ok}";
				addrClient.sin_port = htons(10000);
				ret = sendto(m_ser, tmp.c_str(), tmp.size(), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
				//CConsole::GetInstance()->Output("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//log
				stro.Format("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//CLog::GetInstance()->WriteLog(stro, logDay);

				PostThreadMessage( nSendID, APP_CMD_ESTOP, 0 , 0 );
			}

			if(StringFind(str.c_str(),"home")>-1)
			{
				tmp = "{home,ok}";
				addrClient.sin_port = htons(10000);
				ret = sendto(m_ser, tmp.c_str(), tmp.size(), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
				//CConsole::GetInstance()->Output("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//log
				stro.Format("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//CLog::GetInstance()->WriteLog(stro, logDay);

				PostThreadMessage( nSendID, APP_CMD_HOME, 0 , 0 );
			}

			if(StringFind(str.c_str(),"go")>-1)
			{
				tmp = "{go,ok}";
				addrClient.sin_port = htons(10000);
				ret = sendto(m_ser, tmp.c_str(), tmp.size(), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
				//CConsole::GetInstance()->Output("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//log
				stro.Format("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//CLog::GetInstance()->WriteLog(stro, logDay);

				PostThreadMessage( nSendID, APP_CMD_GO, 0 , 0 );
			}

			if(StringFind(str.c_str(),"back")>-1)
			{
				tmp = "{back,ok}";
				addrClient.sin_port = htons(10000);
				ret = sendto(m_ser, tmp.c_str(), tmp.size(), 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
				//CConsole::GetInstance()->Output("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//log
				stro.Format("ret = %d, 发送应答消息: %s 到IP:  %s, PORT: %d \r\n", ret, tmp.c_str(), inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
				//CLog::GetInstance()->WriteLog(stro, logDay);

				PostThreadMessage( nSendID, APP_CMD_BACK, 0 , 0 );
			}
		}

		Sleep(10);
	}

	closesocket(m_ser);
	return 0;
}

int CUdp::SenMsg2App(char* buff, int sendlen)
{
	int ret = -1;
	ret = sendto(m_ser, buff, sendlen, 0, (SOCKADDR*)&addrClient, sizeof(SOCKADDR));
	return ret;
}

CUdp::~CUdp()
{
	if(m_queueMsg != NULL)
	{
		delete m_queueMsg;
		m_queueMsg = NULL;
	}
	::DeleteCriticalSection(&m_cs);	
	TRACE("CUdp id = %d, exit\r\n", this->GetThreadID());
}