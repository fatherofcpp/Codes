#pragma once
#include "DataTypeDef.h"

class CParamIdMap
{
public:
	CParamIdMap(void);
	~CParamIdMap(void);

private:
	void SetOffSet();//���� Ԥ��
	int GetOffSet(int dev_type, int param_id);//��ȡƫ��ֵ Ԥ��
public:
	//ͨ������id������·����
	void SetValueByParamId(u_w_measure* measure, e_dev_type dev_type, char* data, int param_id);

private:
	int devTypeNum;//��ʵ���豸����������1������ֱ��ͨ���豸���ͱ���
	int paramNum;//��ʵ�ʲ���������1������ֱ��ͨ������id����
	int** offSet;//�ṹ���Ա��Խṹ���׵�ַ��ƫ��
};

