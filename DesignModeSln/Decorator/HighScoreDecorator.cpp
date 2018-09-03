#include "StdAfx.h"
#include "HighScoreDecorator.h"
#include <iostream>
using std::cout;
using std::endl;
CHighScoreDecorator::CHighScoreDecorator( ISchoolReport *psr ) : CReportDecorator(psr)
{
}
CHighScoreDecorator::~CHighScoreDecorator(void)
{
}
void CHighScoreDecorator::Report()
{
	this->ReportHighScore();
	this->CReportDecorator::Report();
}
void CHighScoreDecorator::ReportHighScore()
{
	cout << "��ο������������75�� ��ѧ��78�� ��Ȼ��80" << endl;
}