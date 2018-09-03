#pragma once
#include <iostream>
using std::string;
class ILetterProcess
{
public:
	ILetterProcess(void);
	virtual ~ILetterProcess(void);
	virtual void WriteContext(string context) = 0;//填写信的内容
	virtual void FillEnvelope(string address) = 0;//填写收件人地址及姓名
	virtual void LetterIntoEnvelope() = 0;//把信放到信封中
	virtual void SendLetter() = 0;//邮递信件
};
