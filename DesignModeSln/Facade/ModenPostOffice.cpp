#include "StdAfx.h"
#include "ModenPostOffice.h"
CModenPostOffice::CModenPostOffice(void)
{
	this->m_pLetterProcess = new CLetterProcessImpl();
	this->m_pLetterPolice = new CLetterPolice();
}
CModenPostOffice::~CModenPostOffice(void)
{
	delete m_pLetterProcess;
	delete m_pLetterPolice;
}
void CModenPostOffice::SendLetter( string context, string address )
{
	//��æд��
	m_pLetterProcess->WriteContext(context);
	//д���ŷ�
	m_pLetterProcess->FillEnvelope(address);
	//����Ҫ����ż���
	m_pLetterPolice->CheckLetter(m_pLetterProcess);
	//���ŷŵ��ŷ���
	m_pLetterProcess->LetterIntoEnvelope();
	//�ʵ��ż�
	m_pLetterProcess->SendLetter();
}