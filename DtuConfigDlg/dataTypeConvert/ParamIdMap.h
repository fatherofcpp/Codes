#pragma once
#include "DataTypeDef.h"

class CParamIdMap
{
public:
	CParamIdMap(void);
	~CParamIdMap(void);

private:
	void SetOffSet();//设置 预留
	int GetOffSet(int dev_type, int param_id);//获取偏移值 预留
public:
	//通过参数id解析回路数据
	void SetValueByParamId(u_w_measure* measure, e_dev_type dev_type, char* data, int param_id);

private:
	int devTypeNum;//比实际设备类型总数大1，便于直接通过设备类型遍历
	int paramNum;//比实际参数总数大1，便于直接通过参数id遍历
	int** offSet;//结构体成员相对结构体首地址的偏移
};

