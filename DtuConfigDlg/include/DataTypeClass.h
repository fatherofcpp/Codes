#pragma once
/*
全局类,依赖于全局结构体等
*/
#include "DataTypeDef.h"
#include "CommonFunction.h"
#include "dataConvert.h"

//类型定义--class

class CTaskParam
{
public:
	CTaskParam(st_thread_tsk_param& thread_param)
	{
		tskResult = 0;
		tskEnd = false;

		memcpy(&threadParam, &thread_param, sizeof(threadParam));
		threadParam.file_data = new char[threadParam.data_len + 1];
		memset(threadParam.file_data, 0, threadParam.data_len + 1);
	};
	~CTaskParam()
	{
		if (NULL != threadParam.file_data)
		{
			delete[] threadParam.file_data;
		}
	};

	st_thread_tsk_param threadParam;//线程参数

	int tskResult;//任务执行返回值
	bool tskEnd;//线程轮询的判断
};//任务线程

//设备主类型的参数信息
class CParamDevType
{
public:
	CParamDevType()
	{
		pStParamInfoArr = new st_param_info[gAllDevTypeReportParamNum];
		ClearStParamInfoArr();
		paramInfNum = gAllDevTypeReportParamNum;//上限
	};

	~CParamDevType()
	{
		delete[] pStParamInfoArr;
	};

	void operator=(CParamDevType* param_devtype)
	{
		paramInfNum = param_devtype->paramInfNum;
		memcpy(pStParamInfoArr, param_devtype->pStParamInfoArr, gAllDevTypeReportParamNum * sizeof(*pStParamInfoArr));
	};
	void operator=(CParamDevType& param_devtype)
	{
		paramInfNum = param_devtype.paramInfNum;
		memcpy(pStParamInfoArr, param_devtype.pStParamInfoArr, gAllDevTypeReportParamNum * sizeof(*pStParamInfoArr));
	};

	void ClearStParamInfoArr()
	{
		memset(pStParamInfoArr, 0, gAllDevTypeReportParamNum * sizeof(*pStParamInfoArr));
	}

	int paramInfNum;//参数个数
	st_param_info* pStParamInfoArr;//参数信息
};

//gDevTypeNum个设备主类型的参数信息集合
class CParamDevTypeSet
{
public:
	CParamDevTypeSet()
	{
		paramDevTypeNum = gDevTypeNum;

		pCParamDevTypeArr = new CParamDevType[gDevTypeNum];
		for (int i = 0; i < gDevTypeNum; i++)
		{
			switch(gDevTypeValue[i])
			{
			case AM_3PHASE:
				pCParamDevTypeArr[i].paramInfNum = gAmThreePhaseReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gAmThreePhaseReportParamValue[j];
				}
				break;

			case AM_1PHASE:
				pCParamDevTypeArr[i].paramInfNum = gAmOnePhaseReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gAmOnePhaseReportParamValue[j];
				}
				break;

			case AM_DC:
				pCParamDevTypeArr[i].paramInfNum = gAmDcReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gAmDcReportParamValue[j];
				}
				break;

			case AM_HARM:
				pCParamDevTypeArr[i].paramInfNum = gAmHaramReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gAmHaramReportParamValue[j];
				}
				break;

			case WM:
				pCParamDevTypeArr[i].paramInfNum = gWaterGasMeterReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gWaterGasMeterReportParamValue[j];
				}
				break;
			case GM:
				pCParamDevTypeArr[i].paramInfNum = gWaterGasMeterReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gWaterGasMeterReportParamValue[j];
				}
				break;

			case HM:
				pCParamDevTypeArr[i].paramInfNum = gHotMeterReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gHotMeterReportParamValue[j];
				}
				break;

			case E_OP:
				pCParamDevTypeArr[i].paramInfNum = gEopReportParamNum;
				for (int j = 0; j < pCParamDevTypeArr[i].paramInfNum; j++)
				{
					pCParamDevTypeArr[i].pStParamInfoArr[j].param_id = gEopReportParamValue[j];
				}
				break;
			default:
				pCParamDevTypeArr[i].paramInfNum = 0;
				break;
			}
		}
	};
	~CParamDevTypeSet()
	{
		delete[] pCParamDevTypeArr;
	};

	void operator=(CParamDevTypeSet* param_devtype_set)
	{
		for (int i = 0; i < gDevTypeNum; i++)
		{
			pCParamDevTypeArr[i] = param_devtype_set->pCParamDevTypeArr[i];
		}
		paramDevTypeNum = param_devtype_set->paramDevTypeNum;
	}

	void operator=(CParamDevTypeSet& param_devtype_set)
	{
		for (int i = 0; i < gDevTypeNum; i++)
		{
			pCParamDevTypeArr[i] = param_devtype_set.pCParamDevTypeArr[i];
		}
		paramDevTypeNum = param_devtype_set.paramDevTypeNum;
	}

	int paramDevTypeNum;//设备数
	CParamDevType* pCParamDevTypeArr;//设备参量信息
};

