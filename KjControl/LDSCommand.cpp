#include "stdafx.h"
#include "LDSCommand.h"
//#include <stdio.h>
using namespace std;
CLDSCommand* CLDSCommand::m_instance = NULL;

CLDSCommand *CLDSCommand::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new CLDSCommand();
	return m_instance;
} 

string CLDSCommand::GetCommand(LDSCommand ldsCommand)
{	
	char xcode = 0x1b;
	char xcode1 = 0x0d;
	char xcode2 = 0x0a;
	strbuf.clear();
	switch(ldsCommand)
	{
	case DM:
		strbuf = "DM\r\n";
		break;
	case DT:
		strbuf = "DT\r\n";
		break;
	case DF:
		strbuf = "DF\r\n";
		break;			
	case TP:
		strbuf = "TP\r\n";
		break;	
	case HW: 	
		strbuf = "HW\r\n";
		break;
	case PA:  	
		strbuf = "PA\r\n";
		break;
	case PR:	  	
		strbuf = "PR\r\n";
		break;
	case DR:		
		strbuf = "DR\r\n";
		break;
	case AS:		
		strbuf = "AS\r\n";
		break;
	case ASs:	
		break;
	case MF:		
		strbuf = "MF\r\n";
		break;
	case MFx:	
		break;
	case GN:		
		strbuf = "GN\r\n";
		break;
	case SA:		
		strbuf = "SA\r\n";
		break;
	case MW:		
		strbuf = "MW\r\n";
		break;
	case OF:		
		strbuf = "OF\r\n";
		break;
	case SO:		
		strbuf = "SO\r\n";
		break;
	case SE:		
		strbuf = "SE\r\n";
		break;
	case Q1:		
		strbuf = "Q1\r\n";
		break;
	case Q2:	
		strbuf = "Q2\r\n";
		break;
	case QA:
		strbuf = "QA\r\n";
		break;		
	case BR:	
		strbuf = "BR\r\n";
		break;	
	case SD:	
		strbuf = "SD\r\n";
		break;
	case UB:		
		strbuf = "UB\r\n";
		break;
	case TE:	
		strbuf = "TE\r\n";
		break;	
	case ST:
		strbuf.append(1, xcode);
		strbuf.append(1, xcode1);
		strbuf.append(1, xcode2);
		break;	
	}
	
	return strbuf;
}

string CLDSCommand::GetCommand(LDSCommand ldsCommand, const char* format, ...)
{
	char fmt[512] = {0};	
	char out[1024] = {0};
	strbuf.clear();
	
	int t = 0;
	va_list ap;	
	switch(ldsCommand)
	{
	case ASs:	//set autostart command
		sprintf(fmt, "AS ");
		break;
	case MFx:	//set measure frequency
		sprintf(fmt, "MF ");
		break;
	case GNx:	//set receiver gain	
		sprintf(fmt, "GN ");
		break;
	case SAx:	//set average value
		sprintf(fmt, "SA ");
		break;
	case MWxyz:	//set measure window
		sprintf(fmt, "MW ");
		break;
	case OFx:	//set distance offset
		sprintf(fmt, "OF ");
		break;
	case SEx:	//set error mode	
		sprintf(fmt, "SE ");
		break;
	case Q1wxyz://set digital out Q1
		sprintf(fmt, "Q1 ");
		break;
	case Q2wxyz://set digital out Q2
		sprintf(fmt, "Q2 ");
		break;
	case QAxy:	//set analog out QA	
		sprintf(fmt, "QA ");
		break;
	case BRx:	//set baud rate RS422
		sprintf(fmt, "BR ");
		break;
	case SDxy:	//set output format RS422	
		sprintf(fmt, "SD ");
		break;
	case UBx:	//set unit for binary output [in mm]
		sprintf(fmt, "UB ");
		break;
	case TEx:	//set terminator RS422
		sprintf(fmt, "TE ");
		break;
	}

	strcat(fmt, format);

	va_start(ap, format);
	t = _vsnprintf(out, 1024, fmt, ap);
	va_end(ap);
	strcat(out, "\r\n");

	strbuf.assign(out);
	
	return strbuf;
}