#include "CommonFunction.h"
#include "DataTypeDef.h"
#include "Log.h"

const int  gDevTypeNum = E_DEV_TYPE_NUM;
const char gDevTypeName[E_DEV_TYPE_NUM][64] =
{
    "电表设备",
    "绝缘设备",
    "开关设备"
};

const int gDevTypeValue[E_DEV_TYPE_NUM] =
{
    E_DEV_ELECTRIC ,
    E_DEV_INSULATION,
    E_DEV_SWITCH
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
    BAUD1200 ,
    BAUD2400 ,
    BAUD4800 ,
    BAUD9600 ,
    BAUD19200,
    BAUD38400,
    BAUD115200
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

string GetSubTypeNameByValue( int type )
{
    string str = "";
    for (int i = 0; i < gSubTypeNum; i++)
    {
        if (gSubTypeValue[i] == type)
        {
            str = gSubTypeName[i];
        }
    }
    return str;
}

int GetSubTypeValueByName( string name )
{
    int ret = -1;
    for (int i = 0; i < gSubTypeNum; i++)
    {
        if (0 == strcmp(gSubTypeName[i], name.c_str()))
        {
            ret = gSubTypeValue[i];
        }
    }
    return ret;
}

string GetBaudNameByValue( int type )
{
    string str = "";
    for (int i = 0; i < gBaudNum; i++)
    {
        if (gBaudValue[i] == type)
        {
            str = gBaudName[i];
        }
    }
    return str;
}

int GetBaudValueByName( string name )
{
    int ret = 0;
    for (int i = 0; i < gBaudNum; i++)
    {
        if (0 == strcmp(gBaudName[i], name.c_str()))
        {
            ret = gBaudValue[i];
        }
    }
    return ret;
}

string GetParityNameByValue( int type )
{
    string str = "";
    for (int i = 0; i < gParityNum; i++)
    {
        if (gParityValue[i] == type)
        {
            str = gParityName[i];
        }
    }
    return str;
}

int GetParityValueByName( string name )
{
    int ret = 0;
    for (int i = 0; i < gParityNum; i++)
    {
        if (0 == strcmp(gParityName[i], name.c_str()))
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

const int gOneReg = 10;
const int gTwoReg = 100;
const int gThreeReg = 1000;
const int gFourReg = 10000;
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

string GetSaveFileNameByDevType( e_dev_type dev_type )
{
    string str;
    switch(dev_type)
    {
    case E_DEV_ELECTRIC:
        str = "电表设备.csv";
        break;

    case E_DEV_INSULATION:
        str = "绝缘设备.csv";
        break;

    case E_DEV_SWITCH:
        str = "开关设备.csv";
        break;
    default:
        break;
    }
    return str;
}

string GetSaveFileHeadByDevType( e_dev_type dev_type )
{
    string str = "";

    //    switch(dev_type)
    //    {
    //    case AM_3PHASE:
    //        for (int i = 0; i < AM_THREE_PHASE_NUM; i++)
    //        {
    //            str.append(gAm3phaseName[i]);
    //            str.append(",");
    //        }
    //        break;

    //    case AM_1PHASE:
    //        for (int i = 0; i < AM_ONE_PHASE_NUM; i++)
    //        {
    //            str.append(gAm1phaseName[i]);
    //            str.append(",");
    //        }
    //        break;

    //    case AM_DC:
    //        for (int i = 0; i < AM_DC_NUM; i++)
    //        {
    //            str.append(gAmdcName[i]);
    //            str.append(",");
    //        }
    //        break;

    //    case AM_HARM:
    //        for (int i = 0; i < AM_HARM_NUM; i++)
    //        {
    //            str.append(gAmHarmName[i]);
    //            str.append(",");
    //        }
    //        break;

    //    case WM:
    //        for (int i = 0; i < WATER_METER_NAME_NUM; i++)
    //        {
    //            str.append(gWaterMeterName[i]);
    //            str.append(",");
    //        }
    //        break;
    //    case GM:
    //        for (int i = 0; i < GASS_METER_NAME_NUM; i++)
    //        {
    //            str.append(gGassMeterName[i]);
    //            str.append(",");
    //        }
    //        break;
    //    default:
    //        break;
    //    }
    return str;
}

string GetDevTypeNameByValue( int type )
{
    string str = "";
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
string gCurrentDir;

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


int GetWayDevTypeValueByName( string name )
{
    int ret = 0;
    for (int i = 0; i < gWayDevTypeNum; i++)
    {
        if (0 == strcmp(gWayDevTypeName[i], name.c_str()))
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

bool IsWayUsedByMask(int way_ind, MU_INT64 mask)
{
    bool used = false;
    MU_INT64 src_val = 0x01;
    MU_INT64 flag = src_val << way_ind;
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

int GetDevTypeValueByName( string name )
{
    int ret = 0;
    for (int i = 0; i < gDevTypeNum; i++)
    {
        if (0 == strcmp(gDevTypeName[i], name.c_str()))
        {
            ret = gDevTypeValue[i];
        }
    }
    return ret;
}

string GetDataBitNameByValue( int type )
{
    string str = "";
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

string GetStopNameByValue( int type )
{
    string str = "";
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

int GetWayListByMask(MU_INT64 way_mask, VEC_INT& vec_data, int max_way_count)
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


int GetAllDevTypeReportParamValueByName( string name )
{
    int ret = 0;
    for (int i = 0; i < gAllDevTypeReportParamNum; i++)
    {
        if (0 == strcmp(gAllDevTypeReportParamName[i], name.c_str()))
        {
            ret = gAllDevTypeReportParamValue[i];
        }
    }
    return ret;
}

string GetReportTypeNameByValue( int type )
{
    string str = "";
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

string GetAllDevTypeReportParamNameByValue( int type )
{
    string str = "";
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
    if(sub_type_ind >= gSubTypeNum)
    {
        gLog.Add("%s sub_type_ind >= gSubTypeNum", __FUNCTION__);
        return;
    }
    if(strlen(sub_name) >= 64)
    {
        gLog.Add("%s strlen sub_name >= 64", __FUNCTION__);
        return;
    }

    if (sub_type_ind >= 0)
    {
        strcpy(gSubTypeName[sub_type_ind], sub_name);
    }
}

void SetWayCountBySubTypeInd( int sub_type_ind, int way_count )
{
    if(sub_type_ind >= gSubTypeNum)
    {
        gLog.Add("%s sub_type_ind >= gSubTypeNum", __FUNCTION__);
        return;
    }
    if(way_count > MAX_WAY_NUM)
    {
        gLog.Add("%s way_count > MAX_WAY_NUM", __FUNCTION__);
        return;
    }

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
    //    int dev_type = sub_type / 100;
    //    if((sub_type % 100 >= 11) &&
    //            (sub_type % 100 <= 14) &&
    //            (dev_type == AM_3PHASE ||
    //             dev_type == AM_1PHASE ||
    //             dev_type == AM_DC ||
    //             dev_type == AM_HARM)
    //            )
    //    {
    //        is_def = true;
    //    }
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

string BootOpenFile( bool open /*= TRUE*/ )
{
    string str_file = ("");

    return str_file;
}

string BootOpenFolder()
{

    string str = "";
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
    if(sub_type_ind >= gSubTypeNum)
    {
        gLog.Add("%s sub_type_ind >= gSubTypeNum", __FUNCTION__);
        return -1;
    }
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

int ModifyReportParamUnitName( string unit_name, int val )
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
        string param_name = gAllDevTypeReportParamName[param_ind];
        string sig_flag = "/";
        int sig_ind = param_name.find(sig_flag);
        string param_name_pre = param_name.substr(0,sig_ind+1);//前缀
        string param_now = param_name_pre + unit_name;
        sprintf(gAllDevTypeReportParamName[param_ind], "%s", param_now.c_str());
        ret = 0;
    }
    else
    {
        ret = -1;
    }
    return ret;
}

string ParseUnitByValue( MU_INT8 val )
{
    string ret = "";
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

int GetDefProtWayChooseValueByName( string name )
{
    int ret = -1;
    for (int i = 0; i < gDefProtWayChooseNum; i++)
    {
        if (0 == strcmp(gDefProtWayChooseName[i], name.c_str()))
        {
            ret = gDefProtWayChooseValue[i];
        }
    }
    return ret;
}

string GetDefProtWayChooseNameByValue( int val )
{
    string ret = "";
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
    //    bool is_def = IsDefSubType(sub_type);
    //    if (is_def)
    //    {
    //        //111-114;211-214;311-314;411-414;
    //        int dev_type = sub_type / 100;
    //        switch(dev_type)
    //        {
    //        case AM_3PHASE:
    //            modbus_ind = sub_type - AM_3PHASE_DEF_01;
    //            break;
    //        case AM_1PHASE:
    //            modbus_ind = sub_type - AM_1PHASE_DEF_01;
    //            break;
    //        case AM_DC:
    //            modbus_ind = sub_type - AM_DC_DEF_01;
    //            break;
    //        case AM_HARM:
    //            modbus_ind = sub_type - AM_HARM_DEF_01;
    //            break;
    //        default:
    //            modbus_ind = -1;
    //            break;
    //        }
    //    }
    //    else
    //    {
    //        modbus_ind = -1;
    //    }
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


void ParseTskResult(int result)
{
    if (result == SEND_FAIL ||
            result == RECEIVE_FAIL
            )
    {
        gLog.Add("[%s]communicate fail", __FUNCTION__);
    }
}


QString gResArrowLeft = "border-image: url(:/images/arrow_left.png)";
QString gResArrowRight = "border-image: url(:/images/arrow_right.png)";
QString gResBackGroud = "border-image: url(:/images/backgroud.png)";
QString gResBell = "border-image: url(:/images/bell.png)";
QString gResBottom = "border-image: url(:/images/bottom.png)";
QString gResButtonBlue = "color: white;border-image: url(:/images/button_blue.png)";
QString gResButtonGreen = "color: white;border-image: url(:/images/button_green.png)";
QString gResButtonRed = "color: white;border-image: url(:/images/button_red.png)";
QString gResCurrent = "border-image: url(:/images/current.png)";
QString gResLeftRight = "border-image: url(:/images/left_right.png)";
QString gResLightGreen = "border-image: url(:/images/light_green.png)";
QString gResLightRed = "border-image: url(:/images/light_red.png)";
QString gResNhb = "border-image: url(:/images/nhb.png)";
QString gResReturnBack = "border-image: url(:/images/return_back.png)";
QString gResTitleGreen = "border-image: url(:/images/title_green.png)";
QString gResTitleInit = "border-image: url(:/images/title_init.png)";
QString gResTop = "border-image: url(:/images/top.png)";
QString gResVoltage = "border-image: url(:/images/voltage.png)";
QString gResSwitchOn = "border-image: url(:/images/switch_on.png)";
QString gResSwitchOff = "border-image: url(:/images/switch_off.png)";
QString gResArrowUp = "border-image: url(:/images/arrow_up.png)";
QString gResArrowDown = "border-image: url(:/images/arrow_down.png)";
int gLeftChild;
int gTopChild;
int gWidthChild;
int gHeightChild;
/////////////////////////////////////////////
QString Uint2DoubleQStr(MU_INT32 val, double unit)
{
    double d_val = val * unit;
    int prec = 0;
    if (unit >= (gOneUnit - 0.1*gOneUnit) &&
            unit <= (gOneUnit + 0.1*gOneUnit))
    {
        prec = 1;
    }
    else if(unit >= (gTwoUnit - 0.1*gTwoUnit) &&
            unit <= (gTwoUnit + 0.1*gTwoUnit))
    {
        prec = 2;
    }
    else if(unit >= (gThreeUnit - 0.1*gThreeUnit) &&
            unit <= (gThreeUnit + 0.1*gThreeUnit))
    {
        prec = 3;
    }
    else if(unit >= (gFourUnit - 0.1*gFourUnit) &&
            unit <= (gFourUnit + 0.1*gFourUnit))
    {
        prec = 4;
    }
    return QString::number(d_val, 'f', prec);
}

QString Sint2DoubleQStr(MS_INT32 val, double unit)
{
    double d_val = val * unit;
    int prec = 0;
    if (unit >= (gOneUnit - 0.1*gOneUnit) &&
            unit <= (gOneUnit + 0.1*gOneUnit))
    {
        prec = 1;
    }
    else if(unit >= (gTwoUnit - 0.1*gTwoUnit) &&
            unit <= (gTwoUnit + 0.1*gTwoUnit))
    {
        prec = 2;
    }
    else if(unit >= (gThreeUnit - 0.1*gThreeUnit) &&
            unit <= (gThreeUnit + 0.1*gThreeUnit))
    {
        prec = 3;
    }
    else if(unit >= (gFourUnit - 0.1*gFourUnit) &&
            unit <= (gFourUnit + 0.1*gFourUnit))
    {
        prec = 4;
    }
    return QString::number(d_val, 'f', prec);
}

//////////////////////////////////////////////////

st_way_data* GetWayInfByCode(str_outer_dev *outer_dev, int way_code, int dev_type)
{
    st_way_data* p_st_way_data = NULL;
    str_outer_dev* pouter_dev = outer_dev;
    if(!pouter_dev)
    {
        return p_st_way_data;
    }

    for(int port_ind = 0; port_ind < pouter_dev->port_num; port_ind++)
    {
        for(int dev_ind = 0; dev_ind < pouter_dev->port_dev[port_ind].dev_num; dev_ind++)
        {
            st_dev_inf* p_st_dev_inf = &pouter_dev->port_dev[port_ind].dev_inf[dev_ind];
            for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
            {
                bool is_way_use = IsWayUsedByMask(way_ind,
                                                  p_st_dev_inf->way_inf.way_mask);
                if(is_way_use)
                {
                    p_st_way_data = &p_st_dev_inf->way_inf.way_data[way_ind];
                    int way_id = p_st_way_data->way_id;
                    //0主路,1-24支路
                    if(way_id == way_code && p_st_dev_inf->dev_type == dev_type)
                    {
                        goto LEAVE;
                    }
                    else
                    {
                        p_st_way_data = NULL;
                    }
                }
            }
        }
    }

LEAVE:
    return p_st_way_data;
}

st_dev_inf* GetDevInfByCode(str_outer_dev *outer_dev, int way_code, int dev_type)
{
    str_outer_dev* pouter_dev = outer_dev;
    st_dev_inf* p_st_dev_inf = NULL;
    if(!pouter_dev)
    {
        return p_st_dev_inf;
    }

    for(int port_ind = 0; port_ind < pouter_dev->port_num; port_ind++)
    {
        for(int dev_ind = 0; dev_ind < pouter_dev->port_dev[port_ind].dev_num; dev_ind++)
        {
            for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
            {
                p_st_dev_inf = &pouter_dev->port_dev[port_ind].dev_inf[dev_ind];
                bool is_way_use = IsWayUsedByMask(way_ind,
                                                  p_st_dev_inf->way_inf.way_mask);
                if(is_way_use)
                {
                    st_way_data* p_st_way_data = &p_st_dev_inf->way_inf.way_data[way_ind];
                    int way_id = p_st_way_data->way_id;
                    //0主路,1-24支路
                    if(way_id == way_code && p_st_dev_inf->dev_type == dev_type)
                    {
                        goto LEAVE;
                    }
                    else
                    {
                        p_st_dev_inf = NULL;
                    }
                }
            }
        }
    }

LEAVE:
    return p_st_dev_inf;
}

st_dev_inf* GetDevInfByDevIdType(str_outer_dev* outer_dev, MU_INT64 id, int dev_type)
{
    st_dev_inf* p_dev_inf = NULL;

    int port_num = outer_dev->port_num;
    for (int port_ind = 0; port_ind < port_num; port_ind++)
    {
        int dev_num = outer_dev->port_dev[port_ind].dev_num;
        for (int i = 0; i < dev_num; i++)
        {
            p_dev_inf = &outer_dev->port_dev[port_ind].dev_inf[i];
            if(id == p_dev_inf->dev_addr.id &&
                    dev_type == p_dev_inf->dev_type)
            {
                return p_dev_inf;
            }
        }
    }
    return NULL;
}

MU_INT32* GetThreByWarnType(st_way_data* p_st_way_data, MU_INT32 warn_type)
{
    MU_INT32* p_thre = NULL;
    switch(warn_type)
    {
    case E_U_DOWN_LIMIT:
        p_thre = &p_st_way_data->way_threshold.u_down_limit[0];
        break;
    case E_U_UP_LIMIT:
        p_thre = &p_st_way_data->way_threshold.u_up_limit[0];
        break;
    case E_U_HARM_PERCENT_UP_LIMIT:
        p_thre = &p_st_way_data->way_threshold.u_harm_percent_up_limit[0];
        break;
    case E_I_ZERO_UP_LIMIT:
        p_thre = &p_st_way_data->way_threshold.i_zero_up_limit[0];
        break;
    case E_I_UNBALANCE_UP_LIMIT:
        p_thre = &p_st_way_data->way_threshold.i_unbalance_up_limit[0];
        break;
    case E_I_HARM_PERCENT_LIMIT:
        p_thre = &p_st_way_data->way_threshold.i_harm_percent_limit[0];
        break;
    case E_F_UP_LIMIT:
        p_thre = &p_st_way_data->way_threshold.f_up_limit[0];
        break;
    case E_F_DOWN_LIMIT:
        p_thre = &p_st_way_data->way_threshold.f_down_limit[0];
        break;
    case E_LOSE_PHASE_LIMIT:
        p_thre = &p_st_way_data->way_threshold.lose_phase_limit[0];
        break;
    case E_I_DOWN_LIMIT:
        p_thre = &p_st_way_data->way_threshold.i_down_limit[0];
        break;
    case E_I_UP_LIMIT:
        p_thre = &p_st_way_data->way_threshold.i_up_limit[0];
        break;
    case E_I_DOWN2_LIMIT:
        p_thre = &p_st_way_data->way_threshold.i_down2_limit[0];
        break;
    case E_I_UP2_LIMIT:
        p_thre = &p_st_way_data->way_threshold.i_up2_limit[0];
        break;
    case E_FUSE_LIMIT:
        p_thre = &p_st_way_data->way_threshold.fuse_limit[0];
        break;
    default:
        break;
    }
    return p_thre;
}

string AnalyzeMeterState( int state )
{
    MU_INT16 err = state & 0xff;
    string str = "";
    bool is_err = false;
    for (int i = 0; i < ERR_NAME_NUM; i++)
    {
        if ((err >> i) & 0x01)
        {
            is_err = true;
            str += gErrName[i];
            str += ";";
        }
    }

    if (is_err == false)
    {
        str = "正常";
    }
    else
    {
        str.substr(0,str.length()-1);
    }

    return str;
}

string AnalyzeThresholdState( int state )
{
    MU_INT16 err = state & 0xff;
    string str = "";
    MU_INT8 flag = 0;
    bool is_err = false;
    //D0-D15
    //D0D1
    flag = (MU_INT8)((err >> 14) & 0x03);
    if (flag == 0x00)
    {
        str += "阀门状态:开";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "阀门状态:关";
        str += ";";
    }
    else if (flag == 0x11)
    {
        str += "阀门状态:异常";
        str += ";";
        is_err = true;
    }

    //D2
    flag = (MU_INT8)((err >> 13) & 0x01);
    if (flag == 0x00)
    {
        str += "电池电压:正常";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "电池电压:欠压";
        str += ";";
        is_err = true;
    }

    //D5
    flag = (MU_INT8)((err >> 10) & 0x01);
    if (flag == 0x00)
    {
        str += "流量传感器状态:正常";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "流量传感器状态:故障";
        str += ";";
        is_err = true;
    }

    //D6
    flag = (MU_INT8)((err >> 9) & 0x01);
    if (flag == 0x00)
    {
        str += "进水温度传感器状态:正常";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "进水温度传感器状态:故障";
        str += ";";
        is_err = true;
    }

    //D7
    flag = (MU_INT8)((err >> 8) & 0x01);
    if (flag == 0x00)
    {
        str += "回水温度传感器状态:正常";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "回水温度传感器状态:故障";
        str += ";";
        is_err = true;
    }

    //D8
    flag = (MU_INT8)((err >> 7) & 0x01);
    if (flag == 0x00)
    {
        str += "积算仪状态:正常";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "积算仪状态:故障";
        str += ";";
        is_err = true;
    }

    //D9
    flag = (MU_INT8)((err >> 6) & 0x01);
    if (flag == 0x00)
    {
        str += "设备状态:上电";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "设备状态:未上电";
        str += ";";
        is_err = true;
    }

    //D10D11
    flag = (MU_INT8)((err >> 4) & 0x03);
    if (flag == 0x00)
    {
        str += "报警器状态:报警";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "报警器状态:上电";
        str += ";";
        is_err = true;
    }
    else if (flag == 0x11)
    {
        str += "报警器状态:未上电";
        str += ";";
        is_err = true;
    }

    //D12
    flag = (MU_INT8)((err >> 3) & 0x01);
    if (flag == 0x00)
    {
        str += "水表测量状态:冷水";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "水表测量状态:热水";
        str += ";";
        is_err = true;
    }

    //D13
    flag = (MU_INT8)((err >> 2) & 0x01);
    if (flag == 0x00)
    {
        str += "通信状态:正常";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "通信状态:故障";
        str += ";";
        is_err = true;
    }

    //D14
    flag = (MU_INT8)((err >> 1) & 0x01);
    if (flag == 0x00)
    {
        str += "EEPROM状态:正常";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "EEPROM状态:故障";
        str += ";";
        is_err = true;
    }

    //D15
    flag = (MU_INT8)((err >> 0) & 0x01);
    if (flag == 0x00)
    {
        str += "水表结算日冷热状态:冷水";
        str += ";";
    }
    else if (flag == 0x01)
    {
        str += "水表结算日冷热状态:热水";
        str += ";";
        is_err = true;
    }

    if (is_err == false)
    {
        str = "正常";
    }
    else
    {
        str.substr(0,str.length()-1);
    }

    return str;
}


int ExtractWayData(u_w_measure& w_measure,e_dev_type dev_type,char* data)
{
#if 1
    return 0;
#else
    /*
    int cnt = 0;
    bool ret = 0;
    int ind = 0;

    int len = strlen(data);
    switch(dev_type)
    {
    case AM_3PHASE:
        cnt = sscanf(data,
            "%12lld"       //meter_time,
            "%4x"          //meter_state,
            "%4x"          //threshold_state,
            "%4x"          //volt_change,
            "%4x"          //current_change,
            "%4x"          //frequency,
            "%4x"          //phase_volt,
            "%4x"          //phase_volt
            "%4x"          //phase_volt
            "%4x"          //line_volt,
            "%4x"          //line_volt,
            "%4x"          //line_volt,
            "%4x"          //phrase_current,
            "%4x"          //phrase_current
            "%4x"          //phrase_current
            "%4x"          //phrase_active,
            "%4x"          //phrase_active,
            "%4x"          //phrase_active,
            "%4x"          //phrase_active,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_apparent,
            "%4x"          //phrase_apparent,
            "%4x"          //phrase_apparent,
            "%4x"          //phrase_apparent,
            "%4x"          //factor,
            "%4x"          //factor,
            "%4x"          //factor,
            "%4x"          //factor,
            "%4x"          //current_zero,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x" 		   //positive_active_energy
            "%8x" 		   //positive_active_energy
            "%8x" 		   //positive_active_energy
            "%8x" 		   //positive_active_energy
            "%8x" 		   //positive_active_energy
            "%8x" 		   //reverse_active_energy
            "%8x" 		   //reverse_active_energy
            "%8x" 		   //reverse_active_energy
            "%8x" 		   //reverse_active_energy
            "%8x" 		   //reverse_active_energy
            "%8x" 		   //combine_reactive_energy[0]
            "%8x" 		   //combine_reactive_energy[0]
            "%8x" 		   //combine_reactive_energy[0]
            "%8x" 		   //combine_reactive_energy[0]
            "%8x" 		   //combine_reactive_energy[0]
            "%8x" 		   //combine_reactive_energy[1]
            "%8x" 		   //combine_reactive_energy[1]
            "%8x" 		   //combine_reactive_energy[1]
            "%8x" 		   //combine_reactive_energy[1]
            "%8x", 		   //combine_reactive_energy[1]
            &w_measure.am_3phase.meter_time,
            &w_measure.am_3phase.meter_state,
            &w_measure.am_3phase.threshold_state,
            &w_measure.am_3phase.volt_change,
            &w_measure.am_3phase.current_change,
            &w_measure.am_3phase.frequency,
            &w_measure.am_3phase.phase_volt[0],
            &w_measure.am_3phase.phase_volt[1],
            &w_measure.am_3phase.phase_volt[2],
            &w_measure.am_3phase.line_volt[0],
            &w_measure.am_3phase.line_volt[1],
            &w_measure.am_3phase.line_volt[2],
            &w_measure.am_3phase.phrase_current[0],
            &w_measure.am_3phase.phrase_current[1],
            &w_measure.am_3phase.phrase_current[2],
            &w_measure.am_3phase.phrase_active[0],
            &w_measure.am_3phase.phrase_active[1],
            &w_measure.am_3phase.phrase_active[2],
            &w_measure.am_3phase.phrase_active[3],
            &w_measure.am_3phase.phrase_reactive[0],
            &w_measure.am_3phase.phrase_reactive[1],
            &w_measure.am_3phase.phrase_reactive[2],
            &w_measure.am_3phase.phrase_reactive[3],
            &w_measure.am_3phase.phrase_apparent[0],
            &w_measure.am_3phase.phrase_apparent[1],
            &w_measure.am_3phase.phrase_apparent[2],
            &w_measure.am_3phase.phrase_apparent[3],
            &w_measure.am_3phase.factor[0],
            &w_measure.am_3phase.factor[1],
            &w_measure.am_3phase.factor[2],
            &w_measure.am_3phase.factor[3],
            &w_measure.am_3phase.n_current,
            &w_measure.am_3phase.combine_active_energy[0],
            &w_measure.am_3phase.combine_active_energy[1],
            &w_measure.am_3phase.combine_active_energy[2],
            &w_measure.am_3phase.combine_active_energy[3],
            &w_measure.am_3phase.combine_active_energy[4],
            &w_measure.am_3phase.positive_active_energy[0],
            &w_measure.am_3phase.positive_active_energy[1],
            &w_measure.am_3phase.positive_active_energy[2],
            &w_measure.am_3phase.positive_active_energy[3],
            &w_measure.am_3phase.positive_active_energy[4],
            &w_measure.am_3phase.reverse_active_energy[0],
            &w_measure.am_3phase.reverse_active_energy[1],
            &w_measure.am_3phase.reverse_active_energy[2],
            &w_measure.am_3phase.reverse_active_energy[3],
            &w_measure.am_3phase.reverse_active_energy[4],
            &w_measure.am_3phase.combine_reactive_energy[0][0],
            &w_measure.am_3phase.combine_reactive_energy[0][1],
            &w_measure.am_3phase.combine_reactive_energy[0][2],
            &w_measure.am_3phase.combine_reactive_energy[0][3],
            &w_measure.am_3phase.combine_reactive_energy[0][4],
            &w_measure.am_3phase.combine_reactive_energy[1][0],
            &w_measure.am_3phase.combine_reactive_energy[1][1],
            &w_measure.am_3phase.combine_reactive_energy[1][2],
            &w_measure.am_3phase.combine_reactive_energy[1][3],
            &w_measure.am_3phase.combine_reactive_energy[1][4]
        );

        if(AM_THREE_PHASE_NUM == cnt && len == (12 + 4 * 31 + 8 * 25))
        {
            ret = true;
        }

        break;

    case AM_1PHASE:
        cnt = sscanf(data,
            "%12lld"       //meter_time,
            "%4x"          //meter_state,
            "%4x"          //threshold_state,
            "%4x"          //volt_change,
            "%4x"          //current_change,
            "%4x"          //frequency,
            "%4x"          //volt,
            "%4x"          //current,
            "%4x"          //active,
            "%4x"          //reactive,
            "%4x"          //apparent,
            "%4x"          //factor,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //reverse_active_energy,
            "%8x"          //reverse_active_energy,
            "%8x"          //reverse_active_energy,
            "%8x"          //reverse_active_energy,
            "%8x"          //reverse_active_energy,
            "%8x"          //combine_reactive_energy[0],
            "%8x"          //combine_reactive_energy[0],
            "%8x"          //combine_reactive_energy[0],
            "%8x"          //combine_reactive_energy[0],
            "%8x"          //combine_reactive_energy[0],
            "%8x"          //combine_reactive_energy[1],
            "%8x"          //combine_reactive_energy[1],
            "%8x"          //combine_reactive_energy[1],
            "%8x"          //combine_reactive_energy[1],
            "%8x",         //combine_reactive_energy[1],
            &w_measure.am_1phase.meter_time,
            &w_measure.am_1phase.meter_state,
            &w_measure.am_1phase.threshold_state,
            &w_measure.am_1phase.volt_change,
            &w_measure.am_1phase.current_change,
            &w_measure.am_1phase.frequency,
            &w_measure.am_1phase.volt,
            &w_measure.am_1phase.current,
            &w_measure.am_1phase.active,
            &w_measure.am_1phase.reactive,
            &w_measure.am_1phase.apparent,
            &w_measure.am_1phase.factor,
            &w_measure.am_1phase.combine_active_energy[0],
            &w_measure.am_1phase.combine_active_energy[1],
            &w_measure.am_1phase.combine_active_energy[2],
            &w_measure.am_1phase.combine_active_energy[3],
            &w_measure.am_1phase.combine_active_energy[4],
            &w_measure.am_1phase.positive_active_energy[0],
            &w_measure.am_1phase.positive_active_energy[1],
            &w_measure.am_1phase.positive_active_energy[2],
            &w_measure.am_1phase.positive_active_energy[3],
            &w_measure.am_1phase.positive_active_energy[4],
            &w_measure.am_1phase.reverse_active_energy[0],
            &w_measure.am_1phase.reverse_active_energy[1],
            &w_measure.am_1phase.reverse_active_energy[2],
            &w_measure.am_1phase.reverse_active_energy[3],
            &w_measure.am_1phase.reverse_active_energy[4],
            &w_measure.am_1phase.combine_reactive_energy[0][0],
            &w_measure.am_1phase.combine_reactive_energy[0][1],
            &w_measure.am_1phase.combine_reactive_energy[0][2],
            &w_measure.am_1phase.combine_reactive_energy[0][3],
            &w_measure.am_1phase.combine_reactive_energy[0][4],
            &w_measure.am_1phase.combine_reactive_energy[1][0],
            &w_measure.am_1phase.combine_reactive_energy[1][1],
            &w_measure.am_1phase.combine_reactive_energy[1][2],
            &w_measure.am_1phase.combine_reactive_energy[1][3],
            &w_measure.am_1phase.combine_reactive_energy[1][4]
        );

        if(AM_ONE_PHASE_NUM == cnt && len == (12 + 4*11 + 8*25))
        {
            ret = true;
        }

        break;

    case AM_DC:
        cnt = sscanf(data,
            "%12lld"       //meter_time,
            "%4x"          //meter_state,
            "%4x"          //volt_change,
            "%4x"          //current_change,
            "%4x"          //volt,
            "%4x"          //current,
            "%4x"          //active,
            "%8x",          //energy,
            &w_measure.am_dc.meter_time,
            &w_measure.am_dc.meter_state,
            &w_measure.am_dc.volt_change,
            &w_measure.am_dc.current_change,
            &w_measure.am_dc.volt,
            &w_measure.am_dc.current,
            &w_measure.am_dc.active,
            &w_measure.am_dc.energy);

        if(AM_DC_NUM == cnt && len == (12 + 4*6 + 8*1))
        {
            ret = true;
        }

        break;

    case AM_HARM:
        cnt = sscanf(data,
            "%12lld"       //meter_time,
            "%4x"          //meter_state,
            "%4x"          //threshold_state,
            "%4x"          //volt_change,
            "%4x"          //current_change,
            "%4x"          //frequency,
            "%4x"          //phase_volt,
            "%4x"          //phase_volt,
            "%4x"          //phase_volt,
            "%4x"          //line_volt,
            "%4x"          //line_volt,
            "%4x"          //line_volt,
            "%4x"          //phrase_current,
            "%4x"          //phrase_current,
            "%4x"          //phrase_current,
            "%4x"          //phrase_active,
            "%4x"          //phrase_active,
            "%4x"          //phrase_active,
            "%4x"          //phrase_active,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_reactive,
            "%4x"          //phrase_apparent,
            "%4x"          //phrase_apparent,
            "%4x"          //phrase_apparent,
            "%4x"          //phrase_apparent,
            "%4x"          //factor,
            "%4x"          //factor,
            "%4x"          //factor,
            "%4x"          //factor,
            "%4x"          //n_current,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //combine_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //positive_active_energy,
            "%8x"          //combine_reactive_energy,
            "%8x"          //combine_reactive_energy,
            "%8x"          //combine_reactive_energy,
            "%8x"          //combine_reactive_energy,
            "%8x"          //combine_reactive_energy,
            "%4x"          //phrase_original_volt
            "%4x"          //phrase_original_volt
            "%4x"          //phrase_original_volt
            "%4x"          //,phrase_original_current
            "%4x"          //,phrase_original_current
            "%4x"          //,phrase_original_current
            "%4x"          //,phrase_original_active
            "%4x"          //,phrase_original_active
            "%4x"          //,phrase_original_active
            "%4x"          //,phrase_original_active
            "%4x"          //,phrase_harm_volt
            "%4x"          //,phrase_harm_volt
            "%4x"          //,phrase_harm_volt
            "%4x"          //,phrase_harm_current
            "%4x"          //,phrase_harm_current
            "%4x"          //,phrase_harm_current
            "%4x"          //phrase_harm_active,
            "%4x"          //phrase_harm_active,
            "%4x"          //phrase_harm_active,
            "%4x"          //phrase_harm_active,
            "%4x"          //total_distor,
            "%4x"          //total_distor,
            "%4x"          //total_distor,
            "%4x"          //total_distor,
            "%4x"          //total_distor,
            "%4x"          //total_distor,
            "%4x"          //total_distor,
            "%4x"          //total_distor,
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u[0],
            "%4x"          //harm_u_parity[0],
            "%4x"          //harm_u_parity[0],
            "%4x"          //harm_u_ridge[0],
            "%4x"          //harm_u_tele[0],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u[1],
            "%4x"          //harm_u_parity[1],
            "%4x"          //harm_u_parity[1],
            "%4x"          //harm_u_ridge[1],
            "%4x"          //harm_u_tele[1],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u[2],
            "%4x"          //harm_u_parity[2],
            "%4x"          //harm_u_parity[2],
            "%4x"          //harm_u_ridge[2],
            "%4x"          //harm_u_tele[2],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia[0],
            "%4x"          //harm_ia_parity[0],
            "%4x"          //harm_ia_parity[0],
            "%4x"          //harm_ia_K[0],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia[1],
            "%4x"          //harm_ia_parity[1],
            "%4x"          //harm_ia_parity[1],
            "%4x"          //harm_ia_K[1],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia[2],
            "%4x"          //harm_ia_parity[2],
            "%4x"          //harm_ia_parity[2],
            "%4x",          //harm_ia_K[2],
            &w_measure.am_harm.meter_time,
            &w_measure.am_harm.meter_state,
            &w_measure.am_harm.threshold_state,
            &w_measure.am_harm.volt_change,
            &w_measure.am_harm.current_change,
            &w_measure.am_harm.frequency,
            &w_measure.am_harm.phase_volt[0],
            &w_measure.am_harm.phase_volt[1],
            &w_measure.am_harm.phase_volt[2],
            &w_measure.am_harm.line_volt[0],
            &w_measure.am_harm.line_volt[1],
            &w_measure.am_harm.line_volt[2],
            &w_measure.am_harm.phrase_current[0],
            &w_measure.am_harm.phrase_current[1],
            &w_measure.am_harm.phrase_current[2],
            &w_measure.am_harm.phrase_active[0],
            &w_measure.am_harm.phrase_active[1],
            &w_measure.am_harm.phrase_active[2],
            &w_measure.am_harm.phrase_active[3],
            &w_measure.am_harm.phrase_reactive[0],
            &w_measure.am_harm.phrase_reactive[1],
            &w_measure.am_harm.phrase_reactive[2],
            &w_measure.am_harm.phrase_reactive[3],
            &w_measure.am_harm.phrase_apparent[0],
            &w_measure.am_harm.phrase_apparent[1],
            &w_measure.am_harm.phrase_apparent[2],
            &w_measure.am_harm.phrase_apparent[3],
            &w_measure.am_harm.factor[0],
            &w_measure.am_harm.factor[1],
            &w_measure.am_harm.factor[2],
            &w_measure.am_harm.factor[3],
            &w_measure.am_harm.n_current,
            &w_measure.am_harm.combine_active_energy[0],
            &w_measure.am_harm.combine_active_energy[1],
            &w_measure.am_harm.combine_active_energy[2],
            &w_measure.am_harm.combine_active_energy[3],
            &w_measure.am_harm.combine_active_energy[4],
            &w_measure.am_harm.positive_active_energy[0],
            &w_measure.am_harm.positive_active_energy[1],
            &w_measure.am_harm.positive_active_energy[2],
            &w_measure.am_harm.positive_active_energy[3],
            &w_measure.am_harm.positive_active_energy[4],
            &w_measure.am_harm.reverse_active_energy[0],
            &w_measure.am_harm.reverse_active_energy[1],
            &w_measure.am_harm.reverse_active_energy[2],
            &w_measure.am_harm.reverse_active_energy[3],
            &w_measure.am_harm.reverse_active_energy[4],
            &w_measure.am_harm.combine_reactive_energy[0][0],
            &w_measure.am_harm.combine_reactive_energy[0][1],
            &w_measure.am_harm.combine_reactive_energy[0][2],
            &w_measure.am_harm.combine_reactive_energy[0][3],
            &w_measure.am_harm.combine_reactive_energy[0][4],
            &w_measure.am_harm.combine_reactive_energy[1][0],
            &w_measure.am_harm.combine_reactive_energy[1][1],
            &w_measure.am_harm.combine_reactive_energy[1][2],
            &w_measure.am_harm.combine_reactive_energy[1][3],
            &w_measure.am_harm.combine_reactive_energy[1][4],
            &w_measure.am_harm.phrase_original_volt[0],
            &w_measure.am_harm.phrase_original_volt[1],
            &w_measure.am_harm.phrase_original_volt[2],
            &w_measure.am_harm.phrase_original_current[0],
            &w_measure.am_harm.phrase_original_current[1],
            &w_measure.am_harm.phrase_original_current[2],
            &w_measure.am_harm.phrase_original_active[0],
            &w_measure.am_harm.phrase_original_active[1],
            &w_measure.am_harm.phrase_original_active[2],
            &w_measure.am_harm.phrase_original_active[3],
            &w_measure.am_harm.phrase_harm_volt[0],
            &w_measure.am_harm.phrase_harm_volt[1],
            &w_measure.am_harm.phrase_harm_volt[2],
            &w_measure.am_harm.phrase_harm_current[0],
            &w_measure.am_harm.phrase_harm_current[1],
            &w_measure.am_harm.phrase_harm_current[2],
            &w_measure.am_harm.phrase_harm_active[0],
            &w_measure.am_harm.phrase_harm_active[1],
            &w_measure.am_harm.phrase_harm_active[2],
            &w_measure.am_harm.phrase_harm_active[3],
            &w_measure.am_harm.total_distor[0],
            &w_measure.am_harm.total_distor[1],
            &w_measure.am_harm.total_distor[2],
            &w_measure.am_harm.total_distor[3],
            &w_measure.am_harm.total_distor[4],
            &w_measure.am_harm.total_distor[5],
            &w_measure.am_harm.total_distor[6],
            &w_measure.am_harm.total_distor[7],
            &w_measure.am_harm.harm_u[0][0],
            &w_measure.am_harm.harm_u[0][1],
            &w_measure.am_harm.harm_u[0][2],
            &w_measure.am_harm.harm_u[0][3],
            &w_measure.am_harm.harm_u[0][4],
            &w_measure.am_harm.harm_u[0][5],
            &w_measure.am_harm.harm_u[0][6],
            &w_measure.am_harm.harm_u[0][7],
            &w_measure.am_harm.harm_u[0][8],
            &w_measure.am_harm.harm_u[0][9],
            &w_measure.am_harm.harm_u[0][10],
            &w_measure.am_harm.harm_u[0][11],
            &w_measure.am_harm.harm_u[0][12],
            &w_measure.am_harm.harm_u[0][13],
            &w_measure.am_harm.harm_u[0][14],
            &w_measure.am_harm.harm_u[0][15],
            &w_measure.am_harm.harm_u[0][16],
            &w_measure.am_harm.harm_u[0][17],
            &w_measure.am_harm.harm_u[0][18],
            &w_measure.am_harm.harm_u[0][19],
            &w_measure.am_harm.harm_u_parity[0][0],
            &w_measure.am_harm.harm_u_parity[0][1],
            &w_measure.am_harm.harm_u_ridge[0],
            &w_measure.am_harm.harm_u_tele[0],
            &w_measure.am_harm.harm_u[1][0],
            &w_measure.am_harm.harm_u[1][1],
            &w_measure.am_harm.harm_u[1][2],
            &w_measure.am_harm.harm_u[1][3],
            &w_measure.am_harm.harm_u[1][4],
            &w_measure.am_harm.harm_u[1][5],
            &w_measure.am_harm.harm_u[1][6],
            &w_measure.am_harm.harm_u[1][7],
            &w_measure.am_harm.harm_u[1][8],
            &w_measure.am_harm.harm_u[1][9],
            &w_measure.am_harm.harm_u[1][10],
            &w_measure.am_harm.harm_u[1][11],
            &w_measure.am_harm.harm_u[1][12],
            &w_measure.am_harm.harm_u[1][13],
            &w_measure.am_harm.harm_u[1][14],
            &w_measure.am_harm.harm_u[1][15],
            &w_measure.am_harm.harm_u[1][16],
            &w_measure.am_harm.harm_u[1][17],
            &w_measure.am_harm.harm_u[1][18],
            &w_measure.am_harm.harm_u[1][19],
            &w_measure.am_harm.harm_u_parity[1][0],
            &w_measure.am_harm.harm_u_parity[1][1],
            &w_measure.am_harm.harm_u_ridge[1],
            &w_measure.am_harm.harm_u_tele[1],
            &w_measure.am_harm.harm_u[2][0],
            &w_measure.am_harm.harm_u[2][1],
            &w_measure.am_harm.harm_u[2][2],
            &w_measure.am_harm.harm_u[2][3],
            &w_measure.am_harm.harm_u[2][4],
            &w_measure.am_harm.harm_u[2][5],
            &w_measure.am_harm.harm_u[2][6],
            &w_measure.am_harm.harm_u[2][7],
            &w_measure.am_harm.harm_u[2][8],
            &w_measure.am_harm.harm_u[2][9],
            &w_measure.am_harm.harm_u[2][10],
            &w_measure.am_harm.harm_u[2][11],
            &w_measure.am_harm.harm_u[2][12],
            &w_measure.am_harm.harm_u[2][13],
            &w_measure.am_harm.harm_u[2][14],
            &w_measure.am_harm.harm_u[2][15],
            &w_measure.am_harm.harm_u[2][16],
            &w_measure.am_harm.harm_u[2][17],
            &w_measure.am_harm.harm_u[2][18],
            &w_measure.am_harm.harm_u[2][19],
            &w_measure.am_harm.harm_u_parity[2][0],
            &w_measure.am_harm.harm_u_parity[2][1],
            &w_measure.am_harm.harm_u_ridge[2],
            &w_measure.am_harm.harm_u_tele[2],
            &w_measure.am_harm.harm_i[0][0],
            &w_measure.am_harm.harm_i[0][1],
            &w_measure.am_harm.harm_i[0][2],
            &w_measure.am_harm.harm_i[0][3],
            &w_measure.am_harm.harm_i[0][4],
            &w_measure.am_harm.harm_i[0][5],
            &w_measure.am_harm.harm_i[0][6],
            &w_measure.am_harm.harm_i[0][7],
            &w_measure.am_harm.harm_i[0][8],
            &w_measure.am_harm.harm_i[0][9],
            &w_measure.am_harm.harm_i[0][10],
            &w_measure.am_harm.harm_i[0][11],
            &w_measure.am_harm.harm_i[0][12],
            &w_measure.am_harm.harm_i[0][13],
            &w_measure.am_harm.harm_i[0][14],
            &w_measure.am_harm.harm_i[0][15],
            &w_measure.am_harm.harm_i[0][16],
            &w_measure.am_harm.harm_i[0][17],
            &w_measure.am_harm.harm_i[0][18],
            &w_measure.am_harm.harm_i[0][19],
            &w_measure.am_harm.harm_i_parity[0][0],
            &w_measure.am_harm.harm_i_parity[0][1],
            &w_measure.am_harm.harm_i_K[0],
            &w_measure.am_harm.harm_i[1][0],
            &w_measure.am_harm.harm_i[1][1],
            &w_measure.am_harm.harm_i[1][2],
            &w_measure.am_harm.harm_i[1][3],
            &w_measure.am_harm.harm_i[1][4],
            &w_measure.am_harm.harm_i[1][5],
            &w_measure.am_harm.harm_i[1][6],
            &w_measure.am_harm.harm_i[1][7],
            &w_measure.am_harm.harm_i[1][8],
            &w_measure.am_harm.harm_i[1][9],
            &w_measure.am_harm.harm_i[1][10],
            &w_measure.am_harm.harm_i[1][11],
            &w_measure.am_harm.harm_i[1][12],
            &w_measure.am_harm.harm_i[1][13],
            &w_measure.am_harm.harm_i[1][14],
            &w_measure.am_harm.harm_i[1][15],
            &w_measure.am_harm.harm_i[1][16],
            &w_measure.am_harm.harm_i[1][17],
            &w_measure.am_harm.harm_i[1][18],
            &w_measure.am_harm.harm_i[1][19],
            &w_measure.am_harm.harm_i_parity[1][0],
            &w_measure.am_harm.harm_i_parity[1][1],
            &w_measure.am_harm.harm_i_K[1],
            &w_measure.am_harm.harm_i[2][0],
            &w_measure.am_harm.harm_i[2][1],
            &w_measure.am_harm.harm_i[2][2],
            &w_measure.am_harm.harm_i[2][3],
            &w_measure.am_harm.harm_i[2][4],
            &w_measure.am_harm.harm_i[2][5],
            &w_measure.am_harm.harm_i[2][6],
            &w_measure.am_harm.harm_i[2][7],
            &w_measure.am_harm.harm_i[2][8],
            &w_measure.am_harm.harm_i[2][9],
            &w_measure.am_harm.harm_i[2][10],
            &w_measure.am_harm.harm_i[2][11],
            &w_measure.am_harm.harm_i[2][12],
            &w_measure.am_harm.harm_i[2][13],
            &w_measure.am_harm.harm_i[2][14],
            &w_measure.am_harm.harm_i[2][15],
            &w_measure.am_harm.harm_i[2][16],
            &w_measure.am_harm.harm_i[2][17],
            &w_measure.am_harm.harm_i[2][18],
            &w_measure.am_harm.harm_i[2][19],
            &w_measure.am_harm.harm_i_parity[2][0],
            &w_measure.am_harm.harm_i_parity[2][1],
            &w_measure.am_harm.harm_i_K[2]
        );
        if( AM_HARM_NUM == cnt && ( len == ( 12 + 4*(31+169)  + 8*15 ) ) )
        {
            ret = true;
        }
        break;

    case WM:
        ind = 0;
        cnt = sscanf(data,
            "%12lld"       //meter_time,
            "%4x",          //meter_state,
            &w_measure.water_meter.meter_time,
            &w_measure.water_meter.meter_state);

        ind += 16;
        str_ten_hex_to_float((char*)&data[ind],w_measure.water_meter.cur_data);

        if(len == (12+4+8))
        {
            ret = true;
        }
        break;

    case GM:
        ind = 0;
        cnt = sscanf((char*)&data[ind],
            "%12lld"       //meter_time,
            "%4x"          //meter_state,
            ,
            &w_measure.gass_meter.meter_time,
            &w_measure.gass_meter.meter_state);

        ind += 16;

        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.instantaneous_flow);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.frequency);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.diff_pressure);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.pressure);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.temperature);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.inlet_temperature);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.outlet_temperature);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.fluid_density);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.instantaneous_heat);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.enthalpy);
        ind += 8;
        str_16hex_to_float((char*)&data[ind],w_measure.gass_meter.enthalpy_diff);
        ind += 8;
        cnt = sscanf((char*)&data[ind],
            "%8x"
            "%8x"
            "%8x",
            &w_measure.gass_meter.cumulative_flow,
            &w_measure.gass_meter.accumulated_heat,
            &w_measure.gass_meter.cumulative_diff_heat);


        if(len == (12 + 4 + 8*14))
        {
            ret = true;
        }
        break;

    default:
        break;
    }

    if (ret == true)
    {
        return 0;
    }
    else
    {
        return -2;
    }
    */
#endif
}

int ConvertDataToCvs( u_w_measure& w_measure,e_dev_type dev_type,char* cvs_data )
{
#if 1
    return 0;
#else
    /*
    bool ret = 0;
    int tmp_len = 0;
    CString str;
    MU_INT64 meter_time = 0;
    int yy = 0;
    int mm = 0;
    int dd = 0;
    int hh= 0;
    int mi = 0;
    int se = 0;
    switch(dev_type)
    {
    case AM_3PHASE:
        meter_time = w_measure.am_3phase.meter_time;//151020130030
        yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
        mm = meter_time / 100 / 100 / 100 / 100 % 100;
        dd = meter_time / 100 / 100 / 100 % 100;
        hh = meter_time / 100 / 100 % 100;
        mi = meter_time / 100 % 100;
        se = meter_time  % 100;
        tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
        str = AnalyzeMeterState(w_measure.am_3phase.meter_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_3phase.meter_state,str.GetString());
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_3phase.threshold_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_3phase.volt_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_3phase.current_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.frequency * gTwoUnit);
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_3phase.phase_volt[i] * gOneUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_3phase.line_volt[i] * gOneUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_current[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_active[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_reactive[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.phrase_apparent[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.factor[i] * gTwoUnit);
        }
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.n_current * gTwoUnit);
        for	(int i = 0; i < 5; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.combine_active_energy[i] * gTwoUnit);
        }
        for	(int i = 0; i < 5; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.positive_active_energy[i] * gTwoUnit);
        }
        for	(int i = 0; i < 5; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.reverse_active_energy[i] * gTwoUnit);
        }
        for	(int i = 0; i < 2; i ++)
        {
            for	(int j = 0; j < 2; j ++)
            {
                tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_3phase.combine_reactive_energy[i][j] * gTwoUnit);
            }
        }

        break;

    case AM_1PHASE:
        meter_time = w_measure.am_1phase.meter_time;//151020130030
        yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
        mm = meter_time / 100 / 100 / 100 / 100 % 100;
        dd = meter_time / 100 / 100 / 100 % 100;
        hh = meter_time / 100 / 100 % 100;
        mi = meter_time / 100 % 100;
        se = meter_time  % 100;
        tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
        str = AnalyzeMeterState(w_measure.am_1phase.meter_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_1phase.meter_state,str.GetString());
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_1phase.threshold_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_1phase.volt_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_1phase.current_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.frequency * gTwoUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_1phase.volt * gOneUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.current * gTwoUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.active * gTwoUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.reactive * gTwoUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.apparent * gTwoUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.factor * gTwoUnit);
        for	(int i = 0; i < 5; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.combine_active_energy[i] * gTwoUnit);
        }
        for	(int i = 0; i < 5; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.positive_active_energy[i] * gTwoUnit);
        }
        for	(int i = 0; i < 5; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.reverse_active_energy[i] * gTwoUnit);
        }
        for	(int i = 0; i < 2; i ++)
        {
            for	(int j = 0; j < 2; j ++)
            {
                tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_1phase.combine_reactive_energy[i][j] * gTwoUnit);
            }
        }

        break;

    case AM_DC:
        meter_time = w_measure.am_dc.meter_time;//151020130030
        yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
        mm = meter_time / 100 / 100 / 100 / 100 % 100;
        dd = meter_time / 100 / 100 / 100 % 100;
        hh = meter_time / 100 / 100 % 100;
        mi = meter_time / 100 % 100;
        se = meter_time  % 100;
        tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
        str = AnalyzeMeterState(w_measure.am_dc.meter_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_dc.meter_state,str.GetString());
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_dc.volt_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_dc.current_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_dc.volt * gOneUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_dc.current * gTwoUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_dc.active * gTwoUnit);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_dc.energy * gTwoUnit);

        break;

    case AM_HARM:
        meter_time = w_measure.am_harm.meter_time;//151020130030
        yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
        mm = meter_time / 100 / 100 / 100 / 100 % 100;
        dd = meter_time / 100 / 100 / 100 % 100;
        hh = meter_time / 100 / 100 % 100;
        mi = meter_time / 100 % 100;
        se = meter_time  % 100;
        tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
        str = AnalyzeMeterState(w_measure.am_harm.meter_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.am_harm.meter_state,str.GetString());
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_harm.threshold_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_harm.volt_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.am_harm.current_change);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.frequency * gTwoUnit);
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.phase_volt[i] * gOneUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.line_volt[i] * gOneUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_current[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_active[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_reactive[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_apparent[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.factor[i] * gTwoUnit);
        }
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.n_current * gTwoUnit);
        for	(int i = 0; i < 5; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.combine_active_energy[i] * gTwoUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.phrase_original_volt[i] * gOneUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_original_current[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_original_active[i] * gTwoUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.am_harm.phrase_harm_volt[i] * gOneUnit);
        }
        for	(int i = 0; i < 3; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_harm_current[i] * gTwoUnit);
        }
        for	(int i = 0; i < 4; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.phrase_harm_active[i] * gTwoUnit);
        }
        for	(int i = 0; i < 8; i ++)
        {
            tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.total_distor[i] * gTwoUnit);
        }

        for	(int i = 0; i < 3; i ++)
        {
            for	(int j = 0; j < 20; j ++)
            {
                tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_u[i][j] * gTwoUnit);
            }
            for	(int j = 0; j < 2; j ++)
            {
                tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_u_parity[i][j] * gTwoUnit);
            }
            tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.am_harm.harm_u_ridge[i] * gThreeUnit);
            tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.am_harm.harm_u_tele[i] * gThreeUnit);
        }

        for	(int i = 0; i < 3; i ++)
        {
            for	(int j = 0; j < 20; j ++)
            {
                tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_i[i][j] * gTwoUnit);
            }
            for	(int j = 0; j < 2; j ++)
            {
                tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.am_harm.harm_i_parity[i][j] * gTwoUnit);
            }
            tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.am_harm.harm_i_K[i] * gThreeUnit);
        }

        break;

    case WM:
        meter_time = w_measure.water_meter.meter_time;//151020130030
        yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
        mm = meter_time / 100 / 100 / 100 / 100 % 100;
        dd = meter_time / 100 / 100 / 100 % 100;
        hh = meter_time / 100 / 100 % 100;
        mi = meter_time / 100 % 100;
        se = meter_time  % 100;
        tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
        str = AnalyzeMeterState(w_measure.water_meter.meter_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.water_meter.meter_state,str.GetString());
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,",w_measure.water_meter.cur_data);
        break;

    case GM:
        meter_time = w_measure.gass_meter.meter_time;//151020130030
        yy = meter_time / 100 / 100 / 100 / 100 / 100 % 100;
        mm = meter_time / 100 / 100 / 100 / 100 % 100;
        dd = meter_time / 100 / 100 / 100 % 100;
        hh = meter_time / 100 / 100 % 100;
        mi = meter_time / 100 % 100;
        se = meter_time  % 100;
        tmp_len += sprintf(cvs_data + tmp_len, "20%02d-%02d-%02d %02d:%02d:%02d,",yy,mm,dd,hh,mi,se);
        str = AnalyzeMeterState(w_measure.gass_meter.meter_state);
        tmp_len += sprintf(cvs_data + tmp_len, "%04x(%s),", w_measure.gass_meter.meter_state,str.GetString());
        tmp_len += sprintf(cvs_data + tmp_len, "%.4f,", w_measure.gass_meter.instantaneous_flow);
        tmp_len += sprintf(cvs_data + tmp_len, "%.1f,", w_measure.gass_meter.frequency);
        tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.diff_pressure);
        tmp_len += sprintf(cvs_data + tmp_len, "%.4f,", w_measure.gass_meter.pressure);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.gass_meter.temperature);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.gass_meter.inlet_temperature);
        tmp_len += sprintf(cvs_data + tmp_len, "%.2f,", w_measure.gass_meter.outlet_temperature);
        tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.fluid_density);
        tmp_len += sprintf(cvs_data + tmp_len, "%.4f,", w_measure.gass_meter.instantaneous_heat);
        tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.enthalpy);
        tmp_len += sprintf(cvs_data + tmp_len, "%.3f,", w_measure.gass_meter.enthalpy_diff);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.gass_meter.cumulative_flow);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.gass_meter.accumulated_heat);
        tmp_len += sprintf(cvs_data + tmp_len, "%d,", w_measure.gass_meter.cumulative_diff_heat);


        break;
    default:
        break;
    }

    if (ret == true)
    {
        return 0;
    }
    else
    {
        return -2;
    }
*/
#endif
}

int pox_system(const char *cmd_line)
{
    int ret = 0;
    sighandler_t old_handler;
    old_handler = signal(SIGCHLD, SIG_DFL);
    ret = system(cmd_line);
    signal(SIGCHLD, old_handler);
    return ret;
}
