
/*
����ģʽ�������󲿷�������ʵ�ֲ��ַ��룬ʹ���Ƕ����Զ����ر仯��ʵ�ַ���İ취��������һ���࣬
main()���ͻ�
IProduct����Ʒ�ӿ�
CHouse������
CIPod��ipod
CClothes����װ
CNewCorp�������࣬MakeMoney()����������
CNewHouseCorp��ֻ���������ӣ����Թ��캯����CHouse*
CShanZhaiCorp��ʲô׬Ǯ������ʲô�����Թ��캯����IProduct*
˵�����ͻ�ֱ��ʹ��CNewHouseCorp��CShanZhaiCorp�࣬��main()�����ﹹ���Ʒ��
Ȼ�󴫵�������������������MakeMoney()�������ȵ��û����MakeMoney()��Ȼ��ֱ�ִ�и��Ե��߼���
ע�⣺CNewCorp�������������á����Էֱ����Ӳ�Ʒ�͹�˾��
*/

#include "stdafx.h"
#include "NewHouseCorp.h"
#include "Clothes.h"
#include "IPod.h"
#include "ShanZhaiCorp.h"
#include <iostream>
using std::cout;
using std::endl;

void DoNewRun1()
{
	cout << "----------���ز���˾���������е�----------" << endl;
	CHouse house;
	CNewHouseCorp newHouseCorp(&house);
	newHouseCorp.MakeMoney();
	cout << endl;

	cout << "----------ɽկ��˾���������е�----------" << endl;
	CClothes clothes;
	CShanZhaiCorp shanZhaiCorp(&clothes);
	shanZhaiCorp.MakeMoney();
	cout << endl;
}

void DoNewRun2()
{
	cout << "----------���ز���˾���������е�----------" << endl;
	CHouse house;
	CNewHouseCorp newHouseCorp(&house);
	newHouseCorp.MakeMoney();
	cout << endl;

	cout << "----------ɽկ��˾���������е�----------" << endl;
	CIPod ipod;
	CShanZhaiCorp shanZhaiCorp(&ipod);
	shanZhaiCorp.MakeMoney();
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//ֻ�����ҹ�˾��һ���Ƿ��ز���˾����һ�ҹ�˾���·�׬Ǯ�������·�
	DoNewRun1();

	//ֻ�����ҹ�˾��һ���Ƿ��ز���˾����һ�ҹ�˾��ipod׬Ǯ������ipod
	DoNewRun2();

	return 0;
}
