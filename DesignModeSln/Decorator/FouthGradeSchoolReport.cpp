#include "StdAfx.h"
#include "FouthGradeSchoolReport.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;
CFouthGradeSchoolReport::CFouthGradeSchoolReport(void)
{
}
CFouthGradeSchoolReport::~CFouthGradeSchoolReport(void)
{
}
void CFouthGradeSchoolReport::Report()
{
	cout << "尊敬的XXX家长：" << endl;
	cout << "......" << endl;
	cout << "语文62  数学65  体育98  自然63" << endl;
	cout << "......" << endl;
	cout << "                家长签名：" << endl;
}
void CFouthGradeSchoolReport::Sign(string name)
{
	cout << "家长签名为：" << name.c_str() << endl;
}