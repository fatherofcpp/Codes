#include "StdAfx.h"
#include "BackDoor.h"

#include <iostream>
using std::cout;
using std::endl;
CBackDoor::CBackDoor(void)
{
}
CBackDoor::~CBackDoor(void)
{
}
void CBackDoor::Operate(void)
{
	cout << __FUNCTION__ << endl;
}
