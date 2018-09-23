#pragma once
/*
ȫ�ֱ�����ȫ�ֺ���
*/
#include "DataTypeDef.h"
#include "vector"
using namespace std;

//////////////////////////////////////
//ȫ�ֳ���
extern const double gOneUnit;
extern const double gTwoUnit;
extern const double gThreeUnit;
extern const double gFourUnit;
extern const double gConvUnit;
//////////////////////////////////////
//ȫ�ֳ�����ֵ
//�豸������
extern const int  gDevTypeNum;
extern const char gDevTypeName[DEV_TYPE_NUM][64]; 
extern const int gDevTypeValue[DEV_TYPE_NUM];
CString GetDevTypeNameByValue(int type);
int GetDevTypeIndByValue(int type);
int GetDevTypeValueByName(CString name);
CString GetSaveFileNameByDevType(e_dev_type dev_type);
CString GetSaveFileHeadByDevType(e_dev_type dev_type);
//////////////////////////////////////
//�豸������
extern const int  gSubTypeNum;
extern char gSubTypeName[SUB_TYPE_NUM][64];//�����Զ������������ƿ��޸�   
extern const int gSubTypeValue[SUB_TYPE_NUM];
extern int gWayCountValue[SUB_TYPE_NUM];//�����Զ��������ͻ�·�����޸�
int GetWayNumBySubTypeValue(int type);
CString GetSubTypeNameByValue(int type);
int GetSubTypeValueByName(CString name);
int GetSubTypeIndByValue(int type);
//�����������ж��Ƿ��Զ���������
bool IsDefSubType(int sub_type);
//����Զ��������͵����ƻ�·����Ϣ;���õ㣺�������ü��أ����ڼ��أ������Զ��������ͶԻ���
void ClearDefSubTypeNameWayCount();
//�����Զ��������͸�������������
void SetSubTypeNameByInd(int sub_type_ind, char* sub_name);
//�����Զ��������͸��������ͻ�·��
void SetWayCountBySubTypeInd(int sub_type_ind, int way_count);
int GetWayCountBySubTypeInd(int sub_type_ind);
//����������ֵ��ѯModbus�Զ��������±�
int GetModBusArrIndBySubType(int val);
//////////////////////////////////////
//����λ
#define DATABIT_NUM 2
extern const int gDataBitNum;
extern const char gDataBitName[DATABIT_NUM][16];
extern const int gDataBitValue[DATABIT_NUM];
CString GetDataBitNameByValue(int type);
int GetDataBitValueByName(string name);
int GetDataBitIndByValue(int type);
//////////////////////////////////////
//ֹͣλ
#define STOP_NUM 2
extern const int gStopNum;
extern const char gStopName[STOP_NUM][16];
extern const int gStopValue[STOP_NUM];
CString GetStopNameByValue(int type);
int GetStopValueByName(string name);
int GetStopIndByValue(int type);
//////////////////////////////////////
//������
extern const int gBaudNum;
extern const char gBaudName[BAUD_NUM][16];
extern const int gBaudValue[BAUD_NUM];
CString GetBaudNameByValue(int type);
int GetBaudValueByName(CString name);
int GetBaudIndByValue(int type);
//////////////////////////////////////
//У��λ
extern const int gParityNum;
extern const char gParityName[PARITY_NUM][16];
extern const int gParityValue[PARITY_NUM];
CString GetParityNameByValue(int type);
int GetParityValueByName(CString name);
int GetParityIndByValue(int type);
//////////////////////////////////////
//��·��������
#define WAY_DEV_TYPE_NAME_NUM 1
extern const int  gWayDevTypeNum;
extern const char gWayDevTypeName[WAY_DEV_TYPE_NAME_NUM][64];
extern const int gWayDevTypeValue[WAY_DEV_TYPE_NAME_NUM];
int GetWayDevTypeValueByName(CString name);
string GetWayDevTypeNameByValue(int type);
//////////////////////////////////////
//�ɼ�������
#define COLTOR_TYPE_NUM 2
extern const int gColtorTypeNum;
extern const char gColtorTypeName[COLTOR_TYPE_NUM][64];
extern const int gColtorTypeValue[COLTOR_TYPE_NUM];
int GetColtorTypeValueByName(string name);
string GetColtorTypeNameByValue(int value);
int GetColtorTypeIndByValue(int value);
//////////////////////////////////////
//������Чĳ��
#define EFFECT_NOW_NUM 2
extern const int gEffectNowNum;
extern const char gEffectNowName[EFFECT_NOW_NUM][64];
///////////////////////////////////////////////////////
//�ϱ�����
//�����豸����
extern const int gAllDevTypeReportParamNum;
extern char gAllDevTypeReportParamName[ALL_DEVTYPE_REPORT_PARAM_NUM][64];
extern const int gAllDevTypeReportParamValue[ALL_DEVTYPE_REPORT_PARAM_NUM];
int GetAllDevTypeReportParamValueByName(CString name);
CString GetAllDevTypeReportParamNameByValue(int type);
int GetAllDevTypeReportParamIndByValue(int val);
int ModifyReportParamUnitName(CString unit_name, int val);//�޸Ĳ�����λ����
CString ParseUnitByValue(MU_INT8 val);
//////////////////////////////////////
//���в�����Ӧ��λ,��ȷ��*1000����0.01��10��ʾ
extern const double gAllDevTypeReportParamUnit[ALL_DEVTYPE_REPORT_PARAM_NUM];
//////////////////////////////////////
//������ͨ���
#define AM_THREE_PHASE_REPORT_PARAM_NUM 56
extern const int gAmThreePhaseReportParamNum;
extern const int gAmThreePhaseReportParamValue[AM_THREE_PHASE_REPORT_PARAM_NUM];
//////////////////////////////////////
//������ͨ���
#define  AM_ONE_PHASE_REPORT_PARAM_NUM 36
extern const int gAmOnePhaseReportParamNum;
extern const int gAmOnePhaseReportParamValue[AM_ONE_PHASE_REPORT_PARAM_NUM];
//////////////////////////////////////
//ֱ����ͨ���
#define  AM_DC_REPORT_PARAM_NUM 7
extern const int gAmDcReportParamNum;
extern const int gAmDcReportParamValue[AM_DC_REPORT_PARAM_NUM];
//////////////////////////////////////
//����г�����
#define  AM_HARM_REPORT_PARAM_NUM 225
extern const int gAmHaramReportParamNum;
extern const int gAmHaramReportParamValue[AM_HARM_REPORT_PARAM_NUM];
//////////////////////////////////////
//ˮ����
#define  WATER_GASS_METER_REPORT_PARAM_NUM 3
extern const int gWaterGasMeterReportParamNum;
extern const int gWaterGasMeterReportParamValue[WATER_GASS_METER_REPORT_PARAM_NUM];
//////////////////////////////////////
//���ܵ��
#define  EOP_METER_REPORT_PARAM_NUM 5
extern const int gEopReportParamNum;
extern const int gEopReportParamValue[EOP_METER_REPORT_PARAM_NUM];
//////////////////////////////////////
//������
#define  HOT_METER_REPORT_PARAM_NUM 9
extern const int gHotMeterReportParamNum;
extern const int gHotMeterReportParamValue[HOT_METER_REPORT_PARAM_NUM];
///////////////////////////////////////////////////////
//ȫ�ֳ�����ֵ
//״̬:������Ϣ
#define ERR_NAME_NUM 7
extern const char gErrName[ERR_NAME_NUM][64];
//////////////////////////////////////
//����г��
#define  AM_HARM_NUM 226
extern const char gAmHarmName[AM_HARM_NUM][64];
//////////////////////////////////////
//������ͨ
#define  AM_THREE_PHASE_NUM 57
extern const char gAm3phaseName[AM_THREE_PHASE_NUM][64];
//////////////////////////////////////
//������ͨ
#define  AM_ONE_PHASE_NUM 37
extern const char gAm1phaseName[AM_ONE_PHASE_NUM][64];
//////////////////////////////////////
//ֱ����ͨ
#define  AM_DC_NUM 8
extern const char gAmdcName[AM_DC_NUM][64];
//////////////////////////////////////
//ˮ��
#define  WATER_METER_NAME_NUM 3
extern const char gWaterMeterName[WATER_METER_NAME_NUM][64];
//////////////////////////////////////
//����
#define  GASS_METER_NAME_NUM 16
extern const char gGassMeterName[GASS_METER_NAME_NUM][64];
////////////////////////////////////////////////////////////
//������λ
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
//�Զ��������ͻ�·ѡ��
extern int  gDefProtWayChooseNum;
extern const char gDefProtWayChooseName[MAX_WAY_NUM][64]; 
extern const int gDefProtWayChooseValue[MAX_WAY_NUM];
int GetDefProtWayChooseValueByName(CString name);
CString GetDefProtWayChooseNameByValue(int type);
int GetDefProtWayChooseIndByValue(int val);
void ModifyDefProtWayChooseNum(int val);//�޸Ĳ�����λ����
////////////////////////////////////////////////////////////
//ȫ�ֱ���
extern int gComNum;
extern e_company_type gCompanyType;
extern CString gCurrentDir;
//////////////////////////////////////
//ȫ�ֺ���
//ͨ��way_mask�жϵ�ǰway_ind�Ƿ�ѡ��
bool IsWayUsedByMask(int way_ind, int mask);
//�����豸id���ͣ��ҵ�����1�е��豸�±�
int GetDevIndByIdSubType(str_outer_dev* outer_dev, MU_INT64 id, e_dev_sub_type sub_type, int port_ind);
//���������ȡʹ�õĻ�·�±�����
int GetWayListByMask(int way_mask, VEC_INT& vec_data, int max_way_count);
//�����豸���ͻ�ȡ����1�е��ϱ������±�
int GetReportIndByDevType(e_dev_type dev_type, st_report_format* report_format);
//���Ҳ���2�в���1��Ӧ��ֵ
int GetMapSecondByFirst(int first, MAP_INT_INT map_val);
//���ļ���·����ѡ
CString BootOpenFile(bool open = TRUE);//TRUE��ʾ��,FALSE��ʾ����
//���ļ��У�·����ѡ
CString BootOpenFolder();//���ļ���



