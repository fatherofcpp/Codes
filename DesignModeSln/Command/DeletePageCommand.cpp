#include "StdAfx.h"
#include "DeletePageCommand.h"
CDeletePageCommand::CDeletePageCommand(void)
{
}
CDeletePageCommand::~CDeletePageCommand(void)
{
}
void CDeletePageCommand::Execute()
{
	//ִ������һ�����������
	this->ICommand::m_ppg->Find();

	//����һ������
	this->ICommand::m_ppg->Delete();

	//�����ƻ�
	this->ICommand::m_ppg->Plan();
}