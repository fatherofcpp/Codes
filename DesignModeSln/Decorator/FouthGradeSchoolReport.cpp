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
	cout << "�𾴵�XXX�ҳ���" << endl;
	cout << "......" << endl;
	cout << "����62  ��ѧ65  ����98  ��Ȼ63" << endl;
	cout << "......" << endl;
	cout << "                �ҳ�ǩ����" << endl;
}
void CFouthGradeSchoolReport::Sign(string name)
{
	cout << "�ҳ�ǩ��Ϊ��" << name.c_str() << endl;
}