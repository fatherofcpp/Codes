/*
װ��ģʽ����̬�ظ�һ���������һЩ�����ְ�𡣾����ӹ�����˵��Decoratorģʽ������������Ϊ��
main()���ϰ�
ISchoolReport���ɼ����ӿ�
CFourthGradeSchoolReport�����꼶�ɼ���
ReportDecorator���ɼ���װ��������
HighScoreDecorator����߷�װ����
SortDecorator���༶����װ����
˵�����ԡ����꼶�ɼ���������װ�Σ�ReportDecorator��Ȼ��һ��private����ָ��ISchoolReport��
*/

#include "stdafx.h"
#include "ISchoolReport.h"
#include "FouthGradeSchoolReport.h"
#include "HighScoreDecorator.h"
#include "SortDecorator.h"
#include <iostream>
using std::cout;
using std::endl;
void DoIt()
{
	ISchoolReport *psr = new CFouthGradeSchoolReport();
	psr->Report();//���ɼ���
	psr->Sign("����");//�ܿ��ģ���ǩ����
	delete psr;
}
void DoNew()
{
	cout << "----------�ֲ��ֽ���װ��----------" << endl;
	ISchoolReport *psr = new CFouthGradeSchoolReport();//ԭװ�ɼ���
	//
	ISchoolReport *pssr = new CSortDecorator(psr);//�ּ��˳ɼ�������˵��
	ISchoolReport *phsr = new CHighScoreDecorator(pssr);//������߷�˵���ĳɼ���
	phsr->Report();//���ɼ���
	phsr->Sign("����");//�ܿ��ģ���ǩ����

	//��װ���ĸ�����Ҫ��˳���Ѿ���װ���ڲ�ȷ���ã���һ��Ҫ�������һ��װ�����Ľӿڡ�
	//ISchoolReport *phsr = new CHighScoreDecorator(psr);//������߷�˵���ĳɼ���
	//ISchoolReport *pssr = new CSortDecorator(phsr);//�ּ��˳ɼ�������˵��
	//pssr->Report();//���ɼ���
	//pssr->Sign("����");//�ܿ��ģ���ǩ����

	delete pssr;
	delete phsr;
	delete psr;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//��װ��֮ǰ�������ü̳еİ취�������м򵥵�����
	DoIt();

	//�������Ҫ���ε���Ŀ̫���أ�����װ�ε���Ŀ���ǹ̶��ģ��̳���Ȼ���ø�����
	DoNew();

	return 0;
}