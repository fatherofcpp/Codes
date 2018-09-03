/*
main()，客户
ILetterProcess，接口
CLetterProcessImpl，信件处理的4个函数
CLetterPolice，警察
CModenPostOffice，邮局
说明：邮局对外只有一个窗口，接收信件内容和邮件地址。对内调用邮件处理的4个函数。
将复杂逻辑封装在邮局的里面，当需要增加警察来检查信件时，只需在邮局内增加警察检查信件的方法。
注意：将复杂逻辑封装起来，对外只有一个简单的接口。
*/

#include "stdafx.h"
#include "ILetterProcess.h"
#include "LetterProcessImpl.h"
#include "ModenPostOffice.h"
#include<iostream>
using std::string;
using std::cout;
using std::endl;
void DoItByPostOffice()
{
	CModenPostOffice modenPostOffice;
	string context = "Hello ... ...";
	string address = "Road No. 666, Beijing City, China";
	modenPostOffice.SendLetter(context, address);
}
void DoItYourself()
{
	ILetterProcess *pLetterProcess = new CLetterProcessImpl();
	pLetterProcess->WriteContext("Hello... ...");
	pLetterProcess->FillEnvelope("Road No. 666, Beijing City, China");
	pLetterProcess->LetterIntoEnvelope();
	pLetterProcess->SendLetter();
	delete pLetterProcess;
}
int _tmain(int argc, _TCHAR* argv[])
{
	//现在的调用方式。对于客户来说确实简单多了。
	//如需要增加逻辑，例如让警察来检查邮件。可以在邮局里完成这项工作。
	DoItByPostOffice();

	//原来的调用方式。
	DoItYourself();
	return 0;
}