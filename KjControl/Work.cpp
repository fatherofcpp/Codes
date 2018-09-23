#include "stdafx.h"
#include "Work.h"
#include "FIFO.h"
#include "MsgDefinition.h"
#include "utility.h"
#include <direct.h>
#include "Cfg.h"
#include <ATLTRACE.H>
#include "TrainInfo.h"
#include "LDSCommand.h"
#include "Reflect.h"
#include "TrainType.h"
#include "mtool.h"


//PLC通信接口
void CALLBACK plc_sock_proc(string str, LPVOID context)
{
	CWork* dlg = (CWork*)context;
	if(dlg != NULL)
		dlg->PlcSock(str);//
}
string CWork::PlcSock(string str)
{
	//parse
	CString cstr, subcstr;
	string substr;
	const char eofchar = '}';
	const char begchar = '{';

	int ind_beg = str.find(begchar, 0);
	int ind_end = str.find(eofchar, 0);
	if(ind_beg<0 || ind_end<0)
	{
		//str_recv.clear();
		cstr.Format("Respones Format error :%s\r\n");
		CFIFO::GetInstance()->Write(cstr.GetString());
		return "";
	}
	else
	{
		if(str.size()==strlen("{c,rv}"))
		{
			if(!strcmp(str.c_str(), "{c,rv}"))
			{
				SetEvent(plc_response_rv);
			}
			if(!strcmp(str.c_str(), "{c,ok}"))
			{
				SetEvent(plc_response_ok);
			}
		}
		else
		{
			if(!strcmp(plc_sock->Getcurrcmd().c_str(),"{c,pci}") && StringFind(str.c_str(), "c,ok")> 0)
			{
				lplcpulse = parse<long>((unsigned char*)const_cast<char*>(str.c_str()));
				SetEvent(plc_response_ok);
				CFIFO::GetInstance()->Write("{c,ok,x} ...\r\n");
			}
			if(!strcmp(plc_sock->Getcurrcmd().c_str(),"{c,dis}") && StringFind(str.c_str(), "c,ok")> 0)
			{
				fplcdis = parse<float>((unsigned char*)const_cast<char*>(str.c_str()));
				SetEvent(plc_response_ok);
				CFIFO::GetInstance()->Write("{c,ok,x1} ...\r\n");
			}
		}
		CFIFO::GetInstance()->Write(str+"\r\n");
	}
	return str;
}
//服务器通信接口
void CALLBACK ctrl2serv_proc(string str, LPVOID context)
{
	CWork* dlg = (CWork*)context;
	if(dlg != NULL)
		dlg->Ctrl2serv(str);//
}
string CWork::Ctrl2serv(string str)
{
	return str;
}

void CALLBACK plc_sock_dis_callback(string str, LPVOID context)
{
	CWork* dlg = (CWork*)context;
	if(dlg != NULL)
		dlg->PlcSockDis(str);
}

void CWork::PlcSockDis(string str)
{
	//TRACE("RUN 11111\r\n");
	char lkh = '{';
	char rkh = '}';
	int ret1 = str.find(lkh, 0);
	int ret2 = str.find(rkh, 0);
	if(ret1 <0 || ret2<0)
		return;

	float v = parse<float>((unsigned char*)const_cast<char*>(str.c_str()));
	fDis_Self->SetDis(v);
}

void CALLBACK recv_auto_dis_callback(string str, LPVOID context)
{
	CWork* dlg = (CWork*)context;
	if(dlg != NULL)
		dlg->RecvAutoDis(str);
}

void CWork::RecvAutoDis(string str)
{
	//TRACE("RUN 22222\r\n");
	float v = parse<float>((unsigned char*)const_cast<char*>(str.c_str()));
	fDis_Other->SetDis(v);
}

CWork::CWork(void):CThread(false),
	/*dev_lightpulse_cmd(NULL),
	dpj_pulse_cmd(NULL),
	dpj_pulse_data(NULL),
	mxdpj_pulse_data(NULL),*/
	plc_sock(NULL),
	ctl2serv(NULL),
	plc_response_rv(NULL),
	plc_response_ok(NULL),
	hHandle_DataProcess(NULL),
	dpj_pulse_ok(NULL),
	curr_pulse(0),
	old_pulse(0),
	plc_sock_dis(NULL),
	recv_auto_dis(NULL),
	isExit_RSAutocarDisLoop(false),
	isExit_LookupPLCDisLoop(false),
	fDis_Self(NULL),
	fDis_Other(NULL),
	isExit_CollisionAnlysis(false),
	mdis1(NULL),
	mdis2(NULL),
	currZjj("")
{
	plc_response_rv			= CreateEvent(NULL, TRUE, FALSE, NULL);
	plc_response_ok			= CreateEvent(NULL, TRUE, FALSE, NULL);
	hHandle_DataProcess		= CreateEvent(NULL, TRUE, FALSE, NULL);
	dpj_pulse_ok			= CreateEvent(NULL, TRUE, FALSE, NULL);

	vecLds1.clear();
	vecLds2.clear();

	fDis_Self = make_shared<AutocarDis>();
	fDis_Other = make_shared<AutocarDis>();
	mdis1 = make_shared<TCPClientImpl>();
	mdis2 = make_shared<TCPClientImpl>();
	mdis1->MdisVecClear();
	mdis2->MdisVecClear();

	memset(zxjRun,0 ,16*sizeof(int));
}

CWork::~CWork(void)
{
	ctl2serv->SetRunning(false);
	plc_sock->SetRunning(false);
	handpad_cmd.SetRunning(false);

	plc_sock_dis->SetRunning(false);
	recv_auto_dis->SetRunning(false);
	
	CloseHandle(plc_response_rv);
	CloseHandle(plc_response_ok);
	CloseHandle(hHandle_DataProcess);

	isExit_RSAutocarDisLoop = true;
	isExit_LookupPLCDisLoop = true;

	mdis1->SetRunning(false);
	mdis2->SetRunning(false);

}

