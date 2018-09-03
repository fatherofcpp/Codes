/*
模板模式也是相当简单的一种模式，而且是比较常用的。模板模式是定义一个操作中的算法的骨架，
而将一些步骤延迟到子类中。TemplateMethod使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。
cbf4life举的例子是悍马车模型，更详细的例子说明可以参考原作者博客：cbf4life.cnblogs.com。
这里只提出C++的设计和实现方法，以方便随用随取。

main()，客户
CHummerModel，悍马模型
CHummerH1Model，悍马模型型号1
CHummerH2Model，悍马模型型号2
说明：在CHummerModel声明Start、Engineboom、Alarm、Stop虚函数，由派生类实现。
基类的Run负责组织逻辑，分别调用这几个派生类实现的函数。
注意：基类中的Run应该禁止派生类覆盖。
*/

#include "stdafx.h"
#include "HummerModel.h"
#include "HummerH1Model.h"
#include "HummerH2Model.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//客户开着H1型号，出去遛弯了
	CHummerModel *ph1 = new CHummerH1Model();
	ph1->Run();
	delete ph1;

	//客户开H2型号，出去玩耍了
	CHummerModel *ph2 = new CHummerH2Model();
	ph2->Run();
	delete ph2;

	//客户开着H1型号，出去遛弯了，并且不让喇叭响
	CHummerH1Model *ph11 = new CHummerH1Model();
	ph11->SetAlarm(false);
	ph11->Run();
	delete ph11;

	return 0;
}

