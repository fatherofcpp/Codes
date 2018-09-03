#include "StdAfx.h"
#include "ReportDecorator.h"
#include <iostream>
using std::string;
CReportDecorator::CReportDecorator(ISchoolReport *psr)
{
	this->m_pSchoolReport = psr;
}
CReportDecorator::~CReportDecorator(void)
{
}
void CReportDecorator::Report()
{
	this->m_pSchoolReport->Report();
}
void CReportDecorator::Sign( string name )
{
	this->m_pSchoolReport->Sign(name);
}