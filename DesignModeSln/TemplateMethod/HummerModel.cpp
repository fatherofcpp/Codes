#include "StdAfx.h"
#include "HummerModel.h"
#include <iostream>
using std::cout;
using std::endl;
CHummerModel::CHummerModel(void)
{
}
CHummerModel::~CHummerModel(void)
{
}
void CHummerModel::Run()
{
	//�ȷ�������
	Start();
	//���濪ʼ����
	EngineBoom();
	//Ȼ��Ϳ�ʼ���ˣ��ܵĹ���������һ������·���Ͱ�����
	if (IsAlarm())
		Alarm();

	//����Ŀ�ĵؾ�ͣ��
	Stop();
}
bool CHummerModel::IsAlarm()
{
	//���ӷ�����Ĭ�������ǻ����
	return true;
}