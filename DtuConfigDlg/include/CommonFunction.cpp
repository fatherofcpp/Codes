#include "stdafx.h"
#include "CommonFunction.h"
#include "DataTypeDef.h"

const int  gDevTypeNum = DEV_TYPE_NUM;
const char gDevTypeName[DEV_TYPE_NUM][64] = 
{
	"AM-3PHASE(3����)",
	"AM-1PHASE(1����)", 
	"AM-DC(ֱ�����)",   
	"AM-HARM(г�����)",
	"E-OP(���ܵ��)",
	"WM(ˮ��)",
	"HM(������)",
	"GM(����)"
};

const int gDevTypeValue[DEV_TYPE_NUM] = 
{
	AM_3PHASE ,		//1:AM-3PHASE(3����);  
	AM_1PHASE,			//2:AM-1PHASE(1����); 
	AM_DC ,				//3:AM-DC(ֱ�����);   
	AM_HARM	,			//4:AM-HARM(г�����);  
	E_OP ,				//5:E-OP(���ܵ��);
	WM,					//6:WM(ˮ��);
	HM,					//7:HM(������)
	GM					//7:GM(����);
};
const int  gSubTypeNum = SUB_TYPE_NUM;
char gSubTypeName[SUB_TYPE_NUM][64] = 
{
	"DLT645-2007-3��-1·-����-���","XHB-modbus.Vc-3��-4·-����-���","XHB-modbus.Vb-3��-1·-����-���","XHB-modbus.Va-3��-1·-����-���-����������","DLT645-1997-3��-1·-����-���",
	"AM-3PHASE(3����)�Զ���������1","AM-3PHASE(3����)�Զ���������2","AM-3PHASE(3����)�Զ���������3","AM-3PHASE(3����)�Զ���������4",
	"XHB-07-1��-1·-����-���","XHB-modbus.Vc-1��-12·-����-���","XHB-modbus.Vb-1��-3·-����-���","XHB-modbus.Va-1��-3·-����-���",
	"AM-1PHASE(1����)�Զ���������1","AM-1PHASE(1����)�Զ���������2","AM-1PHASE(1����)�Զ���������3","AM-1PHASE(1����)�Զ���������4",
	"XHB-07-12·-ֱ��-���","XHB-modbus.vb-1·-ֱ��-���","XHB-modbus.vb-2·-ֱ��-���",
	"AM-DC(ֱ�����)�Զ���������1","AM-DC(ֱ�����)�Զ���������2","AM-DC(ֱ�����)�Զ���������3","AM-DC(ֱ�����)�Զ���������4",
	"XHB-modbus.Va-3��-1·-г��-���",
	"AM-HARM(г�����)�Զ���������1","AM-HARM(г�����)�Զ���������2","AM-HARM(г�����)�Զ���������3","AM-HARM(г�����)�Զ���������4",
	"XHB-1·-���ܵ��",
	"CJT188-2004-��ˮˮ��", "CJT188-2004-������ˮˮ��", "CJT188-2004-ֱ��ˮˮ��", "CJT188-2004-��ˮˮ��",
	"CJT188-2004-������-������","CJT188-2004-������-������",   
	"CJT188-2004-ȼ����"
};

const int gSubTypeValue[SUB_TYPE_NUM] = 
{
	AM_3PHASE_01,AM_3PHASE_02,AM_3PHASE_03,AM_3PHASE_04,AM_3PHASE_05,
	AM_3PHASE_DEF_01,AM_3PHASE_DEF_02,AM_3PHASE_DEF_03,AM_3PHASE_DEF_04,
	AM_1PHASE_01,AM_1PHASE_02,AM_1PHASE_03,AM_1PHASE_04,
	AM_1PHASE_DEF_01,AM_1PHASE_DEF_02,AM_1PHASE_DEF_03,AM_1PHASE_DEF_04,
	AM_DC_01,AM_DC_02,AM_DC_03,
	AM_DC_DEF_01,AM_DC_DEF_02,AM_DC_DEF_03,AM_DC_DEF_04,
	AM_HARM_01,
	AM_HARM_DEF_01,AM_HARM_DEF_02,AM_HARM_DEF_03,AM_HARM_DEF_04,
	E_OP_01,
	WM_01,WM_02,WM_03,WM_04,
	HM_01,HM_02,
	GM_01
};

int gWayCountValue[SUB_TYPE_NUM] =
{
	1,4,1,1,1,
	0,0,0,0,
	1,12,3,3,
	0,0,0,0,
	12,1,2,
	0,0,0,0,
	1,
	0,0,0,0,
	1,
	1,1,1,1,
	1,1,
	1
};

const int gDataBitNum = DATABIT_NUM;
const char gDataBitName[DATABIT_NUM][16] = 
{
	"7","8"
};//
const int gDataBitValue[DATABIT_NUM] = {7,8};

const int gStopNum = STOP_NUM;
const char gStopName[STOP_NUM][16] = 
{
	"1","2"
};//
const int gStopValue[STOP_NUM] = {1,2};

const int gBaudNum = BAUD_NUM;
const char gBaudName[BAUD_NUM][16] = 
{
	"1200","2400","4800","9600","19200","38400","115200"
};// 1:2400; 2:4800; 3:9600; 4:19200; 5:38400;

const int gBaudValue[BAUD_NUM] = 
{
	B1200 ,
	B2400 ,
	B4800 ,
	B9600 ,
	B19200,
	B38400,
	B115200
};

const int gParityNum = PARITY_NUM;
const char gParityName[PARITY_NUM][16] = 
{"��","��","ż"};// 0:��; 1:��; 2:ż;
const int gParityValue[PARITY_NUM] = 
{	
	NONE ,
	ODD,
	EVEN
};

const int  gReportTypeNum = REPORT_TYPE_NUM;
const char gReportTypeName[REPORT_TYPE_NUM][64] =
{
	"SQLITE",
	"CSV"
};
const int gReportTypeValue[REPORT_TYPE_NUM] = {SQLITE_REPORT,CSV_REPORT};
////////////////////////////////////////////////////////////////

int GetWayNumBySubTypeValue( int type )
{
	int ret = 0;
	for (int i = 0; i < gSubTypeNum; i++)
	{
		if (gSubTypeValue[i] == type)
		{
			ret = gWayCountValue[i];
		}
	}
	return ret;
}

CString GetSubTypeNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gSubTypeNum; i++)
	{
		if (gSubTypeValue[i] == type)
		{
			str = gSubTypeName[i];
		}
	}
	return str;
}

int GetSubTypeValueByName( CString name )
{
	int ret = -1;
	for (int i = 0; i < gSubTypeNum; i++)
	{
		if (gSubTypeName[i] == name)
		{
			ret = gSubTypeValue[i];
		}
	}
	return ret;
}

CString GetBaudNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gBaudNum; i++)
	{
		if (gBaudValue[i] == type)
		{
			str = gBaudName[i];
		}
	}
	return str;
}

int GetBaudValueByName( CString name )
{
	int ret = 0;
	for (int i = 0; i < gBaudNum; i++)
	{
		if (gBaudName[i] == name)
		{
			ret = gBaudValue[i];
		}
	}
	return ret;
}

CString GetParityNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gParityNum; i++)
	{
		if (gParityValue[i] == type)
		{
			str = gParityName[i];
		}
	}
	return str;
}

int GetParityValueByName( CString name )
{
	int ret = 0;
	for (int i = 0; i < gParityNum; i++)
	{
		if (gParityName[i] == name)
		{
			ret = gParityValue[i];
		}
	}
	return ret;
}

////////////////////////////////////////////////////////
const double gOneUnit = 0.1;
const double gTwoUnit = 0.01;
const double gThreeUnit = 0.001;
const double gFourUnit = 0.0001;
const double gConvUnit = 1.000;
/*
0x00 ����
0x01 ����оƬ�쳣
0x02 �����洢���쳣
0x04 �������ݴ洢���쳣
0x08 ��ѹ�������
0x10 �����������
0x20 ʧѹ
0x40 ʧ��
*/
const char gErrName[ERR_NAME_NUM][64] =
{
	"����оƬ�쳣",
	"�����洢���쳣",
	"�������ݴ洢���쳣",
	"��ѹ�������",
	"�����������",
	"ʧѹ",
	"ʧ��"
};

