#include "RunningState.h"
#include "Context.h"
#include <iostream>
using std::cout;
using std::endl;

CRunningState::CRunningState(void)
{
}

CRunningState::~CRunningState(void)
{
}

void CRunningState::Open()
{
	//do nothing
}
void CRunningState::Close()
{
	//do nothing
}

void CRunningState::Run()
{
	cout << "µçÌÝÉÏÏÂÅÜ..." << endl;
}

void CRunningState::Stop()
{
	this->CLiftState::m_pContext->SetLiftState(CContext::pStoppingState);
	this->CLiftState::m_pContext->GetLiftState()->Stop();
}