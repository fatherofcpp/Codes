#include "StdAfx.h"
#include "SortDecorator.h"
#include <iostream>
using std::cout;
using std::endl;
CSortDecorator::CSortDecorator( ISchoolReport *psr ) : CReportDecorator(psr)
{
}
CSortDecorator::~CSortDecorator(void)
{
}
void CSortDecorator::ReportSort()
{
	cout << "����������38��..." << endl;
}
void CSortDecorator::Report()
{
	this->CReportDecorator::Report();
	this->ReportSort();
}