const char gAmHarmName[AM_HARM_NUM][64] = 
{
	"���ʱ��",                      
	"���״̬",
	"��ֵ�澯״̬",
	"��ѹ���",
	"�������",                        
	"Ƶ��/Hz",                             
	"A���ѹ/V",                                
	"B���ѹ/V",                                
	"C���ѹ/V",                                
	"AB�ߵ�ѹ/V",                               
	"BC�ߵ�ѹ/V",                               
	"CA�ߵ�ѹ/V",                               
	"A�����/A",                             
	"B�����/A",                             
	"C�����/A",                             
	"���й�����/kW",                                    
	"A���й�����/kW",                                  
	"B���й�����/kW",                                  
	"C���й�����/kW",                                  
	"���޹�����/kVar",                                  
	"A���޹�����/kVar",                                
	"B���޹�����/kVar",                                
	"C���޹�����/kVar",                                
	"�����ڹ���/kVa",                                   
	"A�����ڹ���/kVa",                                 
	"B�����ڹ���/kVa",                                 
	"C�����ڹ���/kVa",                                 
	"�ܹ�������/",                          
	"A�๦������/",                        
	"B�๦������/",                        
	"C�๦������/",                        
	"���ߵ���/A",                          
	"����й��ܵ���/kWh", 
	"����й�����1����/kWh",
	"����й�����2����/kWh",
	"����й�����3����/kWh",
	"����й�����4����/kWh",
	"�����й��ܵ���/kWh",   
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"�����й��ܵ���/kWh",      
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"����޹�1�ܵ���/kWh",    
	"����޹�1����1����/kWh",
	"����޹�1����2����/kWh",
	"����޹�1����3����/kWh",
	"����޹�1����4����/kWh",
	"����޹�2�ܵ���/kWh",                             
	"����޹�2����1����/kWh",
	"����޹�2����2����/kWh",
	"����޹�2����3����/kWh",
	"����޹�2����4����/kWh",
	"A�������ѹ/V",                           
	"B�������ѹ/V",                           
	"C�������ѹ/V",                           
	"A���������/A",                        
	"B���������/A",                        
	"C���������/A",                        
	"�ܻ����й�����/kW",                               
	"A������й�����/kW",                             
	"B������й�����/kW",                             
	"C������й�����/kW",                             
	"A��г����ѹ/V",                           
	"B��г����ѹ/V",                           
	"C��г����ѹ/V",                           
	"A��г������/A",                        
	"B��г������/A",                        
	"C��г������/A",                        
	"��г���й�����/kW",                               
	"A��г���й�����/kW",                             
	"B��г���й�����/kW",                             
	"C��г���й�����/kW",                             
	"Ua�ܻ�����/",                             
	"Ub�ܻ�����/",                             
	"Uc�ܻ�����/",                             
	"�����ѹ�ܻ�����/",                     
	"Ia�ܻ�����/",                             
	"Ib�ܻ�����/",                             
	"Ic�ܻ�����/",                             
	"��������ܻ�����/",                  
	"UA 2��г��/",                             
	"UA 3��г��/",                             
	"UA 4��г��/",                             
	"UA 5��г��/",                             
	"UA 6��г��/",                             
	"UA 7��г��/",                             
	"UA 8��г��/",                             
	"UA 9��г��/",                             
	"UA 10��г��/",                            
	"UA 11��г��/",                            
	"UA 12��г��/",                            
	"UA 13��г��/",                            
	"UA 14��г��/",                            
	"UA 15��г��/",                            
	"UA 16��г��/",                            
	"UA 17��г��/",                            
	"UA 18��г��/",                            
	"UA 19��г��/",                            
	"UA 20��г��/",                            
	"UA 21��г��/",                            
	"UA ���г��������/",                           
	"UA ż��г��������/",                           
	"UA ����ϵ��",                              
	"UA �绰г����������",                               
	"UB 2��г��/",                             
	"UB 3��г��/",                             
	"UB 4��г��/",                             
	"UB 5��г��/",                             
	"UB 6��г��/",                             
	"UB 7��г��/",                             
	"UB 8��г��/",                             
	"UB 9��г��/",                             
	"UB 10��г��/",                            
	"UB 11��г��/",                            
	"UB 12��г��/",                            
	"UB 13��г��/",                            
	"UB 14��г��/",                            
	"UB 15��г��/",                            
	"UB 16��г��/",                            
	"UB 17��г��/",                            
	"UB 18��г��/",                            
	"UB 19��г��/",                            
	"UB 20��г��/",                            
	"UB 21��г��/",                            
	"UB ���г��������/",                           
	"UB ż��г��������/",                           
	"UB ����ϵ��",                              
	"UB �绰г����������",                               
	"UC 2��г��/",                             
	"UC 3��г��/",                             
	"UC 4��г��/",                             
	"UC 5��г��/",                             
	"UC 6��г��/",                             
	"UC 7��г��/",                             
	"UC 8��г��/",                             
	"UC 9��г��/",                             
	"UC 10��г��/",                            
	"UC 11��г��/",                            
	"UC 12��г��/",                            
	"UC 13��г��/",                            
	"UC 14��г��/",                            
	"UC 15��г��/",                            
	"UC 16��г��/",                            
	"UC 17��г��/",                            
	"UC 18��г��/",                            
	"UC 19��г��/",                            
	"UC 20��г��/",                            
	"UC 21��г��/",   	
	"UC ���г��������/",                           
	"UC ż��г��������/",                           
	"UC ����ϵ��",                              
	"UC �绰г����������",
	"IA 2��г��/",                             
	"IA 3��г��/",                             
	"IA 4��г��/",                             
	"IA 5��г��/",                             
	"IA 6��г��/",                             
	"IA 7��г��/",                             
	"IA 8��г��/",                             
	"IA 9��г��/",                             
	"IA 10��г��/",                            
	"IA 11��г��/",                            
	"IA 12��г��/",                            
	"IA 13��г��/",                            
	"IA 14��г��/",                            
	"IA 15��г��/",                            
	"IA 16��г��/",                            
	"IA 17��г��/",                            
	"IA 18��г��/",                            
	"IA 19��г��/",                            
	"IA 20��г��/",                            
	"IA 21��г��/",   	
	"IA ���г��������/",                           
	"IA ż��г��������/",   
	"IA  Kϵ��",
	"IB 2��г��/",                             
	"IB 3��г��/",                             
	"IB 4��г��/",                             
	"IB 5��г��/",                             
	"IB 6��г��/",                             
	"IB 7��г��/",                             
	"IB 8��г��/",                             
	"IB 9��г��/",                             
	"IB 10��г��/",                            
	"IB 11��г��/",                            
	"IB 12��г��/",                            
	"IB 13��г��/",                            
	"IB 14��г��/",                            
	"IB 15��г��/",                            
	"IB 16��г��/",                            
	"IB 17��г��/",                            
	"IB 18��г��/",                            
	"IB 19��г��/",                            
	"IB 20��г��/",                            
	"IB 21��г��/",   	
	"IB ���г��������/",                           
	"IB ż��г��������/",   
	"IB  Kϵ��",
	"IC 2��г��/",                             
	"IC 3��г��/",                             
	"IC 4��г��/",                             
	"IC 5��г��/",                             
	"IC 6��г��/",                             
	"IC 7��г��/",                             
	"IC 8��г��/",                             
	"IC 9��г��/",                             
	"IC 10��г��/",                            
	"IC 11��г��/",                            
	"IC 12��г��/",                            
	"IC 13��г��/",                            
	"IC 14��г��/",                            
	"IC 15��г��/",                            
	"IC 16��г��/",                            
	"IC 17��г��/",                            
	"IC 18��г��/",                            
	"IC 19��г��/",                            
	"IC 20��г��/",                            
	"IC 21��г��/",   	
	"IC ���г��������/",                           
	"IC ż��г��������/",   
	"IC  Kϵ��"
};

const char gAm3phaseName[AM_THREE_PHASE_NUM][64] = 
{
	"���ʱ��",                      
	"���״̬",
	"��ֵ�澯״̬",
	"��ѹ���",
	"�������",                        
	"Ƶ��/Hz",                             
	"A���ѹ/V",                                
	"B���ѹ/V",                                
	"C���ѹ/V",                                
	"AB�ߵ�ѹ/V",                               
	"BC�ߵ�ѹ/V",                               
	"CA�ߵ�ѹ/V",                               
	"A�����/A",                             
	"B�����/A",                             
	"C�����/A",                             
	"���й�����/kW",                                    
	"A���й�����/kW",                                  
	"B���й�����/kW",                                  
	"C���й�����/kW",                                  
	"���޹�����/kVar",                                  
	"A���޹�����/kVar",                                
	"B���޹�����/kVar",                                
	"C���޹�����/kVar",                                
	"�����ڹ���/kVA",                                   
	"A�����ڹ���/kVA",                                 
	"B�����ڹ���/kVA",                                 
	"C�����ڹ���/kVA",                                 
	"�ܹ�������/",                          
	"A�๦������/",                        
	"B�๦������/",                        
	"C�๦������/",                        
	"���ߵ���/A",                          
	"����й��ܵ���/kWh",
	"����й�����1����/kWh",
	"����й�����2����/kWh",
	"����й�����3����/kWh",
	"����й�����4����/kWh",
	"�����й��ܵ���/kWh",   
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"�����й��ܵ���/kWh",                               
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"����޹�1�ܵ���/kVarh",  
	"����޹�1����1����/kVarh",
	"����޹�1����2����/kVarh",
	"����޹�1����3����/kVarh",
	"����޹�1����4����/kVarh",
	"����޹�2�ܵ���/kVarh",
	"����޹�2����1����/kVarh",
	"����޹�2����2����/kVarh",
	"����޹�2����3����/kVarh",
	"����޹�2����4����/kVarh"
};

const char gAm1phaseName[AM_ONE_PHASE_NUM][64] = 
{
	"���ʱ��",                      
	"���״̬",
	"��ֵ�澯״̬",
	"��ѹ���",
	"�������",                        
	"Ƶ��/Hz",                             
	"��ѹ/V",                                                              
	"����/A",                                                          
	"�й�����/kW",                                                                  
	"�޹�����/kVar",                                                                 
	"���ڹ���/kVA",                                                               
	"��������/",                                                                          
	"����й��ܵ���/kWh", 
	"����й�����1����/kWh",
	"����й�����2����/kWh",
	"����й�����3����/kWh",
	"����й�����4����/kWh",
	"�����й��ܵ���/kWh",    
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"�����й��ܵ���/kWh",   
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"����޹�1�ܵ���/kVarh",  
	"����޹�1����1����/kVarh",
	"����޹�1����2����/kVarh",
	"����޹�1����3����/kVarh",
	"����޹�1����4����/kVarh",
	"����޹�2�ܵ���/kVarh",
	"����޹�2����1����/kVarh",
	"����޹�2����2����/kVarh",
	"����޹�2����3����/kVarh",
	"����޹�2����4����/kVarh"
};

const char gAmdcName[AM_DC_NUM][64] = 
{
	"���ʱ��",                      
	"���״̬",
	"��ѹ���",
	"�������",                        
	"��ѹ/V",                                                              
	"����/A",                                                          
	"����/kW",                                                                  
	"�ܵ���/kWh"                              
};

const char gWaterMeterName[WATER_METER_NAME_NUM][64] = 
{
	"���ʱ��",                      
	"���״̬",
	"��ǰ�ۼ�����/������"
};

const char gGassMeterName[GASS_METER_NAME_NUM][64] = 
{
	"���ʱ��",                      
	"���״̬",
	"˲ʱ����/t/h",
	"Ƶ��/Hz",
	"��ѹ/kPa",
	"ѹ��/MPa",
	"�¶�/���϶�",
	"����¶�/���϶�",
	"�����¶�/���϶�",
	"�����ܶ�/Kg/m3",
	"˲ʱ����/Gj/h",
	"����/KJ/Kg",
	"���ʲ�/KJ/Kg",
	"�ۼ�����/t",
	"�ۼ�����/Gj",
	"�ۼ��Ȳ���/Gj"
};

CString GetSaveFileNameByDevType( e_dev_type dev_type )
{
	CString str;
	switch(dev_type)
	{
	case AM_3PHASE:
		str = "AM-3PHASE(3����).csv";
		break;

	case AM_1PHASE:
		str = "AM-1PHASE(1����).csv";
		break;

	case AM_DC:
		str = "AM-DC(ֱ�����).csv";
		break;

	case AM_HARM:
		str = "AM-HARM(г�����).csv";
		break;

	case WM:
		str = "WM(ˮ��).csv";
		break;
	case GM:
		str = "GM(����).csv";
		break;
	default:
		break;
	}
	return str;
}

CString GetSaveFileHeadByDevType( e_dev_type dev_type )
{
	CString str;

	switch(dev_type)
	{
	case AM_3PHASE:
		for (int i = 0; i < AM_THREE_PHASE_NUM; i++)
		{
			str.Append(gAm3phaseName[i]);
			str.Append(",");
		}
		break;

	case AM_1PHASE:
		for (int i = 0; i < AM_ONE_PHASE_NUM; i++)
		{
			str.Append(gAm1phaseName[i]);
			str.Append(",");
		}
		break;

	case AM_DC:
		for (int i = 0; i < AM_DC_NUM; i++)
		{
			str.Append(gAmdcName[i]);
			str.Append(",");
		}
		break;

	case AM_HARM:
		for (int i = 0; i < AM_HARM_NUM; i++)
		{
			str.Append(gAmHarmName[i]);
			str.Append(",");
		}
		break;

	case WM:
		for (int i = 0; i < WATER_METER_NAME_NUM; i++)
		{
			str.Append(gWaterMeterName[i]);
			str.Append(",");
		}
		break;
	case GM:
		for (int i = 0; i < GASS_METER_NAME_NUM; i++)
		{
			str.Append(gGassMeterName[i]);
			str.Append(",");
		}
		break;
	default:
		break;
	}
	return str;
}

CString GetDevTypeNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gDevTypeNum; i++)
	{
		if (gDevTypeValue[i] == type)
		{
			str = gDevTypeName[i];
		}
	}
	return str;
}

int gComNum = 0;

e_company_type gCompanyType = DEFAUL_COMPANY;
CString gCurrentDir;

////
/*
ELECTRICITY,���磩
AIR_CONDITIONING,���յ���
LIGHTING,��������
POWER_SOCKET,��������
HARMONIC,��г����
TEMPERATURE,���¶ȣ�
SWITCH,�����أ�
RELAY,���̵�����
RECLOSER,�غ�բ
APF_FILTER,��Ե�˲���
WATERSTEAM ,ˮ����


*/
const int gWayDevTypeNum = WAY_DEV_TYPE_NAME_NUM;
const char gWayDevTypeName[WAY_DEV_TYPE_NAME_NUM][64] = 
{
	"����"
};
const int gWayDevTypeValue[WAY_DEV_TYPE_NAME_NUM] =
{
	E_WAY_TYPE_TV
};


