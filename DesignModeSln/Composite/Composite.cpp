#include "stdafx.h"
#include "Menuu.h"
#include "SubMenu.h"
#include "CompositMenu.h"

int main(int argc, char* argv[])
{
	Menuu* pMenu = new CompositMenu("��������");
	pMenu->Add(new SubMenu("ʱ������"));
	pMenu->Add(new SubMenu("�������"));
	pMenu->Display();
	pMenu = new CompositMenu("��������");
	pMenu->Add(new SubMenu("����Ҫ��"));
	pMenu->Add(new SubMenu("������Ұ"));
	pMenu->Display();

	return 0;
}