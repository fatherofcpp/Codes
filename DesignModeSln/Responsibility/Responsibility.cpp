/*
���ʹ��������л��ᴦ�����󣬴Ӷ���������ķ����ߺͽ�����֮�����Ϲ�ϵ��
����Щ��������һ���������������������ݸ�����ֱ����һ����������Ϊֹ��
main()���ͻ�
IWomen�����������߽ӿ�
CWomen��������ʵ����
CHandler���������������
CFather����������ʵ���࣬������֮һ
CHusband����������ʵ���࣬������֮��
CSon����������ʵ���࣬������֮��
˵����CHandler�����ฺ��ۺ�������֮�е��������������SetNext�����������������
HandleMessage�ڴ�������ʱ�����ж��Ƿ����Լ�Ҫ����������������ֱ�Ӵ���
������ǣ��������һ���������ϵĴ�������ҵ���������һ������
*/

#include "stdafx.h"

#include "IWomen.h"
#include "Women.h"
#include "Handler.h"
#include "Father.h"
#include "Husband.h"
#include "Son.h"
#include <iostream>
using std::cout;
using std::endl;
void DoIt()
{
	cout << "----------ԭ���Ĵ���ʽ----------" << endl;
	//IWomen.h, CWomen.h, CHandler.h, CFather.h, CHusband.h, CSon.h
	IWomen *pwomen = new CWomen(1, "��Ҫ��ȥ���");
	CHandler *pfather = new CFather();
	CHandler *phusband = new CHusband();
	CHandler *pson = new CSon();
	if (pwomen->GetType() == 1)
	{
		cout << "Ů��������ʾ��" << endl;
		pfather->HandleMessage(pwomen);
	}
	else if (pwomen->GetType() == 2)
	{
		cout << "�������ɷ���ʾ��" << endl;
		phusband->HandleMessage(pwomen);
	}
	else if (pwomen->GetType() == 3)
	{
		cout << "ĸ���������ʾ��" << endl;
		pson->HandleMessage(pwomen);
	}
	else
	{
		//ʲôҲ����
	}
	delete pwomen;
	delete pfather;
	delete phusband;
	delete pson;
}
void DoNew()
{
	cout << "----------ʹ��ģʽ��Ĵ���ʽ----------" << endl;
	//Handler.h, Handler.cpp, IWomen.h, Women.h, Women.cpp, Father.h, Father,cpp, Husband.h, Husband.cpp, Son.h, Son.cpp
	IWomen *pwomen1 = new CWomen(1, "��Ҫ��ȥ���");
	IWomen *pwomen2 = new CWomen(2, "��Ҫ��ȥ�Է�");
	IWomen *pwomen3 = new CWomen(3, "��ҲҪ��ȥ�Է�");
	IWomen *pwomen4 = new CWomen(4, "��ҲҪ��ȥ���");

	CHandler *pfather = new CFather();
	CHandler *phusband = new CHusband();
	CHandler *pson = new CSon();

	pfather->SetNext(phusband);
	phusband->SetNext(pson);

	pfather->HandleMessage(pwomen1);
	pfather->HandleMessage(pwomen2);
	pfather->HandleMessage(pwomen3);
	pfather->HandleMessage(pwomen4);

	delete pfather;
	delete phusband;
	delete pson;
	delete pwomen1;
	delete pwomen2;
	delete pwomen3;
	delete pwomen4;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//����
	DoIt();
	//Ҫʵ���߼��жϣ���Ů�Ե������ȷ��͵������࣬������һ�����Լ�Ҫ����ģ��ͻ�Ӧ���д������Ů���Ѿ������ˣ��ǾͰ��������ת����Ů�����������������ƣ��γ���һ����������
	DoNew();
	return 0;
}