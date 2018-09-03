/*
������������һ��һ�Զ��������ϵ����һ�������״̬�����ı�ʱ��
�������������Ķ��󶼵õ�֪ͨ�����Զ����¡�
main()��
IObservable�����۲��߽ӿ�
CHanFeiZiObservable�����۲��ߺ�����
IObserver���۲��߽ӿ�
CLiSiObserver���۲�����˹
CZhouSiObserver�۲�����˹
˵�������۲��߾ۼ������۲��ߺ�������ߣ������ӵ�ÿһ���ٶ�����֪ͨ���۲��ߣ�����˹����˹��
ע�⣺�������һ��������ǹ۲���Ҳ�Ǳ��۲��ߡ��������ݿ��еĴ�����һ����
��Ϊһ�����ӵ����ͺ���ά���ˡ��۲���������ί�еĴ���ʽ��
*/

#include "stdafx.h"
#include "HanFeiziObservable.h"
#include "LiSiObserver.h"
#include "ZhouSiObserver.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;

void DoNew()
{
	//IHanFeiZi.h, HanFeiZiNew.h, ILiSi.h, LiSi.h
	// cout << "----------���µķ�������----------" << endl;

	//CHanFeiZiNew hanfeizi;

	//hanfeizi.HaveBreakfast();

	//hanfeizi.HaveFun();
}


void DoNewNew()
{
	//IObservable.h, HanfeiziObservable.h, IObserver.h, LiSiObserver.h
	cout << "----------�ø��µķ���������----------" << endl;
	IObserver *pLiSi = new CLiSiObserver();
	IObserver *pZhouSi = new CZhouSiObserver();

	CHanFeiziObservable *pHanFeiZi = new CHanFeiziObservable();

	pHanFeiZi->AddObserver(pLiSi);
	pHanFeiZi->AddObserver(pZhouSi);
	pHanFeiZi->HaveBreakfast();

	delete pLiSi;
	pLiSi = NULL;
	delete pHanFeiZi;
	pHanFeiZi = NULL;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//�Ƚ�ԭʼ�ķ��������߳����۲졣
	//DoIt();

	//����˹�����ۼ���������������ϣ������Ļ���϶�̫���ˣ������ø�����ķ�ʽ��
	DoNew();

	//������ķ�ʽ����Ҫ�۲캫���ӵ��˶���ȥ�ˣ�������ֻ������˹�۲졣
	DoNewNew();

	return 0;
}