/*
命令模式，将一个请求封装为一个对象，从而使你可用不同的请求对客户进行参数化；
对请求排队或记录请求日志，以及支持可撤消的操作。应该是一个比较简单的模式了。

main()，客户 
CInvoker，命令接收者，如项目经理 
IGroup，执行者接口 
CRequirementGroup，实际执行者之一
CPageGroup，实际执行者之二
CCodePage，实际执行者之三
ICommand，命令接口
CAddRequirementCommand，Execute函数，将调用CRequirementGroup的多个命令。来组合执行用户发出的命令。
CDeletePageCommand，同上
... ... 其它命令。
说明：客户只需要知道向Invoker发出命令（多个命令），而不是将命令直接传达给具体的执行者。
当然，客户是需要知道都有什么命令的。 
注意：客户只发命令，不需要知道由谁来执行和怎么执行，体现出高内聚的特点。
用户在发出命令后，是允许撤回的，所以可以增加一个命令“Undo ”，Undo是状态的变更。
*/

#include "stdafx.h"
#include "IGroup.h"
#include "CodeGroup.h"
#include "PageGroup.h"
#include "RequirementGroup.h"
#include "Invoker.h"
#include "AddRequirementCommand.h"
#include "DeletePageCommand.h"
#include <iostream>
using std::cout;
using std::endl;

void DoIt()
{
	cout << "----------客户想增加一个需求----------" << endl;
	IGroup *rg = new CRequirementGroup();
	rg->Find();
	rg->Add();
	rg->Plan();
	delete rg;
	cout << endl;

	cout << "----------客户又想修改一个页面----------" << endl;
	IGroup *pg = new CPageGroup();
	pg->Find();
	pg->Add();
	pg->Plan();
	delete pg;
	cout << endl;

	cout << "----------客户又想删除一个功能----------" << endl;
	IGroup *cg = new CCodeGroup();
	cg->Find();
	cg->Add();
	cg->Plan();
	delete cg;
	cout << endl;
}

void DoNew()
{
	cout << "----------客户觉得烦了，希望只找一个人，并告诉他要做什么----------" << endl;
	cout << "----------客户要求增加一项需求----------" << endl;
	CInvoker gary;
	ICommand *pcommand = new CAddRequirementCommand();
	gary.SetCommand(pcommand);
	gary.Action();
	delete pcommand;
	cout << endl;

	//客户想要改动只需要找CInvoker就可以了。
	cout << "----------客户要求删除一个页面----------" << endl;
	CInvoker ricky;
	ICommand *pcommand2 = new CDeletePageCommand();
	ricky.SetCommand(pcommand2);
	ricky.Action();
	delete pcommand2;
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//客户原来的运行流程
	DoIt();

	//客户觉得麻烦了，每次改动都要找不同的组，谈不同的事
	//客户只想找一个人，告诉他要做什么就可以，不想关心由哪几个组来做和怎么做
	DoNew();

	return 0;
}
