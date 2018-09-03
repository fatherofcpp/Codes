#include "CloseingState.h"
#include "Context.h"
#include <iostream>
using std::cout;
using std::endl;

CCloseingState::CCloseingState(void)
{
}

CCloseingState::~CCloseingState(void)
{
}

void CCloseingState::Open()
{
	this->CLiftState::m_pContext->SetLiftState(CContext::pOpenningState);
	this->CLiftState::m_pContext->GetLiftState()->Open();
}

void CCloseingState::Close()
{
	cout << "µçÌÝÃÅ¹Ø±Õ..." << endl;
}

void CCloseingState::Run()
{
	this->CLiftState::m_pContext->SetLiftState(CContext::pRunningState);
	this->CLiftState::m_pContext->GetLiftState()->Run();
}

void CCloseingState::Stop()
{
	this->CLiftState::m_pContext->SetLiftState(CContext::pStoppingState);
	this->CLiftState::m_pContext->GetLiftState()->Stop();
}
