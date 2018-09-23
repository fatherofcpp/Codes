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
//���Ʋ����
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
	void KsLightOC(bool);	//��ɨ���ص�
	void JsLightOC(bool);	//��ɨ���ص�

	void AutoRun();		
	void EstopAction();
	void HomeAction();
	void GoAction();
	void BackAction();

	inline int DeviceOn();			//�����豸�Ŀ���	
	inline int DeviceOff();		//�����豸�Ĺر�	
	static void dataprocess(LPVOID param); //����������־��¼

	DWORD  EventWait(HANDLE hHandle, DWORD milsec);
	void UseMemMode();

	//vector<PAIR> vecAxis;	 //�������ģ������������vector 	
	vector<long> vecAxis;	 //�������ģ������������vector 	
	vector<PAIR> vecReal;	 //
	HANDLE& GetPLCResponseRV(){return plc_response_rv;}
	HANDLE& GetPLCResponseOK(){return plc_response_ok;}

	void InitVec();
	
	shared_ptr<CUdp2Extend> GetPLCSock()   {return plc_sock;};
	shared_ptr<CUdp2Extend> GetCtrl2Serv() {return ctl2serv;};
	shared_ptr<CUdp2Extend> GetPLCSockDis(){return plc_sock_dis;};
	shared_ptr<CUdp2Extend> GetRecvAutoCarDis() {return recv_auto_dis;};

	static void RSAutocarDisLoop(LPVOID param);	//���շ���С������ֵ
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
	//����ȥ�Ϳ����ˣ�����ҪӦ��
	UdpSimple	linecamera_cmd;	//����ɼ�����
	UdpSimple	areacamera_cmd;	//����ɼ�����
	
	//�ֳֻ��Կ���
	CUdp	handpad_cmd;
	
	//����UDP���Ȼ��Ҫ�ж��Ƿ���Ӧ��
	shared_ptr<CUdp2Extend> plc_sock;		//2001 һ�������
	shared_ptr<CUdp2Extend> plc_sock_dis;	//2002 ��PLC��ѯС����ǰλ����Ϣ
	
	shared_ptr<CUdp2Extend> ctl2serv;		//6000 �ͷ��������򽻻�
	shared_ptr<CUdp2Extend> recv_auto_dis;	//		��������һ̨С������ֵ
	float lds30_dis1;	//LDS30�豸1�ľ���ֵ
	float lds30_dis2;	//LDS30�豸2�ľ���ֵ

	long curr_pulse;	//��Ƭ����ǰ����	
	long old_pulse;		//��Ƭ��ǰһ������
	//ϵͳʱ��ͳ��
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
	long HeadPCI;		//��ͷ����ֵ
	long TailPCI;		//��β����ֵ
	long trainpulse;	//
	float lds1value;	//LDS1��ֵ
	float lds2value;	//LDS2��ֵ
	string ttype; //����
	string tch;	  //����
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
	string currZjj;  //����
};
