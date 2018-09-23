#pragma once

#include "Thread.h"
#include "SerialPort.h"
#include "Udp.h"
#include "UdpSimple.h"
#include "UdpBroadcast.h"
#include "UdpExtend.h"
#include "MOXASerialport.h"
#include "Algo.h"
#include "AutocarDis.h"
//西科测距仪
#include "TCPClientImpl.h"

#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

typedef void (__stdcall	*DataShowCallBack)(void* context, long v1, long v2);

class CWork :public CThread
{
public:
	CWork(void);
	virtual ~CWork(void);
	virtual int Run();
	int Initialize();
	
	UdpSimple&	GetLineCameraCmd() {return linecamera_cmd;};
	UdpSimple&  GetAreaCameraCmd() {return areacamera_cmd;};
	
	CUdp& GetHandPadCmd() {return handpad_cmd;};
	
	string PlcSock(string str);
	string Ctrl2serv(string str);

	bool isRunningMode();
	int zxjRun[16];
	void KsLightOC(bool);	//快扫开关灯
	void JsLightOC(bool);	//精扫开关灯

	void AutoRun();		
	void EstopAction();
	void HomeAction();
	void GoAction();
	void BackAction();

	inline int DeviceOn();			//车载设备的开启	
	inline int DeviceOff();		//车载设备的关闭	
	static void dataprocess(LPVOID param); //轮轴数据日志记录

	DWORD  EventWait(HANDLE hHandle, DWORD milsec);
	void UseMemMode();

	//vector<PAIR> vecAxis;	 //轮轴中心，按测量结果入vector 	
	vector<long> vecAxis;	 //轮轴中心，按测量结果入vector 	
	vector<PAIR> vecReal;	 //
	HANDLE& GetPLCResponseRV(){return plc_response_rv;}
	HANDLE& GetPLCResponseOK(){return plc_response_ok;}

	void InitVec();
	
	shared_ptr<CUdp2Extend> GetPLCSock()   {return plc_sock;};
	shared_ptr<CUdp2Extend> GetCtrl2Serv() {return ctl2serv;};
	shared_ptr<CUdp2Extend> GetPLCSockDis(){return plc_sock_dis;};
	shared_ptr<CUdp2Extend> GetRecvAutoCarDis() {return recv_auto_dis;};

	static void RSAutocarDisLoop(LPVOID param);	//接收发送小车距离值
	static void LookupPLCDisLoop(LPVOID param);

	shared_ptr<AutocarDis> GetDisSelf() {return fDis_Self;};
	shared_ptr<AutocarDis> GetDisOther() {return fDis_Other;};

	void PlcSockDis(string str);
	void RecvAutoDis(string str);
	static void CollisionAnlysis(LPVOID param);
	shared_ptr<TCPClientImpl> GetMdis1() {return mdis1;}
	shared_ptr<TCPClientImpl> GetMdis2() {return mdis2;}

	void STPosAction();
private:
	//发过去就可以了，不需要应答
	UdpSimple	linecamera_cmd;	//线阵采集命令
	UdpSimple	areacamera_cmd;	//面阵采集命令
	
	//手持机对控制
	CUdp	handpad_cmd;
	
	//发送UDP命令，然后要判断是否有应答
	shared_ptr<CUdp2Extend> plc_sock;		//2001 一般命令交互
	shared_ptr<CUdp2Extend> plc_sock_dis;	//2002 向PLC查询小车当前位置信息
	
	shared_ptr<CUdp2Extend> ctl2serv;		//6000 和服务器程序交互
	shared_ptr<CUdp2Extend> recv_auto_dis;	//		接收另外一台小车距离值
	float lds30_dis1;	//LDS30设备1的距离值
	float lds30_dis2;	//LDS30设备2的距离值

	long curr_pulse;	//单片机当前脉冲	
	long old_pulse;		//单片机前一个数据
	//系统时间统计
	LARGE_INTEGER litmp_s1; 
	LONGLONG QPart1_s1,QPart2_s1;
	double dfMinus_s1, dfFreq_s1, dfTim_s1; 

	LARGE_INTEGER litmp_s2; 
	LONGLONG QPart1_s2,QPart2_s2;
	double dfMinus_s2, dfFreq_s2, dfTim_s2;

	HANDLE	plc_response_rv;
	HANDLE	plc_response_ok;
	HANDLE  hHandle_DataProcess;
	HANDLE  dpj_pulse_ok;
	long HeadPCI;		//车头脉冲值
	long TailPCI;		//车尾脉冲值
	long trainpulse;	//
	float lds1value;	//LDS1的值
	float lds2value;	//LDS2的值
	string ttype; //车型
	string tch;	  //车号
	vector<OR_PAIR> vecLds1Str;
	vector<OR_PAIR> vecLds2Str;
	vector<PAIR> vecLds1;
	vector<PAIR> vecLds2;
	vector<string> vecdpj;

	bool isExit_RSAutocarDisLoop;
	bool isExit_LookupPLCDisLoop;

	shared_ptr<AutocarDis> fDis_Self;
	shared_ptr<AutocarDis> fDis_Other;

	bool isExit_CollisionAnlysis;

	long lplcpulse;
	float fplcdis;
	vector<PAIR> lds1_zhou;
	shared_ptr<TCPClientImpl> mdis1;
	shared_ptr<TCPClientImpl> mdis2;
	string currZjj;  //轴间距
};