int GetWayDevTypeValueByName( CString name )
{
	int ret = 0;
	for (int i = 0; i < gWayDevTypeNum; i++)
	{
		if (gWayDevTypeName[i] == name)
		{
			ret = i;
		}
	}
	return ret;
}

std::string GetWayDevTypeNameByValue( int type )
{
	string str = "";
	for (int i = 0; i < gWayDevTypeNum; i++)
	{
		if (gWayDevTypeValue[i] == type)
		{
			str = gWayDevTypeName[i];
		}
	}
	return str;
}

bool IsWayUsedByMask(int way_ind, int mask)
{
	bool used = false;
	int flag = 0x01 << way_ind;
	if(flag & mask)
	{
		used = true;
	}
	else
	{
		used = false;
	}
	return used;
}

int GetDevIndByIdSubType( str_outer_dev* outer_dev, MU_INT64 id, e_dev_sub_type sub_type, int port_ind )
{
	int dev_num = outer_dev->port_dev[port_ind].dev_num;

	for (int i = 0; i < dev_num; i++)
	{
		if(id == outer_dev->port_dev[port_ind].dev_inf[i].dev_addr.id &&
			sub_type == outer_dev->port_dev[port_ind].dev_inf[i].sub_type)
		{
			return i;
		}
	}
	return -1;
}

const int gEffectNowNum = EFFECT_NOW_NUM;
const char gEffectNowName[EFFECT_NOW_NUM][64] = 
{
	"hostapd",
	"server"
};

const int gAllDevTypeReportParamNum = ALL_DEVTYPE_REPORT_PARAM_NUM;
char gAllDevTypeReportParamName[ALL_DEVTYPE_REPORT_PARAM_NUM][64] = 
{
	"��ֵ�ܵ���/kWh",
	"����й��ܵ���/kWh",
	"�����й��ܵ���/kWh",
	"�����й��ܵ���/kWh",
	"��ѹ/V",
	"����/A",
	"��ѹ���/",
	"�������/",
	"Ƶ��/Hz",
	"���й�����/kW",
	"���޹�����/kVar",
	"�����ڹ���/kVA",
	"�ܹ�������/",
	"����޹�1�ܵ���/kVarh",
	"����޹�2�ܵ���/kVarh",
	"���ߵ���/A",
	"A���ѹ/V",
	"B���ѹ/V",
	"C���ѹ/V",
	"AB�ߵ�ѹ/V",
	"BC�ߵ�ѹ/V",
	"CA�ߵ�ѹ/V",
	"A�����/A",
	"B�����/A",
	"C�����/A",
	"A���й�����/kW",
	"B���й�����/kW",
	"C���й�����/kW",
	"A���޹�����/kVar",
	"B���޹�����/kVar",
	"C���޹�����/kVar",
	"A�����ڹ���/kVa",
	"B�����ڹ���/kVa",
	"C�����ڹ���/kVa",
	"A�๦������/",
	"B�๦������/",
	"C�๦������/",
	"A������й��ܵ���/kWh",
	"B������й��ܵ���/kWh",
	"C������й��ܵ���/kWh",
	"���״̬/",
	"A�������ѹ/V",
	"B�������ѹ/V",
	"C�������ѹ/V",
	"A���������/A",
	"B���������/A",
	"C���������/A",
	"�ܻ����й�����/kW",
	"A������й�����/kW",
	"B������й�����/kW",
	"C������й�����/kW",
	"A��г����ѹ/V",
	"B��г����ѹ/V",
	"C��г����ѹ/V",
	"A��г������/A",
	"B��г������/A",
	"C��г������/A",
	"��г���й�����/kW",
	"A��г���й�����/kW",
	"B��г���й�����/kW",
	"C��г���й�����/kW",
	"Ua�ܻ�����/%",
	"Ub�ܻ�����/%",
	"Uc�ܻ�����/%",
	"�����ѹ�ܻ�����/%",
	"Ia�ܻ�����/%",
	"Ib�ܻ�����/%",
	"Ic�ܻ�����/%",
	"��������ܻ�����/%",
	"UA 2��г��/%",
	"UA 3��г��/%",
	"UA 4��г��/%",
	"UA 5��г��/%",
	"UA 6��г��/%",
	"UA 7��г��/%",
	"UA 8��г��/%",
	"UA 9��г��/%",
	"UA 10��г��/%",
	"UA 11��г��/%",
	"UA 12��г��/%",
	"UA 13��г��/%",
	"UA 14��г��/%",
	"UA 15��г��/%",
	"UA 16��г��/%",
	"UA 17��г��/%",
	"UA 18��г��/%",
	"UA 19��г��/%",
	"UA 20��г��/%",
	"UA 21��г��/%",
	"UA ���г��������/%",
	"UA ż��г��������/%",
	"UA ����ϵ��/",
	"UA �绰г����������/",
	"UB 2��г��/%",
	"UB 3��г��/%",
	"UB 4��г��/%",
	"UB 5��г��/%",
	"UB 6��г��/%",
	"UB 7��г��/%",
	"UB 8��г��/%",
	"UB 9��г��/%",
	"UB 10��г��/%",
	"UB 11��г��/%",
	"UB 12��г��/%",
	"UB 13��г��/%",
	"UB 14��г��/%",
	"UB 15��г��/%",
	"UB 16��г��/%",
	"UB 17��г��/%",
	"UB 18��г��/%",
	"UB 19��г��/%",
	"UB 20��г��/%",
	"UB 21��г��/%",
	"UB ���г��������/%",
	"UB ż��г��������/%",
	"UB ����ϵ��/",
	"UB �绰г����������/",
	"UC 2��г��/%",
	"UC 3��г��/%",
	"UC 4��г��/%",
	"UC 5��г��/%",
	"UC 6��г��/%",
	"UC 7��г��/%",
	"UC 8��г��/%",
	"UC 9��г��/%",
	"UC 10��г��/%",
	"UC 11��г��/%",
	"UC 12��г��/%",
	"UC 13��г��/%",
	"UC 14��г��/%",
	"UC 15��г��/%",
	"UC 16��г��/%",
	"UC 17��г��/%",
	"UC 18��г��/%",
	"UC 19��г��/%",
	"UC 20��г��/%",
	"UC21��г��/%",
	"UC ���г��������/%",
	"UC ż��г��������/%",
	"UC ����ϵ��/",
	"UC �绰г����������/",
	"IA 2��г��/%",
	"IA 3��г��/%",
	"IA 4��г��/%",
	"IA 5��г��/%",
	"IA 6��г��/%",
	"IA 7��г��/%",
	"IA 8��г��/%",
	"IA 9��г��/%",
	"IA 10��г��/%",
	"IA 11��г��/%",
	"IA 12��г��/%",
	"IA 13��г��/%",
	"IA 14��г��/%",
	"IA 15��г��/%",
	"IA 16��г��/%",
	"IA 17��г��/%",
	"IA 18��г��/%",
	"IA 19��г��/%",
	"IA 20��г��/%",
	"IA21��г��/%",
	"IA ���г��������/%",
	"IA ż��г��������/%",
	"IA  Kϵ��/",
	"IB 2��г��/%",
	"IB 3��г��/%",
	"IB 4��г��/%",
	"IB 5��г��/%",
	"IB 6��г��/%",
	"IB 7��г��/%",
	"IB 8��г��/%",
	"IB 9��г��/%",
	"IB 10��г��/%",
	"IB 11��г��/%",
	"IB 12��г��/%",
	"IB 13��г��/%",
	"IB 14��г��/%",
	"IB 15��г��/%",
	"IB 16��г��/%",
	"IB 17��г��/%",
	"IB 18��г��/%",
	"IB 19��г��/%",
	"IB 20��г��/%",
	"IB 21��г��/%",
	"IB ���г��������/%",
	"IB ż��г��������/%",
	"IB  Kϵ��/",
	"IC 2��г��/%",
	"IC 3��г��/%",
	"IC 4��г��/%",
	"IC 5��г��/%",
	"IC 6��г��/%",
	"IC 7��г��/%",
	"IC 8��г��/%",
	"IC 9��г��/%",
	"IC 10��г��/%",
	"IC 11��г��/%",
	"IC 12��г��/%",
	"IC 13��г��/%",
	"IC 14��г��/%",
	"IC 15��г��/%",
	"IC 16��г��/%",
	"IC 17��г��/%",
	"IC 18��г��/%",
	"IC 19��г��/%",
	"IC 20��г��/%",
	"IC 21��г��/%",
	"IC ���г��������/%",
	"IC ż��г��������/%",
	"IC  Kϵ��/",
	"����������/��λ",
	"��ǰ����/��λ",
	"�ȹ���/��λ",
	"����/��λ",
	"�ۼ�����/��λ",
	"��ˮ�¶�/��",
	"��ˮ�¶�/��",
	"�ۼƹ���ʱ��/h",
	"��ǰ�ۼ�����/��λ",
	"�������ۼ�����/��λ",
	"ʱ��/",
	"��ֵ�澯״̬/",
	"����й�����1����/kWh",
	"����й�����2����/kWh",
	"����й�����3����/kWh",
	"����й�����4����/kWh",
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"�����й�����1����/kWh",
	"�����й�����2����/kWh",
	"�����й�����3����/kWh",
	"�����й�����4����/kWh",
	"����޹�1����1����/kVarh",
	"����޹�1����2����/kVarh",
	"����޹�1����3����/kVarh",
	"����޹�1����4����/kVarh",
	"����޹�2����1����/kVarh",
	"����޹�2����2����/kVarh",
	"����޹�2����3����/kVarh",
	"����޹�2����4����/kVarh",
	"��ٹ���/",
	"�Ϸ�״̬/",
	"�غϴ���/",
	"�غ�ʱ����/��",
	"�ɹ���բ�ж�ʱ��/��"
};

