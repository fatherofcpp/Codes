/*
����ģʽ����һ�������װΪһ�����󣬴Ӷ�ʹ����ò�ͬ������Կͻ����в�������
�������Ŷӻ��¼������־���Լ�֧�ֿɳ����Ĳ�����Ӧ����һ���Ƚϼ򵥵�ģʽ�ˡ�

main()���ͻ� 
CInvoker����������ߣ�����Ŀ���� 
IGroup��ִ���߽ӿ� 
CRequirementGroup��ʵ��ִ����֮һ
CPageGroup��ʵ��ִ����֮��
CCodePage��ʵ��ִ����֮��
ICommand������ӿ�
CAddRequirementCommand��Execute������������CRequirementGroup�Ķ����������ִ���û����������
CDeletePageCommand��ͬ��
... ... �������
˵�����ͻ�ֻ��Ҫ֪����Invoker��������������������ǽ�����ֱ�Ӵ���������ִ���ߡ�
��Ȼ���ͻ�����Ҫ֪������ʲô����ġ� 
ע�⣺�ͻ�ֻ���������Ҫ֪����˭��ִ�к���ôִ�У����ֳ����ھ۵��ص㡣
�û��ڷ���������������صģ����Կ�������һ�����Undo ����Undo��״̬�ı����
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
	cout << "----------�ͻ�������һ������----------" << endl;
	IGroup *rg = new CRequirementGroup();
	rg->Find();
	rg->Add();
	rg->Plan();
	delete rg;
	cout << endl;

	cout << "----------�ͻ������޸�һ��ҳ��----------" << endl;
	IGroup *pg = new CPageGroup();
	pg->Find();
	pg->Add();
	pg->Plan();
	delete pg;
	cout << endl;

	cout << "----------�ͻ�����ɾ��һ������----------" << endl;
	IGroup *cg = new CCodeGroup();
	cg->Find();
	cg->Add();
	cg->Plan();
	delete cg;
	cout << endl;
}

void DoNew()
{
	cout << "----------�ͻ����÷��ˣ�ϣ��ֻ��һ���ˣ���������Ҫ��ʲô----------" << endl;
	cout << "----------�ͻ�Ҫ������һ������----------" << endl;
	CInvoker gary;
	ICommand *pcommand = new CAddRequirementCommand();
	gary.SetCommand(pcommand);
	gary.Action();
	delete pcommand;
	cout << endl;

	//�ͻ���Ҫ�Ķ�ֻ��Ҫ��CInvoker�Ϳ����ˡ�
	cout << "----------�ͻ�Ҫ��ɾ��һ��ҳ��----------" << endl;
	CInvoker ricky;
	ICommand *pcommand2 = new CDeletePageCommand();
	ricky.SetCommand(pcommand2);
	ricky.Action();
	delete pcommand2;
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//�ͻ�ԭ������������
	DoIt();

	//�ͻ������鷳�ˣ�ÿ�θĶ���Ҫ�Ҳ�ͬ���飬̸��ͬ����
	//�ͻ�ֻ����һ���ˣ�������Ҫ��ʲô�Ϳ��ԣ�����������ļ�������������ô��
	DoNew();

	return 0;
}
