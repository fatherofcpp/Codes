/*
装饰模式，动态地给一个对象添加一些额外的职责。就增加功能来说，Decorator模式相比生成子类更为灵活。
main()，老爸
ISchoolReport，成绩单接口
CFourthGradeSchoolReport，四年级成绩单
ReportDecorator，成绩单装饰器基类
HighScoreDecorator，最高分装饰器
SortDecorator，班级排名装饰器
说明：对“四年级成绩单”进行装饰，ReportDecorator必然有一个private变量指向ISchoolReport。
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
	psr->Report();//看成绩单
	psr->Sign("老三");//很开心，就签字了
	delete psr;
}
void DoNew()
{
	cout << "----------分部分进行装饰----------" << endl;
	ISchoolReport *psr = new CFouthGradeSchoolReport();//原装成绩单
	//
	ISchoolReport *pssr = new CSortDecorator(psr);//又加了成绩排名的说明
	ISchoolReport *phsr = new CHighScoreDecorator(pssr);//加了最高分说明的成绩单
	phsr->Report();//看成绩单
	phsr->Sign("老三");//很开心，就签字了

	//先装饰哪个不重要，顺序已经在装饰内部确定好，但一定要调用最后一个装饰器的接口。
	//ISchoolReport *phsr = new CHighScoreDecorator(psr);//加了最高分说明的成绩单
	//ISchoolReport *pssr = new CSortDecorator(phsr);//又加了成绩排名的说明
	//pssr->Report();//看成绩单
	//pssr->Sign("老三");//很开心，就签字了

	delete pssr;
	delete phsr;
	delete psr;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//在装饰之前，可以用继承的办法，来进行简单的修饰
	DoIt();

	//但如果需要修饰的项目太多呢？或者装饰的项目不是固定的，继承显然会变得更复杂
	DoNew();

	return 0;
}