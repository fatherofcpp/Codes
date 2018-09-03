#include "stdafx.h"
#include "CompositMenu.h"
#include <iostream>

using namespace std;

CompositMenu::CompositMenu()
{

}

CompositMenu::CompositMenu(string strName) : Menuu(strName)
{

}

CompositMenu::~CompositMenu()
{

}

void CompositMenu::Add(Menuu* pMenu)
{
	m_vMenu.push_back(pMenu);
}

void CompositMenu::Remove(Menuu* pMenu)
{
	//m_vMenu.erase(&pMenu);
}

Menuu* CompositMenu::GetChild(int index)
{
	return m_vMenu[index];
}

void CompositMenu::Display()
{
	cout << "+" << m_strName << endl;
	vector<Menuu*>::iterator it = m_vMenu.begin();
	for (; it != m_vMenu.end(); ++it)
	{
		cout << "|-";
		(*it)->Display();
	}
}
