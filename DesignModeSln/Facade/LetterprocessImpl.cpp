#include "StdAfx.h"
#include "LetterProcessImpl.h"
#include <iostream>
using std::string;
using std::cout;
using std::endl;
CLetterProcessImpl::CLetterProcessImpl(void)
{
}
CLetterProcessImpl::~CLetterProcessImpl(void)
{
}
void CLetterProcessImpl::WriteContext(string context)
{
	cout << "��д�ŵ�����... ..." << endl;
}
void CLetterProcessImpl::FillEnvelope(string address)
{
	cout << "��д�ռ��˵�ַ������... ..." << endl;
}
void CLetterProcessImpl::LetterIntoEnvelope()
{
	cout << "���ŷŵ��ŷ���..." << endl;
}
void CLetterProcessImpl::SendLetter()
{
	cout << "�ʵ��ż�..." << endl;
}