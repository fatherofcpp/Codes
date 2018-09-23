#pragma once
/*
全局变量，全局函数
*/
#include "DataTypeDef.h"
#include "common_qt.h"

//////////////////////////////////////
//全局常量
extern const double gOneUnit;
extern const double gTwoUnit;
extern const double gThreeUnit;
extern const double gFourUnit;
extern const double gConvUnit;

extern const int gOneReg;
extern const int gTwoReg;
extern const int gThreeReg;
extern const int gFourReg;
//////////////////////////////////////
//全局常量数值
//设备主类型
extern const int  gDevTypeNum;
extern const char gDevTypeName[E_DEV_TYPE_NUM][64];
extern const int gDevTypeValue[E_DEV_TYPE_NUM];
string GetDevTypeNameByValue(int type);
int GetDevTypeIndByValue(int type);
int GetDevTypeValueByName(string name);
string GetSaveFileNameByDevType(e_dev_type dev_type);
string GetSaveFileHeadByDevType(e_dev_type dev_type);
//////////////////////////////////////
//设备子类型
extern const int  gSubTypeNum;
extern char gSubTypeName[SUB_TYPE_NUM][64];//对于自定义子类型名称可修改
extern const int gSubTypeValue[SUB_TYPE_NUM];
extern int gWayCountValue[SUB_TYPE_NUM];//对于自定义子类型回路数可修改
int GetWayNumBySubTypeValue(int type);
string GetSubTypeNameByValue(int type);
int GetSubTypeValueByName(string name);
int GetSubTypeIndByValue(int type);
//根据子类型判断是否自定义子类型
bool IsDefSubType(int sub_type);
//清空自定义子类型的名称回路数信息;调用点：本地配置加载，串口加载，保存自定义子类型对话框
void ClearDefSubTypeNameWayCount();
//根据自定义子类型更新子类型名称
void SetSubTypeNameByInd(int sub_type_ind, char* sub_name);
//根据自定义子类型更新子类型回路数
void SetWayCountBySubTypeInd(int sub_type_ind, int way_count);
int GetWayCountBySubTypeInd(int sub_type_ind);
//根据子类型值查询Modbus自定义数组下标
int GetModBusArrIndBySubType(int val);
//////////////////////////////////////
//数据位
#define DATABIT_NUM 2
extern const int gDataBitNum;
extern const char gDataBitName[DATABIT_NUM][16];
extern const int gDataBitValue[DATABIT_NUM];
string GetDataBitNameByValue(int type);
int GetDataBitValueByName(string name);
int GetDataBitIndByValue(int type);
//////////////////////////////////////
//停止位
#define STOP_NUM 2
extern const int gStopNum;
extern const char gStopName[STOP_NUM][16];
extern const int gStopValue[STOP_NUM];
string GetStopNameByValue(int type);
int GetStopValueByName(string name);
int GetStopIndByValue(int type);
//////////////////////////////////////
//波特率
extern const int gBaudNum;
extern const char gBaudName[BAUD_NUM][16];
extern const int gBaudValue[BAUD_NUM];
string GetBaudNameByValue(int type);
int GetBaudValueByName(string name);
int GetBaudIndByValue(int type);
//////////////////////////////////////
//校验位
extern const int gParityNum;
extern const char gParityName[PARITY_NUM][16];
extern const int gParityValue[PARITY_NUM];
string GetParityNameByValue(int type);
int GetParityValueByName(string name);
int GetParityIndByValue(int type);
//////////////////////////////////////
//回路类型名称
#define WAY_DEV_TYPE_NAME_NUM 1
extern const int  gWayDevTypeNum;
extern const char gWayDevTypeName[WAY_DEV_TYPE_NAME_NUM][64];
extern const int gWayDevTypeValue[WAY_DEV_TYPE_NAME_NUM];
int GetWayDevTypeValueByName(string name);
string GetWayDevTypeNameByValue(int type);
//////////////////////////////////////
//采集器类型
#define COLTOR_TYPE_NUM 2
extern const int gColtorTypeNum;
extern const char gColtorTypeName[COLTOR_TYPE_NUM][64];
extern const int gColtorTypeValue[COLTOR_TYPE_NUM];
int GetColtorTypeValueByName(string name);
string GetColtorTypeNameByValue(int value);
int GetColtorTypeIndByValue(int value);
//////////////////////////////////////
//立即生效某项
#define EFFECT_NOW_NUM 2
extern const int gEffectNowNum;
extern const char gEffectNowName[EFFECT_NOW_NUM][64];
///////////////////////////////////////////////////////
//上报参数
//所有设备类型
extern const int gAllDevTypeReportParamNum;
extern char gAllDevTypeReportParamName[ALL_DEVTYPE_REPORT_PARAM_NUM][64];
extern const int gAllDevTypeReportParamValue[ALL_DEVTYPE_REPORT_PARAM_NUM];
int GetAllDevTypeReportParamValueByName(string name);
string GetAllDevTypeReportParamNameByValue(int type);
int GetAllDevTypeReportParamIndByValue(int val);
int ModifyReportParamUnitName(string unit_name, int val);//修改参量单位名称
string ParseUnitByValue(MU_INT8 val);
//////////////////////////////////////
//所有参数对应单位,精确度*1000，如0.01用10表示
extern const double gAllDevTypeReportParamUnit[ALL_DEVTYPE_REPORT_PARAM_NUM];
//////////////////////////////////////
//三相普通电表
#define AM_THREE_PHASE_REPORT_PARAM_NUM 56
extern const int gAmThreePhaseReportParamNum;
extern const int gAmThreePhaseReportParamValue[AM_THREE_PHASE_REPORT_PARAM_NUM];
//////////////////////////////////////
//单相普通电表
#define  AM_ONE_PHASE_REPORT_PARAM_NUM 36
extern const int gAmOnePhaseReportParamNum;
extern const int gAmOnePhaseReportParamValue[AM_ONE_PHASE_REPORT_PARAM_NUM];
//////////////////////////////////////
//直流普通电表
#define  AM_DC_REPORT_PARAM_NUM 7
extern const int gAmDcReportParamNum;
extern const int gAmDcReportParamValue[AM_DC_REPORT_PARAM_NUM];
//////////////////////////////////////
//三相谐波电表
#define  AM_HARM_REPORT_PARAM_NUM 225
extern const int gAmHaramReportParamNum;
extern const int gAmHaramReportParamValue[AM_HARM_REPORT_PARAM_NUM];
//////////////////////////////////////
//水汽表
#define  WATER_GASS_METER_REPORT_PARAM_NUM 3
extern const int gWaterGasMeterReportParamNum;
extern const int gWaterGasMeterReportParamValue[WATER_GASS_METER_REPORT_PARAM_NUM];
//////////////////////////////////////
//智能电操
#define  EOP_METER_REPORT_PARAM_NUM 5
extern const int gEopReportParamNum;
extern const int gEopReportParamValue[EOP_METER_REPORT_PARAM_NUM];
//////////////////////////////////////
//热量表
#define  HOT_METER_REPORT_PARAM_NUM 9
extern const int gHotMeterReportParamNum;
extern const int gHotMeterReportParamValue[HOT_METER_REPORT_PARAM_NUM];
///////////////////////////////////////////////////////
//全局常量数值
//状态:故障信息
#define ERR_NAME_NUM 7
extern const char gErrName[ERR_NAME_NUM][64];
//////////////////////////////////////
//三相谐波
#define  AM_HARM_NUM 226
extern const char gAmHarmName[AM_HARM_NUM][64];
//////////////////////////////////////
//三相普通
#define  AM_THREE_PHASE_NUM 57
extern const char gAm3phaseName[AM_THREE_PHASE_NUM][64];
//////////////////////////////////////
//单相普通
#define  AM_ONE_PHASE_NUM 37
extern const char gAm1phaseName[AM_ONE_PHASE_NUM][64];
//////////////////////////////////////
//直流普通
#define  AM_DC_NUM 8
extern const char gAmdcName[AM_DC_NUM][64];
//////////////////////////////////////
//水表
#define  WATER_METER_NAME_NUM 3
extern const char gWaterMeterName[WATER_METER_NAME_NUM][64];
//////////////////////////////////////
//气表
#define  GASS_METER_NAME_NUM 16
extern const char gGassMeterName[GASS_METER_NAME_NUM][64];
////////////////////////////////////////////////////////////
//参量单位
#define DEF_PROT_UNIT_NUM 4
extern const int  gDefProtUnitNum;
extern const char gDefProtUnitName[DEF_PROT_UNIT_NUM][64];
extern const double gDefProtUnitValue[DEF_PROT_UNIT_NUM];
string GetNameByValueDefProtUnit(double val);
int GetIndByValueDefProtUnit(double val);
double GetValueByNameDefProtUnit(string name);
int MapUnitDoubleToInt(double val);
double MapUnitIntToDouble(int val);
////////////////////////////////////////////////////////////
//自定义子类型回路选择
extern int  gDefProtWayChooseNum;
extern const char gDefProtWayChooseName[MAX_WAY_NUM][64];
extern const int gDefProtWayChooseValue[MAX_WAY_NUM];
int GetDefProtWayChooseValueByName(string name);
string GetDefProtWayChooseNameByValue(int type);
int GetDefProtWayChooseIndByValue(int val);
void ModifyDefProtWayChooseNum(int val);//修改参量单位名称
////////////////////////////////////////////////////////////
//全局变量
extern int gComNum;
extern e_company_type gCompanyType;
extern string gCurrentDir;
//////////////////////////////////////
extern QString gResArrowLeft;
extern QString gResArrowRight;
extern QString gResBackGroud;
extern QString gResBell;
extern QString gResBottom;
extern QString gResButtonBlue;
extern QString gResButtonGreen;
extern QString gResButtonRed;
extern QString gResCurrent;
extern QString gResLeftRight;
extern QString gResLightGreen;
extern QString gResLightRed;
extern QString gResNhb;
extern QString gResReturnBack;
extern QString gResTitleGreen;
extern QString gResTitleInit;
extern QString gResTop;
extern QString gResVoltage;
extern QString gResSwitchOn;
extern QString gResSwitchOff;
extern QString gResArrowUp;
extern QString gResArrowDown;

