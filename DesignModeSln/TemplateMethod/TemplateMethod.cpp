/*
ģ��ģʽҲ���൱�򵥵�һ��ģʽ�������ǱȽϳ��õġ�ģ��ģʽ�Ƕ���һ�������е��㷨�ĹǼܣ�
����һЩ�����ӳٵ������С�TemplateMethodʹ��������Բ��ı�һ���㷨�Ľṹ�����ض�����㷨��ĳЩ�ض����衣
cbf4life�ٵ������Ǻ���ģ�ͣ�����ϸ������˵�����Բο�ԭ���߲��ͣ�cbf4life.cnblogs.com��
����ֻ���C++����ƺ�ʵ�ַ������Է���������ȡ��

main()���ͻ�
CHummerModel������ģ��
CHummerH1Model������ģ���ͺ�1
CHummerH2Model������ģ���ͺ�2
˵������CHummerModel����Start��Engineboom��Alarm��Stop�麯������������ʵ�֡�
�����Run������֯�߼����ֱ�����⼸��������ʵ�ֵĺ�����
ע�⣺�����е�RunӦ�ý�ֹ�����า�ǡ�
*/

#include "stdafx.h"
#include "HummerModel.h"
#include "HummerH1Model.h"
#include "HummerH2Model.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//�ͻ�����H1�ͺţ���ȥ������
	CHummerModel *ph1 = new CHummerH1Model();
	ph1->Run();
	delete ph1;

	//�ͻ���H2�ͺţ���ȥ��ˣ��
	CHummerModel *ph2 = new CHummerH2Model();
	ph2->Run();
	delete ph2;

	//�ͻ�����H1�ͺţ���ȥ�����ˣ����Ҳ���������
	CHummerH1Model *ph11 = new CHummerH1Model();
	ph11->SetAlarm(false);
	ph11->Run();
	delete ph11;

	return 0;
}