const int gAllDevTypeReportParamValue[ALL_DEVTYPE_REPORT_PARAM_NUM] =
{
	E_REPORT_ID_COMBINE_DIF    ,     //��ֵ�ܵ���          
	E_REPORT_ID_T_COMBINE             ,     //����й��ܵ���      
	E_REPORT_ID_T_POSITIVE            ,     //�����й��ܵ���      
	E_REPORT_ID_T_REVERSE             ,     //�����й��ܵ���      
	E_REPORT_ID_VOLT             ,     //��ѹ                
	E_REPORT_ID_CURRENT             ,     //����                
	E_REPORT_ID_VOLT_CHANGE         ,     //��ѹ���            
	E_REPORT_ID_CURR_CHANGE         ,     //�������            
	E_REPORT_ID_FREQUENCY           ,     //Ƶ��                
	E_REPORT_ID_ACTIVE              ,     //���й�����          
	E_REPORT_ID_REACTIVE            ,     //���޹�����          
	E_REPORT_ID_APPARENT            ,     //�����ڹ���          
	E_REPORT_ID_FACTOR              ,     //�ܹ�������          
	E_REPORT_ID_COMBINE1            ,     //����޹�1�ܵ���     
	E_REPORT_ID_COMBINE2            ,     //����޹�2�ܵ���     
	E_REPORT_ID_N_CURRENT           ,     //���ߵ���            
	E_REPORT_ID_A_VOLT              ,     //A���ѹ             
	E_REPORT_ID_B_VOLT              ,     //B���ѹ             
	E_REPORT_ID_C_VOLT              ,     //C���ѹ             
	E_REPORT_ID_AB_VOLT             ,     //AB�ߵ�ѹ            
	E_REPORT_ID_BC_VOLT             ,     //BC�ߵ�ѹ            
	E_REPORT_ID_CA_VOLT             ,     //CA�ߵ�ѹ            
	E_REPORT_ID_A_CURRENT           ,     //A�����             
	E_REPORT_ID_B_CURRENT           ,     //B�����             
	E_REPORT_ID_C_CURRENT           ,     //C�����             
	E_REPORT_ID_ACTIVE_A            ,     //A���й�����         
	E_REPORT_ID_ACTIVE_B            ,     //B���й�����         
	E_REPORT_ID_ACTIVE_C            ,     //C���й�����         
	E_REPORT_ID_REACTIVE_A          ,     //A���޹�����         
	E_REPORT_ID_REACTIVE_B          ,     //B���޹�����         
	E_REPORT_ID_REACTIVE_C          ,     //C���޹�����         
	E_REPORT_ID_APPARENT_A          ,     //A�����ڹ���         
	E_REPORT_ID_APPARENT_B          ,     //B�����ڹ���         
	E_REPORT_ID_APPARENT_C          ,     //C�����ڹ���         
	E_REPORT_ID_FACTOR_A            ,     //A�๦������         
	E_REPORT_ID_FACTOR_B            ,     //B�๦������         
	E_REPORT_ID_FACTOR_C            ,     //C�๦������         
	E_REPORT_ID_A_COMBINE           ,     //A������й��ܵ���   
	E_REPORT_ID_B_COMBINE           ,     //B������й��ܵ���   
	E_REPORT_ID_C_COMBINE           ,     //C������й��ܵ���   
	E_REPORT_ID_METER_STATE         ,     //���״̬            
	E_REPORT_ID_ORIGINAL_VOLTAGE_A  ,     //A�������ѹ         
	E_REPORT_ID_ORIGINAL_VOLTAGE_B  ,     //B�������ѹ         
	E_REPORT_ID_ORIGINAL_VOLTAGE_C  ,     //C�������ѹ         
	E_REPORT_ID_ORIGINAL_CURRENT_A  ,     //A���������         
	E_REPORT_ID_ORIGINAL_CURRENT_B  ,     //B���������         
	E_REPORT_ID_ORIGINAL_CURRENT_C  ,     //C���������         
	E_REPORT_ID_ORIGINAL_ACTIVE_T   ,     //�ܻ����й�����      
	E_REPORT_ID_ORIGINAL_ACTIVE_A   ,     //A������й�����     
	E_REPORT_ID_ORIGINAL_ACTIVE_B   ,     //B������й�����     
	E_REPORT_ID_ORIGINAL_ACTIVE_C   ,     //C������й�����     
	E_REPORT_ID_HARMONIC_VOLTAGE_A  ,     //A��г����ѹ         
	E_REPORT_ID_HARMONIC_VOLTAGE_B  ,     //B��г����ѹ         
	E_REPORT_ID_HARMONIC_VOLTAGE_C  ,     //C��г����ѹ         
	E_REPORT_ID_HARMONIC_CURRENT_A  ,     //A��г������         
	E_REPORT_ID_HARMONIC_CURRENT_B  ,     //B��г������         
	E_REPORT_ID_HARMONIC_CURRENT_C  ,     //C��г������         
	E_REPORT_ID_HARMONIC_ACTIVE_T   ,     //��г���й�����      
	E_REPORT_ID_HARMONIC_ACTIVE_A   ,     //A��г���й�����     
	E_REPORT_ID_HARMONIC_ACTIVE_B   ,     //B��г���й�����     
	E_REPORT_ID_HARMONIC_ACTIVE_C   ,     //C��г���й�����     
	E_REPORT_ID_THD_UA              ,     //Ua�ܻ�����          
	E_REPORT_ID_THD_UB              ,     //Ub�ܻ�����          
	E_REPORT_ID_THD_UC              ,     //Uc�ܻ�����          
	E_REPORT_ID_THD_UT              ,     //�����ѹ�ܻ�����    
	E_REPORT_ID_THD_IA              ,     //Ia�ܻ�����          
	E_REPORT_ID_THD_IB              ,     //Ib�ܻ�����          
	E_REPORT_ID_THD_IC              ,     //Ic�ܻ�����          
	E_REPORT_ID_THD_IT              ,     //��������ܻ�����    
	E_REPORT_ID_HARMONIC_UA_2       ,     //UA 2��г��          
	E_REPORT_ID_HARMONIC_UA_3       ,     //UA 3��г��          
	E_REPORT_ID_HARMONIC_UA_4       ,     //UA 4��г��          
	E_REPORT_ID_HARMONIC_UA_5       ,     //UA 5��г��          
	E_REPORT_ID_HARMONIC_UA_6       ,     //UA 6��г��          
	E_REPORT_ID_HARMONIC_UA_7       ,     //UA 7��г��          
	E_REPORT_ID_HARMONIC_UA_8       ,     //UA 8��г��          
	E_REPORT_ID_HARMONIC_UA_9       ,     //UA 9��г��          
	E_REPORT_ID_HARMONIC_UA_10      ,     //UA 10��г��         
	E_REPORT_ID_HARMONIC_UA_11      ,     //UA 11��г��         
	E_REPORT_ID_HARMONIC_UA_12      ,     //UA 12��г��         
	E_REPORT_ID_HARMONIC_UA_13      ,     //UA 13��г��         
	E_REPORT_ID_HARMONIC_UA_14      ,     //UA 14��г��         
	E_REPORT_ID_HARMONIC_UA_15      ,     //UA 15��г��         
	E_REPORT_ID_HARMONIC_UA_16      ,     //UA 16��г��         
	E_REPORT_ID_HARMONIC_UA_17      ,     //UA 17��г��         
	E_REPORT_ID_HARMONIC_UA_18      ,     //UA 18��г��         
	E_REPORT_ID_HARMONIC_UA_19      ,     //UA 19��г��         
	E_REPORT_ID_HARMONIC_UA_20      ,     //UA 20��г��         
	E_REPORT_ID_HARMONIC_UA_21      ,     //UA 21��г��         
	E_REPORT_ID_HARMONIC_UA_ODD     ,     //UA ���г��������   
	E_REPORT_ID_HARMONIC_UA_EVEN    ,     //UA ż��г��������   
	E_REPORT_ID_HARMONIC_UA_RIDGE   ,     //UA ����ϵ��         
	E_REPORT_ID_HARMONIC_UA_TETL    ,     //UA �绰г���������� 
	E_REPORT_ID_HARMONIC_UB_2       ,     //UB 2��г��          
	E_REPORT_ID_HARMONIC_UB_3       ,     //UB 3��г��          
	E_REPORT_ID_HARMONIC_UB_4       ,     //UB 4��г��          
	E_REPORT_ID_HARMONIC_UB_5       ,     //UB 5��г��          
	E_REPORT_ID_HARMONIC_UB_6       ,     //UB 6��г��          
	E_REPORT_ID_HARMONIC_UB_7       ,     //UB 7��г��          
	E_REPORT_ID_HARMONIC_UB_8       ,     //UB 8��г��          
	E_REPORT_ID_HARMONIC_UB_9       ,     //UB 9��г��          
	E_REPORT_ID_HARMONIC_UB_10      ,     //UB 10��г��         
	E_REPORT_ID_HARMONIC_UB_11      ,     //UB 11��г��         
	E_REPORT_ID_HARMONIC_UB_12      ,     //UB 12��г��         
	E_REPORT_ID_HARMONIC_UB_13      ,     //UB 13��г��        
	E_REPORT_ID_HARMONIC_UB_14      ,     //UB 14��г��        
	E_REPORT_ID_HARMONIC_UB_15      ,     //UB 15��г��        
	E_REPORT_ID_HARMONIC_UB_16      ,     //UB 16��г��        
	E_REPORT_ID_HARMONIC_UB_17      ,     //UB 17��г��        
	E_REPORT_ID_HARMONIC_UB_18      ,     //UB 18��г��        
	E_REPORT_ID_HARMONIC_UB_19      ,     //UB 19��г��        
	E_REPORT_ID_HARMONIC_UB_20      ,     //UB 20��г��        
	E_REPORT_ID_HARMONIC_UB_21      ,     //UB 21��г��        
	E_REPORT_ID_HARMONIC_UB_ODD     ,     //UB ���г��������  
	E_REPORT_ID_HARMONIC_UB_EVEN    ,     //UB ż��г��������  
	E_REPORT_ID_HARMONIC_UB_RIDGE   ,     //UB ����ϵ��        
	E_REPORT_ID_HARMONIC_UB_TETL    ,     //UB �绰г����������
	E_REPORT_ID_HARMONIC_UC_2       ,     //UC 2��г��         
	E_REPORT_ID_HARMONIC_UC_3       ,     //UC 3��г��         
	E_REPORT_ID_HARMONIC_UC_4       ,     //UC 4��г��         
	E_REPORT_ID_HARMONIC_UC_5       ,     //UC 5��г��         
	E_REPORT_ID_HARMONIC_UC_6       ,     //UC 6��г��         
	E_REPORT_ID_HARMONIC_UC_7       ,     //UC 7��г��         
	E_REPORT_ID_HARMONIC_UC_8       ,     //UC 8��г��         
	E_REPORT_ID_HARMONIC_UC_9       ,     //UC 9��г��         
	E_REPORT_ID_HARMONIC_UC_10      ,     //UC 10��г��        
	E_REPORT_ID_HARMONIC_UC_11      ,     //UC 11��г��        
	E_REPORT_ID_HARMONIC_UC_12      ,     //UC 12��г��        
	E_REPORT_ID_HARMONIC_UC_13      ,     //UC 13��г��        
	E_REPORT_ID_HARMONIC_UC_14      ,     //UC 14��г��        
	E_REPORT_ID_HARMONIC_UC_15      ,     //UC 15��г��        
	E_REPORT_ID_HARMONIC_UC_16      ,     //UC 16��г��        
	E_REPORT_ID_HARMONIC_UC_17      ,     //UC 17��г��        
	E_REPORT_ID_HARMONIC_UC_18      ,     //UC 18��г��        
	E_REPORT_ID_HARMONIC_UC_19      ,     //UC 19��г��        
	E_REPORT_ID_HARMONIC_UC_20      ,     //UC 20��г��        
	E_REPORT_ID_HARMONIC_UC_21      ,     //UC21��г��         
	E_REPORT_ID_HARMONIC_UC_ODD     ,     //UC ���г��������  
	E_REPORT_ID_HARMONIC_UC_EVEN    ,     //UC ż��г��������  
	E_REPORT_ID_HARMONIC_UC_RIDGE   ,     //UC ����ϵ��        
	E_REPORT_ID_HARMONIC_UC_TETL    ,     //UC �绰г����������
	E_REPORT_ID_HARMONIC_IA_2       ,     //IA 2��г��         
	E_REPORT_ID_HARMONIC_IA_3       ,     //IA 3��г��         
	E_REPORT_ID_HARMONIC_IA_4       ,     //IA 4��г��         
	E_REPORT_ID_HARMONIC_IA_5       ,     //IA 5��г��         
	E_REPORT_ID_HARMONIC_IA_6       ,     //IA 6��г��         
	E_REPORT_ID_HARMONIC_IA_7       ,     //IA 7��г��         
	E_REPORT_ID_HARMONIC_IA_8       ,     //IA 8��г��         
	E_REPORT_ID_HARMONIC_IA_9       ,     //IA 9��г��         
	E_REPORT_ID_HARMONIC_IA_10      ,     //IA 10��г��        
	E_REPORT_ID_HARMONIC_IA_11      ,     //IA 11��г��        
	E_REPORT_ID_HARMONIC_IA_12      ,     //IA 12��г��        
	E_REPORT_ID_HARMONIC_IA_13      ,     //IA 13��г��        
	E_REPORT_ID_HARMONIC_IA_14      ,     //IA 14��г��        
	E_REPORT_ID_HARMONIC_IA_15      ,     //IA 15��г��        
	E_REPORT_ID_HARMONIC_IA_16      ,     //IA 16��г��        
	E_REPORT_ID_HARMONIC_IA_17      ,     //IA 17��г��        
	E_REPORT_ID_HARMONIC_IA_18      ,     //IA 18��г��        
	E_REPORT_ID_HARMONIC_IA_19      ,     //IA 19��г��        
	E_REPORT_ID_HARMONIC_IA_20      ,     //IA 20��г��        
	E_REPORT_ID_HARMONIC_IA_21      ,     //IA21��г��         
	E_REPORT_ID_HARMONIC_IA_ODD     ,     //IA ���г��������  
	E_REPORT_ID_HARMONIC_IA_EVEN    ,     //IA ż��г��������  
	E_REPORT_ID_HARMONIC_IA_K       ,     //IA  Kϵ��          
	E_REPORT_ID_HARMONIC_IB_2       ,     //IB 2��г��         
	E_REPORT_ID_HARMONIC_IB_3       ,     //IB 3��г��         
	E_REPORT_ID_HARMONIC_IB_4       ,     //IB 4��г��         
	E_REPORT_ID_HARMONIC_IB_5       ,     //IB 5��г��         
	E_REPORT_ID_HARMONIC_IB_6       ,     //IB 6��г��         
	E_REPORT_ID_HARMONIC_IB_7       ,     //IB 7��г��         
	E_REPORT_ID_HARMONIC_IB_8       ,     //IB 8��г��         
	E_REPORT_ID_HARMONIC_IB_9       ,     //IB 9��г��         
	E_REPORT_ID_HARMONIC_IB_10      ,     //IB 10��г��        
	E_REPORT_ID_HARMONIC_IB_11      ,     //IB 11��г��        
	E_REPORT_ID_HARMONIC_IB_12      ,     //IB 12��г��        
	E_REPORT_ID_HARMONIC_IB_13      ,     //IB 13��г��        
	E_REPORT_ID_HARMONIC_IB_14      ,     //IB 14��г��        
	E_REPORT_ID_HARMONIC_IB_15      ,     //IB 15��г��        
	E_REPORT_ID_HARMONIC_IB_16      ,     //IB 16��г��        
	E_REPORT_ID_HARMONIC_IB_17      ,     //IB 17��г��        
	E_REPORT_ID_HARMONIC_IB_18      ,     //IB 18��г��        
	E_REPORT_ID_HARMONIC_IB_19      ,     //IB 19��г��        
	E_REPORT_ID_HARMONIC_IB_20      ,     //IB 20��г��        
	E_REPORT_ID_HARMONIC_IB_21      ,     //IB 21��г��        
	E_REPORT_ID_HARMONIC_IB_ODD     ,     //IB ���г��������  
	E_REPORT_ID_HARMONIC_IB_EVEN    ,     //IB ż��г��������  
	E_REPORT_ID_HARMONIC_IB_K       ,     //IB  Kϵ��          
	E_REPORT_ID_HARMONIC_IC_2       ,     //IC 2��г��         
	E_REPORT_ID_HARMONIC_IC_3       ,     //IC 3��г��         
	E_REPORT_ID_HARMONIC_IC_4       ,     //IC 4��г��         
	E_REPORT_ID_HARMONIC_IC_5       ,     //IC 5��г��         
	E_REPORT_ID_HARMONIC_IC_6       ,     //IC 6��г��         
	E_REPORT_ID_HARMONIC_IC_7       ,     //IC 7��г��         
	E_REPORT_ID_HARMONIC_IC_8       ,     //IC 8��г��         
	E_REPORT_ID_HARMONIC_IC_9       ,     //IC 9��г��         
	E_REPORT_ID_HARMONIC_IC_10      ,     //IC 10��г��        
	E_REPORT_ID_HARMONIC_IC_11      ,     //IC 11��г��        
	E_REPORT_ID_HARMONIC_IC_12      ,     //IC 12��г��        
	E_REPORT_ID_HARMONIC_IC_13      ,     //IC 13��г��        
	E_REPORT_ID_HARMONIC_IC_14      ,     //IC 14��г��        
	E_REPORT_ID_HARMONIC_IC_15      ,     //IC 15��г��        
	E_REPORT_ID_HARMONIC_IC_16      ,     //IC 16��г��        
	E_REPORT_ID_HARMONIC_IC_17      ,     //IC 17��г��        
	E_REPORT_ID_HARMONIC_IC_18      ,     //IC 18��г��        
	E_REPORT_ID_HARMONIC_IC_19      ,     //IC 19��г��        
	E_REPORT_ID_HARMONIC_IC_20      ,     //IC 20��г��        
	E_REPORT_ID_HARMONIC_IC_21      ,     //IC 21��г��        
	E_REPORT_ID_HARMONIC_IC_ODD     ,     //IC ���г��������  
	E_REPORT_ID_HARMONIC_IC_EVEN    ,     //IC ż��г��������  
	E_REPORT_ID_HARMONIC_IC_K       ,//IC  Kϵ��  
	E_REPORT_ID_HEAT_SETTLE_DATA,//����������
	E_REPORT_ID_CUR_HEAT,//��ǰ����
	E_REPORT_ID_THERMAL_POWER,//�ȹ���
	E_REPORT_ID_FLOW,//����
	E_REPORT_ID_CUMULA_FLOW,//�ۼ�����
	E_REPORT_ID_SUPPLYWATER_TEMP,//��ˮ�¶�
	E_REPORT_ID_BACKWATER_TEMP,//��ˮ�¶�
	E_REPORT_ID_CUMULA_TIME,//�ۼƹ���ʱ��
	E_REPORT_ID_CUR_CUMULA_FLOW,//��ǰ�ۼ�����
	E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA,//�������ۼ�����
	E_REPORT_ID_METER_TIME,//ʱ��
	E_REPORT_ID_THRESHOLD_STATE,//��ֵ�澯״̬
	E_REPORT_ID_T_COMBINE_RATE1,//����й�����1����
	E_REPORT_ID_T_COMBINE_RATE2,//����й�����2����
	E_REPORT_ID_T_COMBINE_RATE3,//����й�����3����
	E_REPORT_ID_T_COMBINE_RATE4,//����й�����4����
	E_REPORT_ID_T_POSITIVE_RATE1,//�����й�����1����
	E_REPORT_ID_T_POSITIVE_RATE2,//�����й�����2����
	E_REPORT_ID_T_POSITIVE_RATE3,//�����й�����3����
	E_REPORT_ID_T_POSITIVE_RATE4,//�����й�����4����
	E_REPORT_ID_T_REVERSE_RATE1,//�����й�����1����
	E_REPORT_ID_T_REVERSE_RATE2,//�����й�����2����
	E_REPORT_ID_T_REVERSE_RATE3,//�����й�����3����
	E_REPORT_ID_T_REVERSE_RATE4,//�����й�����4����
	E_REPORT_ID_T_COMBINE1_RATE1,//����޹�1����1����
	E_REPORT_ID_T_COMBINE1_RATE2,//����޹�1����2����
	E_REPORT_ID_T_COMBINE1_RATE3,//����޹�1����3����
	E_REPORT_ID_T_COMBINE1_RATE4,//����޹�1����4����
	E_REPORT_ID_T_COMBINE2_RATE1,//����޹�2����1����
	E_REPORT_ID_T_COMBINE2_RATE2,//����޹�2����2����
	E_REPORT_ID_T_COMBINE2_RATE3,//����޹�2����3����
	E_REPORT_ID_T_COMBINE2_RATE4,//����޹�2����4����
	E_REPORT_ID_E_OP_FAULT,//��ٹ���
	E_REPORT_ID_ON_OFF_STATE,//�Ϸ�״̬
	E_REPORT_ID_ON_COUNT,//�غϴ���
	E_REPORT_ID_ON_TIMER,//�غ�ʱ����
	E_REPORT_ID_ON_THINK_TIME//�ɹ���բ�ж�ʱ��
};

