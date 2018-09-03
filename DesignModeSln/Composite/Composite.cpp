#include "stdafx.h"
#include "Menuu.h"
#include "SubMenu.h"
#include "CompositMenu.h"

int main(int argc, char* argv[])
{
	Menuu* pMenu = new CompositMenu("国内新闻");
	pMenu->Add(new SubMenu("时事新闻"));
	pMenu->Add(new SubMenu("社会新闻"));
	pMenu->Display();
	pMenu = new CompositMenu("国际新闻");
	pMenu->Add(new SubMenu("国际要闻"));
	pMenu->Add(new SubMenu("环球视野"));
	pMenu->Display();

	return 0;
}