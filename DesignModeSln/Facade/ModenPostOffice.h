#pragma once
#include "ILetterProcess.h"
#include "LetterProcessImpl.h"
#include "LetterPolice.h"
#include <iostream>
using std::string;
class CModenPostOffice
{
public:
	CModenPostOffice(void);
	~CModenPostOffice(void);
	void SendLetter(string context, string address);
private:
	ILetterProcess *m_pLetterProcess;
	CLetterPolice *m_pLetterPolice;
};
