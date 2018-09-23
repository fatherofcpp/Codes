/*
	LDS30激光测距仪-串口命令封装
	说明：把LDS30相应命令转向成字符串
*/

#pragma once
#include "stdafx.h"
#include <string>
using namespace std;


enum LDSCommand
{
	DM,		//single distance measurement
	DT,		//continuous distance measurement
	DF,		//fast continuous distance measurement
	TP,		//Display internal temperature [°C]
	HW, 	//Display hardware status
	PA,  	//Display parameter 
	PR,	  	//reset parameter
	DR,		//reset device
	AS,		//display autostart command
	ASs,	//set autostart command
	MF,		//display measure frequency
	MFx,	//set measure frequency
	GN,		//display receiver gain
	GNx,	//set receiver gain	
	SA,		//display average value
	SAx,	//set average value
	MW,		//display measure window
	MWxyz,	//set measure window
	OF,		//display distance offset
	OFx,	//set distance offset
	SO,		//set current distance to offset
	SE,		//display error mode
	SEx,	//set error mode	
	Q1,		//display digital out Q1
	Q1wxyz,	//set digital out Q1
	Q2,		//display digital out Q2
	Q2wxyz,	//set digital out Q2
	QA,		//display analog out QA
	QAxy,	//set analog out QA	
	BR,		//display baud rate RS422
	BRx,	//set baud rate RS422	
	SD,		//display output format RS422
	SDxy,	//set output format RS422	
	UB,		//display unit for binary output [in mm]
	UBx,	//set unit for binary output [in mm]
	TE,		//display terminator RS422
	TEx,	//set terminator RS422
	ST		//stop output	
};

class CLDSCommand
{
public:
	static CLDSCommand *GetInstance();
	string GetCommand(LDSCommand ldsCommand);
	string GetCommand(LDSCommand ldsCommand, const char* format, ...);
private:	
	CLDSCommand(void){};
	~CLDSCommand(){};
	string strbuf;
	static CLDSCommand* m_instance;
};
