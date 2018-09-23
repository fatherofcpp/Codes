/*
	读配置文件
*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Noncopyable.h"
using namespace std;


class Ccfg: 
	public noncopyable
{
protected:
	
	//static Ccfg *m_Instance;
	static shared_ptr<Ccfg> m_Instance;
public:
	Ccfg(void);
	virtual ~Ccfg(void);
	static shared_ptr<Ccfg> GetInstance();
	int Load(string filepath);
	int Save();
	bool CreateCfg(string filepath);
		
	//配置
	// 获取系统运行模式
	string GetDevID(){return deviceID;} //ADD 2017/02/16
	int GetRunningMode() {return running_model;}
	int GetMemeoryMode() {return memory_model;}	//ADD 2017/04/11
	long GetTrainRoutine() {return trainRoutine;}
	string  GetTrainLW()		{return trainlw;}

	string GetLDS1Com() {return lds1com;}
	int GetLDS1BD() {return lds1bd;}

	string GetLDS2Com() {return lds2com;}
	int GetLDS2BD() {return lds2bd;}

	string GetDpjCmdCom() {return dpjcmdcom;}
	int GetDpjCmdBD() {return dpjcmdbd;}

	string GetDpjDataCom() {return dpjdatacom;}
	int GetDpjDataBD() {return dpjdatabd;}

	int GetHandpadPort() {return handpad_port;}
	string GetServerIP() {return serverip;}
	int GetServerPort()	 {return serverport;}
	int GetServerDisPort() {return serverdisport;}

	string GetLinecameraIP() {return linecameraip;}
	int GetLinecameraPort()  {return linecameraport;}

	string GetAreacameraIP() {return areacameraip;}
	int GetAreacameraPort()  {return areacameraport;}

	string GetPlcIP()        {return plcip;}
	int GetPlcPort()		 {return plcport;}	
/*
	库检二代机器人新增配置 2017-12-07
*/
	
	int GetTrainLW1pci()	{return trainlw1pci;}
	int GetTrainLW2pci()	{return trainlw2pci;}
	int GetTrHeadSearchWid(){return trainheadsearchwidth;}
/*
	库检二代机器人新增配置 2017-12-07
*/
	string Getxike1ip()		{return xike1ip;}
	int    Getxike1pport()  {return xike1port;}
	string Getctrl1ip()		{return ctr1ip;}
	int	   Getctrl1port()	{return ctr1port;}

	string Getxike2ip()		{return xike2ip;}
	int    Getxike2pport()  {return xike2port;}
	string Getctrl2ip()		{return ctr2ip;}
	int	   Getctrl2port()	{return ctr2port;}

	string GetDemoAction() {return demoaction;}
protected:	
	string deviceID;			//设备运用ID
	int running_model;			//系统运行模式：0 手动， 1 自动
	int memory_model;			//轴定位记忆模式
	long trainRoutine;			//小车行驶里程
	string trainlw;				//动车列位

	string lds1com;				//LDS1 COM
	int lds1bd;					//LDS1 BD

	string lds2com;				//LDS2 COM
	int lds2bd;					//LDS2 BD

	string dpjcmdcom;			//单片机命令口
	int dpjcmdbd;				//单片机命令BD

	string dpjdatacom;			//单片机数据口
	int dpjdatabd;				//单片机数据BD

	int handpad_port;			//手持机广播端口 
	
	string serverip;			//服务器IP
	int serverport;				//服务器端口
	int serverdisport;			//发服务器距离数据端口

	string linecameraip;		//线阵IP
	int linecameraport;			//线阵端口

	string areacameraip;		//面阵IP
	int areacameraport;			//面阵端口
	
	string plcip;				//PLC IP
	int plcport;				//PLC端口
	/*
		库检二代机器人新增配置 2017-12-07
	*/
	
	int trainlw1pci;			//动车1列位PCI
	int trainlw2pci;			//动车2列位PCI
	int trainheadsearchwidth;	//动车车头搜索脉冲宽度
	/*
		库检二代机器人新增配置 2017-12-07
	*/
	string xike1ip;				//西科1
	int xike1port;				//
	string ctr1ip;				//
	int ctr1port;				//

	string xike2ip;				//
	int xike2port;				//	
	string ctr2ip;				//
	int ctr2port;				//

	string demoaction;			//演示用的动车型号
};

