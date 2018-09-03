/*
������ģʽ����һ�����Ӷ���Ĺ��������ı�ʾ���룬ʹ��ͬ���Ĺ������̿��Դ�����ͬ�ı�ʾ��
һ�λ�ɬ�Ѷ������֣�ʵ�ִ�����ͬ��ʾ�ķ������Ǹ������Ĺ��̴��봴���Ĳ�����

main()���ͻ�
CCarModel����Ʒģ��
CBenzModel������ģ��
CBMWModel������ģ��
ICarBuilder�������߽ӿ�
CBenzBuilder�����۽�����
CBMWBuilder����������
CDirector������
˵����CCarModelʵ��ģ�巽����Builder����ʼ�����Ʒ�������Ʒʱ��������˳����Director��main������
ע�⣺������ģʽ�ͳ��󹤳��ǳ����ơ������߸����Ӳ�Ʒ����ʱ���߼�˳��
�����󹤳���������������ͬ�ͺŵĲ�Ʒ�����󹤳�������˳��
*/

#include "stdafx.h"
#include "CarModel.h"
#include "BenzModel.h"
#include "BMWModel.h"
#include "BenzBuilder.h"
#include "BMWBuilder.h"
#include "Director.h"
#include <vector>
#include <iostream>
using std::vector;
using std::string;
using std::cout;
using std::endl;

void DoBenzRun()//û��ʹ��ģʽʱ����Ҫ�Ѳ���һ��һ���Ĵ���ģ�͡�
{
	cout << "----------���ɱ���ģ��----------" << endl;
	CBenzModel *pBenz = new CBenzModel();
	vector<string> seq;
	seq.push_back("engine boom");//�ͻ�Ҫ��run��ʱ���ȷ�������
	seq.push_back("start");//��������
	seq.push_back("stop");//����һ�ξ�ͣ����

	pBenz->SetSequence(&seq);
	pBenz->Run();
	delete pBenz;
}


void DoBuilder()//ʹ��ģʽ����benzBuilder��bmwBuilder�����ɣ�����ʹ��ͬ���Ĵ���˳��
{
	cout << "----------��ͬһ��˳������ģ��----------" << endl;
	vector<string> seq;
	seq.push_back("engine boom");
	seq.push_back("start");
	seq.push_back("stop");

	CBenzBuilder benzBuilder;
	benzBuilder.SetSequence(&seq);
	CBenzModel *pBenz = dynamic_cast<CBenzModel*>(benzBuilder.GetCarModel());
	pBenz->Run();

	CBMWBuilder bmwBuilder;
	bmwBuilder.SetSequence(&seq);
	CBMWModel *pBmw = dynamic_cast<CBMWModel*>(bmwBuilder.GetCarModel());
	pBmw->Run();
}
void DoDirector()//ʹ��ָ��������װ�������߼����Ѵ�����˳���ھ���ָ���������档
{
	cout << "----------��������ģ��----------" << endl;
	CDirector director;

	//1W��A���͵ı��۳�
	for(int i = 0; i < 2; i++)
		director.GetABenzModel()->Run();

	//100W��B���͵ı��۳�
	for(int i = 0; i < 2; i++)
		director.GetBBenzModel()->Run();

	//1000W��C���͵ı���
	for(int i = 0; i < 2; i++)
		director.GetCBMWModel()->Run();
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoBenzRun();

	DoBuilder();

	DoDirector();

	return 0;
}