//ModBus类型的参数信息
class CParamModbus
{
public:
	CParamModbus()
	{
		paramDevTypeNum = MAX_WAY_NUM;//上限
		pCParamDevTypeArr = new CParamDevType[MAX_WAY_NUM];
	};

	~CParamModbus()
	{
		delete[] pCParamDevTypeArr;
	};

	void operator=(CParamModbus* param_modbus)
	{
		paramDevTypeNum = param_modbus->paramDevTypeNum;
		for(int i = 0; i < MAX_WAY_NUM; i++)
		{
			pCParamDevTypeArr[i] = param_modbus->pCParamDevTypeArr[i];
		}
	};
	void operator=(CParamModbus& param_modbus)
	{
		paramDevTypeNum = param_modbus.paramDevTypeNum;
		for(int i = 0; i < MAX_WAY_NUM; i++)
		{
			pCParamDevTypeArr[i] = param_modbus.pCParamDevTypeArr[i];
		}
	};

	void ClearParamDevTypeArr()
	{
		for(int i = 0; i < MAX_WAY_NUM; i++)
		{
			pCParamDevTypeArr[i].ClearStParamInfoArr();
		}
	}

	int paramDevTypeNum;//回路个数
	CParamDevType* pCParamDevTypeArr;//参数信息
};

//gDevTypeNum个Modbus类型的参数信息集合
class CParamModbusSet
{
public:
	CParamModbusSet()
	{
		paramModbusNum = gDevTypeNum;
		pCParamModbusArr = new CParamModbus[gDevTypeNum];
		for (int i = 0; i < gDevTypeNum; i++)
		{
			CParamModbus* p_CParamModbus = &pCParamModbusArr[i];
			CParamDevType* p_CParamDevType = NULL;
			int way_num = p_CParamModbus->paramDevTypeNum;
			switch(gDevTypeValue[i])
			{
			case AM_3PHASE:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = gAmThreePhaseReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gAmThreePhaseReportParamValue[j];
					}
				}
				break;

			case AM_1PHASE:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];

					p_CParamDevType->paramInfNum = gAmOnePhaseReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gAmOnePhaseReportParamValue[j];
					}
				}
				break;

			case AM_DC:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = gAmDcReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gAmDcReportParamValue[j];
					}
				}
				break;

			case AM_HARM:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = gAmHaramReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gAmHaramReportParamValue[j];
					}
				}

				break;

			case WM:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = gWaterGasMeterReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gWaterGasMeterReportParamValue[j];
					}
				}
				break;
			case GM:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = gWaterGasMeterReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gWaterGasMeterReportParamValue[j];
					}
				}
				break;

			case HM:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = gHotMeterReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gHotMeterReportParamValue[j];
					}
				}
				break;

			case E_OP:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = gEopReportParamNum;
					for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
					{
						p_CParamDevType->pStParamInfoArr[j].param_id = gEopReportParamValue[j];
					}
				}
				break;
			default:
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					p_CParamDevType->paramInfNum = 0;
				}
				break;
			}
		}
	};
	~CParamModbusSet()
	{
		delete[] pCParamModbusArr;
	};

	void operator=(CParamModbusSet* param_modbus_set)
	{
		for (int i = 0; i < gDevTypeNum; i++)
		{
			pCParamModbusArr[i] = param_modbus_set->pCParamModbusArr[i];
		}
		paramModbusNum = param_modbus_set->paramModbusNum;
	}

	void operator=(CParamModbusSet& param_modbus_set)
	{
		for (int i = 0; i < gDevTypeNum; i++)
		{
			pCParamModbusArr[i] = param_modbus_set.pCParamModbusArr[i];
		}
		paramModbusNum = param_modbus_set.paramModbusNum;
	}

	int paramModbusNum;//设备数
	CParamModbus* pCParamModbusArr;//设备参量信息
};

