#include "StdAfx.h"
#include "YellowHuman.h"
#include <iostream>
using std::cout;
using std::endl;
CYellowHuman::CYellowHuman(void)
{
}
CYellowHuman::~CYellowHuman(void)
{
}
void CYellowHuman::Cry()
{
	cout << "黄色人种会哭" << endl;
}
void CYellowHuman::Laugh()
{
	cout << "黄色人种会大笑，幸福呀！" << endl;
}
void CYellowHuman::Talk()
{
	cout << "黄色人种会说话，一般说的都是双字节" << endl;
}