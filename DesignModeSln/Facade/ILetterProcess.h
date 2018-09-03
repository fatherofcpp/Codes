#pragma once
#include <iostream>
using std::string;
class ILetterProcess
{
public:
	ILetterProcess(void);
	virtual ~ILetterProcess(void);
	virtual void WriteContext(string context) = 0;//��д�ŵ�����
	virtual void FillEnvelope(string address) = 0;//��д�ռ��˵�ַ������
	virtual void LetterIntoEnvelope() = 0;//���ŷŵ��ŷ���
	virtual void SendLetter() = 0;//�ʵ��ż�
};
