/*
main()���ͻ�
ILetterProcess���ӿ�
CLetterProcessImpl���ż������4������
CLetterPolice������
CModenPostOffice���ʾ�
˵�����ʾֶ���ֻ��һ�����ڣ������ż����ݺ��ʼ���ַ�����ڵ����ʼ������4��������
�������߼���װ���ʾֵ����棬����Ҫ���Ӿ���������ż�ʱ��ֻ�����ʾ������Ӿ������ż��ķ�����
ע�⣺�������߼���װ����������ֻ��һ���򵥵Ľӿڡ�
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
	//���ڵĵ��÷�ʽ�����ڿͻ���˵ȷʵ�򵥶��ˡ�
	//����Ҫ�����߼��������þ���������ʼ����������ʾ�������������
	DoItByPostOffice();

	//ԭ���ĵ��÷�ʽ��
	DoItYourself();
	return 0;
}