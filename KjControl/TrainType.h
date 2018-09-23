/*
	TrainType.h 
*/
#pragma once
#include <iostream>
#include <string>
#include "TrainInfo.h"
#include "FIFO.h"
using namespace std;
class TrainBase
{
public:
	virtual void LoadCfg(string name);
	virtual void GetTrainInfo(TRAINMSG&);
	//int GetDzxjh() {return dzxjh;}
	//int GetTzxjh() {return tzxjh;}
private:
	string traintype;	// 车型
	string zxjsel;		// 转向架序列
	string zxjinfo;		// 转向架偏移量示教点信息
	TRAINMSG trainmsg;	// 解析后的转向架偏移量示教点信息i

	int dzxjh;		//	动车转向架基础高度
	int tzxjh;		//	拖车转向架基础高度
	string zjj;		//	轴间距

	int zxjnum;		//  要运行的转向架个数
};

class TestDemo: public TrainBase
{
public:
	TestDemo(){CFIFO::GetInstance()->Write("TestDemo Struct running\r\n");}
};

class CRH380B: public TrainBase
{
public:
	CRH380B(){CFIFO::GetInstance()->Write("CRH380B Struct running\r\n");}
};

class CRH380BL: public TrainBase
{
public:
	CRH380BL(){CFIFO::GetInstance()->Write("CRH380BL Struct running\r\n");}
};

class CR400BF: public TrainBase
{
public:
	CR400BF(){CFIFO::GetInstance()->Write("CR400BF Struct running\r\n");}
};
///成都东动车所-成贵线 20171213
class CRH1A: public TrainBase
{
public:
	CRH1A(){CFIFO::GetInstance()->Write("CRH1A Struct running\r\n");}
};

class CRH2A: public TrainBase
{
public:
	CRH2A(){CFIFO::GetInstance()->Write("CRH2A Struct running\r\n");}
};

class CRH3A: public TrainBase
{
public:
	CRH3A(){CFIFO::GetInstance()->Write("CRH3A Struct running\r\n");}
};

class CRH380A: public TrainBase
{
public:
	CRH380A(){CFIFO::GetInstance()->Write("CRH380A Struct running\r\n");}
};

class CRH380D: public TrainBase
{
public:
	CRH380D(){CFIFO::GetInstance()->Write("CRH380D Struct running\r\n");}
};