//Modbus协议
class CProtModbus
{
public:
	CProtModbus()
	{
		pStDefSubTypeInfo = new st_def_sub_type_info;
		ClearStDefSubTypeInf();

		pCParamModbus = new CParamModbus;
	};
	~CProtModbus()
	{
		delete pStDefSubTypeInfo;
		delete pCParamModbus;
	};

	void ChangeDevType(int dev_type)
	{
		pCParamModbus->ClearParamDevTypeArr();
		SetDevType(dev_type);
	}

	void SetDevType(int dev_type)
	{
		pStDefSubTypeInfo->dev_type = dev_type;

		CParamDevType* p_CParamDevType = NULL;
		int way_num = pCParamModbus->paramDevTypeNum;
		switch(dev_type)
		{
		case AM_3PHASE:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = gAmThreePhaseReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gAmThreePhaseReportParamValue[j];
				}
			}
			break;

		case AM_1PHASE:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];

				p_CParamDevType->paramInfNum = gAmOnePhaseReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gAmOnePhaseReportParamValue[j];
				}
			}
			break;

		case AM_DC:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = gAmDcReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gAmDcReportParamValue[j];
				}
			}
			break;

		case AM_HARM:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = gAmHaramReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gAmHaramReportParamValue[j];
				}
			}

			break;

		case WM:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = gWaterGasMeterReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gWaterGasMeterReportParamValue[j];
				}
			}
			break;
		case GM:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = gWaterGasMeterReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gWaterGasMeterReportParamValue[j];
				}
			}
			break;

		case HM:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = gHotMeterReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gHotMeterReportParamValue[j];
				}
			}
			break;

		case E_OP:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = gEopReportParamNum;
				for (int j = 0; j < p_CParamDevType->paramInfNum; j++)
				{
					p_CParamDevType->pStParamInfoArr[j].param_id = gEopReportParamValue[j];
				}
			}
			break;
		default:
			for (int way_ind = 0; way_ind < way_num; way_ind++)
			{
				p_CParamDevType = &pCParamModbus->pCParamDevTypeArr[way_ind];
				p_CParamDevType->paramInfNum = 0;
			}
			break;
		}
	}

	void operator=(CProtModbus* prot_modbus)
	{
		memcpy(pStDefSubTypeInfo, prot_modbus->pStDefSubTypeInfo, sizeof(*pStDefSubTypeInfo));
		*pCParamModbus = prot_modbus->pCParamModbus;
	};

	void operator=(CProtModbus& prot_modbus)
	{
		memcpy(pStDefSubTypeInfo, prot_modbus.pStDefSubTypeInfo, sizeof(*pStDefSubTypeInfo));
		*pCParamModbus = prot_modbus.pCParamModbus;
	};

	void ClearStDefSubTypeInf()
	{
		memset(pStDefSubTypeInfo, 0, sizeof(*pStDefSubTypeInfo));
	};

	st_def_sub_type_info* pStDefSubTypeInfo;//自定义modbus协议信息
	CParamModbus* pCParamModbus;
};

//Modbus协议集合
class CProtModbusSet
{
public:
	CProtModbusSet()
	{
		protModbusNum = MAX_DEFINE_MODBUS_PROTOCOL;
		pCProtModbusArr = new CProtModbus[protModbusNum];

	};
	~CProtModbusSet()
	{
		delete[] pCProtModbusArr;
	};

	void operator=(CProtModbusSet& prot_modbus_set)
	{
		for(int i = 0; i < protModbusNum; i++)
		{
			pCProtModbusArr[i] = &prot_modbus_set.pCProtModbusArr[i];
		}
		protModbusNum = prot_modbus_set.protModbusNum;
	};

	void operator=(CProtModbusSet* prot_modbus_set)
	{
		for(int i = 0; i < protModbusNum; i++)
		{
			pCProtModbusArr[i] = &prot_modbus_set->pCProtModbusArr[i];
		}
		protModbusNum = prot_modbus_set->protModbusNum;
	};

	void ClearDefModbusArr()
	{
		for(int i = 0; i < protModbusNum; i++)
		{
			pCProtModbusArr[i].ClearStDefSubTypeInf();
		}
	};

	int protModbusNum;//自定义子类型个数
	CProtModbus* pCProtModbusArr;
};

