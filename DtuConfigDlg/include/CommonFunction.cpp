#include "stdafx.h"
#include "CommonFunction.h"
#include "DataTypeDef.h"

const int  gDevTypeNum = DEV_TYPE_NUM;
const char gDevTypeName[DEV_TYPE_NUM][64] = 
{
	"AM-3PHASE(3相电表)",
	"AM-1PHASE(1相电表)", 
	"AM-DC(直流电表)",   
	"AM-HARM(谐波电表)",
	"E-OP(智能电操)",
	"WM(水表)",
	"HM(热量表)",
	"GM(汽表)"
};

const int gDevTypeValue[DEV_TYPE_NUM] = 
{
	AM_3PHASE ,		//1:AM-3PHASE(3相电表);  
	AM_1PHASE,			//2:AM-1PHASE(1相电表); 
	AM_DC ,				//3:AM-DC(直流电表);   
	AM_HARM	,			//4:AM-HARM(谐波电表);  
	E_OP ,				//5:E-OP(智能电操);
	WM,					//6:WM(水表);
	HM,					//7:HM(热量表)
	GM					//7:GM(汽表);
};
const int  gSubTypeNum = SUB_TYPE_NUM;
char gSubTypeName[SUB_TYPE_NUM][64] = 
{
	"DLT645-2007-3相-1路-交流-电表","XHB-modbus.Vc-3相-4路-交流-电表","XHB-modbus.Vb-3相-1路-交流-电表","XHB-modbus.Va-3相-1路-交流-电表-电力分析仪","DLT645-1997-3相-1路-交流-电表",
	"AM-3PHASE(3相电表)自定义子类型1","AM-3PHASE(3相电表)自定义子类型2","AM-3PHASE(3相电表)自定义子类型3","AM-3PHASE(3相电表)自定义子类型4",
	"XHB-07-1相-1路-交流-电表","XHB-modbus.Vc-1相-12路-交流-电表","XHB-modbus.Vb-1相-3路-交流-电表","XHB-modbus.Va-1相-3路-交流-电表",
	"AM-1PHASE(1相电表)自定义子类型1","AM-1PHASE(1相电表)自定义子类型2","AM-1PHASE(1相电表)自定义子类型3","AM-1PHASE(1相电表)自定义子类型4",
	"XHB-07-12路-直流-电表","XHB-modbus.vb-1路-直流-电表","XHB-modbus.vb-2路-直流-电表",
	"AM-DC(直流电表)自定义子类型1","AM-DC(直流电表)自定义子类型2","AM-DC(直流电表)自定义子类型3","AM-DC(直流电表)自定义子类型4",
	"XHB-modbus.Va-3相-1路-谐波-电表",
	"AM-HARM(谐波电表)自定义子类型1","AM-HARM(谐波电表)自定义子类型2","AM-HARM(谐波电表)自定义子类型3","AM-HARM(谐波电表)自定义子类型4",
	"XHB-1路-智能电操",
	"CJT188-2004-冷水水表", "CJT188-2004-生活热水水表", "CJT188-2004-直饮水水表", "CJT188-2004-中水水表",
	"CJT188-2004-热气表-计热量","CJT188-2004-热气表-计冷量",   
	"CJT188-2004-燃气表"
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
{"无","奇","偶"};// 0:无; 1:奇; 2:偶;
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
0x00 正常
0x01 计量芯片异常
0x02 参数存储区异常
0x04 冻结数据存储区异常
0x08 电压相序出错
0x10 电流相序出错
0x20 失压
0x40 失流
*/
const char gErrName[ERR_NAME_NUM][64] =
{
	"计量芯片异常",
	"参数存储区异常",
	"冻结数据存储区异常",
	"电压相序出错",
	"电流相序出错",
	"失压",
	"失流"
};

const char gAmHarmName[AM_HARM_NUM][64] = 
{
	"电表时间",                      
	"电表状态",
	"阀值告警状态",
	"电压变比",
	"电流变比",                        
	"频率/Hz",                             
	"A相电压/V",                                
	"B相电压/V",                                
	"C相电压/V",                                
	"AB线电压/V",                               
	"BC线电压/V",                               
	"CA线电压/V",                               
	"A相电流/A",                             
	"B相电流/A",                             
	"C相电流/A",                             
	"总有功功率/kW",                                    
	"A相有功功率/kW",                                  
	"B相有功功率/kW",                                  
	"C相有功功率/kW",                                  
	"总无功功率/kVar",                                  
	"A相无功功率/kVar",                                
	"B相无功功率/kVar",                                
	"C相无功功率/kVar",                                
	"总视在功率/kVa",                                   
	"A相视在功率/kVa",                                 
	"B相视在功率/kVa",                                 
	"C相视在功率/kVa",                                 
	"总功率因数/",                          
	"A相功率因数/",                        
	"B相功率因数/",                        
	"C相功率因数/",                        
	"零线电流/A",                          
	"组合有功总电能/kWh", 
	"组合有功费率1电能/kWh",
	"组合有功费率2电能/kWh",
	"组合有功费率3电能/kWh",
	"组合有功费率4电能/kWh",
	"正向有功总电能/kWh",   
	"正向有功费率1电能/kWh",
	"正向有功费率2电能/kWh",
	"正向有功费率3电能/kWh",
	"正向有功费率4电能/kWh",
	"反向有功总电能/kWh",      
	"反向有功费率1电能/kWh",
	"反向有功费率2电能/kWh",
	"反向有功费率3电能/kWh",
	"反向有功费率4电能/kWh",
	"组合无功1总电能/kWh",    
	"组合无功1费率1电能/kWh",
	"组合无功1费率2电能/kWh",
	"组合无功1费率3电能/kWh",
	"组合无功1费率4电能/kWh",
	"组合无功2总电能/kWh",                             
	"组合无功2费率1电能/kWh",
	"组合无功2费率2电能/kWh",
	"组合无功2费率3电能/kWh",
	"组合无功2费率4电能/kWh",
	"A相基波电压/V",                           
	"B相基波电压/V",                           
	"C相基波电压/V",                           
	"A相基波电流/A",                        
	"B相基波电流/A",                        
	"C相基波电流/A",                        
	"总基波有功功率/kW",                               
	"A相基波有功功率/kW",                             
	"B相基波有功功率/kW",                             
	"C相基波有功功率/kW",                             
	"A相谐波电压/V",                           
	"B相谐波电压/V",                           
	"C相谐波电压/V",                           
	"A相谐波电流/A",                        
	"B相谐波电流/A",                        
	"C相谐波电流/A",                        
	"总谐波有功功率/kW",                               
	"A相谐波有功功率/kW",                             
	"B相谐波有功功率/kW",                             
	"C相谐波有功功率/kW",                             
	"Ua总畸变率/",                             
	"Ub总畸变率/",                             
	"Uc总畸变率/",                             
	"合相电压总畸变率/",                     
	"Ia总畸变率/",                             
	"Ib总畸变率/",                             
	"Ic总畸变率/",                             
	"合相电流总畸变率/",                  
	"UA 2次谐波/",                             
	"UA 3次谐波/",                             
	"UA 4次谐波/",                             
	"UA 5次谐波/",                             
	"UA 6次谐波/",                             
	"UA 7次谐波/",                             
	"UA 8次谐波/",                             
	"UA 9次谐波/",                             
	"UA 10次谐波/",                            
	"UA 11次谐波/",                            
	"UA 12次谐波/",                            
	"UA 13次谐波/",                            
	"UA 14次谐波/",                            
	"UA 15次谐波/",                            
	"UA 16次谐波/",                            
	"UA 17次谐波/",                            
	"UA 18次谐波/",                            
	"UA 19次谐波/",                            
	"UA 20次谐波/",                            
	"UA 21次谐波/",                            
	"UA 奇次谐波畸变率/",                           
	"UA 偶次谐波畸变率/",                           
	"UA 波峰系数",                              
	"UA 电话谐波波形因数",                               
	"UB 2次谐波/",                             
	"UB 3次谐波/",                             
	"UB 4次谐波/",                             
	"UB 5次谐波/",                             
	"UB 6次谐波/",                             
	"UB 7次谐波/",                             
	"UB 8次谐波/",                             
	"UB 9次谐波/",                             
	"UB 10次谐波/",                            
	"UB 11次谐波/",                            
	"UB 12次谐波/",                            
	"UB 13次谐波/",                            
	"UB 14次谐波/",                            
	"UB 15次谐波/",                            
	"UB 16次谐波/",                            
	"UB 17次谐波/",                            
	"UB 18次谐波/",                            
	"UB 19次谐波/",                            
	"UB 20次谐波/",                            
	"UB 21次谐波/",                            
	"UB 奇次谐波畸变率/",                           
	"UB 偶次谐波畸变率/",                           
	"UB 波峰系数",                              
	"UB 电话谐波波形因数",                               
	"UC 2次谐波/",                             
	"UC 3次谐波/",                             
	"UC 4次谐波/",                             
	"UC 5次谐波/",                             
	"UC 6次谐波/",                             
	"UC 7次谐波/",                             
	"UC 8次谐波/",                             
	"UC 9次谐波/",                             
	"UC 10次谐波/",                            
	"UC 11次谐波/",                            
	"UC 12次谐波/",                            
	"UC 13次谐波/",                            
	"UC 14次谐波/",                            
	"UC 15次谐波/",                            
	"UC 16次谐波/",                            
	"UC 17次谐波/",                            
	"UC 18次谐波/",                            
	"UC 19次谐波/",                            
	"UC 20次谐波/",                            
	"UC 21次谐波/",   	
	"UC 奇次谐波畸变率/",                           
	"UC 偶次谐波畸变率/",                           
	"UC 波峰系数",                              
	"UC 电话谐波波形因数",
	"IA 2次谐波/",                             
	"IA 3次谐波/",                             
	"IA 4次谐波/",                             
	"IA 5次谐波/",                             
	"IA 6次谐波/",                             
	"IA 7次谐波/",                             
	"IA 8次谐波/",                             
	"IA 9次谐波/",                             
	"IA 10次谐波/",                            
	"IA 11次谐波/",                            
	"IA 12次谐波/",                            
	"IA 13次谐波/",                            
	"IA 14次谐波/",                            
	"IA 15次谐波/",                            
	"IA 16次谐波/",                            
	"IA 17次谐波/",                            
	"IA 18次谐波/",                            
	"IA 19次谐波/",                            
	"IA 20次谐波/",                            
	"IA 21次谐波/",   	
	"IA 奇次谐波畸变率/",                           
	"IA 偶次谐波畸变率/",   
	"IA  K系数",
	"IB 2次谐波/",                             
	"IB 3次谐波/",                             
	"IB 4次谐波/",                             
	"IB 5次谐波/",                             
	"IB 6次谐波/",                             
	"IB 7次谐波/",                             
	"IB 8次谐波/",                             
	"IB 9次谐波/",                             
	"IB 10次谐波/",                            
	"IB 11次谐波/",                            
	"IB 12次谐波/",                            
	"IB 13次谐波/",                            
	"IB 14次谐波/",                            
	"IB 15次谐波/",                            
	"IB 16次谐波/",                            
	"IB 17次谐波/",                            
	"IB 18次谐波/",                            
	"IB 19次谐波/",                            
	"IB 20次谐波/",                            
	"IB 21次谐波/",   	
	"IB 奇次谐波畸变率/",                           
	"IB 偶次谐波畸变率/",   
	"IB  K系数",
	"IC 2次谐波/",                             
	"IC 3次谐波/",                             
	"IC 4次谐波/",                             
	"IC 5次谐波/",                             
	"IC 6次谐波/",                             
	"IC 7次谐波/",                             
	"IC 8次谐波/",                             
	"IC 9次谐波/",                             
	"IC 10次谐波/",                            
	"IC 11次谐波/",                            
	"IC 12次谐波/",                            
	"IC 13次谐波/",                            
	"IC 14次谐波/",                            
	"IC 15次谐波/",                            
	"IC 16次谐波/",                            
	"IC 17次谐波/",                            
	"IC 18次谐波/",                            
	"IC 19次谐波/",                            
	"IC 20次谐波/",                            
	"IC 21次谐波/",   	
	"IC 奇次谐波畸变率/",                           
	"IC 偶次谐波畸变率/",   
	"IC  K系数"
};

const char gAm3phaseName[AM_THREE_PHASE_NUM][64] = 
{
	"电表时间",                      
	"电表状态",
	"阀值告警状态",
	"电压变比",
	"电流变比",                        
	"频率/Hz",                             
	"A相电压/V",                                
	"B相电压/V",                                
	"C相电压/V",                                
	"AB线电压/V",                               
	"BC线电压/V",                               
	"CA线电压/V",                               
	"A相电流/A",                             
	"B相电流/A",                             
	"C相电流/A",                             
	"总有功功率/kW",                                    
	"A相有功功率/kW",                                  
	"B相有功功率/kW",                                  
	"C相有功功率/kW",                                  
	"总无功功率/kVar",                                  
	"A相无功功率/kVar",                                
	"B相无功功率/kVar",                                
	"C相无功功率/kVar",                                
	"总视在功率/kVA",                                   
	"A相视在功率/kVA",                                 
	"B相视在功率/kVA",                                 
	"C相视在功率/kVA",                                 
	"总功率因数/",                          
	"A相功率因数/",                        
	"B相功率因数/",                        
	"C相功率因数/",                        
	"零线电流/A",                          
	"组合有功总电能/kWh",
	"组合有功费率1电能/kWh",
	"组合有功费率2电能/kWh",
	"组合有功费率3电能/kWh",
	"组合有功费率4电能/kWh",
	"正向有功总电能/kWh",   
	"正向有功费率1电能/kWh",
	"正向有功费率2电能/kWh",
	"正向有功费率3电能/kWh",
	"正向有功费率4电能/kWh",
	"反向有功总电能/kWh",                               
	"反向有功费率1电能/kWh",
	"反向有功费率2电能/kWh",
	"反向有功费率3电能/kWh",
	"反向有功费率4电能/kWh",
	"组合无功1总电能/kVarh",  
	"组合无功1费率1电能/kVarh",
	"组合无功1费率2电能/kVarh",
	"组合无功1费率3电能/kVarh",
	"组合无功1费率4电能/kVarh",
	"组合无功2总电能/kVarh",
	"组合无功2费率1电能/kVarh",
	"组合无功2费率2电能/kVarh",
	"组合无功2费率3电能/kVarh",
	"组合无功2费率4电能/kVarh"
};

const char gAm1phaseName[AM_ONE_PHASE_NUM][64] = 
{
	"电表时间",                      
	"电表状态",
	"阀值告警状态",
	"电压变比",
	"电流变比",                        
	"频率/Hz",                             
	"电压/V",                                                              
	"电流/A",                                                          
	"有功功率/kW",                                                                  
	"无功功率/kVar",                                                                 
	"视在功率/kVA",                                                               
	"功率因数/",                                                                          
	"组合有功总电能/kWh", 
	"组合有功费率1电能/kWh",
	"组合有功费率2电能/kWh",
	"组合有功费率3电能/kWh",
	"组合有功费率4电能/kWh",
	"正向有功总电能/kWh",    
	"正向有功费率1电能/kWh",
	"正向有功费率2电能/kWh",
	"正向有功费率3电能/kWh",
	"正向有功费率4电能/kWh",
	"反向有功总电能/kWh",   
	"反向有功费率1电能/kWh",
	"反向有功费率2电能/kWh",
	"反向有功费率3电能/kWh",
	"反向有功费率4电能/kWh",
	"组合无功1总电能/kVarh",  
	"组合无功1费率1电能/kVarh",
	"组合无功1费率2电能/kVarh",
	"组合无功1费率3电能/kVarh",
	"组合无功1费率4电能/kVarh",
	"组合无功2总电能/kVarh",
	"组合无功2费率1电能/kVarh",
	"组合无功2费率2电能/kVarh",
	"组合无功2费率3电能/kVarh",
	"组合无功2费率4电能/kVarh"
};

const char gAmdcName[AM_DC_NUM][64] = 
{
	"电表时间",                      
	"电表状态",
	"电压变比",
	"电流变比",                        
	"电压/V",                                                              
	"电流/A",                                                          
	"功率/kW",                                                                  
	"总电能/kWh"                              
};

const char gWaterMeterName[WATER_METER_NAME_NUM][64] = 
{
	"电表时间",                      
	"电表状态",
	"当前累计流量/立方米"
};

const char gGassMeterName[GASS_METER_NAME_NUM][64] = 
{
	"电表时间",                      
	"电表状态",
	"瞬时流量/t/h",
	"频率/Hz",
	"差压/kPa",
	"压力/MPa",
	"温度/摄氏度",
	"入口温度/摄氏度",
	"出口温度/摄氏度",
	"流体密度/Kg/m3",
	"瞬时热量/Gj/h",
	"热焓/KJ/Kg",
	"热焓差/KJ/Kg",
	"累计流量/t",
	"累计热量/Gj",
	"累计热差量/Gj"
};

CString GetSaveFileNameByDevType( e_dev_type dev_type )
{
	CString str;
	switch(dev_type)
	{
	case AM_3PHASE:
		str = "AM-3PHASE(3相电表).csv";
		break;

	case AM_1PHASE:
		str = "AM-1PHASE(1相电表).csv";
		break;

	case AM_DC:
		str = "AM-DC(直流电表).csv";
		break;

	case AM_HARM:
		str = "AM-HARM(谐波电表).csv";
		break;

	case WM:
		str = "WM(水表).csv";
		break;
	case GM:
		str = "GM(汽表).csv";
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
ELECTRICITY,（电）
AIR_CONDITIONING,（空调）
LIGHTING,（照明）
POWER_SOCKET,（插座）
HARMONIC,（谐波）
TEMPERATURE,（温度）
SWITCH,（开关）
RELAY,（继电器）
RECLOSER,重合闸
APF_FILTER,有缘滤波器
WATERSTEAM ,水蒸汽


*/
const int gWayDevTypeNum = WAY_DEV_TYPE_NAME_NUM;
const char gWayDevTypeName[WAY_DEV_TYPE_NAME_NUM][64] = 
{
	"电视"
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
	"差值总电能/kWh",
	"组合有功总电能/kWh",
	"正向有功总电能/kWh",
	"反向有功总电能/kWh",
	"电压/V",
	"电流/A",
	"电压变比/",
	"电流变比/",
	"频率/Hz",
	"总有功功率/kW",
	"总无功功率/kVar",
	"总视在功率/kVA",
	"总功率因数/",
	"组合无功1总电能/kVarh",
	"组合无功2总电能/kVarh",
	"零线电流/A",
	"A相电压/V",
	"B相电压/V",
	"C相电压/V",
	"AB线电压/V",
	"BC线电压/V",
	"CA线电压/V",
	"A相电流/A",
	"B相电流/A",
	"C相电流/A",
	"A相有功功率/kW",
	"B相有功功率/kW",
	"C相有功功率/kW",
	"A相无功功率/kVar",
	"B相无功功率/kVar",
	"C相无功功率/kVar",
	"A相视在功率/kVa",
	"B相视在功率/kVa",
	"C相视在功率/kVa",
	"A相功率因数/",
	"B相功率因数/",
	"C相功率因数/",
	"A相组合有功总电能/kWh",
	"B相组合有功总电能/kWh",
	"C相组合有功总电能/kWh",
	"电表状态/",
	"A相基波电压/V",
	"B相基波电压/V",
	"C相基波电压/V",
	"A相基波电流/A",
	"B相基波电流/A",
	"C相基波电流/A",
	"总基波有功功率/kW",
	"A相基波有功功率/kW",
	"B相基波有功功率/kW",
	"C相基波有功功率/kW",
	"A相谐波电压/V",
	"B相谐波电压/V",
	"C相谐波电压/V",
	"A相谐波电流/A",
	"B相谐波电流/A",
	"C相谐波电流/A",
	"总谐波有功功率/kW",
	"A相谐波有功功率/kW",
	"B相谐波有功功率/kW",
	"C相谐波有功功率/kW",
	"Ua总畸变率/%",
	"Ub总畸变率/%",
	"Uc总畸变率/%",
	"合相电压总畸变率/%",
	"Ia总畸变率/%",
	"Ib总畸变率/%",
	"Ic总畸变率/%",
	"合相电流总畸变率/%",
	"UA 2次谐波/%",
	"UA 3次谐波/%",
	"UA 4次谐波/%",
	"UA 5次谐波/%",
	"UA 6次谐波/%",
	"UA 7次谐波/%",
	"UA 8次谐波/%",
	"UA 9次谐波/%",
	"UA 10次谐波/%",
	"UA 11次谐波/%",
	"UA 12次谐波/%",
	"UA 13次谐波/%",
	"UA 14次谐波/%",
	"UA 15次谐波/%",
	"UA 16次谐波/%",
	"UA 17次谐波/%",
	"UA 18次谐波/%",
	"UA 19次谐波/%",
	"UA 20次谐波/%",
	"UA 21次谐波/%",
	"UA 奇次谐波畸变率/%",
	"UA 偶次谐波畸变率/%",
	"UA 波峰系数/",
	"UA 电话谐波波形因数/",
	"UB 2次谐波/%",
	"UB 3次谐波/%",
	"UB 4次谐波/%",
	"UB 5次谐波/%",
	"UB 6次谐波/%",
	"UB 7次谐波/%",
	"UB 8次谐波/%",
	"UB 9次谐波/%",
	"UB 10次谐波/%",
	"UB 11次谐波/%",
	"UB 12次谐波/%",
	"UB 13次谐波/%",
	"UB 14次谐波/%",
	"UB 15次谐波/%",
	"UB 16次谐波/%",
	"UB 17次谐波/%",
	"UB 18次谐波/%",
	"UB 19次谐波/%",
	"UB 20次谐波/%",
	"UB 21次谐波/%",
	"UB 奇次谐波畸变率/%",
	"UB 偶次谐波畸变率/%",
	"UB 波峰系数/",
	"UB 电话谐波波形因数/",
	"UC 2次谐波/%",
	"UC 3次谐波/%",
	"UC 4次谐波/%",
	"UC 5次谐波/%",
	"UC 6次谐波/%",
	"UC 7次谐波/%",
	"UC 8次谐波/%",
	"UC 9次谐波/%",
	"UC 10次谐波/%",
	"UC 11次谐波/%",
	"UC 12次谐波/%",
	"UC 13次谐波/%",
	"UC 14次谐波/%",
	"UC 15次谐波/%",
	"UC 16次谐波/%",
	"UC 17次谐波/%",
	"UC 18次谐波/%",
	"UC 19次谐波/%",
	"UC 20次谐波/%",
	"UC21次谐波/%",
	"UC 奇次谐波畸变率/%",
	"UC 偶次谐波畸变率/%",
	"UC 波峰系数/",
	"UC 电话谐波波形因数/",
	"IA 2次谐波/%",
	"IA 3次谐波/%",
	"IA 4次谐波/%",
	"IA 5次谐波/%",
	"IA 6次谐波/%",
	"IA 7次谐波/%",
	"IA 8次谐波/%",
	"IA 9次谐波/%",
	"IA 10次谐波/%",
	"IA 11次谐波/%",
	"IA 12次谐波/%",
	"IA 13次谐波/%",
	"IA 14次谐波/%",
	"IA 15次谐波/%",
	"IA 16次谐波/%",
	"IA 17次谐波/%",
	"IA 18次谐波/%",
	"IA 19次谐波/%",
	"IA 20次谐波/%",
	"IA21次谐波/%",
	"IA 奇次谐波畸变率/%",
	"IA 偶次谐波畸变率/%",
	"IA  K系数/",
	"IB 2次谐波/%",
	"IB 3次谐波/%",
	"IB 4次谐波/%",
	"IB 5次谐波/%",
	"IB 6次谐波/%",
	"IB 7次谐波/%",
	"IB 8次谐波/%",
	"IB 9次谐波/%",
	"IB 10次谐波/%",
	"IB 11次谐波/%",
	"IB 12次谐波/%",
	"IB 13次谐波/%",
	"IB 14次谐波/%",
	"IB 15次谐波/%",
	"IB 16次谐波/%",
	"IB 17次谐波/%",
	"IB 18次谐波/%",
	"IB 19次谐波/%",
	"IB 20次谐波/%",
	"IB 21次谐波/%",
	"IB 奇次谐波畸变率/%",
	"IB 偶次谐波畸变率/%",
	"IB  K系数/",
	"IC 2次谐波/%",
	"IC 3次谐波/%",
	"IC 4次谐波/%",
	"IC 5次谐波/%",
	"IC 6次谐波/%",
	"IC 7次谐波/%",
	"IC 8次谐波/%",
	"IC 9次谐波/%",
	"IC 10次谐波/%",
	"IC 11次谐波/%",
	"IC 12次谐波/%",
	"IC 13次谐波/%",
	"IC 14次谐波/%",
	"IC 15次谐波/%",
	"IC 16次谐波/%",
	"IC 17次谐波/%",
	"IC 18次谐波/%",
	"IC 19次谐波/%",
	"IC 20次谐波/%",
	"IC 21次谐波/%",
	"IC 奇次谐波畸变率/%",
	"IC 偶次谐波畸变率/%",
	"IC  K系数/",
	"结算日热量/单位",
	"当前热量/单位",
	"热功率/单位",
	"流量/单位",
	"累计流量/单位",
	"供水温度/℃",
	"回水温度/℃",
	"累计工作时间/h",
	"当前累计流量/单位",
	"结算日累计流量/单位",
	"时间/",
	"阀值告警状态/",
	"组合有功费率1电能/kWh",
	"组合有功费率2电能/kWh",
	"组合有功费率3电能/kWh",
	"组合有功费率4电能/kWh",
	"正向有功费率1电能/kWh",
	"正向有功费率2电能/kWh",
	"正向有功费率3电能/kWh",
	"正向有功费率4电能/kWh",
	"反向有功费率1电能/kWh",
	"反向有功费率2电能/kWh",
	"反向有功费率3电能/kWh",
	"反向有功费率4电能/kWh",
	"组合无功1费率1电能/kVarh",
	"组合无功1费率2电能/kVarh",
	"组合无功1费率3电能/kVarh",
	"组合无功1费率4电能/kVarh",
	"组合无功2费率1电能/kVarh",
	"组合无功2费率2电能/kVarh",
	"组合无功2费率3电能/kVarh",
	"组合无功2费率4电能/kVarh",
	"电操故障/",
	"合分状态/",
	"重合次数/",
	"重合时间间隔/秒",
	"成功合闸判断时间/秒"
};

const int gAllDevTypeReportParamValue[ALL_DEVTYPE_REPORT_PARAM_NUM] =
{
	E_REPORT_ID_COMBINE_DIF    ,     //差值总电能          
	E_REPORT_ID_T_COMBINE             ,     //组合有功总电能      
	E_REPORT_ID_T_POSITIVE            ,     //正向有功总电能      
	E_REPORT_ID_T_REVERSE             ,     //反向有功总电能      
	E_REPORT_ID_VOLT             ,     //电压                
	E_REPORT_ID_CURRENT             ,     //电流                
	E_REPORT_ID_VOLT_CHANGE         ,     //电压变比            
	E_REPORT_ID_CURR_CHANGE         ,     //电流变比            
	E_REPORT_ID_FREQUENCY           ,     //频率                
	E_REPORT_ID_ACTIVE              ,     //总有功功率          
	E_REPORT_ID_REACTIVE            ,     //总无功功率          
	E_REPORT_ID_APPARENT            ,     //总视在功率          
	E_REPORT_ID_FACTOR              ,     //总功率因数          
	E_REPORT_ID_COMBINE1            ,     //组合无功1总电能     
	E_REPORT_ID_COMBINE2            ,     //组合无功2总电能     
	E_REPORT_ID_N_CURRENT           ,     //零线电流            
	E_REPORT_ID_A_VOLT              ,     //A相电压             
	E_REPORT_ID_B_VOLT              ,     //B相电压             
	E_REPORT_ID_C_VOLT              ,     //C相电压             
	E_REPORT_ID_AB_VOLT             ,     //AB线电压            
	E_REPORT_ID_BC_VOLT             ,     //BC线电压            
	E_REPORT_ID_CA_VOLT             ,     //CA线电压            
	E_REPORT_ID_A_CURRENT           ,     //A相电流             
	E_REPORT_ID_B_CURRENT           ,     //B相电流             
	E_REPORT_ID_C_CURRENT           ,     //C相电流             
	E_REPORT_ID_ACTIVE_A            ,     //A相有功功率         
	E_REPORT_ID_ACTIVE_B            ,     //B相有功功率         
	E_REPORT_ID_ACTIVE_C            ,     //C相有功功率         
	E_REPORT_ID_REACTIVE_A          ,     //A相无功功率         
	E_REPORT_ID_REACTIVE_B          ,     //B相无功功率         
	E_REPORT_ID_REACTIVE_C          ,     //C相无功功率         
	E_REPORT_ID_APPARENT_A          ,     //A相视在功率         
	E_REPORT_ID_APPARENT_B          ,     //B相视在功率         
	E_REPORT_ID_APPARENT_C          ,     //C相视在功率         
	E_REPORT_ID_FACTOR_A            ,     //A相功率因数         
	E_REPORT_ID_FACTOR_B            ,     //B相功率因数         
	E_REPORT_ID_FACTOR_C            ,     //C相功率因数         
	E_REPORT_ID_A_COMBINE           ,     //A相组合有功总电能   
	E_REPORT_ID_B_COMBINE           ,     //B相组合有功总电能   
	E_REPORT_ID_C_COMBINE           ,     //C相组合有功总电能   
	E_REPORT_ID_METER_STATE         ,     //电表状态            
	E_REPORT_ID_ORIGINAL_VOLTAGE_A  ,     //A相基波电压         
	E_REPORT_ID_ORIGINAL_VOLTAGE_B  ,     //B相基波电压         
	E_REPORT_ID_ORIGINAL_VOLTAGE_C  ,     //C相基波电压         
	E_REPORT_ID_ORIGINAL_CURRENT_A  ,     //A相基波电流         
	E_REPORT_ID_ORIGINAL_CURRENT_B  ,     //B相基波电流         
	E_REPORT_ID_ORIGINAL_CURRENT_C  ,     //C相基波电流         
	E_REPORT_ID_ORIGINAL_ACTIVE_T   ,     //总基波有功功率      
	E_REPORT_ID_ORIGINAL_ACTIVE_A   ,     //A相基波有功功率     
	E_REPORT_ID_ORIGINAL_ACTIVE_B   ,     //B相基波有功功率     
	E_REPORT_ID_ORIGINAL_ACTIVE_C   ,     //C相基波有功功率     
	E_REPORT_ID_HARMONIC_VOLTAGE_A  ,     //A相谐波电压         
	E_REPORT_ID_HARMONIC_VOLTAGE_B  ,     //B相谐波电压         
	E_REPORT_ID_HARMONIC_VOLTAGE_C  ,     //C相谐波电压         
	E_REPORT_ID_HARMONIC_CURRENT_A  ,     //A相谐波电流         
	E_REPORT_ID_HARMONIC_CURRENT_B  ,     //B相谐波电流         
	E_REPORT_ID_HARMONIC_CURRENT_C  ,     //C相谐波电流         
	E_REPORT_ID_HARMONIC_ACTIVE_T   ,     //总谐波有功功率      
	E_REPORT_ID_HARMONIC_ACTIVE_A   ,     //A相谐波有功功率     
	E_REPORT_ID_HARMONIC_ACTIVE_B   ,     //B相谐波有功功率     
	E_REPORT_ID_HARMONIC_ACTIVE_C   ,     //C相谐波有功功率     
	E_REPORT_ID_THD_UA              ,     //Ua总畸变率          
	E_REPORT_ID_THD_UB              ,     //Ub总畸变率          
	E_REPORT_ID_THD_UC              ,     //Uc总畸变率          
	E_REPORT_ID_THD_UT              ,     //合相电压总畸变率    
	E_REPORT_ID_THD_IA              ,     //Ia总畸变率          
	E_REPORT_ID_THD_IB              ,     //Ib总畸变率          
	E_REPORT_ID_THD_IC              ,     //Ic总畸变率          
	E_REPORT_ID_THD_IT              ,     //合相电流总畸变率    
	E_REPORT_ID_HARMONIC_UA_2       ,     //UA 2次谐波          
	E_REPORT_ID_HARMONIC_UA_3       ,     //UA 3次谐波          
	E_REPORT_ID_HARMONIC_UA_4       ,     //UA 4次谐波          
	E_REPORT_ID_HARMONIC_UA_5       ,     //UA 5次谐波          
	E_REPORT_ID_HARMONIC_UA_6       ,     //UA 6次谐波          
	E_REPORT_ID_HARMONIC_UA_7       ,     //UA 7次谐波          
	E_REPORT_ID_HARMONIC_UA_8       ,     //UA 8次谐波          
	E_REPORT_ID_HARMONIC_UA_9       ,     //UA 9次谐波          
	E_REPORT_ID_HARMONIC_UA_10      ,     //UA 10次谐波         
	E_REPORT_ID_HARMONIC_UA_11      ,     //UA 11次谐波         
	E_REPORT_ID_HARMONIC_UA_12      ,     //UA 12次谐波         
	E_REPORT_ID_HARMONIC_UA_13      ,     //UA 13次谐波         
	E_REPORT_ID_HARMONIC_UA_14      ,     //UA 14次谐波         
	E_REPORT_ID_HARMONIC_UA_15      ,     //UA 15次谐波         
	E_REPORT_ID_HARMONIC_UA_16      ,     //UA 16次谐波         
	E_REPORT_ID_HARMONIC_UA_17      ,     //UA 17次谐波         
	E_REPORT_ID_HARMONIC_UA_18      ,     //UA 18次谐波         
	E_REPORT_ID_HARMONIC_UA_19      ,     //UA 19次谐波         
	E_REPORT_ID_HARMONIC_UA_20      ,     //UA 20次谐波         
	E_REPORT_ID_HARMONIC_UA_21      ,     //UA 21次谐波         
	E_REPORT_ID_HARMONIC_UA_ODD     ,     //UA 奇次谐波畸变率   
	E_REPORT_ID_HARMONIC_UA_EVEN    ,     //UA 偶次谐波畸变率   
	E_REPORT_ID_HARMONIC_UA_RIDGE   ,     //UA 波峰系数         
	E_REPORT_ID_HARMONIC_UA_TETL    ,     //UA 电话谐波波形因数 
	E_REPORT_ID_HARMONIC_UB_2       ,     //UB 2次谐波          
	E_REPORT_ID_HARMONIC_UB_3       ,     //UB 3次谐波          
	E_REPORT_ID_HARMONIC_UB_4       ,     //UB 4次谐波          
	E_REPORT_ID_HARMONIC_UB_5       ,     //UB 5次谐波          
	E_REPORT_ID_HARMONIC_UB_6       ,     //UB 6次谐波          
	E_REPORT_ID_HARMONIC_UB_7       ,     //UB 7次谐波          
	E_REPORT_ID_HARMONIC_UB_8       ,     //UB 8次谐波          
	E_REPORT_ID_HARMONIC_UB_9       ,     //UB 9次谐波          
	E_REPORT_ID_HARMONIC_UB_10      ,     //UB 10次谐波         
	E_REPORT_ID_HARMONIC_UB_11      ,     //UB 11次谐波         
	E_REPORT_ID_HARMONIC_UB_12      ,     //UB 12次谐波         
	E_REPORT_ID_HARMONIC_UB_13      ,     //UB 13次谐波        
	E_REPORT_ID_HARMONIC_UB_14      ,     //UB 14次谐波        
	E_REPORT_ID_HARMONIC_UB_15      ,     //UB 15次谐波        
	E_REPORT_ID_HARMONIC_UB_16      ,     //UB 16次谐波        
	E_REPORT_ID_HARMONIC_UB_17      ,     //UB 17次谐波        
	E_REPORT_ID_HARMONIC_UB_18      ,     //UB 18次谐波        
	E_REPORT_ID_HARMONIC_UB_19      ,     //UB 19次谐波        
	E_REPORT_ID_HARMONIC_UB_20      ,     //UB 20次谐波        
	E_REPORT_ID_HARMONIC_UB_21      ,     //UB 21次谐波        
	E_REPORT_ID_HARMONIC_UB_ODD     ,     //UB 奇次谐波畸变率  
	E_REPORT_ID_HARMONIC_UB_EVEN    ,     //UB 偶次谐波畸变率  
	E_REPORT_ID_HARMONIC_UB_RIDGE   ,     //UB 波峰系数        
	E_REPORT_ID_HARMONIC_UB_TETL    ,     //UB 电话谐波波形因数
	E_REPORT_ID_HARMONIC_UC_2       ,     //UC 2次谐波         
	E_REPORT_ID_HARMONIC_UC_3       ,     //UC 3次谐波         
	E_REPORT_ID_HARMONIC_UC_4       ,     //UC 4次谐波         
	E_REPORT_ID_HARMONIC_UC_5       ,     //UC 5次谐波         
	E_REPORT_ID_HARMONIC_UC_6       ,     //UC 6次谐波         
	E_REPORT_ID_HARMONIC_UC_7       ,     //UC 7次谐波         
	E_REPORT_ID_HARMONIC_UC_8       ,     //UC 8次谐波         
	E_REPORT_ID_HARMONIC_UC_9       ,     //UC 9次谐波         
	E_REPORT_ID_HARMONIC_UC_10      ,     //UC 10次谐波        
	E_REPORT_ID_HARMONIC_UC_11      ,     //UC 11次谐波        
	E_REPORT_ID_HARMONIC_UC_12      ,     //UC 12次谐波        
	E_REPORT_ID_HARMONIC_UC_13      ,     //UC 13次谐波        
	E_REPORT_ID_HARMONIC_UC_14      ,     //UC 14次谐波        
	E_REPORT_ID_HARMONIC_UC_15      ,     //UC 15次谐波        
	E_REPORT_ID_HARMONIC_UC_16      ,     //UC 16次谐波        
	E_REPORT_ID_HARMONIC_UC_17      ,     //UC 17次谐波        
	E_REPORT_ID_HARMONIC_UC_18      ,     //UC 18次谐波        
	E_REPORT_ID_HARMONIC_UC_19      ,     //UC 19次谐波        
	E_REPORT_ID_HARMONIC_UC_20      ,     //UC 20次谐波        
	E_REPORT_ID_HARMONIC_UC_21      ,     //UC21次谐波         
	E_REPORT_ID_HARMONIC_UC_ODD     ,     //UC 奇次谐波畸变率  
	E_REPORT_ID_HARMONIC_UC_EVEN    ,     //UC 偶次谐波畸变率  
	E_REPORT_ID_HARMONIC_UC_RIDGE   ,     //UC 波峰系数        
	E_REPORT_ID_HARMONIC_UC_TETL    ,     //UC 电话谐波波形因数
	E_REPORT_ID_HARMONIC_IA_2       ,     //IA 2次谐波         
	E_REPORT_ID_HARMONIC_IA_3       ,     //IA 3次谐波         
	E_REPORT_ID_HARMONIC_IA_4       ,     //IA 4次谐波         
	E_REPORT_ID_HARMONIC_IA_5       ,     //IA 5次谐波         
	E_REPORT_ID_HARMONIC_IA_6       ,     //IA 6次谐波         
	E_REPORT_ID_HARMONIC_IA_7       ,     //IA 7次谐波         
	E_REPORT_ID_HARMONIC_IA_8       ,     //IA 8次谐波         
	E_REPORT_ID_HARMONIC_IA_9       ,     //IA 9次谐波         
	E_REPORT_ID_HARMONIC_IA_10      ,     //IA 10次谐波        
	E_REPORT_ID_HARMONIC_IA_11      ,     //IA 11次谐波        
	E_REPORT_ID_HARMONIC_IA_12      ,     //IA 12次谐波        
	E_REPORT_ID_HARMONIC_IA_13      ,     //IA 13次谐波        
	E_REPORT_ID_HARMONIC_IA_14      ,     //IA 14次谐波        
	E_REPORT_ID_HARMONIC_IA_15      ,     //IA 15次谐波        
	E_REPORT_ID_HARMONIC_IA_16      ,     //IA 16次谐波        
	E_REPORT_ID_HARMONIC_IA_17      ,     //IA 17次谐波        
	E_REPORT_ID_HARMONIC_IA_18      ,     //IA 18次谐波        
	E_REPORT_ID_HARMONIC_IA_19      ,     //IA 19次谐波        
	E_REPORT_ID_HARMONIC_IA_20      ,     //IA 20次谐波        
	E_REPORT_ID_HARMONIC_IA_21      ,     //IA21次谐波         
	E_REPORT_ID_HARMONIC_IA_ODD     ,     //IA 奇次谐波畸变率  
	E_REPORT_ID_HARMONIC_IA_EVEN    ,     //IA 偶次谐波畸变率  
	E_REPORT_ID_HARMONIC_IA_K       ,     //IA  K系数          
	E_REPORT_ID_HARMONIC_IB_2       ,     //IB 2次谐波         
	E_REPORT_ID_HARMONIC_IB_3       ,     //IB 3次谐波         
	E_REPORT_ID_HARMONIC_IB_4       ,     //IB 4次谐波         
	E_REPORT_ID_HARMONIC_IB_5       ,     //IB 5次谐波         
	E_REPORT_ID_HARMONIC_IB_6       ,     //IB 6次谐波         
	E_REPORT_ID_HARMONIC_IB_7       ,     //IB 7次谐波         
	E_REPORT_ID_HARMONIC_IB_8       ,     //IB 8次谐波         
	E_REPORT_ID_HARMONIC_IB_9       ,     //IB 9次谐波         
	E_REPORT_ID_HARMONIC_IB_10      ,     //IB 10次谐波        
	E_REPORT_ID_HARMONIC_IB_11      ,     //IB 11次谐波        
	E_REPORT_ID_HARMONIC_IB_12      ,     //IB 12次谐波        
	E_REPORT_ID_HARMONIC_IB_13      ,     //IB 13次谐波        
	E_REPORT_ID_HARMONIC_IB_14      ,     //IB 14次谐波        
	E_REPORT_ID_HARMONIC_IB_15      ,     //IB 15次谐波        
	E_REPORT_ID_HARMONIC_IB_16      ,     //IB 16次谐波        
	E_REPORT_ID_HARMONIC_IB_17      ,     //IB 17次谐波        
	E_REPORT_ID_HARMONIC_IB_18      ,     //IB 18次谐波        
	E_REPORT_ID_HARMONIC_IB_19      ,     //IB 19次谐波        
	E_REPORT_ID_HARMONIC_IB_20      ,     //IB 20次谐波        
	E_REPORT_ID_HARMONIC_IB_21      ,     //IB 21次谐波        
	E_REPORT_ID_HARMONIC_IB_ODD     ,     //IB 奇次谐波畸变率  
	E_REPORT_ID_HARMONIC_IB_EVEN    ,     //IB 偶次谐波畸变率  
	E_REPORT_ID_HARMONIC_IB_K       ,     //IB  K系数          
	E_REPORT_ID_HARMONIC_IC_2       ,     //IC 2次谐波         
	E_REPORT_ID_HARMONIC_IC_3       ,     //IC 3次谐波         
	E_REPORT_ID_HARMONIC_IC_4       ,     //IC 4次谐波         
	E_REPORT_ID_HARMONIC_IC_5       ,     //IC 5次谐波         
	E_REPORT_ID_HARMONIC_IC_6       ,     //IC 6次谐波         
	E_REPORT_ID_HARMONIC_IC_7       ,     //IC 7次谐波         
	E_REPORT_ID_HARMONIC_IC_8       ,     //IC 8次谐波         
	E_REPORT_ID_HARMONIC_IC_9       ,     //IC 9次谐波         
	E_REPORT_ID_HARMONIC_IC_10      ,     //IC 10次谐波        
	E_REPORT_ID_HARMONIC_IC_11      ,     //IC 11次谐波        
	E_REPORT_ID_HARMONIC_IC_12      ,     //IC 12次谐波        
	E_REPORT_ID_HARMONIC_IC_13      ,     //IC 13次谐波        
	E_REPORT_ID_HARMONIC_IC_14      ,     //IC 14次谐波        
	E_REPORT_ID_HARMONIC_IC_15      ,     //IC 15次谐波        
	E_REPORT_ID_HARMONIC_IC_16      ,     //IC 16次谐波        
	E_REPORT_ID_HARMONIC_IC_17      ,     //IC 17次谐波        
	E_REPORT_ID_HARMONIC_IC_18      ,     //IC 18次谐波        
	E_REPORT_ID_HARMONIC_IC_19      ,     //IC 19次谐波        
	E_REPORT_ID_HARMONIC_IC_20      ,     //IC 20次谐波        
	E_REPORT_ID_HARMONIC_IC_21      ,     //IC 21次谐波        
	E_REPORT_ID_HARMONIC_IC_ODD     ,     //IC 奇次谐波畸变率  
	E_REPORT_ID_HARMONIC_IC_EVEN    ,     //IC 偶次谐波畸变率  
	E_REPORT_ID_HARMONIC_IC_K       ,//IC  K系数  
	E_REPORT_ID_HEAT_SETTLE_DATA,//结算日热量
	E_REPORT_ID_CUR_HEAT,//当前热量
	E_REPORT_ID_THERMAL_POWER,//热功率
	E_REPORT_ID_FLOW,//流量
	E_REPORT_ID_CUMULA_FLOW,//累计流量
	E_REPORT_ID_SUPPLYWATER_TEMP,//供水温度
	E_REPORT_ID_BACKWATER_TEMP,//回水温度
	E_REPORT_ID_CUMULA_TIME,//累计工作时间
	E_REPORT_ID_CUR_CUMULA_FLOW,//当前累计流量
	E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA,//结算日累计流量
	E_REPORT_ID_METER_TIME,//时间
	E_REPORT_ID_THRESHOLD_STATE,//阀值告警状态
	E_REPORT_ID_T_COMBINE_RATE1,//组合有功费率1电能
	E_REPORT_ID_T_COMBINE_RATE2,//组合有功费率2电能
	E_REPORT_ID_T_COMBINE_RATE3,//组合有功费率3电能
	E_REPORT_ID_T_COMBINE_RATE4,//组合有功费率4电能
	E_REPORT_ID_T_POSITIVE_RATE1,//正向有功费率1电能
	E_REPORT_ID_T_POSITIVE_RATE2,//正向有功费率2电能
	E_REPORT_ID_T_POSITIVE_RATE3,//正向有功费率3电能
	E_REPORT_ID_T_POSITIVE_RATE4,//正向有功费率4电能
	E_REPORT_ID_T_REVERSE_RATE1,//反向有功费率1电能
	E_REPORT_ID_T_REVERSE_RATE2,//反向有功费率2电能
	E_REPORT_ID_T_REVERSE_RATE3,//反向有功费率3电能
	E_REPORT_ID_T_REVERSE_RATE4,//反向有功费率4电能
	E_REPORT_ID_T_COMBINE1_RATE1,//组合无功1费率1电能
	E_REPORT_ID_T_COMBINE1_RATE2,//组合无功1费率2电能
	E_REPORT_ID_T_COMBINE1_RATE3,//组合无功1费率3电能
	E_REPORT_ID_T_COMBINE1_RATE4,//组合无功1费率4电能
	E_REPORT_ID_T_COMBINE2_RATE1,//组合无功2费率1电能
	E_REPORT_ID_T_COMBINE2_RATE2,//组合无功2费率2电能
	E_REPORT_ID_T_COMBINE2_RATE3,//组合无功2费率3电能
	E_REPORT_ID_T_COMBINE2_RATE4,//组合无功2费率4电能
	E_REPORT_ID_E_OP_FAULT,//电操故障
	E_REPORT_ID_ON_OFF_STATE,//合分状态
	E_REPORT_ID_ON_COUNT,//重合次数
	E_REPORT_ID_ON_TIMER,//重合时间间隔
	E_REPORT_ID_ON_THINK_TIME//成功合闸判断时间
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
	E_REPORT_ID_CUR_CUMULA_FLOW,//当前累计流量
	E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA,//结算日累计流量
};

const int gHotMeterReportParamNum = HOT_METER_REPORT_PARAM_NUM;
const int gHotMeterReportParamValue[HOT_METER_REPORT_PARAM_NUM] = 
{
	E_REPORT_ID_METER_STATE,
	E_REPORT_ID_HEAT_SETTLE_DATA,//结算日热量
	E_REPORT_ID_CUR_HEAT,//当前热量
	E_REPORT_ID_THERMAL_POWER,//热功率 
	E_REPORT_ID_FLOW,//流量
	E_REPORT_ID_CUMULA_FLOW,//累计流量
	E_REPORT_ID_SUPPLYWATER_TEMP,//供水温度
	E_REPORT_ID_BACKWATER_TEMP,//回水温度
	E_REPORT_ID_CUMULA_TIME,//累计工作时间
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
			sprintf(tail,"自定义子类型%d", ind);
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
	//unit的映射关系 {0->0.001; 1->0.01; 2->0.1; 3->1; 4->10; 5->100; 6->1000}
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
	//unit的映射关系 {0->0.001; 1->0.01; 2->0.1; 3->1; 4->10; 5->100; 6->1000}
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
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	ZeroMemory(&bi,sizeof(bi));
	//bi.hwndOwner = m_hWnd;   
	//bi.pidlRoot = NULL;   
	//bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择保存历史数据的目录：";   
	//bi.ulFlags = 0;   
	//bi.lpfn = NULL;   
	//bi.lParam = 0;   
	//bi.iImage = 0;   
	//弹出选择目录对话框
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
		val ==  E_REPORT_ID_CUR_CUMULA_FLOW  ||//当前累计流量
		val ==  E_REPORT_ID_CUMULA_FLOW_SETTLE_DATA  ||//结算日累计流量
		val ==  E_REPORT_ID_HEAT_SETTLE_DATA ||//结算日热量
		val ==  E_REPORT_ID_CUR_HEAT  ||//当前热量
		val ==  E_REPORT_ID_THERMAL_POWER ||//热功率
		val ==  E_REPORT_ID_FLOW ||//流量
		val ==  E_REPORT_ID_CUMULA_FLOW//累计流量
		)
	{
		int param_ind = GetAllDevTypeReportParamIndByValue(val);
		// 123/unitOld -> 123/unitNew
		CString param_name = gAllDevTypeReportParamName[param_ind];
		CString sig_flag = "/";
		int sig_ind = param_name.Find(sig_flag);
		CString param_name_pre = param_name.Left(sig_ind+1);//前缀
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
		ret = "错误";
	}
	return ret;
}

int  gDefProtWayChooseNum = MAX_WAY_NUM;
const char gDefProtWayChooseName[MAX_WAY_NUM][64] = 
{
	"第1回路参数信息",
	"第2回路参数信息",
	"第3回路参数信息",
	"第4回路参数信息",
	"第5回路参数信息",
	"第6回路参数信息",
	"第7回路参数信息",
	"第8回路参数信息",
	"第9回路参数信息",
	"第10回路参数信息",
	"第11回路参数信息",
	"第12回路参数信息"
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
	"新宏博","紫衡"
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