int CWork::Initialize()
{
	bool ret = false;
	int mxret ;
	CString cstr;
	string  stra, strb;
	int COM, BD;
	
	//手持机
	BD = Ccfg::GetInstance()->GetHandpadPort();
	handpad_cmd.initlized(BD); //手持机与控制程序通信端口：6000
	handpad_cmd.SetMSGSendThreadID(this->m_nID);	//设置线程的ID		
	handpad_cmd.SetRelease(true);
	TRACE("handpad_cmd thread ID = %d\r\n", handpad_cmd.GetThreadID());
	//线阵面阵相机
	stra = Ccfg::GetInstance()->GetLinecameraIP();
	BD = Ccfg::GetInstance()->GetLinecameraPort();
	linecamera_cmd.initlized(BD,stra, BD);
	stra = Ccfg::GetInstance()->GetAreacameraIP();
	BD = Ccfg::GetInstance()->GetAreacameraPort();
	areacamera_cmd.initlized(BD,stra, BD);
	
#if 1
	//PLC网络通信
	stra = Ccfg::GetInstance()->GetPlcIP();
	BD = Ccfg::GetInstance()->GetPlcPort();
	plc_sock = make_shared<CUdp2Extend>();
	//plc_sock->initlized(BD, stra,BD);
	plc_sock->initlized(2000, stra,BD);
	plc_sock->SetCallBack(plc_sock_proc, this);
	plc_sock->SetRelease(true);
	TRACE("plc_sock thread ID = %d\r\n", plc_sock->GetThreadID());

	plc_sock_dis = make_shared<CUdp2Extend>();
	plc_sock_dis->initlized(2002, "192.168.1.180",2002);
	plc_sock_dis->SetCallBack(plc_sock_dis_callback, this);	//接收到PLC发来的最新位置信息
	plc_sock_dis->SetRelease(true);
	TRACE("plc_sock_dis thread ID = %d\r\n", plc_sock_dis->GetThreadID());
#endif
	//控制到服务器通信
	stra = Ccfg::GetInstance()->GetServerIP();
	BD = Ccfg::GetInstance()->GetServerPort();
	ctl2serv = make_shared<CUdp2Extend>();
	ctl2serv->initlized(BD, stra, BD);
	ctl2serv->SetCallBack(ctrl2serv_proc, this);
	ctl2serv->SetRelease(true);
	TRACE("ctl2serv thread ID = %d\r\n", ctl2serv->GetThreadID());
#if 1//temp mark for shijiao
	recv_auto_dis = make_shared<CUdp2Extend>();
	recv_auto_dis->initlized(6009, "192.168.1.170", 6009);
	recv_auto_dis->SetCallBack(recv_auto_dis_callback, this); //接收到对方发来的最新小车位置
	recv_auto_dis->SetRelease(true);
	//_beginthread(RSAutocarDisLoop, 0, this);		//两台小车互发距离			
	//_beginthread(LookupPLCDisLoop, 0, this);		//当前小车查询距离	
#endif
	string xike1ip = Ccfg::GetInstance()->Getxike1ip();
	int    xike1port = Ccfg::GetInstance()->Getxike1pport();
	string ctr1ip = Ccfg::GetInstance()->Getctrl1ip();
	int    ctr1port = Ccfg::GetInstance()->Getctrl1port();
	//mdis1->Inital(ctr1ip, ctr1port, xike1ip, xike1port);
	mdis1->SetRelease(true);
	
	
	string xike2ip = Ccfg::GetInstance()->Getxike2ip();
	int    xike2port = Ccfg::GetInstance()->Getxike2pport();
	string ctr2ip = Ccfg::GetInstance()->Getctrl2ip();
	int    ctr2port = Ccfg::GetInstance()->Getctrl2port();
	//mdis2->Inital(ctr2ip, ctr2port, xike2ip, xike2port);
	mdis2->SetRelease(true);
	return 0;
}
/*
	两台小车相互之间对发“小车距离值”
*/
void CWork::RSAutocarDisLoop(LPVOID param) 
{
	CWork* dlg = (CWork*)param;
	char buff[12] = {0};
	string str;
	while(!dlg->isExit_RSAutocarDisLoop)
	{
		memset(buff, 0, 12);
		float v = dlg->GetDisSelf()->GetDis();
		sprintf(buff, "%f", v);
		dlg->recv_auto_dis->Send(buff, strlen(buff));
		Sleep(1000);
	}
	TRACE("RSAutocarDisLoop exit\r\n");
}
/*
	小车向PLC查询当前的“小车距离值”
*/
void CWork::LookupPLCDisLoop(LPVOID param)
{
	CWork* dlg = (CWork*)param;
	string cmd = "{c,dis}";
	while(!dlg->isExit_LookupPLCDisLoop)
	{
		dlg->plc_sock_dis->Send(const_cast<char*>(cmd.c_str()), cmd.size());
		Sleep(1000);
	}
	TRACE("LookupPLCDisLoop exit\r\n");
}

void CWork::CollisionAnlysis(LPVOID param)
{
	CWork* dlg = (CWork*)param;
	float vselfpos, votherpos;
	string cmd1 = "{c,estop}";
	string cmd2 = "{c,recover}";
	bool isSelfStop = false;
	while(1)
	{
		vselfpos = dlg->GetDisSelf()->GetDis();
		votherpos = dlg->GetDisOther()->GetDis();
		if(isSelfStop == false && vselfpos < votherpos && vselfpos >175)
		{
			dlg->GetPLCSockDis()->Send(const_cast<char*>(cmd1.c_str()), cmd1.size());
			isSelfStop = true;
		}
		if(isSelfStop == true && vselfpos > 175 && votherpos < 175)
		{
			dlg->GetPLCSockDis()->Send(const_cast<char*>(cmd2.c_str()), cmd2.size());
			isSelfStop = false;
		}
		Sleep(500);
	}
	
}

