#include "StdAfx.h"
#include "AddRequirementCommand.h"
CAddRequirementCommand::CAddRequirementCommand(void)
{
}
CAddRequirementCommand::~CAddRequirementCommand(void)
{
}
void CAddRequirementCommand::Execute()
{
	//ִ������һ�����������
	this->ICommand::m_prg->Find();

	//����һ������
	this->ICommand::m_prg->Add();

	//�����ƻ�
	this->ICommand::m_prg->Plan();
}