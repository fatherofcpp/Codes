#include "OpenningState.h"
#include "Context.h"
#include <iostream>
using std::cout;
using std::endl;

COpenningState::COpenningState(void)
{
}

COpenningState::~COpenningState(void)
{
}

void COpenningState::Open()
{
	cout << "µçÌÝÃÅ¿ªÆô..." << endl;
}

void COpenningState::Close()
{
	this->CLiftState::m_pContext->SetLiftState(CContext::pCloseingState);
	this->CLiftState::m_pContext->GetLiftState()->Close();
}

void COpenningState::Run()
{
	//do nothing
}

void COpenningState::Stop()
{
	//do nothing
}