int CWork::Run()
{
	if(!isRunningMode())
	{
		//Debug Mode
		CFIFO::GetInstance()->Write("Control程序进入调试模式 ... ... \r\n");
	}
	else
	{
		//Running Mode
		CFIFO::GetInstance()->Write("Control程序进入自动模式 ... ... \r\n");
		string str = Ccfg::GetInstance()->GetTrainLW();
		CFIFO::GetInstance()->Write("当前小车列位: " + str + "\r\n");
		while(this->m_running)
		{
			MSG	msg;
			BOOL MsgReturn = PeekMessage(&msg, NULL, APP_CMD_START, APP_CMD_BACK, PM_REMOVE);
			if(MsgReturn)
			{
				switch(msg.message)
				{
				case APP_CMD_START: 
					InitVec();
					AutoRun();
					break;
				case APP_CMD_ESTOP:
					EstopAction();
					break;
				case APP_CMD_HOME:
					HomeAction();
					break;
				case APP_CMD_GO:
					GoAction();
					break;
				case APP_CMD_BACK:
					BackAction();
					break;
				}
			}
		}
	}
	TRACE("CWork thread exit ... ... \r\n");
	return 0;
}

bool CWork::isRunningMode()
{
	//add detail code here
	int mode = Ccfg::GetInstance()->GetRunningMode();
	if(mode)
		return true;
	else
		return false;
}