int GetDevTypeValueByName( CString name )
{
	int ret = 0;
	for (int i = 0; i < gDevTypeNum; i++)
	{
		if (gDevTypeName[i] == name)
		{
			ret = gDevTypeValue[i];
		}
	}
	return ret;
}

CString GetDataBitNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gDataBitNum; i++)
	{
		if (gDataBitValue[i] == type)
		{
			str = gDataBitName[i];
		}
	}
	return str;
}

int GetDataBitValueByName( string name )
{
	int ret = 0;
	for (int i = 0; i < gDataBitNum; i++)
	{
		if (strcmp(gDataBitName[i], name.c_str()) == 0)
		{
			ret = gDataBitValue[i];
		}
	}
	return ret;
}

CString GetStopNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gDataBitNum; i++)
	{
		if (gStopValue[i] == type)
		{
			str = gStopName[i];
		}
	}
	return str;
}

int GetStopValueByName( string name )
{
	int ret = 0;
	for (int i = 0; i < gStopNum; i++)
	{
		if (strcmp(gStopName[i], name.c_str()) == 0)
		{
			ret = gStopValue[i];
		}
	}
	return ret;
}

int GetDevTypeIndByValue( int type )
{
	int ind = 0;
	for (int i = 0; i < gDevTypeNum; i++)
	{
		if (gDevTypeValue[i] == type)
		{
			ind = i;
		}
	}
	return ind;
}

int GetWayListByMask(int way_mask, VEC_INT& vec_data, int max_way_count)
{
	int list_num = 0;

	for (int i = 0; i < max_way_count; i++)
	{
		int flag = 0x01 << i;
		if(way_mask & flag)
		{
			vec_data.push_back(i);
			list_num ++;
		}
	}
	return list_num;
}

int GetAllDevTypeReportParamValueByName( CString name )
{
	int ret = 0;
	for (int i = 0; i < gAllDevTypeReportParamNum; i++)
	{
		if (gAllDevTypeReportParamName[i] == name)
		{
			ret = gAllDevTypeReportParamValue[i];
		}
	}
	return ret;
}

CString GetReportTypeNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gReportTypeNum; i++)
	{
		if (gReportTypeValue[i] == type)
		{
			str = gReportTypeName[i];
		}
	}
	return str;
}

int GetReportTypeIndByValue( int type )
{
	int ind = -1;
	for (int i = 0; i < gReportTypeNum; i++)
	{
		if (gReportTypeValue[i] == type)
		{
			ind = i;
		}
	}
	return ind;
}

int GetReportIndByDevType(e_dev_type dev_type, st_report_format* report_format)
{
	for (int i = 0; i < report_format->dev_type_num; i++)
	{
		if(report_format->dev_type_report[i].dev_type == dev_type)
		{
			return i;
		}
	}
	return -1;
}

CString GetAllDevTypeReportParamNameByValue( int type )
{
	CString str = "";
	for (int i = 0; i < gAllDevTypeReportParamNum; i++)
	{
		if (gAllDevTypeReportParamValue[i] == type)
		{
			str = gAllDevTypeReportParamName[i];
		}
	}
	return str;
}

