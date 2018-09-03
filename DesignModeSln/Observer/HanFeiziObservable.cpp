#include "StdAfx.h"
#include "HanFeiziObservable.h"
#include <iostream>
using std::string;
using std::cout;
using std::endl;
CHanFeiziObservable::CHanFeiziObservable(void)
{
}
CHanFeiziObservable::~CHanFeiziObservable(void)
{
}
void CHanFeiziObservable::AddObserver( IObserver *pObserver )
{
	m_observerList.push_back(pObserver);
}
void CHanFeiziObservable::DeleteObserver( IObserver *pObserver )
{
	ObserverList_C_iterator it = m_observerList.begin();
	for (; it != m_observerList.end(); it++)
	{
		string name = (*it)->GetName();
		if (name.compare(pObserver->GetName()) == 0)
		{
			//�ҵ���ɾ����
		}
	}
}
void CHanFeiziObservable::NotifyObservers( string context )
{
	ObserverList_C_iterator it = m_observerList.begin();
	for (; it != m_observerList.end(); it ++)
	{
		(*it)->Update(context);
	}
}
void CHanFeiziObservable::HaveBreakfast()
{
	cout << "�����ӣ���ʼ�Է���..." << endl;

	this->NotifyObservers("�������ڳԷ�");
}
void CHanFeiziObservable::HaveFun()
{
	cout << "�����ӣ���ʼ������..." << endl;

	this->NotifyObservers("������������");
}