void CWork::KsLightOC(bool isSetting)
{
	string cmd_o = "{jiecheks}";
	string cmd_c = "{jiechejs}";
	string cmd;
	int ret;
	CString cstr;
	Sleep(100);
	if(isSetting)
	{
		Sleep(100);
		cmd = cmd_o+"11";
		ret = linecamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
		Sleep(100);
		
		cmd = cmd_o+"19";
		ret = linecamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
		Sleep(100);
		
		cmd = cmd_o+"11";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
		Sleep(100);

		cmd = cmd_o+"19";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
	}
	else
	{
		Sleep(100);
		cmd = cmd_c+"11";
		ret = linecamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
		
		Sleep(100);
		cmd = cmd_c+"19";
		ret = linecamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
		
		Sleep(100);
		cmd = cmd_c+"11";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());		
		CFIFO::GetInstance()->Write(cstr.GetString());

		Sleep(100);
		cmd = cmd_c+"19";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());		
		CFIFO::GetInstance()->Write(cstr.GetString());
	}
}
void CWork::JsLightOC(bool isSetting)
{
	string cmd_o = "{jiecheks}";
	string cmd_c = "{jiechejs}";
	string cmd;
	CString cstr;
	int ret;
	Sleep(100);
	if(isSetting)
	{
		Sleep(100);
		cmd = cmd_o+"1";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
	
		Sleep(100);
		cmd = cmd_o+"2";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
	}
	else
	{
		Sleep(100);
		cmd = cmd_c+"1";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());

		Sleep(100);
		cmd = cmd_c+"2";
		ret = areacamera_cmd.Send((BYTE*)const_cast<char*>(cmd.c_str()), cmd.size());
		cstr.Format("ret = %d, cmd = %s\r\n", ret, cmd.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
	}
}
void CWork::AutoRun()
{
	
	string str0, str1, str2, str3, str4, str5, str6, str2ser; /* 库&股道&车型&车号&端位&主 */
	string devID; //设备ID
	string str;
	str0 = handpad_cmd.GetTrainStorage();	//车库
	str1 = handpad_cmd.GetTrainGudao();		//股道
	str2 = handpad_cmd.GetTrainMsship();	//主/从机
	devID = Ccfg::GetInstance()->GetDevID(); //设备ID
	DWORD ret = -1;
	CString cstr = "";				
	cstr.Format("动车开始作业，作业信息， 车库：%s, 股道：%s, 主/从：%s, 设备ID：%s\r\n", str0.c_str(), str1.c_str(), str2.c_str(), devID.c_str());
	CFIFO::GetInstance()->Write(cstr.GetString());							
	
	cstr.Format("%s\r\n", str2ser.c_str());
	CFIFO::GetInstance()->Write(cstr.GetString());

	ttype = handpad_cmd.GetTrainType();		     // 车型 380B || 380BL
	tch   = handpad_cmd.GetTrainNum();		     // 车号
	devID = Ccfg::GetInstance()->GetDevID(); // 设备ID

	if(handpad_cmd.IsTrainConn())
	{
		cstr = "当前车为重连车\r\n";
		CFIFO::GetInstance()->Write(cstr.GetString());
		// 重连车双臂要走15.5节车厢，单臂要走4.5节车厢
		// TODO 设定快扫路程长度
	}
	else
	{
		cstr = "当前车不是重连车, 准备启动作业\r\n";
		CFIFO::GetInstance()->Write(cstr.GetString());
		// 非重连车要走8节车厢
		// TODO 设定快扫路程长度
	}

#if 0	//打开车载设备
	if(!DeviceOn())
	{
		cstr = "车载设备开启失败!\r\n";
		CFIFO::GetInstance()->Write(cstr.GetString());
		return;
	}
#endif //打开车载设备

#if 0	//快扫流程
	do
	{
		long dist = Ccfg::GetInstance()->GetTrainRoutine();
		cstr.Format("{c,step,%ld,100}", dist);
		str = cstr.GetString();
		ResetEvent(plc_response_rv);
		plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
		ret = EventWait(plc_response_rv, 2000);
		CFIFO::GetInstance()->Write(str+"\r\n");
		Sleep(20);
	}while(ret != WAIT_OBJECT_0);
	
	ResetEvent(plc_response_ok);
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
	EventWait(plc_response_ok, INFINITE);
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
	/*
	step-2:
		快扫采集关闭
		关闭激光测距仪，通过串口发送“0x1B”命令
		关闭脉冲计数器，通过串口发送“{jiechejs}”命令
		小车轴定位，读取配置文件中相关车型的转向架“动/拖”信息，偏移量信息，示教点信息
		给服务器发送轴信息
	*/
	DeviceOff();
	
	//获取AB相脉冲计数值，并且在UI上显示
#endif //快扫流程
	vector<PAIR> veczhou1;
	//vector<long> veczhou1;
	long currpci =  0;
	int stplen = 0;
	long curroffset = 0 ;
	long currstp = 0;
	int idx = 0;
	int i , j;
	int zxjlen ;
	
	string traintype = handpad_cmd.GetTrainType();
	TrainBase* t =(TrainBase*)ClassFactory::getInstance().getClassByName(traintype); 
	//通过是否为重连车来调用不同的配置文件参数
	if(handpad_cmd.IsTrainConn())
		t->LoadCfg(traintype+"-Ext");
	else
		t->LoadCfg(traintype);
	TRAINMSG trmsg;
	memset(&trmsg, 0, sizeof(TRAINMSG));
	t->GetTrainInfo(trmsg);
	currZjj = trmsg.zjj;

	int testZxjNum = trmsg.zxjnum;		//转向架个数
	int testZhouNum = testZxjNum*2;		//转向架对应的轴数
#if 0	//轴数据处理
	_beginthread(dataprocess, 0, this); 
	EventWait(hHandle_DataProcess, INFINITE);
	UseMemMode();
#endif //轴数据处理
	int lenCalcZhou = vecReal.size();
	lenCalcZhou = 32;//TEST
	cstr.Format("配置文件:%s.ini中的测试转向架个数为: %d, 实际计算轴个数为: %d\r\n", traintype.c_str(), testZhouNum, lenCalcZhou);
	CFIFO::GetInstance()->Write(cstr.GetString());
	/*
	step-3:
		根据手持机发来的车型，车号信息读取相应的配置文件，通过配置文件来了解具体转向架型号，示教点数信息
	*/

	/*
		Module :Read Cfg file, and then parse the cfg string key
	*/
	
	int l1 = strcmp(currZjj.c_str(),"2.5");
	int l2 = strcmp(currZjj.c_str(),"2.7");
	//if(!strcmp(currZjj.c_str(),"2.5") || !strcmp(currZjj.c_str(),"2.7"))
	if(!l1 || !l2)
	{
		cstr.Format("当前车型转向架轴间距:%s \r\n", currZjj.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
	}
	else
	{
		cstr.Format("当前车型转向架轴间距:%s, 错误, 返回!!!\r\n", currZjj.c_str());
		CFIFO::GetInstance()->Write(cstr.GetString());
		return;
	}
#if 1
	//轴信息归并-> veczhou1用来定位用的轴
	veczhou1.clear();
	if(lenCalcZhou >= testZhouNum)
	{
		//计算得到的轴个数比要做测试的轴个数相等或多
		cstr.Format("vecReal.size() = %d, testZhouNum = %d\r\n", lenCalcZhou, testZhouNum);
		CFIFO::GetInstance()->Write(cstr.GetString());
#if 0
		for(i = 0 ; i<testZhouNum -1; i++)
		{
			if(i%2==0)
				veczhou1.push_back(vecReal.at(i));
		}
#endif
	}
	else
	{
		//计算得到的轴个数比要做测试的轴个数少
		cstr.Format("vecReal.size() = %d, testZhouNum = %d, 实际轴比测试轴少, 返回!!!\r\n", lenCalcZhou, testZhouNum);
		CFIFO::GetInstance()->Write(cstr.GetString());
		return ;
	}
	
	
	int cfgzxjlen = trmsg.zxjdata.size() ;
	string ttype = trmsg.traintype;
	cstr.Format("转向架个数：%d\t动车类型：%s\r\n", cfgzxjlen, ttype.c_str());
	CFIFO::GetInstance()->Write(cstr.GetString());

	
	/*
	1.	{c,type,traintype,zxjtype}
	2.	{c,sjt,x}
	3.	{c,step,x,100}
	4.	{c,mpos,n}
	5.	goto 1
	*/
	TRAINMSG testMsg;
	memset(&testMsg, 0, sizeof(TRAINMSG));
	testMsg.traintype = trmsg.traintype;
	testMsg.dzxjh = trmsg.dzxjh;
	testMsg.tzxjh = trmsg.tzxjh;
	testMsg.zjj   = trmsg.zjj;
	testMsg.zxjnum = trmsg.zxjnum;
	if(cfgzxjlen >= testZxjNum)
	{
		//配置文件中的转向架个数大于等于测试转向架个数
		testMsg.zxjdata.assign(trmsg.zxjdata.begin(),trmsg.zxjdata.begin() + testZxjNum);
		int lenTestMsg = testMsg.zxjdata.size();
		cstr.Format("lenTestMsg = %d\r\n", lenTestMsg);
		CFIFO::GetInstance()->Write(cstr.GetString());
	}
	else
	{
		cstr.Format("cfgzxjlen =%d, testZxjNum = %d, 配置的转向架个数少于测试转向架个数, 返回!!!\r\n", cfgzxjlen, testZxjNum);
		CFIFO::GetInstance()->Write(cstr.GetString());
	}

	//JsLightOC(true);
	int lenTestMsg = testMsg.zxjdata.size();
	if(lenTestMsg == testZxjNum)
	{
		
		for(/*auto*/ i = testZxjNum-1 ;i >= 0; i--)
		{
			/*int*/ idx = 16-1-i;
			if(zxjRun[idx] == 0)
			{
				cstr.Format("转向架编号:%d, continue\r\n", idx);
				CFIFO::GetInstance()->Write(cstr.GetString());
				continue;
			}
			string trainname  = testMsg.traintype;
			string zxjname = testMsg.zxjdata.at(i).zxjname;

			do{
				cstr.Format("{c,type,%s,%s}",trainname.c_str(), zxjname.c_str());
				str = cstr.GetString();
				plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
				ret = EventWait(plc_response_ok, 1000);
				CFIFO::GetInstance()->Write(str+"\r\n");
			}while(ret != WAIT_OBJECT_0);

			/*
				{c,sjt,x}
			*/
			/*long*/ currpci = veczhou1.at(i).second;
			float h = veczhou1.at(i).first;
			int sjh = 0;
			int ind = zxjname.find_first_of('D', 0);
			if(ind>= 0)
			{
				//sjh = h - t->dzxjh;
				sjh = h - testMsg.dzxjh;
				if(sjh <= 0)
					sjh = 0;
				cstr.Format("当前转向架:%s, 升降值:%d\r\n", zxjname.c_str(), sjh);
				CFIFO::GetInstance()->Write(cstr.GetString());
			}
			else
			{
				//sjh = h - t->tzxjh;
				sjh = h - testMsg.tzxjh;
				if(sjh <= 0)
					sjh = 0;
				cstr.Format("当前转向架:%s, 升降值:%d\r\n", zxjname.c_str(), sjh);
				CFIFO::GetInstance()->Write(cstr.GetString());
			}

			do{
				cstr.Format("{c,sjt,%d}",sjh);
				str = cstr.GetString();
				plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
				ret = EventWait(plc_response_ok, 1000);
				CFIFO::GetInstance()->Write(str+"\r\n");
			}while(ret != WAIT_OBJECT_0);

			/*int*/ stplen = testMsg.zxjdata.at(i).stpdata.size();
			for(/*auto*/ j = 0; j<stplen; j++)
			{
				/*long*/ curroffset = testMsg.zxjdata.at(i).stpdata.at(j).offset;
				/*long*/ currstp = currpci + curroffset;
				//cout<<"currstp"<<endl;

				do
				{
					cstr.Format("{c,step,%ld,100}",currstp);
					str = cstr.GetString();
					CFIFO::GetInstance()->Write(str+"wait rv\r\n");
					plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
					ret = EventWait(plc_response_rv, 1000);
					CFIFO::GetInstance()->Write(str+"wait rv ok\r\n");
					//Sleep(20);
				}while(ret != WAIT_OBJECT_0);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
				EventWait(plc_response_ok, INFINITE);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");

				do
				{
					cstr.Format("{c,mpos,%d}",j+1);
					str = cstr.GetString();
					CFIFO::GetInstance()->Write(str+"wait rv\r\n");
					plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
					ret = EventWait(plc_response_rv, 1000);
					CFIFO::GetInstance()->Write(str+"wait rv ok\r\n");
					//Sleep(20);
				}while(ret != WAIT_OBJECT_0);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
				EventWait(plc_response_ok, INFINITE);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
			}
		}
	}
	//}
#endif
	JsLightOC(false);
#if 0 //TEST USE
	//if(vecAxis.size() ==32)
	{
		for(int i = vecAxis.size()-1; i>= 0; i--)
		{
			do
			{
				//long  ldis = vecAxis.at(i).second;
				long  ldis = vecAxis.at(i);
				cstr.Format("{c,step,%ld,100}",ldis);
				str = cstr.GetString();
				CFIFO::GetInstance()->Write(str+"wait rv\r\n");
				plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
				ret = EventWait(plc_response_rv, 1000);
				CFIFO::GetInstance()->Write(str+"wait rv ok\r\n");
				Sleep(20);
			}while(ret != WAIT_OBJECT_0);
			CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
			EventWait(plc_response_ok, INFINITE);
			CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
			Sleep(5000);
		}
	}
#endif //TEST USE

#if 1
	GOHOME:
	do
	{
		str = "{c,home,100}";
		plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
		ret = EventWait(plc_response_rv, 300);
		CFIFO::GetInstance()->Write(str+"\r\n");
		Sleep(20);
	}while(ret != WAIT_OBJECT_0);
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
	EventWait(plc_response_ok, INFINITE);
	CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
#endif
	CFIFO::GetInstance()->Write("流程结束 ... ...\r\n");
}
void CWork::EstopAction()
{
	//紧急停止
}
void CWork::HomeAction()
{
	//回到原点
}
void CWork::GoAction()
{
	//前进动作
}
void CWork::BackAction()
{
	//后退动作
}

static inline int RecvStrParse(string& str, vector<PAIR>& out)
{
	/*
		errcnt:
		-1表示str长度不足
		0表示没有错误数
		大于1表示有错误计数
	*/
	int errcnt = 0;
	CString cstr;
	string substr;
	unsigned int ad, ad1,ad2;
	unsigned long pl1,pl2,pl3;
	ad1 = ad2 = 0;
	pl1 = pl2 = pl3 = 0;
	
	long strlen = str.size();
	cstr.Format("当前长度 strlen = %ld\r\n", strlen);
	CFIFO::GetInstance()->Write(cstr.GetString());
	if(strlen<8)
	{
		cstr.Format("strlen = %ld, 长度不足!\r\n", strlen);
		CFIFO::GetInstance()->Write(cstr.GetString());
		return -1;
	}
	for(long i = 0; i<strlen; i+=8)
	{
		substr = str.substr(i, 8);
		if(substr.at(0) == '{' && substr.at(7) == '}')
		{
			ad1 = unsigned char(substr.at(1))<<8;
			ad2 = unsigned char(substr.at(2));
			ad = ad1 + ad2;
			float  lvheight = ad*AD_XISHU;

			pl1 = unsigned char(substr.at(4))<<16;
			pl2 = unsigned char(substr.at(5))<<8;
			pl3 = unsigned char(substr.at(6));
			long pcilong = pl1 + pl2 + pl3;
			out.push_back(make_pair<float, long>(lvheight, pcilong));
		}
		else
			errcnt++;
	}
	return errcnt;
}

void CWork::dataprocess(LPVOID param)
{
	CWork* dlg = (CWork*)param;
	DWORD ret;
	CString cstr;
	string str;
	
	vector<AIXS_DETAIL> veclds1detail;
	vector<AIXS_DETAIL> veclds2detail;
	/*
		第二代库检机器人新增算法 2017-12-07
	*/
	vector<PAIR> lds1_dbrush; lds1_dbrush.clear();
	vector<PAIR> lds2_dbrush; lds2_dbrush.clear();
	//dlg->lds1_zhou.clear();
	try
	{
#if 1
		string mdis1str = dlg->mdis1->GetRecvString();
		long mdis1strlen = mdis1str.size();
		string mdis2str = dlg->mdis2->GetRecvString();
		long mdis2strlen = mdis2str.size();

		cstr.Format("dis1str len = %ld\r\n", mdis1strlen);
		CFIFO::GetInstance()->Write(cstr.GetString());
		cstr.Format("dis2str len = %ld\r\n", mdis2strlen);
		CFIFO::GetInstance()->Write(cstr.GetString());

		dlg->vecLds1.clear();
		int errt1 = RecvStrParse(mdis1str, dlg->vecLds1);
		cstr.Format("errt1 len = %ld\r\n", errt1);
		CFIFO::GetInstance()->Write(cstr.GetString());
		dlg->mdis1->ClearRecvString();

		dlg->vecLds2.clear();
		int errt2 = RecvStrParse(mdis2str, dlg->vecLds2);
		cstr.Format("errt2 len = %ld\r\n", errt2);
		CFIFO::GetInstance()->Write(cstr.GetString());
		dlg->mdis1->ClearRecvString();

		LogVec(&dlg->vecLds1, "mdis1");
		cstr.Format("vecLds1 len = %d\n", dlg->vecLds1.size());
		CFIFO::GetInstance()->Write(cstr.GetString());
		CFIFO::GetInstance()->Write("mdis1 log finished ... ...\r\n");

		LogVec(&dlg->vecLds2, "mdis2");
		cstr.Format("vecLds2 len = %d\n", dlg->vecLds2.size());
		CFIFO::GetInstance()->Write(cstr.GetString());
		CFIFO::GetInstance()->Write("mdis2 log finished ... ...\r\n");
		
		vector<PAIR> canwu;
		canwu.clear();
		FindAixeCandiate_wu(dlg->vecLds2, canwu);
		LogVec(&canwu, "canwu");
		CFIFO::GetInstance()->Write("canwu log finished ... ...\r\n");
#endif
		//vector<PAIR> canwu;/////
		vector<PAIR> canzhou;
		canzhou.clear();
		if(dlg->currZjj == "2.7")
		{
			CFIFO::GetInstance()->Write("currzxj zjj = 2.7 ...\r\n");
			FindFZ54000_wu(canwu, canzhou);
		}
		if(dlg->currZjj == "2.5")
		{
			CFIFO::GetInstance()->Write("currzxj zjj = 2.5 ...\r\n");
			FindFZ50000_wu(canwu, canzhou);
		}
		LogVec(&canzhou,"canzhou");
		CFIFO::GetInstance()->Write("canzhou log finished ... ...\r\n");
		/*dlg->vecAxis.clear();
		for(auto it = canzhou.begin(); it != canzhou.end(); it++)
			dlg->vecAxis.push_back(it->second);*/
		dlg->vecReal.clear();
		dlg->vecReal.assign(canzhou.begin(), canzhou.end());
	}
	catch(...)
	{
		ret = GetLastError();
		cstr.Format("process procedular crashed!, errcode = %d, please check!", ret);
		CFIFO::GetInstance()->Write(cstr.GetString());
	}
END:
	SetEvent(dlg->hHandle_DataProcess);
}

DWORD CWork::EventWait(HANDLE hHandle, DWORD milsec)
{
	DWORD ret = WaitForSingleObject(hHandle, milsec);
	ResetEvent(hHandle);
	return ret;
}

//使用记忆模式
void CWork::UseMemMode()
{
	CString cstr;
	vecAxis.clear();
	//	是否使用记忆模式
	if(Ccfg::GetInstance()->GetMemeoryMode())
	{
		//CConsole::GetInstance()->Output("Run Mem-Model here ...\r\n");
		cstr = "Run Mem-Model here ...\r\n";
		CFIFO::GetInstance()->Write(cstr.GetString());
		string CurrDate = GetDateString();
		//获取配置文件
		BOOL ret = FALSE;
		char szPath[MAX_PATH];
		::GetModuleFileName(NULL, szPath, MAX_PATH);
		CString strFileName = szPath;
		strFileName.Delete(strFileName.ReverseFind('\\')+1, strFileName.GetLength ()-strFileName.ReverseFind('\\')-1);
		strFileName += "axieconfig.ini";

		//CConsole::GetInstance()->Output("Mem Mode cfg = %s ...\r\n", strFileName.GetString());
		cstr.Format("Mem Mode cfg = %s ...\r\n", strFileName.GetString());
		CFIFO::GetInstance()->Write(cstr.GetString());

		string strCX, strCH, strDate, strCTPCI, strCWPCI, strZhou;
		long chetouPCI, cheweiPCI;
		CString cstr;
		const int MAX_BUFF_LEN = 1024;
		char buff[MAX_BUFF_LEN]= {0};
		//step-1:read
		GetPrivateProfileString("MEM-TEST", "CX", "",buff, MAX_BUFF_LEN, strFileName.GetString());
		strCX = buff;
		GetPrivateProfileString("MEM-TEST", "CH", "",buff, MAX_BUFF_LEN, strFileName.GetString());
		strCH = buff;
		GetPrivateProfileString("MEM-TEST", "DATE", "", buff, MAX_BUFF_LEN, strFileName.GetString());
		strDate = buff;
		GetPrivateProfileString("MEM-TEST", "CTPCI", "", buff, MAX_BUFF_LEN, strFileName.GetString());
		strCTPCI = buff;
		chetouPCI = atol(strCTPCI.c_str());

		GetPrivateProfileString("MEM-TEST", "CWPCI", "", buff, MAX_BUFF_LEN, strFileName.GetString());
		strCWPCI = buff;
		cheweiPCI = atol(strCWPCI.c_str());

		GetPrivateProfileString("MEM-TEST", "Zhou", "", buff, MAX_BUFF_LEN, strFileName.GetString());
		strZhou = buff;
		long diff = abs(chetouPCI - HeadPCI);
		//CConsole::GetInstance()->Output("strCTPCI = %s, strCWPCI = %s\r\n", strCTPCI.c_str(), strCWPCI.c_str());
		if(strCX == ttype && strCH == tch && strDate == CurrDate /*&& diff< 1000*/)
		{
			//CConsole::GetInstance()->Output("Mem-Mode match ok ...\r\n");
			CFIFO::GetInstance()->Write("Mem-Mode match ok ...\r\n");
			//读取轴信息匹配成功，然后用配置文件中的轴覆盖当前计算的轴信息
			HeadPCI = chetouPCI;
			TailPCI = cheweiPCI;
			vecAxis.clear();
			for(int i = 0; i<32; ++i)
			{
				int ind = strZhou.find('&', 0);
				string sub = strZhou.substr(0, ind);
				long pv = atol(sub.c_str());
				//vecAxis.push_back(pv);
				strZhou.erase(0, ind+1);
			}
		}
		else
		{
			//CConsole::GetInstance()->Output("Mem-Mode match ng ...");
			CFIFO::GetInstance()->Write("Mem-Mode match ng ...\r\n");
			//把老的配置文件做备份
			int len = vecAxis.size();
			//	rename config file and backup
			SYSTEMTIME systime;
			GetLocalTime(&systime);
			CString timeflag, newName;
			timeflag.Format("%4d%02d%02d%02d%02d%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
			newName = strFileName + timeflag;
			if(rename(strFileName.GetString(),newName.GetString())==0)
			{
				//CConsole::GetInstance()->Output("rename cfg-file success \r\n");
				CFIFO::GetInstance()->Write("rename cfg-file success \r\n");
			}
			else
			{
				//CConsole::GetInstance()->Output("rename cfg-file failed \r\n");
			}
			if(len == 32)
			{
				//CConsole::GetInstance()->Output("Mem-Mode write info ...\r\n");
				CFIFO::GetInstance()->Write("Mem-Mode write info ...\r\n");
				//把当前的轴信息写入到配置文件中
				ret = WritePrivateProfileString("MEM-TEST", "CX", ttype.c_str(), strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CX write result = %d\r\n", ret);
				
				ret = WritePrivateProfileString("MEM-TEST", "CH", tch.c_str(), strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CH write result = %d\r\n", ret);
				
				ret = WritePrivateProfileString("MEM-TEST", "DATE", CurrDate.c_str(), strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = DATE write result = %d\r\n", ret);
				
				cstr.Format("%ld", HeadPCI);
				ret = WritePrivateProfileString("MEM-TEST", "CTPCI",cstr, strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CTPCI write result = %d\r\n", ret);
				
				cstr.Format("%ld", TailPCI);
				ret = WritePrivateProfileString("MEM-TEST", "CWPCI", cstr, strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CWPCI write result = %d\r\n", ret);
				
				cstr.Empty();
				for(int i = 0; i< len; ++i)
				{
					long pv = vecAxis.at(i);
					cstr.AppendFormat("%ld&", pv);
				}
				ret = WritePrivateProfileString("MEM-TEST", "Zhou", cstr, strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = Zhou write result = %d\r\n", ret);
			}
			else
			{
				//老的配置文件作备份
				//CConsole::GetInstance()->Output("Mem-Mode clear info ...\r\n");
				CFIFO::GetInstance()->Write("Mem-Mode clear info ...\r\n");
				//清空配置文件中的相关项
				ret = WritePrivateProfileString("MEM-TEST", "CX", "", strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CX write result = %d\r\n", ret);
				ret = WritePrivateProfileString("MEM-TEST", "CH", "", strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CH write result = %d\r\n", ret);
				ret = WritePrivateProfileString("MEM-TEST", "DATE", "", strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = DATE write result = %d\r\n", ret);
				ret = WritePrivateProfileString("MEM-TEST", "CTPCI", "", strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CTPCI write result = %d\r\n", ret);
				ret = WritePrivateProfileString("MEM-TEST", "CWPCI", "", strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = CWPCI write result = %d\r\n", ret);
				ret = WritePrivateProfileString("MEM-TEST", "Zhou", "", strFileName.GetString());
				TRACE("appname = MEM-TEST, keyname = Zhou write result = %d\r\n", ret);
			}
		}
	}
}

//void CWork::DeviceOn()
int CWork::DeviceOn()
{
	old_pulse = 0;
	curr_pulse = 0;
	KsLightOC(true);
	string cmd = "{jiecheks}";
	CString cstr;
	int ret = 0;
	for(int i = 0; i<5; i++)
	{
		string xike1ip = Ccfg::GetInstance()->Getxike1ip();
		int    xike1port = Ccfg::GetInstance()->Getxike1pport();
		mdis1->Close();
		ret = mdis1->Connection(xike1ip.c_str(), xike1port/*,1000*/);
		//Sleep(20);
		if(ret)
		{
			cstr = "mdis1->Connect(), ret = 1\r\n";
			CFIFO::GetInstance()->Write(cstr.GetString());
			break;
		}
		else
		{
			
			cstr.Format("mdis1->Connect(), ret = 0, error code = %d\r\n", mdis1->GetError());
			CFIFO::GetInstance()->Write(cstr.GetString());
		}
	}
	if(ret)
	{
		CFIFO::GetInstance()->Write("mdis1连接服务端成功!\r\n");
		mdis1->SendData(const_cast<char*>(cmd.c_str()), cmd.size(), 200);
	}
	else
	{
		CFIFO::GetInstance()->Write("mdis1连接服务端失败!\r\n");
		return 0;
	}
	
	for(int i = 0; i<5; i++)
	{
		string xike2ip = Ccfg::GetInstance()->Getxike2ip();
		int    xike2port = Ccfg::GetInstance()->Getxike2pport();
		mdis2->Close();
		ret = mdis2->Connection(xike2ip.c_str(), xike2port/*, 1000*/);
		//Sleep(20);
		if(ret)
		{
			CFIFO::GetInstance()->Write("mdis2->Connect(), ret = 1\r\n");
			break;
		}
		else
		{
			CFIFO::GetInstance()->Write("mdis2->Connect(), ret = 0\r\n");
		}
	}
	if(ret)
	{
		CFIFO::GetInstance()->Write("mdis2连接服务端成功!\r\n");
		mdis2->SendData(const_cast<char*>(cmd.c_str()), cmd.size(), 200);
	}
	else
	{
		CFIFO::GetInstance()->Write("mdis2连接服务端失败!\r\n");
		return 0;
	}

	return 1;
}

//void CWork::DeviceOff()
int CWork::DeviceOff()
{
	
	KsLightOC(false);

	string cmd = "{jiechejs}";
	
	mdis1->SendData(const_cast<char*>(cmd.c_str()), cmd.size(), 200);
	mdis2->SendData(const_cast<char*>(cmd.c_str()), cmd.size(), 200);
	Sleep(1000);
	mdis1->Close();
	mdis2->Close();
	return 1;
}

void CWork::InitVec()
{
	old_pulse = 0;
	curr_pulse = 0;
	vecAxis.clear();
	vecLds1Str.clear();
	vecLds2Str.clear();
	vecLds1.clear();
	vecLds2.clear();
	vecdpj.clear();
	mdis1->MdisVecClear();
	mdis2->MdisVecClear();
	mdis1->ClearRecvString();
	mdis2->ClearRecvString();
	lds1_zhou.clear();
}

void CWork::STPosAction()
{
	CFIFO::GetInstance()->Write("小车原地动作... ...\r\n");
	JsLightOC(true);
	
	string traintype/* = handpad_cmd.GetTrainType()*/;
	//traintype = "CRH380BL";
	traintype = Ccfg::GetInstance()->GetDemoAction();
	TrainBase* t =(TrainBase*)ClassFactory::getInstance().getClassByName(traintype); 
	t->LoadCfg(traintype);
	TRAINMSG trmsg;
	t->GetTrainInfo(trmsg);
	CString cstr;
	string str;
	cstr.Format("车型: %s", traintype.c_str());
	str = cstr.GetString();
	CFIFO::GetInstance()->Write(str+"\r\n");
	Sleep(5000);
	int ret = 0;
	int zxjlen = 16;
	if(zxjlen == 16)
	{
		for(int i = zxjlen-1 ;i >= 0; i--)
		{
			int idx = zxjlen-1-i;
			if(zxjRun[idx] == 0)
				continue;
			string trainname  = trmsg.traintype;
			string zxjname = trmsg.zxjdata.at(i).zxjname;

			do{
				cstr.Format("{c,type,%s,%s}",trainname.c_str(), zxjname.c_str());
				str = cstr.GetString();
				plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
				ret = EventWait(plc_response_ok, 1000);
				CFIFO::GetInstance()->Write(str+"\r\n");
			}while(ret != WAIT_OBJECT_0);

			do{
				int sj = 30;
				cstr.Format("{c,sjt,%d}",sj);
				str = cstr.GetString();
				plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
				ret = EventWait(plc_response_ok, 1000);
				
				cstr.Format("EventWait ret = %d, %s\r\n", ret, str.c_str());
				CFIFO::GetInstance()->Write(cstr.GetString());
				//Sleep(200);
			}while(ret != WAIT_OBJECT_0);

			/*
				{c,sjt,x}
			*/
			//long currpci = veczhou1.at(i);
			int stplen = trmsg.zxjdata.at(i).stpdata.size();
			for(int j = 0; j<stplen; j++)
			{
				//long curroffset = trmsg.zxjdata.at(i).stpdata.at(j).offset;
				//long currstp = currpci + curroffset;
				//cout<<"currstp"<<endl;
				/*
				long currstp = 0;
				do
				{
					currstp = 0;
					cstr.Format("{c,step,%ld,100}",currstp);
					str = cstr.GetString();
					CFIFO::GetInstance()->Write(str+"wait rv\r\n");
					plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
					ret = EventWait(plc_response_rv, 1000);
					CFIFO::GetInstance()->Write(str+"wait rv ok\r\n");
					//Sleep(20);
				}while(ret != WAIT_OBJECT_0);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
				EventWait(plc_response_ok, INFINITE);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
				*/
				do
				{
					cstr.Format("{c,mpos,%d}",j+1);
					str = cstr.GetString();
					CFIFO::GetInstance()->Write(str+"wait rv\r\n");
					plc_sock->Send(const_cast<char*>(str.c_str()), str.size());
					ret = EventWait(plc_response_rv, 1000);
					CFIFO::GetInstance()->Write(str+"wait rv ok\r\n");
					//Sleep(20);
				}while(ret != WAIT_OBJECT_0);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD\r\n");
				EventWait(plc_response_ok, INFINITE);
				CFIFO::GetInstance()->Write("Wait PLC plc_response_ok CMD, Success\r\n");
			}
		}
	}

	JsLightOC(false);
}