class CDevAdaptRs485Cont
{
public:
	CDevAdaptRs485Cont()
	{
		pStRs485Arr = new st_rs485[gSubTypeNum];
		ClearRs485();
	};

	~CDevAdaptRs485Cont()
	{
		delete[] pStRs485Arr;
	};

	void ClearRs485()
	{
		memset(pStRs485Arr, 0, gSubTypeNum * sizeof(*pStRs485Arr));
	};

	void SetSubType(int sub_type)
	{
		deqRs485.clear();

		subType = sub_type;

		st_rs485 rs485;
		memset(&rs485, 0, sizeof(rs485));
		rs485.databits = 8;
		rs485.stop = 1;

		switch(subType)
		{
		case AM_3PHASE_01:
		case AM_1PHASE_01:
			rs485.pari = EVEN;

			rs485.baud = B2400;
			deqRs485.push_back(rs485);
			rs485.baud = B4800;
			deqRs485.push_back(rs485);
			rs485.baud = B9600;
			deqRs485.push_back(rs485);
			rs485.baud = B19200;
			deqRs485.push_back(rs485);
			rs485.baud = B1200;
			deqRs485.push_back(rs485);
			break;

		case AM_3PHASE_02:
		case AM_3PHASE_03:
		case AM_1PHASE_02:
		case AM_1PHASE_03:
			for (int i = 0; i < gParityNum; i++)
			{
				rs485.pari = (e_parity)gParityValue[gParityNum - 1 - i];

				rs485.baud = B9600;
				deqRs485.push_back(rs485);
				rs485.baud = B19200;
				deqRs485.push_back(rs485);
				rs485.baud = B4800;
				deqRs485.push_back(rs485);
				rs485.baud = B2400;
				deqRs485.push_back(rs485);
				rs485.baud = B1200;
				deqRs485.push_back(rs485);
			}
			break;

		case AM_3PHASE_04:
		case AM_1PHASE_04:
		case AM_HARM_01:
			for (int i = 0; i < gParityNum; i++)
			{
				rs485.pari = (e_parity)gParityValue[gParityNum - 1 - i];

				rs485.baud = B9600;
				deqRs485.push_back(rs485);
				rs485.baud = B19200;
				deqRs485.push_back(rs485);
				rs485.baud = B4800;
				deqRs485.push_back(rs485);
				rs485.baud = B2400;
				deqRs485.push_back(rs485);
				rs485.baud = B1200;
				deqRs485.push_back(rs485);
			}
			break;

		case AM_3PHASE_05:
			rs485.pari = EVEN;

			rs485.baud = B1200;
			deqRs485.push_back(rs485);
			rs485.baud = B2400;
			deqRs485.push_back(rs485);
			rs485.baud = B4800;
			deqRs485.push_back(rs485);
			rs485.baud = B9600;
			deqRs485.push_back(rs485);
			break;

		case WM_01:
		case WM_02:
		case WM_03:
		case WM_04:
		case HM_01:
		case HM_02:
		case GM_01:
			rs485.pari = EVEN;

			rs485.baud = B9600;
			deqRs485.push_back(rs485);
			rs485.baud = B4800;
			deqRs485.push_back(rs485);
			rs485.baud = B2400;
			deqRs485.push_back(rs485);
			rs485.baud = B1200;
			deqRs485.push_back(rs485);
			break;

		case AM_3PHASE_DEF_01:
		case AM_3PHASE_DEF_02:
		case AM_3PHASE_DEF_03:
		case AM_3PHASE_DEF_04:
		case AM_1PHASE_DEF_01:
		case AM_1PHASE_DEF_02:
		case AM_1PHASE_DEF_03:
		case AM_1PHASE_DEF_04:
		case AM_DC_DEF_01:
		case AM_DC_DEF_02:
		case AM_DC_DEF_03:
		case AM_DC_DEF_04:
		case AM_HARM_DEF_01:
		case AM_HARM_DEF_02:
		case AM_HARM_DEF_03:
		case AM_HARM_DEF_04:
			for (int pari_ind = 0; pari_ind < gParityNum; pari_ind++)
			{
				for (int baud_ind = 0; baud_ind < gBaudNum; baud_ind ++)
				{
					rs485.pari = (e_parity)gParityValue[pari_ind];
					rs485.baud = gBaudValue[baud_ind];
					deqRs485.push_back(rs485);
				}
			}
			break;
		default:
			break;
		}
	};