const int gAmThreePhaseReportParamNum = AM_THREE_PHASE_REPORT_PARAM_NUM;
const int gAmThreePhaseReportParamValue[AM_THREE_PHASE_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_METER_STATE,
	E_REPORT_ID_THRESHOLD_STATE,
	E_REPORT_ID_VOLT_CHANGE ,
	E_REPORT_ID_CURR_CHANGE,
	E_REPORT_ID_FREQUENCY,
	E_REPORT_ID_A_VOLT,
	E_REPORT_ID_B_VOLT,
	E_REPORT_ID_C_VOLT,
	E_REPORT_ID_AB_VOLT,
	E_REPORT_ID_BC_VOLT,
	E_REPORT_ID_CA_VOLT,
	E_REPORT_ID_A_CURRENT,
	E_REPORT_ID_B_CURRENT,
	E_REPORT_ID_C_CURRENT,
	E_REPORT_ID_ACTIVE,
	E_REPORT_ID_ACTIVE_A,
	E_REPORT_ID_ACTIVE_B,
	E_REPORT_ID_ACTIVE_C,
	E_REPORT_ID_REACTIVE,
	E_REPORT_ID_REACTIVE_A,
	E_REPORT_ID_REACTIVE_B,
	E_REPORT_ID_REACTIVE_C,
	E_REPORT_ID_APPARENT,
	E_REPORT_ID_APPARENT_A,
	E_REPORT_ID_APPARENT_B,
	E_REPORT_ID_APPARENT_C,
	E_REPORT_ID_FACTOR,
	E_REPORT_ID_FACTOR_A,
	E_REPORT_ID_FACTOR_B,
	E_REPORT_ID_FACTOR_C,
	E_REPORT_ID_N_CURRENT,
	E_REPORT_ID_T_COMBINE,
	E_REPORT_ID_T_COMBINE_RATE1,
	E_REPORT_ID_T_COMBINE_RATE2,
	E_REPORT_ID_T_COMBINE_RATE3,
	E_REPORT_ID_T_COMBINE_RATE4,
	E_REPORT_ID_T_POSITIVE,
	E_REPORT_ID_T_POSITIVE_RATE1,
	E_REPORT_ID_T_POSITIVE_RATE2,
	E_REPORT_ID_T_POSITIVE_RATE3,
	E_REPORT_ID_T_POSITIVE_RATE4,
	E_REPORT_ID_T_REVERSE,
	E_REPORT_ID_T_REVERSE_RATE1,
	E_REPORT_ID_T_REVERSE_RATE2,
	E_REPORT_ID_T_REVERSE_RATE3,
	E_REPORT_ID_T_REVERSE_RATE4,
	E_REPORT_ID_COMBINE1,
	E_REPORT_ID_T_COMBINE1_RATE1,
	E_REPORT_ID_T_COMBINE1_RATE2,
	E_REPORT_ID_T_COMBINE1_RATE3,
	E_REPORT_ID_T_COMBINE1_RATE4,
	E_REPORT_ID_COMBINE2,
	E_REPORT_ID_T_COMBINE2_RATE1,
	E_REPORT_ID_T_COMBINE2_RATE2,
	E_REPORT_ID_T_COMBINE2_RATE3,
	E_REPORT_ID_T_COMBINE2_RATE4,
};

const int gAmOnePhaseReportParamNum = AM_ONE_PHASE_REPORT_PARAM_NUM;
const int gAmOnePhaseReportParamValue[AM_ONE_PHASE_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_METER_STATE,
	E_REPORT_ID_THRESHOLD_STATE,
	E_REPORT_ID_VOLT_CHANGE,
	E_REPORT_ID_CURR_CHANGE,
	E_REPORT_ID_FREQUENCY,
	E_REPORT_ID_VOLT,
	E_REPORT_ID_CURRENT,
	E_REPORT_ID_ACTIVE,
	E_REPORT_ID_REACTIVE,
	E_REPORT_ID_APPARENT,
	E_REPORT_ID_FACTOR,
	E_REPORT_ID_T_COMBINE,
	E_REPORT_ID_T_COMBINE_RATE1,
	E_REPORT_ID_T_COMBINE_RATE2,
	E_REPORT_ID_T_COMBINE_RATE3,
	E_REPORT_ID_T_COMBINE_RATE4,
	E_REPORT_ID_T_POSITIVE,
	E_REPORT_ID_T_POSITIVE_RATE1,
	E_REPORT_ID_T_POSITIVE_RATE2,
	E_REPORT_ID_T_POSITIVE_RATE3,
	E_REPORT_ID_T_POSITIVE_RATE4,
	E_REPORT_ID_T_REVERSE,
	E_REPORT_ID_T_REVERSE_RATE1,
	E_REPORT_ID_T_REVERSE_RATE2,
	E_REPORT_ID_T_REVERSE_RATE3,
	E_REPORT_ID_T_REVERSE_RATE4,
	E_REPORT_ID_COMBINE1,
	E_REPORT_ID_T_COMBINE1_RATE1,
	E_REPORT_ID_T_COMBINE1_RATE2,
	E_REPORT_ID_T_COMBINE1_RATE3,
	E_REPORT_ID_T_COMBINE1_RATE4,
	E_REPORT_ID_COMBINE2,
	E_REPORT_ID_T_COMBINE2_RATE1,
	E_REPORT_ID_T_COMBINE2_RATE2,
	E_REPORT_ID_T_COMBINE2_RATE3,
	E_REPORT_ID_T_COMBINE2_RATE4
};

const int gAmDcReportParamNum = AM_DC_REPORT_PARAM_NUM ;
const int gAmDcReportParamValue[AM_DC_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_METER_STATE,
	E_REPORT_ID_VOLT_CHANGE,
	E_REPORT_ID_CURR_CHANGE,
	E_REPORT_ID_VOLT ,
	E_REPORT_ID_CURRENT ,
	E_REPORT_ID_ACTIVE  ,
	E_REPORT_ID_T_COMBINE 
};

