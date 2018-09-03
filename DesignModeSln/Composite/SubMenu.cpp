#include "stdafx.h"
#include "SubMenu.h"
#include <iostream>

using namespace std;

SubMenu::SubMenu()
{

}

SubMenu::SubMenu(string strName) : Menuu(strName)
{

}

SubMenu::~SubMenu()
{

}

void SubMenu::Display()
{
	cout << m_strName << endl;
}