	void SetRs485(int sub_type, st_rs485& rs485)
	{
		int sub_type_ind = GetSubTypeIndByValue(sub_type);
		assert(sub_type_ind >= 0);
		memcpy(&pStRs485Arr[sub_type_ind], &rs485, sizeof(rs485));
	};

	deque<st_rs485> GetRs485Cont()
	{
		int sub_type_ind = GetSubTypeIndByValue(subType);
		assert(sub_type_ind >= 0);

		st_rs485 rs485;
		memset(&rs485, 0, sizeof(rs485));
		if (0 != memcmp(&pStRs485Arr[sub_type_ind], &rs485, sizeof(rs485)))
		{
			//优先级集合中有值
			for(deque<st_rs485>::iterator iter = deqRs485.begin(); iter != deqRs485.end(); )
			{
				rs485 = *iter;
				if (0 == memcmp(&rs485, &pStRs485Arr[sub_type_ind], sizeof(rs485)))
				{
					//删除优先值
					iter = deqRs485.erase(iter);
				}
				else
				{
					iter ++;
				}
			}
			//优先值置首位
			deqRs485.push_front(pStRs485Arr[sub_type_ind]);
		}
		return deqRs485;
	};

	int subType;//设备子类型
	deque<st_rs485> deqRs485;//通讯参数集合
	st_rs485* pStRs485Arr;//成功的参数集合，对应subtype

};//下行设备通讯参数集合,波特率，数据位，校验位，停止位