const int gAmHaramReportParamNum = AM_HARM_REPORT_PARAM_NUM ;
const int gAmHaramReportParamValue[AM_HARM_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_THRESHOLD_STATE,
	E_REPORT_ID_METER_STATE,
	E_REPORT_ID_VOLT_CHANGE,
	E_REPORT_ID_CURR_CHANGE,
	E_REPORT_ID_FREQUENCY,
	E_REPORT_ID_A_VOLT   ,
	E_REPORT_ID_B_VOLT   ,
	E_REPORT_ID_C_VOLT   ,
	E_REPORT_ID_AB_VOLT  ,
	E_REPORT_ID_BC_VOLT  ,
	E_REPORT_ID_CA_VOLT  ,
	E_REPORT_ID_A_CURRENT,
	E_REPORT_ID_B_CURRENT,
	E_REPORT_ID_C_CURRENT,
	E_REPORT_ID_ACTIVE   ,
	E_REPORT_ID_ACTIVE_A ,
	E_REPORT_ID_ACTIVE_B ,
	E_REPORT_ID_ACTIVE_C ,
	E_REPORT_ID_REACTIVE  ,
	E_REPORT_ID_REACTIVE_A,
	E_REPORT_ID_REACTIVE_B,
	E_REPORT_ID_REACTIVE_C,
	E_REPORT_ID_APPARENT  ,
	E_REPORT_ID_APPARENT_A,
	E_REPORT_ID_APPARENT_B,
	E_REPORT_ID_APPARENT_C,
	E_REPORT_ID_FACTOR   ,
	E_REPORT_ID_FACTOR_A ,
	E_REPORT_ID_FACTOR_B ,
	E_REPORT_ID_FACTOR_C ,
	E_REPORT_ID_N_CURRENT,
	E_REPORT_ID_T_COMBINE  ,
	E_REPORT_ID_T_COMBINE_RATE1,
	E_REPORT_ID_T_COMBINE_RATE2,
	E_REPORT_ID_T_COMBINE_RATE3,
	E_REPORT_ID_T_COMBINE_RATE4,
	E_REPORT_ID_T_POSITIVE ,
	E_REPORT_ID_T_POSITIVE_RATE1,
	E_REPORT_ID_T_POSITIVE_RATE2,
	E_REPORT_ID_T_POSITIVE_RATE3,
	E_REPORT_ID_T_POSITIVE_RATE4,
	E_REPORT_ID_T_REVERSE  ,
	E_REPORT_ID_T_REVERSE_RATE1,
	E_REPORT_ID_T_REVERSE_RATE2,
	E_REPORT_ID_T_REVERSE_RATE3,
	E_REPORT_ID_T_REVERSE_RATE4,
	E_REPORT_ID_COMBINE1 ,
	E_REPORT_ID_T_COMBINE1_RATE1,
	E_REPORT_ID_T_COMBINE1_RATE2,
	E_REPORT_ID_T_COMBINE1_RATE3,
	E_REPORT_ID_T_COMBINE1_RATE4,
	E_REPORT_ID_COMBINE2 ,
	E_REPORT_ID_T_COMBINE2_RATE1,
	E_REPORT_ID_T_COMBINE2_RATE2,
	E_REPORT_ID_T_COMBINE2_RATE3,
	E_REPORT_ID_T_COMBINE2_RATE4,
	E_REPORT_ID_ORIGINAL_VOLTAGE_A ,
	E_REPORT_ID_ORIGINAL_VOLTAGE_B ,
	E_REPORT_ID_ORIGINAL_VOLTAGE_C ,
	E_REPORT_ID_ORIGINAL_CURRENT_A ,
	E_REPORT_ID_ORIGINAL_CURRENT_B ,
	E_REPORT_ID_ORIGINAL_CURRENT_C ,
	E_REPORT_ID_ORIGINAL_ACTIVE_T ,
	E_REPORT_ID_ORIGINAL_ACTIVE_A ,
	E_REPORT_ID_ORIGINAL_ACTIVE_B ,
	E_REPORT_ID_ORIGINAL_ACTIVE_C ,
	E_REPORT_ID_HARMONIC_VOLTAGE_A,
	E_REPORT_ID_HARMONIC_VOLTAGE_B,
	E_REPORT_ID_HARMONIC_VOLTAGE_C,
	E_REPORT_ID_HARMONIC_CURRENT_A,
	E_REPORT_ID_HARMONIC_CURRENT_B,
	E_REPORT_ID_HARMONIC_CURRENT_C,
	E_REPORT_ID_HARMONIC_ACTIVE_T ,
	E_REPORT_ID_HARMONIC_ACTIVE_A ,
	E_REPORT_ID_HARMONIC_ACTIVE_B ,
	E_REPORT_ID_HARMONIC_ACTIVE_C ,
	E_REPORT_ID_THD_UA ,
	E_REPORT_ID_THD_UB ,
	E_REPORT_ID_THD_UC ,
	E_REPORT_ID_THD_UT ,
	E_REPORT_ID_THD_IA ,
	E_REPORT_ID_THD_IB ,
	E_REPORT_ID_THD_IC ,
	E_REPORT_ID_THD_IT ,
	E_REPORT_ID_HARMONIC_UA_2  ,
	E_REPORT_ID_HARMONIC_UA_3  ,
	E_REPORT_ID_HARMONIC_UA_4  ,
	E_REPORT_ID_HARMONIC_UA_5  ,
	E_REPORT_ID_HARMONIC_UA_6  ,
	E_REPORT_ID_HARMONIC_UA_7  ,
	E_REPORT_ID_HARMONIC_UA_8  ,
	E_REPORT_ID_HARMONIC_UA_9  ,
	E_REPORT_ID_HARMONIC_UA_10 ,
	E_REPORT_ID_HARMONIC_UA_11 ,
	E_REPORT_ID_HARMONIC_UA_12 ,
	E_REPORT_ID_HARMONIC_UA_13 ,
	E_REPORT_ID_HARMONIC_UA_14 ,
	E_REPORT_ID_HARMONIC_UA_15 ,
	E_REPORT_ID_HARMONIC_UA_16 ,
	E_REPORT_ID_HARMONIC_UA_17 ,
	E_REPORT_ID_HARMONIC_UA_18 ,
	E_REPORT_ID_HARMONIC_UA_19 ,
	E_REPORT_ID_HARMONIC_UA_20 ,
	E_REPORT_ID_HARMONIC_UA_21 ,
	E_REPORT_ID_HARMONIC_UA_ODD,
	E_REPORT_ID_HARMONIC_UA_EVEN,
	E_REPORT_ID_HARMONIC_UA_RIDGE,
	E_REPORT_ID_HARMONIC_UA_TETL,
	E_REPORT_ID_HARMONIC_UB_2  ,
	E_REPORT_ID_HARMONIC_UB_3  ,
	E_REPORT_ID_HARMONIC_UB_4  ,
	E_REPORT_ID_HARMONIC_UB_5  ,
	E_REPORT_ID_HARMONIC_UB_6  ,
	E_REPORT_ID_HARMONIC_UB_7  ,
	E_REPORT_ID_HARMONIC_UB_8  ,
	E_REPORT_ID_HARMONIC_UB_9  ,
	E_REPORT_ID_HARMONIC_UB_10 ,
	E_REPORT_ID_HARMONIC_UB_11 ,
	E_REPORT_ID_HARMONIC_UB_12 ,
	E_REPORT_ID_HARMONIC_UB_13 ,
	E_REPORT_ID_HARMONIC_UB_14 ,
	E_REPORT_ID_HARMONIC_UB_15 ,
	E_REPORT_ID_HARMONIC_UB_16 ,
	E_REPORT_ID_HARMONIC_UB_17 ,
	E_REPORT_ID_HARMONIC_UB_18 ,
	E_REPORT_ID_HARMONIC_UB_19 ,
	E_REPORT_ID_HARMONIC_UB_20 ,
	E_REPORT_ID_HARMONIC_UB_21 ,
	E_REPORT_ID_HARMONIC_UB_ODD,
	E_REPORT_ID_HARMONIC_UB_EVEN,
	E_REPORT_ID_HARMONIC_UB_RIDGE,
	E_REPORT_ID_HARMONIC_UB_TETL,
	E_REPORT_ID_HARMONIC_UC_2  ,
	E_REPORT_ID_HARMONIC_UC_3  ,
	E_REPORT_ID_HARMONIC_UC_4  ,
	E_REPORT_ID_HARMONIC_UC_5  ,
	E_REPORT_ID_HARMONIC_UC_6  ,
	E_REPORT_ID_HARMONIC_UC_7  ,
	E_REPORT_ID_HARMONIC_UC_8  ,
	E_REPORT_ID_HARMONIC_UC_9  ,
	E_REPORT_ID_HARMONIC_UC_10 ,
	E_REPORT_ID_HARMONIC_UC_11 ,
	E_REPORT_ID_HARMONIC_UC_12 ,
	E_REPORT_ID_HARMONIC_UC_13 ,
	E_REPORT_ID_HARMONIC_UC_14 ,
	E_REPORT_ID_HARMONIC_UC_15 ,
	E_REPORT_ID_HARMONIC_UC_16 ,
	E_REPORT_ID_HARMONIC_UC_17 ,
	E_REPORT_ID_HARMONIC_UC_18 ,
	E_REPORT_ID_HARMONIC_UC_19 ,
	E_REPORT_ID_HARMONIC_UC_20 ,
	E_REPORT_ID_HARMONIC_UC_21 ,
	E_REPORT_ID_HARMONIC_UC_ODD,
	E_REPORT_ID_HARMONIC_UC_EVEN,
	E_REPORT_ID_HARMONIC_UC_RIDGE,
	E_REPORT_ID_HARMONIC_UC_TETL,
	E_REPORT_ID_HARMONIC_IA_2  ,
	E_REPORT_ID_HARMONIC_IA_3  ,
	E_REPORT_ID_HARMONIC_IA_4  ,
	E_REPORT_ID_HARMONIC_IA_5  ,
	E_REPORT_ID_HARMONIC_IA_6  ,
	E_REPORT_ID_HARMONIC_IA_7  ,
	E_REPORT_ID_HARMONIC_IA_8  ,
	E_REPORT_ID_HARMONIC_IA_9  ,
	E_REPORT_ID_HARMONIC_IA_10 ,
	E_REPORT_ID_HARMONIC_IA_11 ,
	E_REPORT_ID_HARMONIC_IA_12 ,
	E_REPORT_ID_HARMONIC_IA_13 ,
	E_REPORT_ID_HARMONIC_IA_14 ,
	E_REPORT_ID_HARMONIC_IA_15 ,
	E_REPORT_ID_HARMONIC_IA_16 ,
	E_REPORT_ID_HARMONIC_IA_17 ,
	E_REPORT_ID_HARMONIC_IA_18 ,
	E_REPORT_ID_HARMONIC_IA_19 ,
	E_REPORT_ID_HARMONIC_IA_20 ,
	E_REPORT_ID_HARMONIC_IA_21 ,
	E_REPORT_ID_HARMONIC_IA_ODD ,
	E_REPORT_ID_HARMONIC_IA_EVEN,
	E_REPORT_ID_HARMONIC_IA_K   ,
	E_REPORT_ID_HARMONIC_IB_2  ,
	E_REPORT_ID_HARMONIC_IB_3  ,
	E_REPORT_ID_HARMONIC_IB_4  ,
	E_REPORT_ID_HARMONIC_IB_5  ,
	E_REPORT_ID_HARMONIC_IB_6  ,
	E_REPORT_ID_HARMONIC_IB_7  ,
	E_REPORT_ID_HARMONIC_IB_8  ,
	E_REPORT_ID_HARMONIC_IB_9  ,
	E_REPORT_ID_HARMONIC_IB_10 ,
	E_REPORT_ID_HARMONIC_IB_11 ,
	E_REPORT_ID_HARMONIC_IB_12 ,
	E_REPORT_ID_HARMONIC_IB_13 ,
	E_REPORT_ID_HARMONIC_IB_14 ,
	E_REPORT_ID_HARMONIC_IB_15 ,
	E_REPORT_ID_HARMONIC_IB_16 ,
	E_REPORT_ID_HARMONIC_IB_17 ,
	E_REPORT_ID_HARMONIC_IB_18 ,
	E_REPORT_ID_HARMONIC_IB_19 ,
	E_REPORT_ID_HARMONIC_IB_20 ,
	E_REPORT_ID_HARMONIC_IB_21 ,
	E_REPORT_ID_HARMONIC_IB_ODD ,
	E_REPORT_ID_HARMONIC_IB_EVEN,
	E_REPORT_ID_HARMONIC_IB_K   ,
	E_REPORT_ID_HARMONIC_IC_2  ,
	E_REPORT_ID_HARMONIC_IC_3  ,
	E_REPORT_ID_HARMONIC_IC_4  ,
	E_REPORT_ID_HARMONIC_IC_5  ,
	E_REPORT_ID_HARMONIC_IC_6  ,
	E_REPORT_ID_HARMONIC_IC_7  ,
	E_REPORT_ID_HARMONIC_IC_8  ,
	E_REPORT_ID_HARMONIC_IC_9  ,
	E_REPORT_ID_HARMONIC_IC_10 ,
	E_REPORT_ID_HARMONIC_IC_11 ,
	E_REPORT_ID_HARMONIC_IC_12 ,
	E_REPORT_ID_HARMONIC_IC_13 ,
	E_REPORT_ID_HARMONIC_IC_14 ,
	E_REPORT_ID_HARMONIC_IC_15 ,
	E_REPORT_ID_HARMONIC_IC_16 ,
	E_REPORT_ID_HARMONIC_IC_17 ,
	E_REPORT_ID_HARMONIC_IC_18 ,
	E_REPORT_ID_HARMONIC_IC_19 ,
	E_REPORT_ID_HARMONIC_IC_20 ,
	E_REPORT_ID_HARMONIC_IC_21 ,
	E_REPORT_ID_HARMONIC_IC_ODD ,
	E_REPORT_ID_HARMONIC_IC_EVEN,
	E_REPORT_ID_HARMONIC_IC_K   
};

const int gWaterGasMeterReportParamNum = WATER_GASS_METER_REPORT_PARAM_NUM;
const int gWaterGasMeterReportParamValue[WATER_GASS_METER_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_METER_STATE,
	E_REPORT_ID_CUR_CUMULA_FLOW,//��ǰ�ۼ�����
	E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA,//�������ۼ�����
};

const int gHotMeterReportParamNum = HOT_METER_REPORT_PARAM_NUM;
const int gHotMeterReportParamValue[HOT_METER_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_METER_STATE,
	E_REPORT_ID_HEAT_SETTLE_DATA,//����������
	E_REPORT_ID_CUR_HEAT,//��ǰ����
	E_REPORT_ID_THERMAL_POWER,//�ȹ��� 
	E_REPORT_ID_FLOW,//����
	E_REPORT_ID_CUMULA_FLOW,//�ۼ�����
	E_REPORT_ID_SUPPLYWATER_TEMP,//��ˮ�¶�
	E_REPORT_ID_BACKWATER_TEMP,//��ˮ�¶�
	E_REPORT_ID_CUMULA_TIME,//�ۼƹ���ʱ��
};

const int gEopReportParamNum = EOP_METER_REPORT_PARAM_NUM;
const int gEopReportParamValue[EOP_METER_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_E_OP_FAULT,
	E_REPORT_ID_ON_OFF_STATE,
	E_REPORT_ID_ON_COUNT,
	E_REPORT_ID_ON_TIMER,
	E_REPORT_ID_ON_THINK_TIME,
};

int GetMapSecondByFirst( int first, MAP_INT_INT map_val )
{
	map<int, int>::iterator  iter;
	for(iter = map_val.begin(); iter != map_val.end(); iter++)
	{
		if (iter->first == first)
		{
			return iter->second;
		}
	}
	return -1;
}

const int  gDefProtUnitNum = DEF_PROT_UNIT_NUM;
const char gDefProtUnitName[DEF_PROT_UNIT_NUM][64] =
{
	"1",
	"0.1",
	"0.01",
	"0.001"	
};
const double gDefProtUnitValue[DEF_PROT_UNIT_NUM] =
{
	1,
	0.1,
	0.01,
	0.001
};

int GetSubTypeIndByValue( int type )
{
	int ind = -1;
	for (int i = 0; i < gSubTypeNum; i++)
	{
		if (gSubTypeValue[i] == type)
		{
			ind = i;
		}
	}
	return ind;
}

void SetSubTypeNameByInd( int sub_type_ind, char* sub_name )
{
	//return;
	assert(sub_type_ind < gSubTypeNum);
	assert(strlen(sub_name) < 64);
	if (sub_type_ind >= 0)
	{
		strcpy(gSubTypeName[sub_type_ind], sub_name);
	}
}

void SetWayCountBySubTypeInd( int sub_type_ind, int way_count )
{
	assert(sub_type_ind < gSubTypeNum);
	assert(way_count <= MAX_WAY_NUM);
	if (sub_type_ind >= 0)
	{
		gWayCountValue[sub_type_ind] = way_count;
	}
}

void ClearDefSubTypeNameWayCount()
{
	for (int i = 0; i < gSubTypeNum; i++)
	{
		if (IsDefSubType(gSubTypeValue[i]) == true)
		{
			char tail[64];
			int ind = gSubTypeValue[i] % 100 % 11 + 1;
			sprintf(tail,"�Զ���������%d", ind);
			string str = GetDevTypeNameByValue(gSubTypeValue[i]/100); 
			str += tail;
			SetSubTypeNameByInd(i, (char*)str.c_str());
			SetWayCountBySubTypeInd(i, 0);
		}
	}
}

bool IsDefSubType( int sub_type )
{
	bool is_def = false;
	int dev_type = sub_type / 100;
	if((sub_type % 100 >= 11) &&
		(sub_type % 100 <= 14) &&
		(dev_type == AM_3PHASE || 
		dev_type == AM_1PHASE || 
		dev_type == AM_DC || 
		dev_type == AM_HARM)
		)
	{
		is_def = true;
	}
	return is_def;
}