extern int gLeftChild;
extern int gTopChild;
extern int gWidthChild;
extern int gHeightChild;
//////////////////////////////////////
//全局函数
//通过way_mask判断当前way_ind是否选中
bool IsWayUsedByMask(int way_ind, MU_INT64 mask);
//根据设备id类型，找到参数1中的设备下标
int GetDevIndByIdSubType(str_outer_dev* outer_dev, MU_INT64 id, e_dev_sub_type sub_type, int port_ind);
st_dev_inf* GetDevInfByDevIdType(str_outer_dev* outer_dev, MU_INT64 id, int dev_type);
//根据掩码获取使用的回路下标数组
int GetWayListByMask(MU_INT64 way_mask, VEC_INT& vec_data, int max_way_count);
//根据设备类型获取参数1中的上报类型下标
int GetReportIndByDevType(e_dev_type dev_type, st_report_format* report_format);
//查找参数2中参数1对应的值
int GetMapSecondByFirst(int first, MAP_INT_INT map_val);
//打开文件，路径可选
string BootOpenFile(bool open = true);//TRUE表示打开,FALSE表示保存
//打开文件夹，路径可选
string BootOpenFolder();//打开文件夹

void ParseTskResult(int result);
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
QString Uint2DoubleQStr(MU_INT32 val, double unit);
QString Sint2DoubleQStr(MS_INT32 val, double unit);

st_way_data* GetWayInfByCode(str_outer_dev *outer_dev, int way_id, int dev_type);
st_dev_inf* GetDevInfByCode(str_outer_dev *outer_dev, int way_id, int dev_type);
MU_INT32* GetThreByWarnType(st_way_data* p_st_way_data, MU_INT32 warn_type);
//////////////////////

//实时数据状态
string AnalyzeMeterState(int state);

//阀值告警状态
string AnalyzeThresholdState(int state);

//提取实时/历史数据
int ExtractWayData(u_w_measure& w_measure,e_dev_type dev_type,char* data);
//组装cvs数据
int ConvertDataToCvs(u_w_measure& w_measure,e_dev_type dev_type,char* cvs_data);

//system
int pox_system(const char *cmd_line);