class CDevAdaptSendFrame
{
public:
	CDevAdaptSendFrame(int sub_type, MU_INT64 dev_addr, CProtModbusSet* prot_modbus_set)
	{
		subType = sub_type;
		devAddr = dev_addr;

		sendFrameLen = 0;
		sendFrameLenMax = 1024;
		sendFrame = new MU_INT8[sendFrameLenMax];
		ClearSendFrame();

		pCProtModbusSet = new CProtModbusSet;
		*pCProtModbusSet = prot_modbus_set;
		
		int frame_ind = 0;//帧下标
		MU_INT8 cs_pari = 0;//和校验
		MU_INT16 crc_pari = 0;//crc校验
		int pari_start = 0;//校验起始偏移
		int pari_end = 0;//校验结束偏移
		MU_INT8 meter_type = 0;//仪表类型，用于水汽热量
		int reg_addr = 0;//寄存器起始地址
		int reg_len = 0;//寄存器长度
		for (int i = 0; i <pCProtModbusSet->protModbusNum; i++)
		{
			CProtModbus* p_CProtModbus = &pCProtModbusSet->pCProtModbusArr[i];
			int sub_type = p_CProtModbus->pStDefSubTypeInfo->sub_type;
			if (sub_type == subType)//过滤非自定义
			{
				CParamModbus* p_CParamModbus = p_CProtModbus->pCParamModbus;

				int way_num = p_CParamModbus->paramDevTypeNum;
				bool is_reg_same = p_CProtModbus->pStDefSubTypeInfo->is_reg_same;
				for (int way_ind = 0; way_ind < way_num; way_ind++)
				{
					if (is_reg_same && way_ind > 0)
					{
						break;
					}

					CParamDevType* p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
					int param_num = p_CParamDevType->paramInfNum;
					for (int k = 0; k < param_num; k++)
					{
						reg_len = p_CParamDevType->pStParamInfoArr[k].param_register_len;
						reg_addr = p_CParamDevType->pStParamInfoArr[k].param_register_addr;
						if (reg_len != 0)
						{
							goto FIND_REG;
						}
					}
				}
			}
		}

FIND_REG:

		switch(subType)
		{
		case AM_3PHASE_01:
		case AM_1PHASE_01:
			//FEH FEH FEH FEH 68H A0 A1 A2 A3 A4 A5 68H 11H 04H 01H 04H 00H 04H CS 16H
			//A0~A5为设备地址，低字节在前，BCD码
			//4660 0x60 0x46
			//0x1234 0x34 0x12
			//0x1234 4660 0x60 0x46
			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			pari_start = frame_ind;
			sendFrame[frame_ind ++] = 0x68;
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 1)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 )% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100 / 100 / 100 )% 100);
			sendFrame[frame_ind ++] = 0x68;
			sendFrame[frame_ind ++] = 0x11;
			sendFrame[frame_ind ++] = 0x04;
			sendFrame[frame_ind ++] = 0x01;
			sendFrame[frame_ind ++] = 0x04;
			sendFrame[frame_ind ++] = 0x00;
			pari_end = frame_ind;
			cs_pari = parity_cs(&sendFrame[pari_start], (pari_end - pari_start));
			sendFrame[frame_ind ++] = cs_pari;//和校验
			sendFrame[frame_ind ++] = 0x16;
			sendFrameLen = frame_ind;
			break;

		case AM_3PHASE_02:
		case AM_3PHASE_03:
		case AM_1PHASE_02:
		case AM_1PHASE_03:
			//modbus  除crc外高字节在前，crc低字节在前
			//A 03H 00H 00H 00H 01H CRC
			//A为设备地址，16进制；CRC为校验码，2个字节，16进制
			pari_start = frame_ind;
			sendFrame[frame_ind ++] = ((devAddr >> (8*0)) & 0xFF);
			sendFrame[frame_ind ++] = 0x03;
			sendFrame[frame_ind ++] = 0x00;
			sendFrame[frame_ind ++] = 0x00;
			sendFrame[frame_ind ++] = 0x00;
			sendFrame[frame_ind ++] = 0x01;
			pari_end = frame_ind;
			crc_pari = ModbusCrc(&sendFrame[pari_start], (pari_end - pari_start));

			sendFrame[frame_ind ++] = ((crc_pari >> (8*0)) & 0xFF);//crc校验
			sendFrame[frame_ind ++] = ((crc_pari >> (8*1)) & 0xFF);//crc校验
			sendFrameLen = frame_ind;

			break;

		case AM_3PHASE_04:
		case AM_1PHASE_04:
		case AM_HARM_01:
			//modbus  除crc外高字节在前，crc低字节在前
			//A 03H 00H 04H 00H 01H CRC
			//A为设备地址，16进制；CRC为校验码，2个字节，16进制
			pari_start = frame_ind;
			sendFrame[frame_ind ++] = ((devAddr >> (8*0)) & 0xFF);
			sendFrame[frame_ind ++] = 0x03;
			sendFrame[frame_ind ++] = 0x00;
			sendFrame[frame_ind ++] = 0x04;
			sendFrame[frame_ind ++] = 0x00;
			sendFrame[frame_ind ++] = 0x01;
			pari_end = frame_ind;
			crc_pari = ModbusCrc(&sendFrame[pari_start], (pari_end - pari_start));

			sendFrame[frame_ind ++] = ((crc_pari >> (8*0)) & 0xFF);//crc校验
			sendFrame[frame_ind ++] = ((crc_pari >> (8*1)) & 0xFF);//crc校验
			sendFrameLen = frame_ind;
			break;

		case AM_3PHASE_05:
			//FEH FEH FEH FEH 68H A0 A1 A2 A3 A4 A5 68H 01H 02H 32H C0H CS 16H
			//A0~A5为设备地址，低字节在前，BCD码；CS为和校验码
			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			pari_start = frame_ind;
			sendFrame[frame_ind ++] = 0x68;
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 1)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 )% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100 / 100 / 100 )% 100);
			sendFrame[frame_ind ++] = 0x68;
			sendFrame[frame_ind ++] = 0x01;
			sendFrame[frame_ind ++] = 0x02;
			sendFrame[frame_ind ++] = 0x32;
			sendFrame[frame_ind ++] = 0xC0;
			pari_end = frame_ind;
			cs_pari = parity_cs(&sendFrame[pari_start], (pari_end - pari_start));
			sendFrame[frame_ind ++] = cs_pari;//和校验
			sendFrame[frame_ind ++] = 0x16;
			sendFrameLen = frame_ind;
			break;

		case WM_01:
		case WM_02:
		case WM_03:
		case WM_04:
		case HM_01:
		case HM_02:
		case GM_01:
			//FEH FEH FEH 68H T A0 A1 A2 A3 A4 A5 A6 01H 03H 1FH 90H 00H CS 16H
			
			//T为仪表类型，查看附件1；A0~A6为设备地址，每个字节为2位BCD码格式。低地址在前，高地址在后
			if (subType == WM_01)
			{
				meter_type = 0x10;
			}else if (subType == WM_02)
			{
				meter_type = 0x11;
			}else if (subType == WM_03)
			{
				meter_type = 0x12;
			}else if (subType == WM_04)
			{
				meter_type = 0x13;
			}else if (subType == HM_01)
			{
				meter_type = 0x20;
			}else if (subType == HM_02)
			{
				meter_type = 0x21;
			}else if (subType == GM_01)
			{
				meter_type = 0x30;
			}else
			{
				meter_type = 0x40;
			}

			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			sendFrame[frame_ind ++] = 0xFE;
			pari_start = frame_ind;
			sendFrame[frame_ind ++] = 0x68;

			sendFrame[frame_ind ++] = meter_type;

			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 1)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 )% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100 / 100)% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100 / 100 / 100 )% 100);
			sendFrame[frame_ind ++] = TenHexAdd0x((devAddr / 100 / 100 / 100 / 100 / 100 / 100)% 100);

			sendFrame[frame_ind ++] = 0x01;
			sendFrame[frame_ind ++] = 0x03;
			sendFrame[frame_ind ++] = 0x1F;
			sendFrame[frame_ind ++] = 0x90;
			sendFrame[frame_ind ++] = 0x00;
			pari_end = frame_ind;
			cs_pari = parity_cs(&sendFrame[pari_start], (pari_end - pari_start));
			sendFrame[frame_ind ++] = cs_pari;//和校验
			sendFrame[frame_ind ++] = 0x16;
			sendFrameLen = frame_ind;

			break;

		case AM_3PHASE_DEF_01:
		case AM_3PHASE_DEF_02:
		case AM_3PHASE_DEF_03:
		case AM_3PHASE_DEF_04:
		case AM_1PHASE_DEF_01:
		case AM_1PHASE_DEF_02:
		case AM_1PHASE_DEF_03:
		case AM_1PHASE_DEF_04:
		case AM_DC_DEF_01:
		case AM_DC_DEF_02:
		case AM_DC_DEF_03:
		case AM_DC_DEF_04:
		case AM_HARM_DEF_01:
		case AM_HARM_DEF_02:
		case AM_HARM_DEF_03:
		case AM_HARM_DEF_04:
			//modbus  除crc外高字节在前，crc低字节在前
			//A 03H 00H 04H 00H 01H CRC
			//A为设备地址，16进制；CRC为校验码，2个字节，16进制
			if (reg_addr == 0 && reg_len == 0)
			{
				sendFrameLen = frame_ind;
				break;
			}
			pari_start = frame_ind;
			sendFrame[frame_ind ++] = ((devAddr >> (8*0)) & 0xFF);
			sendFrame[frame_ind ++] = 0x03;
			sendFrame[frame_ind ++] = ((reg_addr >> (8*1)) & 0xFF);
			sendFrame[frame_ind ++] = ((reg_addr >> (8*0)) & 0xFF);
			sendFrame[frame_ind ++] = ((reg_len >> (8*1)) & 0xFF);
			sendFrame[frame_ind ++] = ((reg_len >> (8*0)) & 0xFF);
			pari_end = frame_ind;
			crc_pari = ModbusCrc(&sendFrame[pari_start], (pari_end - pari_start));

			sendFrame[frame_ind ++] = ((crc_pari >> (8*0)) & 0xFF);//crc校验
			sendFrame[frame_ind ++] = ((crc_pari >> (8*1)) & 0xFF);//crc校验
			sendFrameLen = frame_ind;
			break;
		default:
			sendFrameLen = frame_ind;
			break;
		}
	};

	~CDevAdaptSendFrame()
	{
		delete pCProtModbusSet;
		delete[] sendFrame;
	};

	void ClearSendFrame(){memset(sendFrame, 0, sendFrameLenMax);};

	void operator=(CDevAdaptSendFrame& adapt_send_frame)
	{
		this->devAddr = adapt_send_frame.devAddr;
		this->subType = adapt_send_frame.subType;
		this->sendFrameLen = adapt_send_frame.sendFrameLen;
		memcpy(this->sendFrame, adapt_send_frame.sendFrame, adapt_send_frame.sendFrameLenMax);
		*pCProtModbusSet = adapt_send_frame.pCProtModbusSet;
	};


	int sendFrameLen;//发送帧实际长度
	int sendFrameLenMax;//发送帧内存长度
	MU_INT8* sendFrame;//发送帧
	int subType;//设备子类型
	MU_INT64 devAddr;//设备地址
	CProtModbusSet* pCProtModbusSet;//自定义子类型数组
};//下行设备通讯发送帧