int MapUnitDoubleToInt( double val )
{
	//unit��ӳ���ϵ {0->0.001; 1->0.01; 2->0.1; 3->1; 4->10; 5->100; 6->1000}
	int ret = 0;
	if (val >= (0.001 - 0.0001) && 
		val <= (0.001 + 0.0001) )
	{
		ret = 0;
	}
	else if (val >= (0.01 - 0.001) &&
		val <= (0.01 + 0.001) )
	{
		ret = 1;
	}
	else if (val >= (0.1 - 0.01) &&
		val <= (0.1 + 0.01) )
	{
		ret = 2;
	}
	else if (val >= (1 - 0.1) &&
		val <= (1 + 0.1) )
	{
		ret = 3;
	}
	else if (val >= (10 - 1) &&
		val <= (10 + 1) )
	{
		ret = 4;
	}
	else if (val >= (100 - 10) &&
		val <= (100 + 10) )
	{
		ret = 5;
	}
	else if (val >= (1000 - 100) &&
		val <= (1000 + 100) )
	{
		ret = 6;
	}
	return ret;
}

double GetValueByNameDefProtUnit( string name )
{
	double ret = 0;
	for (int i = 0; i < gDefProtUnitNum; i++)
	{
		if (0 == strcmp(gDefProtUnitName[i], name.c_str()))
		{
			ret = gDefProtUnitValue[i];
		}
	}
	return ret;
}

const double gAllDevTypeReportParamUnit[ALL_DEVTYPE_REPORT_PARAM_NUM] = 
{
	0.01,
	0.01,
	0.01,
	0.01,
	0.1,
	0.01,
	1,
	1,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.1,
	0.1,
	0.1,
	0.1,
	0.1,
	0.1,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	1,
	0.1,
	0.1,
	0.1,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.1,
	0.1,
	0.1,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.001,
	0.001,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.001,
	0.001,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.001,
	0.001,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.001,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.001,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.001,//210
	0.01,//211
	0.01,
	0.01,
	0.001,
	0.001,
	0.01,
	0.01,
	1,//218
	0.01,//231
	0.01,//232
	1,//245
	1,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	0.01,
	1,
	1,
	1,
	1,
	1//271
};

std::string GetNameByValueDefProtUnit( double val)
{
	string ret = gDefProtUnitName[0];
	for (int i = 0; i < gDefProtUnitNum; i++)
	{
		if (val >= (gDefProtUnitValue[i] - gDefProtUnitValue[i] * 0.1) &&
			val <= (gDefProtUnitValue[i] + gDefProtUnitValue[i] * 0.1))
		{
			ret = gDefProtUnitName[i];
		}
	}
	return ret;
}

double MapUnitIntToDouble( int val )
{
	//unit��ӳ���ϵ {0->0.001; 1->0.01; 2->0.1; 3->1; 4->10; 5->100; 6->1000}
	double ret = 0.0;
	if (val == 0)
	{
		ret = 0.001;
	}
	else if (val == 1)
	{
		ret = 0.01;
	}
	else if (val == 2)
	{
		ret = 0.1;
	}
	else if (val == 3)
	{
		ret = 1;
	}
	else if (val == 4)
	{
		ret = 10;
	}
	else if (val == 5)
	{
		ret = 100;
	}
	else if (val == 6)
	{
		ret = 1000;
	}
	return ret;
}

CString BootOpenFile( bool open /*= TRUE*/ )
{
	CString str_file = _T("");
	//CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.cfg)|*.cfg|All Files (*.*)|*.*||"), NULL);
	CFileDialog dlgFile(open, NULL, NULL, OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		str_file = dlgFile.GetPathName();
	}
	return str_file;
}

CString BootOpenFolder()
{
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(bi));
	//bi.hwndOwner = m_hWnd;   
	//bi.pidlRoot = NULL;   
	//bi.pszDisplayName = szPath;   
	bi.lpszTitle = "��ѡ�񱣴���ʷ���ݵ�Ŀ¼��";   
	//bi.ulFlags = 0;   
	//bi.lpfn = NULL;   
	//bi.lParam = 0;   
	//bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		str.Format("%s",  szPath);
	}
	else
	{
		str.Format("");
	}

	return str;
}

int GetAllDevTypeReportParamIndByValue( int val )
{
	int ret = 0;
	for (int i = 0; i < gAllDevTypeReportParamNum; i++)
	{
		if (gAllDevTypeReportParamValue[i] == val)
		{
			ret = i;
			return ret;
		}
	}
	return ret;
}

int GetWayCountBySubTypeInd( int sub_type_ind )
{
	assert(sub_type_ind < gSubTypeNum);
	return gWayCountValue[sub_type_ind];
}

int GetBaudIndByValue( int type )
{
	int ret = -1;
	for (int i = 0; i < gBaudNum; i++)
	{
		if (gBaudValue[i] == type)
		{
			ret = i;
		}
	}
	return ret;
}

int GetStopIndByValue( int type )
{
	int ret = -1;
	for (int i = 0; i < gDataBitNum; i++)
	{
		if (gStopValue[i] == type)
		{
			ret = i;
		}
	}
	return ret;
}

int GetParityIndByValue( int type )
{
	int ret = -1;
	for (int i = 0; i < gParityNum; i++)
	{
		if (gParityValue[i] == type)
		{
			ret = i;
		}
	}
	return ret;
}

int GetDataBitIndByValue( int type )
{
	int ret = -1;
	for (int i = 0; i < gDataBitNum; i++)
	{
		if (gDataBitValue[i] == type)
		{
			ret = i;
		}
	}
	return ret;
}

int ModifyReportParamUnitName( CString unit_name, int val )
{
	int ret = 0;
	if (
		val ==  E_REPORT_ID_CUR_CUMULA_FLOW  ||//��ǰ�ۼ�����
		val ==  E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA  ||//�������ۼ�����
		val ==  E_REPORT_ID_HEAT_SETTLE_DATA ||//����������
		val ==  E_REPORT_ID_CUR_HEAT  ||//��ǰ����
		val ==  E_REPORT_ID_THERMAL_POWER ||//�ȹ���
		val ==  E_REPORT_ID_FLOW ||//����
		val ==  E_REPORT_ID_CUMULA_FLOW//�ۼ�����
		)
	{
		int param_ind = GetAllDevTypeReportParamIndByValue(val);
		// 123/unitOld -> 123/unitNew
		CString param_name = gAllDevTypeReportParamName[param_ind];
		CString sig_flag = "/";
		int sig_ind = param_name.Find(sig_flag);
		CString param_name_pre = param_name.Left(sig_ind+1);//ǰ׺
		CString param_now = param_name_pre + unit_name;
		sprintf(gAllDevTypeReportParamName[param_ind], "%s", param_now.GetString());
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

CString ParseUnitByValue( MU_INT8 val )
{
	CString ret = "";
	if (val == 0x02)
	{
		ret = "Wh";
	}
	else if (val == 0x05)
	{
		ret = "kWh";
	}
	else if (val == 0x08)
	{
		ret = "MWh";
	}
	else if (val == 0x0A)
	{
		ret = "MWh*100";
	}
	else if (val == 0x01)
	{
		ret = "J";
	}
	else if (val == 0x0B)
	{
		ret = "kJ";
	}
	else if (val == 0x0E)
	{
		ret = "MJ";
	}
	else if (val == 0x11)
	{
		ret = "GJ";
	}
	else if (val == 0x13)
	{
		ret = "GJ*100";
	}
	else if (val == 0x14)
	{
		ret = "W";
	}
	else if (val == 0x17)
	{
		ret = "kW";
	}
	else if (val == 0x1A)
	{
		ret = "MW";
	}
	else if (val == 0x29)
	{
		ret = "L";
	}
	else if (val == 0x2C)
	{
		ret = "m3";
	}
	else if (val == 0x32)
	{
		ret = "L/h";
	}
	else if (val == 0x35)
	{
		ret = "m3/h";
	}
	else
	{
		ret = "����";
	}
	return ret;
}

int  gDefProtWayChooseNum = MAX_WAY_NUM;
const char gDefProtWayChooseName[MAX_WAY_NUM][64] = 
{
	"��1��·������Ϣ",
	"��2��·������Ϣ",
	"��3��·������Ϣ",
	"��4��·������Ϣ",
	"��5��·������Ϣ",
	"��6��·������Ϣ",
	"��7��·������Ϣ",
	"��8��·������Ϣ",
	"��9��·������Ϣ",
	"��10��·������Ϣ",
	"��11��·������Ϣ",
	"��12��·������Ϣ"
};
const int gDefProtWayChooseValue[MAX_WAY_NUM] = 
{
	0,1,2,3,4,5,6,7,8,9,10,11
};

int GetDefProtWayChooseValueByName( CString name )
{
	int ret = -1;
	for (int i = 0; i < gDefProtWayChooseNum; i++)
	{
		if (gDefProtWayChooseName[i] == name)
		{
			ret = gDefProtWayChooseValue[i];
		}
	}
	return ret;
}

CString GetDefProtWayChooseNameByValue( int val )
{
	CString ret = "";
	for (int i = 0; i < gDefProtWayChooseNum; i++)
	{
		if (gDefProtWayChooseValue[i] == val)
		{
			ret = gDefProtWayChooseName[i];
		}
	}
	return ret;
}

int GetDefProtWayChooseIndByValue( int val )
{
	int ret = -1;
	for (int i = 0; i < gDefProtWayChooseNum; i++)
	{
		if (gDefProtWayChooseValue[i] == val)
		{
			ret = i;
		}
	}
	return ret;
}

void ModifyDefProtWayChooseNum( int val )
{
	if (val == 0)
	{
		val = MAX_WAY_NUM;
	}
	if (val > 0 && val <= MAX_WAY_NUM)
	{
		gDefProtWayChooseNum = val;
	}

}

int GetModBusArrIndBySubType( int sub_type )
{
	int modbus_ind = -1;
	bool is_def = IsDefSubType(sub_type);
	if (is_def)
	{
		//111-114;211-214;311-314;411-414;
		int dev_type = sub_type / 100;
		switch(dev_type)
		{
		case AM_3PHASE:
			modbus_ind = sub_type - AM_3PHASE_DEF_01;
			break;
		case AM_1PHASE:
			modbus_ind = sub_type - AM_1PHASE_DEF_01;
			break;
		case AM_DC:
			modbus_ind = sub_type - AM_DC_DEF_01;
			break;
		case AM_HARM:
			modbus_ind = sub_type - AM_HARM_DEF_01;
			break;
		default:
			modbus_ind = -1;
			break;
		}
	}
	else
	{
		modbus_ind = -1;
	}
	return modbus_ind;
}

const int gColtorTypeNum = COLTOR_TYPE_NUM ;
const char gColtorTypeName[COLTOR_TYPE_NUM][64] =
{
	"�º격","�Ϻ�"
};

const int gColtorTypeValue[COLTOR_TYPE_NUM] = 
{
	0,1
};

int GetColtorTypeValueByName(string name)
{
	for (int i = 0; i < gColtorTypeNum; i++)
	{
		if (0 == strcmp(gColtorTypeName[i], name.c_str()))
		{
			return gColtorTypeValue[i];
		}
	}
	return -1;
}

string GetColtorTypeNameByValue(int value)
{
	string str = "";
	for (int i = 0; i < gColtorTypeNum; i++)
	{
		if (gColtorTypeValue[i] == value)
		{
			str = gColtorTypeName[i];
		}
	}
	return str;
}

int GetColtorTypeIndByValue( int value )
{
	for (int i = 0; i < gColtorTypeNum; i++)
	{
		if (gColtorTypeValue[i] == value)
		{
			return i;
		}
	}
	return -1;
}
